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

#include "kFileManager.h"

namespace file {

	void kManager::prepare( void ) {
		curl = loador.begin(); };		// this is certainly minimal.

	int kManager::getCycles( void ) const { return loador.size(); };
	int kManager::getCurCycle( void ) const { return curl - loador.begin(); };
	void kManager::doCycles( int count ) {
		while( count > 0 ) {
			(*curl)->go( this );
			++curl;
			--count;
		};
	};

	void kManager::complete( void ) {
		for( std::vector< zutil::kFunctor< RVOID, kManager * > * >::iterator aitr = loador.begin(); aitr != loador.end(); ++aitr )
			delete *aitr;
		loador.clear();
		for( std::vector< file::kWrapped * >::iterator bitr = interfaces.begin(); bitr != interfaces.end(); ++bitr )
			(*bitr)->init();
		for( std::vector< file::kBase * >::iterator citr = raws.begin(); citr != raws.end(); ++citr )
			(*citr)->init();
		completed = true;
	};

	void kManager::tick( void ) {
		for( std::vector< kWrapped * >::iterator itr = interfaces.begin(); itr != interfaces.end(); ++itr )
			(*itr)->tick();
	};

	kManager::kManager() : completed( false ) { };
	kManager::~kManager() {
		if( !completed )
			complete();
		std::vector< kWrapped * >::iterator iitr;
		std::vector< kBase * >::iterator ritr;
		for( iitr = interfaces.begin(); iitr != interfaces.end(); ++iitr )
			(*iitr)->deinit();
		for( ritr = raws.begin(); ritr != raws.end(); ++ritr )
			(*ritr)->deinit();
		for( iitr = interfaces.begin(); iitr != interfaces.end(); ++iitr )
			delete *iitr;
		for( ritr = raws.begin(); ritr != raws.end(); ++ritr )
			delete *ritr;
	};

	void kManager::addInterface( file::kWrapped *nter ) {
		interfaces.push_back( nter ); };
	void kManager::addRaw( file::kBase *base ) {
		raws.push_back( base ); };

	void kManager::addLoader( zutil::kFunctor< RVOID, kManager * > *ldr ) {
		loador.push_back( ldr ); };

	bool kManager::isComplete() const {
		return completed; };

};

file::kManager *g_manager;