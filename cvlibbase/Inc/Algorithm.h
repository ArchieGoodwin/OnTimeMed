/*!
 * \file	Algorithm.h
 * \ingroup base
 * \brief	·º´Ç Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "PtrArray.h"
#include "SString.h"
#include "CoImage.h"

namespace CVLib
{

/**
 * @brief  IDÌ© ¾×´Ý±¬ËË
 */
#define CVLIB_IDLEN			256

/**
 * @brief  Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC Algorithm : public Object
{
public:
	//!	°é¾¢»ô
	Algorithm();
	//!	ÃÍ¾Ë»ô
	virtual ~Algorithm();

	/** 
	 * @brief   Ê²°Ö¶®´ÍË¾ ¾¡°¡Â×²÷ ÂÜºãËË³Þ.
	 *
	 * @param 	pAlgorithm [in] : ¾¡°¡Â×µØ²÷ Ê²°Ö¶®´Í
	 * @return 	Ê²°Ö¶®´ÍÌ© ¸ÜÂö
	 *
	 * @see  DelAlgorithm(), RemoveAlgorithms()
	 *
	 * @par Example:
	 * @code
	 *	...
	 *  int nCount = 0;
	 *	
	 *  Algorithm total;
	 *  Algorithm sub1;
	 *  Algorithm sub2;
	 *  Algorithm sub3;
	 *  ...
	 *  total.AddAlgorithm(&sub1);
	 *  total.AddAlgorithm(&sub2);
	 *
	 *  total.SetAlgorithm(2, &sub3);  
	 *  nCount = total.GetAlgorithmCount();
	 *  assert (nCount == 3);
	 * 
	 *  temp = total.GetAlgorithm(2);
	 *  assert(temp != Null);
	 *
	 *  total.DelAlgorithm(2);
	 *  assert (nCount == 2);
	 *
	 *  total.RemoveAlgorithms();
	 *  assert (nCount == 0);
	 *  ...
	 * @endcode
	 *
	 */
	int AddAlgorithm (Algorithm* pAlgorithm);

	/** 
	 * @brief	Ê²°Ö¶®´ÍË¾ ÊÐ±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 * @param	nIdx [in] : ÊÐËºµØ²÷ Ê²°Ö¶®´ÍÌ© ¸ÜÂö
	 * @return	¸ÜÂöËæ Åà¶£²÷ Ê²°Ö¶®´Í
	 *
	 * @see  AddAlgorithm()
	 *
     */
	Algorithm* GetAlgorithm (int nIdx) const;

	/**
	 * @brief   Ê²°Ö¶®´ÍË¾ º¼¼³Â×±¨ Ì¡ÂÙ ÂÜºãËË³Þ. 
	 *
	 * @param   nIdx       [in] : º¼¼³Â×µØ²÷ Ê²°Ö¶®´ÍÌ© ¸ÜÂö 
	 * @param   pAlgorithm [in] : º¼¼³Â×µØ²÷ Ê²°Ö¶®´Í 
	 * @return  void 
	 *
	 * @see  AddAlgorithm()
	 *
	 */
	void SetAlgorithm (int nIdx, Algorithm* pAlgorithm);

	/** 
	 * @brief	Ê²°Ö¶®´ÍÌ© ±¶ºã¶¦ ÊÐ±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 * 
	 * @return	Ê²°Ö¶®´ÍÌ© ±¶ºã
	 *
	 * @see  AddAlgorithm(),  GetAlgorithm()
	 *
	 */
	int GetAlgorithmCount() const;

	/** 
	 * @brief	Ê²°Ö¶®´ÍË¾ ·º´¸ º¢½£Â×±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 * 
	 * @return	void
	 *
	 * @see  AddAlgorithm(),  DelAlgorithm()
	 *
	 */
	void RemoveAlgorithms ();
	
	/** 
	 * @brief	Ê²°Ö¶®´ÍË¾ º¢½£Â×±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 *
	 * @param	nIdx [in] : º¢½£Â×µØ²÷ Ê²°Ö¶®´ÍÌ© ¸ÜÂö
	 * @return	void
	 *
	 * @see  AddAlgorithm()
	 *
	 */
	void DelAlgorithm(int nIdx);

	/** 
	 * @brief	Ê²°Ö¶®´ÍÌ© ID¶¦ ÊÐ±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 * 
	 * @return	Ê²°Ö¶®´Í ID
	 *
	 * @see  SetSelfID(), GetSelfID() 
	 *
	 */
	virtual const char* GetID ();

	/** 
	 * @brief	»ô±¨ ID¶¦ º¼¼³Â×±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 *
	 * @param	szID [in] : Ê²°Ö¶®´Í ID
	 * @return	void
	 *
	 * @see  AddAlgorithm(), GetSelfID()
	 *
	 * @par Example:
	 * @code
	 *	...
	 *	char* szTotalID = NULL;
	 *  char* szSubID1 = NULL; 
	 *	
	 *  Algorithm total;
	 *  Algorithm sub1;
	 *  Algorithm sub2;
	 *  
	 *	sub1.SetSelfID("sub1");
	 *	sub2.SetSelfID("sub2");
	 *  total.SetSelfID("total");
	 *  total.AddAlgorithm(&sub1);
	 *  total.AddAlgorithm(&sub2);
     *
	 *	szTotalID = total.GetID();
	 *	assert (!strcmp(szTotalID, "total[sub1][sub2]"));
	 *
	 *	szSubID1 = sub1.GetSelfID();
	 *	assert (!strcmp(szSubID1, "sub1"));
	 *  ...
	 * @endcode
	 *
	 */
	void SetSelfID (const char* szID);

	/** 
	 * @brief	»ô±¨Ì© Ê²°Ö¶®´Í ID¶¦ ÊÐ±¨ Ì¡ÂÙ ÂÜºãËË³Þ.
	 * 
	 * @return	Ê²°Ö¶®´Í ID
	 *
	 * @see  SetSelfID()
	 *
	 */
	const char* GetSelfID() const;
protected:
	//! »ô¾ËID
	char m_szID[CVLIB_IDLEN];
	//! ¼­¾ËID
	char m_szTotalID[CVLIB_IDLEN];
	//! Ê²°Ö¶®´ÍÌ© ¹ÉµÛ
	PtrArray	m_AlgorithmArray;
};

}

#endif //_ALGORITHM_H_