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

#include "kMGrfxRenderableRaster.h"

#include "kFileManager.h"
#include "kFileWrappedNode.h"
#include "kGrfxRenderablePNG.h"
#include "kGrfxRenderableRasterNull.h"
#include "kFileUtil.h"
#include "kFileBaseNull.h"
#include "kMGrfxRenderableFinite.h"

#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

namespace module {

	namespace rasterFunctors {

		class kPNG : public file::kModuleStdfunctor< grfx::kRenderableRaster > {
		public:
			virtual RVOID operator()( file::kManager *inp );
			kPNG( std::pair< const char *, file::kModule< grfx::kRenderableRaster > * > ind );
		};

	};

	void kGrfxRenderableRaster::specDat(
			std::string *spath,
					std::map<
						std::string,		// string: the extension
						zutil::kIOFunctor<	// the functor that creates the item that parses files
							zutil::kIOFunctor< RVOID, file::kManager * > *,	// the thing that parses files - returns nothing,
																	// takes a manager, returns by pointer for
																	// polymorphism
							std::pair< const char *, file::kModule< grfx::kRenderableRaster > * >
																	// the file data - needs the filename and a pointer
																	// to what-to-register-with.
						> *				// and it's a pointer itself for polymorphism, again.
					> *assoc
			) {

		// oy. where to start?

		*spath = "rstr\\";

		(*assoc)[ "png" ] = new zutil::kIOFunctorFactory< zutil::kIOFunctor< RVOID, file::kManager * > *, std::pair< const char *, file::kModule< grfx::kRenderableRaster > * >
			, rasterFunctors::kPNG >; // ow.

	};

// TODO: wrap a little further down.
	grfx::kRenderableRaster *kGrfxRenderableRaster::createNull() {
		return &null::grfxRenderableRaster; };

// TODO: check to make sure it's not being double-called
	void kGrfxRenderableRaster::linkup() {
		if( !linker )
			linker = new zutil::kIOFunctorFullPoly< file::kHandle< grfx::kRenderableFinite >, const std::string &, file::kModule< grfx::kRenderableRaster >, file::kHandle< grfx::kRenderableRaster > >( this, &file::kModule< grfx::kRenderableRaster >::getExisting );
		finite.attachChild( linker ); };

	kGrfxRenderableRaster::kGrfxRenderableRaster() : linker( NULL ) { };
	kGrfxRenderableRaster::~kGrfxRenderableRaster() {
		delete linker; };

// TODO: dehack.
using file::kModule;
	void kGrfxRenderableRaster::describe( std::ostream &ostr ) const {
		ostr << "grfxRenderableRaster module (thrunch)";
		kModule< grfx::kRenderableRaster >::chaindown( ostr ); };

	void kGrfxRenderableRaster::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-grfxRenderableRaster module) (thrunch)";
		kModule< grfx::kRenderableRaster >::chaindown( ostr ); };

	kGrfxRenderableRaster raster;

	namespace rasterFunctors {

		RVOID kPNG::operator()( file::kManager *inp ) {
			grfx::kRenderablePNG *dat = new grfx::kRenderablePNG( fname.c_str() );
			file::kWrapped *wrp = new file::kWrappedNode( dat );
			module->addWrapped( wrp );
			module->addKey( file::extractFname( fname ), file::kHandle< grfx::kRenderableRaster >( dat, wrp ) );
			return RVOIDVAL;
		};

		kPNG::kPNG( std::pair< const char *, file::kModule< grfx::kRenderableRaster > * > ind ) :
			file::kModuleStdfunctor< grfx::kRenderableRaster >( ind ) { };

	};

};