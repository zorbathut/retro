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

// this file is hackish.

#pragma warning( disable : 4786 )

#define WIN32_LEAN_AND_MEAN
#include <d3d8.h>
#include <d3dx8core.h>
#include <process.h>
#include <assert.h>

#include "kinterface.h"
#include "kGrfxWritableOwnedraster.h"
#include "kGrfxWritable16bpp565.h"
#include "errlog.h"
#include "config.h"

class kInterfaceWin32 : public kInterface {
public:

	virtual grfx::kWritable *lockBuffer( grfx::kColor backwash );
	virtual void unlockBuffer( grfx::kWritable * );

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

	HWND hWnd;

	volatile bool closing;
	HANDLE finishedEvent;

	CRITICAL_SECTION surfaceLock;
	
	IDirect3DSurface8 *surf;
	IDirect3DSurface8 *writsurf;

	bool starting;

	D3DFORMAT bbfmt;

	grfx::kWritableOwnedraster *kwr;

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

		return new grfx::kWritableRaster( grfx::kRaster( sd.Width, sd.Height, static_cast< grfx::kColor * >( lockedrect.pBits ), lockedrect.Pitch / 4 ) ); }

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
		EXACTUINT32 *linesrc = reinterpret_cast< EXACTUINT32 * >( rast->getRaster().getData() );
		EXACTUINT32 *linedst = reinterpret_cast< EXACTUINT32 * >( lockedrect.pBits );

		for( int y = 0; y < sd.Height; y++ ) {
			EXACTUINT32 *src = linesrc;
			EXACTUINT32 *dst = linedst;
			int x = sd.Width;
			while( x > 0 ) {
				switch( x ) {
				case 1: {
					BYTE *minisrc = reinterpret_cast< BYTE * >( src );
					BYTE *minidst = reinterpret_cast< BYTE * >( dst );
					minidst[ 0 ] = minisrc[ 0 ];
					minidst[ 1 ] = minisrc[ 1 ];
					minidst[ 2 ] = minisrc[ 2 ];
					x--;
					break; }
				case 2: {
					dst[ 0 ] = ( src[ 0 ] & 0x00FFFFFF       ) | ( src[ 1 ] & 0x000000FF << 24 );
					BYTE *minisrc = reinterpret_cast< BYTE * >( src );
					BYTE *minidst = reinterpret_cast< BYTE * >( dst );
					minidst[ 4 ] = minisrc[ 5 ];
					minidst[ 5 ] = minisrc[ 6 ];
					x -= 2;
					break; }
				case 3: {
					dst[ 0 ] = ( src[ 0 ] & 0x00FFFFFF       ) | ( src[ 1 ] & 0x000000FF << 24 );
					dst[ 1 ] = ( src[ 1 ] & 0x00FFFF00 >>  8 ) | ( src[ 2 ] & 0x0000FFFF << 16 );
					BYTE *minisrc = reinterpret_cast< BYTE * >( src );
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
			linesrc += rast->getRaster().getPitch();
			linedst += lockedrect.Pitch / 4;
		}

		writsurf->UnlockRect();

		D3DDevice->CopyRects( writsurf, NULL, 0, surf, NULL );

		// Present the backbuffer contents to the display
		D3DDevice->Present( NULL, NULL, NULL, NULL );

		break; }

	case D3DFMT_X1R5G5B5: {

		grfx::kWritableRaster *rast = static_cast< grfx::kWritableRaster * >( ret );

		writsurf->LockRect( &lockedrect, NULL, D3DLOCK_NOSYSLOCK );
		// todo: errorchecking for pitch%4
		BYTE *linesrc = reinterpret_cast< BYTE * >( rast->getRaster().getData() );
		EXACTUINT32 *linedst = reinterpret_cast< EXACTUINT32 * >( lockedrect.pBits );

		for( int y = 0; y < sd.Height; y++ ) {
			BYTE *src = linesrc;
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
			linesrc += rast->getRaster().getPitch() * sizeof( grfx::kColor );
			linedst += lockedrect.Pitch / 4;
		}

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

			kwr = new grfx::kWritableOwnedraster( grfx::kRaster( sd.Width, sd.Height, new grfx::kColor[ sd.Width * sd.Height ], sd.Width ) );
		}

		D3DDevice->CreateImageSurface( sd.Width, sd.Height, sd.Format, &writsurf );

		LeaveCriticalSection( &surfaceLock );

	}

};

kInterfaceWin32 *inter = NULL;

kInterfaceWin32::kInterfaceWin32( void ) : D3D( NULL ), D3DDevice( NULL ), closing( false ),
		finishedEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) ), surf( NULL ), writsurf( NULL ),
		starting( false ), kwr( NULL ) {
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
			sprintf( buffer, "Using frontbuffer %d, backbuffer %d, HAL", d3ddm.Format, bbfmt );
			MessageBox( NULL, buffer, "Info", NULL );
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
				sprintf( buffer, "Using frontbuffer %d, backbuffer %d, REF", d3ddm.Format, bbfmt );
				MessageBox( NULL, buffer, "Info", NULL );
				hal = false;
				break;
			}
			testitr++;
		}
		if( bbfmt == D3DFMT_UNKNOWN ) {
			char buffer[ 80 ];
			sprintf( buffer, "Couldn't find usable backbuffer format for %d", d3ddm.Format );
			MessageBox( NULL, buffer, "Critical error", NULL );
			int i;
			for( i = 0; i < 100; i++ ) {
				if( !FAILED( D3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, (D3DFORMAT)i, TRUE ) ) ) {
					char buffer[ 80 ];
					sprintf( buffer, "Found frontbuffer %d, backbuffer %d, HAL", d3ddm.Format, i );
					MessageBox( NULL, buffer, "Info", NULL );
				}
				testitr++;
			}
			for( i = 0; i < 100; i++ ) {
				if( !FAILED( D3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, d3ddm.Format, (D3DFORMAT)i, TRUE ) ) ) {
					char buffer[ 80 ];
					sprintf( buffer, "Found frontbuffer %d, backbuffer %d, REF", d3ddm.Format, i );
					MessageBox( NULL, buffer, "Info", NULL );
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

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
 
	HRESULT bort;
    if( FAILED( bort = D3D->CreateDevice( D3DADAPTER_DEFAULT, hal ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF, hWnd,
                                      D3DCREATE_MIXED_VERTEXPROCESSING,
                                      &d3dpp, &D3DDevice ) ) ) {
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
		return true;
	}

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
    UnregisterClass( "D3D Tutorial", GetModuleHandle( NULL ) );

};

void __cdecl mainThread( void *parameter ) {

	g_errlog.open( "errlog.txt" );
	g_errlog.setf( std::ios::unitbuf );	// whee, autoflush!

	gameMain( inter );

	inter->shutDownNow();

	delete g_errlog;

}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int ) {

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