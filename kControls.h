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

#ifndef RETRO_KCONTROLS
#define RETRO_KCONTROLS

class kControls;

#include "types.h"
#include "butility.h"
#include "kString.h"

const BYTE CONTROL_KEYDOWN = 0x01;
const BYTE CONTROL_KEYUP = 0x02;
const BYTE CONTROL_KEYPUSH = 0x04;
const BYTE CONTROL_KEYRELEASE = 0x08;
const BYTE CONTROL_KEYCHANGE = 0x10;

class kControls : private boost::noncopyable {
public:

	const BYTE *getButtons() const;
	const INT32 *getAxes() const;

	int getButtoncount() const;
	int getAxiscount() const;

	const zutil::kString *getButtonlabels() const;
	const zutil::kString *getAxislabels() const;

protected:

	BYTE *accessButtons();
	INT32 *accessAxes();	// yes, I realize I should be using set functions here, but this lets me
							// change the data in place.

	void setButtoncount( int newcount );
	void setAxiscount( int newcount );

	zutil::kString *accessButtonlabels() const;
	zutil::kString *accessAxislabels() const;

	kControls( int buttons, int axes );
	virtual ~kControls();

private:

	BYTE *buttons;
	INT32 *axes;

	zutil::kString *buttonlabels;
	zutil::kString *axislabels;

	int buttoncount;
	int axiscount;

};

#endif