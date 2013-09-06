/*!
 * \file	MatLib.h
 * \ingroup base
 * \brief	ÃÔµÛÌ© °ÖË±°ª±á °ÖË±¹ÖÀË¶£ ´Ğ Êàº¤Ë¾ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _MATLIB_H_
#define _MATLIB_H_

#include "cvlibbaseDef.h"

namespace CVLib
{
/** 
 * @brief   ÊßÃÔµÛË¾ ÊĞ²÷ ÂÜºãËË³Ş. (floatÂô¸êºãÃÔµÛ)
 *
 * @param	nDim      [in]  : ÃÔµÛÌ© ½ÓÌººã 
 * @param	ppdMatrix [out]	: ÃÔµÛ ( In ) / ÊßÃÔµÛ ( Out )
 * @return	ºÂ°áËË·² true
 * 
 */
int CVLIB_DECLSPEC ComputeInverseMatrix ( int nDim, float** ppdMatrix );

/** 
 * @brief   ÊßÃÔµÛË¾ ÊĞ²÷ ÂÜºãËË³Ş. (doubleÂô¸êºãÃÔµÛ)
 *
 * @param	nDim      [in]  : ÃÔµÛÌ© ½ÓÌººã 
 * @param	ppdMatrix [out]	: ÃÔµÛ ( In ) / ÊßÃÔµÛ ( Out )
 * @return	ºÂ°áËË·² true
 * 
 */
int CVLIB_DECLSPEC ComputeInverseMatrix ( int nDim, double** ppdMatrix );

/**
 * @brief   nDim*2ÃÔµÛÌ© °ÖË±°ª±á °ÖË±¹ÖÀË¶£¶¦ °éÂ×²÷ ÂÜºãËË³Ş.
 * 
 * @param	pparData	  [in]	: ËÓµÙÃÔµÛ
 * @param	nDim		  [in]	: ÃÔµÛÌ© ½ÓÌººã 
 * @param	parEigenValue [out]	: °ÖË±°ª¹ÉµÛ
 * @param	pparVector	  [out]	: °ÖË±¹ÖÀË¶£¹ÉµÛ 
 * @param	nMaxCount	  [in]	: ¸É¸ôÃäºã 
 * @return	void
 *
 */
void CVLIB_DECLSPEC EigenJacobian(float** pparData, int nDim, float* parEigenValue, float** pparVector,
	  int nMaxCount);

/**
 * @brief   nDim*2ÃÔµÛÌ© °ÖË±°ª±á °ÖË±¹ÖÀË¶£¶¦ °éÂ×²÷ ÂÜºãËË³Ş.
 * 
 * @param	pprData		 [in]  : ËÓµÙÃÔµÛ
 * @param	nDim		 [in]  : ÃÔµÛÌ© ½ÓÌººã 
 * @param	prEigenvalue [out] : °ÖË±°ª¹ÉµÛ
 * @param	pprVector	 [out] : °ÖË±¹ÖÀË¶£¹ÉµÛ 
 * @param	nNumEigen	 [in]  : ¸É¸ôÃäºã 
 * @return	void
 *
 */
void CVLIB_DECLSPEC EigenPower (float** pprData, int nDim, float* prEigenvalue, float** pprVector,
			int nNumEigen);

/**
 * @brief   nDim*2ÃÔµÛÌ© °ÖË±°ª±á °ÖË±¹ÖÀË¶£¶¦ °éÂ×²÷ ÂÜºãËË³Ş. (64bit)
 * 
 * @param	pprData		 [in]  : ËÓµÙÃÔµÛ 
 * @param	nDim		 [in]  : ÃÔµÛÌ© ½ÓÌººã 
 * @param	prEigenvalue [out] : °ÖË±°ª¹ÉµÛ 
 * @param	pprVector	 [out] : °ÖË±¹ÖÀË¶£¹ÉµÛ 
 * @param	nNumEigen	 [in]  : ¸É¸ôÃäºã 
 * @return	void
 *
 */
void CVLIB_DECLSPEC EigenPower_64 (double** pprData, int nDim, double* prEigenvalue, double** pprVector,
			int nNumEigen);

/**
 * @brief   ÃÔµÛÌ© ¾×´İ°ÖË±°ªË¾ °éÂ×²÷ ÂÜºãËË³Ş. 
 * 
 * @param	pprData	 [in]  : ËÓµÙÃÔµÛ 
 * @param	nDim	 [in]  : ÃÔµÛÌ© ½ÓÌººã 
 * @param	prVector [out] : °ÖË±¹ÖÀË¶£¹ÉµÛ 
 * @return	¾×´İ°ÖË±°ª 
 *
 */
float CVLIB_DECLSPEC FindMaxEigenvalue (float** pprData, int nDim, float* prVector);

/**
 * @brief   ¹ÖÀË¶£Ì® (nDim * nCols)ÃÔµÛÌ© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. prC = T(prA) * prB
 * 
 * @param   prA   [in]  : ËÓµÙ¹ÖÀË¶£
 * @param   pprB  [in]  : ËÓµÙÃÔµÛ  
 * @param   prC   [out] : ·»¼¬¹ÖÀË¶£ 
 * @param   nDim  [in]  : ËÓµÙ¹ÖÀË¶£Ì© ½ÓÌººã ·ñ ÃÔµÛÌ© ÃÔºã 
 * @param   nCols [in]  : ÃÔµÛÌ© µÛºã 
 * @return  void
 *
 */
void CVLIB_DECLSPEC VecMulMat (float* prA, float** pprB, float *prC, int nDim, int nCols);

/**
 * @brief   ÃÔµÛ±á ¹ÖÀË¶£Ì© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. 
 *
 * @param   prResult [in] : ·»¼¬¹ÖÀË¶£
 * @param   pprMat   [in] : ËÓµÙÃÔµÛ 
 * @param   prVec    [in] : ËÓµÙ¹ÖÀË¶£
 * @param   nDim     [in] : ÃÔµÛ ·ñ ¹ÖÀË¶£Ì© ½ÓÌººã 
 * @return  void
 *
 */
// void MatMulVec(float* prResult, float** pprMat, float* prVec, int nDim);

/**
 * @brief   ÃÔµÛÌ© ÃÔµÛ»¥Ë¾ °éÂ×²÷ ÂÜºãËË³Ş.
 * 
 * @param	pparMat	[in]  : ËÓµÙÃÔµÛ 
 * @param	nDim	[out] :	ÃÔµÛÌ© ½ÓÌººã 
 * @return	ÃÔµÛ»¥  
 *
 */
float CVLIB_DECLSPEC Determinant (float** pparMat, int nDim);

/**
 * @brief   ÃÔµÛÌ© ¾×´İ°ÖË±°ªË¾ °éÂ×²÷ ÂÜºãËË³Ş. 
 * 
 * @param	ppdMatrix	  [in]	: ËÓµÙÃÔµÛ 
 * @param	pdEigenVector [out]	: °ÖË±°ª¹ÉµÛ 
 * @param	sN			  [in]	: ÃÔµÛÌ© ½ÓÌººã 
 * @return	¾×´İ°ÖË±°ª 
 *
 */
double CVLIB_DECLSPEC FindingLargestEigenValue (double** ppdMatrix, double* pdEigenVector, int sN );
}

#endif //_MATLIB_H_