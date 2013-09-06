
/*!
 * \file    StringArray.h
 * \ingroup base
 * \brief   ·Í»ôµÛ¹ÉµÛ¼¿»õË¾ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _STRINGARRAY_H_
#define _STRINGARRAY_H_

#include "SString.h"
#include "Object.h"

namespace CVLib
{

/**
 * @brief ·Í»ôµÛ¹ÉµÛ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC StringArray : public Object
{
public:
	//! °é¾¢»ô 
	StringArray();
	StringArray(const StringArray& t);
	//! ÃÍ¾Ë»ô 
	virtual ~StringArray();
	
	// Attributes
	/**
	 * @brief   ¹ÉµÛÌ© ¿Í±¨(ÌººÏºã)¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  ÌººÏºã 
	 *
	 * @see   SetSize()
	 *
	 */
	int GetSize() const;
	/**
	 * @brief   ¹ÉµÛÌ© º¬±Ë¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  º¬±Ë 
	 *
	 */
	int GetUpperBound() const;
	
	/**
	 * @brief   ¹ÉµÛÌ© ¿Í±¨¶¦ »¯µá º¼¼³ÂÙ³Þ. 
	 *
	 * @param   nNewSize [in] : º¼¼³Â×µØ²÷ ¿Í±¨ 
	 * @param   nGrowBy  [in] : »þºÂºã 
	 * @return  void
	 *
	 * @see  GetSize()
	 *
	 */
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	/**
	 * @brief   ¹ÉµÛÌ© ¹À Ì°¾©±¨°éÊßË¾ ÃÍ¸ÒÂÙ³Þ.
	 *
	 * @return  void
	 *
	 */
	void FreeExtra();
	
	/**
	 * @brief   ¹ÉµÛË¾ ÃÍ¸ÒÂÙ³Þ.
	 *
	 * @return  void 
	 *
	 */
	void RemoveAll();
	
	// Accessing elements
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·Ì© ÌººÏ¶¦ ÊÐ²÷³Þ. 
	 *
	 * @param   nIndex [in] : ÊÐËºµØ²÷ ÌººÏÌ© Ì¡¾·¸ÜÂö 
	 * @return  ÊÐÊÌ¼ê ÌººÏ 
	 *
	 * @see  SetAt(int nIndex, char* newElement), ElementAt()
	 *
	 */
	SString GetAt(int nIndex) const;
	
	/**
	 * @brief   ¹ÉµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ »¯ ÌººÏ¶¦ º¼¼³ÂÙ³Þ.
	 *
	 * @param   nIndex     [in] : Ì¡¾· 
	 * @param   newElement [in] : »¯ ÌººÏ 
	 * @return  void
	 *
	 * @see   GetAt()
	 *
	 */
	void SetAt(int nIndex, char* newElement);
	
	/**
	 * @brief  ¹ÉµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ »¯ ÌººÏ¶¦ º¼¼³ÂÙ³Þ.
	 *
	 * @param   nIndex     [in] : Ì¡¾· 
	 * @param   newElement [in] : »¯ ÌººÏ 
	 * @return  void
	 *
	 * @see   GetAt()
	 *
	 */
	void SetAt(int nIndex, SString& newElement);
	
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·Ì© ÌººÏ¶¦ ÊÐ²÷³Þ. 
	 *
	 * @param   nIndex [in] : ÊÐËºµØ²÷ ÌººÏÌ© Ì¡¾·¸ÜÂö 
	 * @return  ÌººÏÌ© ¼ÑºÏ  
	 *
	 * @see   GetAt()
	 *
	 */
	SString& ElementAt(int nIndex);
	
	// Direct Access to the element data (may return NULL)
	/**
	 * @brief   »ôµê¹ÉµÛË¾ ÊÐ²÷³Þ.
	 *
	 * @return  »ôµê¹ÉµÛ 
	 *
	 */
	const SString* GetData() const;
	
	/**
	 * @brief   »ôµê¹ÉµÛË¾ ÊÐ²÷³Þ. 
	 *
	 * @return  »ôµê¹ÉµÛ 
	 *
	 */
	SString* GetData();
	
	// Potentially growing the array
	/**
	 * @brief   ¹ÉµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ »¯ ÌººÏ¶¦ º¼¼³ÂÙ³Þ. 
	 *          Â­ÊýÂ×³Þ·² ¹ÉµÛË¾ »þºÂ»¤¿Ö³Þ.
	 *
	 * @param   nIndex     [in] : Ì¡¾· 
	 * @param   newElement [in] : »¯ ÌººÏ 
	 * @return  void 
	 *
	 * @see  SetAt(int nIndex, char* newElement)
	 *
	 */
	void SetAtGrow(int nIndex, const char* newElement);
	
	/**
	 * @brief   ¹ÉµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ »¯ ÌººÏ¶¦ ºªËÓÂÙ³Þ. 
	 *          Â­ÊýÂ×³Þ·² ¹ÉµÛË¾ »þºÂ»¤¿Ö³Þ. 
	 *
	 * @param   nIndex     [in] : Ì¡¾· 
	 * @param   newElement [in] : »¯ ÌººÏ 
	 * @return  void 
	 *
	 * @see   SetAt(int nIndex, SString& newElement)
	 *
	 */
	void SetAtGrow(int nIndex, SString& newElement);
	
	/**
	 * @brief   ¹ÉµÛËæ ¶ë¼è¶ìËæ ÌººÏ¶¦ ¾¡°¡ÂÙ³Þ. 
	 *
	 * @param   newElement [in] : ¾¡°¡Â×µØ²÷ ÌººÏ 
	 * @return  ¾¡°¡´ô Ì¡¾·¸ÜÂö 
	 *
	 */
	int Add(const char* newElement);
	
	/**
	 * @brief   ¹ÉµÛÌ© ¶ë¼è¶ìËæ ÌººÏ¶¦ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param   newElement [in] : ¾¡°¡Â×µØ²÷ ÌººÏ 
	 * @return  ¾¡°¡´ô Ì¡¾·¸ÜÂö 
	 *
	 */
	int Add(SString& newElement);
	
	/**
	 * @brief   ¹ÉµÛËæ »¯ ¹ÉµÛË¾ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param   src [in] : ¾¡°¡Â×µØ²÷ ¹ÉµÛ 
	 * @return  ¸õ¶· ¹ÉµÛÌ© ¿Í±¨ 
	 *
	 */
	int Append(const StringArray& src);

	/**
	 * @brief   ¹ÉµÛË¾ ¸ôº¡ÂÙ³Þ.
	 *
	 * @param   src [in] : Ìº½ç¹ÉµÛ 
	 * @return  void
	 *
	 */
	void Copy(const StringArray& src);
	
	// overloaded operator helpers
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·Ì© ÌººÏ¶¦ ÊÐ²÷³Þ.
	 *
	 * @param   nIndex [in] : ÊÐËºµØ²÷ ÌººÏÌ© Ì¡¾·¸ÜÂö
	 * @return  ÊÐÊÌ¼ê ÌººÏ 
	 *
	 */
	SString operator[](int nIndex) const;

	/**
	 * @brief   ¼è¼³´ô Ì¡¾·Ì© ÌººÏ¶¦ ÊÐ²÷³Þ.
	 *
	 * @param   nIndex [in] : ÊÐËºµØ²÷ ÌººÏÌ© Ì¡¾·¸ÜÂö 
	 * @return  ÌººÏÌ© ¼ÑºÏ 
	 *
	 */
	SString& operator[](int nIndex);
	
	// Operations that move elements around
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·µá¹¢À¾ ¼ÑÊÌ¼ê ±¶ºã¶í¿Ñ »¯ ÌººÏ¶¦ ºªËÓÂÙ³Þ.
	 *
	 * @param   nIndex     [in] : ºªËÓÂ×µØ²÷ Ì¡¾·¸ÜÂö
	 * @param   newElement [in] : »¯ ÌººÏ 
	 * @param   nCount     [in] : ±¶ºã 
	 * @return  void
	 *
	 */
	void InsertAt(int nIndex, char* newElement, int nCount = 1);
	
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·µá¹¢À¾ ¼ÑÊÌ¼ê ±¶ºã¶í¿Ñ »¯ ÌººÏ¶¦ ºªËÓÂÙ³Þ.
	 *
	 * @param   nIndex     [in] : ºªËÓÂ×µØ²÷ Ì¡¾·¸ÜÂö
	 * @param   newElement [in] : »¯ ÌººÏ 
	 * @param   nCount     [in] : ±¶ºã 
	 * @return  void
	 *
	 */
	void InsertAt(int nIndex, SString& newElement, int nCount = 1);
	
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·µá¹¢À¾ ¼ÑÊÌ¼ê ±¶ºã¶í¿Ñ ÌººÏ´ÉË¾ º¢½£ÂÙ³Þ.
	 *
	 * @param   nIndex [in] : Ì¡¾·¸ÜÂö 
	 * @param   nCount [in] : ±¶ºã 
	 * @return  void 
	 *
	 */
	void RemoveAt(int nIndex, int nCount = 1);

	/**
	 * @brief   ¹ÉµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ ³Þ¶¥ ¹ÉµÛË¾ ºªËÓÂÙ³Þ.
	 *
	 * @param   nStartIndex [in] : Ì¡¾·¸ÜÂö 
	 * @param   pNewArray   [in] : ºªËÓÂ×µØ²÷ ¹ÉµÛ 
	 * @return  void
	 *
	 */
	void InsertAt(int nStartIndex, StringArray* pNewArray);
	
	StringArray& operator=(const StringArray& t);
	// Implementation
protected:
	//! »¨¼è »ôµê¹ÉµÛ 
	SString* m_pData;  
	//! ¹ÉµÛÌ© ÊýºÏºã 
	int m_nSize;     
	//! ¾×´ÝÊýºÏºã 
	int m_nMaxSize;  
	//! ¹ÉµÛÌ© »þºÂºã 
	int m_nGrowBy;  
	
	void InsertEmpty(int nIndex, int nCount);
	
protected:
	// local typedefs for class templates
	typedef SString BASE_TYPE;
	typedef char* BASE_ARG_TYPE;
};

}

#endif //_STRINGARRAY_H_
