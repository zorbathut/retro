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

#ifndef RETRO_KWRITABLED3D
#define RETRO_KWRITABLED3D

#pragma warning( disable : 4786 )

#include <d3d8.h>
#include <map>
#include "kGrfxWritable.h"

class kWritableD3DTextureChunk {
public:
	IDirect3DTexture8 *tex;
	INT32 start;
	INT32 end;
};

class kWritableD3DTextureLine {
public:
	std::vector< kWritableD3DTextureChunk > width;
	INT32 start;
	INT32 end;
};

class kWritableD3DTexture {
public:
	std::vector< kWritableD3DTextureLine > length;

	void clear();
};

class kWritableD3DDestructFunctor : public zutil::kFunctor< RVOID, const grfx::kRasterConst * > {
public:

	virtual RVOID operator()( const grfx::kRasterConst * inp );

	kWritableD3DDestructFunctor( std::map< const grfx::kRasterConst *, kWritableD3DTexture > *mp );

private:
	std::map< const grfx::kRasterConst *, kWritableD3DTexture > *mp;

};

class kWritableD3D : public grfx::kWritable {
public:

	// information functions
	virtual const kPoint< INT32 > &getDimensions( void ) const;

	// drawing functions
	virtual void drawRaster( const grfx::kRasterConst *rstr, const kPoint< INT32 > &loc );
	virtual void drawRasterPart( const grfx::kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end );
	virtual void drawPoints( const std::pair< kPoint< INT32 >, grfx::kColor > *pointArray, int count );
	virtual void clear( grfx::kColor color );

	// setup functions
	kWritableD3D( IDirect3DDevice8 *dev );
	virtual ~kWritableD3D();

	void start();
	void end();

private:

	kWritableD3DTexture *getTex( const grfx::kRasterConst *rstr );

	IDirect3DDevice8 *dev;

	float cpos;

	kPoint< INT32 > curdim;

	HWND hWnd;

	std::map< const grfx::kRasterConst *, kWritableD3DTexture > texes;

	kWritableD3DDestructFunctor df;

	IDirect3DVertexBuffer8 *vbuff;

	kPoint< INT32 > maxTex;
	int maxTexAspect;

};

#endif
