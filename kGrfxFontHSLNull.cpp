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

#include "kGrfxFontHSLNull.h"

#include "kFileWrappedNull.h"
#include "kGrfxFontNull.h"

namespace grfx {

	kFontHSLNull::kFontHSLNull() { };
	kFontHSLNull::~kFontHSLNull() { };

	void kFontHSLNull::describe( std::ostream &ostr ) const {
		ostr << "null fontHSL";
		kFontHSL::chaindown( ostr ); };

	void kFontHSLNull::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-null fontHSL)";
		kFontHSL::chaindown( ostr ); };

	file::kHandle< kFont > kFontHSLNull::genChroma( kColor val ) const {
		return file::kHandle< kFont >( &null::grfxFont, &null::wrapped ); };
	void kFontHSLNull::wipeChroma( kColor val ) const { };

};

namespace null {

	grfx::kFontHSLNull grfxFontHSL;

};
