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

#include "win32_hidid.h"

#include <string.h>

#include "hididConsts.h"
#include "errlog.h"

kHidid getHidid( const char *dev, const char *key ) {

	if( !strcmp( dev, "Keyboard" ) ) {
		if( !strcmp( key, "A" ) ) return hidid::kbd::A;
		if( !strcmp( key, "B" ) ) return hidid::kbd::B;
		if( !strcmp( key, "C" ) ) return hidid::kbd::C;
		if( !strcmp( key, "D" ) ) return hidid::kbd::D;
		if( !strcmp( key, "E" ) ) return hidid::kbd::E;
		if( !strcmp( key, "F" ) ) return hidid::kbd::F;
		if( !strcmp( key, "G" ) ) return hidid::kbd::G;
		if( !strcmp( key, "H" ) ) return hidid::kbd::H;
		if( !strcmp( key, "I" ) ) return hidid::kbd::I;
		if( !strcmp( key, "J" ) ) return hidid::kbd::J;
		if( !strcmp( key, "K" ) ) return hidid::kbd::K;
		if( !strcmp( key, "L" ) ) return hidid::kbd::L;
		if( !strcmp( key, "M" ) ) return hidid::kbd::M;
		if( !strcmp( key, "N" ) ) return hidid::kbd::N;
		if( !strcmp( key, "O" ) ) return hidid::kbd::O;
		if( !strcmp( key, "P" ) ) return hidid::kbd::P;
		if( !strcmp( key, "Q" ) ) return hidid::kbd::Q;
		if( !strcmp( key, "R" ) ) return hidid::kbd::R;
		if( !strcmp( key, "S" ) ) return hidid::kbd::S;
		if( !strcmp( key, "T" ) ) return hidid::kbd::T;
		if( !strcmp( key, "U" ) ) return hidid::kbd::U;
		if( !strcmp( key, "V" ) ) return hidid::kbd::V;
		if( !strcmp( key, "W" ) ) return hidid::kbd::W;
		if( !strcmp( key, "X" ) ) return hidid::kbd::X;
		if( !strcmp( key, "Y" ) ) return hidid::kbd::Y;
		if( !strcmp( key, "Z" ) ) return hidid::kbd::Z;

		if( !strcmp( key, "1" ) ) return hidid::kbd::n1;
		if( !strcmp( key, "2" ) ) return hidid::kbd::n2;
		if( !strcmp( key, "3" ) ) return hidid::kbd::n3;
		if( !strcmp( key, "4" ) ) return hidid::kbd::n4;
		if( !strcmp( key, "5" ) ) return hidid::kbd::n5;
		if( !strcmp( key, "6" ) ) return hidid::kbd::n6;
		if( !strcmp( key, "7" ) ) return hidid::kbd::n7;
		if( !strcmp( key, "8" ) ) return hidid::kbd::n8;
		if( !strcmp( key, "9" ) ) return hidid::kbd::n9;
		if( !strcmp( key, "0" ) ) return hidid::kbd::n0;

		if( !strcmp( key, "Esc" ) ) return hidid::kbd::esc;
		if( !strcmp( key, "-" ) ) return hidid::kbd::dash;
		if( !strcmp( key, "=" ) ) return hidid::kbd::equals;
		if( !strcmp( key, "Backspace" ) ) return hidid::kbd::backspace;
		if( !strcmp( key, "Tab" ) ) return hidid::kbd::tab;
		if( !strcmp( key, "[" ) ) return hidid::kbd::lbracket;
		if( !strcmp( key, "]" ) ) return hidid::kbd::rbracket;
		if( !strcmp( key, "Enter" ) ) return hidid::kbd::enter;
		if( !strcmp( key, "Ctrl" ) ) return hidid::kbd::leftctrl;
		if( !strcmp( key, ";" ) ) return hidid::kbd::semicolon;
		if( !strcmp( key, "'" ) ) return hidid::kbd::quote;
		if( !strcmp( key, "`" ) ) return hidid::kbd::tilde;
		if( !strcmp( key, "Shift" ) ) return hidid::kbd::leftshift;
		if( !strcmp( key, "\\" ) ) return hidid::kbd::backslash;
		if( !strcmp( key, "," ) ) return hidid::kbd::comma;
		if( !strcmp( key, "." ) ) return hidid::kbd::period;
		if( !strcmp( key, "/" ) ) return hidid::kbd::slash;
		if( !strcmp( key, "Right Shift" ) ) return hidid::kbd::rightshift;
		if( !strcmp( key, "Alt" ) ) return hidid::kbd::leftalt;
		if( !strcmp( key, "Space" ) ) return hidid::kbd::space;
		if( !strcmp( key, "Caps Lock" ) ) return hidid::kbd::capslock;
		if( !strcmp( key, "F1" ) ) return hidid::kbd::f1;
		if( !strcmp( key, "F2" ) ) return hidid::kbd::f2;
		if( !strcmp( key, "F3" ) ) return hidid::kbd::f3;
		if( !strcmp( key, "F4" ) ) return hidid::kbd::f4;
		if( !strcmp( key, "F5" ) ) return hidid::kbd::f5;
		if( !strcmp( key, "F6" ) ) return hidid::kbd::f6;
		if( !strcmp( key, "F7" ) ) return hidid::kbd::f7;
		if( !strcmp( key, "F8" ) ) return hidid::kbd::f8;
		if( !strcmp( key, "F9" ) ) return hidid::kbd::f9;
		if( !strcmp( key, "F10" ) ) return hidid::kbd::f10;
		if( !strcmp( key, "F11" ) ) return hidid::kbd::f11;
		if( !strcmp( key, "F12" ) ) return hidid::kbd::f12;
		if( !strcmp( key, "F13" ) ) return hidid::kbd::f13;
		if( !strcmp( key, "F14" ) ) return hidid::kbd::f14;
		if( !strcmp( key, "F15" ) ) return hidid::kbd::f15;
		if( !strcmp( key, "F16" ) ) return hidid::kbd::f16;
		if( !strcmp( key, "F17" ) ) return hidid::kbd::f17;
		if( !strcmp( key, "F18" ) ) return hidid::kbd::f18;
		if( !strcmp( key, "F19" ) ) return hidid::kbd::f19;
		if( !strcmp( key, "F20" ) ) return hidid::kbd::f20;
		if( !strcmp( key, "F21" ) ) return hidid::kbd::f21;
		if( !strcmp( key, "F22" ) ) return hidid::kbd::f22;
		if( !strcmp( key, "F23" ) ) return hidid::kbd::f23;
		if( !strcmp( key, "F24" ) ) return hidid::kbd::f24;
		if( !strcmp( key, "Num Lock" ) ) return hidid::kbd::numlock;
		if( !strcmp( key, "Scroll Lock" ) ) return hidid::kbd::scrolllock;
		if( !strcmp( key, "Num 7" ) ) return hidid::kbd::pad7;
		if( !strcmp( key, "Num 8" ) ) return hidid::kbd::pad8;
		if( !strcmp( key, "Num 9" ) ) return hidid::kbd::pad9;
		if( !strcmp( key, "Num -" ) ) return hidid::kbd::padminus;
		if( !strcmp( key, "Num 4" ) ) return hidid::kbd::pad4;
		if( !strcmp( key, "Num 5" ) ) return hidid::kbd::pad5;
		if( !strcmp( key, "Num 6" ) ) return hidid::kbd::pad6;
		if( !strcmp( key, "Num +" ) ) return hidid::kbd::padplus;
		if( !strcmp( key, "Num 1" ) ) return hidid::kbd::pad1;
		if( !strcmp( key, "Num 2" ) ) return hidid::kbd::pad2;
		if( !strcmp( key, "Num 3" ) ) return hidid::kbd::pad3;
		if( !strcmp( key, "Num 0" ) ) return hidid::kbd::pad0;
		if( !strcmp( key, "Num /" ) ) return hidid::kbd::padslash;
		if( !strcmp( key, "Num *" ) ) return hidid::kbd::padstar;
		if( !strcmp( key, "*" ) ) return hidid::kbd::padstar;
		if( !strcmp( key, "+" ) ) return hidid::kbd::padplus;
		if( !strcmp( key, "Num Del" ) ) return hidid::kbd::paddel;
		if( !strcmp( key, "Num Enter" ) ) return hidid::kbd::padenter;

		if( !strcmp( key, "Right Alt" ) ) return hidid::kbd::rightalt;
		if( !strcmp( key, "Right Ctrl" ) ) return hidid::kbd::rightctrl;
		if( !strcmp( key, "Left Windows" ) ) return hidid::kbd::leftOS;
		if( !strcmp( key, "Right Windows" ) ) return hidid::kbd::rightOS;

		if( !strcmp( key, "Application" ) ) return hidid::kbd::application;
		if( !strcmp( key, "Power" ) ) return hidid::kbd::power;
		if( !strcmp( key, "Execute" ) ) return hidid::kbd::execute;
		if( !strcmp( key, "Help" ) ) return hidid::kbd::help;
		if( !strcmp( key, "Menu" ) ) return hidid::kbd::menu;
		if( !strcmp( key, "Select" ) ) return hidid::kbd::select;
		if( !strcmp( key, "Stop" ) ) return hidid::kbd::stop;
		if( !strcmp( key, "Again" ) ) return hidid::kbd::again;
		if( !strcmp( key, "Undo" ) ) return hidid::kbd::undo;
		if( !strcmp( key, "Cut" ) ) return hidid::kbd::cut;
		if( !strcmp( key, "Copy" ) ) return hidid::kbd::copy;
		if( !strcmp( key, "Paste" ) ) return hidid::kbd::paste;
		if( !strcmp( key, "Find" ) ) return hidid::kbd::find;
		if( !strcmp( key, "Mute" ) ) return hidid::kbd::mute;
		if( !strcmp( key, "Volume +" ) ) return hidid::kbd::volup;
		if( !strcmp( key, "Volume -" ) ) return hidid::kbd::voldown;

		if( !strcmp( key, "Pause" ) ) return hidid::kbd::pause;
		if( !strcmp( key, "SysRq" ) ) return hidid::kbd::printscreen;
		if( !strcmp( key, "Prnt Scrn" ) ) return hidid::kbd::printscreen;

		if( !strcmp( key, "Ins" ) ) return hidid::kbd::insert;
		if( !strcmp( key, "Insert" ) ) return hidid::kbd::insert;
		if( !strcmp( key, "Del" ) ) return hidid::kbd::del;
		if( !strcmp( key, "Delete" ) ) return hidid::kbd::del;
		if( !strcmp( key, "Home" ) ) return hidid::kbd::home;
		if( !strcmp( key, "End" ) ) return hidid::kbd::end;
		if( !strcmp( key, "Page Up" ) ) return hidid::kbd::pageup;
		if( !strcmp( key, "Page Down" ) ) return hidid::kbd::pagedown;

		if( !strcmp( key, "Up" ) ) return hidid::kbd::arrowup;
		if( !strcmp( key, "Down" ) ) return hidid::kbd::arrowdown;
		if( !strcmp( key, "Left" ) ) return hidid::kbd::arrowleft;
		if( !strcmp( key, "Right" ) ) return hidid::kbd::arrowright;

	} else if( !strcmp( dev, "Microsoft SideWinder game pad" ) ) {

		if( !strcmp( key, "X" ) ) return hidid::gamepad::xaxis;
		if( !strcmp( key, "Y" ) ) return hidid::gamepad::yaxis;
		if( !strcmp( key, "Start" ) ) return hidid::gamepad::start;
		if( !strcmp( key, "Shift" ) ) return hidid::gamepad::select;
		if( !strcmp( key, "Button A" ) ) return hidid::gamepad::x1y1;
		if( !strcmp( key, "Button B" ) ) return hidid::gamepad::x2y1;
		if( !strcmp( key, "Button C" ) ) return hidid::gamepad::x3y1;
		if( !strcmp( key, "Button X" ) ) return hidid::gamepad::x1y2;
		if( !strcmp( key, "Button Y" ) ) return hidid::gamepad::x2y2;
		if( !strcmp( key, "Button Z" ) ) return hidid::gamepad::x3y2;
		if( !strcmp( key, "Button L" ) ) return hidid::gamepad::l1;
		if( !strcmp( key, "Button R" ) ) return hidid::gamepad::r1;

	} else if( !strcmp( dev, "Mouse" ) ) {

		if( !strcmp( key, "X-axis" ) ) return hidid::mouse::xaxis;
		if( !strcmp( key, "Y-axis" ) ) return hidid::mouse::yaxis;
		if( !strcmp( key, "Wheel" ) ) return hidid::mouse::wheel;
		// are these next three always true?
		if( !strcmp( key, "Button 0" ) ) return hidid::mouse::buttonl;
		if( !strcmp( key, "Button 1" ) ) return hidid::mouse::buttonr;
		if( !strcmp( key, "Button 2" ) ) return hidid::mouse::wheelbutton;
		if( !strcmp( key, "Button 3" ) ) return hidid::mouse::button4;
		if( !strcmp( key, "Button 4" ) ) return hidid::mouse::button5;

	}

	return kHidid();

}