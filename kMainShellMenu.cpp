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

#include "kMainShellMenu.h"
#include "kMGrfxRenderableFinite.h"
#include "kMGrfxFontHSL.h"
#include "kMGrfxFont.h"
#include "hididConsts.h"
#include "errlog.h"

#include <stdlib.h>

#include <sstream>
#include <string>

int dx[ 4 ] = {  0, 0, -1, 1 };
int dy[ 4 ] = { -1, 1,  0, 0 };

kMainShell *kMainShellMenu::clockTick( const kControls *controls ) {
	int i;

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

		kroma[ 0 ][ 0 ] = controls->findButton( hidid::kbd::E );
		kroma[ 1 ][ 0 ] = controls->findButton( hidid::kbd::R );
		kroma[ 2 ][ 0 ] = controls->findButton( hidid::kbd::T );
		kroma[ 3 ][ 0 ] = controls->findButton( hidid::kbd::Y );

		kroma[ 0 ][ 1 ] = controls->findButton( hidid::kbd::D );
		kroma[ 1 ][ 1 ] = controls->findButton( hidid::kbd::F );
		kroma[ 2 ][ 1 ] = controls->findButton( hidid::kbd::G );
		kroma[ 3 ][ 1 ] = controls->findButton( hidid::kbd::H );

		ae[ 0 ][ 0 ] = controls->findButton( hidid::kbd::Q );
		ae[ 1 ][ 0 ] = controls->findButton( hidid::kbd::W );
		ae[ 2 ][ 0 ] = controls->findButton( hidid::kbd::E );
		ae[ 3 ][ 0 ] = controls->findButton( hidid::kbd::R );

		ae[ 0 ][ 1 ] = controls->findButton( hidid::kbd::A );
		ae[ 1 ][ 1 ] = controls->findButton( hidid::kbd::S );
		ae[ 2 ][ 1 ] = controls->findButton( hidid::kbd::D );
		ae[ 3 ][ 1 ] = controls->findButton( hidid::kbd::F );

		be[ 0 ][ 0 ] = controls->findButton( hidid::kbd::T );
		be[ 1 ][ 0 ] = controls->findButton( hidid::kbd::Y );
		be[ 2 ][ 0 ] = controls->findButton( hidid::kbd::U );
		be[ 3 ][ 0 ] = controls->findButton( hidid::kbd::I );

		be[ 0 ][ 1 ] = controls->findButton( hidid::kbd::G );
		be[ 1 ][ 1 ] = controls->findButton( hidid::kbd::H );
		be[ 2 ][ 1 ] = controls->findButton( hidid::kbd::J );
		be[ 3 ][ 1 ] = controls->findButton( hidid::kbd::K );

		balzac = controls->findButton( hidid::kbd::B );
		balspam = controls->findButton( hidid::kbd::N );

		got = true;

	}

	for( i = 0; i < 4; i++ ) {

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

	if( ( controls->getButton( balzac ) & keyis::push ) || ( controls->getButton( balspam ) & keyis::down ) ) {
		balls.push_back( std::make_pair( kPoint< double >( 0.0, 0.0 ), kPoint< double >( rand() / ( RAND_MAX * 100.0 ), rand() / ( RAND_MAX * 100.0 ) ) ) );
		fart = true;
	} else {
		fart = false;
	};

	for( i = 0; i < 4; i++ ) {

		if( controls->getButton( kroma[ i ][ 0 ] ) & keyis::down )
			dc.bytes[ i ]++;
		if( controls->getButton( kroma[ i ][ 1 ] ) & keyis::down )
			dc.bytes[ i ]--;

	}

	for( i = 0; i < 4; i++ ) {

		if( controls->getButton( ae[ i ][ 0 ] ) & keyis::down )
			a.bytes[ i ]++;
		if( controls->getButton( ae[ i ][ 1 ] ) & keyis::down )
			a.bytes[ i ]--;
		if( controls->getButton( be[ i ][ 0 ] ) & keyis::down )
			b.bytes[ i ]++;
		if( controls->getButton( be[ i ][ 1 ] ) & keyis::down )
			b.bytes[ i ]--;

	}

	for( size_t i = 0; i < balls.size(); i++ ) {
		balls[ i ].first += balls[ i ].second;
		if( balls[ i ].first.x < 0 ) {
			balls[ i ].first.x *= -1;
			balls[ i ].second.x *= -1;
		}
		if( balls[ i ].first.y < 0 ) {
			balls[ i ].first.y *= -1;
			balls[ i ].second.y *= -1;
		}
		if( balls[ i ].first.x > 1 ) {
			balls[ i ].first.x = 1 - ( balls[ i ].first.x - 1 );
			balls[ i ].second.x *= -1;
		}
		if( balls[ i ].first.y > 1 ) {
			balls[ i ].first.y = 1 - ( balls[ i ].first.y - 1 );
			balls[ i ].second.y *= -1;
		}
	}

	return this; };

void kMainShellMenu::renderFrame( grfx::kWritable *target ) {
	kPoint< INT32 > size = target->getDimensions();
	kPoint< INT32 > bs = ball->getDimensions();
	fin->renderTo( target, kPoint< INT32 >( 0, 0 ) );
	for( size_t i = 0; i < balls.size(); i++ ) {
		ball->renderTo( target, kPoint< INT32 >( INT32( size.x * balls[ i ].first.x - bs.x / 2 ), INT32( size.y * balls[ i ].first.y - bs.y / 2 ) ) );
	}
	std::stringstream foo;
	foo << size.x << "x" << size.y;
	std::string texxt;
	std::getline( foo, texxt );
	fnt->renderTextTo( target, texxt.c_str(), kPoint< INT32 >( 0, 0 ) );
	foo.clear();
	foo << gp << ", " << gb;
	std::getline( foo, texxt );
	gn->renderPartTo( target, gp, gb );
	fnt->renderTextTo( target, texxt.c_str(), kPoint< INT32 >( 100, 0 ) );
	foo.clear();
	foo << balls.size() << " giant bouncy balls";
	std::getline( foo, texxt );
	fnt->renderTextTo( target, texxt.c_str(), kPoint< INT32 >( 0, 16 ) );

	alph->renderTo( target, kPoint< INT32 >( 400, 400 ) );
	target->drawRect( makeRect( 100, 100, 300, 300 ), a );

	std::stringstream ss;

	ss << dc;

	fnt->renderTextTo( target, ss.str().c_str(), makePoint( 0, 150 ) );
	fnthslty = fnthsl->getChroma( dc );
	fnthslty.activate();
	fnthslty->renderTextTo( target, ss.str().c_str(), makePoint( 0, 200 ) );
	fnthslty.deactivate();
	fnthsl->dropChroma( dc );

	std::stringstream ss2;

	ss2 << a << " = " << grfx::cutil::RGBtoHSL( a );

	fnt->renderTextTo( target, ss2.str().c_str(), makePoint( 0, 500 ) );

/*	std::pair< kPoint< INT32 >, grfx::kColor > det[ 100 ];
	for( int i = 0; i < 100; i++ ) {
		det[ i ].first.x = rand() % target->getDimensions().x;
		det[ i ].first.y = rand() % target->getDimensions().y;
		det[ i ].second.argb = 0xffffff00;
	}
	target->drawPoints( det, 100 );*/
};

kMainShellMenu::kMainShellMenu() : fin( module::finite.get( "gnarl" ) ),
			fnt( module::font.get( "arial" ) ),
			gn( module::finite.get( "greenity" ) ),
			alph( module::finite.get( "alpha" ) ),
			ball( module::finite.get( "bouncy" ) ),
			gp( 0, 0 ), gb( 0, 0, 0, 0 ), got( false ) {
	fin.activate();
	fnt.activate();
	alph.activate();
	ball.activate();
	gn.activate();

	fnthsl = module::fonthsl.get( "arial" );
	fnthsl.activate();
	/*fnthslty = fnthsl->getChroma( COL );
	fnthslty.activate();*/

};

kMainShellMenu::~kMainShellMenu() {

	/*fnthslty.deactivate();
	fnthsl->dropChroma( COL );*/
	fnthsl.deactivate();
	fin.deactivate();
	fnt.deactivate();
	alph.deactivate();
	ball.deactivate();
	gn.deactivate();
};