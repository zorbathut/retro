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

#include "kFileBase.h"

#include <iostream>
#include <map>

namespace file {

	class kGMap {

		std::map< kGeneratable::eState, std::string > mep;

		std::string fail;

	public:
		kGMap() {
			mep[ kGeneratable::EMPTY ] = "EMPTY";
			mep[ kGeneratable::LOADING ] = "LOADING";
			mep[ kGeneratable::DONE ] = "DONE";
			mep[ kGeneratable::READY ] = "READY";

			fail = "*UNKNOWN*";
		};

		const std::string &get( kGeneratable::eState state ) const {
			std::map< kGeneratable::eState, std::string >::const_iterator fnd = mep.find( state );
			if( fnd == mep.end() )
				return fail;
			return fnd->second;
		};

	};

	const kGMap kgmap;

	int kGeneratable::getProgressiveResolution() const { return progres; };
    int kGeneratable::currentProgressive() const { return progcur; };
	kGeneratable::eState kGeneratable::getState() const { return state; };
	const std::string &kGeneratable::getStatetext() const {
		return kgmap.get( state ); };

    bool kGeneratable::getFritz() const { return fritz; };

	kGeneratable::kGeneratable() : state( EMPTY ), fritz( false ), progres( 0 ), progcur( 0 ) { };
	kGeneratable::~kGeneratable() { };

	void kGeneratable::describe( std::ostream &ostr ) const {
		ostr << "unidentified generatable";
		kDescribable::chaindown( ostr ); };

	void kGeneratable::chaindown( std::ostream &ostr ) const {
		kDescribable::chaindown( ostr ); };

}
