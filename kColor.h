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

#ifndef RETRO_KCOLOR
#define RETRO_KCOLOR

namespace grfx {

	union kColor;

};

#include "types.h"
#include "iostream_mini.h"

namespace grfx {

	union kColor {

		EXACTUINT32 argb;

		struct {
			BYTE b;
			BYTE g;
			BYTE r;
			BYTE a;
		} split;

		BYTE bytes[ 4 ];

		kColor() { };				// this *should* auto-inline, but isn't. grrr.
		kColor( const kColor &alt );
		kColor( BYTE a, BYTE r, BYTE g, BYTE b );
		kColor( EXACTUINT32 in_argb );

	};	// alpha and RGB are perfectly normal in every way.

	union kColorHSL {

		EXACTUINT32 ahsl;

		struct {
			BYTE l;
			BYTE s;
			BYTE h;
			BYTE a;
		} split;

		BYTE bytes[ 4 ];

		kColorHSL() { };				// this *should* auto-inline, but isn't. grrr.
		kColorHSL( const kColorHSL &alt );
		kColorHSL( BYTE a, BYTE h, BYTE s, BYTE l );
		kColorHSL( EXACTUINT32 in_ahsl );

	};

	std::ostream &operator<<( std::ostream &ostr, kColor inp );
	std::ostream &operator<<( std::ostream &ostr, kColorHSL inp );

	bool operator<( kColor lhs, kColor rhs );
	bool operator<( kColorHSL lhs, kColorHSL rhs );

	kColorHSL operator*( kColorHSL lhs, kColorHSL rhs );

	namespace cutil {

		kColor HSLtoRGB( kColorHSL hsl );
		kColorHSL RGBtoHSL( kColor hsl );

	};

};

#endif
