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

#include "dx8util.h"

#include <dinput.h>
#include <stdio.h>

void directInput8Error( HRESULT ecode, char *buf ) {
	switch( ecode ) {
//	case DI_BUFFEROVERFLOW:
//	case DI_NOTATTACHED:
//	case DI_PROPNOEFFECT:	// same
	case DI_NOEFFECT: sprintf( buf, "DI_NOEFFECT/DI_BUFFEROVERFLOW/DI_NOTATTACHED/DI_PROPNOEFFECT" ); break;
	case DI_DOWNLOADSKIPPED: sprintf( buf, "DI_DOWNLOADSKIPPED" ); break;
	case DI_EFFECTRESTARTED: sprintf( buf, "DI_EFFECTRESTARTED" ); break;
	case DI_OK: sprintf( buf, "DI_OK" ); break;
	case DI_POLLEDDEVICE: sprintf( buf, "DI_POLLEDDEVICE" ); break;
	case DI_SETTINGSNOTSAVED: sprintf( buf, "DI_SETTINGSNOTSAVED" ); break;
	case DI_TRUNCATED: sprintf( buf, "DI_TRUNCATED" ); break;
	case DI_TRUNCATEDANDRESTARTED: sprintf( buf, "DI_TRUNCATEDANDRESTARTED" ); break;
	case DI_WRITEPROTECT: sprintf( buf, "DI_WRITEPROTECT" ); break;
	case DIERR_ACQUIRED: sprintf( buf, "DIERR_ACQUIRED" ); break;
	case DIERR_ALREADYINITIALIZED: sprintf( buf, "DIERR_ALREADYINITIALIZED" ); break;
	case DIERR_BADDRIVERVER: sprintf( buf, "DIERR_BADDRIVERVER" ); break;
	case DIERR_BETADIRECTINPUTVERSION: sprintf( buf, "DIERR_BETADIRECTINPUTVERSION" ); break;
	case DIERR_DEVICEFULL: sprintf( buf, "DIERR_DEVICEFULL" ); break;
	case DIERR_DEVICENOTREG: sprintf( buf, "DIERR_DEVICENOTREG" ); break;
	case DIERR_EFFECTPLAYING: sprintf( buf, "DIERR_EFFECTPLAYING" ); break;
	case DIERR_GENERIC: sprintf( buf, "DIERR_GENERIC" ); break;
//	case DIERR_OTHERAPPHASPRIO:
//	case DIERR_READONLY: // same
	case DIERR_HANDLEEXISTS: sprintf( buf, "DIERR_HANDLEEXISTS/DIERR_OTHERAPPHASPRIO/DIERR_READONLY" ); break;
	case DIERR_HASEFFECTS: sprintf( buf, "DIERR_HASEFFECTS" ); break;
	case DIERR_INCOMPLETEEFFECT: sprintf( buf, "DIERR_INCOMPLETEEFFECT" ); break;
	case DIERR_INPUTLOST: sprintf( buf, "DIERR_INPUTLOST" ); break;
	case DIERR_INVALIDPARAM: sprintf( buf, "DIERR_INVALIDPARAM" ); break;
	case DIERR_MAPFILEFAIL: sprintf( buf, "DIERR_MAPFILEFAIL" ); break;
	case DIERR_MOREDATA: sprintf( buf, "DIERR_MOREDATA" ); break;
	case DIERR_NOAGGREGATION: sprintf( buf, "DIERR_NOAGGREGATION" ); break;
	case DIERR_NOINTERFACE: sprintf( buf, "DIERR_NOINTERFACE" ); break;
	case DIERR_NOTACQUIRED: sprintf( buf, "DIERR_NOTACQUIRED" ); break;
	case DIERR_NOTBUFFERED: sprintf( buf, "DIERR_NOTBUFFERED" ); break;
	case DIERR_NOTDOWNLOADED: sprintf( buf, "DIERR_NOTDOWNLOADED" ); break;
	case DIERR_NOTEXCLUSIVEACQUIRED: sprintf( buf, "DIERR_NOTEXCLUSIVEACQUIRED" ); break;
//	case DIERR_OBJECTNOTFOUND: // same
	case DIERR_NOTFOUND: sprintf( buf, "DIERR_NOTFOUND" ); break;
	case DIERR_NOTINITIALIZED: sprintf( buf, "DIERR_NOTINITIALIZED" ); break;
	case DIERR_OLDDIRECTINPUTVERSION: sprintf( buf, "DIERR_OLDDIRECTINPUTVERSION" ); break;
	case DIERR_OUTOFMEMORY: sprintf( buf, "DIERR_OUTOFMEMORY" ); break;
	case DIERR_REPORTFULL: sprintf( buf, "DIERR_REPORTFULL" ); break;
	case DIERR_UNPLUGGED: sprintf( buf, "DIERR_UNPLUGGED" ); break;
	case DIERR_UNSUPPORTED: sprintf( buf, "DIERR_UNSUPPORTED" ); break;
	case E_HANDLE: sprintf( buf, "E_HANDLE" ); break;
	case E_PENDING: sprintf( buf, "E_PENDING" ); break;
	case E_POINTER: sprintf( buf, "E_POINTER" ); break;
	default: sprintf( buf, "Unknown (%x)", ecode ); break;
	}
};

void dInputLocalbuf( std::ostream &ostr, HRESULT *dat ) {
	char buf[ 256 ];
	directInput8Error( *dat, buf );
	ostr << buf;
};

zutil::kStreamtoken< HRESULT > dInput8Stream( HRESULT ecode ) {
	return zutil::kStreamtoken< HRESULT >( ecode, dInputLocalbuf ); };