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

#ifndef RETRO_KFILEMODULEHANDLE
#define RETRO_KFILEMODULEHANDLE

namespace file {

	template < typename kType > class kModuleHandle;

};

#include "kFileModule.h"
#include "kFileHandle.h"
#include "kFileBaseNull.h"
#include "kFileWrappedNode.h"

namespace file {

	template < typename kType > class kModuleHandle : public kModule< kHandle< kType > > {
	private:

		virtual void specDat(
				std::string *spath,
				std::map<
					std::string,		// string: the extension
					zutil::kFunctor<	// the functor that creates the item that parses files
						zutil::kFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
																// takes a manager, returns by pointer for
																// polymorphism
						std::pair< const char *, kModule< kHandle< kType > > * >
																// the file data - needs the filename and a pointer
																// to what-to-register-with.
					> *				// and it's a pointer itself for polymorphism, again.
				> *assoc
		) = 0;

		virtual kHandle< kType > createNull();

		virtual kType *createNullQuantity() = 0;

	protected:

		std::vector< file::kWrapped * > nullses;

	public:

		~kModuleHandle() {
			for( std::vector< file::kWrapped * >::iterator itr = nullses.begin(); itr < nullses.end(); ++itr )
				delete *itr;
		};

		virtual void describe( std::ostream &ostr ) const VAGUEDESC;
	protected:  void chaindown( std::ostream &ostr ) const;

	};

	template < typename kType > void kModuleHandle< kType >::describe( std::ostream &ostr ) const {
		ostr << "unidentified modulehandle";
		kModule< file::kHandle< kType > >::chaindown( ostr ); };
	template < typename kType > void kModuleHandle< kType >::chaindown( std::ostream &ostr ) const {
		kModule< file::kHandle< kType > >::chaindown( ostr ); };

	template < typename kType > kHandle< kType > kModuleHandle< kType >::createNull() {
		kWrapped *wr = new kWrappedNode( new kBaseNull() );
		nullses.push_back( wr );
		return kHandle< kType >( createNullQuantity(), wr ); };

};

#endif