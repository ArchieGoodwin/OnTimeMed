/*!
 * \file	News.cpp
 * \ingroup base
 * \brief	new, delete ‡∫§ªÙ∂¶ º≥Ã©¬Ÿ≥ﬁ.
 * \author	±ÆΩËÀß
 */

#include "News.h"
#include "cvlibMalloc.h"

// void* __cdecl operator new(size_t nSize, char* szFilename, int nLine)
// {
// 	void* ptr;
// #if CVLIB_MEMCHECK
// 	assert ((nSize) != 0);
// 	(ptr) = malloc (nSize);
// 	AddMemblock(ptr, nSize, szFilename, nLine, 0);
// #else
// 	ptr = malloc(nSize);
// #endif
// 	return ptr;
// }
// 
// void* __cdecl operator new[](size_t nSize, char* szFilename, int nLine)
// {
// 	void* ptr;
// #if CVLIB_MEMCHECK
// 	assert ((nSize) != 0);
// 	(ptr) = malloc (nSize);
// 	AddMemblock(ptr, nSize, szFilename, nLine, 0);
// #else
// 	ptr = malloc(nSize);
// #endif
// 	return ptr;
// }
// 
// void operator delete(void* ptr, char* szFilename, int nLine)
// {
// #if CVLIB_MEMCHECK
// 	FreeMembolck (ptr, 0);
// #else
// 	free (ptr);
// #endif
// }
// 
// void operator delete[](void* ptr, char* szFilename, int nLine)
// {
// #if CVLIB_MEMCHECK
// 	FreeMembolck (ptr, 0);
// #else
// 	free (ptr);
// #endif
// }
// 
// // void* operator new(size_t nSize)
// // {
// // 	void* ptr;
// // #if CVLIB_MEMCHECK
// // 	assert ((nSize) != 0);
// // 	(ptr) = malloc (nSize);
// // 	AddMemblock(ptr, nSize, NULL, 0, 0);
// // #else
// // 	ptr = malloc(nSize);
// // #endif
// // 	return ptr;
// // }
// 
// void* operator new[](size_t nSize)
// {
// 	void* ptr;
// #if CVLIB_MEMCHECK
// 	assert ((nSize) != 0);
// 	(ptr) = malloc (nSize);
// 	AddMemblock(ptr, nSize, NULL, 0, 0);
// #else
// 	ptr = malloc(nSize);
// #endif
// 	return ptr;
// }
// 
// // void operator delete(void* ptr)
// // {
// // #if CVLIB_MEMCHECK
// // 	FreeMembolck (ptr, 0);
// // #else
// // 	free (ptr);
// // #endif
// // }
// 
// void operator delete[](void* ptr)
// {
// #if CVLIB_MEMCHECK
// 	FreeMembolck (ptr, 0);
// #else
// 	free (ptr);
// #endif
// }
// 
