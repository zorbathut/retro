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

#ifndef TIMESPACE_KFILEHANDLE
#define TIMESPACE_KFILEHANDLE

namespace file {

	template < class kHeld > class kHandle;

}

#include "kFileWrapped.h"
#include "null.h"

namespace file {

	template < class kHeld > class kHandle {
	public:		// these have to be public. But I'm not happy about that one bit.
				// They *should* be private, with this a template friend of itself.
		const kHeld *held;
		kWrapped *wrp;

	public:

		const kHeld *getHeld() const { return held; };
		bool isEmpty() const { return !(held || wrp); };
		void clear() { held = NULL; wrp = NULL; };

		void init() const { wrp->init(); };

		void activate() const { wrp->activate(); };	// can you say "delegation"?
		void deactivate() const { wrp->deactivate(); };
		void request( int ticks ) const { wrp->request( ticks ); }; // De-le-ga-tion.

		kHandle() : held( NULL ), wrp( NULL ) { };
		template < class kAlter >
			kHandle( const kHandle< kAlter > &in ) : held( in.held ), wrp( in.wrp ) { };
		kHandle( const kHeld *in_held, kWrapped *in_wrp ) : held( in_held ), wrp( in_wrp ) { };

		template < class kAlter >
			const kHandle< kHeld > &operator=( const kHandle< kAlter > &in ) { held = in.held; wrp = in.wrp; return *this; };
		~kHandle() { }; // right . . .

		const kHeld &operator*() const { return *held; };
		const kHeld *operator->() const { return held; };

	};

};

#endif