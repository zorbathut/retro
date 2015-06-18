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

#include "kGrfxRenderableRasterHSL.h"

namespace grfx {

	const kRasterConst *kRenderableRasterHSL::getRaster() const {
		return rast; };

	void kRenderableRasterHSL::loadAll() {
		beginProgressive();
		continueProgressive( progres );
		completeProgressive();
	};

	void kRenderableRasterHSL::beginProgressive() {
		rast = new kRaster( src->getDimensions() );
		state = LOADING;
		rast->writeLock( makeRect( src->getDimensions() ), &lw );
		
	};

	void kRenderableRasterHSL::continueProgressive( int steps ) {
		kLockedRead lr;
		if( src->getRaster()->lock( makeRect( 0, progcur, src->getDimensions().y, progcur + steps ), &lr ) ) {
			fritz = true;
		} else {
			int i;
			int j;
			const kColor *rdl = lr.data;
			kColor *wrl = lw.data + lw.pitch * progcur;
			for( i = 0; i < steps; i++ ) {
				const kColor *rdc = rdl;
				kColor *wrc = wrl;
				for( j = 0; j < rast->getDimensions().x; j++ ) {
					*wrc = cutil::HSLtoRGB( kColorHSL( rdc->argb ) * hsl );
					wrc++;
					rdc++;
				}
				rdl += lr.pitch;
				wrl += lw.pitch;
			}
			src->getRaster()->unlock( &lr );
		}
		progcur += steps;
		if( progcur == progres )
			state = DONE;
	};
		
	void kRenderableRasterHSL::completeProgressive() {
		rast->unlock( &lw );
		state = READY;
	};

	void kRenderableRasterHSL::cancelProgressive() {
		rast->unlock( &lw );
		state = READY;
		fritz = true;
	};

	void kRenderableRasterHSL::unload() {
		delete rast;
		rast = NULL;
		state = EMPTY;
		fritz = false;
		progcur = 0;
	};

	kRenderableRasterHSL::kRenderableRasterHSL( file::kHandle< kRenderableRaster > in_src, kColor in_hsl ) :
		src( in_src ), hsl( cutil::RGBtoHSL( in_hsl ) )
	{
		progres = src->getDimensions().y;
		rast = NULL;
	};

	kRenderableRasterHSL::~kRenderableRasterHSL() {
		delete rast;
	};

// TODO: dehackify.
using file::kGeneratable;
	void kRenderableRasterHSL::describe( std::ostream &ostr ) const {
		ostr << "HSLraster (" << src->textdesc() << ", " << hsl << ")";
		kRenderableRaster::chaindown( ostr );
		kGeneratable::chaindown( ostr ); };

	void kRenderableRasterHSL::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-HSLraster (" << src->textdesc() << ", " << hsl << "))";
		kRenderableRaster::chaindown( ostr );
		kGeneratable::chaindown( ostr ); };

// TODO: regeneration if the source changes
};
