
/*!
 * \file	PtrQueue.h
 * \ingroup base
 * \brief	¼ÑºÏ´Ý±¨µÛ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _PTRQUEUE_H_
#define _PTRQUEUE_H_
#include "Object.h"
#include <assert.h>

namespace CVLib 
{
	
/**
 * @brief  ¼ÑºÏ´Ý±¨µÛ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC PtrQueue : public Object
{
public:
	/**
	 *  @brief  ´Ý±¨µÛÌ© Ì¡¾·¶¦ ¼è¼³Â×²÷ µÛ°ºÂô 
	 */
	enum
	{
		POS_HEAD   = 0,    /**< ´Ý±¨µÛÌ© »¤»õ */
		POS_TAIL   = 1     /**< ´Ý±¨µÛÌ© Äù */
	};
	
public:
	// Constructor and Destructor
	//! °é¾¢»ô
	PtrQueue ();

	/**
	 * @brief   °é¾¢»ô
	 *
	 * @param   nCount [in] : ÌººÏÌ© ±¶ºã
	 */
	PtrQueue (int nCount);
	
	//! ÃÍ¾Ë»ô
	virtual ~PtrQueue ();

	// Check if the instance was created
	/**
	 * @brief   ´Ý±¨µÛËË ½Û¼¿´óÊî²÷°¡¶¦ ±ýÀ°³­²÷ ±¨¸Ë
	 *
	 * @return  ½Û¼¿´óÊîËº·² true, Ê­³£·² false
	 *
	 * @see	 Create(), Release()
	 *
	 */
	bool FlagCreate ();

	// Initialize the queue.
	/**
	 * @brief   ´Ý±¨µÛË¾ ½Û¼¿ÂÙ³Þ.
	 *
	 * @param   nCount [in] : ´Ý±¨µÛÌººÏÌ© ±¶ºã
	 * @return  ½Û¼¿´óÊîËº·² 0ËË Ê­³¥°ª, Ê­³£·² 0
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
	 * @brief   Ãú¸ó´ô ±¨ÊÍ±¨¶¦ ÃÍ½£ÂÙ³Þ.
	 *
	 * @see	 Create(), FlagCreate()
	 *
	 */
	virtual void Release();

	// Get the size of data.
	/**
	 * @brief   ÌººÏÌ© ±¶ºã¶¦ ÊÐ²÷³Þ.
	 *
	 * @return	ÌººÏ±¶ºã
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
	 * @brief   Âï¼õ ¼è»¤±¨Ì© ¼ÑºÏ¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  Âï¼õ¼è»¤±¨Ì© ¼ÑºÏ
	 *
	 * @see	 SetPosition()
	 *
	 */
	void* Data() const;

	// Pushes data to the head of queue.
	/**
	 * @brief   »ôµê¶¦ ´Ý±¨µÛÌ© Ê¼Ëæ ²Ä²÷³Þ.
	 *
	 * @param   pData [in] : »ôµêÌ© ¼ÑºÏ
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
	 * @brief   »ôµê¶¦ ´Ý±¨µÛÌ© ÄùËæ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param   pData [in] : »ôµêÌ© ¼ÑºÏ
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
	 * @param   nWhence [in] : »¤»õÌ¡¾·
	 * @param   nIndex  [in] : »¤»õÌ¡¾·µá¹¢À¾Ì© ÁÝÌ¡
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
	//! ¼ÑºÏ´Ý±¨µÛ»ôµêÌ© ¼è¼¬»ô
	void*	m_pData;
};

}

#endif //_PTRQUEUE_H_
 