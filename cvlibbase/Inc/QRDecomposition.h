/*!
 * \file	QRDecomposition.h
 * \ingroup math
 * \brief	QR���Ͷ� �����ײ� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _QRDECOMPOSITION_H_
#define _QRDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{

/** 
 * @brief  	QR���Ͷ� �����ײ� �е��� 
  <pre>
    m * n�Ե� A (m >= n)�� ������ ��̩ QR���Ͳ� �Ե� A�� m * n������Ե� Q̮ 
	n * n��˯�������Ե� R̩ �� A = Q * R ˺�� �����ײ����˳�.
	QR���Ͳ� �� �����׷� ���� �Ե��� ̰��̡�㶦 ����� ʱ˾ ���洪 �����ٳ�.
	�൹�� �龢���� ���ݵ� ��²�׼� ʱ����. 
  </pre>
 */
class CVLIB_DECLSPEC QRDecomposition
{
public:
	/** 
	 * @brief   �龢�� 
	  <pre>  QR���Ͷ� �����ٳ�. </pre>
     *
	 * @param	pA [in] : �ݾ��Ե� 
	 *
	 * @see  Q()
     *	
	 */
	QRDecomposition(Mat* pA, Matd* pmQR=NULL);

	//!  �;˻� 
	virtual ~QRDecomposition();

	/** 
	 * @brief	�Ե��� full rank�Ͱ��� �����ٳ�.
	 *
	 * @return	R�� full rank �˷� ture
	 *          (R�� full rank �˷� A�� full rank �˷��)
	 *
	 * @see  Q()
     *	
	 */
	virtual bool FullRank();

	/** 
	 * @brief	��˧��������˶���˾ ���ٳ�.
	 *
	 * @return	��˧����������Ե�(ʭ�������Ե�)
	 *
	 * @see  Q()
     *	
	 */
	virtual Mat* H();

	/** 
	 * @brief	˯�������Ե�˾ ���ٳ�.
	 *
	 * @return	R
	 *
	 * @see  Q()
     *	
	 */
	virtual Mat* R();

	/** 
	 * @brief	����Ե� Q�� ���ٳ�.
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
     * 	printf( "QR����̩ ����: A = Q * R \n" );
	 *  
	 *  // A ���� 
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
	 *  // Q ���� 
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
	 *  // R ���� 
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
	 *  // H ���� 
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
	 *  // ��˦
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
	 *  QR����̩ ����: A = Q * R 
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
	 * @brief	A * X = B̩ �׺ϴ��������� X�� ���ٳ�. 
	 *
	 * @param	pB [in] : A̮ �Ժ㰡 ��˼ �Ե� 
	 * @return	X : (Q * R * X - B̩ �����߲Ѷ�˾ �׺����ײ� X)
	  <exception cref="System.ArgumentException"> A̮ B̩ �Ժ㰡 ��ʭʿ �ٳ�.</exception>
	 *
	 * @see  Q(), LUDecomposition::Solve()
     *	
	 */
	virtual Mat* Solve(Mat* pB);

private:
	//! QR�Ե� 
	Mat* m_pmQR;
    //! ����̩ �������Ͱᰣ 
	double** QR;
    //! �Ժ� 
	int m;
	//! �ۺ� 
	int n;
    //! �ݰ����Ե� R̩ �������Ͱᰣ 
	double* Rdiag;
};

}

#endif //_QRDECOMPOSITION_H_