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

#ifndef RETRO_KGRFXRENDERABLEPNG
#define RETRO_KGRFXRENDERABLEPNG

namespace grfx {

	class kRenderablePNG;

};

#include "kGrfxRenderableOwnedraster.h"
#include "kFileBase.h"

// png typedefs so I can get away with not includeing png.h, which is big and stuff
struct png_struct_def;
typedef png_struct_def png_struct;
struct png_info_struct;
typedef png_info_struct png_info;
typedef unsigned char png_byte;

// FILE typedef so I can get away with not includeing stdio.h, which is also big and stuff
struct _iobuf;
typedef _iobuf FILE;

namespace grfx {

	class kRenderablePNG : public kRenderableOwnedraster, public file::kBase {
	public:

		virtual void loadAll();

		virtual void beginProgressive();
		virtual void continueProgressive( int steps );	// return true on error
		virtual void cancelProgressive();
		virtual void completeProgressive();

		virtual void unload();

		kRenderablePNG( const char *fname );
		~kRenderablePNG();

		virtual void describe( std::ostream &ostr ) const;
	protected:  void chaindown( std::ostream &ostr ) const;
	public:
		kOutputtoken textdesc() const;	// activate de-ambiguification ray!
										// it's ugly and I don't like it. sigh.
										// make it better somehow? (but how?)
										// global function? is virtual inheritance as
										// icky when you're not using any data?
										// shazbot. :(
										// TODO: fix.

	private:

		// PNG-read stuff. Figure out some way to avoid the overhead in the future. (png-struct repository?)
		png_struct *png_ptr;
		png_info *info_ptr;
		png_byte **row_pointers;
		FILE *fp;

		bool preparepng();	// returns dimensions
		bool initpngread();
		void destructpngread();

	};

};

#endif*/