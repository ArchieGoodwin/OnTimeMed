/*!
 * \file	Object.cpp
 * \ingroup base
 * \brief	·º´Ç ¿Ðµ¹º÷Ì© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ Object¿Ðµ¹º÷¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include "Object.h"
#include "cvlibMalloc.h"

namespace CVLib 
{

Object::Object()
{
}

Object::~Object()
{
}

int	Object::FromFile (const char* szFilename)
{
	int nFlag = 0;
	FILE* pFile = fopen (szFilename, "rb");
	if (pFile != NULL)
	{
		nFlag = FromFile (pFile);
		fclose (pFile);
	}
	return nFlag;
}

int	Object::ToFile (const char* szFilename) const
{
	int nFlag = 0;
	FILE* pFile = fopen (szFilename, "wb");
	if (pFile != NULL)
	{
		nFlag = ToFile(pFile);
		fclose (pFile);
	}
	return nFlag;
}

int	Object::FromFile (FILE* pFile)
{
	return 0;
}

int	Object::ToFile (FILE* pFile) const
{
	return 0;
}

// int	Object::FromFile (FILE* pFile)
// {
// 	XFileDisk xfile(pFile);
// 	return FromFile(&xfile);
// }
// 
// int	Object::ToFile (FILE* pFile) const
// {
// 	XFileDisk xfile(pFile);
// 	return ToFile(&xfile);
// }
// 
// int	Object::FromFile (XFile* pFile)
// {
// 	return 0;
// }
// 
// int	Object::ToFile (XFile* pFile) const
// {
// 	return 0;
// }

void* Object::operator new(size_t nSize)
{
	void* ptr;
#if CVLIB_MEMCHECK
	MallocOBJECT (ptr, nSize);
#else
	ptr = malloc(nSize);
#endif
	return ptr;
}

void* Object::operator new(size_t nSize, char* szFilename, int nLine)
{
	void* ptr;
#if CVLIB_MEMCHECK
	assert ((nSize) != 0);
	(ptr) = malloc (nSize);
	AddMemblock(ptr, nSize, szFilename, nLine, 1);
#else
	ptr = malloc(nSize);
#endif
	return ptr;
}

void* Object::operator new[](size_t nSize)
{
	void* ptr;
#if CVLIB_MEMCHECK
	MallocOBJECT (ptr, nSize);
#else
	ptr = malloc(nSize);
#endif
	return ptr;
}

void* Object::operator new[](size_t nSize, char* szFilename, int nLine)
{
	void* ptr;
#if CVLIB_MEMCHECK
	assert ((nSize) != 0);
	(ptr) = malloc (nSize);
	AddMemblock(ptr, nSize, szFilename, nLine, 1);
#else
	ptr = malloc(nSize);
#endif
	return ptr;
}

void Object::operator delete(void *ptr, char* szFilename, int nLine)
{
#if CVLIB_MEMCHECK
	FreeMembolck (ptr, 1);
#else
	free (ptr);
#endif
}

void Object::operator delete[](void *ptr, char* szFilename, int nLine)
{
#if CVLIB_MEMCHECK
	FreeMembolck (ptr, 1);
#else
	free (ptr);
#endif
}

void Object::operator delete(void *ptr)
{
#if CVLIB_MEMCHECK
	FreeMembolck (ptr, 1);
#else
	free (ptr);
#endif
}

void Object::operator delete[](void *ptr)
{
#if CVLIB_MEMCHECK
	FreeMembolck (ptr, 1);
#else
	free (ptr);
#endif
}

}
