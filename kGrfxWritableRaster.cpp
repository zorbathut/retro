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

#include "kGrfxWritableRaster.h"

#include "minmax.h"
#include "kGrfxUtil.h"

namespace grfx {

	const kPoint< INT32 > &kWritableRaster::getDimensions( void ) const {
		return raster.getDimensions(); };

	void kWritableRaster::drawRaster( const kRasterConst &rstr, const kPoint< INT32 > &loc ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > start( 0, 0 );
		kPoint< INT32 > end( rstr.getDimensions() );
		if( calcDrawrect( rstr.getDimensions(), raster.getDimensions(), &nloc, &start, &end ) )
			raster.drawRasterPart( rstr, nloc, start, end );
	
	};

	void kWritableRaster::drawRasterPart( const kRasterConst &rstr, const kPoint< INT32 > &loc,
										  const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > nstart( start );
		kPoint< INT32 > nend( end );
		if( calcDrawrect( rstr.getDimensions(), raster.getDimensions(), &nloc, &nstart, &nend ) )
			raster.drawRasterPart( rstr, nloc, nstart, nend );
	
	};

	void kWritableRaster::drawPoints( const std::pair< kPoint< INT32 >, kColor > *pointArray, int count ) {
		bool bad = false;;
		const std::pair< kPoint< INT32 >, kColor > *mpt = pointArray;
		for( int cur = 0; cur < count; cur++ ) {
			if( mpt->first.x < 0 || mpt->first.x >= raster.getDimensions().x ||
				mpt->first.y < 0 || mpt->first.x >= raster.getDimensions().y ) {
				bad = true;
				break;
			}
			mpt++;
		}
		if( !bad ) {
			raster.drawPoints( pointArray, count );
		} else {
			std::pair< kPoint< INT32 >, kColor > *nptarr = new std::pair< kPoint< INT32 >, kColor >[ count ];
			std::pair< kPoint< INT32 >, kColor > *npt = nptarr;
			mpt = pointArray;
			for( int curg = 0; curg < count; curg++ ) {
				if( mpt->first.x >= 0 && mpt->first.x < raster.getDimensions().x &&
					mpt->first.y >= 0 && mpt->first.x < raster.getDimensions().y ) {
					*npt = *mpt;
					npt++;
				}
				mpt++;
			}
			raster.drawPoints( nptarr, npt - nptarr );
		}
	};

	void kWritableRaster::clear( kColor color ) {
		raster.clear( color ); };

	const kRaster &kWritableRaster::getRaster() {
		return raster; };

	kWritableRaster::kWritableRaster( const kRaster &inrast ) : raster( inrast ) { };

}