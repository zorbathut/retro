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

#include "kFileWrappedNode.h"

#include "kFileManager.h"
#include "minmax.h"
#include "errlog.h"
#include "config.h"

// yes, there are TONS of defines in here. This is one of the most potentially buggy and glitchy areas, as well as
// one of the best to detect problems. Deal with it :P

namespace file {

	void kWrappedNode::activate() {
		++activatecount;
#if POSTDEBUGINFO
		g_errlog << "WRAPPED: (debug) Activating " << textdesc() << " (now at " << activatecount << ")" << std::endl;
#endif
		switch( file->getState() ) {
		case kGeneratable::EMPTY:
#if POSTDEBUGINFO
			g_errlog << "WRAPPED: (debug) Full-loading " << textdesc() << std::endl;
#endif
			file->loadAll();
#if EXTRACHECK
			if( file->getState() != kGeneratable::READY ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after activate-EMPTY, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			break;

		case kGeneratable::LOADING:
#if POSTDEBUGINFO
			g_errlog << "WRAPPED: (debug) Short-cutting LOADING " << textdesc() << std::endl;
#endif
			file->continueProgressive( file->getProgressiveResolution() - file->currentProgressive() );
#if EXTRACHECK
			if( file->getState() != kGeneratable::DONE ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after activate-LOADING, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			// dropthrough intentional

		case kGeneratable::DONE:
#if POSTDEBUGINFO
			g_errlog << "WRAPPED: (debug) Finishing and dehooking DONE " << textdesc() << std::endl;
#endif
			file->completeProgressive();
#if EXTRACHECK
			if( file->getState() != kGeneratable::READY ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after activate-DONE, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			g_manager->removeWrapped( this );
			break;

		case kGeneratable::READY:
			break;

		default:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during activation" << std::endl;
			tossException();
			break;
		}
	};

	void kWrappedNode::deactivate() {
		--activatecount;
#if POSTDEBUGINFO
		g_errlog << "WRAPPED: (debug) Deactivating " << textdesc() << " (now at " << activatecount << ")" << std::endl;
#endif
		if( activatecount < 0 ) {
			g_errlog << "WRAPPED: Negative activation count for " << textdesc() << std::endl;
			activatecount = 0;
			tossException(); }
		if( !activatecount ) {
			switch( file->getState() ) {
			case kGeneratable::EMPTY:
				g_errlog << "WRAPPED: Insane status for " << textdesc() << " during deactivation (already empty)" << std::endl;
				tossException();
				break;

			case kGeneratable::LOADING:
#if POSTDEBUGINFO
				g_errlog << "WRAPPED: (debug) Canceling " << textdesc() << std::endl;
#endif
				file->cancelProgressive();
#if EXTRACHECK
				if( file->getState() != kGeneratable::DONE ) {
					g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after deactivate-LOADING, is " << file->getStatetext() << std::endl;
					tossException();
				}
#endif				// dropthrough intentional

			case kGeneratable::DONE:
#if POSTDEBUGINFO
				g_errlog << "WRAPPED: (debug) Completing and dehooking " << textdesc() << std::endl;
#endif
				file->completeProgressive();
#if EXTRACHECK
				if( file->getState() != kGeneratable::READY ) {
					g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after deactivate-DONE, is " << file->getStatetext() << std::endl;
					tossException();
				}
#endif		
				g_manager->removeWrapped( this );
				// dropthrough also intentional

			case kGeneratable::READY:
#if POSTDEBUGINFO
				g_errlog << "WRAPPED: (debug) Deallocating " << textdesc() << std::endl;
#endif
				file->unload();
#if EXTRACHECK
				if( file->getState() != kGeneratable::EMPTY ) {
					g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after deactivate-READY, is " << file->getStatetext() << std::endl;
					tossException();
				}
#endif
				break;

			default:
				g_errlog << "WRAPPED: Insane status for " << textdesc() << " during deactivation" << std::endl;
				tossException();
				break;
			}
		}

	};

	void kWrappedNode::request( int ticks ) {
		activatecount++;
#if POSTDEBUGINFO
		g_errlog << "WRAPPED: (debug) Requesting " << textdesc() << " (now at " << activatecount << ")" << std::endl;
#endif
		switch( file->getState() ) {
		case kGeneratable::EMPTY:
#if POSTDEBUGINFO
			g_errlog << "WRAPPED: (debug) Beginning progressive and hooking " << textdesc() << std::endl;
#endif
			count = ticks;
			file->beginProgressive();
#if EXTRACHECK
			if( file->getState() != kGeneratable::LOADING ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after request-EMPTY, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			g_manager->activateWrapped( this );
			break;

		case kGeneratable::LOADING:
			count = zutil::zmin( ticks, count );
			break;

		case kGeneratable::DONE:
			break;

		case kGeneratable::READY:
			break;
			
		default:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during request" << std::endl;
			tossException();
			break;
		}
	};

	void kWrappedNode::urgent() {
#if POSTDEBUGINFO
		g_errlog << "WRAPPED: (debug) Urgent for " << textdesc() << " (now at " << activatecount << ")" << std::endl;
#endif
		switch( file->getState() ) {
		case kGeneratable::EMPTY:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during urgent, full-loading" << std::endl;
			tossException();
			file->loadAll();
			break;

		case kGeneratable::LOADING:
			file->continueProgressive( file->getProgressiveResolution() - file->currentProgressive() );
#if EXTRACHECK
			if( file->getState() != kGeneratable::DONE ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after urgent-LOADING, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			// dropthrough intentional

		case kGeneratable::DONE:
#if POSTDEBUGINFO
			g_errlog << "WRAPPED: (debug) Urgent short-cutting and dehooking " << textdesc() << std::endl;
#endif
			file->completeProgressive();
#if EXTRACHECK
			if( file->getState() != kGeneratable::READY ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after urgent-DONE, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			g_manager->removeWrapped( this );
			break;

		case kGeneratable::READY:
			break;

		default:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during urgent" << std::endl;
			tossException();
			break;
		}

	};


	void kWrappedNode::tick() {
		switch( file->getState() ) {
		case kGeneratable::EMPTY:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during tick (EMPTY, removing)" << std::endl;
			tossException();
			g_manager->removeWrapped( this );
			break;

		case kGeneratable::LOADING:
			if( count <= 0 )
				count = 1;
			file->continueProgressive( ( ( file->getProgressiveResolution() - file->currentProgressive() ) * 2 / count + 1 ) / 2 );
#if EXTRACHECK
			if( file->getState() != kGeneratable::LOADING && file->getState() != kGeneratable::DONE ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after tick-LOADING, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			count--;
			break;

		case kGeneratable::DONE:
#if POSTDEBUGINFO
			g_errlog << "WRAPPED: (debug) Completing progressive and dehooking " << textdesc() << std::endl;
#endif
			file->completeProgressive();
#if EXTRACHECK
			if( file->getState() != kGeneratable::READY ) {
				g_errlog << "ERRCHECK: WRAPPED: Inconsistent state of " << textdesc() << " after tick-DONE, is " << file->getStatetext() << std::endl;
				tossException();
			}
#endif
			g_manager->removeWrapped( this );
			break;

		case kGeneratable::READY:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during tick (READY, removing)" << std::endl;
			tossException();
			g_manager->removeWrapped( this );
			break;

		default:
			g_errlog << "WRAPPED: Insane status for " << textdesc() << " during tick (UNKNOWN)" << std::endl;
			tossException();
			break;
		}
	};

	kWrappedNode::kWrappedNode( kGeneratable *in_file ) : file( in_file ), activatecount( 0 ),
			count( 0 ) { };
	kWrappedNode::~kWrappedNode() {
		if( activatecount != 0 ) {
			g_errlog << "WRAPPED: Incomplete decoupling for " << textdesc() << " (still at " << activatecount << ")" << std::endl;
			tossException();
		}
		delete file;
	};

	void kWrappedNode::describe( std::ostream &ostr ) const {
		ostr << "wrappednode \"" << file->textdesc() << "\"";
		kWrapped::chaindown( ostr ); };

	void kWrappedNode::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-wrappednode \"" << file->textdesc() << "\")" << std::endl;
		kWrapped::chaindown( ostr ); };



};

