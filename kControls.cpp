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

BYTE kControls::getButton( int num ) const {
	return buttons[ num ]; };
INT32 kControls::getAxis( int num ) const {
	return axes[ num ]; };

int kControls::getButtoncount() const {
	return buttoncount; };
int kControls::getAxiscount() const {
	return axiscount; };

const zutil::kString &kControls::getButtonlabel( int num ) const {
	return buttonlabels[ num ]; };
const zutil::kString &kControls::getAxislabel( int num ) const {
	return axislabels[ num ]; };

const kDevicespecs &kControls::getButtondev( int num ) const {
	return *buttondevs[ num ]; };
const kDevicespecs &kControls::getAxisdev( int num ) const {
	return *axisdevs[ num ]; };

BYTE *kControls::accessButtons() {
	return buttons; };
INT32 *kControls::accessAxes() {
	return axes; };

void kControls::setButtoncount( int newcount ) {
	delete [] buttons;
	delete [] buttonlabels;
	delete [] buttondevs;
	buttons = new BYTE[ newcount ];
	buttonlabels = new zutil::kString[ newcount ];
	buttondevs = new const kDevicespecs*[ newcount ];
	buttoncount = newcount;
};

void kControls::setAxiscount( int newcount ) {
	delete [] axes;
	delete [] axislabels;
	delete [] axisdevs;
	axes = new INT32[ newcount ];
	axislabels = new zutil::kString[ newcount ];
	axisdevs = new const kDevicespecs*[ newcount ];
	axiscount = newcount;
};

zutil::kString *kControls::accessButtonlabels() {
	return buttonlabels; }
zutil::kString *kControls::accessAxislabels() {
	return axislabels; };

const kDevicespecs **kControls::accessButtondevs() {
	return buttondevs; }
const kDevicespecs **kControls::accessAxisdevs() {
	return axisdevs; };

kControls::kControls( int in_buttons, int in_axes ) :
		buttons( new BYTE[ in_buttons ] ),
		buttonlabels( new zutil::kString[ in_buttons ] ),
		buttondevs( new const kDevicespecs*[ in_buttons ] ),
		buttoncount( in_buttons ),
		axes( new INT32[ in_axes ] ),
		axislabels( new zutil::kString[ in_axes ] ),
		axisdevs( new const kDevicespecs*[ in_axes ] ),
		axiscount( in_axes )
	{ };

		
kControls::~kControls() {
	delete [] buttons;
	delete [] buttonlabels;
	delete [] axes;
	delete [] axislabels;
};
