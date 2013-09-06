
/*!
 * \file	PtrQueue.h
 * \ingroup base
 * \brief	�Ѻϴݱ��ۿе����� ��̩��.
 * \author	����˧
 */

#ifndef _PTRQUEUE_H_
#define _PTRQUEUE_H_
#include "Object.h"
#include <assert.h>

namespace CVLib 
{
	
/**
 * @brief  �Ѻϴݱ��ۿе���
 */
class CVLIB_DECLSPEC PtrQueue : public Object
{
public:
	/**
	 *  @brief  �ݱ���̩ ̡���� �輳�ײ� �۰��� 
	 */
	enum
	{
		POS_HEAD   = 0,    /**< �ݱ���̩ ���� */
		POS_TAIL   = 1     /**< �ݱ���̩ �� */
	};
	
public:
	// Constructor and Destructor
	//! �龢��
	PtrQueue ();

	/**
	 * @brief   �龢��
	 *
	 * @param   nCount [in] : ̺��̩ ����
	 */
	PtrQueue (int nCount);
	
	//! �;˻�
	virtual ~PtrQueue ();

	// Check if the instance was created
	/**
	 * @brief   �ݱ����� �ۼ���������� �������� ����
	 *
	 * @return  �ۼ�����˺�� true, ʭ���� false
	 *
	 * @see	 Create(), Release()
	 *
	 */
	bool FlagCreate ();

	// Initialize the queue.
	/**
	 * @brief   �ݱ���˾ �ۼ��ٳ�.
	 *
	 * @param   nCount [in] : �ݱ���̺��̩ ����
	 * @return  �ۼ�����˺�� 0�� ʭ����, ʭ���� 0
	 *
	 * @see	 FlagCreate(), Release()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	PtrQueue queue;
	 *	assert (queue.FlagCreate() == false);
	 *	queue.Create (5);
	 *	assert (queue.FlagCreate() == true);
	 *	assert (queue.Create(3) == false);
	 *	queue.Release();
	 *  ...
	 * @endcode
	 *
	 */
	bool Create(int	nCount);

	// Destroy the queue.
	/**
	 * @brief   ����� ���ͱ��� �ͽ��ٳ�.
	 *
	 * @see	 Create(), FlagCreate()
	 *
	 */
	virtual void Release();

	// Get the size of data.
	/**
	 * @brief   ̺��̩ ���㶦 �в���.
	 *
	 * @return	̺�ϱ���
	 *
	 * @par	Example:
	 * @code
	 *
	 *	PtrQueue queue;
	 *	queue.Create (5);
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
	 * @see	 SetPosition()
	 *
	 */
	void* Data() const;

	// Pushes data to the head of queue.
	/**
	 * @brief   ���궦 �ݱ���̩ ʼ�� �Ĳ���.
	 *
	 * @param   pData [in] : ����̩ �Ѻ�
	 * @return  void
	 *
	 * @see	 PushTail(), PopHead(), PopTail()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	PtrQueue intQueue;
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
	 *	pnData = (int*)intQueue.PopTail();
	 *	assert (*pnData == 1);
	 *	pnData = (int*)intQueue.PopTail();
	 *	assert (*pnData == 2);
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
	 * @return  void
	 *
	 * @see	 PushHead(), PopHead(), PopTail()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	PtrQueue intQueue;
	 *	intQueue.Create(5);
	 *	int* pn1 = new int;
	 *	*pn1 = 1;
	 *	int* pn2 = new int;
	 *	*pn1 = 2;
	 *	int* pn5 = new int;
	 *	*pn5 = 5;
	 *
	 *	intQueue.PushTail(pn1);
	 *	intQueue.PushTail(pn2);
	 *	intQueue.PushTail(pn5);
	 *
	 *	int* pnData = (int*)intQueue.PopHead();
	 *	assert (*pnData == 1);
	 *	pnData = (int*)intQueue.PopHead();
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
	 * @param   nIndex  [in] : ����̡���Ṣ��̩ ��̡
	 * @return	void
	 *
	 * @see	 Data()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	PtrQueue intQueue;
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
	 *	intQueue.SetPosition (PtrQueue::POS_HEAD, 1);
	 *	pnData = (int*)intQueue.Data();
	 *	assert (*pnData == 5);
	 *	...
	 * @endcode
	 *
	 */
	void SetPosition(int nWhence, int nIndex);

private:
	//! �Ѻϴݱ��ۻ���̩ �輬��
	void*	m_pData;
};

}

#endif //_PTRQUEUE_H_
 