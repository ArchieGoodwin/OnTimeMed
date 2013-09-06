
/*!
 * \file    SString.h
 * \ingroup base
 * \brief   ·Í»ôµÛ¼¿»õË¾ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _CVLIB_STRING_H_
#define _CVLIB_STRING_H_

#include "cvlibbaseDef.h"
	
namespace CVLib
{
/**
 * @brief ·Í»ôµÛ»ôµê°é¼¿¾Ë
 */
struct StringData
{
	int nRefs;              /**< ½Ø¼¿±¶ºã */
	int nDataLength;        /**< ·Í»ôµÛ»ôµêÌ© ±¬ËË (¼Ç³àÁäÂÜ) */
	int nAllocLength;       /**< ÂÚ³ë´ô ±¨ÊÍ±¨±¬ËË */
	// char data[nAllocLength]

	char* data()           // char* to managed data
	{ return (char *)(this+1); }
};

/**
 * @brief ·Í»ôµÛ¼¿»õË¾ Ì¡ÂÙ ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC SString
{
public:
// Constructors
	//! ËÎ¸É°é¾¢»ô 
	SString();
	
	/**
	 * @brief   °é¾¢»ô(¸ôº¡)
	 *
	 * @param   stringSrc [in] : Ìº½çÊï¹¶½¤Àâ 
	 *
	 */
	SString(const SString& stringSrc);

	/**
	 * @brief   °é¾¢»ô(ÂÙ±¶ ·Í»ôµá ¼ÑÊÌ¼ê ±¬ËËÌ© ·Í»ôµÛË¾ °é¾¢ÂÙ³Þ.)
	 *
	 * @param   ch      [in] : ËÓµÙ·Í»ô
	 * @param   nLength [in] : ·Í»ôµÛÌ© ±¬ËË 
	 *
	 */
	SString(char ch, int nLength=1);

	/**
	 * @brief   °é¾¢»ô 
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * 
	 */
	SString(char * lpsz);
	
	/**
	 * @brief   °é¾¢»ô 
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ
	 *
	 */
	SString(const char * lpsz);

	/**
	 * @brief   °é¾¢»ô (ËÓµÙ·Í»ôµÛµá¹¢À¾ ¹¢¹¤·Í»ôµÛË¾ °é¾¢ÂÙ³Þ.)
	 *
	 * @param  lpch    [in] : ËÓµÙ·Í»ôµÛ
	 * @param  nLength [in] : ¹¢¹¤·Í»ôµÛ±¬ËË 
	 *
	 */
	SString(char * lpch, int nLength);
	
// Attributes & Operations
	/**
	 * @brief   ·Í»ôµÛÌ© ±¬ËË¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  ·Í»ôµÛÌ© ±¬ËË 
	 *
	 */
	int GetLength() const;
	
	/**
	 * @brief   ¹À ·Í»ôµÛËÍ°¡¶¦ °Áº¡ÂÙ³Þ.
	 *
	 * @return  ¹À ·Í»ôµÛËË·² ture, Ê­³£·² false
	 *
	 */
	DWORD IsEmpty() const;
	
	/**
	 * @brief   ·Í»ôµÛË¾ ¹À ·Í»ôµÛµá ½ó±¨ÃùÂÙ³Þ.
	 *
	 * @return  void
	 *
	 */
	void Empty();

	/**
	 * @brief   ·Í»ôµÛËæº· ¼è¼³´ô Ì¡¾·Ì© ·Í»ô¶¦ ÊÐ²÷³Þ.(0 - ±¨¼Ó)
	 *
	 * @param   nIndex [in] : Ì¡¾·¸ÜÂö 
	 * @return  ÊÐÊÌ¼ê ·Í»ô 
	 *
	 * @see   SetAt()
	 *
	 */
	BYTE GetAt(int nIndex) const;

	/**
	 * @brief   ·Í»ôµÛËæº· ¼è¼³´ô Ì¡¾·Ì© ·Í»ô¶¦ ÊÐ²÷³Þ.(0 - ±¨¼Ó)
	 *
	 * @param   nIndex [in] : Ì¡¾·¸ÜÂö 
	 * @return  ÊÐÊÌ¼ê ·Í»ô 
	 *
	 * @see   GetAt(), SetAt()
	 *
	 */
	BYTE operator[](int nIndex) const;

	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ ÂÙ ·Í»ô¶¦ ºªËÓÂÙ³Þ.
	 *
	 * @param   nIndex [in] : Ì¡¾·¸ÜÂö 
	 * @param   ch     [in] : ËÓµÙ·Í»ô 
	 * @return  void
	 *
	 * @see  GetAt()
	 *
	 */
	void SetAt(int nIndex, char ch);

	// overloaded assignment
	/**
	 * @brief   ·Í»ôµÛ»ôµêÌ© ¸ôº¡¶¦ ¼êÃÔÂÙ³Þ.
	 *
	 * @param   stringSrc [in] : Ìº½çÊï¹¶½¤Àâ 
	 * @return  ¸ôº¡´ô Êï¹¶½¤Àâ   
	 *
	 * @see  SString(SString & stringSrc)
	 *
	 */
	const SString& operator=(const SString & stringSrc);

	/**
	 * @brief   ·Í»ôµÛ»ôµê¶¦ ¸ôº¡ÂÙ³Þ.
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  ¸ôº¡´ô »ôµê 
	 *
	 * @see   SString(char * lpsz)
	 *
	 */
	const SString& operator=(const char* lpsz);

	/**
	 * @brief   ÂÙ ·Í»ôµá ´ô ·Í»ôµÛ»ôµê¶¦ °é¾¢ÂÙ³Þ.
	 *
	 * @param   ch [in] : ËÓµÙ·Í»ô 
	 * @return  ·Í»ôµÛ»ôµê 
	 *
	 * @see   SString(char ch, int nLength)
	 *
	 */
	const SString& operator=(char ch);

	/**
	 * @brief   ·Í»ôµÛË¾ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param   string [in] : ËÓµÙ·Í»ôµÛ»ôµê ¡¡
	 * @return  ¾¡°¡´ô ·Í»ôµÛ»ôµê 
	 *
	 */
	const SString& operator+=(const SString& string);

	/**
	 * @brief   ·Í»ôµÛËæ ÂÙ ·Í»ô¶¦ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param   ch [in] : ËÓµÙ·Í»ô 
	 * @return  ¾¡°¡´ô ·Í»ôµÛ»ôµê
	 *
	 */
	const SString& operator+=(char ch);

	/**
	 * @brief   ·Í»ôµÛË¾ ¾¡°¡ÂÙ³Þ.
	 *
	 * @param  lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return ¾¡°¡´ô ·Í»ôµÛ»ôµê 
	 *
	 */
	const SString& operator+=(const char* lpsz);

	/**
	 * @brief   ·Í»ôµÛË¾ ÊÐ²÷³Þ.
	 *
	 * @return  ·Í»ôµÛ¼è¼¬»ô 
	 *
     */
	const char * GetLPCTSTR() const {return m_pchData;}

	/**
	 * @brief   ·Í»ôµÛË¾ ÊÐ²÷³Þ.
	 *
	 * @return  ·Í»ôµÛ¼è¼¬»ô 
	 *
	 */
	operator const char*() const
	{
		return m_pchData;
	}

	// string comparison
	/**
	 * @brief   ·Í»ôµÛÌ© ¹¾°ä¶¦ ¼êÃÔÂÙ³Þ.
	 *
	 * @param   pStr [in] : ·Í»ôµÛ»ôµê 
	 * @return  ´¸ ·Í»ôµÛËË °¯Ëº·² 0, ËÓµÙ·Í»ôµÛ¸ó³Þ »õËº·² <0, ËÓµÙ·Í»ôµÛ¸ó³Þ ¿Í·² >0
	 *
	 */
	int Compare(const SString& pStr) const;

	// straight character comparison
	/**
	 * @brief   ·Í»ôµÛÌ© ¹¾°ä¶¦ ¼êÃÔÂÙ³Þ. 
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  ´¸ ·Í»ôµÛËË °¯Ëº·² 0, ËÓµÙ·Í»ôµÛ¸ó³Þ »õËº·² <0, ËÓµÙ·Í»ôµÛ¸ó³Þ ¿Í·² >0
	 *
	 */
	int Compare(const char * lpsz) const;

	// compare ignoring case
	/**
	 * @brief   ·Í»ôµÛÌ© ¹¾°ä¶¦ ¼êÃÔÂÙ³Þ. 
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  ´¸ ·Í»ôµÛËË °¯Ëº·² 0, ËÓµÙ·Í»ôµÛ¸ó³Þ »õËº·² <0, ËÓµÙ·Í»ôµÛ¸ó³Þ ¿Í·² >0
	 * 
	 */
	int CompareNoCase(const char * lpsz) const;
	
	// NLS aware comparison, case sensitive
	/**
	 * @brief   ·Í»ôµÛÌ© ¹¾°ä¶¦ ¼êÃÔÂÙ³Þ.
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  ´¸ ·Í»ôµÛËË °¯Ëº·² 0, ËÓµÙ·Í»ôµÛ¸ó³Þ »õËº·² <0, ËÓµÙ·Í»ôµÛ¸ó³Þ ¿Í·² >0
	 *
	 * @see   Compare()
	 *
	 */
	int Collate(char * lpsz) const;

	// NLS aware comparison, case insensitive
	/**
	 * @brief   ·Í»ôµÛÌ© ¹¾°ä¶¦ ¼êÃÔÂÙ³Þ.
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  Compare(char * lpsz)
	 *
	 */
	int CollateNoCase(char * lpsz) const;

	// simple sub-string extraction

  /******************************************************/
  //MD_906 by pch
	/**
	 * @brief   ¼è¼³´ô Ì¡¾·µá¹¢À¾ ¼è¼³´ô ±¬ËË¶í¿Ñ ¹¢¹¤·Í»ôµÛË¾ ¸ôº¡ÂÙ³Þ.
	 *
	 * @param   nFirst [in] : »¤»õÌ¡¾· 
	 * @param   nCount [in] : ±¬ËË 
	 * @return  ¹¢¹¤·Í»ôµÛ 
	 *
	 */
	SString Mid(int nFirst, int nCount) const;

	/**
	 * @brief   ¼è¼³´ô Ì¡¾·µá¹¢À¾ ·Í»ôµÛÌ© ÄùÄ´¼è ¹¢¹¤·Í»ôµÛË¾ ¸ôº¡ÂÙ³Þ.
	 *
	 * @param   nFirst [in] : »¤»õÌ¡¾· 
	 * @return  ¹¢¹¤·Í»ôµÛ 
	 *
	 * @see   Mid(int nFirst, int nCount)
	 *
	 */
	SString Mid(int nFirst) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ±¬ËËÄ´¼èÌ© ¹¢¹¤·Í»ôµÛË¾ ´®¶°³Þ.
	 *
	 * @param   nCount [in] : ¹¢¹¤·Í»ôµÛÌ© ±¬ËË 
	 * @return  ¹¢¹¤·Í»ôµÛ 
	 *
	 * @see   Right()
	 *
	 */
	SString Left(int nCount) const;

	/**
	 * @brief   ·Í»ôµÛÌ© ÄùËæº·¹¢À¾ ¼è¼³´ô ±¬ËËÄ´¼èÌ© ¹¢¹¤·Í»ôµÛË¾ ´®¶°³Þ. 
	 *
	 * @param   nCount [in] : ¹¢¹¤·Í»ôµÛÌ© ±¬ËË 
	 * @return  ¹¢¹¤·Í»ôµÛ 
	 *
	 * @see  Left()
	 *
	 */
	SString Right(int nCount) const;
  /******************************************************/

	/**
	 * @brief   ·Í»ôµÛË¾ ¼è¼³´ô Âô»¥Ëæ ¶÷¾¡ÊÌ Èº³Þ.
	 *
	 * @param   lpszFormat [in] : 
	 * @return  void
	 *
	 */
	void Format(char * lpszFormat, ...);
	
	/**
	 * @brief   ·Í»ôµÛË¾ ¼è¼³´ô Âô»¥Ëæ ¶÷¾¡ÊÌ Èº³Þ. 
	 *
	 * @param   lpszFormat [in] : 
	 * @param   argList    [in] :
	 * @return  void
	 *
	 */
	void FormatV(char * lpszFormat, va_list argList);
  
  /******************************************************/
  //MD_906 by pch
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ lpszCharSetÊ¯Ì© ·º´Ç ·Í»ô¶¦ ÁäÂÜÂ×²÷ 
	 *          ¹¢¹¤·Í»ôµÛË¾ ÊÐ²÷³Þ. 
	 *
	 * @param   lpszCharSet [in] : ·Í»ô·ºËÒ 
	 * @return  ÊÐÊÌ¼ê ¹¢¹¤·Í»ôµÛ 
	 *
	 * @see  SpanExcluding()
	 *
	 */
	SString SpanIncluding(const char * lpszCharSet) const;

	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ lpszCharSetÊ¯Ì© ·Í»ô°¡ Ê­³¥ 
     *          ·Í»ô´ÉË¾ ÁäÂÜÂ×²÷ ¹¢¹¤·Í»ôµÛË¾ ÊÐ²÷³Þ. 
	 *
	 * @param   lpszCharSet [in] : ·Í»ô·ºËÒ 
	 * @return  ÊÐÊÌ¼ê ¹¢¹¤·Í»ôµÛ 
	 *
	 * @see   SpanIncluding()
	 *
	 */
	SString SpanExcluding(const char * lpszCharSet) const;
  /******************************************************/
  
	// upper/lower/reverse conversion
	/**
	 * @brief   ·Í»ôµÛÊ¯Ì© ·º´Ç ·Í»ô´ÉË¾ ´Ý·Í»ôµá ¸êÃûÂÙ³Þ. 
	 *
	 * @return  ¸êÃû´ô ·Í»ôµÛ 
	 *
	 * @see   MakeLower()
	 *
	 */
	void MakeUpper();

	/**
	 * @brief   ·Í»ôµÛÊ¯Ëº ·º´Ç ·Í»ô´ÉË¾ ºÏ·Í»ôµá ¸êÃûÂÙ³Þ. 
	 *
	 * @return  ¸êÃû´ô ·Í»ôµÛ 
	 *
	 * @see  MakeUpper()
	 *
	 */
	void MakeLower();

	/**
	 * @brief   ·Í»ôµÛË¾ ´û¼ï²÷³Þ.
	 *
	 * @return  void
	 *
	 */
	void MakeReverse();

	// trimming whitespace (either side)
	/**
	 * @brief   ·Í»ôµÛÌ© ÄùËæº·¹¢À¾ °á¹ÊË¾ ½£°ºÂÙ³Þ.
	 *
	 * @return  void 
	 *
	 * @see   TrimLeft()
	 *
	 */
	void TrimRight();

	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ °á¹ÊË¾ ½£°ºÂÙ³Þ.
	 *
	 * @return  void
	 *
	 * @see   TrimRight()
	 *
	 */
	void TrimLeft();

	// trimming anything (either side)
	/**
	 * @brief   ·Í»ôµÛÌ© ÄùËæº·¹¢À¾ ¼è¼³´ô ·Í»ô¶¦ ½£°ºÂÙ³Þ. 
	 *
	 * @param   chTarget [in] : ½£°ºÂ×µØ²÷ ·Í»ô 
	 * @return  void 
	 *
	 */
	void TrimRight(char chTarget);

	/**
	 * @brief   ·Í»ôµÛÌ© ÄùËæº·¹¢À¾ ¼è¼³´ô ·Í»ôµÛË¾ ½£°ºÂÙ³Þ. 
	 *
	 * @param   lpszTargets [in] : ½£°ºÂ×µØ²÷ ·Í»ôµÛ 
	 * @return  void 
	 *
	 */
	void TrimRight(char * lpszTargets);
	
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ·Í»ô¶¦ ½£°ºÂÙ³Þ. 
	 *
	 * @param   chTarget [in] : ½£°ºÂ×µØ²÷ ·Í»ô 
	 * @return  void
	 *
	 */
	void TrimLeft(char chTarget);
	
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ·Í»ôµÛË¾ ½£°ºÂÙ³Þ.
	 *
	 * @param   lpszTargets [in] : ½£°ºÂ×µØ²÷ ·Í»ôµÛ 
	 * @return  void 
	 *
	 */
	void TrimLeft(char * lpszTargets);

	// advanced manipulation
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô ·Í»ô¶¦ »¯µáË© ·Í»ôµá °ä¾ËÂÙ³Þ. 
	 *
	 * @param   chOld [in] : °ä¾ËÂ×µØ²÷ ·Í»ô 
	 * @param   chNew [in] : »¯ ·Í»ô 
	 * @return  °ä¾ËÂÙ ±¶ºã 
	 *
	 */
	int Replace(char chOld, char chNew);
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô ¹¢¹¤·Í»ôµÛË¾ »¯µáË© ·Í»ôµÛµá °ä¾ËÂÙ³Þ. 
	 *
	 * @param   lpszOld [in] : °ä¾ËÂ×µØ²÷ ·Í»ôµÛ 
	 * @param   lpszNew [in] : »¯ ·Í»ôµÛ 
	 * @return  °ä¾ËÂÙ ±¶ºã 
	 *
	 */
	int Replace(char * lpszOld, char * lpszNew);
	
	/**
	 * @brief   ·Í»ôµÛË¾ »¯µáË© ·Í»ôµÛµá °ä¾ËÂÙ³Þ. 
	 *
	 * @param   Old [in] : °ä¾ËÂ×µØ²÷ ·Í»ôµÛ 
	 * @param   New [in] : »¯ ·Í»ôµÛ 
	 * @return  °ä¾ËÂÙ ±¶ºã 
	 *
	 */
	int Replace(SString Old, SString New);
	
	/**
	 * @brief   ·Í»ôµÛËæº· ¼è¼³´ô ·Í»ô¶¦ ½£°ºÂÙ³Þ. 
	 *
	 * @param   chRemove [in] : ½£°ºÂ×µØ²÷ ·Í»ô 
	 * @return  ½£°ºÂÙ ±¶ºã 
	 *
	 * @see   Insert()
	 *
	 */
	int Remove(char chRemove);

	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ ¼è¼³´ô ·Í»ô¶¦ ºªËÓÂÙ³Þ. 
	 *
	 * @param   nIndex [in] : Ì¡¾· 
	 * @param   ch     [in] : ºªËÓÂ×µØ²÷ ·Í»ô 
	 * @return  »¯ ·Í»ôµÛÌ© ±¬ËË 
	 *
	 * @see   Remove()
	 *
	 */
	int Insert(int nIndex, char ch);

	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·Ëæ ·Í»ôµÛË¾ ºªËÓÂÙ³Þ. 
	 *
	 * @param   nIndex [in] : Ì¡¾· 
	 * @param   pstr   [in] : ºªËÓÂ×µØ²÷ ·Í»ôµÛ 
	 * @return  »¯ ·Í»ôµÛÌ© ±¬ËË 
	 *
	 */
	int Insert(int nIndex, char * pstr);
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ ¼ÑÊÌ¼ê ±¶ºã¶íÂÙ ·Í»ô¶¦ º¢½£ÂÙ³Þ. 
	 *
	 * @param   nIndex [in] : Ì¡¾· 
	 * @param   nCount [in] : º¢½£Â×µØ²÷ ±¶ºã 
	 * @return  ·Í»ôµÛÌ© ¸õ¶·±¬ËË 
	 *
	 */
	int Delete(int nIndex, int nCount = 1);

	// searching
	/**
	 * @brief   ·Í»ôµÛÌ© ÄùËæº·¹¢À¾ ¼è¼³´ô ·Í»ô¶¦ ½Ü²÷³Þ. 
	 *
	 * @param   ch [in] : ½ÜËºµØ²÷ ·Í»ô 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int ReverseFind(char ch) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ ËùÉ¸Ëºµá °¡·²º· ¼è¼³´ô ·Í»ôµÛË¾ ½Ü²÷³Þ. 
	 *
	 * @param   lpszSub    [in] : ½ÜËºµØ²÷ ·Í»ôµÛ 
	 * @param   nBackStart [in] : »¤»õÌ¡¾· 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int ReverseFind(char* lpszSub, int nBackStart = 0) const;

	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ Êï¶¥É¸Ëºµá °¡·²º· ¼è¼ñ´ô ·Í»ôµÛË¾ ½Ü²÷³Þ. 
	 *
	 * @param   pStr   [in] : ½ÜËºµØ²÷ ·Í»ôµÛ 
	 * @param   nStart [in] : »¤»õÌ¡¾· 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(SString *pStr, int nStart) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ Êï¶¥É¸Ëºµá °¡·²º· ¼è¼³´ô ·Í»ôµÛË¾ ½Ü²÷³Þ. 
	 *
	 * @param   Str    [in] : ½ÜËºµØ²÷ ·Í»ôµÛ 
	 * @param   nStart [in] : »¤»õÌ¡¾· 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(SString Str, int nStart) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ·Í»ôµÛË¾ ½Ü²÷³Þ. 
	 *
	 * @param   Str [in] : ½ÜËºµØ²÷ ·Í»ùµØ 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(SString Str) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ·Í»ôµÛË¾ ½Ü²÷³Þ. 
	 *
	 * @param   pStr [in] : ½ÜËºµØ²÷ ·Í»ôµÛ 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(SString *pStr) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ·Í»ô¶¦ ½Ü²÷³Þ. 
	 *
	 * @param   ch [in] : ½ÜËºµØ²÷ ·Í»ô 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(char ch) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ Êï¶¥É¸Ëºµá °¡·²º· ¼è¼³´ô ·Í»ô¶¦ ½Ü²÷³Þ. 
	 *
	 * @param   ch     [in] : ½ÜËºµØ²÷ ·Í»ô 
	 * @param   nStart [in] : »¤»õÌ¡¾· 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(char ch, int nStart) const;

	/**
	 * @brief   ·Í»ôµÛÌ© »¤»õËºµá¹¢À¾ ¼è¼³´ô ·Í»ôµÛË¾ ½Ü²÷³Þ.
	 *
	 * @param   lpszSub [in] : ½ÜËºµØ²÷ ·Í»ôµÛ 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(char*lpszSub) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ Êï¶¥É¸Ëºµá °¡·²º· ¼è¼³´ô ·Í»ôµÛË¾ ½Ü²÷³Þ. 
	 *
	 * @param   lpszSub [in] : ½ÜËºµØ²÷ ·Í»ôµÛ 
	 * @param   nStart  [in] : »¤»õÌ¡¾· 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int Find(char*lpszSub, int nStart) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ ¼è¼³´ô ·Í»ô·ºËÒÊ¯Ì© ·Í»ô´ÉË¾ ½Ü²÷³Þ. 
	 *
	 * @param   lpszCharSet [in] : ·Í»ô·ºËÒ 
	 * @param   nStart      [in] : »¤»õÌ¡¾· 
	 * @return  ½åËÁ ½ÜË¼ Ì¡¾· 
	 *
	 */
	int FindOneOf(const char * lpszCharSet, int nStart = 0) const;
	
	/**
	 * @brief   ·Í»ôµÛÌ© ¼è¼³´ô Ì¡¾·µá¹¢À¾ ½ÜË¼ ¹¢¹¤·Í»ôµÛÌ© ±¶ºã¶¦ ÊÐ²÷³Þ. 
	 *
	 * @param   lpszSub [in] : ½ÜËºµØ²÷ ¹¢¹¤·Í»ôµÛ 
	 * @param   nStart  [in] : »¤»õÌ¡¾· 
	 * @return  ½ÜË¼ ±¶ºã  
	 *
	 */
	int GetCharCount(char *lpszSub,int nStart = 0) const;

	// get pointer to modifiable buffer at least as int as nMinBufLength
	/**
	 * @brief   ¾×ºÏÂÙ ¼ÑÊÌ¼ê ±¬ËËµá ¸ê°Ò´ô ·Í»ôµÛÌ°¾©±¨Ì© ¼ÑºÏ¶¦ ÊÐ²÷³Þ.    
	 *
	 * @param   nMinBufLength [in] : ·Í»ôµÛ±¬ËË 
	 * @return  ·Í»ôµÛÌ°¾©±¨¼ÑºÏ 
	 *
	 */
	char * GetBuffer(int nMinBufLength);
	
	/**
	 * @brief   ·Í»ôµÛÌ°¾©±¨¶¦ ÃÍ¸ÒÂ×°Ö ·Í»ôµÛÌ© ±¬ËË¶¦ »¯µá º¼¼³ÂÙ³Þ.
	 *
	 * @param   nNewLength [in] : º¼¼³Â×µØ²÷ ±¬ËË 
	 * @return  void
	 *
	 * @see   FreeExtra()
	 *
	 */
	void ReleaseBuffer(int nNewLength = -1);

	// get pointer to modifiable buffer exactly as int as nNewLength
	/**
	 * @brief   ¼ÑÊÌ¼ê ±¬ËËµá ¸ê°Ò´ô ·Í»ôµÛÌ°¾©±¨Ì© ¼ÑºÏ¶¦ ÊÐ²÷³Þ.
	 *
	 * @param   nNewLength [in] : ·Í»ôµÛ±¬ËË 
	 * @return  ·Í»ôµÛÌ°¾©±¨¼ÑºÏ 
	 *
	 * @see   GetBuffer()
	 *
	 */
	char * GetBufferSetLength(int nNewLength);
	// release memory allocated to but unused by string
	/**
	 * @brief    ¹À Ì°¾©±¨°éÊßË¾ ÃÍ¸ÒÂÙ³Þ.
	 *
	 * @return   void
	 *
	 * @see   ReleaseBuffer()
	 *
	 */
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off

	// turn refcounting back on
	/**
	 * @brief   nRef = -1µá º¼¼³ÂÙ³Þ.
	 *
	 * @return  ·Í»ôµÛ¼è¼¬»ô  
	 *
	 */
	char * LockBuffer();
	// turn refcounting off
	/**
	 * @brief   nRef = -1µá º¼¼³ÂÙ³Þ.
	 *
	 * @return   void
	 *
	 */
	void UnlockBuffer();
	int FromFile(FILE* pfile);
	int ToFile(FILE* pfile);

// Implementation
public:
	//! ÃÍ¾Ë»ô 
	~SString();

	/**
	 * @brief   ÂÚ³ë´ô ·Í»ôµÛÌ°¾©±¨¿Í±¨¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  Ì°¾©±¨¿Í±¨ 
	 *
	 */
	int         GetAllocLength() ;

	/**
	 * @brief   ·Í»ôµÛ»ôµê¶¦ ÊÐ²÷³Þ. 
	 *
	 * @return  ·Í»ôµÛ»ôµê¼è¼¬»ô 
	 *
	 */
	StringData* GetData() const;

	/**
	 * @brief   ¼ÑÊÌ¼ê ´¸ ·Í»ôµÛË¾ ·Í»ôµÛÌ°¾©±¨Ëæ µÚ°Î¸ôº¡ÂÙ³Þ. 
	 *
	 * @param   nSrc1Len     [in] : ½ë ·Í»ôµÛÌ© ±¬ËË 
	 * @param   lpszSrc1Data [in] : ½ë ·Í»ôµÛ¼ÑºÏ 
	 * @param   nSrc2Len     [in] : ´¸¸ÜÉã ·Í»ôµÛÌ© ±¬ËË 
	 * @param   lpszSrc2Data [in] : ´¸¸ÜÉã ·Í»ôµÛ¼ÑºÏ 
	 * @return  void 
	 *
	 */
	void        ConcatCopy(int nSrc1Len, const char * lpszSrc1Data, int nSrc2Len, const char * lpszSrc2Data);

	/**
	 * @brief   ·Í»ôµÛÌ© ±¬ËË¶¦ ÊÐ²÷³Þ. 
	 *
	 * @param   lpsz [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  ·Í»ôµÛÌ© ±¬ËË 
	 *
	 */
	int         SafeStrlen(const char * lpsz) const;

	//!  ·Í»ôµÛ¼è¼¬»ô 
	char*       m_pchData;   

protected:

	// implementation helpers

	void Init();
	void AllocCopy(SString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, const char * lpszSrcData);

	void ConcatInPlace(int nSrcLen, const char * lpszSrcData);
	void CopyBeforeWrite();
	void AllocBeforeWrite(int nLen);
	void Release();
	void Release(StringData* pData);

	void  FreeData(StringData* pData);

};

// #ifdef VC_EXTRALEAN
// inline SString GetString(SString Str)
// {
// 	return (char*)const char*(Str);
// }
// #endif

inline DWORD CVLIB_DECLSPEC operator==( SString& s1,  SString& s2)
	{ return s1.Compare(s2) == 0; }
inline DWORD CVLIB_DECLSPEC operator==( SString& s1, char * s2)
	{ return s1.Compare(s2) == 0; }
inline DWORD CVLIB_DECLSPEC operator==(char * s1,  SString& s2)
	{ return s2.Compare(s1) == 0; }
inline DWORD CVLIB_DECLSPEC operator!=( SString& s1,  SString& s2)
	{ return s1.Compare(s2) != 0; }
inline DWORD CVLIB_DECLSPEC operator!=( SString& s1, char * s2)

	{ return s1.Compare(s2) != 0; }
inline DWORD CVLIB_DECLSPEC operator!=(char * s1,  SString& s2)
	{ return s2.Compare(s1) != 0; }

inline DWORD CVLIB_DECLSPEC operator<( SString& s1,  SString& s2)
	{ return s1.Compare(s2) < 0; }
inline DWORD CVLIB_DECLSPEC operator<( SString& s1, char * s2)
	{ return s1.Compare(s2) < 0; }
inline DWORD CVLIB_DECLSPEC operator<(char * s1,  SString& s2)
	{ return s2.Compare(s1) > 0; }
inline DWORD CVLIB_DECLSPEC operator>( SString& s1,  SString& s2)
	{ return s1.Compare(s2) > 0; }
inline DWORD CVLIB_DECLSPEC operator>( SString& s1, char * s2)
	{ return s1.Compare(s2) > 0; }
inline DWORD CVLIB_DECLSPEC operator>(char * s1,  SString& s2)
	{ return s2.Compare(s1) < 0; }
inline DWORD CVLIB_DECLSPEC operator<=( SString& s1,  SString& s2)
	{ return s1.Compare(s2) <= 0; }
inline DWORD CVLIB_DECLSPEC operator<=( SString& s1, char * s2)
	{ return s1.Compare(s2) <= 0; }

inline DWORD CVLIB_DECLSPEC operator<=(char * s1,  SString& s2)
	{ return s2.Compare(s1) >= 0; }
inline DWORD CVLIB_DECLSPEC operator>=( SString& s1,  SString& s2)
	{ return s1.Compare(s2) >= 0; }

inline DWORD CVLIB_DECLSPEC operator>=( SString& s1, char * s2)
	{ return s1.Compare(s2) >= 0; }
inline DWORD CVLIB_DECLSPEC operator>=(char * s1,  SString& s2)
	{ return s2.Compare(s1) <= 0; }

#define IsTCharCode(c) ((((BYTE)(c)) > 0x7F) ? (TRUE) : (FALSE))
#define IsSpaceCode(c) (((c) == 0x20 || (c) == 0xA1A1) ? (TRUE) : (FALSE))
	
/*increase the pointer to current string, appositing the multibyte string*/
CVLIB_DECLSPEC inline BYTE* cbsinc(const BYTE * current)
{
  BYTE * rslt;
  rslt = (BYTE *)current;
  if(*rslt > 0x80)
    rslt += 2;
  else
    rslt++;
  return rslt;
}

CVLIB_DECLSPEC inline BYTE* cbsstr(const BYTE *string1, const BYTE *string2)
{
	BYTE * pszFind = NULL ;

	if(string1 == NULL || string2 == NULL)
		return pszFind ;
	
	int nLen1 = (int)strlen((const char*)string1) ;
	int nLen2 = (int)strlen((const char*)string2) ;
	if(nLen1 == 0 || nLen2 == 0)
		return pszFind ;

	int nIdx1 = 0 ;
	int nIdx2 = 0 ;
	while( nIdx1 < nLen1 - nLen2 + 1 )
	{
		nIdx2 = 0 ;
		while( nIdx2 < nLen2 && string2[nIdx2] == string1[nIdx1 + nIdx2] )
		{
			nIdx2++ ;
		}
		if( nIdx2 >= nLen2 )
		{
			pszFind = (BYTE*)(string1 + nIdx1);
			break ;
		}
		if(IsTCharCode(string1[nIdx1]))
			nIdx1 += 2;
		else
			nIdx1++ ;
	}
	
	return pszFind ;
}

CVLIB_DECLSPEC inline BYTE* cbschr(const BYTE *strSrc, int nCode)
{
	BYTE * pszFind = NULL ;
	int nLen = (int)strlen((const char*)strSrc) ;
	int nIdx = 0 ;
	while( nIdx < nLen )
	{
		int nSrcCode = strSrc[nIdx] ;
		if(IsTCharCode(strSrc[nIdx]))
			nSrcCode = (nSrcCode << 8) + (strSrc[nIdx+1] & 0xFF) ;

		if(nCode == nSrcCode)
		{
			pszFind = (BYTE*)(strSrc + nIdx);
			break ;
		}

		if(IsTCharCode(strSrc[nIdx]))
			nIdx += 2;
		else
			nIdx++ ;
	}
	
	return pszFind ;
}

// Compare helpers
bool CVLIB_DECLSPEC operator==(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator==(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator==(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator!=(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator!=(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator!=(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator<(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator<(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator<(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator>(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator>(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator>(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator<=(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator<=(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator<=(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator>=(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator>=(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator>=(const char* s1, const SString& s2);
SString CVLIB_DECLSPEC operator+(const SString& string1, const SString& string2);
SString CVLIB_DECLSPEC operator+(const SString& string, char ch);
SString CVLIB_DECLSPEC operator+(char ch, const SString& string);
SString CVLIB_DECLSPEC operator+(const SString& string, const char* lpsz);
SString CVLIB_DECLSPEC operator+(const char* lpsz, const SString& string);

}

#endif //_CVLIB_STRING_H_
