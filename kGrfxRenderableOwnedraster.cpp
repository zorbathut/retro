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

#include "kGrfxRenderableOwnedraster.h"

#include <iostream>

namespace grfx {

	const kRasterConst kRenderableOwnedraster::getRaster() const {
		return raster; };

	const kPoint< INT32 > &kRenderableOwnedraster::getDimensions() const {
		return raster.getDimensions(); };

	void kRenderableOwnedraster::renderTo( kWritable *target, const kPoint< INT32 > &loc ) const {
		target->drawRaster( raster, loc ); };

	void kRenderableOwnedraster::renderPartTo( kWritable *target, const kPoint< INT32 > &pos, const kPoint< INT32 > &start, const kPoint< INT32 > &end ) const {
		target->drawRasterPart( raster, pos, start, end ); };

	kRenderableOwnedraster::kRenderableOwnedraster( const kRaster &inrast ) : raster( inrast ) { };
	kRenderableOwnedraster::kRenderableOwnedraster() : raster( kRaster( 0, 0, NULL, 0 ) ) { };
	kRenderableOwnedraster::~kRenderableOwnedraster() { 
		delete [] raster.getData(); };

	void kRenderableOwnedraster::describe( std::ostream &ostr ) const VAGUEDESC {
		ostr << "unidentified renderableOwnedraster";
		kRenderableOwnedraster::chaindown( ostr ); };

	void kRenderableOwnedraster::chaindown( std::ostream &ostr ) const {
		kRenderableRaster::chaindown( ostr ); };

}