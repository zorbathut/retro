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

#pragma warning( disable : 4786 )

#include "kHIDID.h"
#include "kString.h"
#include "kUniqueID.h"

#include "types.h"
#include "butility.h"

#include <map>

namespace keyis {

	const BYTE down = 0x01;
	const BYTE up = 0x02;
	const BYTE push = 0x04;
	const BYTE release = 0x08;
	const BYTE change = 0x10;

}; // deal :P

namespace control {

	class kDevicespecs  {
	public:

		zutil::kString name;
		kUniqueDevId uid;
		int axes;
		int buttons;

	};

	class kObjectspecs {
	public:

		zutil::kString name;
		kHidid hidid;
		kUniqueKeyId uid;
		const kDevicespecs *dev;

		kUniqueKeyDevId getKeyDevId() const {
			return kUniqueKeyDevId( dev->uid, uid ); };

	};

};

class kControls : private boost::noncopyable {
public:

	BYTE getButton( int num ) const;
	INT32 getAxis( int num ) const;

	int getButtoncount() const;
	int getAxiscount() const;

	const control::kObjectspecs &getButtoninfo( int num ) const;
	const control::kObjectspecs &getAxisinfo( int num ) const;			// so I can reimplement these at some point

	int findAxis( const kUniqueKeyDevId &devid ) const;
	int findAxis( const kHidid &devid ) const;

	int findButton( const kUniqueKeyDevId &devid ) const;
	int findButton( const kHidid &devid ) const;	// all of these return -1 on failure

protected:

	BYTE *accessButtons();
	INT32 *accessAxes();	// yes, I realize I should be using set functions here, but this lets me
							// change the data in place.

	void setButtoncount( int newcount );
	void setAxiscount( int newcount );

	control::kObjectspecs *accessButtoninfo();
	control::kObjectspecs *accessAxisinfo();

	void rebuildFindLists();

	kControls( int buttons, int axes );
	virtual ~kControls();

private:

	BYTE *buttons;
	INT32 *axes;

	control::kObjectspecs *buttoninfos;
	control::kObjectspecs *axisinfos;

	int buttoncount;
	int axiscount;

	std::map< kUniqueKeyDevId, int > axisDevid;
	std::map< kUniqueKeyDevId, int > buttonDevid;

	std::map< kHidid, int > axisHidid;
	std::map< kHidid, int > buttonHidid;

};

#endif