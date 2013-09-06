
/*!
 * \file	Statistics.h
 * \ingroup math
 * \brief	ÀÒ±ËµÌË¾ °éÂ×±¨ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "Mat.h"

namespace CVLib 
{

/** 
 * @brief   ¹ÖÀË¶£´ÉÌ© Áâ°÷¹ÖÀË¶£¶¦ °éÂÙ³Ş.
 *
 * @param	pvecExamples [in]  : ¹ÖÀË¶£´ÉÌ© ºº´¸¼ÑºÏ
 * @param	nNum		 [in]  : ¹ÖÀË¶£±¶ºã
 * @param	pvecMean	 [out] : Áâ°÷¹ÖÀË¶£
 * @param	pvecWeight	 [in]  : ¹ÖÀË¶£´ÉÌ© ·Ë±Â¶¦ ¸ÉÊéÂÙ ¹ÖÀË¶£ \n
 *				NullËÍ °ÒË§ ¹ÖÀË¶£´ÉÌ© ·Ë±Â¶¦ °ÖµØÂ×¼è Ê±²÷³Ş.
 * @return	void
 *
 * @see	Covariance(), Variation()
 *
 */
void	CVLIB_DECLSPEC MeanVector (Vec* pvecExamples, int nNum, Vec* pvecMean, Vec* pvecWeight = NULL);

/** 
 * @brief	¹ÖÀË¶£´ÉÌ© °á¹¤º¤ÃÔµÛË¾ ±Ëº¤ÂÙ³Ş.
 *
 * @param   pvecExamples [in]     : ¹ÖÀË¶£´ÉÌ© ºº´¸¼ÑºÏ
 * @param	nNum		 [in]     : ¹ÖÀË¶£±¶ºã
 * @param	pmatCov		 [out]    : °á¹¤º¤ÃÔµÛ
 * @param	pvecMean	 [in/out] : Áâ°÷¹ÖÀË¶£ \n
 *				nullËÍ °ÒË§ Áâ°÷¹ÖÀË¶£±Ëº¤Ë¾ ¼êÃÔÂ×·° Ê­³¥ °ÒË§ ¼êÃÔÂ×¼è Ê±²÷³Ş.
 * @param	pvecWeight	 [in]     : ¹ÖÀË¶£´ÉÌ© ·Ë±Â¶¦ ¸ÉÊéÂÙ ¹ÖÀË¶£ \n
 *				nullËÍ °ÒË§ ¹ÖÀË¶£´ÉÌ© ·Ë±Â¶¦ °ÖµØÂ×¼è Ê±²÷³Ş.
 * @return	void
 *
 * @see	MeanVector(), Variation()
 *
 */
void	CVLIB_DECLSPEC Covariance (Vec* pvecExamples, int nNum, Mat* pmatCov, Vec* pvecMean = NULL, Vec* pvecWeight = NULL);

/** 
 * @brief   ¹ÖÀË¶£´ÉÌ© ¸ê´´ÃÔµÛË¾ ±Ëº¤ÂÙ³Ş.
 *
 * @param	pvecExamples [in]     : ¹ÖÀË¶£´ÉÌ© ºº´¸¼ÑºÏ
 * @param	nNum		 [in]     : ¹ÖÀË¶£±¶ºã
 * @param	pmatVar		 [out]    : ¸ê´´ÃÔµÛ
 * @param	pvecMean	 [in/out] : Áâ°÷¹ÖÀË¶£ \n
 *				nullËÍ °ÒË§ Áâ°÷¹ÖÀË¶£±Ëº¤Ë¾ ¼êÃÔÂ×·° Ê­³¥ °ÒË§ ¼êÃÔÂ×¼è Ê±²÷³Ş.
 * @param	pvecWeight	 [in]     : ¹ÖÀË¶£´ÉÌ© ·Ë±Â¶¦ ¸ÉÊéÂÙ ¹ÖÀË¶£ \n
 *				nullËÍ °ÒË§ ¹ÖÀË¶£´ÉÌ© ·Ë±Â¶¦ °ÖµØÂ×¼è Ê±²÷³Ş.
 *
 * @return	void
 *
 * @see	MeanVector(), Covariance()
 *
 */
void	CVLIB_DECLSPEC Variation (Vec* pvecExamples, int nNum, Mat* pmatVar, Vec* pvecMean = NULL, Vec* pvecWeight = NULL);

}

#endif //_STATISTICS_H_
