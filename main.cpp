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

#include <ctime>
#include <cstdlib>

#include "kInterface.h"
#include "kMainShellStackhead.h"
#include "kMainShellMenu.h"
#include "kGrfxFont.h"
#include "errlog.h"
#include "kGrfxRenderablePNG.h"
#include "kMGrfxRenderableFinite.h"
#include "kFileManager.h"
#include "kMGrfxFontBoundaries.h"
#include "kMGrfxFontHSL.h"
#include "kMGrfxFont.h"
#include "kMGrfxRenderableRaster.h"

void gameInit( kStartupData *dte ) {
	dte->minWindow = makePoint( 600, 400 );
	dte->maxWindow = makePoint( 1024, 768 );
	dte->startWindow = makePoint( 640, 480 );
};

bool initManager( kInterface *inter ) {		// return 1 on failure

	grfx::kRenderablePNG bg( "hardcode\\loadingbg.png" );
	grfx::kRenderablePNG bar( "hardcode\\loadingbar.png" );
	bg.loadAll();
	bar.loadAll();

	grfx::kWritable *target = inter->lockBuffer();
	target->clear( 0x00000000 );
	bg.renderTo( target, ( target->getDimensions() - bg.getDimensions() ) / 2 );
	inter->unlockBuffer( target );

	module::finite.generate( g_manager );
	module::font.generate( g_manager );
	module::fontbounds.generate( g_manager );
	module::fonthsl.generate( g_manager );
	module::raster.generate( g_manager );
	g_manager->prepare();

	while( g_manager->getCurCycle() != g_manager->getCycles() ) {
		target = inter->lockBuffer();
		target->clear( 0x00000000 );
		kPoint< INT32 > pos = ( target->getDimensions() - bg.getDimensions() ) / 2;
		bg.renderTo( target, pos );
		pos.x += 120;
		pos.y += 120 + 360;
		int curp = ( g_manager->getCurCycle() * 360 ) / g_manager->getCycles();
		pos.y -= curp;
		bar.renderPartTo( target, pos, kRect< INT32 >( kPoint< INT32 >( 0, 360 - curp ), bar.getDimensions() ) );
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

	srand( time( NULL ) );

	file::kManager manager;
	g_manager = &manager;

	if( initManager( inter ) )
		return;

	kMainShellStackhead mainstack( new kMainShellMenu );

	UINT64 freq = inter->getCounterFreq();
	UINT64 curframe = inter->getCounterPos();
	UINT64 lastsecond = inter->getCounterPos();
	int skipped = 0;

	freq /= tickspersec;	// how many ticks per, well, tick?

	int done = 0;
	int outof = 0;
	int outofnext = 0;

	int curl = 0;

	bool skip = false;
	bool skipnext = false;

	inter->updateControls(); // just to flush

	file::kHandle< grfx::kFont > fnt = module::font.get( "small" );
	fnt.activate();

	while( !inter->shutDown() && mainstack.ready() ) {

		const kControls *controls = inter->updateControls();

		mainstack.clockTick( controls );
		
		g_manager->tick();

		outofnext++;

		if( curframe + freq >= inter->getCounterPos() || skipped >= 14 ) {

			if( skipped >= 14 ) {
				curframe = inter->getCounterPos();
				skipnext = true; };

			while( curframe >= inter->getCounterPos() )
				inter->sleep( 1 );

			grfx::kWritable *it = inter->lockBuffer();
			it->clear( 0xff000000 );
			mainstack.renderFrame( it );
			int vert = 32;
			char buf[ 1024 ];
			int x;
			for( x = 0; x < controls->getAxiscount(); x++ ) {
				sprintf( buf, "%s: %s (%d, %d): %d", controls->getAxisinfo( x ).dev->name.c_str(), controls->getAxisinfo( x ).name.c_str(), controls->getAxisinfo( x ).hidid.getPage(), controls->getAxisinfo( x ).hidid.getItem(), controls->getAxis( x ) );
				fnt->renderTextTo( it, buf, kPoint< INT32 >( 0, vert ) );
				vert += fnt->getVerticalOffset();
			};
			vert += fnt->getVerticalOffset();
			for( x = 0; x < controls->getButtoncount(); x++ ) {
				if( controls->getButton( x ) != keyis::up ) {
					sprintf( buf, "%s: %s (%c%c%c%c%c) (%d, %d)",
						controls->getButtoninfo( x ).dev->name.c_str(),
						controls->getButtoninfo( x ).name.c_str(),
						( controls->getButton( x ) & keyis::up ) ? 'U' : ' ',
						( controls->getButton( x ) & keyis::down ) ? 'D' : ' ',
						( controls->getButton( x ) & keyis::push ) ? 'P' : ' ',
						( controls->getButton( x ) & keyis::release ) ? 'R' : ' ',
						( controls->getButton( x ) & keyis::change ) ? 'C' : ' ',
						controls->getButtoninfo( x ).hidid.getPage(),
						controls->getButtoninfo( x ).hidid.getItem() );
					fnt->renderTextTo( it, buf, kPoint< INT32 >( 0, vert ) );
					vert += fnt->getVerticalOffset();
				}
			}
			sprintf( buf, "%2d/%2d", curl, outof );
			if( skip )
				strcat( buf, " S" );
			fnt->renderTextTo( it, buf, kPoint< INT32 >( 0, it->getDimensions().y - fnt->getVerticalOffset() ) );
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
