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

#include "kGrfxFontHSL.h"

#include "errlog.h"

namespace grfx {

	// TODO: more errorchecking with flags
	// TODO: more logging
	file::kHandle< kFont > kFontHSL::getChroma( kColor val ) const {
#if POSTDEBUGINFO
		g_errlog << "FONTHSL: " << textdesc() << " had " << val << " acquired" << std::endl;
#endif
		if( hnend[ val ].first++ )
			return hnend[ val ].second;
		return hnend[ val ].second = genChroma( val );
	};

	void kFontHSL::dropChroma( kColor val ) const {
#if POSTDEBUGINFO
		g_errlog << "FONTHSL: " << textdesc() << " had " << val << " released" << std::endl;
#endif
		if( !--hnend[ val ].first ) {
			wipeChroma( val );
			hnend.erase( hnend.find( val ) );
		}
	};

	kFontHSL::kFontHSL() { };
	kFontHSL::~kFontHSL() {
		std::map< kColor, std::pair< int, file::kHandle< kFont > > >::iterator itr;
		for( itr = hnend.begin(); itr != hnend.end(); itr++ ) {
			if( itr->second.first )
				g_errlog << "FONTHSL: Chroma " << itr->first << " on " << textdesc() << " not properly emptied, still at " << itr->second.first << std::endl;
		}	// pity there's nothing I can do about it at this point.
	};

	void kFontHSL::describe( std::ostream &ostr ) const {
		ostr << "unidentified FontHSL";
		kDescribable::chaindown( ostr ); };

	void kFontHSL::chaindown( std::ostream &ostr ) const {
		kDescribable::chaindown( ostr ); };

};
