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

#include "kMainShellMenu.h"
#include "kMGrfxRenderableFinite.h"
#include "kMGrfxFont.h"
#include "hididConsts.h"

#include <stdlib.h>

#include <sstream>
#include <string>

int dx[ 4 ] = {  0, 0, -1, 1 };
int dy[ 4 ] = { -1, 1,  0, 0 };

kMainShell *kMainShellMenu::clockTick( const kControls *controls ) {
	if( !got ) {

		pbs[ 0 ] = controls->findButton( hidid::kbd::arrowup );
		pbs[ 1 ] = controls->findButton( hidid::kbd::arrowdown );
		pbs[ 2 ] = controls->findButton( hidid::kbd::arrowleft );
		pbs[ 3 ] = controls->findButton( hidid::kbd::arrowright );

		ulbbs[ 0 ] = controls->findButton( hidid::kbd::W );
		ulbbs[ 1 ] = controls->findButton( hidid::kbd::Z );
		ulbbs[ 2 ] = controls->findButton( hidid::kbd::A );
		ulbbs[ 3 ] = controls->findButton( hidid::kbd::S );

		brbbs[ 0 ] = controls->findButton( hidid::kbd::I );
		brbbs[ 1 ] = controls->findButton( hidid::kbd::M );
		brbbs[ 2 ] = controls->findButton( hidid::kbd::J );
		brbbs[ 3 ] = controls->findButton( hidid::kbd::K );

		got = true;

	}

	for( int i = 0; i < 4; i++ ) {

		if( pbs[ i ] != -1 ) {
			gp.x += ( ( controls->getButton( pbs[ i ] ) & keyis::down ) != 0 ) * dx[ i ];
			gp.y += ( ( controls->getButton( pbs[ i ] ) & keyis::down ) != 0 ) * dy[ i ];
		}

		if( ulbbs[ i ] != -1 ) {
			gb.ul.x += ( ( controls->getButton( ulbbs[ i ] ) & keyis::down ) != 0 ) * dx[ i ];
			gb.ul.y += ( ( controls->getButton( ulbbs[ i ] ) & keyis::down ) != 0 ) * dy[ i ];
		}

		if( brbbs[ i ] != -1 ) {
			gb.br.x += ( ( controls->getButton( brbbs[ i ] ) & keyis::down ) != 0 ) * dx[ i ];
			gb.br.y += ( ( controls->getButton( brbbs[ i ] ) & keyis::down ) != 0 ) * dy[ i ];
		}

	}

	return this; };

void kMainShellMenu::renderFrame( grfx::kWritable *target ) {
	kPoint< INT32 > size = target->getDimensions();
	fin->renderTo( target, kPoint< INT32 >( 0, 0 ) );
	std::stringstream foo;
	foo << size.x << "x" << size.y;
	std::string texxt;
	std::getline( foo, texxt );
	fnt->renderTextTo( target, texxt.c_str(), kPoint< INT32 >( 0, 0 ) );
	foo.clear();
	foo << gp << ", " << gb;
	std::getline( foo, texxt );
	fnt->renderTextTo( target, texxt.c_str(), kPoint< INT32 >( 100, 0 ) );
	gn->renderPartTo( target, gp, gb );

/*	std::pair< kPoint< INT32 >, grfx::kColor > det[ 100 ];
	for( int i = 0; i < 100; i++ ) {
		det[ i ].first.x = rand() % target->getDimensions().x;
		det[ i ].first.y = rand() % target->getDimensions().y;
		det[ i ].second.argb = 0xffffff00;
	}
	target->drawPoints( det, 100 );*/
};

kMainShellMenu::kMainShellMenu() : fin( module::finite.get( "gnarl" ) ),
			fnt( module::font.get( "big" ) ),
			gn( module::finite.get( "greenity" ) ),
			gp( 0, 0 ), gb( 0, 0, 0, 0 ), got( false ) {
	fin.activate();
	fnt.activate();
	gn.activate();
};

kMainShellMenu::~kMainShellMenu() {
	fin.deactivate();
	fnt.deactivate();
	gn.deactivate();
};