/*!
 * \file	PtrArray.cpp
 * \ingroup base
 * \brief	¼ÑºÏ´ÉÌ© ¹ÉµÛË¾ ±ã¶®Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷Ì© »¨Âï¹¢¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include "PtrArray.h"

namespace CVLib 
{

PtrArray::PtrArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

PtrArray::~PtrArray()
{
	assert(this);

	delete[] (BYTE*)m_pData;
}

int PtrArray::GetSize() const
{
	return m_nSize; 
}

int PtrArray::GetUpperBound() const
{
	return m_nSize-1;
}

void PtrArray::SetSize(int nNewSize, int nGrowBy)
{
	assert(this);
	assert(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData)
		delete[] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		assert(nNewSize <= SIZE_T_MAX/sizeof(void*));    // no overflow
#endif
		m_pData = (void**) new BYTE[nNewSize * sizeof(void*)];

		memset(m_pData, 0, nNewSize * sizeof(void*));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));
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
			nGrowBy = MIN(1024, MAX(4, m_nSize / 8));
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		assert(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		assert(nNewMax <= SIZE_T_MAX/sizeof(void*)); // no overflow
#endif
		void** pNewData = (void**) new BYTE[nNewMax * sizeof(void*)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(void*));

		// construct remaining elements
		assert(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));


		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void PtrArray::FreeExtra()
{
	assert(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		assert(m_nSize <= SIZE_T_MAX/sizeof(void*)); // no overflow
#endif
		void** pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (void**) new BYTE[m_nSize * sizeof(void*)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(void*));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

void PtrArray::RemoveAll()
{
	SetSize(0);
}

void* PtrArray::GetAt(int nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

void PtrArray::SetAt(int nIndex, void* newElement)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = newElement;
}

void*& PtrArray::ElementAt(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

const void** PtrArray::GetData() const
{
	return (const void**)m_pData;
}

void** PtrArray::GetData()
{
	return (void**)m_pData;
}

void PtrArray::SetAtGrow(int nIndex, void* newElement)
{
	assert(this);
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);
	m_pData[nIndex] = newElement;
}

int PtrArray::Add(void* newElement)
{
	int nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex;
}

int PtrArray::Append(const PtrArray& src)
{
	assert(this);
	assert(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void*));

	return nOldSize;
}

void PtrArray::Copy(const PtrArray& src)
{
	assert(this);
	assert(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void*));

}

void* PtrArray::operator[](int nIndex) const
{
	return GetAt(nIndex);
}

void*& PtrArray::operator[](int nIndex)
{
	return ElementAt(nIndex);
};

void PtrArray::InsertAt(int nIndex, void* newElement, int nCount)
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
			(nOldSize-nIndex) * sizeof(void*));

		// re-init slots we copied from
		memset(&m_pData[nIndex], 0, nCount * sizeof(void*));
	}

	// insert new value in the gap
	assert(nIndex + nCount <= m_nSize);



	// copy elements into the empty space
	while (nCount--)
		m_pData[nIndex++] = newElement;

}

void PtrArray::InsertAt(int nStartIndex, PtrArray* pNewArray)
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

void PtrArray::RemoveAt(int nIndex, int nCount)
{
	assert(this);
	assert(nIndex >= 0);
	assert(nCount >= 0);
	assert(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(void*));
	m_nSize -= nCount;
}

}
