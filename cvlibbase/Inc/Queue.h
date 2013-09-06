/*!
 * \file	Queue.h
 * \ingroup base
 * \brief	´Ý±¨µÛ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Object.h"
#include <assert.h>

namespace CVLib 
{
	
/**
 * @brief ´Ý±¨µÛ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC Queue : public Object
{
public:

	//! ´Ý±¨µÛÌ© Ì¡¾·¶¦ ¼è¼³Â×²÷ µÛ°ºÂô 
	enum
	{
		POS_HEAD   = 0,    /**< ´Ý±¨µÛÌ© »¤»õ */
		POS_TAIL   = 1     /**< ´Ý±¨µÛÌ© Äù   */
	};

public:
	// Constructor and Destructor
	//! °é¾¢»ô
	Queue ();

	/**
	 * @brief   °é¾¢»ô
	 *
	 * @param   nCount [in] : ÌººÏÌ© ±¶ºã
	 * @param   nSize  [in] : ÌººÏÌ© ¿Í±¨
	 *
	 */
	Queue (int nCount, int nSize);
	
	//! ÃÍ¾Ë»ô
	virtual ~Queue ();

	// Check if the instance was created
	/**
	 * @brief   ´Ý±¨µÛËË ½Û¼¿´óÊî²÷°¡¶¦ °Áº¡ÂÙ³Þ.
	 *
	 * @return  ½Û¼¿´óÊîËº·² 1, Ê­³£·² 0
	 *
	 * @see	 Create(), Release()
	 *
	 */
	int FlagCreate ();

	// Initialize the queue.
	/**
	 * @brief   ´Ý±¨µÛË¾ ½Û¼¿ÂÙ³Þ.
	 *
	 * @param   nCount [in] : ´Ý±¨µÛÌººÏÌ© ±¶ºã
	 * @param   nSize  [in] : ´Ý±¨µÛÌººÏÌ© ¿Í±¨
	 * @return  ½Û¼¿´óÊîËº·² 1, Ê­³£·² 0
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
	 * @brief   Ãú¸ó´ô ±¨ÊÍ±¨¶¦ ÃÍ½£ÂÙ³Þ.
	 *
	 * @return	void
	 *
	 * @see	 Create(), FlagCreate()
	 *
	 */
	virtual void Release();

	// Init contents of array.
	/**
	 * @brief   ½ó±¨Ãù¶¦ ¼êÃÔÂÙ³Þ.
	 *
	 * @return	void
	 *
	 * @see	Queue::Create()
	 *
	 */
	void Init();

	// Get the size of data.
	/**
	 * @brief   ÌººÏÌ© ±¶ºã¶¦ ÊÐ²÷³Þ.
	 *
	 * @return	ÌººÏ±¶ºã
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
	 * @brief   Âï¼õ ¼è»¤±¨Ì© ¼ÑºÏ¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  Âï¼õ¼è»¤±¨Ì© ¼ÑºÏ
	 *
	 * @see  SetPosition()
	 *
	 */
	void* Data() const;

	// Pushes data to the head of queue.
	/**
	 * @brief   »ôµê¶¦ ´Ý±¨µÛÌ© Ê¼Ëæ ²Ä²÷³Þ.
	 *
	 * @param   pData [in] : »ôµêÌ© ¼ÑºÏ
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
	 * @brief   »ôµê¶¦ ´Ý±¨µÛÌ© ÄùËæ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param   pData [in] : »ôµêÌ© ¼ÑºÏ
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
	 * @brief   ´Ý±¨µÛÌ© Ê¼Ëæº· »ôµê¶¦ ÄÅ³¯³Þ.
	 *
	 * @return  »ôµêÌ© ¼ÑºÏ
	 *
	 * @see	 PushHead(), PushTail(), PopTail()
	 *
	 */
	void* PopHead ();

	// Pops data from the tail of queue.
	/**
	 * @brief   ´Ý±¨µÛÌ© ÄùËæº· »ôµê¶¦ Ç°²÷³Þ.
	 *
	 * @return  »ôµêÌ© ¼ÑºÏ
	 *
	 * @see	 PushHead(), PushTail(), PopHead()
	 *
	 */
	void* PopTail ();

	// Set current position of the queue.
	/**
	 * @brief   ´Ý±¨µÛÌ© ¼è»¤±¨Ì© Ì¡¾·¶¦ º¼¼³ÂÙ³Þ.
	 *
	 * @param   nWhence [in] : ±¨¼ÓÌ¡¾·
	 * @param   nIndex  [in] : ±¨¼ÓÌ¡¾·µá¹¢À¾Ì© Êú°ýºãÁÝÌ¡, ÁÝÌ¡°ªË¼ 0ËËº¬ËËÊÞÊ¿ ÂÙ³Þ.
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
	//! »¤»õÌ¡¾·
	int		m_nFirst;
	//! ¶ë¼è¶ìÌ¡¾·
	int		m_nLast;
	//! Âï¼õÌ¡¾·
	int		m_nCur;

	//! ´Ý±¨µÛËæ ´ÉÊÌ°¡²÷ ÌººÏÌ© ±¶ºã
	int		m_nCount;
	//! ´Ý±¨µÛËæ ´ÉÊÌ°¡²÷ ÌººÏÌ© ¿Í±¨
	int		m_nSize;
	//! ´Ý±¨µÛÌ© Âï¼õ ¼è»¤±¨
	char*	m_pData;
};

}

#endif //_QUEUE_H_
