/* Copyright (c) 2001, Ben Wilhelm
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met: 

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer. 

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 
   3. Neither the name of Pavlovian Productions nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
   OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE. */

// this file is one colossal hack.

#pragma warning( disable : 4786 )
#pragma warning( disable : 4800 )	// bool performance warning

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d8.h>
#include <d3dx8core.h>
#include <dinput.h>
#include <process.h>
#include <assert.h>
#include <vector>

#include "kinterface.h"
#include "kGrfxWritableRaster.h"
#include "kGrfxWritable16bpp565.h"
#include "errlog.h"
#include "config.h"
#include "kControlsOpen.h"
#include "dx8util.h"
#include "hididConsts.h"
#include "win32_hidid.h"
// There's nothing slow about this at all.

HWND hWnd; // oh bugger off :P
const int bufferarray = 16; // size of the buffered data

class kInputDevice : private boost::noncopyable {
public:

	int getButtoncount() const;
	int getAxiscount() const;

	void getLabels( control::kObjectspecs **butts, control::kObjectspecs **axes );

	void getData( BYTE **buttons, INT32 **axes );	// increments pointers, too.

	kInputDevice( IDirectInputDevice8 *dev );

private:

	int butcount;
	int axicount;

	int axioffs;
	int fullsize;

	IDirectInputDevice8 *dev;

	control::kDevicespecs devspecs;

	bool polling;
	bool acquired;

};

class kInterfaceWin32 : public kInterface {
public:

	virtual grfx::kWritable *lockBuffer( grfx::kColor backwash );
	virtual void unlockBuffer( grfx::kWritable * );

	virtual const kControls *updateControls();

	virtual bool shutDown( void ) const { return closing; };	// should I quit?
	virtual void shutDownNow( void ) { SetEvent( finishedEvent ); PostQuitMessage( 0 ); };

	kInterfaceWin32( void );
	virtual ~kInterfaceWin32( void );

	bool init( void );
	void close( void );

	virtual UINT64 getCounterFreq() const;
	virtual UINT64 getCounterPos() const;

	void closeGamethread( void ) {
		closing = true;
		WaitForSingleObject( finishedEvent, INFINITE ); }

	void resetGraphics( void );

	IDirect3D8             *D3D;
	IDirect3DDevice8       *D3DDevice;

	volatile bool closing;
	HANDLE finishedEvent;

	CRITICAL_SECTION surfaceLock;
	
	IDirect3DSurface8 *surf;
	IDirect3DSurface8 *writsurf;

	IDirectInput8		*dinput;

	bool starting;

	D3DFORMAT bbfmt;

	grfx::kWritableRaster *kwr;

	kControlsOpen controls;

	std::vector< kInputDevice * > inputs;

};

grfx::kWritable *kInterfaceWin32::lockBuffer( grfx::kColor backwash ) {

	D3DLOCKED_RECT    lockedrect;
	D3DSURFACE_DESC	  sd;

	EnterCriticalSection( &surfaceLock );

	surf->GetDesc( &sd );


	switch( bbfmt ) {

	case D3DFMT_X8R8G8B8: {

		if( backwash.split.a == 0x00 )
			D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, backwash.argb, 1.0f, 0 );

		// Begin the scene
		writsurf->LockRect( &lockedrect, NULL, D3DLOCK_NOSYSLOCK );
		
		// todo: error on pitch % 4
		assert( ( lockedrect.Pitch % 4 ) == 0 );

		return new grfx::kWritableRaster( kPoint< INT32 >( sd.Width, sd.Height ), static_cast< grfx::kColor * >( lockedrect.pBits ), lockedrect.Pitch / 4, false ); }

	case D3DFMT_R5G6B5: {

		if( backwash.split.a == 0x00 )
			D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, backwash.argb, 1.0f, 0 );

		// Begin the scene
		writsurf->LockRect( &lockedrect, NULL, D3DLOCK_NOSYSLOCK );
		
		// todo: error on pitch % 2
		assert( ( lockedrect.Pitch % 2 ) == 0 );

		return new grfx::kWritable16bpp565( sd.Width, sd.Height, reinterpret_cast< EXACTUINT16 * >( lockedrect.pBits ), lockedrect.Pitch / 2 ); }

	case D3DFMT_R8G8B8:
	case D3DFMT_X1R5G5B5: {
	// todo: special kWritables that render directly to this bitdepth

		if( backwash.split.a == 0x00 )
			kwr->clear( backwash );

		return kwr; }

	default: {

		MessageBox( NULL, "Unexpected back buffer format", "Critical Error", NULL );

		PostQuitMessage( 1 );

		return kwr; }

	}

}

void kInterfaceWin32::unlockBuffer( grfx::kWritable *ret ) {

	D3DLOCKED_RECT    lockedrect;
	D3DSURFACE_DESC	  sd;

	surf->GetDesc( &sd );

	//	// well . . . yes.

	switch( bbfmt ) {

	case D3DFMT_X8R8G8B8:
	case D3DFMT_R5G6B5: {

		writsurf->UnlockRect();

		D3DDevice->CopyRects( writsurf, NULL, 0, surf, NULL );

		// Present the backbuffer contents to the display
		D3DDevice->Present( NULL, NULL, NULL, NULL );

		delete ret;

		break; }

	case D3DFMT_R8G8B8: {

		grfx::kWritableRaster *rast = static_cast< grfx::kWritableRaster * >( ret );

		writsurf->LockRect( &lockedrect, NULL, D3DLOCK_NOSYSLOCK );
		// todo: errorchecking for pitch%4
		grfx::kLockedRead lock;
		rast->getRaster()->lock( kRect< INT32 >( 0, 0, rast->getDimensions().x, rast->getDimensions().y ), &lock );
		const EXACTUINT32 *linesrc = reinterpret_cast< const EXACTUINT32 * >( lock.data );
		EXACTUINT32 *linedst = reinterpret_cast< EXACTUINT32 * >( lockedrect.pBits );

		for( int y = 0; y < sd.Height; y++ ) {
			const EXACTUINT32 *src = linesrc;
			EXACTUINT32 *dst = linedst;
			int x = sd.Width;
			while( x > 0 ) {
				switch( x ) {
				case 1: {
					const BYTE *minisrc = reinterpret_cast< const BYTE * >( src );
					BYTE *minidst = reinterpret_cast< BYTE * >( dst );
					minidst[ 0 ] = minisrc[ 0 ];
					minidst[ 1 ] = minisrc[ 1 ];
					minidst[ 2 ] = minisrc[ 2 ];
					x--;
					break; }
				case 2: {
					dst[ 0 ] = ( src[ 0 ] & 0x00FFFFFF       ) | ( src[ 1 ] & 0x000000FF << 24 );
					const BYTE *minisrc = reinterpret_cast< const BYTE * >( src );
					BYTE *minidst = reinterpret_cast< BYTE * >( dst );
					minidst[ 4 ] = minisrc[ 5 ];
					minidst[ 5 ] = minisrc[ 6 ];
					x -= 2;
					break; }
				case 3: {
					dst[ 0 ] = ( src[ 0 ] & 0x00FFFFFF       ) | ( src[ 1 ] & 0x000000FF << 24 );
					dst[ 1 ] = ( src[ 1 ] & 0x00FFFF00 >>  8 ) | ( src[ 2 ] & 0x0000FFFF << 16 );
					const BYTE *minisrc = reinterpret_cast< const BYTE * >( src );
					BYTE *minidst = reinterpret_cast< BYTE * >( dst );
					minidst[ 8 ] = minisrc[ 10 ];
					x -= 3;
					break; }
				default: {
					dst[ 0 ] = ( src[ 0 ] & 0x00FFFFFF       ) | ( src[ 1 ] & 0x000000FF << 24 );
					dst[ 1 ] = ( src[ 1 ] & 0x00FFFF00 >>  8 ) | ( src[ 2 ] & 0x0000FFFF << 16 );
					dst[ 2 ] = ( src[ 2 ] & 0x00FF0000 >> 16 ) | ( src[ 3 ] & 0x00FFFFFF <<  8 );
					x -= 4;
					src += 4;
					dst += 3;
					break; }
				}
			}
			linesrc += lock.pitch;
			linedst += lockedrect.Pitch / 4;
		}

		rast->getRaster()->unlock( &lock );

		writsurf->UnlockRect();

		D3DDevice->CopyRects( writsurf, NULL, 0, surf, NULL );

		// Present the backbuffer contents to the display
		D3DDevice->Present( NULL, NULL, NULL, NULL );

		break; }

	case D3DFMT_X1R5G5B5: {

		grfx::kWritableRaster *rast = static_cast< grfx::kWritableRaster * >( ret );

		writsurf->LockRect( &lockedrect, NULL, D3DLOCK_NOSYSLOCK );
		// todo: errorchecking for pitch%4
		grfx::kLockedRead lock;
		rast->getRaster()->lock( kRect< INT32 >( 0, 0, rast->getDimensions().x, rast->getDimensions().y ), &lock );
		const BYTE *linesrc = reinterpret_cast< const BYTE * >( lock.data );
		EXACTUINT32 *linedst = reinterpret_cast< EXACTUINT32 * >( lockedrect.pBits );

		for( int y = 0; y < sd.Height; y++ ) {
			const BYTE *src = linesrc;
			EXACTUINT32 *dst = linedst;
			int x = sd.Width;
			while( x > 0 ) {
				switch( x ) {
				case 1: {
					EXACTUINT16 *minidst = reinterpret_cast< EXACTUINT16 * >( dst );
					*minidst = ( src[ 0 ] >> 3 ) | ( ( src[ 1 ] & 0xF8 ) << 2 ) | ( ( src[ 2 ] & 0xF8 ) << 7 );
					x--;
					break; }
				default: {
					*dst = (   src[ 0 ]          >>  3 ) | ( ( src[ 1 ] & 0xF8 ) <<  2 ) | ( ( src[ 2 ] & 0xF8 ) <<  7 ) |
						   ( ( src[ 4 ] & 0xF8 ) << 13 ) | ( ( src[ 5 ] & 0xF8 ) << 18 ) | ( ( src[ 6 ] & 0xF8 ) << 23 );
					x -= 2;
					src += 8;
					dst++;
					break; }
					// todo: unroll a little.
				}
			}
			linesrc += lock.pitch * sizeof( grfx::kColor );
			linedst += lockedrect.Pitch / 4;
		}

		rast->getRaster()->unlock( &lock );

		writsurf->UnlockRect();

		D3DDevice->CopyRects( writsurf, NULL, 0, surf, NULL );

		// Present the backbuffer contents to the display
		D3DDevice->Present( NULL, NULL, NULL, NULL );

		break; }

	default: {

		MessageBox( NULL, "Unexpected back buffer format", "Critical Error", NULL );

		PostQuitMessage( 1 );

		break; }

	}

	LeaveCriticalSection( &surfaceLock );

}

void kInterfaceWin32::resetGraphics( void ) {

	if( !starting ) {

		D3DPRESENT_PARAMETERS parms;

		D3DDISPLAYMODE dev;
		D3DSURFACE_DESC	  sd;

		D3DDevice->GetDisplayMode( &dev );

		ZeroMemory( &parms, sizeof( parms ) );
		parms.BackBufferFormat = dev.Format;
		parms.SwapEffect = D3DSWAPEFFECT_DISCARD;
		parms.Windowed = TRUE;

		EnterCriticalSection( &surfaceLock );

		if( surf )
			surf->Release();
		if( writsurf )
			writsurf->Release();

		D3DDevice->Reset( &parms );

		D3DDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &surf );
			
		surf->GetDesc( &sd );

		if( bbfmt != D3DFMT_X8R8G8B8 ) {
			delete kwr;

			kwr = new grfx::kWritableRaster( kPoint< INT32 >( sd.Width, sd.Height ) );
		}

		D3DDevice->CreateImageSurface( sd.Width, sd.Height, sd.Format, &writsurf );

		LeaveCriticalSection( &surfaceLock );

	}

};

kInterfaceWin32 *inter = NULL;

kInterfaceWin32::kInterfaceWin32( void ) : D3D( NULL ), D3DDevice( NULL ), closing( false ),
		finishedEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) ), surf( NULL ), writsurf( NULL ),
		starting( false ), kwr( NULL ), controls( 256, 0 ) {
	InitializeCriticalSection( &surfaceLock );
};
kInterfaceWin32::~kInterfaceWin32( void ) {
	DeleteCriticalSection( &surfaceLock );
	delete kwr;
};

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;

		case WM_SIZE:
			inter->resetGraphics();
			return 0;

		case WM_GETMINMAXINFO: {
			MINMAXINFO *memi = (MINMAXINFO *)lParam;
			memi->ptMinTrackSize.x = config_xmin + GetSystemMetrics( SM_CXSIZEFRAME ) * 2;
			memi->ptMinTrackSize.y = config_ymin + GetSystemMetrics( SM_CYSIZEFRAME ) * 2 + GetSystemMetrics( SM_CYMENU );
			return 0; }

        case WM_PAINT:
            ValidateRect( hWnd, NULL );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

BOOL CALLBACK devenumcallback( const DIDEVICEINSTANCE *lpddi, void *pvRef ) {
	std::pair< IDirectInput8 *, std::vector< kInputDevice * > * > *inputs = reinterpret_cast< std::pair< IDirectInput8 *, std::vector< kInputDevice * > * > * >( pvRef );

	if( strcmp( lpddi->tszInstanceName, "USB Mouse" ) ) {
		// TODO: work on this :/

		IDirectInputDevice8 *leapdi;

		inputs->first->CreateDevice( lpddi->guidInstance, &leapdi, NULL );
		inputs->second->push_back( new kInputDevice( leapdi ) );

	};

	return DIENUM_CONTINUE;
};

bool kInterfaceWin32::init( void ) {

	starting = true;

    // Initialize Direct3D
    if( NULL == ( D3D = Direct3DCreate8( D3D_SDK_VERSION ) ) ) {
		MessageBox( NULL, "Error initializing Direct3D8", "Critical Error", NULL );
        return true;
	}

    // Get the current desktop display mode
    D3DDISPLAYMODE d3ddm;
    if( FAILED( D3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) ) {
		MessageBox( NULL, "Couldn't get display mode information", "Critical Error", NULL );
		D3D->Release();
        return true;
	}

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.

	bbfmt = D3DFMT_UNKNOWN;
	const D3DFORMAT tests[] = { D3DFMT_X8R8G8B8, D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5,
		/*D3DFMT_P8,*/ D3DFMT_UNKNOWN };
	const D3DFORMAT *testitr = tests;
	bool hal = false;

	while( *testitr != D3DFMT_UNKNOWN ) {
		if( !FAILED( D3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, *testitr, TRUE ) ) ) {
			bbfmt = *testitr;
			char buffer[ 80 ];
			sprintf( buffer, "WIN32: Using frontbuffer %d, backbuffer %d, HAL", d3ddm.Format, bbfmt );
			//MessageBox( NULL, buffer, "Info", NULL );
			g_errlog << buffer << std::endl;
			hal = true;
			break;
		}
		testitr++;
	}

	if( bbfmt == D3DFMT_UNKNOWN ) {
		testitr = tests;
		while( *testitr != D3DFMT_UNKNOWN ) {
			if( !FAILED( D3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, d3ddm.Format, *testitr, TRUE ) ) ) {
				bbfmt = *testitr;
				char buffer[ 80 ];
				sprintf( buffer, "WIN32: Using frontbuffer %d, backbuffer %d, REF", d3ddm.Format, bbfmt );
				//MessageBox( NULL, buffer, "Info", NULL );
				g_errlog << buffer << std::endl;
				hal = false;
				break;
			}
			testitr++;
		}
		if( bbfmt == D3DFMT_UNKNOWN ) {
			char buffer[ 80 ];
			sprintf( buffer, "WIN32: Couldn't find usable backbuffer format for %d", d3ddm.Format );
			//MessageBox( NULL, buffer, "Info", NULL );
			g_errlog << buffer << std::endl;
			MessageBox( NULL, "Couldn't find usable screen mode", "Critical error", NULL );
			int i;
			for( i = 0; i < 100; i++ ) {
				if( !FAILED( D3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, (D3DFORMAT)i, TRUE ) ) ) {
					char buffer[ 80 ];
					sprintf( buffer, "WIN32: Found frontbuffer %d, backbuffer %d, HAL", d3ddm.Format, i );
					//MessageBox( NULL, buffer, "Info", NULL );
					g_errlog << buffer << std::endl;
				}
				testitr++;
			}
			for( i = 0; i < 100; i++ ) {
				if( !FAILED( D3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, d3ddm.Format, (D3DFORMAT)i, TRUE ) ) ) {
					char buffer[ 80 ];
					sprintf( buffer, "WIN32: Found frontbuffer %d, backbuffer %d, REF", d3ddm.Format, i );
					//MessageBox( NULL, buffer, "Info", NULL );
					g_errlog << buffer << std::endl;
				}
				testitr++;
			}
			D3D->Release();
			return true;
		}
	}

    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = bbfmt;

	// Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
                      "Timespace", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    hWnd = CreateWindow( "Timespace", "Timespace",
                              WS_OVERLAPPEDWINDOW, 0, 0, config_xmin + GetSystemMetrics( SM_CXSIZEFRAME ) * 2,
							  config_ymin + GetSystemMetrics( SM_CYSIZEFRAME ) * 2 + GetSystemMetrics( SM_CYMENU ),
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

    // Create the Direct3D device. Here we are using the default adapter and
	// choosing the HAL we figured out above.
 
	HRESULT bort;
	bort = D3D->CreateDevice( D3DADAPTER_DEFAULT, hal ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &D3DDevice );
    if( FAILED( bort ) ) {
		switch( bort ) {  
		case D3DERR_INVALIDCALL  :
			MessageBox( NULL, "Unexpected error (invalid call) initializing graphics card", "Critical error", NULL );
			break;

		case D3DERR_OUTOFVIDEOMEMORY  :
			MessageBox( NULL, "Out-of-memory error initializing graphics card", "Critical error", NULL );
			break;

		case D3DERR_NOTAVAILABLE  :
			MessageBox( NULL, "Unexpected error (not available) initializing graphics card", "Critical error", NULL );
			break;

		default: {
			char foo[ 256 + 1 ];
			D3DXGetErrorString( bort, foo, 256 );
			char bar[ 512 ];
			sprintf( bar, "Unexpected error (0x%x, %s) initializing graphics card", bort, foo );
			MessageBox( NULL, bar, "Critical error", NULL );
			break; }
		}

		D3D->Release();
		return true;		// TODO: make errors make entries in the errlog
	}

	HRESULT hr; 
 
    // Create the DirectInput object. 
    hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, 
                            IID_IDirectInput8, ( void** )&dinput, NULL); 
 
    if( FAILED(hr) ) {
		D3DDevice->Release();
		D3D->Release();
		MessageBox( NULL, "Error creating DirectInput8 object", "Critical error", NULL );
		return true;
	};

	std::pair< IDirectInput8 *, std::vector< kInputDevice * > * > det( dinput, &inputs );

	dinput->EnumDevices( DI8DEVCLASS_ALL, &devenumcallback, &det, DIEDFL_ATTACHEDONLY );

	int buttoncount = 0;
	int axiscount = 0;

	std::vector< kInputDevice * >::iterator itr;
 	for( itr = inputs.begin(); itr != inputs.end(); ++itr ) {
		buttoncount += (*itr)->getButtoncount();
		axiscount += (*itr)->getAxiscount(); };

	controls.setButtoncount( buttoncount );
	controls.setAxiscount( axiscount );

	control::kObjectspecs *buttoninfos = controls.accessButtoninfo();
	control::kObjectspecs *axisinfos = controls.accessAxisinfo();

	for( itr = inputs.begin(); itr != inputs.end(); ++itr ) {
		(*itr)->getLabels( &buttoninfos, &axisinfos ); };

	controls.rebuildFindLists();

	starting = false;

    // Device state would normally be set here
    ShowWindow( hWnd, SW_SHOWDEFAULT );

	resetGraphics();

    return false;

}

void kInterfaceWin32::close( void ) {

	surf->Release();
	writsurf->Release();
    D3DDevice->Release();
    D3D->Release();
	for( std::vector< kInputDevice * >::iterator itr = inputs.begin(); itr != inputs.end(); ++itr )
		delete *itr;
	dinput->Release();

    UnregisterClass( "D3D Tutorial", GetModuleHandle( NULL ) );

};

void __cdecl mainThread( void *parameter ) {

	gameMain( inter );

	inter->shutDownNow();

}

#include <direct.h>

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int ) {

	_chdir( "c:\\win98se\\desktop\\werk\\sea\\zem\\datadog" );

	g_errlog.open( "errlog.txt" );
	g_errlog.setf( std::ios::unitbuf );	// whee, autoflush!
	// todo: THREADSAFETY! or just be careful not to use it while the main thread is open.

	assert( sizeof( grfx::kColor ) == 4 );	// good god I hope so . . .
	
	inter = new kInterfaceWin32();

	if( inter->init() )
		return 1;

	_beginthread( mainThread, 0, NULL );

    // Enter the message loop
    MSG msg; 
	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	inter->closeGamethread();

	inter->close();

	delete inter;

	g_errlog.close();

	return 0;

}

UINT64 kInterfaceWin32::getCounterFreq() const {
	LARGE_INTEGER foo;
	QueryPerformanceFrequency( &foo );
	return foo.QuadPart; }

UINT64 kInterfaceWin32::getCounterPos() const {
	LARGE_INTEGER foo;
	QueryPerformanceCounter( &foo );
	return foo.QuadPart; }

const kControls *kInterfaceWin32::updateControls() {

	BYTE *button = controls.accessButtons();
	INT32 *axis  = controls.accessAxes();

	for( std::vector< kInputDevice * >::iterator itr = inputs.begin(); itr != inputs.end(); ++itr )
		(*itr)->getData( &button, &axis );

	return &controls;

};

int kInputDevice::getButtoncount() const {
	return butcount; };

int kInputDevice::getAxiscount() const {
	return axicount; };

BOOL CALLBACK ncback( const DIDEVICEOBJECTINSTANCE *lpddoi, void *pvRef ) {

	std::pair< std::pair< control::kObjectspecs **, control::kObjectspecs ** >, control::kDevicespecs * > *det = (std::pair< std::pair< control::kObjectspecs **, control::kObjectspecs ** >, control::kDevicespecs * > *)pvRef;

	if( lpddoi->dwType & DIDFT_BUTTON ) {
		unsigned short uid = DIDFT_GETINSTANCE( lpddoi->dwType );
		(*det->first.first)->name = lpddoi->tszName;
		(*det->first.first)->hidid = kHidid( lpddoi->wUsagePage, lpddoi->wUsage );
		(*det->first.first)->uid = kUniqueKeyId( (const char *)&uid );
		(*det->first.first)->dev = det->second;
		if( !(*det->first.first)->hidid.valid() ) {
			(*det->first.first)->hidid = getHidid( det->second->name.c_str(), lpddoi->tszName );
			g_errlog << "CONTROL: Found HIDID for button \"" << det->second->name << "," << lpddoi->tszName << "\" (" << (*det->first.first)->hidid.getPage() << ", " << (*det->first.first)->hidid.getItem() << ")" << std::endl;
		} else {
			g_errlog << "CONTROL: Provided HIDID for button \"" << det->second->name << "," << lpddoi->tszName << "\" (" << (*det->first.first)->hidid.getPage() << ", " << (*det->first.first)->hidid.getItem() << ")" << std::endl;
		}
		(*det->first.first)++;
	};

	if( lpddoi->dwType & DIDFT_AXIS ) {
		unsigned short uid = DIDFT_GETINSTANCE( lpddoi->dwType );
		(*det->first.second)->name = lpddoi->tszName;
		(*det->first.second)->hidid = kHidid( lpddoi->wUsagePage, lpddoi->wUsage );
		(*det->first.second)->uid = kUniqueKeyId( (const char *)&uid );
		(*det->first.second)->dev = det->second;
		if( !(*det->first.second)->hidid.valid() ) {
			(*det->first.second)->hidid = getHidid( det->second->name.c_str(), lpddoi->tszName );
			g_errlog << "CONTROL: Found HIDID for axis \"" << det->second->name << "," << lpddoi->tszName << "\" (" << (*det->first.second)->hidid.getPage() << ", " << (*det->first.second)->hidid.getItem() << ")" << std::endl;
		} else {
			g_errlog << "CONTROL: Provided HIDID for axis \"" << det->second->name << "," << lpddoi->tszName << "\" (" << (*det->first.second)->hidid.getPage() << ", " << (*det->first.second)->hidid.getItem() << ")" << std::endl;
		}
		(*det->first.second)++;
	};

	return DIENUM_CONTINUE;

};

void kInputDevice::getLabels( control::kObjectspecs **butts, control::kObjectspecs **axes ) {
	
	std::pair< std::pair< control::kObjectspecs **, control::kObjectspecs ** >, control::kDevicespecs * > det;
	det.first.first = butts;
	det.first.second = axes;
	det.second = &devspecs;

	dev->EnumObjects( &ncback, &det, DIDFT_BUTTON | DIDFT_AXIS );

};

BOOL CALLBACK objcback( const DIDEVICEOBJECTINSTANCE *lpddoi, void *pvRef ) {

	std::pair< std::vector< DWORD > *, std::vector< DWORD > * > *dort = (std::pair< std::vector< DWORD > *, std::vector< DWORD > * > *)pvRef;

	if( lpddoi->dwType & DIDFT_AXIS )
		dort->second->push_back( lpddoi->dwType ); // hope this works . . .

	if( lpddoi->dwType & DIDFT_BUTTON )
		dort->first->push_back( lpddoi->dwType );

	return DIENUM_CONTINUE;

};

kInputDevice::kInputDevice( IDirectInputDevice8 *in_dev ) {

	std::vector< DWORD > buttons;
	std::vector< DWORD > axes;
	std::pair< std::vector< DWORD > *, std::vector< DWORD > * > det( &buttons, &axes );

	bool relative;

	acquired = false;

	dev = in_dev;

	{
		DIDEVICEINSTANCE ddi;
		ddi.dwSize = sizeof( DIDEVICEINSTANCE );

		dev->GetDeviceInfo( &ddi );

		char boof[ MAX_PATH * 2 + 10 ];
		sprintf( boof, "%s", ddi.tszInstanceName );
		devspecs.name = boof;
		devspecs.uid = kUniqueDevId( (const char *)&ddi.guidInstance );

		if( ddi.dwDevType & 0xff == DI8DEVTYPE_MOUSE )
			relative = true;
		  else
			relative = false;
		// TODO: store both values.

		DIDEVCAPS ddc;
		ddc.dwSize = sizeof( DIDEVCAPS );

		dev->GetCapabilities( &ddc );

		polling = ddc.dwFlags & DIDC_POLLEDDEVICE;


	}		// moved it, left it in brackets so it was obvious

	dev->EnumObjects( &objcback, &det, DIDFT_BUTTON | DIDFT_AXIS );

	butcount = buttons.size();
	axicount = axes.size();
	axioffs = butcount;
	if( axioffs % 4 ) {
		axioffs &= ~0x3;
		axioffs += 4;
	};
	fullsize = axioffs + axicount * 4;

	DIDATAFORMAT ddf;
	ddf.dwSize = sizeof( DIDATAFORMAT );
	ddf.dwObjSize = sizeof( DIOBJECTDATAFORMAT );
	ddf.dwFlags = relative ? DIDF_RELAXIS : DIDF_ABSAXIS;
	ddf.dwDataSize = fullsize;
	ddf.dwNumObjs = butcount + axicount;

	DIOBJECTDATAFORMAT *objs = new DIOBJECTDATAFORMAT[ butcount + axicount ];
	ddf.rgodf = objs;
	int offs = 0;

	DIOBJECTDATAFORMAT *deez = objs;
	std::vector< DWORD >::iterator itr = buttons.begin();
	int i;
	for( i = 0; i < butcount; i++ ) {
		//deez->pguid = &*itr; // please don't ask.
		deez->pguid = NULL;
		deez->dwOfs = offs;
		deez->dwType = *itr;
		deez->dwFlags = NULL;

		offs++;
		itr++;
		deez++;
	};

	offs = axioffs;
	itr = axes.begin();

	for( i = 0; i < axicount; i++ ) {
		//deez->pguid = &*itr; // please don't ask.
		deez->pguid = NULL;
		deez->dwOfs = offs;
		deez->dwType = *itr;
		deez->dwFlags = NULL;

		offs += 4;
		itr++;
		deez++;
	};

    HRESULT hr = dev->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ); 
	g_errlog << "WIN32: Mode set for " << devspecs.name << " (" << dInput8Stream( hr ) << ")" << std::endl;

	DIPROPDWORD prop;
	prop.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	prop.diph.dwSize = sizeof( DIPROPDWORD );
	prop.diph.dwHow = DIPH_DEVICE;
	prop.diph.dwObj = 0;
	prop.dwData = bufferarray;

	hr = dev->SetProperty( DIPROP_BUFFERSIZE, &prop.diph );
	g_errlog << "WIN32: Buffered-property set for " << devspecs.name << " (" << dInput8Stream( hr ) << ")" << std::endl;

	hr = dev->SetDataFormat( &ddf );
	g_errlog << "WIN32: Format set for " << devspecs.name << " (" << dInput8Stream( hr ) << ")" << std::endl;
	// todo: error checking 'n stuff.

	delete [] ddf.rgodf;

};

void kInputDevice::getData( BYTE **buttons, INT32 **axes ) {

    char     *buffer; 
	buffer = new char[ fullsize ];
    HRESULT  hr;

	if( polling )
		dev->Poll(); // todo: errorcheck here?
    hr = dev->GetDeviceState( fullsize, buffer ); 
    if( FAILED( hr ) ) { 
		if( acquired ) {
			g_errlog << "WIN32: " << devspecs.name << " lost (error " << dInput8Stream( hr ) << ")" << std::endl;
			acquired = false;
		}
		if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
			hr = dev->Acquire();
		    hr = dev->GetDeviceState( fullsize, buffer );
			if( FAILED( hr ) ) {
				delete [] buffer;
				memset( *buttons, keyis::up, butcount );
				memset( *axes, 0, axicount * 4 );
				*buttons += butcount;
				*axes += axicount;
				return;
			} else {
				g_errlog << "WIN32: " << devspecs.name << " acquired" << std::endl;
				acquired = true;
			}
		} else {
			delete [] buffer;
			memset( *buttons, keyis::up, butcount );
			memset( *axes, 0, axicount * 4 );
			*buttons += butcount;
			*axes += axicount;
			return;			// yes, it's a dupe. Go get yourself a big serving of shut-up.
		}
    }

	BYTE *butorig = *buttons;

	char *btemp = buffer;
	int i;
	for( i = 0; i < butcount; i++ )
		*((*buttons)++) = ( *(btemp++) & 0x80 ) ? keyis::down : keyis::up;
	// todo: the push/release/change stuff.

	INT32 *atemp = reinterpret_cast< INT32 * >( buffer + axioffs );

	for( i = 0; i < axicount; i++ )
		*((*axes)++) = *(atemp++);

	delete [] buffer;

	DIDEVICEOBJECTDATA buff[ bufferarray ];
	DWORD datsize = bufferarray;
	hr = dev->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), buff, &datsize, 0 );
	if( FAILED( hr ) ) {
		g_errlog << "WIN32: Data snag failed " << devspecs.name << " (error " << dInput8Stream( hr ) << ")" << std::endl;
		// shazbot. I give up.
		return;
	}

	for( i = 0; i < datsize; i++ ) {
		butorig[ buff[ i ].dwOfs ] |= ( buff[ i ].dwData & 0x80 ? ( keyis::push | keyis::change ) : ( keyis::release | keyis::change ) );
	};	// shut up :P

};