#include "SString.h"
#include "template.h"

#include <string.h>

#if CVLIB_OS != CVLIB_OS_APPLE && CVLIB_OS != CVLIB_OS_LINUX
  #include <tchar.h>
  #define CUCHAR const BYTE
#else
   #include <ctype.h>
   #define CUCHAR const BYTE
#endif

#if CVLIB_OS == CVLIB_OS_APPLE
#include <_types.h>
#define __int64 __int64_t
#elif CVLIB_OS == CVLIB_OS_LINUX
#include <sys/types.h>
#define __int64 int64_t
#endif

#define lstrlenA lstrlen

namespace CVLib
{
	
#if CVLIB_OS != CVLIB_OS_WIN32

#define _T 
	unsigned char* _mbsupr (unsigned char* str)
	{
		return str;
	}
	unsigned char* _mbslwr (unsigned char* str)
	{
		return str;
	}
	unsigned char* _mbsrev (unsigned char* str)
	{
		return str;
	}
	int _ismbblead (unsigned int c)
	{
		return (int)c;
	}
	int _ismbcspace(unsigned int c)
	{
		return (int)c;
	}
	
	size_t _tcsspn(const char* str1, const char* str2)
	{
		return 0;
	}
	
	int _ttoi (const char* str)
	{
		return atoi(str);
	}
	size_t _tcscspn(const char* str1, const char* str2)
	{
		return 0;
	}
	int _tclen(const char* str)
	{
		return strlen(str);
	}
	int _istdigit(int c)
	{
		return isdigit(c);
	}
	int _tcsncmp(const char* str1, const char* str2, size_t n)
	{
		return strncmp(str1, str2, n);
	}
	int wcslen (const wchar_t* str)
	{
		return 0;
	}
#endif
	
/*get the length in bytes(ANSI version)
or characters(Unicode version) of the specified string*/
static int lstrlen(const char* lpString)
{
  int nLen = 0;
  int c;
  char * pc;
  pc = (char *)lpString;
  DWORD bpreValue = FALSE;
  DWORD i;
  for( i = 0; i < strlen(pc); i++)
  {
    c = *pc++;
    if(c < 0x80)
      nLen++;
    else if(bpreValue)
    {
      nLen++;
      bpreValue = FALSE; continue;
    }
    else
    {
      bpreValue = TRUE; continue;
    }
  }
  return nLen;
}

static int InterlockedIncrement(int* pData)
{
  int i;
  i = (int) ++(*pData);
  if(i < 0)
	  return -1;
  else if(i > 0)
	  return 1;
  else
	  return 0;
}
static int InterlockedDecrement(int* pData)
{
  int i;
  i = (int) --(*pData);
  if(i < 0)
	  return -1;
  else if(i > 0)
	  return 1;
  else
	  return 0;
  
}

/////////////////////////////////////////////////////////////////////////////
// static class data, special inlines
// SafxChNil is left for backward compatibility
static char SafxChNil = '\0';

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
int _SafxInitData[] = { -1, 0, 0, 0 };
StringData* _SafxDataNil = (StringData*)&_SafxInitData;
char *_SafxPchNil = (char*)(((BYTE*)&_SafxInitData)+sizeof(StringData));
// special function to make SafxEmptyString work even during initialization

SString g_StrBuf;//Global SString buffer:MD_906 by pch

#define SafxEmptyString ((SString&)*(SString*)&_SafxPchNil)

template<> void ConstructElements1<SString> (SString* pElements, int nCount)
{
	for (; nCount--; ++pElements)
		memcpy(pElements, &SafxEmptyString, sizeof(*pElements));
}

template<> void DestructElements1<SString> (SString* pElements, int nCount)
{
	for (; nCount--; ++pElements)
		pElements->~SString();
}

template<> void CopyElements1<SString> (SString* pDest, const SString* pSrc, int nCount)
{
	for (; nCount--; ++pDest, ++pSrc)
		*pDest = *pSrc;
}

template<> UINT cvlHashKey<const SString&>(const SString& key)
{
	const char* data=key.GetLPCTSTR();
	UINT nHash = 0;
	while (*data)
		nHash = (nHash<<5) + nHash + *data++;
	return nHash;
}

SString& SGetEmptyString()
	{ return *(SString*)&_SafxPchNil; }

StringData* SString::GetData() const
{
	return ((StringData*)m_pchData)-1; 
}
void SString::Init()
{ 
	m_pchData = SafxEmptyString.m_pchData; 
}

SString::SString()
{
	m_pchData = SafxEmptyString.m_pchData; 
}

SString::SString(char* lpsz)
{ 
	Init();
	*this = (char *)lpsz; 
}

SString::SString(const char* lpsz)
{ 
	Init();
	*this = (char *)lpsz; 
}

int SString::GetLength() const
{ 
	return GetData()->nDataLength; 
}

int SString::GetAllocLength() 
{ 
	return GetData()->nAllocLength; 
}

DWORD SString::IsEmpty() const
{ 
	return GetData()->nDataLength == 0; 
}


int  SString::SafeStrlen(const char * lpsz) const
{ 
	return (lpsz == NULL) ? 0 : strlen(lpsz); 
}

// SString support (windows specific)
int SString::Compare(const char * lpsz) const
{ 
//	return _mbscmp((CUCHAR *)m_pchData, (CUCHAR *)lpsz); 
	return strcmp(m_pchData, lpsz); 
}
// MBCS/Unicode aware
int SString::Compare(const SString& str) const
{
	const char *ch;
	ch = str.GetLPCTSTR();
//	return _mbscmp((CUCHAR *)m_pchData, (CUCHAR *)ch);
	return strcmp(m_pchData, ch);
}

int SString::CompareNoCase(const char * lpsz) const
{ 
//	return _mbsicmp((CUCHAR *)m_pchData, (CUCHAR *)lpsz); 
	return strcmp(m_pchData, lpsz); 
}
// MBCS/Unicode aware
// SString::Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
int SString::Collate(char * lpsz) const
{ 
//	return _mbscoll((CUCHAR *)m_pchData, (CUCHAR *)lpsz); 
	return strcoll(m_pchData, lpsz); 
}   // locale sensitive

int SString::CollateNoCase(char * lpsz) const
{ 
//	return _mbsicoll((CUCHAR *)m_pchData, (CUCHAR *)lpsz); 
	return strcoll(m_pchData, lpsz); 
}   // locale sensitive

BYTE SString::GetAt(int nIndex) const
{
	return m_pchData[nIndex];
}

BYTE SString::operator[](int nIndex) const
{
	return m_pchData[nIndex];
}

SString::SString(const SString & stringSrc)
{
	if (stringSrc.GetData()->nRefs >= 0)
	{
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement(&GetData()->nRefs);
	}
	else
	{
		Init();
		*this = stringSrc.m_pchData;
	}
}
/*
#ifndef _DEBUG

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)
AFX_STATIC CFixedAlloc _afxAlloc64(ROUND4(65*sizeof(char)+sizeof(StringData)));
AFX_STATIC CFixedAlloc _afxAlloc128(ROUND4(129*sizeof(char)+sizeof(StringData)));
AFX_STATIC CFixedAlloc _afxAlloc256(ROUND4(257*sizeof(char)+sizeof(StringData)));
AFX_STATIC CFixedAlloc _afxAlloc512(ROUND4(513*sizeof(char)+sizeof(StringData)));

#endif //!_DEBUG
*/
void SString::AllocBuffer(int nLen)
// always allocate one extra character for '\0' termination
// assumes [optimistically] that data length will equal allocation length
{
	if (nLen == 0)
		Init();
	else
	{
		StringData* pData;
/*
#ifndef _DEBUG
		if (nLen <= 64)
		{
			pData = (StringData*)_afxAlloc64.Alloc();

			pData->nAllocLength = 64;
		}
		else if (nLen <= 128)
		{
			pData = (StringData*)_afxAlloc128.Alloc();

			pData->nAllocLength = 128;
		}
		else if (nLen <= 256)
		{
			pData = (StringData*)_afxAlloc256.Alloc();
			pData->nAllocLength = 256;
		}
		else if (nLen <= 512)
		{
			pData = (StringData*)_afxAlloc512.Alloc();
			pData->nAllocLength = 512;
		}
		else
#endif
*/
		{
			pData = (StringData*)
//				new BYTE[sizeof(StringData) + (nLen+1)*sizeof(char)];
				malloc(sizeof(StringData) + (nLen+1)*sizeof(char));
			pData->nAllocLength = nLen;
		}
		pData->nRefs = 1;
		pData->data()[nLen] = '\0';
		pData->nDataLength = nLen;
		m_pchData = pData->data();
	}
}

void SString::FreeData(StringData* pData)
{
/*
#ifndef _DEBUG
	int nLen = pData->nAllocLength;
	if (nLen == 64)
		_afxAlloc64.Free(pData);
	else if (nLen == 128)
		_afxAlloc128.Free(pData);
	else if (nLen == 256)
		_afxAlloc256.Free(pData);
	else  if (nLen == 512)
		_afxAlloc512.Free(pData);

	else
	{
		delete[] (BYTE*)pData;
	}
*/
//#else
//	delete[] (BYTE*)pData;
	free(pData);
//#endif
}

void SString::Release()
{
	if (GetData() != _SafxDataNil)
	{
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			FreeData(GetData());
		Init();
	}
}

void SString::Release(StringData* pData)
{
	if (pData != _SafxDataNil)
	{
		if (InterlockedDecrement(&pData->nRefs) <= 0)
			FreeData(pData);
	}
}

void SString::Empty()
{
	if (GetData()->nDataLength == 0)
		return;
	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = &SafxChNil;
}

void SString::CopyBeforeWrite()
{
	if (GetData()->nRefs > 1)
	{
		StringData* pData = GetData();
		Release();
		AllocBuffer(pData->nDataLength);
		memcpy(m_pchData, pData->data(), (pData->nDataLength+1)*sizeof(char));
	}
}

void SString::AllocBeforeWrite(int nLen)
{
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		AllocBuffer(nLen);
	}
}

SString::~SString()
//  free any attached data
{
	if (GetData() != _SafxDataNil)
	{
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			FreeData(GetData());
	}

}

//////////////////////////////////////////////////////////////////////////////
// Helpers for the rest of the implementation

void SString::AllocCopy(SString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
{
	// will clone the data attached to this string
	// allocating 'nExtraLen' characters
	// Places results in uninitialized string 'dest'
	// Will copy the part or all of original data to start of new string

	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	}
	else
	{
		dest.AllocBuffer(nNewLen);
		memcpy(dest.m_pchData, m_pchData+nCopyIndex, nCopyLen*sizeof(char));
	}
}

//////////////////////////////////////////////////////////////////////////////
// More sophisticated ruction


//////////////////////////////////////////////////////////////////////////////
// Assignment operators
//  All assign a new value to the string
//      (a) first see if the buffer is big enough
//      (b) if enough room, copy on top of old buffer, set size and type
//      (c) otherwise free old string data, and create a new one
//
//  All routines return the new string (but as a 'SString&' so that
//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
//

void SString::AssignCopy(int nSrcLen, const char* lpszSrcData)
{
	AllocBeforeWrite(nSrcLen);
	memcpy(m_pchData, lpszSrcData, nSrcLen*sizeof(char));
	GetData()->nDataLength = nSrcLen;
	m_pchData[nSrcLen] = '\0';
}

const SString& SString::operator=(const SString & stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != _SafxDataNil) ||
			stringSrc.GetData()->nRefs < 0)
		{
			// actual copy necessary since one of the strings is locked
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// can just copy references around
			Release();
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement(&GetData()->nRefs);
		}
	}
	return *this;
}

const SString& SString::operator=(char ch)
{
	if(ch == 0)
		return *this;
	AssignCopy(1, &ch);
	return *this;
}

const SString& SString::operator=(const char* lpsz)
{
	AssignCopy(SafeStrlen(lpsz), lpsz);
	return *this;
}

void SString::ConcatCopy(int nSrc1Len, const char* lpszSrc1Data, int nSrc2Len, const char* lpszSrc2Data)
{
	// -- master concatenation routine
	// Concatenate two sources
	// -- assume that 'this' is a new SString object

	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		AllocBuffer(nNewLen);
		memcpy(m_pchData, lpszSrc1Data, nSrc1Len*sizeof(char));
		memcpy(m_pchData+nSrc1Len, lpszSrc2Data, nSrc2Len*sizeof(char));
	}
}

void SString::ConcatInPlace(int nSrcLen, const char* lpszSrcData)
{
	//  -- the main routine for += operators

	// concatenating an empty string is a no-op!
	if (nSrcLen == 0)

		return;

	// if the buffer is too small, or we have a width mis-match, just
	//   allocate a new buffer (slow but sure)
	if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
	{
		// we have to grow the buffer, use the ConcatCopy routine
		StringData* pOldData = GetData();
		ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
		SString::Release(pOldData);
	}
	else
	{
		// fast concatenation when buffer big enough
		memcpy(m_pchData+GetData()->nDataLength, lpszSrcData, nSrcLen*sizeof(char));
		GetData()->nDataLength += nSrcLen;
		m_pchData[GetData()->nDataLength] = '\0';
	}
}

const SString& SString::operator+=(const char* lpsz)
{
	ConcatInPlace(SafeStrlen(lpsz), lpsz);
	return *this;
}

const SString& SString::operator+=(char ch)
{
	if(ch == 0)
		return *this;
	ConcatInPlace(1, &ch);
	return *this;
}

const SString& SString::operator+=(const SString& string)
{
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}


///////////////////////////////////////////////////////////////////////////////
// Advanced direct buffer access

char* SString::GetBuffer(int nMinBufLength)
{
	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
	{
		// we have to grow the buffer
		StringData* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;

		AllocBuffer(nMinBufLength);
		memcpy(m_pchData, pOldData->data(), (nOldLen+1)*sizeof(char));
		GetData()->nDataLength = nOldLen;
		SString::Release(pOldData);
	}
	return m_pchData;
}

void SString::ReleaseBuffer(int nNewLength)
{
	CopyBeforeWrite();  // just in case GetBuffer was not called

	if (nNewLength == -1)
		nNewLength = strlen(m_pchData); // zero terminated

	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
}

char* SString::GetBufferSetLength(int nNewLength)
{
	GetBuffer(nNewLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
	return m_pchData;
}

void SString::FreeExtra()
{
	if (GetData()->nDataLength != GetData()->nAllocLength)
	{
		StringData* pOldData = GetData();
		AllocBuffer(GetData()->nDataLength);
		memcpy(m_pchData, pOldData->data(), pOldData->nDataLength*sizeof(char));
		SString::Release(pOldData);
	}
}

char* SString::LockBuffer()
{
	char* lpsz = GetBuffer(0);
	GetData()->nRefs = -1;
	return lpsz;
}

void SString::UnlockBuffer()
{
	if (GetData() != _SafxDataNil)
		GetData()->nRefs = 1;
}

int SString::Find(char ch) const
{
	return Find(ch, 0);
}

int SString::Find(char ch, int nStart) const
{
	int nLength = GetData()->nDataLength;
	if (nStart >= nLength)
		return -1;

	// find first single character
	BYTE* lpsz = (BYTE*)cbschr((CUCHAR*)m_pchData + nStart, ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : ((int)lpsz - (int)m_pchData);//MD_906 by pch
}

int SString::Find(char *lpszSub) const
{
 	return Find(lpszSub, 0);
}

int SString::Find(SString *pStr) const
{
	return Find(pStr,0);
}

int SString::Find(SString *pStr, int nStart) const
{
	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;

	// find first matching substring
	char* lpsz = (char*)cbsstr((CUCHAR *)m_pchData + nStart, (CUCHAR *)pStr->m_pchData);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int SString::Find(SString Str, int nStart) const
{
	return Find(&Str,nStart);
}

int SString::Find(SString Str) const
{
	return Find(&Str);
}

int SString::GetCharCount(char *lpszSub, int nStart) const
{
	int count = 0;
	char *p;
	while( (nStart = Find(lpszSub, nStart)) != -1)
	{
		count++;
		p = (char*)cbsinc((CUCHAR *)m_pchData + nStart);
		nStart = p - m_pchData;
	}
	return count;

}

int SString::Find(char*lpszSub, int nStart) const
{

 	int nLength = GetData()->nDataLength;
 	if (nStart > nLength)
 		return -1;
	if(!strlen(lpszSub)) return -1;
 
 	// find first matching substring
 	char *lpsz = (char*)cbsstr((CUCHAR *)m_pchData + nStart, (CUCHAR *)lpszSub);
 
 	// return -1 for not found, distance from beginning otherwise
 	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);

}

int SString::ReverseFind(char ch) const
{
	// find last single character
//	char * lpsz = (char*)_mbsrchr((CUCHAR *)m_pchData,ch);
	char * lpsz = (char*)strrchr(m_pchData,ch);

	// return -1 if not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);

}

int SString::ReverseFind(char* lpszSub, int nBackStart) const
{
	// find last single character
	int nRightStart = 0;
	int nBoundLen = strlen(m_pchData) - nBackStart - strlen(lpszSub);

	int SearchPos = -1;
	while(1){
		nRightStart = Find(lpszSub, nRightStart);

		if(nRightStart == -1) break;
		if(nRightStart > nBoundLen) break;
		SearchPos = nRightStart;
		if(nRightStart == nBoundLen) break;
		( *((BYTE*)(m_pchData + nRightStart))) >= 0x80 ? nRightStart += 2 : nRightStart++;
		// ATTENTION 2BYTE Process
	}

	// return -1 if not found, distance from beginning otherwise
	return SearchPos;
}

int SString::FindOneOf(const char* lpszCharSet, int nStart) const
{
//	char* lpsz = (char*)_mbspbrk((CUCHAR *)m_pchData + nStart,(CUCHAR *)lpszCharSet);
	char* lpsz = (char*)strpbrk(m_pchData + nStart,lpszCharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

void SString::MakeUpper()
{
	CopyBeforeWrite();
	_mbsupr((BYTE *)m_pchData);
}

void SString::MakeLower()
{
	CopyBeforeWrite();
	_mbslwr((BYTE *)m_pchData);
}

void SString::MakeReverse()
{
	CopyBeforeWrite();
	_mbsrev((BYTE *)m_pchData);
}

void SString::SetAt(int nIndex, char ch)
{
	CopyBeforeWrite();
	m_pchData[nIndex] = ch;
}

int SString::Delete(int nIndex, int nCount /* = 1 */)
{
	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		CopyBeforeWrite();
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

		memcpy(m_pchData + nIndex,
			m_pchData + nIndex + nCount, nBytesToCopy * sizeof(char));
		GetData()->nDataLength = nNewLength - nCount;
	}

	return nNewLength;
}

int SString::Insert(int nIndex, char ch)
{
	CopyBeforeWrite();

	if (nIndex < 0)
		nIndex = 0;

	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength += 2;


	if (GetData()->nAllocLength < nNewLength)
	{
		StringData* pOldData = GetData();
		char *pstr = m_pchData;
		AllocBuffer(nNewLength);
		memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(char));

		SString::Release(pOldData);
	}

	// move existing bytes down
	memcpy(m_pchData + nIndex + 1,
		m_pchData + nIndex, (nNewLength-nIndex)*sizeof(char));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;

	return nNewLength;
}
int SString::Insert(int nIndex, char * pstr)
{
	if (nIndex < 0)
		nIndex = 0;

	int nInsertLength = SafeStrlen(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite();
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength + 2;

		if (GetData()->nAllocLength < nNewLength)
		{
			StringData* pOldData = GetData();
			char *pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(char));
			SString::Release(pOldData);
		}


		// move existing bytes down
		memcpy(m_pchData + nIndex + nInsertLength,
			m_pchData + nIndex,
			(nNewLength-nIndex-nInsertLength+1)*sizeof(char));
		*(m_pchData + nNewLength) = 0; //2004.6.11 By AJN
		memcpy(m_pchData + nIndex,
			pstr, nInsertLength*sizeof(char));
		GetData()->nDataLength = nNewLength;
	}

	return nNewLength;
}

/****************************************************************/
//MD_906 by pch
SString SString::Mid(int nFirst) const
{
	int nCount;
	nCount = GetData()->nDataLength - nFirst;
	return Mid(nFirst, nCount);
}

SString SString::Mid(int nFirst, int nCount) const
{
	// out-of-bounds requests return sensible things
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength - nFirst;
	if (nFirst > GetData()->nDataLength)
		nCount = 0;

	// optimize case of returning entire string
	if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
		return *this;
	SString dest;
	AllocCopy(dest, nCount, nFirst, 0);
	g_StrBuf = dest;
	return g_StrBuf;
}

SString SString::Right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;
	
	SString dest;
	AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
	g_StrBuf = dest;
	return g_StrBuf;
}

SString SString::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;

	SString dest;
	AllocCopy(dest, nCount, 0, 0);
	g_StrBuf = dest;
	return g_StrBuf;
}
/*****************************************************************/

void SString::TrimRight(char * lpszTargetList)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)

	CopyBeforeWrite();
	BYTE* lpsz = (BYTE*)m_pchData;
	BYTE* lpszLast = NULL;

	while (*lpsz != '\0')
	{
		WORD code = *lpsz;
		if(_ismbblead(code)) {
			code = (code << 8) + *(lpsz + 1);
		}
		if (cbschr((CUCHAR *)lpszTargetList, code) != NULL)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = (BYTE *)cbsinc((CUCHAR *)lpsz);
	}

	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (DWORD)lpszLast - (DWORD)m_pchData;
	}
}


void SString::TrimRight(char chTarget)
{
	// find beginning of trailing matches
	// by starting at beginning (DBCS aware)

	CopyBeforeWrite();
	BYTE * lpsz = (BYTE*)m_pchData;
	BYTE * lpszLast = NULL;

	WORD code = *lpsz;
	if(_ismbblead(code)) {
		code = (code << 8) + *(lpsz + 1);
	}


	while (code != '\0'){
		if (code == chTarget)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = (BYTE*)cbsinc((CUCHAR *)lpsz);
		code = *lpsz;
		if(_ismbblead(code)) {
			code = (code << 8) + *(lpsz + 1);
		}
	}

	if (lpszLast != NULL)
	{
		// truncate at left-most matching character
		*lpszLast = '\0';
		GetData()->nDataLength = (DWORD)lpszLast - (DWORD)m_pchData;
	}
}

void SString::TrimRight()
{
	// find beginning of trailing spaces by starting at beginning (DBCS aware)

	CopyBeforeWrite();
	BYTE * lpsz = (BYTE*)m_pchData;

	BYTE * lpszLast = NULL;

	WORD code = *lpsz;
	if(_ismbblead(code)) {
		code = (code << 8) + *(lpsz + 1);
	}
	while (code != '\0'){
		if (_ismbcspace(code)){
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz = (BYTE*)cbsinc((CUCHAR *)lpsz);

		code = *lpsz;
		if(_ismbblead(code)) {
			code = (code << 8) + *(lpsz + 1);
		}
	}

	if (lpszLast != NULL)
	{
		// truncate at trailing space start
		*lpszLast = '\0';
		GetData()->nDataLength = (DWORD)lpszLast - (DWORD)m_pchData;
	}
}

void SString::TrimLeft(char * lpszTargets)
{
	// if we're not trimming anything, we're not doing any work
	if (SafeStrlen(lpszTargets) == 0)
		return;

	CopyBeforeWrite();
	BYTE * lpsz = (BYTE*)m_pchData;
	WORD code = *lpsz;
	if(_ismbblead(code)) {
		code = (code << 8) + *(lpsz + 1);
	}
	while (*lpsz != '\0'){
		
		if (cbschr((CUCHAR *)lpszTargets, code) == NULL)
			break;
		lpsz = (BYTE*)cbsinc((CUCHAR *)lpsz);
		code = *lpsz;
		if(_ismbblead(code)) {
			code = (code << 8) + *(lpsz + 1);
		}
	}

	if (lpsz != (BYTE*)m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - ((DWORD)lpsz - (DWORD)m_pchData);
		memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(char));
		GetData()->nDataLength = nDataLength;
	}
}

void SString::TrimLeft(char chTarget)
{
	// find first non-matching character

	CopyBeforeWrite();
	char * lpsz = m_pchData;

	while (chTarget == *lpsz)
	{
		lpsz = (char*)cbsinc((CUCHAR *)lpsz);
	}

	if (lpsz != m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(char));
		GetData()->nDataLength = nDataLength;
	}
}

void SString::TrimLeft()
{
	// find first non-space character

	CopyBeforeWrite();
	BYTE * lpsz = (BYTE*)m_pchData;
	WORD code = *lpsz;
	if(_ismbblead(code))
		code = (code << 8) + *(lpsz + 1);

	while (_ismbcspace(code))
	{
		lpsz = (BYTE*)cbsinc((CUCHAR *)lpsz);
		code = *lpsz;
		if(_ismbblead(code))
			code = (code << 8) + *(lpsz + 1);
	}

	if (lpsz != (BYTE*)m_pchData)
	{
		// fix up data and length
		int nDataLength = GetData()->nDataLength - ((DWORD)lpsz - (DWORD)m_pchData);
		memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(char));
		GetData()->nDataLength = nDataLength;
	}
}

int SString::Remove(char chRemove)
{
	CopyBeforeWrite();

	char * pstrSource = m_pchData;
	char * pstrDest = m_pchData;
	char * pstrEnd = m_pchData + GetData()->nDataLength;

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = (char*)cbsinc((CUCHAR *)pstrDest);
		}
		pstrSource = (char*)cbsinc((CUCHAR *)pstrSource);
	}
	*pstrDest = '\0';
	int nCount = pstrSource - pstrDest;

	GetData()->nDataLength -= nCount;

	return nCount;
}
int SString::Replace(char chOld, char chNew)

{
	int nCount = 0;

	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		char * psz = m_pchData;
		char * pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}

			psz = (char*)cbsinc((CUCHAR *)psz);
		}
	}
	return nCount;
}

int SString::Replace(SString Old, SString New)
{
	return Replace(Old.m_pchData, New.m_pchData);
}


int SString::Replace(char * lpszOld, char * lpszNew)
{
	// can't have empty or NULL lpszOld

	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	char * lpszStart = m_pchData;
	char * lpszEnd = m_pchData + GetData()->nDataLength;
	char * lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = (char*)cbsstr((CUCHAR *)lpszStart, (CUCHAR *)lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}

		lpszStart += strlen(lpszStart) + 1;
	}

	// if any changes were made, make them
	if (nCount > 0)
	{
		CopyBeforeWrite();

		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen-nSourceLen)*nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			StringData* pOldData = GetData();
			char * pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, pOldData->nDataLength*sizeof(char));
			SString::Release(pOldData);
		}

		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;

		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = (char*)cbsstr((CUCHAR *)lpszStart, (CUCHAR *)lpszOld)) != NULL)
			{
				int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
					nBalance * sizeof(char));
				memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(char));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += strlen(lpszStart) + 1;
		}
		GetData()->nDataLength = nNewLength;
	}

	return nCount;
}

SString::SString(char ch, int nLength)
{
	Init();
	if (nLength >= 1)
	{
		AllocBuffer(nLength);
#ifdef _UNICODE
      int i;
		for ( i = 0; i < nLength; i++)
			m_pchData[i] = ch;
#else
		memset(m_pchData, ch, nLength);
#endif
	}

}

SString::SString(char * lpch, int nLength)
{
	Init();
	if (nLength != 0)
	{
		AllocBuffer(nLength);
		memcpy(m_pchData, lpch, nLength*sizeof(char));
	}
}

SString SString::SpanIncluding(const char * lpszCharSet) const
{//MD_906 by pch
	return Left(_tcsspn(m_pchData, lpszCharSet));
}


SString SString::SpanExcluding(const char * lpszCharSet) const
{//MD_906 by pch
	return Left(_tcscspn(m_pchData, lpszCharSet));
}

//#define TCHAR_ARG   TCHAR//MD_821 by pch
#define WCHAR_ARG   unsigned short
#define CHAR_ARG    char

#define DOUBLE_ARG  double

#define FORCE_ANSI      0x10000

#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000


//MD_817 by pch
void SString::FormatV(char * lpszFormat, va_list argList)
{
	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	//MD_820 by pch
	SString str(lpszFormat);
	const char * lpsz;
	for (lpsz = str.GetLPCTSTR(); *lpsz != '\0'; lpsz = (char*)cbsinc((CUCHAR *)lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = (char*)cbsinc((CUCHAR *)lpsz)) == '%')
		{
			nMaxLen += _tclen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;

		for (; *lpsz != '\0'; lpsz = (char*)cbsinc((CUCHAR *)lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != '\0' && _istdigit(*lpsz);
			lpsz = (char*)cbsinc((CUCHAR *)lpsz));
		}

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = (char*)cbsinc((CUCHAR *)lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = (char*)cbsinc((CUCHAR *)lpsz);
			}

			else
			{
				nPrecision = _ttoi(lpsz);
				for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = (char*)cbsinc((CUCHAR *)lpsz))
					;
			}

		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = (char*)cbsinc((CUCHAR *)lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = (char*)cbsinc((CUCHAR *)lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = (char*)cbsinc((CUCHAR *)lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		/*
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		*/
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG );
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:

			nItemLen = 2;
			va_arg(argList, WCHAR_ARG );
			break;

		// strings
		case 's':
			{
				char* pstrNextArg = va_arg(argList, char*);

				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = lstrlen(pstrNextArg);
				   nItemLen = MAX(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
#ifndef _UNICODE
				WORD* pstrNextArg = va_arg(argList, WORD* );
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = wcslen((const wchar_t*)pstrNextArg);
				   nItemLen = MAX(1, nItemLen);
				}
#else
				const char* pstrNextArg = va_arg(argList, const char* );
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"

				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = MAX(1, nItemLen);
				}
#endif
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				const char*  pstrNextArg = va_arg(argList, const char* );
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = MAX(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{

				WORD* pstrNextArg = va_arg(argList, WORD* );
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = wcslen((const wchar_t*)pstrNextArg);
				   nItemLen = MAX(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = MIN(nItemLen, nPrecision);
			nItemLen = MAX(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':

			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = MAX(nItemLen, nWidth+nPrecision);
				break;


			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = MAX(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128; // width isn't truncated
				// 312 == strlen("-1+(309 zeroes).")
				// 309 zeroes == MAX precision of a double
				nItemLen = MAX(nItemLen, 312+nPrecision);
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = MAX(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	GetBuffer(nMaxLen);
//	VERIFY(_vstprintf(m_pchData, lpszFormat, argListSave) <= GetAllocLength());
	sprintf(m_pchData, lpszFormat, argListSave);
	ReleaseBuffer();

	va_end(argListSave);
}

// formatting (using wsprintf style formatting)
void SString::Format(char * lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

bool operator ==( const SString& s1, const SString& s2 )
{
	if (s1.Compare(s2) == 0)
		return true;
	return false;
}
bool operator==(const SString& s1, const char* s2)
{
	if (s1.Compare(s2) == 0)
		return true;
	return false;
}
bool operator==(const char* s1, const SString& s2)
{
	if (s2.Compare(s1) == 0)
		return true;
	return false;
}
bool operator!=(const SString& s1, const SString& s2)
{
	if (s1.Compare(s2) != 0)
		return true;
	return false;
}
bool operator!=(const SString& s1, const char* s2)
{
	if (s1.Compare(s2) != 0)
		return true;
	return false;
}
bool operator!=(const char* s1, const SString& s2)
{
	if (s2.Compare(s1) != 0)
		return true;
	return false;
}
bool operator<(const SString& s1, const SString& s2)
{
	if (s1.Compare(s2) < 0)
		return true;
	return false;
}
bool operator<(const SString& s1, const char* s2)
{
	if (s1.Compare(s2) < 0)
		return true;
	return false;
}
bool operator<(const char* s1, const SString& s2)
{
	if (s2.Compare(s1) > 0)
		return true;
	return false;
}
bool operator>(const SString& s1, const SString& s2)
{
	if (s1.Compare(s2) > 0)
		return true;
	return false;
}
bool operator>(const SString& s1, const char* s2)
{
	if (s1.Compare(s2) > 0)
		return true;
	return false;
}
bool operator>(const char* s1, const SString& s2)
{
	if (s2.Compare(s1) < 0)
		return true;
	return false;
}
bool operator<=(const SString& s1, const SString& s2)
{
	if (s1.Compare(s2) <= 0)
		return true;
	return false;
}
bool operator<=(const SString& s1, const char* s2)
{
	if (s1.Compare(s2) <= 0)
		return true;
	return false;
}
bool operator<=(const char* s1, const SString& s2)
{
	if (s2.Compare(s1) >= 0)
		return true;
	return false;
}
bool operator>=(const SString& s1, const SString& s2)
{
	if (s1.Compare(s2) >= 0)
		return true;
	return false;
}
bool operator>=(const SString& s1, const char* s2)
{
	if (s1.Compare(s2) >= 0)
		return true;
	return false;
}
bool operator>=(const char* s1, const SString& s2)
{
	if (s2.Compare(s1) <= 0)
		return true;
	return false;
}

SString operator+(const SString& string1, const SString& string2)
{
	SString dest = string1;
	dest += string2;
	return dest;
}
SString operator+(const SString& string, char ch)
{
	SString dest = string;
	dest += ch;
	return dest;
}
SString operator+(char ch, const SString& string)
{
	SString dest = string;
	dest += ch;
	return dest;
}
SString operator+(const SString& string, const char* lpsz)
{
	SString dest = string;
	dest += lpsz;
	return dest;
}
SString operator+(const char* lpsz, const SString& string)
{
	SString dest = string;
	dest += lpsz;
	return dest;
}


int SString::FromFile(FILE* pfile)
{
	int nlength;
	fread (&nlength, sizeof(nlength), 1, pfile);
	AllocBeforeWrite(nlength+1);
	fread (GetBuffer(0), nlength, 1, pfile);
	GetBuffer(0)[nlength]=0;
	return 1;
}
int SString::ToFile(FILE* pfile)
{
	int nlength;
	fwrite (&nlength, sizeof(nlength), 1, pfile);
	fwrite (GetBuffer(0), nlength, 1, pfile);
	return 1;
}

}

//////////////////////////////////////////////////////////////////////////////
// less common string expressions




