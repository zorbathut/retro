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

#include "kControls.h"

#include "errlog.h"
#include "config.h"

BYTE kControls::getButton( int num ) const {
	return buttons[ num ]; };
INT32 kControls::getAxis( int num ) const {
	return axes[ num ]; };

int kControls::getButtoncount() const {
	return buttoncount; };
int kControls::getAxiscount() const {
	return axiscount; };

const control::kObjectspecs &kControls::getButtoninfo( int num ) const {
	return buttoninfos[ num ]; };
const control::kObjectspecs &kControls::getAxisinfo( int num ) const {
	return axisinfos[ num ]; };

BYTE *kControls::accessButtons() {
	return buttons; };
INT32 *kControls::accessAxes() {
	return axes; };

void kControls::setButtoncount( int newcount ) {
	delete [] buttons;
	delete [] buttoninfos;
	buttons = new BYTE[ newcount ];
	buttoninfos = new control::kObjectspecs[ newcount ];
	buttoncount = newcount;
};

void kControls::setAxiscount( int newcount ) {
	delete [] axes;
	delete [] axisinfos;
	axes = new INT32[ newcount ];
	axisinfos = new control::kObjectspecs[ newcount ];
	axiscount = newcount;
};

control::kObjectspecs *kControls::accessButtoninfo() {
	return buttoninfos; }
control::kObjectspecs *kControls::accessAxisinfo() {
	return axisinfos; };

void kControls::rebuildFindLists() {

	axisDevid.clear();
	buttonDevid.clear();

	axisHidid.clear();
	buttonHidid.clear();

	int i;
	for( i = 0; i < getAxiscount(); i++ ) {
		if( getAxisinfo( i ).hidid.valid() ) {
			if( axisHidid.find( getAxisinfo( i ).hidid ) != axisHidid.end() ) {
				int old = axisHidid[ getAxisinfo( i ).hidid ];
				g_errlog << "CONTROL: Axis HIDID conflict for \"" << getAxisinfo( old ).dev->name.get() << "," << getAxisinfo( old ).name.get() << "\" and \"" << getAxisinfo( i ).dev->name.get() << "," << getAxisinfo( i ).name.get() << "\"" << std::endl;
			}
			axisHidid[ getAxisinfo( i ).hidid ] = i;
		} else {
#if POSTDEBUGINFO
			g_errlog << "CONTROL: (debug) No provided HIDID for axis \"" << getAxisinfo( i ).dev->name.get() << "," << getAxisinfo( i ).name.get() << "\"" << std::endl;
#endif
		}
		axisDevid[ getButtoninfo( i ).getKeyDevId() ] = i;
	}

	for( i = 0; i < getButtoncount(); i++ ) {
		if( getButtoninfo( i ).hidid.valid() ) {
			if( buttonHidid.find( getButtoninfo( i ).hidid ) != buttonHidid.end() ) {
				int old = buttonHidid[ getButtoninfo( i ).hidid ];
				g_errlog << "CONTROL: Button HIDID conflict for \"" << getButtoninfo( old ).dev->name.get() << "," << getButtoninfo( old ).name.get() << "\" and \"" << getButtoninfo( i ).dev->name.get() << "," << getButtoninfo( i ).name.get() << "\"" << std::endl;
			}
			buttonHidid[ getButtoninfo( i ).hidid ] = i;
		} else {
#if POSTDEBUGINFO
			g_errlog << "CONTROL: (debug) No provided HIDID for button \"" << getButtoninfo( i ).dev->name.get() << "," << getButtoninfo( i ).name.get() << "\"" << std::endl;
#endif
		}
		buttonDevid[ getButtoninfo( i ).getKeyDevId() ] = i;
	}

};

kControls::kControls( int in_buttons, int in_axes ) :
		buttons( new BYTE[ in_buttons ] ),
		buttoninfos( new control::kObjectspecs[ in_buttons ] ),
		buttoncount( in_buttons ),
		axes( new INT32[ in_axes ] ),
		axisinfos( new control::kObjectspecs[ in_axes ] ),
		axiscount( in_axes )
	{ };

		
kControls::~kControls() {
	delete [] buttons;
	delete [] buttoninfos;
	delete [] axes;
	delete [] axisinfos;
};
