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

#include "kFileBaseNull.h"

#include <iostream>
#include <assert.h>

namespace file {

	void kBaseNull::loadAll() {

		assert( state == kBase::EMPTY );

		state = READY;

	};

	void kBaseNull::beginProgressive() {

		assert( state == kBase::EMPTY );

		state = LOADING;
		progcur = 0;

	};

	void kBaseNull::continueProgressive( int steps ) {

		assert( state == kBase::LOADING );

		state = DONE;
		progcur = 1;

	};

	void kBaseNull::completeProgressive() {

		assert( state == kBase::DONE );

		state = READY;

	};

	void kBaseNull::cancelProgressive() {

		assert( state == kBase::LOADING );

		state = DONE;
		fritz = true;

	};

	void kBaseNull::unload() {
		
		assert( state == kBase::READY );

		state = EMPTY;
		fritz = false;

	};

	kBaseNull::kBaseNull() : kBase( "null" ) {
		progres = 1; };

		
	void kBaseNull::describe( std::ostream &ostr ) const {
		ostr << "NULL base file";
		kBase::chaindown( ostr ); };

	void kBaseNull::chaindown( std::ostream &ostr ) const {
		kBase::chaindown( ostr ); };

};

namespace null { // no way jose.

	// file::kBaseNull fileBase;

};
