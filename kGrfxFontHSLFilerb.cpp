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

#include "kGrfxFontHSLFilerb.h"

#include "kGrfxFontLinkhandle.h"
#include "kGrfxRenderableRasterHSL.h"

#include "kMGrfxFontBoundaries.h"
#include "kMGrfxRenderableRaster.h"

namespace grfx {

	void kFontHSLFilerb::init( const std::string &boundname, const std::string &rendername ) {
		bounds = module::fontbounds.get( boundname );
		base = module::raster.get( rendername );
	};

	void kFontHSLFilerb::activate() {
		base.activate();
		bounds.activate(); };

	void kFontHSLFilerb::deactivate() {
		base.deactivate();
		bounds.deactivate(); };

	void kFontHSLFilerb::request( int ticks ) {
		base.request( ticks );
		bounds.request( ticks ); };

	void kFontHSLFilerb::urgent() {
		base.urgent();
		bounds.urgent(); };

	kFontHSLFilerb::kFontHSLFilerb( const std::string &fname ) :
		file::kWrappedFile( fname ) { };
	kFontHSLFilerb::~kFontHSLFilerb() {
		std::map< kColor, std::pair< file::kWrapped *, file::kWrapped * > >::iterator itr;
		for( itr = wreps.begin(); itr != wreps.end(); itr++ ) {
			delete itr->second.first;
			delete itr->second.second; };
		// TODO: status message?
	};

	file::kHandle< kFont > kFontHSLFilerb::genChroma( kColor val ) const {
#if POSTDEBUGINFO
		g_errlog << "FONTHSL: Generating " << val << " for " << descr( this ) << std::endl;
#endif

		kRenderableRasterHSL *hsl = new kRenderableRasterHSL( base, val );
		file::kWrappedNode *hslnod = new file::kWrappedNode( hsl );
		file::kHandle< kRenderableRaster > hslhnd( hsl, hslnod );

		kFontLinkhandle *kflh = new kFontLinkhandle( hslhnd, bounds );
		file::kHandle< kFont > fhnd( kflh, kflh );

		wreps[ val ].first = hslnod;
		wreps[ val ].second = kflh;

		return fhnd;
		
	};

	void kFontHSLFilerb::wipeChroma( kColor val ) const {
#if POSTDEBUGINFO
		g_errlog << "FONTHSL: Wiping " << val << " for " << descr( this ) << std::endl;
#endif

		delete wreps[ val ].first;
		delete wreps[ val ].second;
		wreps.erase( wreps.find( val ) ); };
		// todo: errchecking?

	void kFontHSLFilerbInitter::operator()() {
		target->init( boundname, rendername ); };

	kFontHSLFilerbInitter::kFontHSLFilerbInitter( const std::string &in_boundname, const std::string &in_rendername,
		kFontHSLFilerb *in_target ) : boundname( in_boundname ), rendername( in_rendername ), target( in_target ) { };

	// TODO: dehackify.
using file::kWrappedFile;
	void kFontHSLFilerb::describe( std::ostream &ostr ) const {
		ostr << "HSLfontlink";
		kFontHSL::chaindown( ostr );
		kWrappedFile::chaindown( ostr ); };

	void kFontHSLFilerb::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-HSLfont)";
		kFontHSL::chaindown( ostr );
		kWrappedFile::chaindown( ostr ); };

};
