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

#include <iostream>

namespace grfx {

	const kPoint< INT32 > &kWritableRaster::getDimensions() const {
		return raster->getDimensions(); };

	void kWritableRaster::drawRaster( const kRasterConst *rstr, const kPoint< INT32 > &loc ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > start( 0, 0 );
		kPoint< INT32 > end( rstr->getDimensions() );
		if( calcDrawrect( rstr->getDimensions(), raster->getDimensions(), &nloc, &start, &end ) )
			doDraw( rstr, nloc, start, end );
	
	};

	void kWritableRaster::drawRasterPart( const kRasterConst *rstr, const kPoint< INT32 > &loc,
										  const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > nstart( start );
		kPoint< INT32 > nend( end );
		if( calcDrawrect( rstr->getDimensions(), raster->getDimensions(), &nloc, &nstart, &nend ) )
			doDraw( rstr, nloc, nstart, nend );
	
	};

	void kWritableRaster::drawPoints( const std::pair< kPoint< INT32 >, kColor > *pdat, int count ) {
		kLockedWrite lock;
		raster->writeLock( kRect< INT32 >( kPoint< INT32 >( 0, 0 ), raster->getDimensions() ), &lock );
		// 'tis easier.
		for( int i = 0; i < count; i++ )
			lock.data[ pdat[ i ].first.x + pdat[ i ].first.y * lock.pitch ] = pdat[ i ].second;
		raster->unlock( &lock );
	};

	void kWritableRaster::clear( kColor color ) {
		kLockedWrite lock;
		raster->writeLock( kRect< INT32 >( kPoint< INT32 >( 0, 0 ), raster->getDimensions() ), &lock );
		kColor *arr;
		arr = new kColor[ lock.bounds.br.x ]; // will this always work?
		int i;
		for( i = 0; i < lock.bounds.br.x; i++ )
			arr[ i ] = color;
		for( i = 0; i < lock.bounds.br.y; i++ )
			memcpy( lock.data + i * lock.pitch, arr, lock.bounds.br.x * sizeof( kColor ) );
		delete [] arr;
		raster->unlock( &lock );
	};

	kRaster      *kWritableRaster::getRaster() {
		return raster; };
	kRasterConst *kWritableRaster::getRaster() const {
		return raster; };

	kWritableRaster::kWritableRaster( const kPoint< INT32 > &dim ) : raster( new kRaster( dim ) ) { };
	kWritableRaster::kWritableRaster( const kPoint< INT32 > &dim, kColor *dat, INT32 pitch, bool owned ) :
			raster( new kRaster( dim, dat, pitch, owned ) ) { };

	void kWritableRaster::doDraw( const kRasterConst *rstr, const kPoint< INT32 > &pos,
								  const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

		kLockedWrite klw;
		kLockedRead klr;

		rstr->lock( kRect< INT32 >( start, end ), &klr );
		raster->writeLock( kRect< INT32 >( pos, pos + ( end - start ) ), &klw );

		int len = ( end.x - start.x ) * sizeof( kColor );

		for( int i = start.y; i < end.y; i++ )
			memcpy( klw.data + pos.x + ( i + pos.y - start.y ) * klw.pitch,
				    klr.data + start.x + ( i ) * klr.pitch, 
					len );

		rstr->unlock( &klr );
		raster->unlock( &klw );

	}

	void kWritableRaster::describe( std::ostream &ostr ) const {
		ostr << "writable raster (" << raster->textdesc() << ")";
		kWritable::chaindown( ostr ); };

	void kWritableRaster::chaindown( std::ostream &ostr ) const {
		ostr << "(writable raster - " << raster->textdesc() << ")";
		kWritable::chaindown( ostr ); };

};
