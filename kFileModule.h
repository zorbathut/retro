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

#ifndef RETRO_KFILEMODULE
#define RETRO_KFILEMODULE

#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )

namespace file {

	template < typename kType > class kModule;

};

#include "kFileManager.h"
#include "kFunctor.h"
#include "butility.h"
#include "errlog.h"
#include "kDescribable.h"
#include "kFileHandle.h"

#include <map>
#include <io.h>
#include <string>
#include <algorithm>
#include <functional>

namespace file {			// notes: virtual isn't ideal here. The item's type should
							// be known at all times. However, it's easy to code, and
							// the only virtual functions are those that basically never
							// get called. So it's adequate.

	template < typename kType > class kModule : public boost::noncopyable, public kDescribable {
	public:

		kHandle< kType > get( const char *id );
		kHandle< kType > get( const std::string &id );
		void generate( kManager *kfm );	// should only be called once, and attaches

		void addKey( const std::string &id, const kHandle< kType > &hnd );
		void addWrapped( kWrapped *wrp );

		virtual ~kModule(); // might not be necessary, but it's safer this way.

		virtual void describe( std::ostream &ostr ) const VAGUEDESC;
	protected:  void chaindown( std::ostream &ostr ) const;

	private:

		virtual void specDat(
			std::string *spath,
			std::map<
				std::string,		// string: the extension
				zutil::kIOFunctor<	// the functor that creates the item that parses files
					zutil::kIOFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, kModule< kType > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *				// and it's a pointer itself for polymorphism, again.
			> *assoc
		) = 0;

		virtual kHandle< kType > createNull( void ) = 0;

	private:

		void stdGenerate(
			const std::string &path,
			const std::map<
				std::string,		// string: the extension
				zutil::kIOFunctor<	// the functor that creates the item that parses files
					zutil::kIOFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, kModule< kType > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *				// and it's a pointer itself for polymorphism, again.
			> &assoc,
			kManager *target
		);

		std::map< std::string, kHandle< kType > > data;

		std::vector< kWrapped * > wrps;

	};

	template< typename kType > class kModuleStdfunctor : public zutil::kIOFunctor< RVOID, file::kManager * > {
	public:
		virtual RVOID operator()( file::kManager *inp ) = 0;
		kModuleStdfunctor< kType >( std::pair< const char *, file::kModule< kType > * > ind );
	protected:
		std::string fname;
		file::kModule< kType > *module;
	}; // TODO: shift to IFunctor

	template < typename kType > kHandle< kType > kModule< kType >::get( const char *id ) {
		return get( std::string( id ) );
	};

	template < typename kType > kHandle< kType > kModule< kType >::get( const std::string &id ) {
		std::map< std::string, kHandle< kType > >::const_iterator itr;
		itr = data.find( id );
		if( itr == data.end() ) {
			g_errlog << "MODULE: Couldn't find item \"" << id << "\" in \"" << textdesc() << "\"" << std::endl;
			data[ id ] = createNull();
			return data[ id ];
		}
		return itr->second;
	};

	template < typename kType > void kModule< kType >::generate( file::kManager *kfm ) {

		std::string path;

		std::map<
			std::string,		// string: the extension
			zutil::kIOFunctor<	// the functor that creates the item that parses files
				zutil::kIOFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
														// takes a manager, returns by pointer for
														// polymorphism
				std::pair< const char *, kModule< kType > * >
														// the file data - needs the filename and a pointer
														// to what-to-register-with.
			> *				// and it's a pointer itself for polymorphism, again.
		> assoc;

		std::map<
			std::string,
			zutil::kIOFunctor<
				zutil::kIOFunctor< RVOID, kManager * > *,
				std::pair< const char *, kModule< kType > * >
			> *
		>::iterator itr; // my mind, it's exploding again!
		
		specDat( &path, &assoc );
		stdGenerate( path, assoc, kfm );

		for( itr = assoc.begin(); itr != assoc.end(); ++itr )
			delete itr->second;

	};

	template < typename kType > void kModule< kType >::addKey( const std::string &id, const kHandle< kType > &hnd ) {
		std::map< std::string, kHandle< kType > >::const_iterator itr;
		itr = data.find( id );
		if( itr != data.end() ) {
			g_errlog << "MODULE: \"" << id << "\" already exists as " << itr->second->textdesc() << ", replaced with " << hnd->textdesc() << std::endl;
		} else {
			g_errlog << "MODULE: \"" << textdesc() << "\" adding \"" << hnd.getHeld()->textdesc() << "\" as \"" << id << "\"" << std::endl;
		}
		data[ id ] = hnd;
	};

	template < typename kType > void kModule< kType >::addWrapped( kWrapped *wrp ) {
		wrps.push_back( wrp ); };

	template < typename kType > void kModule< kType >::stdGenerate(
			const std::string &path,
			const std::map<
				std::string,		// string: the extension
				zutil::kIOFunctor<	// the functor that creates the item that parses files
					zutil::kIOFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, kModule< kType > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *				// and it's a pointer itself for polymorphism, again.
			> &assoc,
			kManager *target
		) {

		std::string pth = path;
		pth += "*.*";

		_finddata_t data;
		long handle = _findfirst( pth.c_str(), &data );

		if( handle != -1 ) {
			do {
				if( !( data.attrib & _A_SUBDIR ) ) {
					std::string tstr = std::string( strrchr( data.name, '.' ) + 1 );
					std::transform( tstr.begin(), tstr.end(), tstr.begin(), std::ptr_fun< int, int >( tolower ) );
					std::map<
						std::string,
						zutil::kIOFunctor<
							zutil::kIOFunctor< RVOID, kManager * > *,
							std::pair< const char *, kModule< kType > * >
						> *
					>::const_iterator itr = assoc.find( tstr );
					if( itr == assoc.end() ) {
						g_errlog << "MODULE: Unidentified file " << path << data.name << " located in file scan" << std::endl;
					} else {
						std::string fn = path;
						fn += data.name;
						target->addLoader(
							( *itr->second )(
								std::pair< const char *, kModule< kType > * >(
									fn.c_str(),
									this
								)
							)
						);
					}
				}
			} while( !_findnext( handle, &data ) );
		}

		_findclose( handle );

	};

	template < typename kType > void kModule< kType >::describe( std::ostream &ostr ) const {
		ostr << "unidentified module";
		kDescribable::chaindown( ostr ); };
	template < typename kType > void kModule< kType >::chaindown( std::ostream &ostr ) const {
		kDescribable::chaindown( ostr ); };

	template < typename kType > kModule< kType >::~kModule() {
		for( std::vector< kWrapped * >::iterator itr = wrps.begin(); itr != wrps.end(); itr++ )
			delete *itr;
	};

	template< typename kType > 
	kModuleStdfunctor< kType >::kModuleStdfunctor< kType >( std::pair< const char *, file::kModule< kType > * > ind ) :
		fname( ind.first ), module( ind.second ) { } ;

};

#endif