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

#ifndef RETRO_KRECT
#define RETRO_KRECT

#include "kPoint.h"

template < typename kPrecision > class kRect {
public:

	kPoint< kPrecision > ul;
	kPoint< kPrecision > br;

	int height() const {
		return br.y - ul.y; }

	int width() const {
		return br.x - ul.x; }

	int area() const {
		return height() * width(); };

	kRect( void ) { };

	kRect( const kPoint< kPrecision > &in_ul, const kPoint< kPrecision > &in_br ) : ul( in_ul ), br( in_br ) { };
	kRect( const kPrecision &in_l, const kPrecision &in_u, const kPrecision &in_r, const kPrecision &in_b ) : ul( in_l, in_u ), br( in_r, in_b ) { };

	kRect( const kRect &kri ) : ul( kri.ul ), br( kri.br ) { };

};

template< typename kPrecision > kRect< kPrecision > makeRect( const kPrecision &l, const kPrecision &u,
															   const kPrecision &r, const kPrecision &d ) {
	return kRect< kPrecision >( l, u, r, d ); };

template< typename kPrecision > kRect< kPrecision > makeRect( const kPoint< kPrecision > &ul,
															   const kPoint< kPrecision > &dr ) {
	return kRect< kPrecision >( ul, dr ); };

template< typename kPrecision > kRect< kPrecision > makeRect( const kPoint< kPrecision > &size ) {
	return kRect< kPrecision >( makePoint< kPrecision >( 0, 0 ), size ); };

template < typename kPrecision >
std::ostream &operator<<( std::ostream &ostr, const kRect< kPrecision > &pt ) {
	ostr << pt.ul << "-" << pt.br;
	return ostr; };
	// See comment in kPoint.h.

template < typename kPrecLhs, typename kPrecRhs >
bool operator==( const kRect< kPrecLhs > &lhs, const kRect< kPrecRhs > &rhs ) {
	return lhs.ul == rhs.ul && lhs.br == rhs.br; };

#endif
