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

#include "kRaster.h"
#include "minmax.h"

namespace grfx {

	const kPoint< INT32 > &kRasterConst::getDimensions() const {
		return dimension; };

	const kColor *kRasterConst::getData() const { return data; };

	INT32 kRasterConst::getPitch() const { return pitch; };

	kRasterConst::kRasterConst( INT32 x, INT32 y, const kColor *dat, INT32 in_pitch ) :
			dimension( x, y ), data( dat ), pitch( in_pitch ) { };

	kRasterConst::kRasterConst( const kRaster &in ) :
			dimension( in.getDimensions() ), data( in.getData() ), pitch( in.getPitch() ) { };

	kRasterConst::kRasterConst( const kRasterConst &in ) :
			dimension( in.dimension ), data( in.data ), pitch( in.pitch ) { };

	const kPoint< INT32 > &kRaster::getDimensions() const {
		return dimension; };

	kColor *kRaster::getData() const { return data; }

	INT32 kRaster::getPitch() const { return pitch; };

    void kRaster::setDimensions( const kPoint< INT32 > &ind ) {
		dimension = ind; };

	void kRaster::setData( kColor *ind ) {
		data = ind; };

	void kRaster::setPitch( int ind ) {
		pitch = ind; };

	void kRaster::drawRasterPart( const kRasterConst &rstr, const kPoint< INT32 > &pos,
								  const kPoint< INT32 > &start, const kPoint< INT32 > &end ) const {

		int len = ( end.x - start.x ) * sizeof( kColor );

		for( int i = start.y; i < end.y; i++ )
			memcpy( getData() + pos.x + ( i + pos.y - start.y ) * getPitch(),
				    rstr.getData() + start.x + ( i ) * rstr.getPitch(), 
					len );

	}

	void kRaster::drawPoints( const std::pair< kPoint< INT32 >, kColor > *pointArray, int count ) const {
		for( int i = 0; i < count; i++ ) {
			*( getData() + pointArray[ i ].first.x + pointArray[ i ].first.y * getPitch() ) =
					pointArray[ i ].second;
		}
	};

	void kRaster::clear( kColor col ) const {
		int i;
		kColor *pre = data;
		kColor *now = data;
		for( i = 0; i < dimension.x; i++ )
			*(now++) = col;
		now = pre + pitch;
		for( i = 1; i < dimension.y; i++ ) {
			memcpy( now, pre, pitch * sizeof( kColor ) );
			now += pitch;
		}
	};

	kRaster::kRaster( const kPoint< INT32 > &dim, kColor *dat, INT32 in_pitch ) :
			dimension( dim ), data( dat ), pitch( in_pitch ) { };

	kRaster::kRaster( INT32 x, INT32 y, kColor *dat, INT32 in_pitch ) :
			dimension( x, y ), data( dat ), pitch( in_pitch ) { };

	kRaster::kRaster( const kRaster &in ) :
			dimension( in.dimension ), data( in.data ), pitch( in.pitch ) { };

	void kRaster::operator=( const kRaster &rhs ) {
		dimension = rhs.dimension;
		data = rhs.data;
		pitch = rhs.pitch; };

};
		
