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

#ifndef TIMESPACE_KPOINT
#define TIMESPACE_KPOINT

#include "iostream_mini.h"

template < class kPrecision > class kPoint {
public:

	kPrecision x;
	kPrecision y;

	kPoint( void ) { };

	kPoint( const kPrecision &in_x, const kPrecision &in_y ) : x( in_x ), y( in_y ) { };

	kPoint( const kPoint &kpi ) : x( kpi.x ), y( kpi.y ) { };

	kPoint operator+( const kPoint &rhs ) const {
		return kPoint< kPrecision >( x + rhs.x, y + rhs.y ); }
	kPoint operator-( const kPoint &rhs ) const {
		return kPoint< kPrecision >( x - rhs.x, y - rhs.y ); }
	kPoint operator/( int scal ) const {
		return kPoint< kPrecision >( x / scal, y / scal ); } // yeah yeah, I need more operators. Shove it :P

};

template < class kPrecision >
std::ostream &operator<<( std::ostream &ostr, const kPoint< kPrecision > &pt ) {
	ostr << "( " << pt.x << ", " << pt.y << " )";
	return ostr; };

	// this works. It's weird, but it works. It only works, however, if the thing
	// that's trying to use this has also included iostream.
	// Funky, huh?
	// Not sure if it's supposed to work by ANSI :)

#endif
