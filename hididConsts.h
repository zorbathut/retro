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

#ifndef RETRO_HIDIDCONSTS
#define RETRO_HIDIDCONSTS

#include "kHIDID.h"

namespace hidid {

	namespace kbd {

		extern const kHidid A;
		extern const kHidid B;
		extern const kHidid C;
		extern const kHidid D;
		extern const kHidid E;
		extern const kHidid F;
		extern const kHidid G;
		extern const kHidid H;
		extern const kHidid I;
		extern const kHidid J;
		extern const kHidid K;
		extern const kHidid L;
		extern const kHidid M;
		extern const kHidid N;
		extern const kHidid O;
		extern const kHidid P;
		extern const kHidid Q;
		extern const kHidid R;
		extern const kHidid S;
		extern const kHidid T;
		extern const kHidid U;
		extern const kHidid V;
		extern const kHidid W;
		extern const kHidid X;
		extern const kHidid Y;
		extern const kHidid Z;

		extern const kHidid n1;
		extern const kHidid n2;
		extern const kHidid n3;
		extern const kHidid n4;
		extern const kHidid n5;
		extern const kHidid n6;
		extern const kHidid n7;
		extern const kHidid n8;
		extern const kHidid n9;
		extern const kHidid n0;

		extern const kHidid enter;
		extern const kHidid esc;
		extern const kHidid backspace;
		extern const kHidid tab;
		extern const kHidid space;
		extern const kHidid dash;
		extern const kHidid equals;
		extern const kHidid lbracket;
		extern const kHidid rbracket;
		extern const kHidid backslash;
		extern const kHidid semicolon;
		extern const kHidid quote;
		extern const kHidid tilde;
		extern const kHidid comma;
		extern const kHidid period;
		extern const kHidid slash;
		extern const kHidid capslock;

		extern const kHidid f1;
		extern const kHidid f2;
		extern const kHidid f3;
		extern const kHidid f4;
		extern const kHidid f5;
		extern const kHidid f6;
		extern const kHidid f7;
		extern const kHidid f8;
		extern const kHidid f9;
		extern const kHidid f10;
		extern const kHidid f11;
		extern const kHidid f12;
		extern const kHidid f13;
		extern const kHidid f14;
		extern const kHidid f15;
		extern const kHidid f16;
		extern const kHidid f17;
		extern const kHidid f18;
		extern const kHidid f19;
		extern const kHidid f20;
		extern const kHidid f21;
		extern const kHidid f22;
		extern const kHidid f23;
		extern const kHidid f24;

		extern const kHidid printscreen;
		extern const kHidid scrolllock;
		extern const kHidid pause;
		extern const kHidid insert;
		extern const kHidid home;
		extern const kHidid pageup;
		extern const kHidid del;
		extern const kHidid end;
		extern const kHidid pagedown;
		extern const kHidid arrowright;
		extern const kHidid arrowleft;
		extern const kHidid arrowdown;
		extern const kHidid arrowup;
		extern const kHidid padnumlock;
		extern const kHidid padslash;
		extern const kHidid padstar;
		extern const kHidid padminus;
		extern const kHidid padplus;
		extern const kHidid padenter;
		extern const kHidid pad1;
		extern const kHidid pad2;
		extern const kHidid pad3;
		extern const kHidid pad4;
		extern const kHidid pad5;
		extern const kHidid pad6;
		extern const kHidid pad7;
		extern const kHidid pad8;
		extern const kHidid pad9;
		extern const kHidid pad0;
		extern const kHidid padins;
		extern const kHidid paddel;

		extern const kHidid application;
		extern const kHidid power;
		extern const kHidid execute;
		extern const kHidid help;
		extern const kHidid menu;
		extern const kHidid select;
		extern const kHidid stop;
		extern const kHidid again;
		extern const kHidid undo;
		extern const kHidid cut;
		extern const kHidid copy;
		extern const kHidid paste;
		extern const kHidid find;
		extern const kHidid mute;
		extern const kHidid volup;
		extern const kHidid voldown;

		extern const kHidid leftctrl;
		extern const kHidid leftshift;
		extern const kHidid leftalt;
		extern const kHidid leftOS;
		extern const kHidid rightctrl;
		extern const kHidid rightshift;
		extern const kHidid rightalt;
		extern const kHidid rightOS;

		// note: get a more usable HIDID
		extern const kHidid numlock;

	};

	namespace gamepad {

		extern const kHidid xaxis;
		extern const kHidid yaxis;
		extern const kHidid start;
		extern const kHidid select;
		extern const kHidid l1;
		extern const kHidid l2;
		extern const kHidid r1;
		extern const kHidid r2;
		extern const kHidid x1y1;
		extern const kHidid x2y1;
		extern const kHidid x3y1;
		extern const kHidid x1y2;
		extern const kHidid x2y2;
		extern const kHidid x3y2;

		/* button IDs are positional in an array - example :
		      
				x1y2   x2y2   x3y2   |
                                     |
                x1y1   x2y1   x3y1   |
				 THUMB               |
			-------------------------/ */

	};

	namespace mouse {

		extern const kHidid xaxis;
		extern const kHidid yaxis;
		extern const kHidid wheel;
		extern const kHidid buttonl;
		extern const kHidid buttonr;
		extern const kHidid wheelbutton;
		extern const kHidid button4;
		extern const kHidid button5;
	
	};

};

#endif