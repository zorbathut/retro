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

#pragma warning( disable : 4786 )

#include "kRaster.h"

#include "errlog.h"
#include "minmax.h"

namespace grfx {
	
	// TODO: improve the whole locking system :P

	const kPoint< INT32 > &kRasterConst::getDimensions() const {
		return dimension; };

		// TODO: more testing
	bool kRasterConst::lock( kRect< INT32 > &bounds, kLockedRead *locked, bool suppressError ) const {
		if( disallowReadLock ) {
			if( !suppressError ) {
				g_errlog << "Failed readlock on " << textdesc() << " for " << bounds << std::endl;
			}
			return true;
		} else {
			readLockCount++;
			locked->bounds.ul.x = 0;
			locked->bounds.ul.y = 0;
			locked->bounds.br = dimension;
			locked->data = data.c;
			locked->pitch = pitch;
#if POSTDEBUGINFO
			g_errlog << "Succeeded readlock on " << textdesc() << " for " << bounds << " (got " << locked->bounds << ")" << std::endl;
#endif
			// yeah yeah.
			return false;
		}
	};

		// TODO: more testing
	void kRasterConst::unlock( const kLockedRead *locked ) const {
#if POSTDEBUGINFO
		g_errlog << "Read unlock on " << textdesc() << std::endl;
#endif
		readLockCount--; };
	// TODO: more errorchecking 'n stuff.

	void kRasterConst::addModifyNotification( const kRect< INT32 > &bounds, zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > *fptr ) const {
		changeNotifies.push_back( fptr ); };
			// and TOTALLY IGNORE bounds! Yeah yeah. I'm lazy right now. I'll fix it up later, if there's need.
			// For now, lazylazylazy! :)

	void kRasterConst::addDestructionNotification( zutil::kFunctor< RVOID, const kRasterConst * > *fptr ) const {
		destructNotifies.push_back( fptr ); };

	kRasterConst::kRasterConst( const kPoint< INT32 > &dim, const kColor *dat, INT32 in_pitch, bool in_owned ) :
			dimension( dim ), pitch( in_pitch ), owned( in_owned ), disallowReadLock( false ), readLockCount( 0 ) {
		data.c = dat; };

	kRasterConst::kRasterConst() :
			dimension( 0, 0 ), pitch( 0 ), owned( false ), disallowReadLock( false ), readLockCount( 0 ) {
		data.c = NULL; };

	kRasterConst::~kRasterConst() {
		if( owned )
			delete [] data.nc;
		std::vector< zutil::kFunctor< RVOID, const kRasterConst * > * >::iterator itr;
		for( itr = destructNotifies.begin(); itr != destructNotifies.end(); itr++ ) {
			(**itr)( this );
			delete *itr;
		};
		std::vector< zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > * >::iterator it2;
		for( it2 = changeNotifies.begin(); it2 != changeNotifies.end(); it2++ )
			delete *it2;
		// TODO: lock changes on traversal (high priority!)
	};

	void kRasterConst::notifyModification( const kRect< INT32 > &bounds ) const {
		std::vector< bool > mark;
		std::vector< zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > * >::iterator itr;
		std::pair< const kRect< INT32 > *, const kRasterConst * > parm( &bounds, this );
		for( itr = changeNotifies.begin(); itr != changeNotifies.end(); itr++ ) {
			mark.push_back( !(**itr)( parm ) );
		};
		std::vector< zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > * > alter;
		for( int i = 0; i < mark.size(); ++i ) {
			if( mark[ i ] )
				alter.push_back( changeNotifies[ i ] );
		}
		changeNotifies.swap( alter );
		// TODO: lock changes on traversal (high priority!)
	};

	void kRasterConst::describe( std::ostream &ostr ) const {
		ostr << "constraster";
		kRasterConst::chaindown( ostr ); }

	void kRasterConst::chaindown( std::ostream &ostr ) const {
		ostr << " (" << dimension << ", pitch " << pitch << ", " << ( !owned ? "un" : "" ) << "owned)";
		kDescribable::chaindown( ostr ); }

	// TODO: more testing
	bool kRaster::writeLock( kRect< INT32 > &bounds, kLockedWrite *locked, bool suppressError ) {
		if( disallowReadLock || readLockCount ) {
			if( !suppressError ) {
				g_errlog << "Failed writelock on " << textdesc() << " for " << bounds << std::endl;
			}
			return true;
		} else {
			disallowReadLock = true;
			locked->bounds.ul.x = 0;
			locked->bounds.ul.y = 0;
			locked->bounds.br = dimension;
			locked->data = data.nc;
			locked->pitch = pitch;
#if POSTDEBUGINFO
			g_errlog << "Succeeded writelock on " << textdesc() << " for " << bounds << " (got " << locked->bounds << ")" << std::endl;
#endif
			return false;
		}
	};

		// TODO: more testing
	void kRaster::unlock( const kLockedRead *locked ) const {
		kRaster::unlock( locked ); };

	void kRaster::unlock( const kLockedWrite *locked ) {
#if POSTDEBUGINFO
		g_errlog << "Write unlock on " << textdesc() << std::endl;
#endif
		disallowReadLock = false;
		notifyModification( locked->bounds ); };

	kRaster::kRaster( const kPoint< INT32 > &dim ) {
		dimension = dim;
		pitch = dim.x;
		owned = true;
		data.nc = new kColor[ dim.x * dim.y ];
	};

	kRaster::kRaster( const kPoint< INT32 > &dim, kColor *dat, INT32 in_pitch, bool in_owned ) {
		dimension = dim;
		pitch = in_pitch;
		owned = true;
		data.nc = dat;
	};

	void kRaster::describe( std::ostream &ostr ) const {
		ostr << "nonconstraster";
		kRasterConst::chaindown( ostr ); }

	void kRaster::chaindown( std::ostream &ostr ) const {
		kRasterConst::chaindown( ostr ); }

};
		
