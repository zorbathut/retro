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

#include "kGrfxUtil.h"

#include "minmax.h"

namespace grfx {

	bool calcDrawrect( const kPoint< INT32 > &srcsize, const kPoint< INT32 > &dstsize,
					   kPoint< INT32 > *loc, kPoint< INT32 > *start, kPoint< INT32 > *end ) {

		int mod;

		mod = zutil::zmax( 0, zutil::zmax( -loc->x, -start->x ) );
		loc->x += mod;
		start->x += mod;
		end->x = zutil::zmin( dstsize.x - loc->x + start->x, zutil::zmin( end->x, srcsize.x ) );

		mod = zutil::zmax( 0, zutil::zmax( -loc->y, -start->y ) );
		loc->y += mod;
		start->y += mod;
		end->y = zutil::zmin( dstsize.y - loc->y + start->y, zutil::zmin( end->y, srcsize.y ) );

		return true;

	}

}
