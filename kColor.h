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

#pragma warning( disable : 4786 )

#ifndef TIMESPACE_KCOLOR
#define TIMESPACE_KCOLOR

namespace grfx {

	union kColor;

};

#include "types.h"

namespace grfx {

	union kColor {

		EXACTUINT32 argb;

		struct {
			BYTE a;
			BYTE r;
			BYTE g;
			BYTE b;
		} split;

		bool checkConsistency();	// evaluates to noop when not in debug. might never implement :)

		kColor() { };
		kColor( const kColor &alt ) : argb( alt.argb ) { };
		kColor( BYTE a, BYTE r, BYTE g, BYTE b ) { split.a = a; split.r = r; split.g = g; split.b = b; };
		kColor( EXACTUINT32 in_argb ) : argb( in_argb ) { };	// todo: endian manipulation

		static kColor makeMultiply( BYTE a, BYTE r, BYTE g, BYTE b );
		static kColor makeMultiply( EXACTUINT32 in_argb );
		

	};	// this *better* be 32bit.
		// info: alpha's inverted, rgb premultiplied. examples:
		// 0x00000000: pure opaque black.
		// 0x80000000: 50% transparent pure black.
		// 0x00808080: opaque 50% gray.
		// 0x807f7f7f: 50% transparent white.
		// 0x807f807f: INVALID. ASSERT( a+g>=0xff ).

};

#endif