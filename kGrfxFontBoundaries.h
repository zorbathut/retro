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

#ifndef RETRO_KGRFXFONTBOUNDARIES
#define RETRO_KGRFXFONTBOUNDARIES

namespace grfx {

	namespace font {

		class kBoundaries;

	}

};

#include "kFileBase.h"
#include "kRect.h"
#include "kDescribable.h"
#include "types.h"

#include <vector>
#include <algorithm>

namespace grfx {

	namespace font {

		class kBoundariesSpot {
		public:
			kRect< INT32 > bounds;
			kPoint< INT32 > offset;
			int spacing;

			kBoundariesSpot() { };
			kBoundariesSpot( const kRect< INT32 > &bounds, const kPoint< INT32 > &offset, int spacing );
				
		};

		class kBoundaries : public kDescribable {
		public:

			int getVoffs() const;
			const kBoundariesSpot &getBound( char c ) const;
			// boundary for placement, the point is x width and y offset-from-top (positive means shift down)

			kBoundaries();
			virtual ~kBoundaries();

			virtual void describe( std::ostream &ostr ) const;
		protected:  void chaindown( std::ostream &ostr ) const;

		protected:

			void setVoffs( int nv );
			void setup();
			void destruct();
			void set( unsigned char c, const kBoundariesSpot &dat );

		private:

			int vOffs;
			std::vector< std::pair< bool, kBoundariesSpot > > dat;

		};

	};

};

namespace null {

	extern grfx::font::kBoundaries fontboundaries;

};

#endif
