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

#include "kWritableD3D.h"

#include "minmax.h"
#include "errlog.h"
#include "dx8util.h"
#include "kGrfxUtil.h"

// A structure for our custom vertex type
struct CUSTOMVERTEX {
    float x, y, z, rhw;
    float u, v;
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0))


// information functions
const kPoint< INT32 > &kWritableD3D::getDimensions( void ) const {
	return curdim; }

// drawing functions
void kWritableD3D::drawRaster( const grfx::kRasterConst *rstr, const kPoint< INT32 > &loc ) {
	drawRasterPart( rstr, loc, kPoint< INT32 >( 0, 0 ), rstr->getDimensions() );
};
void kWritableD3D::drawRasterPart( const grfx::kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

	kWritableD3DTexture *texture = getTex( rstr );

	for( int i = 0; i < texture->length.size(); i++ ) {
		for( int j = 0; j < texture->length[ i ].width.size(); j++ ) {
			kPoint< INT32 > nloc( loc );
			kPoint< INT32 > nstart( start );
			kPoint< INT32 > nend( end );
			nloc.x += zutil::zmax( nstart.x, texture->length[ i ].width[ j ].start ) - nstart.x;
			nloc.y += zutil::zmax( nstart.y, texture->length[ i ].start ) - nstart.y;
			nstart.x = zutil::zmax( nstart.x, texture->length[ i ].width[ j ].start );
			nstart.y = zutil::zmax( nstart.y, texture->length[ i ].start );
			nend.x = zutil::zmin( nend.x, texture->length[ i ].width[ j ].end );
			nend.y = zutil::zmin( nend.y, texture->length[ i ].end );
			if( grfx::calcDrawrect( rstr->getDimensions(), getDimensions(), &nloc, &nstart, &nend ) ) {
				int xlen = texture->length[ i ].width[ j ].end - texture->length[ i ].width[ j ].start;
				int ylen = texture->length[ i ].end - texture->length[ i ].start;
				float txstart = static_cast< float >( nstart.x - texture->length[ i ].width[ j ].start ) / xlen;
				float txend = static_cast< float >( nend.x - texture->length[ i ].width[ j ].start ) / xlen;
				float tystart = static_cast< float >( nstart.y - texture->length[ i ].start ) / ylen;
				float tyend = static_cast< float >( nend.y - texture->length[ i ].start ) / ylen;
				CUSTOMVERTEX g_Vertices[] =
				{
					{ static_cast< float >( nloc.x ) - 0.5, static_cast< float >( nloc.y + nend.y - nstart.y ) - 0.5, cpos, cpos, txstart, tyend },
					{ static_cast< float >( nloc.x ) - 0.5, static_cast< float >( nloc.y ) - 0.5, cpos, cpos, txstart, tystart },
					{ static_cast< float >( nloc.x + nend.x - nstart.x ) - 0.5, static_cast< float >( nloc.y + nend.y - nstart.y ) - 0.5, cpos, cpos, txend, tyend },
					{ static_cast< float >( nloc.x + nend.x - nstart.x ) - 0.5, static_cast< float >( nloc.y ) - 0.5, cpos, cpos, txend, tystart },
				};

				// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
				// gain access to the vertices. This mechanism is required becuase vertex
				// buffers may be in device memory.
				VOID* pVertices;
				if( FAILED( vbuff->Lock( 0, sizeof(g_Vertices), (BYTE**)&pVertices, 0 ) ) )
					return;
				memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
				vbuff->Unlock();

				dev->SetTexture( 0, texture->length[ i ].width[ j ].tex );
				dev->SetStreamSource( 0, vbuff, sizeof(CUSTOMVERTEX) );
				dev->SetVertexShader( D3DFVF_CUSTOMVERTEX );
				dev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

				cpos--;
			}

		}

	}

}

void kWritableD3D::drawPoints( const std::pair< kPoint< INT32 >, grfx::kColor > *pointArray, int count ) { };
void kWritableD3D::clear( grfx::kColor color ) {
	dev->Clear( 0, NULL, D3DCLEAR_TARGET, color.argb, 1.0, 0 ); };

// setup functions
kWritableD3D::kWritableD3D( IDirect3DDevice8 *in_dev ) : df( &texes ) {
	dev = in_dev;
	D3DDEVICE_CREATION_PARAMETERS creparam;
	dev->GetCreationParameters( &creparam );
	hWnd = creparam.hFocusWindow;

	D3DCAPS8 caps;
	dev->GetDeviceCaps( &caps );

	maxTex.x = caps.MaxTextureWidth;
	maxTex.y = caps.MaxTextureHeight;

	maxTexAspect = caps.MaxTextureAspectRatio;

};

kWritableD3D::~kWritableD3D() {
	g_errlog << "D3D: Destroying hardware textures" << std::endl;
	std::map< const grfx::kRasterConst *, kWritableD3DTexture >::iterator texit;
	for( texit = texes.begin(); texit != texes.end(); texit++ ) {
#if POSTDEBUGINFO
		g_errlog << "D3D: (debug) Removing hardware texture for " << texit->first->textdesc() << " (" << texit->first << ")" << std::endl;
#endif
		texit->first->removeDestructionNotification( &df );
		texit->second.clear();
	}
};

void kWritableD3D::start() {
	dev->BeginScene();
    if( FAILED( dev->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vbuff ) ) )
    {
		g_errlog << "D3D: Vertex buffer creation failed! Things will now break." << std::endl;
		// TODO: make things not break as badly.
        return;
    } // had some problems putting this in the constructor. Don't know why.
	cpos = 1000000;
	RECT rect;
	GetClientRect( hWnd, &rect );
	curdim.x = rect.right;
	curdim.y = rect.bottom;
	if( FAILED( dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ) ) ) {
		g_errlog << "D3D: Problem with setting alpha blend" << std::endl;
	}
	if( FAILED( dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ) ) ) {
		g_errlog << "D3D: Problem with setting source blend" << std::endl;
	}
	if( FAILED( dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ) ) ) {
		g_errlog << "D3D: Problem with setting destination blend" << std::endl;
	}
};

void kWritableD3D::end() {
	vbuff->Release();
	dev->EndScene(); };

int nextPower( int x ) {
	int tx = 1;
	while( tx < x )
		tx *= 2;
	return tx;
};


// TODO: aspect ratio checking
kWritableD3DTexture *kWritableD3D::getTex( const grfx::kRasterConst *rstr ) {

	std::map< const grfx::kRasterConst *, kWritableD3DTexture >::iterator texit = texes.find( rstr );

	if( texit != texes.end() ) 
		return &texit->second;

#if POSTDEBUGINFO
	g_errlog << "D3D: (debug) Generating new hardware texture for " << rstr->textdesc() << " (" << rstr << ")" << std::endl;
#endif

	kWritableD3DTexture newtex;
	kPoint< INT32 > dim = rstr->getDimensions();
	for( int cury = 0; cury < dim.y; cury += maxTex.y ) {
		int vsize = zutil::zmin( maxTex.y, nextPower( dim.y - cury ) );
		int vreal = zutil::zmin( vsize, dim.y - cury );
		kWritableD3DTextureLine tl;
		tl.start = cury;
		tl.end = cury + vsize;
		for( int curx = 0; curx < dim.x; curx += maxTex.x ) {
			kWritableD3DTextureChunk tc;
			int hsize = zutil::zmin( maxTex.x, nextPower( dim.x - curx ) );
			int hreal = zutil::zmin( hsize, dim.x - curx );
			tc.start = curx;
			tc.end = curx + hsize;
			HRESULT hr = dev->CreateTexture( hsize, vsize, 1, NULL, D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED, &tc.tex );
			// TODO: make D3DPOOL_DEFAULT
			if( FAILED( hr ) ) {
				g_errlog << "D3D: Failed to make tex at " << curx << "," << cury << " - " << d3D8Stream( hr ) << std::endl;
				continue;
			}

			D3DLOCKED_RECT dlock;
			RECT lockarea;
			lockarea.top = 0;
			lockarea.bottom = vreal;
			lockarea.left = 0;
			lockarea.right = hreal;
			hr = tc.tex->LockRect( 0, &dlock, &lockarea, NULL );
			if( FAILED( hr ) ) {
				g_errlog << "D3D: Failed to lock tex at " << curx << "," << cury << " - " << d3D8Stream( hr ) << std::endl;
				tc.tex->Release();
				continue;
			}
			BYTE *dlbyte = reinterpret_cast< BYTE * >( dlock.pBits );

			grfx::kLockedRead glock;
			rstr->lock( kRect< INT32 >( tc.start, tl.start, tc.start + hreal, tl.start + vreal ), &glock );

			for( int i = 0; i < vreal; i++ )
				memcpy( dlbyte + i * dlock.Pitch, glock.data + curx + ( cury + i ) * glock.pitch, sizeof( grfx::kColor ) * hreal );

			rstr->unlock( &glock );
			tc.tex->UnlockRect( 0 );

			tl.width.push_back( tc );

		}

		newtex.length.push_back( tl );
	}

	texes[ rstr ] = newtex;

	rstr->addDestructionNotification( &df );

	return &texes[ rstr ];

};

RVOID kWritableD3DDestructFunctor::operator()( const grfx::kRasterConst *inp ) {
#if POSTDEBUGINFO
	g_errlog << "D3D: (debug) Removing hardware texture for " << inp->textdesc() << " (" << inp << ")" << std::endl;
#endif
	(*mp)[ inp ].clear();
	mp->erase( inp );
	return RVOIDVAL;
};

kWritableD3DDestructFunctor::kWritableD3DDestructFunctor( std::map< const grfx::kRasterConst *, kWritableD3DTexture > *in_mp ) {
	mp = in_mp; };

void kWritableD3DTexture::clear() {
	for( int i = 0; i < length.size(); i++ ) {
		for( int j = 0; j < length[ i ].width.size(); j++ ) {
			length[ i ].width[ j ].tex->Release();
		}
	}
};
