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

#ifndef RETRO_KGRFXFONTHSLFILERB
#define RETRO_KGRFXFONTHSLFILERB

namespace grfx {

	class kFontHSLFilerb;

}

#include "kGrfxFontHSL.h"
#include "kFileWrappedFile.h"

#include "kGrfxRenderableRaster.h"

#include "kFunctor.h"

namespace grfx {

	class kFontHSLFilerb : public kFontHSL, public file::kWrappedFile {
	public:

		virtual void init( const std::string &boundname, const std::string &rendername );

		virtual void activate();
		virtual void deactivate();
		virtual void request( int ticks );
		virtual void urgent();

		kFontHSLFilerb( const std::string &fname );
		virtual ~kFontHSLFilerb();

		virtual void describe( std::ostream &ostr ) const;
	protected:  void chaindown( std::ostream &ostr ) const;

	private:

		virtual file::kHandle< kFont > genChroma( kColor val ) const;
		virtual void wipeChroma( kColor val ) const;

		file::kHandle< kRenderableRaster > base;
		file::kHandle< font::kBoundaries > bounds;

		mutable std::map< kColor, std::pair< file::kWrapped *, file::kWrapped * > > wreps;
		// TODO: mutable and const are hacks. find out some way to get rid of. semantics semantics.

	};

	class kFontHSLFilerbInitter : public zutil::kNFunctor {
	public:
		virtual void operator()();
		kFontHSLFilerbInitter( const std::string &boundname, const std::string &rendername, kFontHSLFilerb *target );

	private:
		std::string boundname;
		std::string rendername;

		kFontHSLFilerb *target;
	};

};

#endif
