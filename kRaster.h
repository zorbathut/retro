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

#ifndef TIMESPACE_KRASTER
#define TIMESPACE_KRASTER

// semantics explanation: rasters are basically more complex pointers. Thus, things like
// rendering are considered const, despite the fact that they change the *contained*
// data. They don't change the *pointer*.

namespace grfx {

	class kRasterConst;
	class kRaster;

}

#include <utility>

#include "kPoint.h"
#include "kColor.h"

namespace grfx {

	class kRasterConst {
	public:

		const kPoint< INT32 > &getDimensions() const;
		const kColor *getData() const;
		INT32 getPitch() const;

	    void setDimensions( const kPoint< INT32 > &ind );
		void setData( const kColor *ind );
		void setPitch( int ind );

		kRasterConst( const kPoint< INT32 > &dim, const kColor *dat, INT32 pitch );
		kRasterConst( INT32 x, INT32 y, const kColor *dat, INT32 pitch );

		kRasterConst( const kRaster &in );
		kRasterConst( const kRasterConst &in );
		void operator=( const kRaster & );
		void operator=( const kRasterConst & );

	private:

		kPoint< INT32 > dimension;
		const kColor *data;
		INT32 pitch;

	}; // raster is defined as 32bit ARGB. Whether A matters or not depends on context.

	class kRaster {
	public:

		const kPoint< INT32 > &getDimensions() const;
		kColor *getData() const;
		INT32 getPitch() const;

	    void setDimensions( const kPoint< INT32 > &ind );
		void setData( kColor *ind );
		void setPitch( int ind );

		void drawRasterPart( const kRasterConst &in, const kPoint< INT32 > &pos, const kPoint< INT32 > &start,
							 const kPoint< INT32 > &end ) const;
		void drawPoints( const std::pair< kPoint< INT32 >, kColor > *pointArray, int count ) const;
		void clear( kColor color ) const;

		kRaster( const kPoint< INT32 > &dim, kColor *dat, INT32 pitch );
		kRaster( INT32 x, INT32 y, kColor *dat, INT32 pitch );

		kRaster( const kRaster &in );
		void operator=( const kRaster & );
			
	private:

		kPoint< INT32 > dimension;
		kColor *data;
		INT32 pitch;

	};

};

#endif