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

#ifndef RETRO_KFILEMANAGER
#define RETRO_KFILEMANAGER

namespace file {

	class kManager;

}

#include "kFunctor.h"
#include "types.h"
#include "butility.h"

#include "kFileWrappedNode.h"
#include "kFileBase.h"

#include <vector>

namespace file {

	class kManager : private boost::noncopyable {
	public:

		void prepare( void );

		int getCycles( void ) const;
		int getCurCycle( void ) const;
		void doCycles( int count );

		void complete( void );

		void tick( void );

		kManager();		// yep, you can make more than one. Not recommended tho. Like that'll stop ya.
		~kManager();

		void addLoader( zutil::kIOFunctor< RVOID, kManager * > *ldr );
		void addInitter( zutil::kNFunctor *ini );

		bool isComplete() const;

		void activateWrapped( file::kWrappedNode *wrp );
		void removeWrapped( file::kWrappedNode *wrp );

	private:

		void wipeRemovals();

		std::vector< kWrappedNode * > active;
		std::vector< kWrappedNode * > removals;

		std::vector< zutil::kIOFunctor< RVOID, kManager * > * > loador;
		std::vector< zutil::kIOFunctor< RVOID, kManager * > * >::iterator curl;

		std::vector< zutil::kNFunctor * > inits;

		bool completed;

	};

};

extern file::kManager *g_manager;

#endif
