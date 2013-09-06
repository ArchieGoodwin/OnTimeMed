
/*!
 * \file
 * \brief	¹¤µù±¨?E´ÝÂÙ ±¨½ó¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "Regressor.h"

namespace CVLib 
{

static Vec m_vConvert;
LinearRegressor::LinearRegressor()
{
}

LinearRegressor::~LinearRegressor()
{
	Release();
}

void	LinearRegressor::Release ()
{
	m_vCoeff.Release();
}

float	LinearRegressor::Eval(Vec* pvFea)
{
	int i;
	float rRet = 0.0f;
	if (pvFea->Type() == MAT_Tfloat)
	{
		for (i = 0; i < pvFea->Length(); i ++)
			rRet += pvFea->data.fl[i] * m_vCoeff.data.fl[i];
		rRet += m_vCoeff.data.fl[i];
	}
	else if (pvFea->Type() == MAT_Tdouble)
	{
		for (i = 0; i < pvFea->Length(); i ++)
			rRet += (float)pvFea->data.db[i] * m_vCoeff.data.fl[i];
		rRet += m_vCoeff.data.fl[i];
	}
	return rRet;
}

float LinearRegressor::GetConstCoeff()
{
	return m_vCoeff.data.fl[m_vCoeff.Length()-1];
}
void LinearRegressor::SetConstCoeff(float rConf)
{
	m_vCoeff.data.fl[m_vCoeff.Length()-1]=rConf;
}
int	LinearRegressor::FromFile (FILE* pFile)
{
	return m_vCoeff.FromFile(pFile);
}
int	LinearRegressor::ToFile (FILE* pFile) const
{
	return m_vCoeff.ToFile(pFile);
}


//////////////////////////////////////////////////////////////////////////

void	QuadraticRegressor::Release ()
{
	m_vCoeff.Release();
	m_vConvert.Release();
}

float	QuadraticRegressor::Eval(Vec* pvFea)
{
	int i;
	float rRet = 0.0f;
	Vec* pvConvert = iConvert(pvFea);
	if (pvConvert->Type() == MAT_Tfloat)
	{
		for (i = 0; i < pvConvert->Length(); i ++)
			rRet += pvConvert->data.fl[i] * m_vCoeff.data.fl[i];
		rRet += m_vCoeff.data.fl[i];
	}
	else if (pvConvert->Type() == MAT_Tdouble)
	{
		for (i = 0; i < pvConvert->Length(); i ++)
			rRet += (float)pvConvert->data.db[i] * m_vCoeff.data.fl[i];
		rRet += m_vCoeff.data.fl[i];
	}
	return rRet;
}

Vec* QuadraticRegressor::iConvert (Vec* pvFea)
{
	int nNum = pvFea->Length();

	if (pvFea->Type() == MAT_Tdouble)
	{
		if (! (m_vConvert.IsValid() && m_vConvert.Type() == MAT_Tdouble) )
		{
			m_vConvert.Release();
			m_vConvert.Create (nNum * 2 + nNum*(nNum-1) / 2, MAT_Tdouble);
		}
		
		double* prI = pvFea->data.db;
		double* prA = m_vConvert.data.db;
		int i, j;
		int iFea = 0;
		for (i = 0; i < nNum; i ++)
		{
			prA[iFea] = prI[i] * prI[i];
			iFea ++;
		}
		for (i = 0; i < nNum; i ++)
			for (j = i+1; j < nNum; j ++)
			{
				prA[iFea] = prI[i] * prI[j];
				iFea ++;
			}
			for (i = 0; i < nNum; i ++)
			{
				prA[iFea] = prI[i];
				iFea ++;
			}
	}
	else if (pvFea->Type() == MAT_Tfloat)
	{
		if (! (m_vConvert.IsValid() && m_vConvert.Type() == MAT_Tfloat) )
		{
			m_vConvert.Release();
			m_vConvert.Create (nNum * 2 + nNum*(nNum-1) / 2, MAT_Tfloat);
		}
		
		float* prI = pvFea->data.fl;
		float* prA = m_vConvert.data.fl;
		int i, j;
		int iFea = 0;
		for (i = 0; i < nNum; i ++)
		{
			prA[iFea] = prI[i] * prI[i];
			iFea ++;
		}
		for (i = 0; i < nNum; i ++)
			for (j = i+1; j < nNum; j ++)
			{
				prA[iFea] = prI[i] * prI[j];
				iFea ++;
			}
			for (i = 0; i < nNum; i ++)
			{
				prA[iFea] = prI[i];
				iFea ++;
			}
	}

	return &m_vConvert;
}

float	LeastSquareMachine::Eval(Vec* pvFea)
{
	int i;
	float rRet = 0.0f;
	if (pvFea->Type() == MAT_Tfloat)
	{
		for (i = 0; i < pvFea->Length(); i ++)
			rRet += pvFea->data.fl[i] * m_vCoeff.data.fl[i];
	}
	else if (pvFea->Type() == MAT_Tdouble)
	{
		for (i = 0; i < pvFea->Length(); i ++)
			rRet += (float)pvFea->data.db[i] * m_vCoeff.data.fl[i];
	}
	return rRet;
}

}