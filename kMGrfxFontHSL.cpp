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

#include "kMGrfxFontHSL.h"

#include "kFileManager.h"
#include "kFileWrappedNode.h"
#include "kGrfxFontHSLFilerb.h"
#include "kGrfxFontHSLNull.h"
#include "kFileUtil.h"
#include "kFileBaseNull.h"

#include <fstream>
#include <string>
#include <limits>

namespace module {

	namespace fontHSLFunctors {

		class kFLF : public file::kModuleStdfunctor< grfx::kFontHSL > {
		public:
			virtual RVOID operator()( file::kManager *inp );
			kFLF( std::pair< const char *, file::kModule< grfx::kFontHSL > * > ind );
		};

	};

	void kGrfxFontHSL::specDat(
			std::string *spath,
					std::map<
						std::string,		// string: the extension
						zutil::kIOFunctor<	// the functor that creates the item that parses files
							zutil::kIOFunctor< RVOID, file::kManager * > *,	// the thing that parses files - returns nothing,
																	// takes a manager, returns by pointer for
																	// polymorphism
							std::pair< const char *, file::kModule< grfx::kFontHSL > * >
																	// the file data - needs the filename and a pointer
																	// to what-to-register-with.
						> *				// and it's a pointer itself for polymorphism, again.
					> *assoc
			) {

		// oy. where to start?

		*spath = "font\\";

		(*assoc)[ "flf" ] = new zutil::kIOFunctorFactory< zutil::kIOFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< grfx::kFontHSL > * >
			, fontHSLFunctors::kFLF >; // ow.

	};

// TODO: wrap a little further down.
	grfx::kFontHSL *kGrfxFontHSL::createNull() {
		return &null::grfxFontHSL; };

// TODO: dehack
using file::kModule;
	void kGrfxFontHSL::describe( std::ostream &ostr ) const {
		ostr << "HSL font module";
		kModule< grfx::kFontHSL >::chaindown( ostr ); };

	void kGrfxFontHSL::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-HSL font module)";
		kModule< grfx::kFontHSL >::chaindown( ostr ); };

	kGrfxFontHSL fonthsl;

	namespace fontHSLFunctors {

		RVOID kFLF::operator()( file::kManager *inp ) {
			std::ifstream ifs( fname.c_str() );
			std::string ffname;
			std::string bfname;
			ifs >> bfname >> ffname;
			grfx::kFontHSLFilerb *dat = new grfx::kFontHSLFilerb( fname );
			module->addWrapped( dat );
			module->addKey( file::extractFname( fname ), file::kHandle< grfx::kFontHSL >( dat, dat ) );
			inp->addInitter( new grfx::kFontHSLFilerbInitter( bfname, ffname, dat ) );
			ifs.close();
			return RVOIDVAL;
		};

		kFLF::kFLF( std::pair< const char *, file::kModule< grfx::kFontHSL > * > ind ) :
			file::kModuleStdfunctor< grfx::kFontHSL >( ind ) { };

	};

};