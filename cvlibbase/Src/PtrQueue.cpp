/*!
 * \file	PtrQueue.cpp
 * \ingroup base
 * \brief	�Ѻϴݱ��ۿе���̩ ��?E��?��̩��.
 * \author	����˧
 */

#include "ptrQueue.h"

namespace CVLib 
{

/**
 * class PtrQueueData : PtrQueue�е���̩ ����E?
 */
class PtrQueueData : public Object
{
public:
	int		m_nFirst;	/**< ����E̡�� */
	int		m_nLast;	/**< ��E趁E��?*/
	int		m_nCur;		/**< ?E�̡�?*/
	int		m_nCount;	/**< �ݱ���?E���̰���E̺��̩ ����E*/
	void**	m_ppData;	/**< �Ѻ϶� ����ײ�E�ɵ� */

public:
	// Constructor and Destructor
	//! �龢��E
	PtrQueueData()
	{
		m_nCount = 0; m_ppData = NULL; m_nFirst = 0; m_nLast = -1; m_nCur = 0;
	}

	//! �ͽ���E
	virtual ~PtrQueueData()	{};

	/**
	 * @brief �ݱ���˾ �����ٳ�.
	 *
	 * @return void
	 */
	void Init ()
	{
		m_nFirst = m_nCur = 0;
		m_nLast = m_nCount - 1;
	}

	/**
	 * @brief �ݱ���̩ ʼ?E����E?�Ĳ���.
	 *
	 * @param pData [in] : ����E?�Ѻ�
	 * @return void
	 */
	void PushHead(void* pData)
	{
		m_nFirst = (m_nFirst - 1 + m_nCount) % m_nCount;
		m_ppData[m_nFirst] = pData;
		m_nCur = m_nFirst;
	}

	/**
	 * @brief �ݱ���̩ ��?E����E?�Ĳ���.
	 *
	 * @param pData [in] : ����E?�Ѻ�
	 * @return void
	 */
	void PushTail(void* pData)
	{
		m_nLast = (m_nLast + 1) % m_nCount;
		m_ppData[m_nLast] = pData;
	}

	/**
	 * @brief �ݱ���̩ ʼ�溷 ����E?�ų���.
	 *
	 * @return ����E?�Ѻ�
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
	 * @brief �ݱ���̩ ���溷 ����E?ǰ����.
	 *
	 * @return ����E?�Ѻ�
	 */
	void* PopTail ()
	{
		void* pRet = m_ppData[m_nLast];
		m_ppData[m_nLast] = NULL;
		m_nLast = (m_nLast - 1 + m_nCount) % m_nCount;
		return pRet;
	}

	/**
	 * @brief �ݱ���̩ �軤��̩ ̡���� �����ٳ�.
	 *
	 * @param nWhence [in] : ����̡��
	 * @param nIndex [in] : ����̡���Ṣ��̩ ��������̡, ��̡��˼ 0�˺�����ʿ �ٳ�.
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
	 * @brief ����E?�輬���� �ܲ���.
	 <pre> ����E輬���?����ʯ?E�����׷� ?E�E̡��
	 *		�軤���� ����E輬���?�����ײ�E����?E����E�����ٳ�. </pre>
	 *
	 * @param	pData [in]	��˺�ز�E����E?�Ѻ�
	 * @return	(bool) \n
	 *			����E?��˺�� true, ʭ���� false�� �󴮶���.
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
//  @brief ������ ��E��ٳ?
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
