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

#include "kInterface.h"
#include "kMainShellStackhead.h"
#include "kMainShellMenu.h"
#include "kGrfxFont.h"
#include "errlog.h"
#include "kGrfxRenderablePNG.h"
#include "kMGrfxRenderableFinite.h"
#include "kMGrfxRenderableFiniteRaw.h"
#include "kFileManager.h"
#include "kMGrfxFont.h"

bool initManager( kInterface *inter ) {		// return 1 on failure

	grfx::kRenderablePNG bg( "hardcode\\loadingbg.png" );
	grfx::kRenderablePNG bar( "hardcode\\loadingbar.png" );
	bg.loadAll();
	bar.loadAll();
	// todo: ignore error bits

	grfx::kWritable *target = inter->lockBuffer( grfx::kColor( 0x00000000 ) );
	bg.renderTo( target, ( target->getDimensions() - bg.getDimensions() ) / 2 );
	inter->unlockBuffer( target );

	module::finite.generate( g_manager );
	module::rawfinite.generate( g_manager );
	module::font.generate( g_manager );
	g_manager->prepare();

	while( g_manager->getCurCycle() != g_manager->getCycles() ) {
		target = inter->lockBuffer( grfx::kColor( 0x00000000 ) );
		kPoint< INT32 > pos = ( target->getDimensions() - bg.getDimensions() ) / 2;
		bg.renderTo( target, pos );
		pos.x += 120;
		pos.y += 120 + 360;
		int curp = ( g_manager->getCurCycle() * 360 ) / g_manager->getCycles();
		pos.y -= curp;
		bar.renderPartTo( target, pos, kPoint< INT32 >( 0, 360 - curp ), bar.getDimensions() );
		inter->unlockBuffer( target );
		g_manager->doCycles( 1 );
	}

	g_manager->complete();

	bg.unload();
	bar.unload();

	return false;

};

// given here: errlog is prepared. No more. No less.
void gameMain( kInterface *inter ) {

	file::kManager manager;
	g_manager = &manager;

	if( initManager( inter ) )
		return;

	kMainShellStackhead mainstack( new kMainShellMenu );

	INT64 freq = inter->getCounterFreq();
	INT64 curframe = inter->getCounterPos();
	INT64 lastsecond = inter->getCounterPos();
	int skipped = 0;

	freq /= tickspersec;	// how many ticks per, well, tick?

	int done = 0;
	int outof = 0;
	int outofnext = 0;

	int curl = 0;

	bool skip = false;
	bool skipnext = false;

	file::kHandle< grfx::kFont > fnt;

	while( !inter->shutDown() && mainstack.ready() ) {

		if( fnt.isEmpty() ) {
			fnt = module::font.get( "small" );
			fnt.activate(); }

		mainstack.clockTick();
		
		g_manager->tick();

		outofnext++;

		if( curframe + freq >= inter->getCounterPos() || skipped >= 14 ) {

			if( skipped >= 14 ) {
				curframe = inter->getCounterPos();
				skipnext = true; };

			while( curframe >= inter->getCounterPos() );
			// todo: sleep here

			char buf[ 8 ];
			sprintf( buf, "%2d/%2d", curl, outof );
			if( skip )
				strcat( buf, " S" );

			grfx::kWritable *it = inter->lockBuffer( grfx::kColor( 0xffffffff ) );
			mainstack.renderFrame( it );
			fnt->renderText( it, buf, kPoint< INT32 >( 0, it->getDimensions().y - fnt->getVerticalOffset() ) );
			inter->unlockBuffer( it );

			done++;

			skipped = 0;

		} else {

			skipped++;

		}

		if( lastsecond + freq * tickspersec < inter->getCounterPos() ) {
			curl = done;
			outof = outofnext;
			done = outofnext = 0;
			skip = skipnext;
			skipnext = false;
			lastsecond = inter->getCounterPos(); }

		curframe += freq;

	}

	fnt.deactivate();

};
