
/*! 
 * \file    LUDecomposition.h
 * \ingroup math
 * \brief   LU���Ͷ� �����ײ� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _LUDECOMPOSITION_H_
#define _LUDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{
	
/** 
 * @brief  	LU���Ͷ� �����ײ� �е��� 
   <pre>
	  m x n (m >= n)�Ե� A�� ������ LU���Ͳ� A(piv,:) = L * U �� 
	  m x n ʭ���������Ե� L, n x n ˯�������Ե� U, ������ 
	  ���� m̩ ��۹��˶� piv�� ���ײ����˳�.
	 <code> ���� m < n �˷� L˼  m x m�Ե�, U�� m x n�Ե�. </code>
	 pivoting˾ ���� LU���Ͳ� �ߺ� �����׷� (���� �����Ե��� ���ͺ��� ����)
	 �൹�� �龢���� ���ݵ� ��²�׼� ʱ����.
   </pre>
 */
class CVLIB_DECLSPEC LUDecomposition
{
public:		
	/** 
	 * @brief	�龢��.
	  <pre>  LU���Ͷ� �����ٳ�. </pre>
	 *
	 * @param	pA [in] : ��4�����Ե� 
	 * @return	L, U, piv
	 *
	 * @see  L()
     *
	 */
	LUDecomposition(const Mat* pA, Mat* pmLU = NULL);
	//!	�;˻� 
	virtual ~LUDecomposition();

	/** 
	 * @brief	�Ե��� �������Ͱ��� �����ٳ�. 
	 *
	 * @return	U�� �������˷� true. 
	 *          (U�� �������˷� A�� �������˷��)
	 *
	 * @see  Solve()
     *
	 */
	virtual bool IsNonSingular();

	/** 
	 * @brief  	ʭ���������Ե�˾ �в���.
	 *
	 * @return 	ʭ���������Ե� L
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
	 *  printf( "LU����̩ ����: A = L * U \n" );
	 *  
	 *  // A ����  
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
	 *  // L ���� 
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
	 *  // U ���� 
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
	 *  // piv ���� 
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
	 *  LU����̩ ����: A = L * U 
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
	 * @brief	˯�������Ե�˾ �в���.
	 *
	 * @return	˯�������Ե� U
	 *
	 * @see  L()
     *
	 */
	virtual Mat* U();

	/** 
	 * @brief	�������˶��� �в���.
	 *
	 * @return	piv
	 *
	 * @see  L()
     *
	 */
	virtual int* Pivot();

	/** 
	 * @brief	double��̩ �������˶��� �в���.
	 *
	 * @return	(double) piv
	 *
	 * @see  L()
     *
	 */
	virtual double* DoublePivot();

	/** 
	 * @brief	�Ե�̩ �Եۻ�˾ �˺��ٳ�. 
	 *
	 * @return	�Եۻ� 
	 *
	 * @see  Solve()
     *
	 */
	virtual double Determinant();
	
	/** 
	 * @brief	A * X = B ̩ ���� X �� ���ٳ�. 
	 *
	 * @param	pB [in] : �Ե� A̮ �Ժ㰡 ��˼ �Ե�.
	 * @return	L * U * X = B(piv,:) �� �Ե� X
	 <exception cref="System.ArgumentException"> A̮ B̩ �Ժ㰡 ��ʭʿ �ٳ�. </exception>
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
	 *      printf( "LU����̩ ��˦:\n inv(M) : \n" );
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
	 *  LU����̩ ��˦: 
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
	//! LU�Ե� 
	Mat* m_pmLU;
	//! ����̩ �������Ͱᰣ 
	double** LU;

	//! �Ժ�  
	int m;
	//! �ۺ�  
	int n;
	//! �������� 
	int pivsign;
	//! �������˶� 
	int* piv;
};

}

#endif //_LUDECOMPOSITION_H_















