/*! 
 * \file    CholeskyDecomposition.h
 * \ingroup math
 * \brief   ¿»¶Áº÷¿Ô¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _CHOLESKYDECOMPOSITION_H_
#define _CHOLESKYDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{
	
/** 
 * @brief	¿»¶Áº÷¿Ô¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷ 
  <pre>
	´İ¾ÀËÍ ¼³¾¸ÃÔµÛ AËæ ´İÂ×ÊŞ ¿»¶Áº÷¿Ô¹¤ÃÍ²÷ A = L * L'ËÍ Ê­¶·º©°¢ÂôÃÔµÛ 
	LË¾ °éÂ×²÷°ÃËË³Ş.
	ÃÔµÛËË ´İ¾ÀËË Ê­³£°º±ı ¼³¾¸ËË Ê­³¦ ÆÁ °é¾¢»ô²÷ ¹¦Ì°¼­ÂÙ ¹¤ÃÍ¶¦ ´®µØ¼Ñ·° 
	SPD()ÂÜºãËæ Ì©ÃÍ ´®µØ¼è²÷ ³­¹¢±¨¸ËË¾ º¼¼³ÂÙ³Ş.
  </pre>
 */
class CVLIB_DECLSPEC CholeskyDecomposition
{
public:
	/** 
	 * @brief	°é¾¢»ô 
	  <pre> ´İ¾ÀËÍ ¼³¾¸ÃÔµÛÌ© ¿»¶Áº÷¿Ô¹¤ÃÍ¶¦ ¼êÃÔÂÙ³Ş. </pre>
	 *
	 * @param	pArg [in] : ´İ¾ÀÃÔµÛ
	 * @return	L, isspd±¨¸Ë
	 *
	 * @see  Solve()
     *
	 */
	CholeskyDecomposition(Mat* pArg, Matd* pmL=NULL);
	//!	ÃÍ¾Ë»ô 
	virtual ~CholeskyDecomposition();

	/** 
	 * @brief	ÃÔµÛËË ´İ¾ÀËË°Ö ¼³¾¸ËÍ°¡¶¦ °Áº¡ÂÙ³Ş.
	 *
	 * @return	A°¡ ´İ¾À, ¼³¾¸ÃÔµÛËË·² true
	 *
	 * @see  Solve()
     *
	 */
	virtual bool SPD();

	/** 
	 * @brief	º©°¢ÂôÃÔµÛË¾ ÊĞ²÷³Ş.
	 *
	 * @return	º©°¢ÂôÃÔµÛ L
	 *
	 * @see  Solve()
     *
	 */
	virtual Mat* GetL();

	/** 
	 * @brief	A * X = BÌ© ÁõËË X¶¦ °éÂÙ³Ş.
	 *
	 * @param	pB [in] : ÃÔµÛ AÌ® ÃÔºã°¡ °¯Ë¼ ÃÔµÛ 
	 * @return	L * L'* X = B ËÍ ÃÔµÛ X
	   <exception cref="System.ArgumentException">  AÌ® BÌ© ÃÔºã°¡ °¯Ê­Ê¿ ÂÙ³Ş. </exception>
	 *
	 * @see  LUDecomposition::Solve()
	 *
	 * @par Example:
	 * @code
	 *  
	 *  int i, j;
	 *  int nDim = 3;
	 *  
	 *  Mat mTest(nDim, nDim, MAT_Tdouble);
	 *  mTest.Zero();
	 *  double** ppD = mTest.data.db;
	 *  ppD[0][0] = 5;	ppD[0][1] = 3;	ppD[0][2] = 0;
	 *  ppD[1][0] = 3;	ppD[1][1] = 5;	ppD[1][2] = 1;
	 *  ppD[2][0] = 0;	ppD[2][1] = 1;	ppD[2][2] = 4;
	 *  
	 *  CholeskyDecomposition ChD(&mTest);
	 *  bool flag = ChD.SPD();
	 *  
	 *  if( flag )
	 *  {	
	 *  	Mat* pmL = ChD.GetL();
	 *    
	 *      Mat mB(nDim, nDim, MAT_Tdouble); 
	 *      mB.Zero();
	 *      for (i = 0; i < nDim; i ++)
	 *  	    mB.data.db[i][i] = 1;
	 *      Mat* pmI = ChD.Solve(&mB);
	 *      ... ...
	 *      mTest.Release();
	 *      mB.Release();
	 *      ReleaseMat(pmI);
	 *  }
	 *  ... 
	 * @endcode
	 * 
	 */
	virtual Mat* Solve(Mat* pB);

private:
	//! ¹¤ÃÍÌ© ³­¹¢±¨ÊÍ°á°£ 
	double** L;
	//! ÃÔµÛÌ© ½ÓÌººã 
	int n;
	//! ´İ¾À ·ñ ¼³¾¸Ë¾ ±ıÀ°³­²÷ ±¨¸Ë 
	bool isspd;
};

}

#endif //_CHOLESKYDECOMPOSITION_H_
