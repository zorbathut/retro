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

	template < typename kReturn, typename kInput >						class kFunctor;
	template < typename kReturn, typename kInput >						class kFunctorSimple;
	template < typename kReturn, typename kInput, typename kTarget >	class kFunctorFull;
	template < typename kReturn, typename kInput, typename kType >		class kFunctorFactory;

	template < typename kReturnclass, typename kInputclass >
	class kFunctor {
	public:

		virtual kReturnclass operator()( kInputclass inp ) = 0;
		kReturnclass go( kInputclass inp ) { return (*this)( inp ); };

	};

	template < typename kReturnclass, typename kInputclass >
	class kFunctorSimple : public kFunctor< kReturnclass, kInputclass > {
	private:

		kReturnclass (*fptr)( kInputclass );

	public:

		virtual kReturnclass operator()( kInputclass inp );

		kFunctorSimple( kReturnclass (*in_fptr)( kInputclass ) );

	};

	template < typename kReturnclass, typename kInputclass, typename kTargetclass >
	class kFunctorFull : public kFunctor< kReturnclass, kInputclass > {
	private:

		kTargetclass	*tgc;
		kReturnclass	(kTargetclass::*fptr)( kInputclass );

	public:

		virtual kReturnclass operator()( kInputclass inp );

		kFunctorFull( kTargetclass *in_tgc, kReturnclass (kTargetclass::*in_fptr)( kInputclass ) );

	};

	template < typename kReturn, typename kInput, typename kType >
	class kFunctorFactory : public kFunctor< kReturn, kInput > {
	public:

		virtual kReturn operator()( kInput inp ) {
			return new kType( inp ); };

	};

	template < typename kReturnclass, typename kInputclass >
	kReturnclass kFunctorSimple< kReturnclass, kInputclass >::operator()( kInputclass inp ) {
		return (*fptr)( inp ); }

	template < typename kReturnclass, typename kInputclass >
	kFunctorSimple< kReturnclass, kInputclass >::kFunctorSimple( kReturnclass (*in_fptr)( kInputclass ) ) {
		fptr = in_fptr; }

	template < typename kReturnclass, typename kInputclass, typename kTargetclass >
	kReturnclass kFunctorFull< kReturnclass, kInputclass, kTargetclass >::operator()( kInputclass inp ) {
		return tgc->*fptr( inp ); }

	template < typename kReturnclass, typename kInputclass, typename kTargetclass >
	kFunctorFull< kReturnclass, kInputclass, kTargetclass >::kFunctorFull(
			kTargetclass *in_tgc, kReturnclass (kTargetclass::*in_fptr)( kInputclass ) ) {

		tgc = in_tgc;
		fptr = in_fptr; }

};

#endif