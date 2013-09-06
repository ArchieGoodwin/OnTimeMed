/*!
 * \file	MatLib.h
 * \ingroup base
 * \brief	�Ե�̩ ��˱���� ��˱���˶� �� �຤˾ ̡�� �ܺ��˾ ��̩��.
 * \author	����˧
 */

#ifndef _MATLIB_H_
#define _MATLIB_H_

#include "cvlibbaseDef.h"

namespace CVLib
{
/** 
 * @brief   ���Ե�˾ �в� �ܺ��˳�. (float������Ե�)
 *
 * @param	nDim      [in]  : �Ե�̩ ��̺�� 
 * @param	ppdMatrix [out]	: �Ե� ( In ) / ���Ե� ( Out )
 * @return	�°��˷� true
 * 
 */
int CVLIB_DECLSPEC ComputeInverseMatrix ( int nDim, float** ppdMatrix );

/** 
 * @brief   ���Ե�˾ �в� �ܺ��˳�. (double������Ե�)
 *
 * @param	nDim      [in]  : �Ե�̩ ��̺�� 
 * @param	ppdMatrix [out]	: �Ե� ( In ) / ���Ե� ( Out )
 * @return	�°��˷� true
 * 
 */
int CVLIB_DECLSPEC ComputeInverseMatrix ( int nDim, double** ppdMatrix );

/**
 * @brief   nDim*2�Ե�̩ ��˱���� ��˱���˶��� ���ײ� �ܺ��˳�.
 * 
 * @param	pparData	  [in]	: �ӵ��Ե�
 * @param	nDim		  [in]	: �Ե�̩ ��̺�� 
 * @param	parEigenValue [out]	: ��˱���ɵ�
 * @param	pparVector	  [out]	: ��˱���˶��ɵ� 
 * @param	nMaxCount	  [in]	: �ɸ���� 
 * @return	void
 *
 */
void CVLIB_DECLSPEC EigenJacobian(float** pparData, int nDim, float* parEigenValue, float** pparVector,
	  int nMaxCount);

/**
 * @brief   nDim*2�Ե�̩ ��˱���� ��˱���˶��� ���ײ� �ܺ��˳�.
 * 
 * @param	pprData		 [in]  : �ӵ��Ե�
 * @param	nDim		 [in]  : �Ե�̩ ��̺�� 
 * @param	prEigenvalue [out] : ��˱���ɵ�
 * @param	pprVector	 [out] : ��˱���˶��ɵ� 
 * @param	nNumEigen	 [in]  : �ɸ���� 
 * @return	void
 *
 */
void CVLIB_DECLSPEC EigenPower (float** pprData, int nDim, float* prEigenvalue, float** pprVector,
			int nNumEigen);

/**
 * @brief   nDim*2�Ե�̩ ��˱���� ��˱���˶��� ���ײ� �ܺ��˳�. (64bit)
 * 
 * @param	pprData		 [in]  : �ӵ��Ե� 
 * @param	nDim		 [in]  : �Ե�̩ ��̺�� 
 * @param	prEigenvalue [out] : ��˱���ɵ� 
 * @param	pprVector	 [out] : ��˱���˶��ɵ� 
 * @param	nNumEigen	 [in]  : �ɸ���� 
 * @return	void
 *
 */
void CVLIB_DECLSPEC EigenPower_64 (double** pprData, int nDim, double* prEigenvalue, double** pprVector,
			int nNumEigen);

/**
 * @brief   �Ե�̩ �״ݰ�˱��˾ ���ײ� �ܺ��˳�. 
 * 
 * @param	pprData	 [in]  : �ӵ��Ե� 
 * @param	nDim	 [in]  : �Ե�̩ ��̺�� 
 * @param	prVector [out] : ��˱���˶��ɵ� 
 * @return	�״ݰ�˱�� 
 *
 */
float CVLIB_DECLSPEC FindMaxEigenvalue (float** pprData, int nDim, float* prVector);

/**
 * @brief   ���˶�̮ (nDim * nCols)�Ե�̩ ���ױ��� �����ٳ�. prC = T(prA) * prB
 * 
 * @param   prA   [in]  : �ӵٹ��˶�
 * @param   pprB  [in]  : �ӵ��Ե�  
 * @param   prC   [out] : �������˶� 
 * @param   nDim  [in]  : �ӵٹ��˶�̩ ��̺�� �� �Ե�̩ �Ժ� 
 * @param   nCols [in]  : �Ե�̩ �ۺ� 
 * @return  void
 *
 */
void CVLIB_DECLSPEC VecMulMat (float* prA, float** pprB, float *prC, int nDim, int nCols);

/**
 * @brief   �Ե۱� ���˶�̩ ���ױ��� �����ٳ�. 
 *
 * @param   prResult [in] : �������˶�
 * @param   pprMat   [in] : �ӵ��Ե� 
 * @param   prVec    [in] : �ӵٹ��˶�
 * @param   nDim     [in] : �Ե� �� ���˶�̩ ��̺�� 
 * @return  void
 *
 */
// void MatMulVec(float* prResult, float** pprMat, float* prVec, int nDim);

/**
 * @brief   �Ե�̩ �Եۻ�˾ ���ײ� �ܺ��˳�.
 * 
 * @param	pparMat	[in]  : �ӵ��Ե� 
 * @param	nDim	[out] :	�Ե�̩ ��̺�� 
 * @return	�Եۻ�  
 *
 */
float CVLIB_DECLSPEC Determinant (float** pparMat, int nDim);

/**
 * @brief   �Ե�̩ �״ݰ�˱��˾ ���ײ� �ܺ��˳�. 
 * 
 * @param	ppdMatrix	  [in]	: �ӵ��Ե� 
 * @param	pdEigenVector [out]	: ��˱���ɵ� 
 * @param	sN			  [in]	: �Ե�̩ ��̺�� 
 * @return	�״ݰ�˱�� 
 *
 */
double CVLIB_DECLSPEC FindingLargestEigenValue (double** ppdMatrix, double* pdEigenVector, int sN );
}

#endif //_MATLIB_H_