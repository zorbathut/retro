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

#include "kGrfxFontBoundariesFile.h"

#include "minmax.h"

#include <fstream>

namespace grfx {

	namespace font {

		void kBoundariesFile::loadAll() {

			int c;
			kBoundariesSpot dat;
			int offs;

			std::ifstream ifs( getFname().c_str() );
			setup();

			ifs >> offs;
			
			setVoffs( offs );

			while( ifs >> c >> dat.bounds.ul.x >> dat.bounds.ul.y >> dat.bounds.br.x >> dat.bounds.br.y >> dat.offset.x >> dat.offset.y >> dat.spacing ) {
				set( c, dat );
			};

			state = READY;

		};

		void kBoundariesFile::unload() {
			destruct();

			state = EMPTY;
		
		};

		kBoundariesFile::kBoundariesFile( const std::string &fname ) :
			file::kBaseOneshot( fname ) { };
		kBoundariesFile::~kBoundariesFile() { };

		// TODO: dehack.
using file::kBaseOneshot;
		void kBoundariesFile::describe( std::ostream &ostr ) const {
			ostr << "boundaryfile";
			kBaseOneshot::chaindown( ostr ); };

		void kBoundariesFile::chaindown( std::ostream &ostr ) const {
			kBaseOneshot::chaindown( ostr ); };

	};

};
