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

#ifndef RETRO_KGRFXWRITABLERASTER
#define RETRO_KGRFXWRITABLERASTER

namespace grfx {

	class kWritableRaster;

}

#include "kGrfxWritable.h"

namespace grfx {

	class kWritableRaster : public kWritable {
	public:

		// information functions
		virtual const kPoint< INT32 > &getDimensions() const;

		// drawing functions
		virtual void drawRaster( const kRasterConst *rstr, const kPoint< INT32 > &pos );
		virtual void drawRasterPart( const kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end );
		virtual void drawPoints( const std::pair< kPoint< INT32 >, kColor > *pointArray, int count );
		virtual void clear( kColor color );

		kRaster      *getRaster();
		kRasterConst *getRaster() const;

		static void getUncroppedDimensions( const kPoint< INT32 > &offset, const kPoint< INT32 > &sourcesize, const kPoint< INT32 > &targetsize, kPoint< INT32 > *start, kPoint< INT32 > *end );
		static void getCroppedDimensions( const kPoint< INT32 > &offset, const kPoint< INT32 > &sourcesize, const kPoint< INT32 > &targetsize, kPoint< INT32 > *start, kPoint< INT32 > *end );

		// setup functions
		kWritableRaster( const kPoint< INT32 > &dim );
		kWritableRaster( const kPoint< INT32 > &dim, kColor *dat, INT32 pitch, bool owned = true );

		virtual void describe( std::ostream &ostr ) const;
	protected:  void chaindown( std::ostream &ostr ) const;

	private:

		kRaster *raster;

		void doDraw( const kRasterConst *rstr, const kPoint< INT32 > &loc, const kPoint< INT32 > &start, const kPoint< INT32 > &end );

	};

}

#endif