
/*!
 * \file	Plex.h
 * \ingroup base
 * \brief	»ôµê¸ó±ã¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _PLEX_H_
#define _PLEX_H_

#include "cvlibbaseDef.h"

namespace CVLib
{

struct CVLIB_DECLSPEC Plex     // warning variable length structure
{
	Plex* pNext;
	// BYTE data[maxNum*elementSize];
	
	void* data() { return this+1; }
	
	static Plex* Create(Plex*& head, UINT nMax, UINT cbElement);
	// like 'calloc' but no zero fill
	// may throw memory exceptions
	
	void FreeDataChain();       // free this one and links
};

}

#endif //_PLEX_H_