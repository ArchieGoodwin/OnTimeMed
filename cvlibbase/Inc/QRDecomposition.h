/*!
 * \file	QRDecomposition.h
 * \ingroup math
 * \brief	QR¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _QRDECOMPOSITION_H_
#define _QRDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{

/** 
 * @brief  	QR¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷ 
  <pre>
    m * nÃÔµÛ A (m >= n)Ëæ ´İÂ×ÊŞ °ûÌ© QR¹¤ÃÍ²÷ ÃÔµÛ A¶¦ m * nÂô¼é°äÃÔµÛ QÌ® 
	n * nÂôË¯º©°¢ÂôÃÔµÛ RÌ© ¼¬ A = Q * R Ëºµá ¹¤ÃÍÂ×²÷°ÃËË³Ş.
	QR¹¤ÃÍ²÷ ²ø ¼Á¼õÂ×·° ¼èÊÌ ÃÔµÛËË Ì°¼­Ì¡ºã¶¦ °¡¼è¼è Ê±Ë¾ ÆÁËæ´ª ¼Á¼õÂÙ³Ş.
	Åàµ¹º· °é¾¢»ô²÷ ¼®´İµá »¨Â²Â×¼è Ê±²÷³Ş. 
  </pre>
 */
class CVLIB_DECLSPEC QRDecomposition
{
public:
	/** 
	 * @brief   °é¾¢»ô 
	  <pre>  QR¹¤ÃÍ¶¦ ¼êÃÔÂÙ³Ş. </pre>
     *
	 * @param	pA [in] : ´İ¾ÀÃÔµÛ 
	 *
	 * @see  Q()
     *	
	 */
	QRDecomposition(Mat* pA, Matd* pmQR=NULL);

	//!  ÃÍ¾Ë»ô 
	virtual ~QRDecomposition();

	/** 
	 * @brief	ÃÔµÛËË full rankËÍ°¡¶¦ °Áº¡ÂÙ³Ş.
	 *
	 * @return	R°¡ full rank ËË·² ture
	 *          (R°¡ full rank ËË·² A´ª full rank ËË·àµá)
	 *
	 * @see  Q()
     *	
	 */
	virtual bool FullRank();

	/** 
	 * @brief	Â×Ë§º÷Âú³ó¹ÖÀË¶£´ÉË¾ °éÂÙ³Ş.
	 *
	 * @return	Â×Ë§º÷Âú³ó¸êÃûÃÔµÛ(Ê­¶·½£ÂôÃÔµÛ)
	 *
	 * @see  Q()
     *	
	 */
	virtual Mat* H();

	/** 
	 * @brief	Ë¯º©°¢ÂôÃÔµÛË¾ °éÂÙ³Ş.
	 *
	 * @return	R
	 *
	 * @see  Q()
     *	
	 */
	virtual Mat* R();

	/** 
	 * @brief	¼é°äÃÔµÛ Q¶¦ °éÂÙ³Ş.
	 *
	 * @return	Q
	 *
	 * @par Example:
	 * @code
	 *  
	 *  int i, j;
	 *  int nDim = 3;
	 *  
	 *  Mat mTest(nDim, nDim, MAT_Tdouble);
	 *  mTest.Zero();
     * 	for(i = 0; i < nDim; i ++)
	 *  	for(j = 0; j <= i; j ++)
	 *  	{
	 *  		if( i == j)  
	 *  			mTest.data.db[i][j] = Random::Normal();
	 *  		else
     * 			    mTest.data.db[i][j] = mTest.data.db[j][i] = Random::Normal();
	 *  	}
	 *	
	 *  QRDecomposition QRD(&mTest);
	 *  bool flag = QRD.FullRank();
	 *  Mat* pmQ = QRD.Q();
	 *  Mat* pmR = QRD.R();
	 *  Mat* pmH = QRD.H();
	 *  
     * 	printf( "QR¹¤ÃÍÌ© »¨¶Ê: A = Q * R \n" );
	 *  
	 *  // A ¾¥µÙ 
	 *  printf( "A = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
     * 		for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		printf( "\t%.2f", mTest.data.db[i][j] );
	 *  	}
     * 		printf( "\n" );
	 *  }
	 *  
	 *  // Q ¾¥µÙ 
	 *  printf( "Q = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
	 *  	for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		printf( "\t%.2f", pmQ->data.db[i][j] );
	 *  		
	 *  	}
	 *  	printf( "\n" );
	 *  }
	 *  
	 *  // R ¾¥µÙ 
	 *  printf( "R = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
	 *  	for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		printf( "\t%.2f", pmR->data.db[i][j] );
	 *  		
	 *  	}
	 *  	printf( "\n" );
	 *  }
	 *  
	 *  // H ¾¥µÙ 
	 *  printf( "H = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
	 *  	for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		printf( "\t%.2f", pmH->data.db[i][j] );
	 *  		
	 *  	}
	 *  	printf( "\n" );
	 *  }
	 * 
	 *  // ¶®Ë¦
	 *  Mat mB(nDim, nDim, MAT_Tdouble);
	 *  mB.Zero();
	 *  for (i = 0; i < nDim; i ++)
	 *  	mB.data.db[i][i] = 1;
	 *	Mat* pmI = QRD.Solve(&mB);
	 *  ... ... 
	 *  mTest.Release();
	 *  ReleaseMat(pmQ);
	 *  ReleaseMat(pmR);
	 *  ReleaseMat(pmH);
	 *  
	 *  mB.Release();
	 *  ReleaseMat(pmI);
	 * 
	 *
	 *  Output
	 *
	 *  QR¹¤ÃÍÌ© »¨¶Ê: A = Q * R 
	 * 	A = 
	 * 		-0.01	-0.58	0.53
	 * 		-0.58	0.25	0.13
	 * 		0.53	0.13	-0.89
	 * 	Q = 
	 * 		-0.02	0.91	-0.41
	 * 		-0.74	-0.29	-0.60
	 * 		0.67	-0.30	-0.68
	 *  R = 
	 * 		0.79	-0.09	-0.70
	 * 		0.00	-0.64	0.70
	 * 		0.00	0.00	0.31
	 * 	H = 
	 * 		1.02	0.00	0.00
	 *  	0.74	1.95	0.00
	 * 		-0.67	-0.30	2.00
	 *
	 * @endcode
	 * 
	 */
	virtual Mat* Q();

	/** 
	 * @brief	A * X = BÌ© ¾×ºÏ´¸½£°ßÁõËË X¶¦ °éÂÙ³Ş. 
	 *
	 * @param	pB [in] : AÌ® ÃÔºã°¡ °¯Ë¼ ÃÔµÛ 
	 * @return	X : (Q * R * X - BÌ© ´¸½£°ß²Ñ¶§Ë¾ ¾×ºÏÃùÂ×²÷ X)
	  <exception cref="System.ArgumentException"> AÌ® BÌ© ÃÔºã°¡ °¯Ê­Ê¿ ÂÙ³Ş.</exception>
	 *
	 * @see  Q(), LUDecomposition::Solve()
     *	
	 */
	virtual Mat* Solve(Mat* pB);

private:
	//! QRÃÔµÛ 
	Mat* m_pmQR;
    //! ¹¤ÃÍÌ© ³­¹¢±¨ÊÍ°á°£ 
	double** QR;
    //! ÃÔºã 
	int m;
	//! µÛºã 
	int n;
    //! ´İ°¢ººÃÔµÛ RÌ© ³­¹¢±¨ÊÍ°á°£ 
	double* Rdiag;
};

}

#endif //_QRDECOMPOSITION_H_