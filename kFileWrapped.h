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


#ifndef TIMESPACE_KFILEWRAPPED
#define TIMESPACE_KFILEWRAPPED

namespace file {

	class kWrapped;

}

#include "kFileBase.h"
#include "butility.h"

namespace file {

	class kWrapped : public boost::noncopyable {
	private:

		kBase *file; // better derive from kBase . . .

		int activatecount;
		int count;

	public:

		void init();

		void activate();
		void deactivate();
		void request( int ticks );

		void tick();

		void deinit();

		kWrapped( kBase *file );
		~kWrapped();

	};

};

/*
namespace file {

	template < class kHeld > class kWrapped;

}

#include "kFileWrapinterface.h"

#include "minmax.h"
#include "errlog.h"

namespace file {

	template < class kHeld > class kWrapped : public kWrapinterface {
	private:

		kHeld *file; // better derive from kBase . . .

		int activatecount;
		int count;

	public:

		virtual void init();

		kHandle< kHeld > getHandle();

		virtual void activate();
		virtual void deactivate();
		virtual void request( int ticks );

		virtual void tick();

		virtual void deinit();

		kWrapped( kHeld *in_file ); // OWNS this, and will delete it on deconstruction.
		virtual ~kWrapped();

	};

	template < class kHeld > void kWrapped< kHeld >::init() {
		file->init(); };

	template < class kHeld > void kWrapped< kHeld >::deinit() {
		file->deinit(); };

	template < class kHeld > kHandle< kHeld > kWrapped< kHeld >::getHandle() {
		return kHandle< kHeld >( file, this ); };

	template < class kHeld > void kWrapped< kHeld >::activate() {
		activatecount++;
		switch( file->getState() ) {
		case kHeld::EMPTY:
			file->loadAll();
			break;

		case kHeld::LOADING:
			file->continueProgressive( file->getProgressiveResolution() - file->currentProgressive() );
			// dropthrough intentional

		case kHeld::DONE:
			file->completeProgressive();
			break;

		case kHeld::READY:
			break;
		}
	};

	template < class kHeld > void kWrapped< kHeld >::deactivate() {
		activatecount--;
		count = 0;
	};

	template < class kHeld > void kWrapped< kHeld >::request( int ticks ) {
		switch( file->getState() ) {
		case kHeld::EMPTY:
			count = ticks;
			file->beginProgressive();
			break;

		case kHeld::LOADING:
			count = zutil::zmin( ticks, count );
			break;

		case kHeld::DONE:
			break;

		case kHeld::READY:
			count = ticks;	// anti-idle.
			break;
		}
	};

	template < class kHeld > void kWrapped< kHeld >::tick() {
		switch( file->getState() ) {
		case kHeld::EMPTY:
			break;

		case kHeld::LOADING:
			if( count <= 0 )
				count = 1;
			file->continueProgressive( ( file->getProgressiveResolution() - file->currentProgressive() ) / count );
			count--;
			break;

		case kHeld::DONE:
			file->completeProgressive();
			break;

		case kHeld::READY:
			if( count-- < -( tickspersec * 10 ) && !activatecount ) // todo: add idletime
				file->unload();
			break;

		default:
			g_errlog << "Insane status for wrapped " << file->textdesc() << std::endl;
			break;
		}
	};

	template < class kHeld > kWrapped< kHeld >::kWrapped( kHeld *in_file ) : file( in_file ), activatecount( 0 ),
			count( 0 ) { };
	template < class kHeld > kWrapped< kHeld >::~kWrapped() {
		if( activatecount != 0 )
			g_errlog << "Incomplete decoupling for " << file->textdesc() << std::endl;
		delete file;
	};

};
*/
#endif