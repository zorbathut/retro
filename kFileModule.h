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

namespace file {

	template < class kType > class kModule;

};


#include "kFileManager.h"
#include "kString.h"
#include <map>
#include "kFunctor.h"
#include "butility.h"
#include <io.h>
#include "errlog.h"
#include "kDescribable.h"

namespace file {			// notes: virtual isn't ideal here. The item's type should
							// be known at all times. However, it's easy to code, and
							// the only virtual functions are those that basically never
							// get called. So it's adequate.

	template < class kType > class kModule : public boost::noncopyable, public kDescribable {
	public:

		kType get( const char *id );
		kType get( const zutil::kString &id );
		void generate( file::kManager *kfm );	// should only be called once, and attaches

		void add( const char *id, const kType &hnd );

		virtual ~kModule(); // might not be necessary, but it's safer this way.

		virtual void describe( std::ostream &ostr ) const VAGUEDESC;
	protected:  void chaindown( std::ostream &ostr ) const;

	private:

		virtual void specDat(
			zutil::kString *spath,
			std::map<
				zutil::kString,		// string: the extension
				zutil::kFunctor<	// the functor that creates the item that parses files
					zutil::kFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, kModule< kType > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *,				// and it's a pointer itself for polymorphism, again.
				zutil::kString::case_insensitive_lessthan			// sorted case-insensitive.
			> *assoc
		) = 0;

		virtual kType createNull( void ) = 0;

	private:

		void stdGenerate(
			const char *path,
			const std::map<
				zutil::kString,		// string: the extension
				zutil::kFunctor<	// the functor that creates the item that parses files
					zutil::kFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, kModule< kType > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *,				// and it's a pointer itself for polymorphism, again.
				zutil::kString::case_insensitive_lessthan			// sorted case-insensitive.
			> &assoc,
			kManager *target
		);

		std::map< zutil::kString, kType > data;

	};

	template < class kType > kType kModule< kType >::get( const char *id ) {
		return get( zutil::kString( id ) );
	};

	template < class kType > kType kModule< kType >::get( const zutil::kString &id ) {
		std::map< zutil::kString, kType >::const_iterator itr;
		itr = data.find( id );
		if( itr == data.end() ) {
			g_errlog << "MODULE: Couldn't find item \"" << id.get() << "\" in \"" << textdesc() << "\"" << std::endl;
			data[ id ] = createNull();
			return data[ id ];
		}
		return itr->second;
	};

	template < class kType > void kModule< kType >::generate( file::kManager *kfm ) {

		zutil::kString path;

		std::map<
			zutil::kString,		// string: the extension
			zutil::kFunctor<	// the functor that creates the item that parses files
				zutil::kFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
														// takes a manager, returns by pointer for
														// polymorphism
				std::pair< const char *, kModule< kType > * >
														// the file data - needs the filename and a pointer
														// to what-to-register-with.
			> *,				// and it's a pointer itself for polymorphism, again.
			zutil::kString::case_insensitive_lessthan			// sorted case-insensitive.
		> assoc;

		std::map<
			zutil::kString,
			zutil::kFunctor<
				zutil::kFunctor< RVOID, kManager * > *,
				std::pair< const char *, kModule< kType > * >
			> *,
			zutil::kString::case_insensitive_lessthan
		>::iterator itr; // my mind, it's exploding again!
		
		specDat( &path, &assoc );
		stdGenerate( path.get(), assoc, kfm );

		for( itr = assoc.begin(); itr != assoc.end(); ++itr )
			delete itr->second;

	};

	template < class kType > void kModule< kType >::add( const char *id, const kType &hnd ) {
		std::map< zutil::kString, kType >::const_iterator itr;
		zutil::kString tmp( id );
		itr = data.find( tmp );
		if( itr != data.end() ) {
			g_errlog << "MODULE: \"" << id << "\" already exists as " << itr->second->textdesc() << ", replaced with " << hnd->textdesc() << std::endl;
		} else {
			g_errlog << "MODULE: \"" << textdesc() << "\" adding \"" << hnd.getHeld()->textdesc() << "\" as \"" << id << "\"" << std::endl;
		}
		data[ tmp ] = hnd;
	};

	template < class kType > void kModule< kType >::stdGenerate(
			const char *path,
			const std::map<
				zutil::kString,		// string: the extension
				zutil::kFunctor<	// the functor that creates the item that parses files
					zutil::kFunctor< RVOID, kManager * > *,	// the thing that parses files - returns nothing,
															// takes a manager, returns by pointer for
															// polymorphism
					std::pair< const char *, kModule< kType > * >
															// the file data - needs the filename and a pointer
															// to what-to-register-with.
				> *,				// and it's a pointer itself for polymorphism, again.
				zutil::kString::case_insensitive_lessthan			// sorted case-insensitive.
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
					zutil::kString tstr = zutil::kString( strrchr( data.name, '.' ) + 1 );
					std::map<
						zutil::kString,
						zutil::kFunctor<
							zutil::kFunctor< RVOID, kManager * > *,
							std::pair< const char *, kModule< kType > * >
						> *,
						zutil::kString::case_insensitive_lessthan
					>::const_iterator itr = assoc.find( tstr );
					if( itr == assoc.end() ) {
						g_errlog << "MODULE: Unidentified file " << data.name << " located in file scan" << std::endl;
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

	template < class kType > void kModule< kType >::describe( std::ostream &ostr ) const {
		ostr << "unidentified module";
		kDescribable::chaindown( ostr ); };
	template < class kType > void kModule< kType >::chaindown( std::ostream &ostr ) const {
		kDescribable::chaindown( ostr ); };

	template < class kType > kModule< kType >::~kModule() { };

};

#endif