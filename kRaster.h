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

#ifndef RETRO_KRASTER
#define RETRO_KRASTER

// semantics explanation: rasters are basically more complex pointers. Thus, things like
// rendering are considered const, despite the fact that they change the *contained*
// data. They don't change the *pointer*.

namespace grfx {

	class kRasterConst;
	class kRaster;

	class kLockedRead;
	class kLockedWrite;

}

#include <utility>
#include <vector>

#include "kRect.h"
#include "kColor.h"
#include "butility.h"
#include "kFunctor.h"
#include "kDescribable.h"

namespace grfx {

	class kRasterConst : private boost::noncopyable, public kDescribable {
	public:

		const kPoint< INT32 > &getDimensions() const;

		bool lock( kRect< INT32 > &bounds, kLockedRead *locked, bool suppressError = false ) const;

		void unlock( const kLockedRead *locked ) const;

		void addModifyNotification( const kRect< INT32 > &bounds, zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > *fptr ) const;
		void addDestructionNotification( zutil::kFunctor< RVOID, const kRasterConst * > *fptr ) const;

		kRasterConst( const kPoint< INT32 > &dim, const kColor *dat, INT32 pitch, bool owned = true );
		virtual ~kRasterConst();

		virtual void describe( std::ostream &ostr ) const;
	protected:  void chaindown( std::ostream &ostr ) const;

	protected:

		void notifyModification( const kRect< INT32 > &bounds ) const;
		
	private:

		kPoint< INT32 > dimension;
		union {
			const kColor *c;
			kColor *nc;
		} data;
		INT32 pitch;

		bool owned;

		mutable std::vector< zutil::kFunctor< bool, std::pair< const kRect< INT32 > *, const kRasterConst * > > * > changeNotifies;
		mutable std::vector< zutil::kFunctor< RVOID, const kRasterConst * > * > destructNotifies;

		// yes yes. this needs to be re-implemented :P
		bool disallowReadLock;
		mutable int readLockCount;

		kRasterConst();

		friend kRaster;

	}; // raster is defined as 32bit ARGB. Whether A matters or not depends on context.

	class kRaster : public kRasterConst {
	public:

		bool writeLock( kRect< INT32 > &bounds, kLockedWrite *locked, bool suppressError = false );

		void unlock( const kLockedRead *locked ) const;
		void unlock( const kLockedWrite *locked );

		kRaster( const kPoint< INT32 > &dim );
		kRaster( const kPoint< INT32 > &dim, kColor *dat, INT32 pitch, bool owned = true );

		virtual void describe( std::ostream &ostr ) const;
	protected:  void chaindown( std::ostream &ostr ) const;

	private:

		kColor *getData();

	};

	class kLockedRead {
	public:
		kRect< INT32 > bounds;
		const kColor *data;
		INT32 pitch;
	};

	class kLockedWrite {
	public:
		kRect< INT32 > bounds;
		kColor *data;
		INT32 pitch;
	};

};

#endif