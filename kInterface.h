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

#ifndef RETRO_KINTERFACE
#define RETRO_KINTERFACE

class kInterface;

#include <utility>

#include "kGrfxWritable.h"
#include "kColor.h"
#include "butility.h"
#include "kControls.h"

class kInterface : private boost::noncopyable {
private:

	kInterface( const kInterface & );
	void operator=( const kInterface & );

public:

	virtual grfx::kWritable *lockBuffer() = 0;
	virtual void unlockBuffer( grfx::kWritable *buf ) = 0;

	virtual const kControls *updateControls() = 0;

	virtual bool shutDown() const = 0;	// should I quit?
	virtual void shutDownNow() = 0;		// Yes, yes you should.

	virtual UINT64 getCounterFreq() const = 0;
	virtual UINT64 getCounterPos() const = 0;

	virtual void sleep( int msec ) const = 0; // or, alternatively, just pause

	virtual ~kInterface() { };
	kInterface() { };

};

class kStartupData {
public:

	kPoint< INT32 > minWindow;
	kPoint< INT32 > maxWindow;
	kPoint< INT32 > startWindow;

};

void gameInit( kStartupData *dat );
void gameMain( kInterface *inter );

#endif
