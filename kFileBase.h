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

#ifndef TIMESPACE_KFILEBASE
#define TIMESPACE_KFILEBASE

namespace file {

	class kBase;

}

#include "kString.h"
#include "butility.h"
#include "kDescribable.h"

namespace file {

	class kBase : private boost::noncopyable, public kDescribable {
	public:

		enum eState { EMPTY, LOADING, DONE, READY };

		virtual void init();		// init stuff: guaranteed to be after construction, before anything
									// else, and might happen more than once. call other inits during
									// init if you need their initted data. And do what you can during
									// construction.

		virtual void loadAll() = 0;

		        int    getProgressiveResolution() const;
		virtual void   beginProgressive() = 0;
		        int    currentProgressive() const;
		virtual void   continueProgressive( int steps ) = 0;
		virtual void   completeProgressive() = 0;
		virtual void   cancelProgressive() = 0;
		        eState getState() const;
		        bool   getFritz() const; // is this data fritzed?

		virtual void unload() = 0;

		virtual void deinit();		// preparation for destruction - nothing's *been* destroyed yet,
									// but get ready for it, since there's no assigned order for
									// destruction. Also may be called more than once, though I can't
									// imagine why (yet).

		kBase( const char *fname );	// better init progres . . . well, not my problem.
		virtual ~kBase();

		virtual void describe( std::ostream &ostr ) const VAGUEDESC;
	protected:  void chaindown( std::ostream &ostr ) const;

	protected:

		int progres;
		int progcur;
		eState state;
		bool fritz;

		const zutil::kString &getFname() const;

	private:

		zutil::kString fname;

	};

};

#endif