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

#include "kFileWrapped.h"

#include "minmax.h"
#include "errlog.h"

#include "config.h"

namespace file {

	void kWrapped::init() {
		file->init(); };

	void kWrapped::deinit() {
		file->deinit(); };

	void kWrapped::activate() {
		activatecount++;
		switch( file->getState() ) {
		case kBase::EMPTY:
			file->loadAll();
			break;

		case kBase::LOADING:
			file->continueProgressive( file->getProgressiveResolution() - file->currentProgressive() );
			// dropthrough intentional

		case kBase::DONE:
			file->completeProgressive();
			break;

		case kBase::READY:
			break;

		default:
			g_errlog << "Insane status for wrapped " << file->textdesc() << " during activation" << std::endl;
			break;
		}
	};

	void kWrapped::deactivate() {
		if( --activatecount < 0 )
			g_errlog << "Negative activation count for " << file->textdesc() << std::endl;
		count = 0;
	};

	void kWrapped::request( int ticks ) {
		switch( file->getState() ) {
		case kBase::EMPTY:
			count = ticks;
			file->beginProgressive();
			break;

		case kBase::LOADING:
			count = zutil::zmin( ticks, count );
			break;

		case kBase::DONE:
			break;

		case kBase::READY:
			count = ticks;	// anti-idle.
			break;
			
		default:
			g_errlog << "Insane status for wrapped " << file->textdesc() << " during request" << std::endl;
			break;
		}
	};

	void kWrapped::tick() {
		switch( file->getState() ) {
		case kBase::EMPTY:
			break;

		case kBase::LOADING:
			if( count <= 0 )
				count = 1;
			file->continueProgressive( ( file->getProgressiveResolution() - file->currentProgressive() ) / count );
			count--;
			break;

		case kBase::DONE:
			file->completeProgressive();
			break;

		case kBase::READY:
			if( count-- < -( tickspersec * 10 ) && !activatecount ) // todo: add idletime
				file->unload();
			break;

		default:
			g_errlog << "Insane status for wrapped " << file->textdesc() << " during tick" << std::endl;
			break;
		}
	};

	kWrapped::kWrapped( kBase *in_file ) : file( in_file ), activatecount( 0 ),
			count( 0 ) { };
	kWrapped::~kWrapped() {
		if( activatecount != 0 )
			g_errlog << "Incomplete decoupling for " << file->textdesc() << std::endl;
		delete file;
	};

};

