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

#error Header disabled and obsolete, leaving it around in case I rebuild it.

#ifndef RETRO_KGRFXWRITABLE16BPP565
#define RETRO_KGRFXWRITABLE16BPP565

namespace grfx {

	class kWritableRaster;

}

#include "kGrfxWritable16bpp.h"

namespace grfx {

	class kWritable16bpp565 : public kWritable16bpp {
	public:

		// drawing functions
		virtual void drawRaster( const kRasterConst *rstr, const kPoint< INT32 > &loc );
		virtual void drawRasterPart( const kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end );
		virtual void clear( kColor color );

		// setup functions
		kWritable16bpp565( int width, int height, EXACTUINT16 *data, int pitch );

		virtual void describe( std::ostream &ostr ) const;
	protected:  void chaindown( std::ostream &ostr ) const;

	private:

		void drawRasterGo( const kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end );

	};

}

#endif