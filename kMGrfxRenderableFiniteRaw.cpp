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

#include "kMGrfxRenderableFiniteRaw.h"
#include "kFileBaseNull.h"
#include "kGrfxRenderablePNG.h"
#include "kGrfxRenderableFiniteNull.h"

#include <limits>
#include <fstream>
#include <string>

namespace module {

	namespace rawfiniteFunctors {

		class kPLN : public zutil::kFunctor< RVOID, file::kManager * > {
		public:
			virtual RVOID operator()( file::kManager *inp );
			kPLN( std::pair< const char *, file::kModule< file::kRawhandle< grfx::kRenderableFinite > > * > ind );
		private:
			std::string fname;
			file::kModule< file::kRawhandle< grfx::kRenderableFinite > > *module;
		};

	};

	void kGrfxRenderableFiniteRaw::specDat(
			std::string *spath,
			std::map<
				std::string,		// string: the extension
				zutil::kFunctor<	// the functor that creates the item that parses files
					zutil::kFunctor< RVOID, file::kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, file::kModule< file::kRawhandle< grfx::kRenderableFinite > > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *				// and it's a pointer itself for polymorphism, again.
			> *assoc
		) {

		// oy. where to start?

		*spath = "sourcegrfx\\";

//		*nullitem = file::kRawhandle< grfx::kRenderableFinite >( &null::grfxRenderableFinite, &null::fileBase );

		(*assoc)[ "pln" ] = new zutil::kFunctorFactory< zutil::kFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< file::kRawhandle< grfx::kRenderableFinite > > * >
			, rawfiniteFunctors::kPLN >; // ow.

	};

	file::kRawhandle< grfx::kRenderableFinite > kGrfxRenderableFiniteRaw::createNull( void ) {
		file::kBaseNull *nul = new file::kBaseNull();
		nulls.push_back( nul );
		return file::kRawhandle< grfx::kRenderableFinite >( &null::grfxRenderableFinite, nul );
	};

	kGrfxRenderableFiniteRaw::~kGrfxRenderableFiniteRaw() {
		for( std::vector< file::kBase * >::iterator itr = nulls.begin(); itr < nulls.end(); ++itr )
			delete *itr;
	};	// sigh.

	void kGrfxRenderableFiniteRaw::describe( std::ostream &ostr ) const {
		ostr << "grfxRenderableFinite rawmodule (thrunch)";
		/*file::kModuleRawhandle< grfx::kRenderableFinite >::chaindown( ostr );*/ };
		// TODO: fix. upgrade.

	void kGrfxRenderableFiniteRaw::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-grfxRenderableFinite rawmodule) (thrunch)";
		/*file::kModuleRawhandle< grfx::kRenderableFinite >::chaindown( ostr );*/ };
		// TODO: fix. upgrade.

	kGrfxRenderableFiniteRaw rawfinite;

	namespace rawfiniteFunctors {

		RVOID kPLN::operator()( file::kManager *inp ) {
			std::ifstream ifs( fname.c_str() );
			int count;
			std::string fname;
			std::string ident;
			ifs >> count;
			ifs.ignore( std::numeric_limits< int >::max(), '\n' );
			for( int i = 0; i < count; ++i ) {
				std::getline( ifs, ident );
				std::getline( ifs, fname );		// TODO: work out better filename tracking!
				grfx::kRenderablePNG *dat = new grfx::kRenderablePNG( fname.c_str() );
				inp->addRaw( dat );
				module->add( ident.c_str(), file::kRawhandle< grfx::kRenderableFinite >( dat, dat ) );
			}
			ifs.close();
			return RVOIDVAL;
		};


		kPLN::kPLN( std::pair< const char *, file::kModule< file::kRawhandle< grfx::kRenderableFinite > > * > ind ) :
				fname( ind.first ),
				module( ind.second )
			{ };

	};

};
