#include "DataSet.h"
#include "cvlibbase/inc/cvlibMalloc.h"

namespace CVLib{

DataSet::DataSet()
{
	Init();
}

DataSet::DataSet (int nDim, int nCount, int nMode/*= DS_ALL*/)
{
	Init ();
	Create (nDim, nCount, nMode);
}

DataSet::DataSet (const DataSet& other)
{
	CopyFrom (other);
}

DataSet::DataSet (const char* szFilename)
{
	Init();

	FILE* pFile = fopen(szFilename, "rb");
	if (pFile)
	{
		FromFile(pFile);
		fclose (pFile);
	}
}

DataSet::~DataSet()
{
	Release();
}

void	DataSet::Init ()
{
	m_nFlagCreate = 0;

	m_nMaxCount = 0;
	m_nCount = 0;
	m_nDim = 0;
	m_prCls = NULL;
	m_pprData = NULL;
	m_prWeights = NULL;
	m_nGrowBy = 0;
	m_nMode = DS_ALL;
}

int	DataSet::FromFile (FILE* pFile)
{
	int nReadNum;
	nReadNum = fread (&m_nDim, sizeof(m_nDim), 1, pFile);
	if (nReadNum == 0)	return 0;
	nReadNum = fread (&m_nCount, sizeof(m_nCount), 1, pFile);
	if (nReadNum == 0)  return 0;
	m_nMaxCount = m_nCount;
	m_nMode = DS_ALL;

	Create (m_nDim, m_nCount);
	nReadNum = fread (m_prCls, sizeof(double), m_nCount, pFile);
	if (nReadNum < m_nCount)
		return 0;
	for (int i = 0; i < m_nMaxCount; i ++)
	{
		nReadNum = fread (m_pprData[i], sizeof(double), m_nDim, pFile);
// 		if (nReadNum < m_nDim)
// 			return 0;
	}

	return 1;
}

int	DataSet::FromFile (XFile* pFile)
{
	int nReadNum;
	nReadNum = pFile->Read(&m_nDim, sizeof(m_nDim), 1);
	if (nReadNum == 0)	return 0;
	nReadNum = pFile->Read(&m_nCount, sizeof(m_nCount), 1);
	if (nReadNum == 0)  return 0;
	m_nMaxCount = m_nCount;
	m_nMode = DS_ALL;
	
	Create (m_nDim, m_nCount);
	nReadNum = pFile->Read(m_prCls, sizeof(double), m_nCount);
	if (nReadNum < m_nCount)
		return 0;
	for (int i = 0; i < m_nMaxCount; i ++)
	{
		nReadNum = pFile->Read(m_pprData[i], sizeof(double), m_nDim);
		// 		if (nReadNum < m_nDim)
		// 			return 0;
	}
	
	return 1;
}

int	DataSet::ToFile (FILE* pFile) const
{
	int nWriteNum;
	nWriteNum = fwrite (&m_nDim, sizeof(m_nDim), 1, pFile);
	if (nWriteNum == 0)	return 0;
	nWriteNum = fwrite (&m_nCount, sizeof(m_nCount), 1, pFile);
	if (nWriteNum == 0)  return 0;

	nWriteNum = fwrite (m_prCls, sizeof(double), m_nCount, pFile);
	if (nWriteNum < m_nCount)
		return 0;
	for (int i = 0; i < m_nMaxCount; i ++)
	{
		nWriteNum = fwrite (m_pprData[i], sizeof(double), m_nDim, pFile);
// 		if (nWriteNum < m_nDim)
// 			return 0;
	}
	return 1;
}

int	DataSet::ToFile (XFile* pFile) const
{
	int nWriteNum;
	nWriteNum = pFile->Write(&m_nDim, sizeof(m_nDim), 1);
	if (nWriteNum == 0)	return 0;
	nWriteNum = pFile->Write (&m_nCount, sizeof(m_nCount), 1);
	if (nWriteNum == 0)  return 0;
	
	nWriteNum = pFile->Write(m_prCls, sizeof(double), m_nCount);
	if (nWriteNum < m_nCount)
		return 0;
	for (int i = 0; i < m_nMaxCount; i ++)
	{
		nWriteNum = pFile->Write(m_pprData[i], sizeof(double), m_nDim);
		// 		if (nWriteNum < m_nDim)
		// 			return 0;
	}
	return 1;
}

int	DataSet::Create (int nDim, int nCount, int nMode/*= DS_ALL*/)
{
	m_nMaxCount = nCount;
	m_nCount = m_nMaxCount;
	m_nDim = nDim;
	m_nMode = nMode;
	CALLOC (m_prCls, m_nMaxCount, double);
	CALLOC (m_prWeights, m_nMaxCount, double);
	CALLOC (m_pprData, m_nMaxCount, double*);
	if (m_nMode == DS_ALL)
	{
		for (int i = 0; i < m_nMaxCount; i ++)
			CALLOC (m_pprData[i], m_nDim, double);
	}
	m_nFlagCreate = 1;
	return 1;
}

int	DataSet::CopyFrom (const DataSet& other)
{
	if (!Create (other.m_nDim, other.m_nCount, DS_ALL))
		return 0;
	memcpy (m_prCls, other.m_prCls, sizeof(double)*m_nCount);
	memcpy (m_prWeights, other.m_prWeights, sizeof(double)*m_nCount);
 	for (int i = 0; i < m_nCount; i ++)
 		memcpy (m_pprData[i], other.m_pprData[i], sizeof(double)*m_nDim);
	return 1;
}

void DataSet::Release ()
{
	if (m_nMaxCount == 0 && m_nDim == 0)
		return;

	if (m_nMode == DS_ALL)
	{
		for (int i = m_nMaxCount - 1; i >= 0; i --)
			FREE (m_pprData[i]);
	}
	FREE (m_pprData);
	FREE (m_prCls);
	FREE (m_prWeights);

	m_nFlagCreate = 0;
	Init ();
}

int	DataSet::GetSampleNum (int nClass)
{
	int nCount = 0;
	for (int i = 0; i < m_nCount; i ++)
	{
		if (fabs(m_prCls[i] - nClass) < 1e-7)
			nCount ++;
	}
	return nCount;
}

int DataSet::GetClassNum()
{
	int		nMaxClassNum = 16;
	int nClassNum = 0;
	int* pnLabel;
	MALLOC (pnLabel, sizeof(int) * nMaxClassNum, int);
	int i;
	for (i = 0; i < m_nCount; i ++)
	{
		int nThisLabel = (int)m_prCls[i];
		int j;
		for (j = 0; j < nClassNum; j ++)
			if (nThisLabel == pnLabel[j])
				break;
		if (j == nClassNum)
		{
			pnLabel[nClassNum] = nThisLabel;
			nClassNum ++;
		}
	}
	FREE (pnLabel);
	return nClassNum;
}

int DataSet::Statistics(double* prMin, double* prMax, double* prMean)
{
	double* prTMin=new double[m_nDim];
	double* prTMax=new double[m_nDim];
	double* prTMean=new double[m_nDim];
	for (int id=0; id<m_nDim; id++)
	{
		prTMin[id]=1e10;
		prTMax[id]=-1e10;
		prTMean[id]=0;
		for (int i=0; i<m_nCount; i++)
		{
			prTMean[id] += m_pprData[i][id];
			prTMin[id] = MIN(prTMin[id], m_pprData[i][id]);
			prTMax[id] = MAX(prTMax[id], m_pprData[i][id]);
		}
		prTMean[id] /= m_nCount;
	}
	if (prMin)
		memcpy (prMin, prTMin, sizeof(double)*m_nDim);
	if (prMax)
		memcpy (prMax, prTMax, sizeof(double)*m_nDim);
	if (prMean)
		memcpy (prMean, prTMean, sizeof(double)*m_nDim);
	delete []prTMin;
	delete []prTMax;
	delete []prTMean;
	return 1;
}

int	DataSet::Add (double* prData, double rCls, double rWei/*=0.0*/)
{
	if (m_nCount == m_nMaxCount)
	{
		if (m_nMode == DS_ALL)
		{
			DataSet newData(*this);
			Release();
			if (m_nGrowBy==0)
			{
				m_nGrowBy = m_nCount/8;
				m_nGrowBy = (m_nGrowBy < 4) ? 4 : ((m_nGrowBy > 1024) ? 1024 : m_nGrowBy);
			}
			int nNewMaxCount = newData.GetMaxCount() + m_nGrowBy;

			Create (newData.m_nDim, nNewMaxCount, DS_ALL);
			memcpy (m_prCls, newData.m_prCls, sizeof(m_prCls[0])*newData.GetCount());
			memcpy (m_prWeights, newData.m_prWeights, sizeof(m_prWeights[0])*newData.GetCount());
			for (int i=0; i<newData.GetCount(); i++)
				memcpy (m_pprData[i], newData.m_pprData[i], sizeof(double)*m_nDim);
			m_nCount=newData.GetCount();
			newData.Release();
		}
		else if (m_nMode == DS_ADDRESS)
		{
			int nDim = m_nDim;
			int nCount=m_nCount;
			double** pprTempData = new double*[m_nCount];
			double* prTempCls = new double[m_nCount];
			double* prTempWei = new double[m_nCount];
			memcpy (prTempCls, m_prCls, sizeof(m_prCls[0])*m_nCount);
			memcpy (prTempWei, m_prWeights, sizeof(m_prWeights[0])*m_nCount);
			memcpy (pprTempData, m_pprData, sizeof(double*)*m_nCount);

			Release();
			if (m_nGrowBy==0)
			{
				int m_nGrowBy = (nCount+1)/8;
				m_nGrowBy = (nCount < 4) ? 4 : ((m_nGrowBy > 1024) ? 1024 : m_nGrowBy);
			}
			int nNewMaxCount = nCount + m_nGrowBy;
			Create (nDim, nNewMaxCount, DS_ALL);
			
			memcpy (m_prCls, prTempCls, sizeof(m_prCls[0])*nCount);
			memcpy (m_prWeights, prTempWei, sizeof(m_prWeights[0])*nCount);
			for (int i=0; i<nCount; i++)
				m_pprData[i]=pprTempData[i];
			m_nCount=nCount;
		}
	}
	m_prCls[m_nCount] = rCls;
	m_prWeights[m_nCount] = rWei;
	if (m_nMode == DS_ALL)
		memcpy (m_pprData[m_nCount], prData, sizeof(double)*m_nDim);
	else if (m_nMode == DS_ADDRESS)
		m_pprData[m_nCount] = prData;
	m_nCount ++;
	return m_nCount;
}

void DataSet::SetData (int nIdx, double* prData, double rCls, double rWei/*=0.0*/)
{
	memcpy (m_pprData[nIdx], prData, sizeof(double)*m_nDim);
	m_prCls[nIdx] = rCls;
	m_prWeights[nIdx] = rWei;
}

Matd* DataSet::ToMat()
{
	return new Matd(m_pprData, m_nCount, m_nDim);
}

void DataSet::FromMat(Matd* pM)
{
	for (int i=0; i<m_nCount; i++)
	{
		for (int ic=0; ic<m_nDim; ic++)
			m_pprData[i][ic] = pM->data.db[i][ic];
	}
}

void DataSet::SortClsIdx()
{
	DataSet temp(*this);
}

int	DataSet::FromFile (const char* szFilename)
{
	return Object::FromFile(szFilename);
}
int	DataSet::ToFile (const char* szFilename) const
{
 	return Object::ToFile(szFilename);
}

}