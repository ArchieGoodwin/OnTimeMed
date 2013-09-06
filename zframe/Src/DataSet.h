
/*!

 * \file    DataSet.h
 * \ingroup	base
 * \brief	»ôµê¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#if !defined (_DATASET_H_)
#define _DATASET_H_

#include "cvlibbase/inc/Mat.h"
#include "cvlibbase/inc/XFile.h"
#include <stdio.h>

namespace CVLib
{

/**
 * @brief  »ôµê·ºËÒ¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC DataSet : public Object
{
public:
	enum 
	{
		DS_ALL, 
		DS_ADDRESS
	};
public:
	//! ÃªµÚ»ôµêÌ© ºã 
	int		m_nCount;			
	//! ÃªµÚ»ôµêÌ© ½ÓÌººã 
	int		m_nDim;	
	//! ·»¼¬°ª´ÉË¾ ÁäÂÜÂ×²÷ ¹ÉµÛ, ¹¤µùËæº· Êú°ıºã, Ãä±ÙËæº· »¨ºã
	double*	m_prCls;			
	//! ÁäËÍÀ¾´ÉÌ© ¹ÉµÛ 
	double**	m_pprData;			
	//! ÃªµÚ»ôµê´ÉÌ© ·Ë±Â 
	double*	m_prWeights;

	//! °é¾¢»ô 
	DataSet();
	DataSet (int nDim, int nCount, int nMode = DS_ALL);
	DataSet (const DataSet& other);
	DataSet (const char* szFilename);
	//! ÃÍ¾Ë»ô 
	~DataSet();

	/**
	 * @brief   ¿Ğµ¹º÷¶¦ ½ó±¨ÃùÂÙ³Ş.
	 *
	 * @param   nDim   [in] : ÃªµÚ»ôµêÌ© ½ÓÌººã
	 * @param   nCount [in] : ÃªµÚ»ôµêÌ© ºã
	 * @param   nMode  [in] : »ôµê¶®Ë¦¸Ò»¥ 
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0
	 *
	 * @see  Release ()
	 *
	 */
	virtual int		Create (int nDim, int nCount, int nMode = DS_ALL);
	int			FlagCreate () const {return m_nFlagCreate;}

	/**
	 * @brief   Ãú¸ó´ô ±¨ÊÍ±¨´ÉË¾ ÃÍ¸ÒÂÙ³Ş. 
	 *
	 * @return  void  
	 *
	 * @see  Create(), Init()
	 *
	 */
	virtual void	Release ();

	/**
	 * @brief   º¡¼­»ôµê¶¦ ¼è¼³´ô ÁÄËÎµá¹¢À¾ ¼¬¼õÂÙ³Ş.
	 *
	 * @param   pFile [in] : ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0
	 *
	 * @see  ToFile()
	 *
	 */
	virtual	int		FromFile (const char* szFilename);
	virtual int		FromFile (FILE* pFile);
	virtual int		FromFile (XFile* pFile);
	
	/**
	 * @brief   º¡¼­»ôµê¶¦ ¼è¼³´ô ÁÄËÎËæ ¸ó±ãÂÙ³Ş.
	 *
	 * @param   pFile [in] : ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0 
	 *
	 * @see  FromFile()
	 *
	 */
	virtual	int		ToFile (const char* szFilename) const;
	virtual int		ToFile (FILE* pFile) const;
	virtual int		ToFile (XFile* pFile) const;

	/**
	 * @brief   ¼è¼³´ô ¿Ğµ¹º÷Ëæ ÁäÂÜ´ô »ôµê±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   nClass [in] : ¿Ğµ¹º÷¸ÜÂö 
	 * @return  »ôµê±¶ºã  
	 *
	 */
	Matd*			ToMat();
	void			FromMat(Matd* pM);

	virtual int		GetSampleNum (int nClass);
	virtual int		GetClassNum ();
	virtual int		Statistics(double* prMin, double* prMax, double* prMean);

	inline int		GetDims() const {return m_nDim;}
	inline int		GetCount() const {return m_nCount;}
	inline int		GetMaxCount() const {return m_nMaxCount;}
	int				CopyFrom (const DataSet& other);
	int				Add (double* prData, double rCls, double rWei=0.0);

	void			SetData (int nIdx, double* prData, double rCls, double rWei=0.0);
	void			SortClsIdx();
protected:
	//! »ôµê¶®Ë¦¸Ò»¥ 
	int		m_nMode;
	int		m_nFlagCreate;
	//! ÃªµÚ»ôµê´ÉÌ© ¾×´İ±¶ºã 
	int		m_nMaxCount;
	int		m_nGrowBy;

	/**
	 * @brief   ¿Ğµ¹º÷¶¦ ½ó±¨ÃùÂÙ³Ş.
	 *
	 * @return  void  
	 *
	 * @see  Create()
	 *
	 */
	void	Init ();
};

}

#endif //_DATASET_H_