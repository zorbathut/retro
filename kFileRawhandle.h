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

#ifndef TIMESPACE_KFILERAWHANDLE
#define TIMESPACE_KFILERAWHANDLE

namespace file {

	template < class kHeld > class kRawhandle;

}

#include "kFileBase.h"
#include "null.h"

namespace file {

	template < class kHeld > class kRawhandle {
	public:		// these have to be public. But I'm not happy about that one bit.
				// They *should* be private, with this a template friend of itself.
		const kHeld *held;
		kBase *base;

	public:

		const kHeld *getHeld() const { return held; };
		bool isEmpty() const { return !(held || base); };
		void clear() { held = NULL; base = NULL; };

		void init() const { base->init(); };

		void loadAll() const { base->loadAll(); };

		int  getProgressiveResolution() const { return base->getProgressiveResolution(); };
		void beginProgressive() const { base->beginProgressive(); };
		int  currentProgressive() const { return base->currentProgressive(); };
		void continueProgressive( int steps ) const { base->continueProgressive( steps ); };
		void completeProgressive() const { base->completeProgressive(); };
		void cancelProgressive() const { base->cancelProgressive(); };
		kBase::eState getState() const { return base->getState(); };
		bool getFritz() const { return base->getFritz(); };

		void unload() const { base->unload(); };		// let's do the delegation mambo!

		kRawhandle() : held( NULL ), base( NULL ) { };
		template < class kAlter >
			kRawhandle( const kRawhandle< kAlter > &in ) : held( in.held ), base( in.base ) { };
		kRawhandle( const kHeld *in_held, kBase *in_base ) : held( in_held ), base( in_base ) { };

		template < class kAlter >
			const kRawhandle< kHeld > &operator=( const kRawhandle< kAlter > &in ) { held = in.held; base = in.base; return *this; };
		~kRawhandle() { }; // right . . .

		const kHeld &operator*() const { return *held; };
		const kHeld *operator->() const { return held; };

	};

};

#endif