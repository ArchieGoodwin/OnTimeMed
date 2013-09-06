/*!
 * \file	Queue.h
 * \ingroup base
 * \brief	�ݱ��ۿе����� ��̩��.
 * \author	����˧
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Object.h"
#include <assert.h>

namespace CVLib 
{
	
/**
 * @brief �ݱ��ۿе���
 */
class CVLIB_DECLSPEC Queue : public Object
{
public:

	//! �ݱ���̩ ̡���� �輳�ײ� �۰��� 
	enum
	{
		POS_HEAD   = 0,    /**< �ݱ���̩ ���� */
		POS_TAIL   = 1     /**< �ݱ���̩ ��   */
	};

public:
	// Constructor and Destructor
	//! �龢��
	Queue ();

	/**
	 * @brief   �龢��
	 *
	 * @param   nCount [in] : ̺��̩ ����
	 * @param   nSize  [in] : ̺��̩ �ͱ�
	 *
	 */
	Queue (int nCount, int nSize);
	
	//! �;˻�
	virtual ~Queue ();

	// Check if the instance was created
	/**
	 * @brief   �ݱ����� �ۼ���������� �����ٳ�.
	 *
	 * @return  �ۼ�����˺�� 1, ʭ���� 0
	 *
	 * @see	 Create(), Release()
	 *
	 */
	int FlagCreate ();

	// Initialize the queue.
	/**
	 * @brief   �ݱ���˾ �ۼ��ٳ�.
	 *
	 * @param   nCount [in] : �ݱ���̺��̩ ����
	 * @param   nSize  [in] : �ݱ���̺��̩ �ͱ�
	 * @return  �ۼ�����˺�� 1, ʭ���� 0
	 *
	 * @see	 FlagCreate(), Release()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	Queue queue;
	 *	assert (queue.FlagCreate() == false);
	 *	queue.Create (5, 2);
	 *	assert (queue.FlagCreate() == true);
	 *	assert (queue.Create(3, 2) == false);
	 *	queue.Release();
	 *
	 * @endcode
	 *
	 */
	int Create(int nCount, int nSize);

	// Destroy the queue.
	/**
	 * @brief   ����� ���ͱ��� �ͽ��ٳ�.
	 *
	 * @return	void
	 *
	 * @see	 Create(), FlagCreate()
	 *
	 */
	virtual void Release();

	// Init contents of array.
	/**
	 * @brief   ������ �����ٳ�.
	 *
	 * @return	void
	 *
	 * @see	Queue::Create()
	 *
	 */
	void Init();

	// Get the size of data.
	/**
	 * @brief   ̺��̩ ���㶦 �в���.
	 *
	 * @return	̺�ϱ���
	 *
	 * @par	Example:
	 * @code
	 *
	 *	Queue queue;
	 *	queue.Create (5, 2);
	 *	assert (queue.Count() == 5);
	 *	...
	 * @endcode
	 *
	 */
	int	Count() const;

	// Get the data at the current queue position. 
	/**
	 * @brief   ��� �軤��̩ �Ѻ϶� �в���.
	 *
	 * @return  ����軤��̩ �Ѻ�
	 *
	 * @see  SetPosition()
	 *
	 */
	void* Data() const;

	// Pushes data to the head of queue.
	/**
	 * @brief   ���궦 �ݱ���̩ ʼ�� �Ĳ���.
	 *
	 * @param   pData [in] : ����̩ �Ѻ�
	 * @return	void
	 *
	 * @see	 PushTail(), PopHead(), PopTail()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	Queue intQueue;
	 *	intQueue.Create(5);
	 *	int* pn1 = new int;
	 *	*pn1 = 1;
	 *	int* pn2 = new int;
	 *	*pn1 = 2;
	 *	int* pn5 = new int;
	 *	*pn5 = 5;
	 *
	 *	intQueue.PushHead(pn1);
	 *	intQueue.PushHead(pn2);
	 *	intQueue.PushHead(pn5);
	 *
	 *	int* pnData = (int*)intQueue.PopHead();
	 *	assert (*pnData == 5);
	 *	pnData = (int*)intQueue.PopHead();
	 *	assert (*pnData == 2);
	 *	pnData = (int*)intQueue.PopTail();
	 *	assert (*pnData == 1);
	 *	...
	 * @endcode
	 *
	 */
	void PushHead(void* pData);

	// Pushes data to the tail of queue.
	/**
	 * @brief   ���궦 �ݱ���̩ ���� �����ٳ�.
	 *
	 * @param   pData [in] : ����̩ �Ѻ�
	 * @return	void
	 *
	 * @see	 PushHead(), PopHead(), PopTail()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	Queue intQueue;
	 *	intQueue.Create(5);
	 *	int* pn1 = new int;
	 *	*pn1 = 1;
	 *	int* pn2 = new int;
	 *	*pn5 = 2;
	 *	int* pn5 = new int;
	 *	*pn5 = 5;
	 *
	 *	intQueue.PushTail(pn1);
	 *	intQueue.PushTail(pn2);
	 *	intQueue.PushTail(pn5);
	 *
	 *	int* pnData = (int*)intQueue.PopHead();
	 *	assert (*pnData == 1);
	 *	pnData = (int*)intQueue.PopTail();
	 *	assert (*pnData == 5);
	 *	pnData = (int*)intQueue.PopTail();
	 *	assert (*pnData == 2);
	 *	...
	 * @endcode
	 *
	 */
	void PushTail(void* pData);

	// Pops data from the head of queue.
	/**
	 * @brief   �ݱ���̩ ʼ�溷 ���궦 �ų���.
	 *
	 * @return  ����̩ �Ѻ�
	 *
	 * @see	 PushHead(), PushTail(), PopTail()
	 *
	 */
	void* PopHead ();

	// Pops data from the tail of queue.
	/**
	 * @brief   �ݱ���̩ ���溷 ���궦 ǰ����.
	 *
	 * @return  ����̩ �Ѻ�
	 *
	 * @see	 PushHead(), PushTail(), PopHead()
	 *
	 */
	void* PopTail ();

	// Set current position of the queue.
	/**
	 * @brief   �ݱ���̩ �軤��̩ ̡���� �����ٳ�.
	 *
	 * @param   nWhence [in] : ����̡��
	 * @param   nIndex  [in] : ����̡���Ṣ��̩ ��������̡, ��̡��˼ 0�˺�����ʿ �ٳ�.
	 * @return	void
	 *
	 * @see  Queue::Data()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	Queue intQueue;
	 *	intQueue.Create(5);
	 *	int* pn1 = new int;
	 *	*pn1 = 1;
	 *	int* pn5 = new int;
	 *	*pn5 = 5;
	 *
	 *	intQueue.PushTail(pn1);
	 *	intQueue.PushTail(pn5);
	 *
	 *	int* pnData;
	 *	intQueue.SetPosition (Queue::POS_HEAD, 1);
	 *	pnData = (int*)intQueue.Data();
	 *	assert (*pnData == 5);
	 *	...
	 * @endcode
	 *
	 */
	void SetPosition(int nWhence, int nIndex);

private:
	//! ����̡��
	int		m_nFirst;
	//! ����̡��
	int		m_nLast;
	//! ���̡��
	int		m_nCur;

	//! �ݱ����� ���̰��� ̺��̩ ����
	int		m_nCount;
	//! �ݱ����� ���̰��� ̺��̩ �ͱ�
	int		m_nSize;
	//! �ݱ���̩ ��� �軤��
	char*	m_pData;
};

}

#endif //_QUEUE_H_
