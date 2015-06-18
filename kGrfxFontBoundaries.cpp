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

#include "kGrfxFontBoundaries.h"

#include "errlog.h"

namespace grfx {

	namespace font {

		kBoundariesSpot::kBoundariesSpot( const kRect< INT32 > &in_bounds, const kPoint< INT32 > &in_offset, int in_spacing ) :
			bounds( in_bounds ), offset( in_offset ), spacing( in_spacing ) { };

		int kBoundaries::getVoffs() const {
			return vOffs; }

		const kBoundariesSpot &kBoundaries::getBound( char c ) const {
			if( !dat[ c ].first )
				g_errlog << "Unknown boundary " << int(c) << " attempted for " << textdesc() << std::endl;
			return dat[ c ].second;
		};

		kBoundaries::kBoundaries() { };
		kBoundaries::~kBoundaries() { };
		
		void kBoundaries::describe( std::ostream &ostr ) const {
			ostr << "unidentified boundary";
			kDescribable::chaindown( ostr ); };

		void kBoundaries::chaindown( std::ostream &ostr ) const {
			kDescribable::chaindown( ostr ); };
 
		void kBoundaries::setVoffs( int nv ) {
			vOffs = nv; };

		void kBoundaries::setup() {
			dat.resize( 256 ); };
		void kBoundaries::destruct() {
			dat.clear(); };

		void kBoundaries::set( unsigned char c, const kBoundariesSpot &in_dat ) {
#if EXTRACHECK
			if( dat.size() != 256 ) {
				g_errlog << "ERRCHECK: FONTBOUND: attempted before setup" << std::endl;
				return;
			}
			if( c < 0 || c >= 256 ) {
				g_errlog << "ERRCHECK: FONTBOUND: " << int( c ) << " out of range" << std::endl;
				return;
			}
#endif
			dat[ c ].first = true;
			dat[ c ].second = in_dat; };

	};

};

namespace null {

	grfx::font::kBoundaries fontboundaries;

};
