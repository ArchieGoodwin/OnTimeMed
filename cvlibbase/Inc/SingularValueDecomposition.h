
/*!
 * \file	SingularValueDecomposition.h
 * \ingroup math
 * \brief	���˰����Ͷ� �����ײ� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _SINGULARVALUEDECOMPOSITION_H_
#define _SINGULARVALUEDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{

/**
 * @brief   ���˰����Ͷ� �����ײ� �е���
  <pre>
	m * n �Ե� A(m >= n)�� ������ ���˰����Ͳ� A = U * S * V' �� ��� 
	m * m ����Ե� U, n * m �ݰ����Ե� S, n * n ����Ե� V �� ���ײ����˳�.
	���˰����� sigma[k] = S[k][k] �� sigma[0] >= sigma[1] >= ... >= sigma[n-1] �� 
	�康������. 
	���˰����Ͳ� �ߺ� �����׷� �൹�� �龢���� ���ݵ� ��²�׼� ʱ����.
	�Ե�̩ ������̮ ˱ã��� �커�� ̡�㶦 �� ���͵Ṣ�� �˺��ں� �س�. 
  </pre>
 */
class CVLIB_DECLSPEC SingularValueDecomposition
{
public:
	/**
	 * @brief   �龢��
	  <pre>  ���˰����Ͷ� �����ٳ�. </pre>
	 *
	 * @param   pmArg [in] : ��4�����Ե� 
	 * @return  U, S, V
	 *
	 * @see  GetS()
     *
	 */
	SingularValueDecomposition(Mat* pmArg, Matd* pmS=NULL, Matd* pmU=NULL, Matd* pmV=NULL);
	//! �;˻� 
	virtual ~SingularValueDecomposition();

	/**
	 * @brief   ���˰��ɵ�˾ �в���. 
	 *
	 * @return  ���˰��ɵ�
	 *
	 * @see  GetS()
     *
	 */
	virtual double* SingularValues();
	
	/**
	 * @brief   ���˰�̩ �ݰ����Ե�˾ �в���.
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
	 * @brief   ��ɸ���˹��˶���˾ �в���.
	 *
	 * @return  U
	 *
	 * @see  GetS()
     *
	 */
	virtual Mat* GetU();

	/**
	 * @brief   �ﶥɸ���˹��˶���˾ �в���.
	 *
	 * @return  V
	 *
	 * @see  GetS()
     *
     */
	virtual Mat* GetV();

	/**
	 * @brief  �״����˰�˾ ���ٳ�.
	 *
	 * @return  max(S).
	 *
	 * @see  GetS()
     *
	 */
	virtual double Norm2();

	/**
	 * @brief   �����㶦 ���ٳ�.
	 *
	 * @return  max(S) / min(S).
	 *
	 * @see  GetS()
     *
	 */
	virtual double Condition();

	/**
	 * @brief   ˱ã��� �커�� �Ե�̡�� 
	 *
	 * @return  ̡��(�˻��ں� �ֲ� ���˰�����)
	 *
	 * @see  GetS()
     *
	 */
	virtual int Rank();

private:

	//!  �Ե� 
	Mat* m_pmA;

	//!  U̩ �������Ͱᰣ 
	double** U;
	//!  V̩ �������Ͱᰣ 
	double** V;
    //!  ���˰�̩ �������Ͱᰣ 	
	double* s;

	//!  �Խ�̺�� 
	int m;
	//!  �۽�̺�� 
	int n;
	//! ��̡����
	bool m_bTrans;
};

}

#endif //_SINGULARVALUEDECOMPOSITION_H_
