/*!
 * \file	Queue.cpp
 * \ingroup base
 * \brief	´Ý±¨µÛ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "Queue.h"

namespace CVLib 
{
	
Queue::Queue () 
{
	m_nCount = 0; m_nSize = 0; m_pData = NULL; m_nFirst = 0; m_nLast = -1; m_nCur = 0;
}

Queue::Queue (int nCount, int nSize) 
{ 
	Create (nCount, nSize); 
}

Queue::~Queue ()
{
	if (FlagCreate())
		Release();
}

int Queue::FlagCreate ()
{
	return m_pData? 1 : 0;
}

int	Queue::Create(int nCount, int nSize)
{
	assert (nCount > 0);
	m_pData = (char*)malloc(nSize * nCount);
	m_nCount = nCount;
	m_nSize = nSize;
	Init();
	return 1;
}

void Queue::Release()	
{
	free (m_pData);	m_pData = NULL; m_nCount = 0;
}

void Queue::Init()
{
	assert (FlagCreate());
	m_nFirst = m_nCur = 0;
	m_nLast = m_nCount - 1;
}

int	Queue::Count() const
{
	return		m_nCount;
}

void* Queue::Data()	const
{
	return (void*)((BYTE*)m_pData + m_nCur * m_nSize);	
}

void Queue::PushHead(void* pData)
{
	m_nFirst = (m_nFirst - 1 + m_nCount) % m_nCount;
	memcpy (m_pData + m_nFirst * m_nSize, (char*)pData, m_nSize);
	m_nCur = m_nFirst;
}

void Queue::PushTail(void* pData)
{
	m_nLast = (m_nLast + 1) % m_nCount;
	memcpy (m_pData + m_nLast * m_nSize, (char*)pData, m_nSize);
}

void* Queue::PopHead ()
{
	char* pRet = m_pData + m_nFirst * m_nSize;
	m_nFirst = (m_nFirst + 1) % m_nCount;
	m_nCur = m_nFirst;
	return pRet;
}

void* Queue::PopTail ()
{
	char* pRet = m_pData + m_nLast * m_nSize;
	m_nLast = (m_nLast - 1 + m_nCount) % m_nCount;
	return pRet;
}

void Queue::SetPosition(int	nWhence, int nIndex)
{
	switch (nWhence)
	{
	case POS_HEAD:
		m_nCur = (m_nFirst + nIndex + m_nCount) % m_nCount;
		break;
	case POS_TAIL:
		m_nCur = (m_nLast - nIndex + m_nCount) % m_nCount;
		break;
	}
}

}
