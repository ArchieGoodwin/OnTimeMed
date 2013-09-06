/*!
 * \file	PtrArray.h
 * \ingroup base
 * \brief	¼ÑºÏ´ÉÌ© ¹ÉµÛË¾ ±ã¶®Â×±¨ Ì¡ÂÙ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _PTRARRAY_H_
#define _PTRARRAY_H_

#include "Object.h"

namespace CVLib 
{

/**
 * @brief  ¼ÑºÏ¹ÉµÛ¼¿»õË¾ ¼êÃÔÂ×±¨ Ì¡ÂÙ ¿Ğµ¹º÷.
 */
class CVLIB_DECLSPEC PtrArray : public Object
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	PtrArray();
	//! ÃÍ¾Ë»ô
	virtual ~PtrArray();

	// Attributes
	/**
	 * @brief   ÌººÏ´ÉÌ© ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @return  ¹ÉµÛÌ© ¿Í±¨, ½éºã´ÉË¼ 0Ëæ ±¨½óÂ×°Ö ËØËº·àµá ¹ÉµÛÌ© ¿Í±¨²÷ \n
	 *			°¡»ş ¿Ï ½éºã¸ó³Ş 1 ¿Í³Ş.
	 *
	 * @see  GetUpperBound(), SetSize()
	 *
	 * @par Example:
	 * @code
	 *  
	 *	PtrArray array;
	 *	Vec* pVec = new Vec;
	 *	array.Add (pVec);
	 *	assert( 1 == array.GetSize() );
	 *  ...
	 * @endcode
	 *
	 */
	int GetSize() const;

	/**
	 * @brief   Ë±Ã£ÂÙ °¡»ş ¿Ï ½éºã¶¦ ´ó´®¶°³Ş.
	 *
	 * @return  °¡»ş ¿Ï ½éºã(0Ëæ ±¨½óÂÜ)
	 *
	 * @see  GetSize(), SetSize()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	assert( 1 == array.GetUpperBound() );	//Largest index
	 *  ...
	 * @endcode
	 *
	 */
	int GetUpperBound() const;

	/**
	 * @brief   ¹ÉµÛËæ ÁäÂÜ´õ ÌººÏ´ÉÌ© ºã¶¦ º¼¼³ÂÙ³Ş.
	 *
	 * @param   nNewSize [in] :	»¯ ¹ÉµÛÌ© ¿Í±¨, 0ËËº¬ËËÊŞÊ¿ ÂÙ³Ş.
	 * @param   nGrowBy	 [in] :	¹ÉµÛÌ© ¿Í±¨¼ç°¡°¡ Â­ÊıÂ×·² ÂÚ³ëÂ×±¨ Ì¡ÂÙ ÌººÏ´ÉÌ© ¾×ºÏºã
	 * @return  void 
	 *
	 * @see  GetSize()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array1;
	 *	PtrArray array2;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array1.Add (pVec1);	
	 *	array2.Add (pVec2);	
	 *	
	 *  array1.SetSize(array1.m_nSize + array2.m_nSize);
	 *  assert(array1.m_nSize == 2);
	 *  ...
	 * @endcode
	 *
	 */
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations for Cleaning up
	/**
	 * @brief   Âï¼õ º¬±ËË§Ì© ¶®Ë¦Â×¼è Ê±²÷ ±¨ÊÍ±¨¶¦ ·º´¸ ÃÍ¸ÒÂÙ³Ş.
	 *
	 * @return  void 
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	
	 *	array.Add (pVec2);	
	 *	...
	 *  array.FreeExtra();
	 *  assert(array.m_nSize == array.m_nMaxSize);
	 *  ...
	 * @endcode
	 *
	 */
	void FreeExtra();

	/**
	 * @brief   ËË ¹ÉµÛµá¹¢À¾ ·º´Ç ÌººÏ´ÉË¾ ½£°ºÂÙ³Ş.
	 *
	 * @return  void 
     *
	 * @see  GetSize(), SetSize()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	assert( 2 == array.GetSize() );
	 *	array.RemoveAll(); // Pointers removed but objects not deleted.
	 *	assert( 0 == array.GetSize() );
	 *	delete pVec1;
	 *	delete pVec2;	// Cleans up memory.
	 *
	 * @endcode
	 *
	 */
	void RemoveAll();

	// Operations for Accessing elements
	/**
	 * @brief   ¼ÑÊÌ¼ê ½éºãËæº·Ì© °ªË¾ ÊĞ²÷³Ş.
	 *
	 * @param   nIndex [in]	: 0ËËº¬ËË°Ö GetUpperBound°¡ ´ó´®¶®²÷ °ªËËÂ×ËÍ Êú°ıºã½éºã
	 * @return  ËË ½éºãËæ ËØ²÷ ¼ÑºÏ
	 *
	 * @see  SetAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	assert( array.GetAt(0) == pVec1 );
	 *	...
	 * @endcode
	 *
	 */
	void* GetAt(int nIndex) const;

	/**
	 * @brief   ¼ÑÊÌ¼ê ½éºãËæ ´İÂ×ÊŞ °ªË¾ º¼¼³ÂÙ³Ş. ¹ÉµÛË¼ »şºÂÂ×¼è Ê±²÷³Ş.
	 *
	 * @param   nIndex	   [in]	: 0ËËº¬ËË°Ö GetUpperBound°¡ ´ó´®¶®²÷ °ªËËÂ×ËÍ Êú°ıºã½éºã
	 * @param   newElement [in] : ¹ÉµÛËæ ºªËÓ´õ ¼ÑºÏ
	 * @return  void 
	 *
	 * @see  GetAt(), SetAtGrow(), ElementAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.SetAt( 0, pVec3 );
	 *	assert( array.GetAt(0) == pVec3 );
	 *	...
	 * @endcode
	 *
	 */
	void SetAt(int nIndex, void* newElement);

	/**
	 * @brief   ¹ÉµÛ³­¹¢Ì© ÌººÏ¼ÑºÏËæ ´İÂÙ ½Ø¼¿¼ÑºÏ¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   nIndex [in]	: 0ËËº¬ËË°Ö GetUpperBound°¡ ´ó´®¶®²÷ °ªËËÂ×ËÍ Êú°ıºã½éºã
	 * @return  ¼ÑºÏÌ© ½Ø¼¿
	 *
	 * @see  operator[]
	 *
	 */
	void*& ElementAt(int nIndex);

	// Operations for Direct Access to the element data (may return NULL)
	/**
	 * @brief   ¹ÉµÛÊ¯Ì© ÌººÏ´ÉËæ ´İÂÙ ¼é¼±¼¬ËÍ Âö¾¥Ë¾ ¼êÃÔÂÙ³Ş.
	 <pre> NULLËÎºã ËØ³Ş. </pre>
	 *
	 * @return  ¼ÑºÏ´ÉÌ© ¹ÉµÛËæ ´İÂÙ ¼ÑºÏ
	 *
	 * @see  GetAt(), SetAt(), ElementAt()
	 *
	 */
	const void** GetData() const;

	/**
	 * @brief   ¹ÉµÛÊ¯Ì© ÌººÏ´ÉËæ ´İÂÙ ¼é¼±¼¬ËÍ Âö¾¥Ë¾ ¼êÃÔÂÙ³Ş.
	 <pre> NULLËÎºã ËØ³Ş. </pre>
	 *
	 * @return  ¼ÑºÏ´ÉÌ© ¹ÉµÛËæ ´İÂÙ ¼ÑºÏ
	 *
	 * @see  GetData()
	 *
	 */
	void** GetData();

	// Operations for Potentially growing the array
	/**
	 * @brief   ¼ÑÊÌ¼ê ½éºãËæ ´İÂ×ÊŞ °ªË¾ º¼¼³ÂÙ³Ş. Â­ÊıÂ×³Ş·² ¹ÉµÛË¾ »şºÂ»¤¿Ö³Ş.
	 *
	 * @param   nIndex	   [in]	: 0ËËº¬ËÍ Êú°ıºã
	 * @param   newElement [in]	: ËË ¹ÉµÛËæ ¾¡°¡ÂÚ ¼ÑºÏËË³Ş. NULL°ª´ª ÂâË¦ÂÙ³Ş.
	 * @return  void 
	 *
	 * @see  GetAt(), ElementAt(), operator[]
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.SetAt( 3, pVec3 );
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	SetAtGrow example: A PtrArray with 4 elements
	 *		[0] = pVec1
	 *		[1] = pVec2
	 *		[2] = NULL
	 *		[3] = pVec3
	 *
	 * @endcode
	 *
	 */
	void SetAtGrow(int nIndex, void* newElement);

	/**
	 * @brief   ¹ÉµÛÌ© ÄùËæ ÌººÏ¶¦ ¾¡°¡ÂÙ³Ş. Â­ÊıÂ×³Ş·² ¹ÉµÛË¾ »şºÂ»¤¿Ö³Ş.
	 *
	 * @param   newElement [in]	: ËË ¹ÉµÛËæ ¾¡°¡´õ ÌººÏ
	 * @return  ¾¡°¡´ô ÌººÏÌ© ½éºã
	 *
	 * @see  SetAt(), SetAtGrow(), InsertAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	Add example: A PtrArray with 2 elements
	 *		[0] = pVec1
	 *		[1] = pVec2
	 *
	 * @endcode
	 *
	 */
	int Add(void* newElement);

	/**
	 * @brief   ¹ÉµÛËæ ³Ş¶¥ ¹ÉµÛË¾ ¾¡°¡ÂÙ³Ş. Â­ÊıÂ×³Ş·² ¹ÉµÛË¾ »şºÂ»¤¾ô³Ş.
	 *
	 * @param   src [in] : ËË ¹ÉµÛËæ ¾¡°¡´õ ÌººÏ´ÉÌ© Ìº½ç
	 * @return  ½åËÁËºµá ¾¡°¡´ô ÌººÏÌ© ½éºã
	 *
	 * @see PtrArray::Copy()
	 *
	 */
	int Append(const PtrArray& src);

	/**
	 * @brief   ¹ÉµÛËæ ³Ş¶¥ ¹ÉµÛË¾ ¸ôº¡ÂÙ³Ş. Â­ÊıÂ×³Ş·² ¹ÉµÛË¾ »şºÂ»¤¾ô³Ş.
	 *
	 * @param   src [in] : ¹ÉµÛËæ ¸ôº¡ÂÚ ÌººÏ´ÉÌ© Ìº½ç
	 * @return  void 
	 *
	 * @see PtrArray::Append()
	 *
	 */
	void Copy(const PtrArray& src);

	// overloaded operator helpers
	/**
	 * @brief   ¼ÑÊÌ¼ê ½éºãËæ ËØ²÷ ÌººÏ¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   nIndex [in]	: 0ËËº¬ËË°Ö GetUpperBound()°¡ ´ó´®¶®²÷ °ªËËÂ×ËÍ Êú°ıºã½éºã
	 * @return  ËË ½éºãËæ ËØ²÷ ¼ÑºÏ 
	 *
	 * @see  GetAt(), SetAt()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	Vec* pA = (Vec*)array[0];
	 *	assert( *pA == pVec1 ); // Get element 0
	 *	array[0] = pVec3; // Replace element 0
	 *	delete pA;
	 *	assert( array[0] == pVec3 ); // Get new element 0
	 *  ...
	 * @endcode
	 *
	 */
	void* operator[](int nIndex) const;

	/**
	 * @brief   ¹ÉµÛ³­¹¢Ì© ÌººÏ¼ÑºÏËæ ´İÂÙ ½Ø¼¿¼ÑºÏ¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   nIndex [in]	: 0ËËº¬ËË°Ö GetUpperBound()°¡ ´ó´®¶®²÷ °ªËËÂ×ËÍ Êú°ıºã½éºã 
	 * @return  ¼ÑºÏÌ© ½Ø¼¿ 
	 *
	 * @see	 operator[]
	 *
	 */
	void*& operator[](int nIndex);

	// Operations that move elements around
	/**
	 * @brief   Àã¼³ÂÙ Ì¡¾·Ëæ ÌººÏ¶¦ ºªËÓÂÙ³Ş.
	 *
	 * @param   nIndex	   [in]	: GetUpperBound°¡ ´ó´®¶° °ª¸ó³Ş ¿Ğºã´ª ËØ²÷ Êú°ıºã½éºã
	 * @param   newElement [in]	: ËË ¹ÉµÛËæ Ì¡¾·ÂÚ ÌººÏ, NULLËÎºã ËØ³Ş.
	 * @param   nCount	   [in]	: ËË ÌººÏºã¶í¿Ñ ºªËÓ´ô³Ş. default²÷ 1ËË³Ş.
	 * @return  void 
	 *
	 * @see  SetAt(), RemoveAt()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.InsertAt( 1, pVec3 );
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	InsertAt example: A PtrArray with 3 elements
	 *		[0] = pVec1
	 *		[1] = pVec3
	 *		[2] = pVec2
	 *
	 * @endcode
	 *
	 */
	void InsertAt(int nIndex, void* newElement, int nCount = 1);

	/**
	 * @brief   Àã¼³ÂÙ Ì¡¾·Ëæ ÌººÏ¶¦ ºªËÓÂÙ³Ş.
	 *
	 * @param   nStartIndex	[in] : GetUpperBound°¡ ´ó´®¶° °ª¸ó³Ş ¿Ğºã´ª ËØ²÷ Êú°ıºã½éºã
	 * @param   pNewArray	[in] : ËË ¹ÉµÛËæ ¾¡°¡´õ ÌººÏ´ÉË¾ ÁäÂÜÂ×²÷ ³Ş¶¥ ¹ÉµÛ
	 * @return  void
	 *
	 * @see  InsertAt()
	 *
	 */
	void InsertAt(int nStartIndex, PtrArray* pNewArray);

	/**
	 * @brief   Àã¼³ÂÙ Ì¡¾·Ëæ ËØ²÷ ÌººÏ´ÉË¾ º¢½£ÂÙ³Ş.
	 *
	 * @param   nIndex [in]	: 0ËËº¬ËË·²º· GetUpperBound°¡ ´ó´®¶° °ªËËÂ×ËÍ Êú°ıºã½éºã
	 * @param   nCount [in]	: º¢½£ÂÚ ÌººÏ´ÉÌ© ºã
	 * @return  void 
	 *
	 * @see  GetAt(), SetAtGrow(), ElementAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.RemoveAt( 0 );
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	RemoveAt example: A PtrArray with 1 elements
	 *		[0] = pVec2
	 *
	 * @endcode
	 *
	 */
	void RemoveAt(int nIndex, int nCount = 1);

// Implementation
protected:
	//! »ôµê¹ÉµÛ 
	void** m_pData;
	//! ÂÚ³ë´ô ÊıºÏºã 
	int m_nSize;
	//! ¾×´İÊıºÏºã 
	int m_nMaxSize;
	//! ¹ÉµÛÌ© »şºÂºã 
	int m_nGrowBy;

protected:
	// local typedefs for class templates
	typedef void* BASE_TYPE;
	typedef void* BASE_ARG_TYPE;
};

}

#endif //_PTRARRAY_H_
