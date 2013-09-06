/*!
 * \file	PtrList.cpp
 * \ingroup base
 * \brief	µÚ°Î·»µâ¿Ðµ¹º÷¶¦ Ì¡ÂÙ »¨Âï¹¢
 * \author	¹ÊËÎ²§, ±®½èË§
 */

#include "ptrList.h"

namespace CVLib
{

/**
 * @brief µÚ°Î·»µâ°é¼¿¾Ë
 */
class Node : public Object
{
public:
	void*	pData;				/**< »ôµê */
	Node*	pPrev;				/**< ¼­ ¶ë´Ñ¼ÑºÏ */
	Node*	pNext;				/**< ³ÞËÁ ¶ë´Ñ¼ÑºÏ */
};

/**
 * @brief µÚ°Î·»µâË¾ ¼è¼³Â×²÷´ç ¶®Ë¦´ó²÷ ¿Ðµ¹º÷
 */
class PtrListData : public Object
{
public:
	int		m_nCount;			/**< ·»µâÌ© ¶ë´Ñºã**/
	Node*	m_pHead;			/**< ·»µâÌ© ½ë ¶ë´ÑÌ© ¼ÑºÏ*/
	Node*	m_pTail;			/**< ·»µâÌ© ¶ë¼è¶ì¶ë´ÑÌ© ¼ÑºÏ*/
	Node*	m_pCur;             /**< ·»µâÌ© Âï¼õ¶ì¶ë´ÑÌ© ¼ÑºÏ*/
public:
	// Constructor and Destructor
	PtrListData();
	virtual ~PtrListData();
};

/**
 * @brief °é¾¢»ô
 */
PtrListData::PtrListData()
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_pCur = m_pHead;
	m_nCount = 0;
}

/**
 * @brief ÁÄ±Ð»ô
 */
PtrListData::~PtrListData()
{
}

PtrList::PtrList()
{
	m_pData = NULL;
}

PtrList::~PtrList()
{
	if (FlagCreate())
		Release ();
	assert (m_pData == NULL);
}

bool PtrList::Create(void)
{
	assert (m_pData == NULL);
	m_pData = new PtrListData;

	if (m_pData)
		return true;
	else
		return false;
}

void PtrList::Release(void)
{
	Node* pCur = ((PtrListData*)m_pData)->m_pHead;
		
	while ( pCur != NULL )
	{
		Node* pTemp = pCur->pNext;
		delete pCur;
		pCur = pTemp;
	}
	delete ((PtrListData*)m_pData);
	m_pData = NULL;
}

unsigned long PtrList::Count(void) const
{
	return ((PtrListData*)m_pData)->m_nCount;
}

void* PtrList::Data(void) const
{
	return ((PtrListData*)m_pData)->m_pCur->pData;
}

bool PtrList::Add(const void* pData, int nMode /*= ADD_AFTER*/)
{
	PtrListData* pListElems = (PtrListData*)m_pData;
	Node* pNewItem = new Node;
	pNewItem->pData = (void*)pData;

	if ( !pListElems->m_pHead ) 
	{
		pNewItem->pNext = NULL;
		pNewItem->pPrev = NULL;
		pListElems->m_pHead = pListElems->m_pTail = pNewItem;
	}
	else
	{
		switch ( nMode )
		{
		case ADD_AFTER:
			pListElems->m_pTail->pNext = pNewItem;
			pNewItem->pPrev = pListElems->m_pTail;
			pNewItem->pNext = NULL;
			pListElems->m_pTail = pNewItem;
			break;
		case ADD_BEFORE:
			pListElems->m_pHead->pPrev = pNewItem;
			pNewItem->pNext = pListElems->m_pHead;
			pNewItem->pPrev = NULL;
			pListElems->m_pHead = pNewItem;
			break;
		}
	}

	pListElems->m_pCur = pNewItem;
	pListElems->m_nCount ++;
	return true;
}

bool PtrList::Find(const void* pData) const
{
	PtrListData* pListElems = (PtrListData*)m_pData;
	Node* pCur = pListElems->m_pHead;
	while (pCur)
	{
		if (pCur->pData == pData)
			break;
		pCur = pCur->pNext;
	}
	if (pCur)
	{
		pListElems->m_pCur = pCur;
		return true;
	}
	else
	{
		return false;
	}
}

bool PtrList::SetPosition(int nWhence, int nIndex)
{
	bool	fRet = false;
	PtrListData* pListElems = (PtrListData*)m_pData;
	if (nIndex >= 0)
	{
		switch (nWhence)
		{
		case POS_BEGIN:
			pListElems->m_pCur = pListElems->m_pHead;
			while (pListElems->m_pCur && nIndex != 0)
			{
				pListElems->m_pCur = pListElems->m_pCur->pNext;
				nIndex --;
			}
			break;
		case POS_CURRENT:
			while (pListElems->m_pCur && nIndex != 0)
			{
				pListElems->m_pCur = pListElems->m_pCur->pNext;
				nIndex --;
			}
			break;
		case POS_END:
			pListElems->m_pCur = pListElems->m_pTail;
			while (pListElems->m_pCur && nIndex != 0)
			{
				pListElems->m_pCur = pListElems->m_pCur->pNext;
				nIndex --;
			}
			break;
		}
	}
	else
	{
		switch (nWhence)
		{
		case POS_BEGIN:
			pListElems->m_pCur = pListElems->m_pHead;
			while (pListElems->m_pCur && nIndex != 0)
			{
				pListElems->m_pCur = pListElems->m_pCur->pPrev;
				nIndex ++;
			}
			break;
		case POS_CURRENT:
			while (pListElems->m_pCur && nIndex != 0)
			{
				pListElems->m_pCur = pListElems->m_pCur->pPrev;
				nIndex ++;
			}
			break;
		case POS_END:
			pListElems->m_pCur = pListElems->m_pTail;
			while (pListElems->m_pCur && nIndex != 0)
			{
				pListElems->m_pCur = pListElems->m_pCur->pPrev;
				nIndex ++;
			}
			break;
		}
	}
	if (pListElems->m_pCur && nIndex == 0)
		fRet = true;
	else
	{
		pListElems->m_pCur = pListElems->m_pTail;
		fRet = false;
	}

	return fRet;
}

void PtrList::Remove(void)
{
	PtrListData* pListElems = (PtrListData*)m_pData;
	Node* pTemp = pListElems->m_pCur;

	if (pTemp == NULL)
		return;
	if (pTemp == pListElems->m_pHead)
	{
		pListElems->m_pHead = pListElems->m_pHead->pNext;
		if ( pListElems->m_pHead )
			pListElems->m_pHead->pPrev = NULL;
		else
			pListElems->m_pTail = NULL;
	}
	else if (pTemp == pListElems->m_pTail)
	{
		pListElems->m_pTail = pListElems->m_pTail->pPrev;
		if ( pListElems->m_pTail )
			pListElems->m_pTail->pNext = NULL;
		else
			pListElems->m_pHead = NULL;
	}
	else
	{
		pTemp->pPrev->pNext = pTemp->pNext;
		pTemp->pNext->pPrev = pTemp->pPrev;
	}
	delete pTemp;
	pListElems->m_pCur = NULL;
	pListElems->m_nCount --;
	return;
}

}
