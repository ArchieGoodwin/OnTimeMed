
/*!
 * \file
 * \brief	ººÂô¹¤µù±¨¶¦ ÃªµÚ»¤¿Ô±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "RegressorTrainer.h"
#include "Regressor.h"
#include "cvlibbase/inc/Matoperation.h"
#include "cvlibbase/inc/LUDecomposition.h"

namespace CVLib
{
//////////////////////////////////////////////////////////////////////////
void RegressorTrainerABC::SetData(DataSet* pData)
{
	Mat mExample(pData->GetCount(), pData->m_nDim, MAT_Tdouble);
	Vec vOvj(pData->GetCount(), MAT_Tdouble);
	for (int i=0; i<pData->GetCount(); i++)
	{
		for (int j=0; j<pData->m_nDim; j++)
			mExample.data.db[i][j] = pData->m_pprData[i][j];
		vOvj.data.db[i]=pData->m_prCls[i];
	}
	SetData (&mExample, &vOvj);
}

//////////////////////////////////////////////////////////////////////////
LinearRegressorTrainer::~LinearRegressorTrainer()
{
	Release();
}

void LinearRegressorTrainer::SetData (Mat* pmExamples, Vec* pvObj)
{
	m_pmData = new Mat(pmExamples->Rows(), pmExamples->Cols() + 1, MAT_Tdouble);
	for(int i = 0; i < pmExamples->Rows(); i ++)
	{
		int j;
		for (j = 0; j < pmExamples->Cols(); j ++)
			m_pmData->data.db[i][j] = pmExamples->data.db[i][j];
		m_pmData->data.db[i][j] = 1;
	}
	m_pvObj = new Vec(pmExamples->Rows(), MAT_Tdouble);
	memcpy (m_pvObj->data.db, pvObj->data.db, sizeof(double)*pmExamples->Rows());
}

int	LinearRegressorTrainer::Train (ClassifierABC* pMachine)
{
	LinearRegressor* pRegressor = (LinearRegressor*)pMachine;

	int i;

	Mat mB(m_pmData->Cols(), m_pmData->Cols(), MAT_Tdouble);
	MatOp::TrAA(&mB, m_pmData);
	LUDecomposition lu(&mB);
	
	Mat mI(m_pmData->Cols(), m_pmData->Cols(), MAT_Tdouble);
	mI.Zero();
	for (i = 0; i < mI.Rows(); i ++)
		mI.data.db[i][i] = 1.0;
	Mat* pmInverse = lu.Solve (&mI);
	if (pmInverse==0)
		return 0;
	
	Mat mTemp(m_pmData->Cols(), m_pmData->Rows(), MAT_Tdouble);
	Mat mTr(m_pmData->Cols(), m_pmData->Rows(), MAT_Tdouble);
	MatOp::Transpose (&mTr, m_pmData);
	MatOp::Mul(&mTemp, pmInverse, &mTr);

	pRegressor->m_vCoeff.Create (m_pmData->Cols(), MAT_Tfloat);
	Vec vRet(m_pmData->Cols(), MAT_Tdouble);
	MatOp::Mul(&vRet, &mTemp, m_pvObj);
	for (i = 0; i < vRet.Length(); i ++)
		pRegressor->m_vCoeff.data.fl[i] = (float)vRet.data.db[i];
	
	ReleaseMat(pmInverse);
	mTemp.Release();
	mTr.Release();
	mI.Release();
	mB.Release();

	return 1;
}

MachineABC* LinearRegressorTrainer::Train(DataSet* pData)
{
	RegressorTrainerABC::SetData (pData);
	LinearRegressor* pRegressor=new LinearRegressor;
	if (!Train (pRegressor)){
		delete pRegressor;
		pRegressor=0;
	}
	return pRegressor;
}

void LinearRegressorTrainer::Release ()
{
	if (m_pmData)
		delete m_pmData;
	if (m_pvObj)
		delete m_pvObj;
	m_pmData = NULL;
	m_pvObj = NULL;
}

//////////////////////////////////////////////////////////////////////////
void QuadraticRegressorTrainer::SetData (Mat* pmExamples, Vec* pvObj)
{
	int nInNum = pmExamples->Cols();
	int nConNum = nInNum * 2 + nInNum*(nInNum-1) / 2 + 1;
	
	m_pmData = new Mat(pmExamples->Rows(), nConNum, MAT_Tdouble);
	int i, j;
	for (i = 0; i < pmExamples->Rows(); i ++)
	{
		Vec vTemp(pmExamples->data.db[i], nInNum, MAT_Tdouble);
		Vec* pvConverted = QuadraticRegressor::iConvert(&vTemp);
		for (j = 0; j < nConNum-1; j ++)
			m_pmData->data.db[i][j] = pvConverted->data.db[j];
		m_pmData->data.db[i][j] = 1;
	}
	m_pvObj = new Vec(pmExamples->Rows(), MAT_Tdouble);
	memcpy (m_pvObj->data.db, pvObj->data.db, sizeof(double)*pmExamples->Rows());
}

MachineABC* QuadraticRegressorTrainer::Train(DataSet* pData)
{
	RegressorTrainerABC::SetData (pData);
	QuadraticRegressor* pRegressor=new QuadraticRegressor;
	if (!LinearRegressorTrainer::Train (pRegressor))
	{
		delete pRegressor; pRegressor=0;
	}
	return pRegressor;
}
//////////////////////////////////////////////////////////////////////////

MachineABC* LeastSquareTrainer::Train(DataSet* pData)
{
	LeastSquareMachine* pMachine=new LeastSquareMachine;

	Matd* m_pmData=new Matd((double**)pData->m_pprData, pData->m_nCount, pData->m_nDim);
	Vecd* m_pvObj=new Vecd((double*)pData->m_prCls, pData->m_nCount);
	
	Mat mB(m_pmData->Cols(), m_pmData->Cols(), MAT_Tdouble);
	MatOp::TrAA(&mB, m_pmData);
	mB.Invert();
		
	Mat mTemp(m_pmData->Cols(), m_pmData->Rows(), MAT_Tdouble);
	Mat mTr(m_pmData->Cols(), m_pmData->Rows(), MAT_Tdouble);
	MatOp::Transpose (&mTr, m_pmData);
	MatOp::Mul(&mTemp, &mB, &mTr);
	
	pMachine->m_vCoeff.Create (m_pmData->Cols(), MAT_Tfloat);
	Vec vRet(m_pmData->Cols(), MAT_Tdouble);
	MatOp::Mul(&vRet, &mTemp, m_pvObj);
	for (int i = 0; i < vRet.Length(); i ++)
		pMachine->m_vCoeff.data.fl[i] = (float)vRet.data.db[i];
	delete m_pmData;
	delete m_pvObj;
	return pMachine;
}

}