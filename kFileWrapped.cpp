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

// TODO: improve logging, make it a little easier to read

#include "kFileWrapped.h"
#include "kFileManager.h"

#include "minmax.h"
#include "errlog.h"

#include "config.h"

namespace file {

	void kWrapped::init() {
		file->init(); };

	void kWrapped::deinit() {
		file->deinit(); };

	void kWrapped::activate() {
		++activatecount;
		g_errlog << "Activating \"" << textdesc() << "\" (now at " << activatecount << ")" << std::endl;
		switch( file->getState() ) {
		case kBase::EMPTY:
			g_errlog << "Full-loading \"" << textdesc() << "\"" << std::endl;
			file->loadAll();
			break;

		case kBase::LOADING:
			file->continueProgressive( file->getProgressiveResolution() - file->currentProgressive() );
			// dropthrough intentional

		case kBase::DONE:
			file->completeProgressive();
			g_errlog << "Short-cutting and dehooking \"" << textdesc() << "\"" << std::endl;
			g_manager->removeWrapped( this );
			break;

		case kBase::READY:
			break;

		default:
			g_errlog << "Insane status for \"" << textdesc() << "\" during activation" << std::endl;
			break;
		}
	};

	void kWrapped::deactivate() {
		--activatecount;
		g_errlog << "Deactivating \"" << textdesc() << "\" (now at " << activatecount << ")" << std::endl;
		if( activatecount < 0 ) {
			g_errlog << "Negative activation count for \"" << textdesc() << "\"" << std::endl;
			activatecount = 0; }
		if( !activatecount ) {
			switch( file->getState() ) {
			case kBase::EMPTY:
				g_errlog << "Insane status for \"" << textdesc() << "\" during deactivation (already empty)" << std::endl;
				break;

			case kBase::LOADING:
				file->cancelProgressive();
				// dropthrough intentional

			case kBase::DONE:
				file->completeProgressive();
				g_manager->removeWrapped( this );
				// dropthrough also intentional

			case kBase::READY:
				g_errlog << "Deallocating \"" << textdesc() << "\"" << std::endl;
				file->unload();
				break;

			default:
				g_errlog << "Insane status for \"" << textdesc() << "\" during deactivation" << std::endl;
				break;
			}
		}

	};

	void kWrapped::request( int ticks ) {
		activatecount++;
		g_errlog << "Requesting \"" << textdesc() << "\" (now at " << activatecount << ")" << std::endl;
		switch( file->getState() ) {
		case kBase::EMPTY:
			g_errlog << "Beginning progressive and hooking \"" << textdesc() << "\"" << std::endl;
			count = ticks;
			file->beginProgressive();
			g_manager->activateWrapped( this );
			break;

		case kBase::LOADING:
			count = zutil::zmin( ticks, count );
			break;

		case kBase::DONE:
			break;

		case kBase::READY:
			break;
			
		default:
			g_errlog << "Insane status for \"" << textdesc() << "\" during request" << std::endl;
			break;
		}
	};

	void kWrapped::tick() {
		switch( file->getState() ) {
		case kBase::EMPTY:
			g_errlog << "Insane status for \"" << textdesc() << "\" during tick (EMPTY, removing)" << std::endl;
			g_manager->removeWrapped( this );
			break;

		case kBase::LOADING:
			if( count <= 0 )
				count = 1;
			file->continueProgressive( ( file->getProgressiveResolution() - file->currentProgressive() ) / count );
			count--;
			break;

		case kBase::DONE:
			g_errlog << "Completing progressive and dehooking \"" << textdesc() << "\"" << std::endl;
			file->completeProgressive();
			g_manager->removeWrapped( this );
			break;

		case kBase::READY:
			g_errlog << "Insane status for \"" << textdesc() << "\" during tick (READY, removing)" << std::endl;
			g_manager->removeWrapped( this );
			break;

		default:
			g_errlog << "Insane status for \"" << textdesc() << "\" during tick (UNKNOWN)" << std::endl;
			break;
		}
	};

	kWrapped::kWrapped( kBase *in_file ) : file( in_file ), activatecount( 0 ),
			count( 0 ) { };
	kWrapped::~kWrapped() {
		if( activatecount != 0 )
			g_errlog << "Incomplete decoupling for \"" << textdesc() << "\" (still at " << activatecount << ")" << std::endl;
		delete file;
	};

	void kWrapped::describe( std::ostream &ostr ) const {
		ostr << "wrapped " << file->textdesc();
		kDescribable::chaindown( ostr ); };

	void kWrapped::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-wrapped " << file->textdesc() << ")" << std::endl;
		kDescribable::chaindown( ostr ); };



};

