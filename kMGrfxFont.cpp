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
#include "kFileWrapped.h"
#include "kGrfxFontFile.h"
#include "kGrfxFontNull.h"

#include <fstream>
#include <string>
#include <limits>

namespace module {

	namespace fontFunctors {

		class kFGF : public zutil::kFunctor< RVOID, file::kManager * > {
		public:
			virtual RVOID operator()( file::kManager *inp );
			kFGF( std::pair< const char *, file::kModule< file::kHandle< grfx::kFont > > * > ind );
		private:
			zutil::kString fname;
			file::kModule< file::kHandle< grfx::kFont > > *module;
		};

	};

	void kGrfxFont::specDat(
			zutil::kString *spath,
					std::map<
						zutil::kString,		// string: the extension
						zutil::kFunctor<	// the functor that creates the item that parses files
							zutil::kFunctor< RVOID, file::kManager * > *,	// the thing that parses files - returns nothing,
																	// takes a manager, returns by pointer for
																	// polymorphism
							std::pair< const char *, file::kModule< file::kHandle< grfx::kFont > > * >
																	// the file data - needs the filename and a pointer
																	// to what-to-register-with.
						> *,				// and it's a pointer itself for polymorphism, again.
						zutil::kString::case_insensitive_lessthan			// sorted case-insensitive.
					> *assoc
			) {

		// oy. where to start?

		*spath = "font\\";

		(*assoc)[ zutil::kString( "fgf" ) ] = new zutil::kFunctorFactory< zutil::kFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< file::kHandle< grfx::kFont > > * >
			, fontFunctors::kFGF >; // ow.

	};

	grfx::kFont *kGrfxFont::createNullQuantity() {
		return &null::grfxFont; };

	kGrfxFont font;

	namespace fontFunctors {

		RVOID kFGF::operator()( file::kManager *inp ) {
			std::ifstream ifs( fname.get() );
			int count;
			std::string curfname;
			std::string infname;
			std::string ident;
			ifs >> count;
			ifs.ignore( std::numeric_limits< int >::max(), '\n' );
			for( int i = 0; i < count; ++i ) {
				std::getline( ifs, ident );
				std::getline( ifs, infname );
				curfname = fname.get();
				curfname += ":";
				curfname += i;		// TODO: fix
				grfx::kFontFile *dat = new grfx::kFontFile( curfname.c_str(), infname.c_str() );
				file::kWrapped *wrp = new file::kWrapped( dat );
				inp->addInterface( wrp );
				module->add( ident.c_str(), file::kHandle< grfx::kFontFile >( dat, wrp ) );
			}
			ifs.close();
			return RVOIDVAL;
		};


		kFGF::kFGF( std::pair< const char *, file::kModule< file::kHandle< grfx::kFont > > * > ind ) :
				fname( ind.first ),
				module( ind.second )
			{ };

	};

};