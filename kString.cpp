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

#include "kString.h"

#include <string.h>
#include <stdlib.h>

#include "null.h"

namespace zutil {

	const char *kString::get() const { return str; };
	void kString::set( const char *dat ) {
		if( dat == str )
			return;
		delete [] str;
		str = new char[ strlen( dat ) + 1 ];
		strcpy( str, dat );
	};

	bool kString::operator< ( const kString &rhs ) const {
		return strcmp( str, rhs.str ) <  0; }
	bool kString::operator<=( const kString &rhs ) const {
		return strcmp( str, rhs.str ) <= 0; }
	bool kString::operator> ( const kString &rhs ) const {
		return strcmp( str, rhs.str ) >  0; }
	bool kString::operator>=( const kString &rhs ) const {
		return strcmp( str, rhs.str ) >= 0; }
	bool kString::operator==( const kString &rhs ) const {
		return strcmp( str, rhs.str ) == 0; }
	bool kString::operator!=( const kString &rhs ) const {
		return strcmp( str, rhs.str ) != 0; }

	const kString &kString::operator=( const kString &rhs ) { set( rhs.str ); return *this; };
	const kString &kString::operator=( const char *rhs ) { set( rhs ); return *this; };

	kString::kString() : str( new char[ 1 ] ) { *str = '\0'; };
	kString::kString( const char *dat ) { str = new char[ strlen( dat ) + 1 ]; strcpy( str, dat ); };
	kString::kString( const kString &in ) { str = new char[ strlen( in.str ) + 1 ]; strcpy( str, in.str ); };
	kString::~kString() { delete [] str; };

	bool kString::case_insensitive_lessthan::operator()( const kString &lhs, const kString &rhs ) const {
		const char *lhsd = lhs.get();
		const char *rhsd = rhs.get();
		while( *lhsd == *rhsd && *lhsd ) {
			lhsd++; rhsd++; }
		return tolower( *lhsd ) < tolower( *rhsd );
	};

	void strcpy( char *dst, const char *src ) {
		while( *(dst++) = *(src++) ); };

	int strlen( const char *str ) {
		int accum = 0;
		while( *(str++) )
			accum++;
		return accum;
	};

	int strcmp( const char *lhs, const char *rhs ) {
		return ::strcmp( lhs, rhs ); };


};