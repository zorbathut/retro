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

#include "kColor.h"

#include "minmax.h"

#include <iostream>

namespace grfx {

	kColor::kColor( const kColor &alt ) : argb( alt.argb ) { };
	kColor::kColor( BYTE a, BYTE r, BYTE g, BYTE b ) { split.a = a; split.r = r; split.g = g; split.b = b; };
	kColor::kColor( EXACTUINT32 in_argb ) : argb( in_argb ) { };	// todo: endian manipulation

	kColorHSL::kColorHSL( const kColorHSL &alt ) : ahsl( alt.ahsl ) { } ;
	kColorHSL::kColorHSL( BYTE a, BYTE h, BYTE s, BYTE l ) { split.a = a; split.h = h; split.s = s; split.l = l; };
	kColorHSL::kColorHSL( EXACTUINT32 in_ahsl ) : ahsl( in_ahsl ) { };

	std::ostream &operator<<( std::ostream &ostr, kColor inp ) {
		ostr << "color( " << int( inp.split.r ) << ", " << int( inp.split.g ) << ", " << int( inp.split.b ) << " a" << int( inp.split.a ) << " )";
		return ostr;
	};

	std::ostream &operator<<( std::ostream &ostr, kColorHSL inp ) {
		ostr << "colorHSL( " << int( inp.split.h ) << ", " << int( inp.split.s ) << ", " << int( inp.split.l ) << " a" << int( inp.split.a ) << " )";
		return ostr;
	};

	bool operator<( kColor lhs, kColor rhs ) {
		return lhs.argb < rhs.argb; };

	bool operator<( kColorHSL lhs, kColorHSL rhs ) {
		return lhs.ahsl < rhs.ahsl; };

	kColorHSL operator*( kColorHSL lhs, kColorHSL rhs ) {
		kColorHSL out;
		out.split.a = lhs.split.a * rhs.split.a / 0xff;
		out.split.h = ( lhs.split.h + rhs.split.h ) % 0xff;
		out.split.s = lhs.split.s * rhs.split.s / 0xff;
		if( lhs.split.l >= 0x80 && rhs.split.l >= 0x80 )
			out.split.l = 0xff - ( 0xff - lhs.split.l ) * ( 0xff - rhs.split.l ) * 2 / 0xff;
		  else
			out.split.l = lhs.split.l * rhs.split.l * 2 / 0xff;
		return out;
	}

	namespace cutil {

		// thanks to http://blas.cis.mcmaster.ca/~monger/hsl-rgb.html for these calculations, if it's still up

		int tc3c( int t1, int t2, int t3 ) {
			if( t3 * 6 < 0x100 )
				return ( t1 * 0x100 + ( t2 - t1 ) * 6 * t3 ) / 0x100 / 0x100;
			else if( t3 * 2 < 0x100 )
				return t2 / 0x100;
			else if( t3 * 3 < 0x200 )
				return ( t1 * 0x100 + ( t2 - t1 ) * ( -t3 * 6 + 0x400 ) ) / 0x100 / 0x100;
			return t1 / 0x100;
		};

		kColor HSLtoRGB( kColorHSL hsl ) {

			kColor rgb;

			rgb.split.a = hsl.split.a;

			if( !hsl.split.s ) {

				rgb.split.r = hsl.split.l;
				rgb.split.g = hsl.split.l;
				rgb.split.b = hsl.split.l;

			} else {

				int t2;

				if( hsl.split.l <= 0x7f )
					t2 = hsl.split.l * ( 0xff + hsl.split.s );
				  else
					t2 = ( hsl.split.l + hsl.split.s ) * 0x100 - hsl.split.l * hsl.split.s;

				int t1 = 2 * hsl.split.l * 0x100 - t2;

				int rt = ( hsl.split.h + 0x100/3 ) % 0x100;
				int gt = hsl.split.h;
				int bt = ( hsl.split.h + 0x100 * 2 /3 ) % 0x100;

				rgb.split.r = BYTE( tc3c( t1, t2, rt ) );
				rgb.split.g = BYTE( tc3c( t1, t2, gt ) );
				rgb.split.b = BYTE( tc3c( t1, t2, bt ) );

			}

			return rgb;

		};
				
		kColorHSL RGBtoHSL( kColor rgb ) {

			kColorHSL hsl;

			hsl.split.a = rgb.split.a;

			int mx = zutil::zmax( rgb.split.r, zutil::zmax( rgb.split.g, rgb.split.b ) );
			int mn = zutil::zmin( rgb.split.r, zutil::zmin( rgb.split.g, rgb.split.b ) );

			hsl.split.l = BYTE( ( mx + mn ) / 2 );

			if( mx == mn ) {

				hsl.split.h = 0;
				hsl.split.s = 0;

			} else {

				if( hsl.split.l <= 0x7f )
					hsl.split.s = BYTE( ( ( mx - mn ) * 0x100 ) / ( mx + mn ) );
				  else
					hsl.split.s = BYTE( ( ( mx - mn ) * 0x100 ) / ( ( 0x100 * 2 ) - mx - mn ) );

				if( mx == rgb.split.r )
					hsl.split.h = BYTE( ( ( ( rgb.split.g - rgb.split.b ) * 0x100 ) / ( mx - mn ) ) / 6 );
				else if( mx == rgb.split.g )
					hsl.split.h = BYTE( ( 0x100 * 2 + ( ( rgb.split.b - rgb.split.r ) * 0x100 ) / ( mx - mn ) ) / 6 );
				else
					hsl.split.h = BYTE( ( 0x100 * 4 + ( ( rgb.split.r - rgb.split.g ) * 0x100 ) / ( mx - mn ) ) / 6 );

			}

			return hsl;

		};


	};

};
