/*!
 * \file	Statistics.cpp
 * \ingroup math
 * \brief	ÀÒ±ËµÌË¾ °éÂ×±¨ Ì¡ÂÙ ·º´ÂË¾ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "Vec.h"
#include "Statistics.h"
#include "Matoperation.h"

namespace CVLib 
{

void MeanVector (Vec* pvecExamples, int nNum, Vec* pvecMean, Vec* pvecWeight /*= NULL*/)
{
	assert (pvecMean->Type() == MAT_Tfloat);
	assert (pvecMean->Length() == pvecExamples->Length());

	int i, j, nDim = pvecExamples->Length();
	Vec* pTemp = pvecExamples;
	float* prWeights = pvecWeight ? pvecWeight->data.fl : NULL;

	Vec		vMean(nDim, MAT_Tdouble);
	TYPE type = pvecExamples->Type();

	double	rSum = 0.0;
	pvecMean->Zero ();
	vMean.Zero();
	if (prWeights == NULL)
	{
		switch (type)
		{
		case MAT_Tfloat:
			for (i = 0; i < nNum; i ++, pTemp ++)
			{
				for (j = 0; j < nDim; j ++)
					vMean.data.db[j] += pTemp->data.fl[j];
			}
			break;
		case MAT_Tbyte:
			for (i = 0; i < nNum; i ++, pTemp ++)
			{
				for (j = 0; j < nDim; j ++)
					vMean.data.db[j] += pTemp->data.ptr[j];
			}
			break;
		}
		for (j = 0; j < nDim; j ++)
		{
			vMean.data.db[j] /= nNum;
			pvecMean->data.fl[j] = (float)vMean.data.db[j];
		}
	}
	else
	{
		switch (type)
		{
		case MAT_Tfloat:
			for (i = 0; i < nNum; i ++, pTemp ++)
			{
				for (j = 0; j < nDim; j ++)
					vMean.data.db[j] += (double)pTemp->data.fl[j] * (double)prWeights[i];
				rSum += prWeights[i];
			}
			break;
		case MAT_Tbyte:
			for (i = 0; i < nNum; i ++, pTemp ++)
			{
				for (j = 0; j < nDim; j ++)
					vMean.data.db[j] += (double)pTemp->data.ptr[j] * (double)prWeights[i];
				rSum += prWeights[i];
			}
			break;
		}
		for (j = 0; j < nDim; j ++)
		{
			vMean.data.db[j] /= rSum;
			pvecMean->data.fl[j] = (float)vMean.data.db[j];
		}
	}
	vMean.Release();
}

void Covariance (Vec* pvecExamples, int nNum, Mat* pmatCov, Vec* pvecMean, Vec* pvecWeight/* = NULL*/)
{
	int		nDim = pvecExamples->Length();
	Vec*	pMean;
	if (pvecMean == NULL)
	{
		pMean = new Vec(nDim, MAT_Tfloat);
		MeanVector (pvecExamples, nNum, pMean, pvecWeight);
	}
	else
	{
		pMean = pvecMean;
	}

	Variation (pvecExamples, nNum, pmatCov, pMean, pvecWeight);

	float rSum = 0.0f;
	if (pvecWeight)
	{
		for (int i = 0; i < nNum; i ++)
			rSum += pvecWeight->data.fl[i];
	}
	else
		rSum = (float)nNum;
	MatOp::Mul (pmatCov, 1.0f / (float)rSum, pmatCov);

	if (pvecMean == NULL)
	{
		pMean->Release();
		delete pMean;
	}
}

void Variation (Vec* pvecExamples, int nNum, Mat* pmatVar, Vec* pvecMean, Vec* pvecWeight/* = NULL*/)
{
	int		nDim = pvecExamples->Length();
	Vec*	pMean;
	if (pvecMean == NULL)
	{
		pMean = new Vec(nDim, MAT_Tfloat);
		MeanVector (pvecExamples, nNum, pMean, pvecWeight);
	}
	else
	{
		pMean = pvecMean;
	}
	int		i, j;
	Vec*	pTemp;
	Vec		vecExample;
//	Mat		matM;
	TYPE	type = pvecExamples->Type();
	vecExample.Create (nDim, MAT_Tfloat);
	float* prExample = vecExample.data.fl;
	float* prWeights = pvecWeight ? pvecWeight->data.fl : NULL;
//	matM.Create (nDim, nDim, MAT_Tfloat);
	pmatVar->Zero();
	for (i = 0; i < nNum; i ++)
	{
		pTemp = pvecExamples + i;
		switch (type)
		{
		case MAT_Tfloat:
			for (j = 0; j < nDim; j ++)
				prExample[j] = pTemp->data.fl[j] - pMean->data.fl[j];
			break;
		case MAT_Tbyte:
			for (j = 0; j < nDim; j ++)
				prExample[j] = pTemp->data.ptr[j] - pMean->data.fl[j];
			break;
		}

//		matM.Zero();
		switch (pmatVar->Type())
		{
		case MAT_Tfloat:
			{
				float** pprVar = pmatVar->data.fl;
				if (prWeights)
				{
					for (int k = 0; k < nDim; k ++)
					{
						for (int j = 0; j < nDim; j ++)
						{
							pprVar[k][j] += prExample[k] * prExample[j] * prWeights[i];
						}
					}
				}
				else
				{
					for (int k = 0; k < nDim; k ++)
					{
						for (int j = 0; j < nDim; j ++)
						{
							pprVar[k][j] += prExample[k] * prExample[j];
						}
					}
				}
			}
			break;
		case MAT_Tdouble:
			{
				double** pprVar = pmatVar->data.db;
				if (prWeights)
				{
					for (int k = 0; k < nDim; k ++)
					{
						for (int j = 0; j < nDim; j ++)
						{
							pprVar[k][j] += prExample[k] * prExample[j] * prWeights[i];
						}
					}
				}
				else
				{
					for (int k = 0; k < nDim; k ++)
					{
						for (int j = 0; j < nDim; j ++)
						{
							pprVar[k][j] += prExample[k] * prExample[j];
						}
					}
				}
			}
			break;
		default:
			{
				assert (FALSE);
			}
		}
//		VecMulTrVec (&vecExample, &vecExample, &matM);
//		if (pvecWeight)
//			RealMulMat (pvecWeight->data.fl[i], &matM, &matM);
//		MatOp::Add (pmatVar, &matM, pmatVar);
	}

//	matM.Release();
	vecExample.Release();

	if (pvecMean == NULL)
		ReleaseVec(pMean);
}

}
