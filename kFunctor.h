#ifndef _ZUTIL_KFUNCTOR
#define _ZUTIL_KFUNCTOR

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

/* Want to contact me? zorbathut@uswest.net - assuming it's still valid. Give it a try.

   Functors, baybee! What's a functor? A class member function AND a class. Classy. *cough*. Or just a function. Either way.
   Data must be entered at construction time. Shouldn't be much trouble.

*/

/* Functor Revision History: version 1.1.0

   1.0.0: First release
   1.1.0: Added .go :)

*/

namespace zutil {

	class kNFunctor;

	template < typename kReturn, typename kInput >						class kIOFunctor;
	template < typename kReturn, typename kInput >						class kIOFunctorSimple;
	template < typename kReturn, typename kInput, typename kTarget >	class kIOFunctorFull;
	template < typename kReturn, typename kInput, typename kType >		class kIOFunctorFactory;

	class kNFunctor {
	public:
		virtual void operator()() = 0;
		void go() { (*this)(); };
	};

	template < typename kTarget >
	class kNFunctorFull : public kNFunctor {
	private:
		kTarget	*tgc;
		void			(kTarget::*fptr)();
	public:

		virtual void operator()();

		kNFunctorFull( kTarget *in_tgc, void (kTarget::*in_fptr)() );

	};

	template < typename kReturn, typename kInput >
	class kIOFunctor {
	public:

		virtual kReturn operator()( kInput inp ) = 0;
		kReturn go( kInput inp ) { return (*this)( inp ); };

	};

	template < typename kReturn, typename kInput >
	class kIOFunctorSimple : public kIOFunctor< kReturn, kInput > {
	private:

		kReturn (*fptr)( kInput );

	public:

		virtual kReturn operator()( kInput inp );

		kIOFunctorSimple( kReturn (*in_fptr)( kInput ) );

	};

	template < typename kReturn, typename kInput, typename kTarget >
	class kIOFunctorFull : public kIOFunctor< kReturn, kInput > {
	private:

		kTarget	*tgc;
		kReturn	(kTarget::*fptr)( kInput );

	public:

		virtual kReturn operator()( kInput inp );

		kIOFunctorFull( kTarget *in_tgc, kReturn (kTarget::*in_fptr)( kInput ) );

	};

	template < typename kReturn, typename kInput, typename kTarget, typename kIntReturn >
	class kIOFunctorFullPoly : public kIOFunctor< kReturn, kInput > {
	private:

		kTarget		*tgc;
		kIntReturn	(kTarget::*fptr)( kInput );

	public:

		virtual kReturn operator()( kInput inp );

		kIOFunctorFullPoly( kTarget *in_tgc, kIntReturn (kTarget::*in_fptr)( kInput ) );

	};

	template < typename kReturn, typename kInput, typename kType >
	class kIOFunctorFactory : public kIOFunctor< kReturn, kInput > {
	public:

		virtual kReturn operator()( kInput inp );

	};

	template < typename kTarget >
	void kNFunctorFull< kTarget >::operator()() {
		( tgc->*fptr )(); }

	template < typename kTarget >
	kNFunctorFull< kTarget >::kNFunctorFull(
			kTarget *in_tgc, void (kTarget::*in_fptr)() ) {

		tgc = in_tgc;
		fptr = in_fptr; }

	template < typename kReturn, typename kInput >
	kReturn kIOFunctorSimple< kReturn, kInput >::operator()( kInput inp ) {
		return ( *fptr )( inp ); }

	template < typename kReturn, typename kInput >
	kIOFunctorSimple< kReturn, kInput >::kIOFunctorSimple( kReturn (*in_fptr)( kInput ) ) {
		fptr = in_fptr; }

	template < typename kReturn, typename kInput, typename kTarget >
	kReturn kIOFunctorFull< kReturn, kInput, kTarget >::operator()( kInput inp ) {
		return ( tgc->*fptr )( inp ); }

	template < typename kReturn, typename kInput, typename kTarget >
	kIOFunctorFull< kReturn, kInput, kTarget >::kIOFunctorFull(
			kTarget *in_tgc, kReturn (kTarget::*in_fptr)( kInput ) ) {

		tgc = in_tgc;
		fptr = in_fptr; }

	template < typename kReturn, typename kInput, typename kTarget, typename kIntReturn >
	kReturn kIOFunctorFullPoly< kReturn, kInput, kTarget, kIntReturn >::operator()( kInput inp ) {
		return ( tgc->*fptr )( inp ); }

	template < typename kReturn, typename kInput, typename kTarget, typename kIntReturn >
	kIOFunctorFullPoly< kReturn, kInput, kTarget, kIntReturn >::kIOFunctorFullPoly(
			kTarget *in_tgc, kIntReturn (kTarget::*in_fptr)( kInput ) ) {

		tgc = in_tgc;
		fptr = in_fptr; }

	template < typename kReturn, typename kInput, typename kType >
	kReturn kIOFunctorFactory< kReturn, kInput, kType >::operator()( kInput inp ) {
		return new kType( inp ); };

};

#endif
