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

#include "kGrfxWritable16bpp565.h"

#include "kGrfxUtil.h"
#include "minmax.h"

#include <assert.h>

namespace grfx {

		void calcDrawrect( const kPoint< INT32 > &loc, const kPoint< INT32 > &srcsize, const kPoint< INT32 > &dstsize, kPoint< INT32 > *start, kPoint< INT32 > *end );

	void kWritable16bpp565::drawRaster( const kRasterConst &rstr, const kPoint< INT32 > &loc ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > start( 0, 0 );
		kPoint< INT32 > end( rstr.getDimensions() );

		if( calcDrawrect( rstr.getDimensions(), dimensions, &nloc, &start, &end ) )
			drawRasterGo( rstr, nloc, start, end );

	};

	void kWritable16bpp565::drawRasterPart( const kRasterConst &rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > nstart( start );
		kPoint< INT32 > nend( end );

		if( calcDrawrect( rstr.getDimensions(), dimensions, &nloc, &nstart, &nend ) )
			drawRasterGo( rstr, nloc, nstart, nend );

	};

	void kWritable16bpp565::drawRasterGo( const kRasterConst &rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

		assert( ( pitch % 2 ) == 0 );
		// todo: better pitch%2 test

		const BYTE *linesrc = reinterpret_cast< const BYTE * >( rstr.getData() );
		EXACTUINT16 *linedst = data;

		linesrc += ( rstr.getPitch() * start.y + start.x ) * sizeof( grfx::kColor );
		linedst += pitch * ( loc.y ) + loc.x;

		for( int cy = start.y; cy < end.y; cy++ ) {
			const BYTE *src = linesrc;
			EXACTUINT16 *dst = linedst;
			EXACTUINT16 *dstend = linedst + ( end.x - start.x );
			while( dst < dstend ) {
				*dst = ( src[ 0 ] >> 3 ) | ( ( src[ 1 ] & 0xFC ) << 3 ) | ( ( src[ 2 ] & 0xF8 ) << 8 );
				src += sizeof( grfx::kColor );
				dst++;
			}
			linesrc += rstr.getPitch() * sizeof( grfx::kColor );
			linedst += pitch;
		}

	}

	void kWritable16bpp565::drawPoints( const std::pair< kPoint< INT32 >, kColor > *pointArray, int count ) {
		for( int i = 0; i < count; i++ ) {
			if( pointArray[ i ].first.x >= 0 && pointArray[ i ].first.y >= 0 &&
				pointArray[ i ].first.x < dimensions.x && pointArray[ i ].first.y < dimensions.y )
				*( data + pointArray[ i ].first.x + pointArray[ i ].first.y * pitch ) =
					  ( pointArray[ i ].second.split.r          >> 3 ) |
					( ( pointArray[ i ].second.split.g & 0xFC ) << 3 ) |
					( ( pointArray[ i ].second.split.b & 0xF8 ) << 8 );
		}
	};

	void kWritable16bpp565::clear( kColor col ) {
		int i;
		EXACTUINT16 ncol = ( col.split.r >> 3 ) | ( ( col.split.g & 0xFC ) << 3 ) | ( ( col.split.b & 0xF8 ) << 8 );
		EXACTUINT16 *pre = data;
		EXACTUINT16 *now = data;
		for( i = 0; i < dimensions.x; i++ )
			*(now++) = ncol;
		now = pre + pitch;
		for( i = 1; i < dimensions.y; i++ ) {
			memcpy( now, pre, pitch * sizeof( EXACTUINT16 ) );
			now += pitch;
		}
	};

	kWritable16bpp565::kWritable16bpp565( int width, int height, EXACTUINT16 *data, int pitch ) :
			kWritable16bpp( width, height, data, pitch ) {
	};

}