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

#ifndef RETRO_KUNIQUEID
#define RETRO_KUNIQUEID

#include "types.h"

class kUniqueKeyId {
private:

	char uid[ KEYUNIQUEIDSIZE ];

public:

	static int getSize();
	const char *getData() const;

	kUniqueKeyId(); // init to something unused. TODO: something in a header to transmit an unused value :)
	kUniqueKeyId( const char *in );

};

bool operator< ( const kUniqueKeyId &a, const kUniqueKeyId &b );
bool operator<=( const kUniqueKeyId &a, const kUniqueKeyId &b );
bool operator> ( const kUniqueKeyId &a, const kUniqueKeyId &b );
bool operator>=( const kUniqueKeyId &a, const kUniqueKeyId &b );
bool operator==( const kUniqueKeyId &a, const kUniqueKeyId &b );
bool operator!=( const kUniqueKeyId &a, const kUniqueKeyId &b );

class kUniqueDevId {
private:

	char uid[ DEVUNIQUEIDSIZE ];

public:

	static int getSize();
	const char *getData() const;

	kUniqueDevId(); // init to something unused.
	kUniqueDevId( const char *in );

};

bool operator< ( const kUniqueDevId &a, const kUniqueDevId &b );
bool operator<=( const kUniqueDevId &a, const kUniqueDevId &b );
bool operator> ( const kUniqueDevId &a, const kUniqueDevId &b );
bool operator>=( const kUniqueDevId &a, const kUniqueDevId &b );
bool operator==( const kUniqueDevId &a, const kUniqueDevId &b );
bool operator!=( const kUniqueDevId &a, const kUniqueDevId &b );

class kUniqueKeyDevId {
private:

	kUniqueDevId dev;
	kUniqueKeyId key;

public:

	const kUniqueDevId &getDev() const;
	const kUniqueKeyId &getKey() const;

	kUniqueKeyDevId();
	kUniqueKeyDevId( const kUniqueDevId &dev, const kUniqueKeyId &key );

};

bool operator< ( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b );
bool operator<=( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b );
bool operator> ( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b );
bool operator>=( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b );
bool operator==( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b );
bool operator!=( const kUniqueKeyDevId &a, const kUniqueKeyDevId &b );

#endif