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

#include "kMGrfxRenderableFinite.h"

#include "kFileManager.h"
#include "kFileWrappedNode.h"
#include "kGrfxRenderablePNG.h"
#include "kGrfxRenderableFiniteNull.h"
#include "kFileUtil.h"
#include "kFileBaseNull.h"

#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

namespace module {

	namespace finiteFunctors {

	};

	void kGrfxRenderableFinite::specDat(
			std::string *spath,
					std::map<
						std::string,		// string: the extension
						zutil::kIOFunctor<	// the functor that creates the item that parses files
							zutil::kIOFunctor< RVOID, file::kManager * > *,	// the thing that parses files - returns nothing,
																	// takes a manager, returns by pointer for
																	// polymorphism
							std::pair< const char *, file::kModule< grfx::kRenderableFinite > * >
																	// the file data - needs the filename and a pointer
																	// to what-to-register-with.
						> *				// and it's a pointer itself for polymorphism, again.
					> *assoc
			) {

		// oy. where to start?

		*spath = "grfx\\";

/*		(*assoc)[ "pln" ] = new zutil::kIOFunctorFactory< zutil::kIOFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< grfx::kRenderableFinite > * >
			, finiteFunctors::kPLN >; // ow.
		(*assoc)[ "png" ] = new zutil::kIOFunctorFactory< zutil::kIOFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< grfx::kRenderableFinite > * >
			, finiteFunctors::kPNG >; // ow.*/
		// okay, yes, that's NOT THE POINT DAMMIT

	};

// TODO: wrap a little further down.
	grfx::kRenderableFinite *kGrfxRenderableFinite::createNull() {
		return &null::grfxRenderableFinite; };

// TODO: dehack.
using file::kModule;
	void kGrfxRenderableFinite::describe( std::ostream &ostr ) const {
		ostr << "grfxRenderableFinite module (thrunch)";
		kModule< grfx::kRenderableFinite >::chaindown( ostr ); };

	void kGrfxRenderableFinite::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-grfxRenderableFinite module) (thrunch)";
		kModule< grfx::kRenderableFinite >::chaindown( ostr ); };

	kGrfxRenderableFinite finite;

	namespace finiteFunctors {

	};

};