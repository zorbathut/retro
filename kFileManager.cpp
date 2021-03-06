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

#include "errlog.h"
#include <algorithm>
#include "config.h"

namespace file {

	void kManager::prepare( void ) {
		curl = loador.begin(); };		// this is certainly minimal.

	int kManager::getCycles( void ) const { return loador.size(); };
	int kManager::getCurCycle( void ) const {
		return std::distance(
			loador.begin(),
			static_cast< std::vector< zutil::kIOFunctor< RVOID, kManager * > * >::const_iterator >( curl )
		);
	};

	void kManager::doCycles( int count ) {
		while( count > 0 ) {
			(*curl)->go( this );
			++curl;
			--count;
		};
	};

	void kManager::complete( void ) {
		for( std::vector< zutil::kIOFunctor< RVOID, kManager * > * >::iterator aitr = loador.begin(); aitr != loador.end(); ++aitr )
			delete *aitr;
		loador.clear();
		for( std::vector< zutil::kNFunctor * >::iterator bitr = inits.begin(); bitr != inits.end(); bitr++ ) {
			(**bitr)();
			delete *bitr;
		};
		completed = true;
	};

	void kManager::tick( void ) {
		for( std::vector< kWrappedNode * >::iterator titr = active.begin(); titr != active.end(); ++titr )
			(*titr)->tick();
		wipeRemovals();

	};

	void kManager::wipeRemovals() {

		for( std::vector< kWrappedNode * >::iterator ritr = removals.begin(); ritr != removals.end(); ++ritr ) {
			std::vector< kWrappedNode * >::iterator find = std::find( active.begin(), active.end(), *ritr );
			if( find == active.end() )
				g_errlog << "GMANAGER: Couldn't find \"" << (*ritr)->textdesc() << "\" in activated list" << std::endl;
			  else {
#if POSTDEBUGINFO
				g_errlog << "GMANAGER: (debug) Removing \"" << (*ritr)->textdesc() << "\" from activated list" << std::endl;
#endif
				active.erase( find );
			}
		}
		removals.clear();

	};

	kManager::kManager() : completed( false ) { };
	kManager::~kManager() {
		if( !completed )
			complete();
	};

	void kManager::addLoader( zutil::kIOFunctor< RVOID, kManager * > *ldr ) {
		loador.push_back( ldr ); };

	void kManager::addInitter( zutil::kNFunctor *ini ) {
		inits.push_back( ini ); };

	bool kManager::isComplete() const {
		return completed; };

	void kManager::activateWrapped( file::kWrappedNode *wrp ) {
#if EXTRACHECK
		if( std::find( active.begin(), active.end(), wrp ) != active.end() ) {
			g_errlog << "ERRCHECK: GMANAGER: Already found \"" << wrp->textdesc() << "\" in activated list" << std::endl;
			return;
		}
#endif
#if POSTDEBUGINFO
		g_errlog << "GMANAGER: (debug) Adding \"" << wrp->textdesc() << "\" to activated list" << std::endl;
#endif
		active.push_back( wrp );
	};

	void kManager::removeWrapped( file::kWrappedNode *wrp ) {
#if POSTDEBUGINFO
		g_errlog << "GMANAGER: (debug) Adding removal for \"" << wrp->textdesc() << "\"" << std::endl;
#endif
		removals.push_back( wrp );
	};

};

file::kManager *g_manager;