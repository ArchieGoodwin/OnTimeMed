
/*!
 * \file	Template.h
 * \ingroup base
 * \brief	¹ÉµÛ¿Ðµ¹º÷ÂôÀ°¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#include "cvlibbaseDef.h"
#include "Object.h"
#include "News.h"
#include "SString.h"
#include "Plex.h"


namespace CVLib
{

/**
 * @brief   ¼ÑÊÌ¼ê ¿Í±¨Ì© ÌººÏ¹ÉµÛË¾ Ãú¸óÂÙ³Þ.
 *
 * @param   pElements [in] : ¹ÉµÛÌ© ¼ÑºÏ 
 * @param   nCount    [in] : ¹ÉµÛÌ© ¿Í±¨ 
 * @return  void 
 * 
 */
template<class TYPE>
void CVLIB_DECLSPEC ConstructElements1(TYPE* pElements, int nCount)
{
	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	// then call the constructor(s)
 	for (; nCount--; pElements++)
		::new((void*)pElements) TYPE;
}

template<class TYPE>
void CVLIB_DECLSPEC ConstructElements1(TYPE* pElements, int nCount, const TYPE& newelem)
{
	// then call the constructor(s)
	for (; nCount--; pElements++)
	{
		::new((void*)pElements) TYPE;
		*pElements = newelem;
	}
}
/**
 * @brief   ÌººÏ¹ÉµÛËæº· ¼ÑÊÌ¼ê ±¶ºã¶í¿Ñ ÌººÏ¶¦ º¢½£ÂÙ³Þ.
 *
 * @param   pElements [in] : ¹ÉµÛÌ© ¼ÑºÏ 
 * @param   nCount    [in] : ±¶ºã 
 * @return  void 
 * 
 */
template<class TYPE>
void CVLIB_DECLSPEC DestructElements1(TYPE* pElements, int nCount)
{
	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}

/**
 * @brief   ¼ÑÊÌ¼ê ¿Í±¨¶í¿Ñ ÌººÏ¶¦ ¸ôº¡ÂÙ³Þ.
 *
 * @param   pDest  [in] : ·»¼¬¼ÑºÏ 
 * @param   pSrc   [in] : Ìº½ç¼ÑºÏ 
 * @param   nCount [in] :¡¡¿Í±¨¡¡
 * @return  void
 * 
 */
template<class TYPE>
void CVLIB_DECLSPEC CopyElements1(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	// default is element-copy using assignment
	while (nCount--)
		*pDest++ = *pSrc++;
}

/**
 * @brief   ´¸ ÌººÏÌ© ¹¾°ä¶¦ ¼êÃÔÂÙ³Þ.
 *
 * @param   pElement1 [in] : ÌººÏ1
 * @param   pElement2 [in] : ÌººÏ2
 * @return  ´¸ ÌººÏ°¡ °¯Ëº·² false, ³Þ¶£·² ture
 * 
 */
template<class TYPE, class ARG_TYPE>
BOOL CVLIB_DECLSPEC CompareElements1(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	return *pElement1 == *pElement2;
}

template<class ARG_KEY>
inline UINT CVLIB_DECLSPEC cvlHashKey(ARG_KEY key)
{
	// default identity hash - works for most primitive values
	return ((UINT)(void*)(DWORD)key) >> 4;
}

#if CVLIB_OS==CVLIB_OS_WIN32

#if _MSC_VER >= 1100
template<> void CVLIB_DECLSPEC ConstructElements1<SString> (SString* pElements, int nCount);
template<> void CVLIB_DECLSPEC DestructElements1<SString> (SString* pElements, int nCount);
template<> void CVLIB_DECLSPEC CopyElements1<SString> (SString* pDest, const SString* pSrc, int nCount);
template<> UINT CVLIB_DECLSPEC cvlHashKey<const SString&>(const SString& key);
#else // _MSC_VER >= 1100
void CVLIB_DECLSPEC ConstructElements1(SString* pElements, int nCount);
void CVLIB_DECLSPEC DestructElements1(SString* pElements, int nCount);
void CVLIB_DECLSPEC CopyElements1(SString* pDest, const SString* pSrc, int nCount);
UINT CVLIB_DECLSPEC cvlHashKey(const SString& key);
#endif

#elif CVLIB_OS==CVLIB_OS_APPLE
template<> void CVLIB_DECLSPEC ConstructElements1<SString> (SString* pElements, int nCount);
template<> void CVLIB_DECLSPEC DestructElements1<SString> (SString* pElements, int nCount);
template<> void CVLIB_DECLSPEC CopyElements1<SString> (SString* pDest, const SString* pSrc, int nCount);
template<> UINT CVLIB_DECLSPEC cvlHashKey<const SString&>(const SString& key);
#endif
/////////////////////////////////////////////////////////////////////////////
// #define new new(__FILE__, __LINE__)

/**
 * @brief  ¹ÉµÛ¿Ðµ¹º÷ÂôÀ° 
 */
template<class TYPE, class ARG_TYPE=const TYPE&>
class CVLIB_DECLSPEC Array : public Object
{
public:
	typedef Array<TYPE, ARG_TYPE> _MyArray;
public:
    //! °é¾¢»ô 
	Array();
	Array(int nSize, const TYPE& _V=TYPE());
	Array(const _MyArray& other)
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
		Append(other);
	}
	_MyArray& operator=(const _MyArray& other)
	{
		if (this == &other)
			return *this;
		RemoveAll();
		Append(other);
		return *this;
	}

// Attributes
	
	int GetSize() const;
	int Length() const;

	int GetUpperBound() const;

	void SetSize(int nNewSize, int nGrowBy = -1);
	void Resize (int nNewSize, ARG_TYPE newElement=TYPE());
// Operations
	void FreeExtra();

	void RemoveAll();

	// Accessing elements
	TYPE GetAt(int nIndex) const;
	TYPE& GetAt(int nIndex);

	void SetAt(int nIndex, ARG_TYPE newElement);

	TYPE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;

	TYPE* GetData();

	void SetAtGrow(int nIndex, ARG_TYPE newElement);

	int Add(ARG_TYPE newElement);

	int Append(const Array& src);

	void Copy(const Array& src);

	// overloaded operator helpers

	TYPE operator[](int nIndex) const;

	TYPE& operator[](int nIndex);

	// Operations that move elements around

	void InsertAt(int nIndex, ARG_TYPE newElement=TYPE(), int nCount = 1);

	void RemoveAt(int nIndex, int nCount = 1);

	void InsertAt(int nStartIndex, Array* pNewArray);
	void InsertAt(int nStartIndex, const _MyArray& newArray);
	void Swap (_MyArray& aray);

	TYPE& Front() {return GetAt(0);}
//	const TYPE& Front() const {return GetAt(0);}
	TYPE& Back() {return GetAt(GetSize()-1);}
//	const TYPE& Back() const {return GetAt(GetSize()-1);}

	void PushBack(ARG_TYPE _X) {Add(_X);}
	void PopBack() {RemoveAt(GetSize()-1);}

	bool IsEmpty() const;

// Implementation
protected:
	//! »ôµê¹ÉµÛ 
	TYPE* m_pData;   
	//! ¹ÉµÛÌ© ÊýºÏºã 
	int m_nSize;     // # of elements (upperBound - 1)
	//! ¹ÉµÛÌ© ¾×´ÝÊýºÏºã 
	int m_nMaxSize;  
	//! ¹ÉµÛÌ© »þºÂºã 
	int m_nGrowBy;   

public:
	//! ÃÍ¾Ë»ô 
	~Array();
};

/////////////////////////////////////////////////////////////////////////////
// Array<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
 int Array<TYPE, ARG_TYPE>::GetSize() const
	{ return m_nSize; }
template<class TYPE, class ARG_TYPE>
 int Array<TYPE, ARG_TYPE>::Length() const
	{ return m_nSize; }
template<class TYPE, class ARG_TYPE>
 int Array<TYPE, ARG_TYPE>::GetUpperBound() const
	{ return m_nSize-1; }
template<class TYPE, class ARG_TYPE>
 void Array<TYPE, ARG_TYPE>::RemoveAll()
	{ SetSize(0, -1); }
template<class TYPE, class ARG_TYPE>
 TYPE Array<TYPE, ARG_TYPE>::GetAt(int nIndex) const
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
 TYPE& Array<TYPE, ARG_TYPE>::GetAt(int nIndex)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
 void Array<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
 TYPE& Array<TYPE, ARG_TYPE>::ElementAt(int nIndex)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
 const TYPE* Array<TYPE, ARG_TYPE>::GetData() const
	{ return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
 TYPE* Array<TYPE, ARG_TYPE>::GetData()
	{ return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
 int Array<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
template<class TYPE, class ARG_TYPE>
 TYPE Array<TYPE, ARG_TYPE>::operator[](int nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
 TYPE& Array<TYPE, ARG_TYPE>::operator[](int nIndex)
	{ return ElementAt(nIndex); }
template<class TYPE, class ARG_TYPE>
 bool Array<TYPE, ARG_TYPE>::IsEmpty() const {return m_nSize==0;}

/////////////////////////////////////////////////////////////////////////////
// Array<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class ARG_TYPE>
Array<TYPE, ARG_TYPE>::Array()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

template<class TYPE, class ARG_TYPE>
Array<TYPE, ARG_TYPE>::Array(int nSize, const TYPE& _V)
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
	SetSize(nSize);
	for (int i=0; i<GetSize(); i++)
		m_pData[i] = _V;
}

template<class TYPE, class ARG_TYPE>
Array<TYPE, ARG_TYPE>::~Array()
{
	assert(this);

	if (m_pData != NULL)
	{
		DestructElements1<TYPE>(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
	}
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
	assert(this);
	assert(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			DestructElements1<TYPE>(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		assert(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];
		ConstructElements1<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			ConstructElements1<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements1<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		assert(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		assert(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		assert(nNewSize > m_nSize);
		ConstructElements1<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::Resize(int nNewSize, ARG_TYPE elem)
{
	assert(this);
	assert(nNewSize >= 0);

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			DestructElements1<TYPE>(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		assert(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];
		ConstructElements1<TYPE>(m_pData, nNewSize, elem);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			ConstructElements1<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize, elem);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements1<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		assert(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		assert(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		assert(nNewSize > m_nSize);
		ConstructElements1<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize, elem);

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class ARG_TYPE>
int Array<TYPE, ARG_TYPE>::Append(const Array& src)
{
	assert(this);
	assert(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	CopyElements1<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::Copy(const Array& src)
{
	assert(this);
	assert(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);
	CopyElements1<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::FreeExtra()
{
	assert(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		assert(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) new BYTE[m_nSize * sizeof(TYPE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	assert(this);
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
	assert(this);
	assert(nIndex >= 0);    // will expand to meet need
	assert(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		DestructElements1<TYPE>(&m_pData[nOldSize], nCount);
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(TYPE));

		// re-init slots we copied from
		ConstructElements1<TYPE>(&m_pData[nIndex], nCount);
	}

	// insert new value in the gap
	assert(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	assert(this);
	assert(nIndex >= 0);
	assert(nCount >= 0);
	assert(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements1<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, Array* pNewArray)
{
	assert(this);
	assert(pNewArray != NULL);
	assert(pNewArray);
	assert(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, const _MyArray& newArray)
{
	assert(this);
	assert(nStartIndex >= 0);
	
	if (newArray.GetSize() > 0)
	{
		int nCount=newArray.GetSize();
		if (nStartIndex >= m_nSize)
		{
			// adding after the end of the array
			SetSize(nStartIndex + nCount, -1);   // grow so nIndex is valid
		}
		else
		{
			// inserting in the middle of the array
			int nOldSize = m_nSize;
			SetSize(m_nSize + nCount, -1);  // grow it to new size
			// destroy intial data before copying over it
			DestructElements1<TYPE>(&m_pData[nOldSize], nCount);
			// shift old data up to fill gap
			memmove(&m_pData[nStartIndex+nCount], &m_pData[nStartIndex],
				(nOldSize-nStartIndex) * sizeof(TYPE));
			
			// re-init slots we copied from
			ConstructElements1<TYPE>(&m_pData[nStartIndex], nCount);
		}
		
		// insert new value in the gap
		assert(nStartIndex + nCount <= m_nSize);
		int i=0;
		while (nCount--)
			m_pData[nStartIndex++] = newArray[i++];
	}
}

template<class TYPE, class ARG_TYPE>
void Array<TYPE, ARG_TYPE>::Swap(_MyArray& aray)
{
	int nTemp;
	TYPE* pType;
	SWAP(m_nGrowBy, aray.m_nGrowBy, nTemp);
	SWAP(m_nMaxSize, aray.m_nMaxSize, nTemp);
	SWAP(m_nSize, aray.m_nSize, nTemp);
	SWAP(m_pData, aray.m_pData, pType);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

template<class KEY, class VALUE, class ARG_KEY=const KEY&, class ARG_VALUE=const VALUE&>
class CVLIB_DECLSPEC Map : public Object
{
protected:
	// Association
	struct mapAssoc
	{
		mapAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration
		KEY key;
		VALUE value;
	};
public:
// Construction
	Map(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(ARG_KEY key, VALUE& rValue) const;

// Operations
	// Lookup and add if not there
	VALUE& operator[](ARG_KEY key);

	// add a new (key, value) pair
	void SetAt(ARG_KEY key, ARG_VALUE newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(ARG_KEY key);
	void RemoveAll();

	// iterating all (key, value) pairs
	CVLIB_POSITION GetStartPosition() const;
	void GetNextAssoc(CVLIB_POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Implementation
protected:
	mapAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	mapAssoc* m_pFreeList;
	struct Plex* m_pBlocks;
	int m_nBlockSize;

	mapAssoc* NewAssoc();
	void FreeAssoc(mapAssoc*);
	mapAssoc* GetAssocAt(ARG_KEY, UINT&) const;

public:
	~Map();
};

/////////////////////////////////////////////////////////////////////////////
// Map<KEY,  VALUE, ARG_KEY,ARG_VALUE> inline functions

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
inline int Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::GetCount() const
	{ return m_nCount; }
template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
inline BOOL Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::IsEmpty() const
	{ return m_nCount == 0; }
template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
inline void Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ (*this)[key] = newValue; }
template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
inline CVLIB_POSITION Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : CVLIB_BEFORE_START_POSITION; }
template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
inline UINT Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::GetHashTableSize() const
	{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////
// Map<KEY,  VALUE, ARG_KEY,ARG_VALUE> out-of-line functions

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::Map(int nBlockSize)
{
	assert(nBlockSize > 0);

	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
void Map<KEY, VALUE, ARG_KEY,ARG_VALUE>::InitHashTable(
	UINT nHashSize, BOOL bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{
	assert(this);
	assert(m_nCount == 0);
	assert(nHashSize > 0);

	if (m_pHashTable != NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if (bAllocNow)
	{
		m_pHashTable = new mapAssoc* [nHashSize];
		memset(m_pHashTable, 0, sizeof(mapAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
void Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::RemoveAll()
{
	assert(this);

	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			mapAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				DestructElements1<VALUE>(&pAssoc->value, 1);
				DestructElements1<KEY>(&pAssoc->key, 1);
			}
		}
	}

	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::~Map()
{
	RemoveAll();
	assert(m_nCount == 0);
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
typename Map<KEY,VALUE,ARG_KEY,ARG_VALUE>::mapAssoc* 
Map<KEY,VALUE,ARG_KEY,ARG_VALUE>::NewAssoc()
{
	if (m_pFreeList == NULL)
	{
		// add another block
		Plex* newBlock = Plex::Create(m_pBlocks, m_nBlockSize, sizeof(Map::mapAssoc));
		// chain them into free list
		typename Map::mapAssoc* pAssoc = (typename Map::mapAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	assert(m_pFreeList != NULL);  // we must have something

	typename Map::mapAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	assert(m_nCount > 0);  // make sure we don't overflow
	ConstructElements1<KEY>(&pAssoc->key, 1);
	ConstructElements1<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
void Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::FreeAssoc(typename Map::mapAssoc* pAssoc)
{
	DestructElements1<VALUE>(&pAssoc->value, 1);
	DestructElements1<KEY>(&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	assert(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
typename Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::mapAssoc*
Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::GetAssocAt(ARG_KEY key, UINT& nHash) const
// find association (or return NULL)
{
	nHash = cvlHashKey<ARG_KEY>(key) % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	mapAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (CompareElements1(&pAssoc->key, &key))
			return pAssoc;
	}
	return NULL;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
BOOL Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const
{
	assert(this);

	UINT nHash;
	mapAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
VALUE& Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::operator[](ARG_KEY key)
{
	assert(this);

	UINT nHash;
	mapAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);

		// it doesn't exist, add a new Association
		pAssoc = NewAssoc();
		pAssoc->nHashValue = nHash;
		pAssoc->key = key;
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
BOOL Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::RemoveKey(ARG_KEY key)
// remove key - return TRUE if removed
{
	assert(this);

	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	mapAssoc** ppAssocPrev;
	ppAssocPrev = &m_pHashTable[cvlHashKey<ARG_KEY>(key) % m_nHashTableSize];

	mapAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (CompareElements1(&pAssoc->key, &key))
		{
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return TRUE;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
void Map<KEY,  VALUE, ARG_KEY,ARG_VALUE>::GetNextAssoc(CVLIB_POSITION& rNextPosition,
	KEY& rKey, VALUE& rValue) const
{
	assert(this);
	assert(m_pHashTable != NULL);  // never call on empty map

	mapAssoc* pAssocRet = (mapAssoc*)rNextPosition;
	assert(pAssocRet != NULL);

	if (pAssocRet == (mapAssoc*) CVLIB_BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		assert(pAssocRet != NULL);  // must find something
	}

	// find next association
	mapAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (UINT nBucket = pAssocRet->nHashValue + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (CVLIB_POSITION) pAssocNext;

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

#define Vector Array

template<class TYPE, class ARG_TYPE=const TYPE&>
class CVLIB_DECLSPEC Matrix : public Vector<Vector<TYPE, ARG_TYPE>, const Vector<TYPE, ARG_TYPE>&>
{
public:
	typedef Matrix<TYPE, ARG_TYPE> _MyMatrix;
	Matrix(){}
	Matrix(int nRows, int nCols, const TYPE& _V=TYPE())
	{
		Create (nRows, nCols, _V);
	}
 	int Create (int nRows, int nCols, const TYPE& _V=TYPE())
	{
		Vector<Vector<TYPE, ARG_TYPE>, const Vector<TYPE, ARG_TYPE>&>::RemoveAll();
		Vector<Vector<TYPE, ARG_TYPE>, const Vector<TYPE, ARG_TYPE>&>::SetSize(nRows);
		for (int i=0; i<Vector<Vector<TYPE, ARG_TYPE>, const Vector<TYPE, ARG_TYPE>&>::Length(); i++)
		{
			Vector<Vector<TYPE, ARG_TYPE>, const Vector<TYPE, ARG_TYPE>&>::GetAt(i).InsertAt(0, _V, nCols);
		}
		return 1;
	}
};

#pragma warning(disable: 4786)

#undef new
#ifdef _REDEF_NEW
//#define new new(__FILE__, __LINE__)
#undef _REDEF_NEW
#endif

}

#endif //_TEMPLATE_H_