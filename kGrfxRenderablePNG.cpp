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

#include "kGrfxRenderablePNG.h"

#include <stdio.h>
#include <assert.h>

#include "minmax.h"
#include "errlog.h"

#include "libpng/png.h"

namespace grfx {

	static void user_error_fn( png_structp png_ptr, png_const_charp error_msg ) { throw 0; }

	void kRenderablePNG::loadAll() {

		assert( state == EMPTY );
		
		if( initpngread() ) {
			fritz = true;
			state = READY;
			progcur = progres;
			g_errlog << "instant initpngread failure in " << textdesc() << std::endl;
			return;
		}

		try {

			png_read_image( png_ptr, row_pointers );

		} catch( int ) {

			destructpngread();

			fritz = true;
			state = READY;
			progcur = progres;
			g_errlog << "png_read_image failure in " << textdesc() << std::endl;
			return;

		}

		destructpngread();

		fritz = false;
		state = READY;
		progcur = progres;

	};

	void kRenderablePNG::beginProgressive() {

		assert( state == EMPTY );

		if( initpngread() ) {
			fritz = true;
			state = DONE;
			progcur = progres;
			g_errlog << "progressive initpngread failure in " << textdesc() << std::endl;
			return;
		} else {
			state = LOADING;
		}

	};

	void kRenderablePNG::continueProgressive( int steps ) {

		assert( state == LOADING );

		steps = zutil::zmin( steps, progres - progcur );

		try {

			png_read_rows( png_ptr, row_pointers + progcur, NULL, steps );

		} catch( int ) {

			fritz = true;
			state = DONE;
			progcur = progres;
			g_errlog << "png_read_rows failure in " << textdesc() << std::endl;
			return;

		}

		progcur += steps;

		if( progcur == progres )
			state = DONE;

		return;

	};

	void kRenderablePNG::cancelProgressive() {

		assert( state == LOADING );

		fritz = true;
		state = DONE;
		progcur = progres;
		return;

	};

	void kRenderablePNG::completeProgressive() {

		assert( state == DONE );

		destructpngread();

		state = READY;
		return;

	};

	void kRenderablePNG::unload() {

		assert( state == READY );

		delete raster.getData();

		raster.setData( NULL );

		fritz = false;
		state = EMPTY;
		progcur = 0;

	};

	bool kRenderablePNG::preparepng() {

		static const int preroll = 8;
		png_byte header[ preroll ];

		fp = fopen( getFname().get(), "rb" );
		if( !fp ) {
			g_errlog << "file open failure in " << textdesc() << std::endl;
			return true;
		}

		fread( header, 1, preroll, fp );
		if( png_sig_cmp( header, 0, preroll ) ) {
			g_errlog << "PNG signature mismatch in " << textdesc() << std::endl;
			fclose( fp );
			return true;
		}

		png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, user_error_fn, NULL );
		if( !png_ptr ) {
			g_errlog << "PNG read failure (stage 1) in " << textdesc() << std::endl;
			fclose( fp );
			return true;
		}

		info_ptr = png_create_info_struct( png_ptr );
		if( !info_ptr ) {
			png_destroy_read_struct( &png_ptr, NULL, NULL );
			g_errlog << "PNG read failure (stage 2) in " << textdesc() << std::endl;
			fclose( fp );
			return true;
		}
	
		png_init_io( png_ptr, fp );
		png_set_sig_bytes( png_ptr, preroll );

		unsigned long width;
		unsigned long height;

		int colordepth;
		int colortype;

		png_read_info( png_ptr, info_ptr );
		png_get_IHDR( png_ptr, info_ptr, &width, &height, &colordepth, &colortype, NULL, NULL, NULL );

		raster.setDimensions( kPoint< INT32 >( width, height ) );
		progres = height;

		return false;

	};

	bool kRenderablePNG::initpngread() {

		if( preparepng() ) { // well, we're screwed, really.
			raster.setData( new kColor[ raster.getDimensions().x * raster.getDimensions().y ] );
			// todo: some unmistakable hash pattern.
			return true;
		}

		png_set_bgr( png_ptr );								// set to BGR
		png_set_filler( png_ptr, 0xff, PNG_FILLER_AFTER );	// add alpha padding if necessary
		png_set_expand( png_ptr );							// expand to 8bit depth if necessary
		png_set_gray_to_rgb( png_ptr );						// and colorize
		png_set_strip_16( png_ptr );						// and downsize to 8bit depth if necessary :P
		// todo: do these produce a speed hit if they're not necessary?
		// possibly add some switches so it's smarter about adding filters.

		png_read_update_info( png_ptr, info_ptr );

		kColor *data = new kColor[ raster.getDimensions().x * raster.getDimensions().y ];
		row_pointers = new png_byte*[ raster.getDimensions().y ];
		for( int i = 0; i < raster.getDimensions().y; i++ )
			row_pointers[ i ] = reinterpret_cast< png_byte * >( &data[ i * raster.getDimensions().x ] );

		raster.setData( data );
		raster.setPitch( raster.getDimensions().x );

		return false;

	};

	void kRenderablePNG::destructpngread() {

		delete [] row_pointers;
		png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
		fclose( fp );

		png_ptr = NULL;
		info_ptr = NULL;
		fp = NULL;
		row_pointers = NULL;

	};

	kRenderablePNG::kRenderablePNG( const char *fname ) : file::kBase( fname ), png_ptr( NULL ), info_ptr( NULL ),
			row_pointers( NULL ), fp( NULL ) {
		preparepng();
		destructpngread();
	};

	kRenderablePNG::~kRenderablePNG() {

		delete [] row_pointers;

		if( png_ptr || info_ptr )
			png_destroy_read_struct( &png_ptr, &info_ptr, NULL );

		if( fp )
			fclose( fp );

	};

	void kRenderablePNG::describe( std::ostream &ostr ) const {
		ostr << "renderablePNG";
		kRenderablePNG::chaindown( ostr ); };

	void kRenderablePNG::chaindown( std::ostream &ostr ) const {
		ostr << " (*final*-PNG)";
		kRenderableOwnedraster::chaindown( ostr );
		kBase::chaindown( ostr ); };

	kOutputtoken kRenderablePNG::textdesc() const {
		return kBase::textdesc(); };


};