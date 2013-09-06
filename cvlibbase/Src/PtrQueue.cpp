/*!
 * \file	PtrQueue.cpp
 * \ingroup base
 * \brief	¼ÑºÏ´İ±¨µÛ¿Ğµ¹º÷Ì© »¨?E¢¶?¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "ptrQueue.h"

namespace CVLib 
{

/**
 * class PtrQueueData : PtrQueue¿Ğµ¹º÷Ì© »ôµE?
 */
class PtrQueueData : public Object
{
public:
	int		m_nFirst;	/**< »¤»EÌ¡¾· */
	int		m_nLast;	/**< ¶Eè¶E¡¾?*/
	int		m_nCur;		/**< ?EõÌ¡¾?*/
	int		m_nCount;	/**< ´İ±¨µÛ?E´ÉÊÌ°¡²EÌººÏÌ© ±¶ºE*/
	void**	m_ppData;	/**< ¼ÑºÏ¶¦ ¸ó±ãÂ×²E¹ÉµÛ */

public:
	// Constructor and Destructor
	//! °é¾¢»E
	PtrQueueData()
	{
		m_nCount = 0; m_ppData = NULL; m_nFirst = 0; m_nLast = -1; m_nCur = 0;
	}

	//! ÃÍ½£»E
	virtual ~PtrQueueData()	{};

	/**
	 * @brief ´İ±¨µÛË¾ ½ó±¨ÃùÂÙ³Ş.
	 *
	 * @return void
	 */
	void Init ()
	{
		m_nFirst = m_nCur = 0;
		m_nLast = m_nCount - 1;
	}

	/**
	 * @brief ´İ±¨µÛÌ© Ê¼?E»ôµE?²Ä²÷³Ş.
	 *
	 * @param pData [in] : »ôµE?¼ÑºÏ
	 * @return void
	 */
	void PushHead(void* pData)
	{
		m_nFirst = (m_nFirst - 1 + m_nCount) % m_nCount;
		m_ppData[m_nFirst] = pData;
		m_nCur = m_nFirst;
	}

	/**
	 * @brief ´İ±¨µÛÌ© Äù?E»ôµE?²Ä²÷³Ş.
	 *
	 * @param pData [in] : »ôµE?¼ÑºÏ
	 * @return void
	 */
	void PushTail(void* pData)
	{
		m_nLast = (m_nLast + 1) % m_nCount;
		m_ppData[m_nLast] = pData;
	}

	/**
	 * @brief ´İ±¨µÛÌ© Ê¼Ëæº· »ôµE?ÄÅ³¯³Ş.
	 *
	 * @return »ôµE?¼ÑºÏ
	 */
	void* PopHead ()
	{
		void* pRet = m_ppData[m_nFirst];
		m_ppData[m_nFirst] = NULL;
		m_nFirst = (m_nFirst + 1) % m_nCount;
		m_nCur = m_nFirst;
		return pRet;
	}

	/**
	 * @brief ´İ±¨µÛÌ© ÄùËæº· »ôµE?Ç°²÷³Ş.
	 *
	 * @return »ôµE?¼ÑºÏ
	 */
	void* PopTail ()
	{
		void* pRet = m_ppData[m_nLast];
		m_ppData[m_nLast] = NULL;
		m_nLast = (m_nLast - 1 + m_nCount) % m_nCount;
		return pRet;
	}

	/**
	 * @brief ´İ±¨µÛÌ© ¼è»¤±¨Ì© Ì¡¾·¶¦ º¼¼³ÂÙ³Ş.
	 *
	 * @param nWhence [in] : ±¨¼ÓÌ¡¾·
	 * @param nIndex [in] : ±¨¼ÓÌ¡¾·µá¹¢À¾Ì© Êú°ıºãÁİÌ¡, ÁİÌ¡°ªË¼ 0ËËº¬ËËÊŞÊ¿ ÂÙ³Ş.
	 * @return void
	 */
	void SetPosition(int nWhence, int nIndex)
	{
		switch (nWhence)
		{
		case PtrQueue::POS_HEAD:
			m_nCur = (m_nFirst + nIndex + m_nCount) % m_nCount;
			break;
		case PtrQueue::POS_TAIL:
			m_nCur = (m_nLast - nIndex + m_nCount) % m_nCount;
			break;
		}
	}

	/**
	 * @brief »ôµE?¼è¼¬»ô¶¦ ½Ü²÷³Ş.
	 <pre> »ôµEè¼¬»ô°?·»µâÊ¯?E¼Á¼õÂ×·² ?EEÌ¡¾·
	 *		¼è»¤±¨¶¦ »ôµEè¼¬»ô¶?ÁäÂÜÂ×²E·ô´Î?E»¯µEº¼¼³ÂÙ³Ş. </pre>
	 *
	 * @param	pData [in]	½ÜËºµØ²E»ôµE?¼ÑºÏ
	 * @return	(bool) \n
	 *			»ôµE?½ÜËº·² true, Ê­³£·² false¶¦ ´ó´®¶°³Ş.
	 */
	bool Find(const void* pData)
	{
		int i;
		for (i = 0; i < m_nCount; i ++)
		{
			if (m_ppData[i] == pData)
				break;
		}
		if (i == m_nCount)
			return false;
		else 
			return true;
	}
};

PtrQueue::PtrQueue ()
{
	m_pData = NULL;
}

PtrQueue::PtrQueue (int	nCount)
{
	Create (nCount);
}

PtrQueue::~PtrQueue ()
{
	if (FlagCreate())
		Release();
	assert(m_pData == NULL);
}

bool PtrQueue::FlagCreate ()
{
	return m_pData ? true : false;
}

bool PtrQueue::Create(int nCount)
{
	assert (m_pData == NULL);
	assert (nCount > 0);
	m_pData = new PtrQueueData;
//	Init();
	((PtrQueueData*)m_pData)->m_nCount = nCount;
	((PtrQueueData*)m_pData)->m_ppData = new void*[nCount];
	memset (((PtrQueueData*)m_pData)->m_ppData, 0, sizeof(void*) * nCount);
	((PtrQueueData*)m_pData)->m_nFirst = 0;
	((PtrQueueData*)m_pData)->m_nLast = nCount - 1;
	return true;
}

void PtrQueue::Release()
{
	delete []((PtrQueueData*)m_pData)->m_ppData;
	delete (PtrQueueData*)m_pData;
	m_pData = NULL;
}
// 
// /**
//  @brief ½ó±¨Ãù¶¦ ¼EÔÂÙ³?
//  */
// void PtrQueue::Init()
// {
// 	assert (FlagCreate());
// 	((PtrQueueData*)m_pData)->Init();
// }

int	PtrQueue::Count() const
{
	return	((PtrQueueData*)m_pData)->m_nCount;
}

void* PtrQueue::Data() const
{
	PtrQueueData* pData = ((PtrQueueData*)m_pData);
	return pData->m_ppData[pData->m_nCur];
}

void PtrQueue::PushHead(void* pData)
{
	((PtrQueueData*)m_pData)->PushHead(pData);
}

void PtrQueue::PushTail(void* pData)
{
	((PtrQueueData*)m_pData)->PushTail(pData);
}

void* PtrQueue::PopHead ()
{
	return 	((PtrQueueData*)m_pData)->PopHead();
}

void* PtrQueue::PopTail ()
{
	return 	((PtrQueueData*)m_pData)->PopTail();
}

void PtrQueue::SetPosition(int	nWhence, int nIndex)
{
	((PtrQueueData*)m_pData)->SetPosition(nWhence, nIndex);
}

}
