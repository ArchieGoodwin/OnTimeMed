/*!
 * \file	News.h
 * \ingroup base
 * \brief	new, delete ‡∫§ªÙ∂¶ º≥Ã©¬‹.
 * \author	±ÆΩËÀß
 */

#ifndef _NEWS_H_
#define _NEWS_H_

#include "cvlibbaseDef.h"

// using namespace CVLib;
// 
// CVLIB_DECLSPEC void* operator new(size_t nSize, char* szFilename, int nLine);
// CVLIB_DECLSPEC void* operator new[](size_t nSize, char* szFilename, int nLine);
// CVLIB_DECLSPEC void* operator new[](size_t nSize);
// CVLIB_DECLSPEC void operator delete(void* ptr, char* szFilename, int nLine);
// CVLIB_DECLSPEC void operator delete[](void* ptr, char* szFilename, int nLine);
// CVLIB_DECLSPEC void operator delete[](void* ptr);
// 

#if CVLIB_OS == CVLIB_OS_WIN32 || CVLIB_OS == CVLIB_OS_LINUX

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
CVLIB_DECLSPEC inline void* operator new(size_t nSize, void* ptr)
{
	return ptr;
}

CVLIB_DECLSPEC inline void operator delete(void* ptr, void* ptr1)
{
	return;
}

CVLIB_DECLSPEC inline void* operator new(size_t nSize, void* ptr, char* szFilename, int nLine)
{
	return ptr;
}

CVLIB_DECLSPEC inline void operator delete(void* ptr, void* ptr1, char* szFilename, int nLine)
{
	return;
}

#endif //__PLACEMENT_NEW_INLINE
#elif CVLIB_OS == CVLIB_OS_APPLE

#import <new>

#endif

#endif //_NEWS_H_