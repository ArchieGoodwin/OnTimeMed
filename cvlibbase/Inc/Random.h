
/*!
 * \file	Random.h
 * \ingroup base
 * \brief	Ë§Êàºã»¶ºÂË¾ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Object.h"

namespace CVLib
{
/**
 @brief   Ë§Êàºã »¶ºÂ¹¢.
 */
class CVLIB_DECLSPEC Random : public Object
{
public:
	// Constructor and Destructor
	//! °é¾¢»ô
	Random();
	//! ÃÍ¾Ë»ô
	virtual ~Random();

	// Initialization
	/**
	 * @brief	Ë§Êàºã»¶ºÂ±¨¶¦ ½ó±¨ÃùÂÙ³Ş.
	 *
	 * @return	void
	 *
	 * @see  Uniform()
	 *
	 */
	static void Init ();

	// Operations for generating random number according to different distribution.
	/**
	 * @brief   Áâ´Ğ¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş. ¾×´İ°ªË¼ RAND_MAX
	 *
	 * @param	fInit [in] : ½ó±¨º¼¼³±¨¸Ë
	 * @return	»¶ºÂ´ô Ë§Êàºã
	 *
	 * @see  Init()
	 *
	 * @par Example:
	 * @code
	 *
	 *	int i;
	 *
	 *	// Display 10 numbers.
	 *	for( i = 0; i < 10; i++ )
	 *		printf( "  %6d\n", Uniform() );
	 *
	 *	Output
	 *
	 *		6929
	 *		8026
	 *		21987
	 *		30734
	 *		20587
	 *		6699
	 *		22034
	 *		25051
	 *		7988
	 *		10104
	 *
	 * @endcode
	 *
	 */
	static int Uniform (bool fInit = true);

	/**
	 * @brief   Áâ´Ğ¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş.
	 *
	 * @param   rMin  [in] : Ë§ÊàºãÌ© ¾×ºÏ°ª
	 * @param   rMax  [in] : Ë§ÊàºãÌ© ¾×´İ°ª
	 * @param   fInit [in] : ½ó±¨º¼¼³±¨¸Ë
	 * @return	»¶ºÂ´ô Ë§Êàºã
	 *
	 * @see  Uniform()
	 *
	 */
	static float BoundedUniform (float rMin = 0.0001f, float rMax = 1.0f, bool fInit = true);

	/**
	 * @brief   ¼³°ö¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş.
	 *
	 * @param   rMean [in] : Áâ°÷°ª
	 * @param	rStd  [in] : Áì¼ÓÁİ½Ó
	 * @return	»¶ºÂ´ô Ë§Êàºã
	 *
	 * @par Example:
	 * @code
	 *
	 *	int i;
	 *  int nNum = 10; 
	 *	float rSigma = 0.5f;
	 *	float rArray1[5];
	 *	float rArray2[3];
	 *
	 *	for (i = 0; i < 5; i ++)
	 *  {
	 *		 rArray1[i] = rSigma * Random::Normal() + 0.3f;
	 *		 printf( "%.2f\n", rArray1[i]);
	 *	}
	 *	for (i = 0; i < 3; i++)
	 *	{
	 *		 rArray2[i] = Random::Normal();
	 *		 printf( "%.1f\n", rArray2[i]);
	 *	}
	 *	...
	 *
	 *	output
	 *    0.36
	 *    0.97
	 *    -0.07
	 *    -0.87
	 *    0.15
	 *    -0.9
	 *    -0.2
	 *    -0.2
	 *
	 * @endcode
	 *
	 */
	static float Normal(float rMean = 0.0f, float rStd = 1.0f);

	/** 
	 * @brief   ¼èºã¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş.
	 *
	 * @param   rLambda	[in] : ÁÄµ¹¸¡À¾
	 * @return  »¶ºÂ´ô Ë§Êàºã
	 *
	 * @see  BoundedUniform()
	 *
	 */
	static float Exponential(float rLambda);

	/**
	 * @brief   ÄÕ»Õ¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş.
	 *
	 * @param	rMedian	[in] : ¼ÚÊº°ª
	 * @param	rSigma	[in] : Áì¼ÓÁİ½Ó
	 * @return	»¶ºÂ´ô Ë§Êàºã 
	 *
	 * @see  BoundedUniform()
	 *
	 */
	static float Cauchy(float rMedian, float rSigma);

	/**
	 * @brief   µá°û¼³°ö¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş.
	 *
	 * @param   rMean [in] : Áâ°÷°ª
	 * @param	rStdv [in] : Áì¼ÓÁİ½Ó
	 * @return	»¶ºÂ´ô Ë§Êàºã
	 *
	 * @see  Normal()
	 *
	 */
	static float LogNormal(float rMean, float rStdv);

	/**
	 * @brief   ±¨Â×ÂØ¼¬¹¤ÁäËæ Åà¶£²÷ Ë§Êàºã¶¦ »¶ºÂÂÙ³Ş.
		  Êú°ıºã¶¦ ´ó´®¶°³Ş. p(i) = (1-rP) * rP^(i-1)$.
	 *
	 * @param	rP [in] : rP ²÷ 0 < p < 1Ë¾ ¶í¼ÀÂ×ÊŞÊ¿ ÂÙ³Ş.
	 * @return	»¶ºÂ´ô Ë§Êàºã
	 *
	 * @see  BoundedUniform()
	 *
	 */
	static int Geometric(float rP);
};

}

#endif //_RANDOM_H_
