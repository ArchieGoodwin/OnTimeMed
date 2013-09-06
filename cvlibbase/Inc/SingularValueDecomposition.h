
/*!
 * \file	SingularValueDecomposition.h
 * \ingroup math
 * \brief	ÀãËË°ª¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _SINGULARVALUEDECOMPOSITION_H_
#define _SINGULARVALUEDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{

/**
 * @brief   ÀãËË°ª¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷
  <pre>
	m * n ÃÔµÛ A(m >= n)Ëæ ´İÂ×ÊŞ ÀãËË°ª¹¤ÃÍ²÷ A = U * S * V' µá ´ó²÷ 
	m * m ¼é°äÃÔµÛ U, n * m ´İ°¢ººÃÔµÛ S, n * n ¼é°äÃÔµÛ V ¶¦ °éÂ×²÷°ÃËË³Ş.
	ÀãËË°ª´ÉËÍ sigma[k] = S[k][k] ²÷ sigma[0] >= sigma[1] >= ... >= sigma[n-1] µá 
	ºåº·Ãù´ô³Ş. 
	ÀãËË°ª¹¤ÃÍ²÷ Âßº¬ ¼Á¼õÂ×·° Åàµ¹º· °é¾¢»ô²÷ ¼®´İµá »¨Â²Â×¼è Ê±²÷³Ş.
	ÃÔµÛÌ© ¼¿°¼ºãÌ® Ë±Ã£ºãµá Áì»¤ÂÙ Ì¡ºã¶¦ ËË ¹¤ÃÍµá¹¢À¾ ±Ëº¤ÂÚºã ËØ³Ş. 
  </pre>
 */
class CVLIB_DECLSPEC SingularValueDecomposition
{
public:
	/**
	 * @brief   °é¾¢»ô
	  <pre>  ÀãËË°ª¹¤ÃÍ¶¦ ¼êÃÔÂÙ³Ş. </pre>
	 *
	 * @param   pmArg [in] : ¼é4°¢ÂôÃÔµÛ 
	 * @return  U, S, V
	 *
	 * @see  GetS()
     *
	 */
	SingularValueDecomposition(Mat* pmArg, Matd* pmS=NULL, Matd* pmU=NULL, Matd* pmV=NULL);
	//! ÃÍ¾Ë»ô 
	virtual ~SingularValueDecomposition();

	/**
	 * @brief   ÀãËË°ª¹ÉµÛË¾ ÊĞ²÷³Ş. 
	 *
	 * @return  ÀãËË°ª¹ÉµÛ
	 *
	 * @see  GetS()
     *
	 */
	virtual double* SingularValues();
	
	/**
	 * @brief   ÀãËË°ªÌ© ´İ°¢ººÃÔµÛË¾ ÊĞ²÷³Ş.
	 *
	 * @return  S
	 *
	 * @par Example:
	 * @code
	 *  
	 *  int i, j;
	 *  int nRows = 6;
	 *  int nCols = 4;
	 *  char szFileName[] = "c:/testSVD.bin";
	 *  
	 *  Mat mTest(nRows, nCols, MAT_Tdouble);
     * 	mTest.Zero();
	 *  for(i = 0; i < nRows; i ++)
	 *  	for(j = 0; j < nCols; j ++)
	 *  		mTest.data.db[i][j] = Random::Normal();
	 *  
	 *  SingularValueDecomposition SVD(&mTest);
	 *  
	 *  Mat* pmU = SVD.GetU();
	 *  Mat* pmS = SVD.GetS();
	 *  Mat* pmV = SVD.GetV();
	 *  double* rSingular = SVD.SingularValues();
	 *  int nRank = SVD.Rank();
	 *  double rCond = SVD.Condition();
	 *  ... ... 
	 *  mTest.Release();
	 *  ReleaseMat(pmU);
	 *  ReleaseMat(pmS);
	 *  ReleaseMat(pmV);
	 *  
	 * @endcode
	 * 
	 */
	virtual Mat* GetS();

	/**
	 * @brief   ËùÉ¸ÀãËË¹ÖÀË¶£´ÉË¾ ÊĞ²÷³Ş.
	 *
	 * @return  U
	 *
	 * @see  GetS()
     *
	 */
	virtual Mat* GetU();

	/**
	 * @brief   Êï¶¥É¸ÀãËË¹ÖÀË¶£´ÉË¾ ÊĞ²÷³Ş.
	 *
	 * @return  V
	 *
	 * @see  GetS()
     *
     */
	virtual Mat* GetV();

	/**
	 * @brief  ¾×´İÀãËË°ªË¾ °éÂÙ³Ş.
	 *
	 * @return  max(S).
	 *
	 * @see  GetS()
     *
	 */
	virtual double Norm2();

	/**
	 * @brief   ¼¿°¼ºã¶¦ °éÂÙ³Ş.
	 *
	 * @return  max(S) / min(S).
	 *
	 * @see  GetS()
     *
	 */
	virtual double Condition();

	/**
	 * @brief   Ë±Ã£ºãµá Áì»¤ÂÙ ÃÔµÛÌ¡ºã 
	 *
	 * @return  Ì¡ºã(·Ë»¤ÂÚºã ÊÖ²÷ ÀãËË°ª±¶ºã)
	 *
	 * @see  GetS()
     *
	 */
	virtual int Rank();

private:

	//!  ÃÔµÛ 
	Mat* m_pmA;

	//!  UÌ© ³­¹¢±¨ÊÍ°á°£ 
	double** U;
	//!  VÌ© ³­¹¢±¨ÊÍ°á°£ 
	double** V;
    //!  ÀãËË°ªÌ© ³­¹¢±¨ÊÍ°á°£ 	
	double* s;

	//!  ÃÔ½ÓÌººã 
	int m;
	//!  µÛ½ÓÌººã 
	int n;
	//! ¼­Ì¡±¨¸Ë
	bool m_bTrans;
};

}

#endif //_SINGULARVALUEDECOMPOSITION_H_
