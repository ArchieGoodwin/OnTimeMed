/*!
 * \file	PtrList.h
 * \ingroup base
 * \brief	µÚ°Î·»µâË¾ ±ã¶®Â×²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	¹ÊËÎ²§, ±®½èË§
 */

#ifndef _PTRLIST_H_
#define _PTRLIST_H_

#include "Object.h"

namespace CVLib
{

/**
 * @brief µÚ°Î·»µâË¾ ±ã¶®Â×²÷ ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC PtrList : public Object
{
public:
	
	/**
	 * @brief µÚ°Î·»µâ¾¡°¡¸Ò»¥Ë¾ ¼è¼³Â×²÷ µÛ°ºÂô
	 */
	enum LISTAddMode
	{
		ADD_BEFORE = 0,     /**< »ôµê²÷ ·»µâÌ© Âï Ì¡¾· ¸Æµá Ê¼Ëæ ¾¡°¡´ô³Þ.*/
		ADD_AFTER  = 1,     /**< »ôµê²÷ ·»µâÌ© Âï Ì¡¾· ¸Æµá ´ûËæ ¾¡°¡´ô³Þ.*/
	};

	/**
	 * @brief  ·»µâÌ© Ì¡¾·¶¦ ¼è¼³Â×²÷ µÛ°ºÂô 
	 */
	enum LISTPositionType
	{
		POS_BEGIN   = 0,    /**< ·»µâÌ© »¤»õÌ¡¾· */
		POS_CURRENT = 1,    /**< ·»µâÌ© Âï¼õÌ¡¾· */
		POS_END     = 2  	/**< ·»µâÌ© ÄùÌ¡¾·   */ 
	};

public:
	
	// Constructor and Destructor
	//! °é¾¢»ô
	PtrList();
	//! ÃÍ¾Ë»ô 
	virtual ~PtrList();
	
	// Check if the instance was created
	/**
	 * @brief   ·»µâÌ© Ë±Ã£ºÂË¾ ÁÆ¼³ÂÙ³Þ.
	 *
	 * @return	Ë±Ã£Â×·² true, Ê­³£·² false
	 *
	 * @see PtrList::Create()
	 *
	 */
	bool FlagCreate(void) { return m_pData != NULL; }

	// Initialize the list.
	/**
	 * @brief   ·»µâË¾ »¶ºÂÂÙ³Þ.
	 *
	 * @return	ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  FlagCreate(), Release()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *	...
	 *	listDB.Release();
	 *
	 * @endcode
	 *
	 */
	bool Create(void);
	
	// Destroy the list and deletes all nodes.
	/**
	 * @brief   ·»µâË¾ ÁÄ±ÐÂÙ³Þ.
	 *
	 * @return  void
	 *
	 * @see  Create(), FlagCreate()
	 *
	 */
	virtual void Release(void);

	// Get the size of data.
	/**
	 * @brief   ·»µâÊ¯Ì© ¶ë´ÑÌ© ºã¶¦ ´ó´®¶°³Þ.
	 *
	 * @return  ¶ë´Ñºã
	 *
	 * @see  Add(), Remove()
	 *
	 */
	unsigned long Count(void) const;
	
	// Get the data at the current list position. 
	/**
	 * @brief   Âï¼õ ¼è»¤»ôËæº·Ì© »ôµê¶¦ ´ó´®¶°³Þ.
	 * @brief   Âï¼õ ¼è»¤»ôËæº·Ì© »ô¶¦ ´ó´®¶°³Þ.
	 *
	 * @return  ¼ÑºÏ, ½Û¼¿´ó¼è Ê±Ê¾Ëº·² NULLË¾ ´ó´®¶°³Þ.
	 *
	 * @see  SetPosition()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *	assert( 2 == listDB.Count() );
	 *
	 *	listDB.SetPosition(PtrList::POS_BEGIN, 0);	//Set element 0
	 *	assert( pVec1 == listDB.Data() );
	 *	...
	 * @endcode
	 *
	 */
	void* Data(void) const;

	// Adds data to the list.
	/**
	 * @brief   ¸Ò»¥Ëæ Åàµ¹ »ôµê¶¦ ¾¡°¡/ºªËÓ ÂÙ³Þ.
	 *
	 * @param   pData [in] : »ôµê
	 * @param   nMode [in] : ¸Ò»¥ : ¾¡°¡/ºªËÓ ( ±¨¼³Ëºµá ·»µâ´ûËæ ¾¡°¡ËË³Þ : ADD_AFTER )
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see  Count(), Remove()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagpVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *	assert( 2 == listDB.Count() );
	 *
	 *	listDB.Remove ();	//Remove element 1
	 *	assert( 1 == listDB.Count() );
	 *	...
	 * @endcode
	 *
	 */
	bool Add(const void* pData, int nMode = ADD_AFTER);
	
	// Search the list.
	/**
	 * @brief   »ôµêÌ© ¼è¼¬»ô¶¦ ½Ü²÷³Þ.
	 <pre>
		»ôµê¼è¼¬»ô°¡ ·»µâÊ¯Ëæ ¼Á¼õÂ×·² Âï¼õ Ì¡¾·¼è»¤±¨¶¦ 
 		»ôµê¼è¼¬»ô¶¦ ÁäÂÜÂ×²÷ ·ô´ÎËæ »¯µá º¼¼³ÂÙ³Þ.
	 </pre>
	 *
	 * @param   pData [in] : ½ÜËºµØ²÷ »ôµêÌ© ¼ÑºÏ
	 * @return  »ôµê¶¦ ½ÜËº·² true, Ê­³£·² false¶¦ ´ó´®¶°³Þ.
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *
	 *	assert( listDB.Find(pVec1) == true );
	 *	...
	 * @endcode
	 *
	 */
	bool Find(const void* pData) const;
	
	// Set current position of the list.
	/**
	 * @brief   ¼ÑºÏ¶¦ º¼¼³ÂÙ³Þ.
	 *
	 * @param   nWhence	[in] : »¤»õÌ¡¾·
	 * @param   nIndex	[in] : »¤»õÌ¡¾·Ëæº·Ì© ¸êÌ¡µÌ
	 * @return  ºÂ°áÂ×·² true, »¨Â²Â×·² false
	 *
	 * @see  Data()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *	assert( 2 == listDB.Count() );
	 *
	 *	listDB.SetPosition(PtrList::POS_BEGIN, 0);	//Set element 0
	 *	assert( pVec1 == listDB.Data() );
	 *	...
	 * @endcode
	 *
	 */
	bool SetPosition(int nWhence, int nIndex);
	
	// Remove an element from the list.
	/**
	 * @brief	Âï¼õ Ì¡¾·Ëæº·Ì© ÌººÏ¶¦ º¢½£ÂÙ³Þ.
	 *
     * @return  void
	 *
	 * @see  Add()
	 *
	 */
	void Remove(void);

private:
	//! µÚ°Î·»µâ»ôµêËæ ´ÝÂÙ ¼è¼¬»ô
	void*	m_pData;
};

}

#endif // _PTRLIST_H_
