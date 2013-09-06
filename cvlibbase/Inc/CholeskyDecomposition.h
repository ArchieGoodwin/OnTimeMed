/*! 
 * \file    CholeskyDecomposition.h
 * \ingroup math
 * \brief   �������Թ��Ͷ� �����ײ� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _CHOLESKYDECOMPOSITION_H_
#define _CHOLESKYDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{
	
/** 
 * @brief	�������Թ��Ͷ� �����ײ� �е��� 
  <pre>
	�ݾ��� �����Ե� A�� ������ �������Թ��Ͳ� A = L * L'�� ʭ���������Ե� 
	L˾ ���ײ����˳�.
	�Ե��� �ݾ��� ʭ������ ������ ʭ�� �� �龢���� ��̰���� ���Ͷ� ���ؼѷ� 
	SPD()�ܺ��� ̩�� ���ؼ�� ��������˾ �����ٳ�.
  </pre>
 */
class CVLIB_DECLSPEC CholeskyDecomposition
{
public:
	/** 
	 * @brief	�龢�� 
	  <pre> �ݾ��� �����Ե�̩ �������Թ��Ͷ� �����ٳ�. </pre>
	 *
	 * @param	pArg [in] : �ݾ��Ե�
	 * @return	L, isspd����
	 *
	 * @see  Solve()
     *
	 */
	CholeskyDecomposition(Mat* pArg, Matd* pmL=NULL);
	//!	�;˻� 
	virtual ~CholeskyDecomposition();

	/** 
	 * @brief	�Ե��� �ݾ��˰� �����Ͱ��� �����ٳ�.
	 *
	 * @return	A�� �ݾ�, �����Ե��˷� true
	 *
	 * @see  Solve()
     *
	 */
	virtual bool SPD();

	/** 
	 * @brief	�������Ե�˾ �в���.
	 *
	 * @return	�������Ե� L
	 *
	 * @see  Solve()
     *
	 */
	virtual Mat* GetL();

	/** 
	 * @brief	A * X = B̩ ���� X�� ���ٳ�.
	 *
	 * @param	pB [in] : �Ե� A̮ �Ժ㰡 ��˼ �Ե� 
	 * @return	L * L'* X = B �� �Ե� X
	   <exception cref="System.ArgumentException">  A̮ B̩ �Ժ㰡 ��ʭʿ �ٳ�. </exception>
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
	//! ����̩ �������Ͱᰣ 
	double** L;
	//! �Ե�̩ ��̺�� 
	int n;
	//! �ݾ� �� ����˾ �������� ���� 
	bool isspd;
};

}

#endif //_CHOLESKYDECOMPOSITION_H_
