
/*! 
 * \file    LUDecomposition.h
 * \ingroup math
 * \brief   LU¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _LUDECOMPOSITION_H_
#define _LUDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{
	
/** 
 * @brief  	LU¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷ 
   <pre>
	  m x n (m >= n)ÃÔµÛ AËæ ´İÂ×ÊŞ LU¹¤ÃÍ²÷ A(piv,:) = L * U ËÍ 
	  m x n Ê­¶·º©°¢ÂôÃÔµÛ L, n x n Ë¯º©°¢ÂôÃÔµÛ U, °û¶®°Ö 
	  ±¬ËË mÌ© ºåµÛ¹ÖÀË¶£ piv¶¦ °éÂ×²÷°ÃËË³Ş.
	 <code> ¶íËÎ m < n ËË·² LË¼  m x mÃÔµÛ, U²÷ m x nÃÔµÛ. </code>
	 pivotingË¾ °¡¼ê LU¹¤ÃÍ²÷ Âßº¬ ¼Á¼õÂ×·° (¼èÊÌ Á¬ÃùÃÔµÛËæ ´İÃÍº·´ª ¼Á¼õ)
	 Åàµ¹º· °é¾¢»ô²÷ ¼®´İµá »¨Â²Â×¼è Ê±²÷³Ş.
   </pre>
 */
class CVLIB_DECLSPEC LUDecomposition
{
public:		
	/** 
	 * @brief	°é¾¢»ô.
	  <pre>  LU¹¤ÃÍ¶¦ ¼êÃÔÂÙ³Ş. </pre>
	 *
	 * @param	pA [in] : ¼é4°¢ÂôÃÔµÛ 
	 * @return	L, U, piv
	 *
	 * @see  L()
     *
	 */
	LUDecomposition(const Mat* pA, Mat* pmLU = NULL);
	//!	ÃÍ¾Ë»ô 
	virtual ~LUDecomposition();

	/** 
	 * @brief	ÃÔµÛËË ¹¦Á¬ÃùËÍ°¡¶¦ °Áº¡ÂÙ³Ş. 
	 *
	 * @return	U°¡ ¹¦Á¬ÃùËË·² true. 
	 *          (U°¡ ¹¦Á¬ÃùËË·² A´ª ¹¦Á¬ÃùËË·àµá)
	 *
	 * @see  Solve()
     *
	 */
	virtual bool IsNonSingular();

	/** 
	 * @brief  	Ê­¶·º©°¢ÂôÃÔµÛË¾ ÊĞ²÷³Ş.
	 *
	 * @return 	Ê­¶·º©°¢ÂôÃÔµÛ L
	 *
	 * @par Example:
	 * @code
	 *  
	 *  int i, j;
	 *  int nRows = 6;
	 *  int nCols = 4;
	 *  float rSigma = 0.5f; 
	 *  
	 *  Mat mTest(nRows, nCols, MAT_Tdouble);
     * 	mTest.Zero();
	 *  for(i = 0; i < nRows; i ++)
	 *  	for(j = 0; j < nCols; j ++)
	 *  		mTest.data.db[i][j] = rSigma * Random::Normal() + 0.3f;
	 *  
	 *  LUDecomposition LUD(&mTest);
	 *  Mat* pmL = LUD.L();
	 *  Mat* pmU = LUD.U();
	 *  int* pPiv = LUD.Pivot();
	 *  double* prPiv = LUD.DoublePivot();
	 *  
	 *  printf( "LU¹¤ÃÍÌ© »¨¶Ê: A = L * U \n" );
	 *  
	 *  // A ¾¥µÙ  
	 *  printf( "A = \n" );
	 *  for(i = 0; i < nRows; i ++)
	 *  {
	 *  	for(j = 0; j < nCols; j ++)
	 *  	{
	 *  		printf( "\t%.2f", mTest.data.db[i][j] );
	 *  	}
	 *  	printf( "\n" );
	 *  }
	 *  
	 *  // L ¾¥µÙ 
	 *  printf( "L = \n" );
	 *  for(i = 0; i < nRows; i ++)
	 *  {
	 *  	for(j = 0; j < nCols; j ++) 
	 *  	{
	 *  		printf( "\t%.2f", pmL->data.db[i][j] );
	 *  		
	 *  	}
	 *  	printf( "\n" );
	 *  }
	 *  
	 *  // U ¾¥µÙ 
	 *  printf( "U = \n" );
	 *  for(i = 0; i < nCols; i ++)
	 *  {
	 *  	for(j = 0; j < nCols; j ++)
	 *  	{
	 *  		printf( "\t%.2f", pmU->data.db[i][j] ); 
	 *  		
	 *  	}
	 *  	printf( "\n" );
	 *  }
	 *  
	 *  // piv ¾¥µÙ 
	 *  printf( "piv = (" );
	 *  for(i = 0; i < nCols; i ++)
	 *	printf( "\t%d", *pPiv++ );
	 *  printf( "\t)\n" );
	 *  
	 *  mTest.Release();
	 *  ReleaseMat(pmL);
	 *  ReleaseMat(pmU); 
	 * 
	 *
	 *  Output
	 *  
	 *  LU¹¤ÃÍÌ© »¨¶Ê: A = L * U 
	 *  A = 
	 *  	0.26	0.51	0.66	0.46
	 *  	-0.43	-0.41	-0.13	-0.14
	 *  	0.92	0.30	0.39	0.03
	 *  	0.10	0.27	0.24	-0.09
	 *  	1.97	-0.33	0.41	0.59
	 *  	0.44	-0.47	-0.07	0.56
	 *  L = 
	 *  	1.00	0.00	0.00	0.00
	 *  	0.13	1.00	0.00	0.00
	 *  	-0.22	-0.88	1.00	0.00
	 *  	0.22	-0.71	0.55	1.00
	 *  	0.47	0.83	-0.61	-0.70
	 *  	0.05	0.53	-0.20	-0.49
	 *  U = 
	 *  	1.97	-0.33	0.41	0.59
	 *  	0.00	0.55	0.60	0.38
	 *  	0.00	0.00	0.49	0.32
	 *  	0.00	0.00	0.00	0.53
	 *  piv = (	4	0	1	5	)
	 *  
	 * @endcode
	 * 
	 */
	virtual Mat* L();

	/** 
	 * @brief	Ë¯º©°¢ÂôÃÔµÛË¾ ÊĞ²÷³Ş.
	 *
	 * @return	Ë¯º©°¢ÂôÃÔµÛ U
	 *
	 * @see  L()
     *
	 */
	virtual Mat* U();

	/** 
	 * @brief	¾·Ãû¹ÖÀË¶£¶¦ ÊĞ²÷³Ş.
	 *
	 * @return	piv
	 *
	 * @see  L()
     *
	 */
	virtual int* Pivot();

	/** 
	 * @brief	doubleÂôÌ© ¾·Ãû¹ÖÀË¶£¶¦ ÊĞ²÷³Ş.
	 *
	 * @return	(double) piv
	 *
	 * @see  L()
     *
	 */
	virtual double* DoublePivot();

	/** 
	 * @brief	ÃÔµÛÌ© ÃÔµÛ»¥Ë¾ ±Ëº¤ÂÙ³Ş. 
	 *
	 * @return	ÃÔµÛ»¥ 
	 *
	 * @see  Solve()
     *
	 */
	virtual double Determinant();
	
	/** 
	 * @brief	A * X = B Ì© ÁõËË X ¶¦ °éÂÙ³Ş. 
	 *
	 * @param	pB [in] : ÃÔµÛ AÌ® ÃÔºã°¡ °¯Ë¼ ÃÔµÛ.
	 * @return	L * U * X = B(piv,:) ËÍ ÃÔµÛ X
	 <exception cref="System.ArgumentException"> AÌ® BÌ© ÃÔºã°¡ °¯Ê­Ê¿ ÂÙ³Ş. </exception>
	 *
	 * @par Example:
	 * @code
	 *  
	 *  int i, j;
	 *  int nDim = 3;
	 *  
	 *  Mat mTest(nDim, nDim, MAT_Tdouble);
	 *  mTest.Zero();
	 *  for(i = 0; i < nDim; i ++)
	 *  	for(j = 0; j < nDim; j ++)
	 *  		mTest.data.db[i][j] = Random::Normal();
	 *  	
	 *  LUDecomposition LUD(&mTest);
	 *  double rDet = LUD.Determinant();
	 *  if( LUD.IsNonSingular() )
	 *  {
	 *  	Mat mB(nDim, nDim, MAT_Tdouble);
	 *  	
	 *  	mB.Zero();
	 *  	for (i = 0; i < nDim; i ++)
	 *  		mB.data.db[i][i] = 1;
	 *      Mat* pmI = LUD.Solve(&mB);
	 *  
	 *      printf( "LU¹¤ÃÍÌ© ¶®Ë¦:\n inv(M) : \n" );
	 *      for(i = 0; i < nDim; i ++)
	 *      {
	 *  	   for(j = 0; j < nDim; j ++)
	 *  	   {
	 *  		   printf( "\t%.2f", pmI->data.db[i][j] );
	 * 	       }
	 *	       printf( "\n" );
	 *      }
	 *
	 *      printf( "M * inv(M): \n" );
	 *      Mat mC(nDim, nDim, MAT_Tdouble);
	 *      MatMulMat(&mTest, pmI, &mC);
	 *
	 *      for(i = 0; i < nDim; i ++)
	 *      {
	 *          for(j = 0; j < nDim; j ++)
	 *          {
	 * 		        printf( "\t%.2f", mC.data.db[i][j] );
	 *	        }
	 *	        printf( "\n" );
	 *      }
	 *
	 *      ReleaseMat(pmI);
	 *      mB.Release();
	 *  }
	 *  
	 *  output
	 *  
	 *  LU¹¤ÃÍÌ© ¶®Ë¦: 
	 *   inv(M) : 
	 *  	1.14	-0.21	-2.01
	 *  	0.91	-0.41	0.02
	 *  	2.22	1.18	0.02
	 *   M * inv(M): 
	 *  	1.00	-0.00	0.00
	 *  	-0.00	1.00	0.00
	 *  	0.00	0.00	1.00
	 *  
	 * @endcode
	 * 
	 */	
	virtual Mat* Solve(Mat* pB);

private:
	//! LUÃÔµÛ 
	Mat* m_pmLU;
	//! ¹¤ÃÍÌ© ³­¹¢±¨ÊÍ°á°£ 
	double** LU;

	//! ÃÔºã  
	int m;
	//! µÛºã  
	int n;
	//! ¾·Ãû¹¢Âö 
	int pivsign;
	//! ¾·Ãû¹ÖÀË¶£ 
	int* piv;
};

}

#endif //_LUDECOMPOSITION_H_















