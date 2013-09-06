/*!
 * \file	StringArray.cpp
 * \ingroup base
 * \brief ·Í»ôµÛ¹ÉµÛË¾ ¼¿»õÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "StringArray.h"
#include "cvlibbaseDef.h"
#include <assert.h>

namespace CVLib
{

	int StringArray::GetSize() const
	{
		return m_nSize;
	}
	
	 int StringArray::GetUpperBound() const
	{
		return m_nSize-1;
	}
	
	 void StringArray::RemoveAll()
	{
		SetSize(0);
	}
	
	 SString StringArray::GetAt(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}
	
	 void StringArray::SetAt(int nIndex, char* newElement)
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement;
	}
	
	 void StringArray::SetAt(int nIndex, SString& newElement)
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement;
	}
	
	 SString& StringArray::ElementAt(int nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}
	
	 const SString* StringArray::GetData() const
	{	
		 return (const SString*)m_pData;
	}
	
	 SString* StringArray::GetData()
	{
		return (SString*)m_pData;
	}
	
	 int StringArray::Add(const char* newElement)
	{
		int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex;
	}
	
	 int StringArray::Add(SString& newElement)
	{
		int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex;
	}
	
	 SString StringArray::operator[](int nIndex) const
	{
		return GetAt(nIndex);
	}
	
	 SString& StringArray::operator[](int nIndex)
	{
		return ElementAt(nIndex);
	}

static SString EmptyString;

static inline void ConstructElement(SString* pNewData)
{
	memcpy(pNewData, &EmptyString, sizeof(SString));
}

static inline void DestructElement(SString* pOldData)
{
	pOldData->~SString();
}

static inline void CopyElement(SString* pSrc, SString* pDest)
{
	*pSrc = *pDest;
}

static void ConstructElements(SString* pNewData, int nCount)
{
	assert(nCount >= 0);

	while (nCount--)
	{
		ConstructElement(pNewData);
		pNewData++;
	}
}

static void DestructElements(SString* pOldData, int nCount)
{
	assert(nCount >= 0);

	while (nCount--)
	{
		DestructElement(pOldData);
		pOldData++;
	}
}

static void CopyElements(SString* pDest, SString* pSrc, int nCount)
{
	assert(nCount >= 0);

	while (nCount--)
	{
		*pDest = *pSrc;
		++pDest;
		++pSrc;
	}
}

/////////////////////////////////////////////////////////////////////////////

StringArray::StringArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}
StringArray::StringArray(const StringArray& t)
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	for (int i=0; i<t.GetSize(); i++)
		Add (t.GetAt(i));
}

StringArray::~StringArray()
{
	assert(this);

	DestructElements(m_pData, m_nSize);
	delete[] (BYTE*)m_pData;
}
StringArray& StringArray::operator=(const StringArray& t)
{
	RemoveAll();
	for (int i=0; i<t.GetSize(); i++)
		Add (t.GetAt(i));
	return *this;
}

void StringArray::SetSize(int nNewSize, int nGrowBy)
{
	assert(this);
	assert(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing

		DestructElements(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		assert(nNewSize <= SIZE_T_MAX/sizeof(SString));    // no overflow
#endif
		m_pData = (SString*) new BYTE[nNewSize * sizeof(SString)];

		ConstructElements(m_pData, nNewSize);

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			ConstructElements(&m_pData[m_nSize], nNewSize-m_nSize);

		}

		else if (m_nSize > nNewSize)  // destroy the old elements
			DestructElements(&m_pData[nNewSize], m_nSize-nNewSize);

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
			nGrowBy = MIN(1024, MAX(4, m_nSize / 8));
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		assert(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		assert(nNewMax <= SIZE_T_MAX/sizeof(SString)); // no overflow
#endif
		SString* pNewData = (SString*) new BYTE[nNewMax * sizeof(SString)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(SString));

		// construct remaining elements
		assert(nNewSize > m_nSize);

		ConstructElements(&pNewData[m_nSize], nNewSize-m_nSize);


		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

int StringArray::Append(const StringArray& src)
{
	assert(this);
	assert(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	CopyElements(m_pData + nOldSize, src.m_pData, src.m_nSize);

	return nOldSize;
}

void StringArray::Copy(const StringArray& src)
{
	assert(this);
	assert(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	CopyElements(m_pData, src.m_pData, src.m_nSize);

}

void StringArray::FreeExtra()
{
	assert(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		assert(m_nSize <= SIZE_T_MAX/sizeof(SString)); // no overflow
#endif
		SString* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (SString*) new BYTE[m_nSize * sizeof(SString)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(SString));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void StringArray::SetAtGrow(int nIndex, const char* newElement)
{
	assert(this);
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}


void StringArray::SetAtGrow(int nIndex, SString& newElement)
{
	assert(this);
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}



void StringArray::InsertEmpty(int nIndex, int nCount)
{
	assert(this);
	assert(nIndex >= 0);    // will expand to meet need
	assert(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(SString));

		// re-init slots we copied from

		ConstructElements(&m_pData[nIndex], nCount);

	}

	// insert new value in the gap
	assert(nIndex + nCount <= m_nSize);
}


void StringArray::InsertAt(int nIndex, char* newElement, int nCount)
{

	// make room for new elements
	InsertEmpty(nIndex, nCount);

	// copy elements into the empty space
	SString temp = SString(newElement);
	while (nCount--)
		m_pData[nIndex++] = temp;

}


void StringArray::InsertAt(int nIndex, SString& newElement, int nCount)
{
	// make room for new elements
	InsertEmpty(nIndex, nCount);

	// copy elements into the empty space
	while (nCount--)
		m_pData[nIndex++] = newElement;
}


void StringArray::RemoveAt(int nIndex, int nCount)
{
	assert(this);
	assert(nIndex >= 0);
	assert(nCount >= 0);
	assert(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	DestructElements(&m_pData[nIndex], nCount);

	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(SString));
	m_nSize -= nCount;
}

void StringArray::InsertAt(int nStartIndex, StringArray* pNewArray)
{
	assert(this);
	assert(pNewArray != NULL);
	assert(pNewArray);
	assert(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, (char*)pNewArray->GetAt(0).GetBuffer(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i).GetBuffer(0));
	}
}

}