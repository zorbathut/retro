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

#include <cassert>
#include <iostream>
#include <fstream>
#include <set>

namespace grfx {

	std::ofstream yakanor;
	bool ope = false;

	class kRostaMan {
	public:

		std::set< const kRasterConst * > rosta;

		~kRostaMan() {
			std::set< const kRasterConst * >::iterator itr;
			for( itr = rosta.begin(); itr != rosta.end(); itr++ )
				yakanor << "*** MAJOR: " << *itr << " not removed!" << std::endl;
		}

	};

	kRostaMan roasta;

	class k565ChaNoti : public zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > {
	public:

		bool operator()( std::pair< const kRect< INT32 > *, const kRasterConst * > inp ) {
			yakanor << "*** MAJOR: " << inp.second->textdesc() << " (" << inp.second << ") changed at " << inp.first << ", removing" << std::endl;
			roasta.rosta.erase( inp.second );
			return false;
		};

	};

	class k565DelNoti : public zutil::kFunctor< RVOID, const kRasterConst * > {
	public:

		RVOID operator()( const kRasterConst *inp ) {
			yakanor << "*** MAJOR: " << inp->textdesc() << " (" << inp << ") deleted, removing" << std::endl;
			roasta.rosta.erase( inp );
			return RVOIDVAL;
		};

	};

	void kWritable16bpp565::drawRaster( const kRasterConst *rstr, const kPoint< INT32 > &loc ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > start( 0, 0 );
		kPoint< INT32 > end( rstr->getDimensions() );

		if( calcDrawrect( rstr->getDimensions(), dimensions, &nloc, &start, &end ) )
			drawRasterGo( rstr, nloc, start, end );

	};

	void kWritable16bpp565::drawRasterPart( const kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

		kPoint< INT32 > nloc( loc );
		kPoint< INT32 > nstart( start );
		kPoint< INT32 > nend( end );

		if( calcDrawrect( rstr->getDimensions(), dimensions, &nloc, &nstart, &nend ) )
			drawRasterGo( rstr, nloc, nstart, nend );

	};

	void kWritable16bpp565::drawRasterGo( const kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end ) {

//		yakanor << "MINOR: Drawing raster " << rstr->textdesc() << " (" << rstr << ") at " << loc << " from " << start << "-" << end << std::endl;

		if( roasta.rosta.find( rstr ) == roasta.rosta.end() ) {
			yakanor << "*** MAJOR: Adding raster " << rstr->textdesc() << " (" << rstr << ")" << std::endl;
			roasta.rosta.insert( rstr );
			rstr->addDestructionNotification( new k565DelNoti() );
			rstr->addModifyNotification( kRect< INT32 >::makeBounds( rstr->getDimensions() ), new k565ChaNoti() );
		}

		assert( ( pitch % 2 ) == 0 );
		// todo: better pitch%2 test

		kLockedRead lock;
		rstr->lock( kRect< INT32 >( start, end ), &lock );

		const BYTE *linesrc = reinterpret_cast< const BYTE * >( lock.data );
		EXACTUINT16 *linedst = data;

		linesrc += ( lock.pitch * start.y + start.x ) * sizeof( grfx::kColor );
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
			linesrc += lock.pitch * sizeof( grfx::kColor );
			linedst += pitch;
		}

		rstr->unlock( &lock );

	}

	void kWritable16bpp565::drawPoints( const std::pair< kPoint< INT32 >, kColor > *pointArray, int count ) {
//		yakanor << "MINOR: Drawing " << count << " points" << std::endl;
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
//		yakanor << "MINOR: Clearing with color " << col << std::endl;
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
		if( !ope ) {
			yakanor.open( "drawlog.txt" );
			ope = true;
		}
	};

	void kWritable16bpp565::describe( std::ostream &ostr ) const {
		ostr << "565 renderable target";
		kWritable16bpp::chaindown( ostr ); }

	void kWritable16bpp565::chaindown( std::ostream &ostr ) const {
		ostr << " (565)";
		kWritable16bpp::chaindown( ostr ); }

}