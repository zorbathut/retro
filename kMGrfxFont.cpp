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

#include "kMGrfxFont.h"

#include "kFileManager.h"
#include "kFileWrappedNode.h"
#include "kGrfxFontNull.h"
#include "kGrfxFontLinkfile.h"
#include "kFileUtil.h"
#include "kFileBaseNull.h"

#include <fstream>
#include <string>
#include <limits>

namespace module {

	namespace fontFunctors {

		class kFLF : public file::kModuleStdfunctor< grfx::kFont > {
		public:
			virtual RVOID operator()( file::kManager *inp );
			kFLF( std::pair< const char *, file::kModule< grfx::kFont > * > ind );
		};

	};

	void kGrfxFont::specDat(
			std::string *spath,
					std::map<
						std::string,		// string: the extension
						zutil::kIOFunctor<	// the functor that creates the item that parses files
							zutil::kIOFunctor< RVOID, file::kManager * > *,	// the thing that parses files - returns nothing,
																	// takes a manager, returns by pointer for
																	// polymorphism
							std::pair< const char *, file::kModule< grfx::kFont > * >
																	// the file data - needs the filename and a pointer
																	// to what-to-register-with.
						> *				// and it's a pointer itself for polymorphism, again.
					> *assoc
			) {

		// oy. where to start?

		*spath = "font\\";

		(*assoc)[ "flf" ] = new zutil::kIOFunctorFactory< zutil::kIOFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< grfx::kFont > * >
			, fontFunctors::kFLF >; // ow.

	};

// TODO: wrap a little further down.
	grfx::kFont *kGrfxFont::createNull() {
		return &null::grfxFont; };

// TODO: dehack
using file::kModule;
	void kGrfxFont::describe( std::ostream &ostr ) const {
		ostr << "font module";
		kModule< grfx::kFont >::chaindown( ostr ); };

	void kGrfxFont::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-font module)";
		kModule< grfx::kFont >::chaindown( ostr ); };

	kGrfxFont font;

	namespace fontFunctors {

		RVOID kFLF::operator()( file::kManager *inp ) {
			std::ifstream ifs( fname.c_str() );
			std::string ffname;
			std::string bfname;
			ifs >> bfname >> ffname;
			grfx::kFontLinkfile *dat = new grfx::kFontLinkfile( fname );
			module->addWrapped( dat );
			module->addKey( file::extractFname( fname ), file::kHandle< grfx::kFont >( dat, dat ) );
			inp->addInitter( new grfx::kFontLinkfileInitter( bfname, ffname, dat ) );
			ifs.close();
			return RVOIDVAL;
		};

		kFLF::kFLF( std::pair< const char *, file::kModule< grfx::kFont > * > ind ) :
			file::kModuleStdfunctor< grfx::kFont >( ind ) { };

	};

};