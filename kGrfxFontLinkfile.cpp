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

#include "kGrfxFontLinkfile.h"

#include "kMGrfxFontBoundaries.h"
#include "kMGrfxRenderableFinite.h"

namespace grfx {

	const font::kBoundaries &kFontLinkfile::getBounds() const {
		return *bounds; };
	const kRenderable &kFontLinkfile::getRenderable() const {
		return *renderable; };

	void kFontLinkfile::init( const std::string &boundname, const std::string &rendername ) {
		bounds = module::fontbounds.get( boundname );
		renderable = module::finite.get( rendername );
	};

	void kFontLinkfile::activate() {
		bounds.activate();
		renderable.activate();
	};

	void kFontLinkfile::deactivate() {
		bounds.deactivate();
		renderable.deactivate();
	};

	void kFontLinkfile::request( int ticks ) {
		bounds.request( ticks );
		renderable.request( ticks );
	};

	void kFontLinkfile::urgent() {
		bounds.urgent();
		renderable.urgent();
	};

	kFontLinkfile::kFontLinkfile( const std::string &filename ) : file::kWrappedFile( filename ) { };
	kFontLinkfile::~kFontLinkfile() { };

// TODO: dehack
using file::kWrappedFile;
	void kFontLinkfile::describe( std::ostream &ostr ) const {
		ostr << "fontlink";
		kWrappedFile::chaindown( ostr );
		kFontRenderable::chaindown( ostr ); }

	void kFontLinkfile::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-fontlink)";
		kWrappedFile::chaindown( ostr );
		kFontRenderable::chaindown( ostr ); }

	void kFontLinkfileInitter::operator()() {
		target->init( boundname, rendername ); };

	kFontLinkfileInitter::kFontLinkfileInitter( const std::string &in_boundname, const std::string &in_rendername,
		kFontLinkfile *in_target ) : boundname( in_boundname ), rendername( in_rendername ), target( in_target ) { };

};
