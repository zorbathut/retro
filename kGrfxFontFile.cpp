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

#include "kGrfxFontFile.h"

#include "kMGrfxRenderableFiniteRaw.h"
#include <fstream>
#include <string>

namespace grfx {

	int kFontFile::getVerticalOffset() const {
		return fnt->getDimensions().y;
	};

	void kFontFile::renderTextTo( kWritable *writ, const char *text, const kPoint< INT32 > &loc ) const {
		const unsigned char *rt = reinterpret_cast< const unsigned char * >( text );
		kPoint< INT32 > dim = fnt->getDimensions();
		kPoint< INT32 > cloc = loc;
		dim.x /= 256; // we can only hope . . .
		while( *rt ) {
			fnt->renderPartTo(
				writ,
				cloc,
				kRect< INT32 > (
					kPoint< INT32 >( dim.x * *rt, 0 ),
					kPoint< INT32 >( dim.x * ( *rt + 1 ), dim.y )
				)
			);
			cloc.x += dim.x;
			rt++;
		};		
	};

	void kFontFile::init() {
		if( fnt.isEmpty() ) {
			fnt = module::rawfinite.get( fontname.get() );
			fnt.init();
			progres = fnt.getProgressiveResolution();
		};
	};

	void kFontFile::loadAll() {
		fnt.loadAll();
		progcur = fnt.currentProgressive();
		state = fnt.getState();
		fritz = fnt.getFritz();
	};

	void kFontFile::beginProgressive() {
		fnt.beginProgressive();
		progcur = fnt.currentProgressive();
		state = fnt.getState();
		fritz = fnt.getFritz();
	};

	void kFontFile::continueProgressive( int steps ) {
		fnt.continueProgressive( steps );
		progcur = fnt.currentProgressive();
		state = fnt.getState();
		fritz = fnt.getFritz();
	};

	void kFontFile::cancelProgressive() {
		fnt.cancelProgressive();
		progcur = fnt.currentProgressive();
		state = fnt.getState();
		fritz = fnt.getFritz();
	};

	void kFontFile::completeProgressive() {
		fnt.completeProgressive();
		progcur = fnt.currentProgressive();
		state = fnt.getState();
		fritz = fnt.getFritz();
	};

	void kFontFile::unload() {
		fnt.unload();
		progcur = fnt.currentProgressive();
		state = fnt.getState();
		fritz = fnt.getFritz();
	};								// bleah. I should make that a "resynch" function.

	void kFontFile::deinit() {
		if( !fnt.isEmpty() ) {
			switch( fnt.getState() ) {
			case file::kBase::LOADING:
				fnt.cancelProgressive();

			case file::kBase::DONE:
				fnt.completeProgressive();

			case file::kBase::READY:
				fnt.unload();

			case file::kBase::EMPTY:		// all dropthroughs intentional.
				break;
			}
//			g_manager->returnRawRenderableFinite( fontname.get() );
			fnt.clear();
		}
	}

	kFontFile::kFontFile( const char *fname, const char *fntname ) : kBase( fname ), fontname( fntname ) { };

	void kFontFile::describe( std::ostream &ostr ) const {
		ostr << "File font referencing \"" << fontname.get() << "\"";
		kFont::chaindown( ostr ); }

	void kFontFile::chaindown( std::ostream &ostr ) const {
		ostr << " (fontfile, \"" << fontname.get() << "\")";
		kFont::chaindown( ostr ); }

};
