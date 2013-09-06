/*!
 * \file	ipFilter.cpp
 * \ingroup ipCore
 * \brief   Ãùº¬µØÁÄ¶¦ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#include "ipFilter.h"
#include "cvlibbase/inc/_cvlibbase.h"

namespace CVLib
{

IMPLEMENT_PUMP(ipFilter, ipCorePump)

ipFilter::ipFilter(const ipFilter& from)
{
	if (from.m_pnKernel)
	{
		m_pnKernel = new int[from.m_nKSize*from.m_nKSize];
		memcpy(m_pnKernel, from.m_pnKernel, sizeof(int) * from.m_nKSize*from.m_nKSize);
	}
	else
		m_pnKernel=NULL;

	if (from.m_prKernel)
	{
		m_prKernel = new float[from.m_nKSize*from.m_nKSize];
		memcpy(m_prKernel, from.m_prKernel, sizeof(int) * from.m_nKSize*from.m_nKSize);
	}
	else
		m_prKernel=NULL;

	m_nKSize = from.m_nKSize;
	m_nKFactor = from.m_nKFactor;
	m_rKFactor = from.m_rKFactor;
	m_nKOffset = from.m_nKOffset;
	m_filterType=from.m_filterType;
	m_fcreate=true;
}

ipFilter::ipFilter()
{
	m_filterType=MAT;
	m_pnKernel = NULL;
	m_prKernel = NULL;
	m_nKSize = 0;
	m_nKFactor = 0;
	m_rKFactor = 0;
	m_nKOffset = 0;
	m_fcreate=false;
}

ipFilter::ipFilter(int* pnKernel, int nKsize, int nKFactor, int nKOffset, filterintype type)
{
	m_fcreate=false;
	Create (pnKernel, nKsize, nKFactor, nKOffset, type);
}

ipFilter::ipFilter(float* prKernel, int nKsize, float rKFactor, int nKOffset, filterintype type)
{
	m_fcreate=false;
	Create (prKernel, nKsize, rKFactor, nKOffset, type);
}

ipFilter::ipFilter(double* prKernel, int nKsize, double rKFactor, int nKOffset, filterintype type)
{
	m_fcreate=false;
	Create (prKernel, nKsize, rKFactor, nKOffset, type);
}

int ipFilter::Create(int* pnKernel, int nKsize, int nKFactor, int nKOffset, filterintype type)
{
	if (!m_fcreate)
	{
		m_fcreate=true;
		m_filterType=type;
		if (m_filterType == MAT)
		{
			m_pnKernel = new int[nKsize*nKsize];
			memcpy(m_pnKernel, pnKernel, sizeof(int) * nKsize*nKsize);
		}
		else if (m_filterType == VECRow || m_filterType == VECCol || m_filterType == VEC)
		{
			m_pnKernel = new int[nKsize];
			memcpy(m_pnKernel, pnKernel, sizeof(int) * nKsize);
		}
		m_prKernel = NULL;
		m_nKSize = nKsize;
		m_nKFactor = nKFactor;
		m_rKFactor = 0;
		m_nKOffset = nKOffset;
		if (m_nKFactor == 0)
			m_nKFactor = 1;
		return 1;
	}
	return 0;
}
int ipFilter::Create(float* prKernel, int nKsize, float rKFactor, int nKOffset, filterintype type)
{
	if (!m_fcreate)
	{
		m_fcreate=true;
		m_filterType=type;
		m_pnKernel = NULL;

		if (m_filterType == MAT)
		{
			m_prKernel = new float[nKsize*nKsize];
			memcpy(m_prKernel, prKernel, sizeof(float)*nKsize*nKsize);
		}
		else if (m_filterType == VECRow || m_filterType == VECCol || m_filterType == VEC)
		{
			m_prKernel = new float[nKsize];
			memcpy(m_prKernel, prKernel, sizeof(float)*nKsize);
		}
		m_nKSize = nKsize;
		m_nKFactor = 0;
		m_rKFactor = rKFactor;
		m_nKOffset = nKOffset;
		if (m_rKFactor == 0)
			m_rKFactor = 1.0f;
		return 1;
	}
	return 0;
}
int ipFilter::Create(double* prKernel, int nKsize, double rKFactor, int nKOffset, filterintype type)
{
	if (!m_fcreate)
	{
		m_fcreate=true;
		m_filterType=type;
		m_pnKernel = NULL;

		if (m_filterType == MAT)
		{
			m_prKernel = new float[nKsize*nKsize];
			for (int i=0;i<nKsize*nKsize; i++)
				m_prKernel[i]=(float)prKernel[i];
		}
		else if (m_filterType == VECRow || m_filterType == VECCol || m_filterType == VEC)
		{
			m_prKernel = new float[nKsize];
			memcpy(m_prKernel, prKernel, sizeof(float)*nKsize);
			for (int i=0;i<nKsize; i++)
				m_prKernel[i]=(float)prKernel[i];
		}
		m_nKSize = nKsize;
		m_nKFactor = 0;
		m_rKFactor = (float)rKFactor;
		m_nKOffset = nKOffset;
		if (m_rKFactor == 0)
			m_rKFactor = 1.0f;
		return 1;
	}
	return 0;
}

void ipFilter::Release ()
{
	if (m_fcreate)
	{
		if (m_pnKernel)
			delete []m_pnKernel;
		m_pnKernel = NULL;
		if (m_prKernel)
			delete []m_prKernel;
		m_prKernel = NULL;
		m_fcreate=false;
	}
}

int	ipFilter::KernelFactor(int* pnKernel, int nLen)
{
	int nRet = 0;
	for (int ii = 0; ii < nLen; ii ++)
		nRet += pnKernel[ii];
	return nRet;
}

float ipFilter::KernelFactor(float* prKernel, int nLen)
{
	float rRet = 0.0f;
	for (int ii = 0; ii < nLen; ii ++)
		rRet += prKernel[ii];
	return rRet;
}

double ipFilter::KernelFactor(double* prKernel, int nLen)
{
	double rRet = 0.0f;
	for (int ii = 0; ii < nLen; ii ++)
		rRet += prKernel[ii];
	return rRet;
}

bool ipFilter::iProcessMat (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (m_pnKernel == NULL)
		return false;
	if (pmDst && !EqualTypeSize(pmSrc, pmDst))
	{
		pmDst->Release();
		pmDst->Create (*pmSrc, FALSE);
	}

	int k2 = m_nKSize/2;
	int kmax= m_nKSize-k2;
	
	int xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmSrc->Cols();		// width
	ymax = pmSrc->Rows();		// height			
	
	int iCount;

	switch(pmSrc->Type())
	{
	case MAT_Tbyte:
		{
			int b,i;
			BYTE**	ppbSrc;
			short**	ppsTemp;
			BYTE**	ppbDst;
			
			Mat matTemp(pmSrc->Rows(), pmSrc->Cols(), MAT_Tshort);
			ppbSrc = pmSrc->data.ptr;
			ppsTemp = matTemp.data.s;
			ppbDst = pmDst ? pmDst->data.ptr : ppbSrc;
			
			for(int y = ymin ; y < ymax ; y++)
			{
				for(int x = xmin ; x < xmax ; x++)
				{
					if (y-k2 > 0 && (y+kmax-1) < ymax && x-k2 > 0 && (x+kmax-1) < xmax)
					{
						b = 0;
						iCount = 0;
						for(int j = -k2 ; j < kmax ; j++)
						{
							for(int k = -k2 ; k < kmax ; k++)
							{
								i = m_pnKernel[iCount];
								b += ppbSrc[y+j][x+k] * i;
								iCount++;
							}
						}
						ppsTemp[y][x] = (BYTE)MIN(255, MAX(0,(int)(b/m_nKFactor + m_nKOffset)));
					}
					else
						ppsTemp[y][x] = ppbSrc[y][x];
				}
			}
			
			for (int iRow = 0 ; iRow < matTemp.Rows() ; iRow ++)
			{
				for (int iCol = 0 ; iCol < matTemp.Cols() ; iCol ++)
					ppbDst[iRow][iCol] = MIN(ppsTemp[iRow][iCol], 255);
			}
		}
		break;
	case MAT_Tfloat:
		{
			float**	pprSrc;
			float**	pprTemp;
			float**	pprDst;
			float b;
			int i;
			
			Mat matTemp(pmSrc->Rows(), pmSrc->Cols(), MAT_Tfloat);
			pprSrc = pmSrc->data.fl;
			pprTemp = matTemp.data.fl;
			pprDst = pmDst ? pmDst->data.fl : pprSrc;
			
			for(int y = ymin ; y < ymax ; y++)
			{
				for(int x = xmin ; x < xmax ; x++)
				{
					if (y-k2 > 0 && (y+kmax-1) < ymax && x-k2 > 0 && (x+kmax-1) < xmax)
					{
						b = 0;
						iCount = 0;
						for(int j = -k2 ; j < kmax ; j++)
						{
							for(int k = -k2 ; k < kmax ; k++)
							{
								i = m_pnKernel[iCount];
								b += pprSrc[y+j][x+k] * i;
								iCount++;
							}
						}
						pprTemp[y][x] = b/m_nKFactor + m_nKOffset;
					}
					else
						pprTemp[y][x] = pprSrc[y][x];
				}
			}
			
			for (int iRow = 0 ; iRow < matTemp.Rows() ; iRow ++)
			{
				for (int iCol = 0 ; iCol < matTemp.Cols() ; iCol ++)
					pprDst[iRow][iCol] = pprTemp[iRow][iCol];
			}
		}
		break;
    default:
            break;
	}

	return true;
}

bool ipFilter::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/) 
{
	if (pDst)
	{
		if (!EqualTypeSize(pSrc, pDst))
		{
			pDst->Release();
			pDst->Create (*pSrc, TRUE);
		}
		Process(&pSrc->m_matX, &pDst->m_matX);
		Process(&pSrc->m_matY, &pDst->m_matY);
		Process(&pSrc->m_matZ, &pDst->m_matZ);
	}
	else
	{
		CoImage tempImage(*pSrc);
		Process(&tempImage, pSrc);
	}
	return true;
}

bool ipFilter::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	switch (m_filterType)
	{
	case MAT:
		return iProcessMat(pmSrc, pmDst);
	case VECRow:
		return iProcessVecRow(pmSrc, pmDst);
	case VECCol:
		return iProcessVecCol(pmSrc, pmDst);
            default:
            assert(false);
	}
	return false;
}

bool ipFilter::iProcessVecRow (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (m_pnKernel == NULL)
		return false;
	Mat mTemp;
	Mat* pmsrc=NULL;
	Mat* pmdst=NULL;
	if (pmDst)
	{
		pmDst->Release();
		pmDst->Create (*pmSrc, FALSE);
		pmsrc=pmSrc;
		pmdst=pmDst;
	}
	else
	{
		mTemp.Create (*pmSrc, TRUE);
		pmsrc=&mTemp;
		pmdst=pmSrc;
	}

	int k2 = m_nKSize/2;
	int kmax= m_nKSize-k2;
	
	int xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmsrc->Cols();		// width
	ymax = pmsrc->Rows();		// height			
	
	int iCount;

	switch(pmsrc->Type())
	{
	case MAT_Tbyte:
		{
			BYTE**	ppbSrc=pmsrc->data.ptr;
			BYTE**	ppbDst=pmdst->data.ptr;
			for(int y = ymin ; y < ymax ; y++)
			{
				for(int x = xmin ; x < xmax ; x++)
				{
					if (x-k2 > 0 && (x+kmax-1) < xmax)
					{
						int b = 0;
						iCount = 0;
						for(int k = -k2; k < kmax; k++, iCount++)
							b += ppbSrc[y][x+k] * m_pnKernel[iCount];
						ppbDst[y][x] = (BYTE)MIN(255, MAX(0,(int)(b/m_nKFactor + m_nKOffset)));
					}
					else
						ppbDst[y][x] = ppbSrc[y][x];
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			float**	ppbSrc=pmsrc->data.fl;
			float**	ppbDst=pmdst->data.fl;
			for(int y = ymin ; y < ymax ; y++)
			{
				for(int x = xmin ; x < xmax ; x++)
				{
					if (x-k2 > 0 && (x+kmax-1) < xmax)
					{
						float b = 0;
						iCount = 0;
						for(int k = -k2; k < kmax; k++, iCount++)
							b += ppbSrc[y][x+k] * (float)m_pnKernel[iCount];
						ppbDst[y][x] = b/(float)m_nKFactor + (float)m_nKOffset;
					}
					else
						ppbDst[y][x] = ppbSrc[y][x];
				}
			}
		}
		break;
        default:
            assert(false);
	}

	return true;
}
bool ipFilter::iProcessVecCol (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (m_pnKernel == NULL)
		return false;
	Mat mTemp;
	Mat* pmsrc=NULL;
	Mat* pmdst=NULL;
	if (pmDst)
	{
		pmDst->Release();
		pmDst->Create (*pmSrc, FALSE);
		pmsrc=pmSrc;
		pmdst=pmDst;
	}
	else
	{
		mTemp.Create (*pmSrc, TRUE);
		pmsrc=&mTemp;
		pmdst=pmSrc;
	}
	
	int k2 = m_nKSize/2;
	int kmax= m_nKSize-k2;
	
	int xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmsrc->Cols();		// width
	ymax = pmsrc->Rows();		// height			
	
	int iCount;
	
	switch(pmsrc->Type())
	{
	case MAT_Tbyte:
		{
			BYTE**	ppbSrc=pmsrc->data.ptr;
			BYTE**	ppbDst=pmdst->data.ptr;
			for(int x = xmin ; x < xmax ; x++)
			{
				for(int y = ymin ; y < ymax ; y++)
				{
					if (y-k2 > 0 && (y+kmax-1) < ymax)
					{
						int b = 0;
						iCount = 0;
						for(int k = -k2; k < kmax; k++, iCount++)
							b += ppbSrc[y+k][x] * m_pnKernel[iCount];
						ppbDst[y][x] = (BYTE)MIN(255, MAX(0,(int)(b/m_nKFactor + m_nKOffset)));
					}
					else
						ppbDst[y][x] = ppbSrc[y][x];
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			float**	ppbSrc=pmsrc->data.fl;
			float**	ppbDst=pmdst->data.fl;
			for(int x = xmin ; x < xmax ; x++)
			{
				for(int y = ymin ; y < ymax ; y++)
				{
					if (y-k2 > 0 && (y+kmax-1) < ymax)
					{
						float b = 0;
						iCount = 0;
						for(int k = -k2; k < kmax; k++, iCount++)
							b += ppbSrc[y+k][x] * (float)m_pnKernel[iCount];
						ppbDst[y][x] = b/(float)m_nKFactor + (float)m_nKOffset;
					}
					else
						ppbDst[y][x] = ppbSrc[y][x];
				}
			}
		}
		break;
        default:
            break;
	}
	
	return true;
}

bool ipFilter::Process (Vec* pvSrc, Vec* pvDst/* = NULL*/)
{
	if (m_pnKernel == NULL)
		return false;
	Vec mTemp;
	Vec* pvsrc=NULL;
	Vec* pvdst=NULL;
	if (pvDst)
	{
		pvDst->Release();
		pvDst->Create (*pvSrc, FALSE);
		pvsrc=pvSrc;
		pvdst=pvDst;
	}
	else
	{
		mTemp.Create (*pvSrc, TRUE);
		pvsrc=&mTemp;
		pvdst=pvSrc;
	}
	
	int k2 = m_nKSize/2;
	int kmax= m_nKSize-k2;
	
	int xmin,xmax;
	xmin = 0;
	xmax = pvsrc->Length();

	switch (pvsrc->Type())
	{
	case MAT_Tbyte:
		{
			BYTE*	pbSrc=pvsrc->data.ptr;
			BYTE*	pbDst=pvdst->data.ptr;
			for(int x = xmin ; x < xmax ; x++)
			{
				if (x-k2 > 0 && (x+kmax-1) < xmax)
				{
					int b = 0;
					int iCount = 0;
					for(int k = -k2; k < kmax; k++, iCount++)
						b += pbSrc[x] * m_pnKernel[iCount];
					pbDst[x] = (BYTE)MIN(255, MAX(0,(int)(b/m_nKFactor + m_nKOffset)));
				}
				else
					pbDst[x] = pbSrc[x];
			}
		}
	case MAT_Tfloat:
		{
			float*	pbSrc=pvsrc->data.fl;
			float*	pbDst=pvdst->data.fl;
			for(int x = xmin ; x < xmax ; x++)
			{
				if (x-k2 > 0 && (x+kmax-1) < xmax)
				{
					float b = 0;
					int iCount = 0;
					for(int k = -k2; k < kmax; k++, iCount++)
						b += pbSrc[x] * (float)m_pnKernel[iCount];
					pbDst[x] = (float)(b/m_nKFactor + m_nKOffset);
				}
				else
					pbDst[x] = pbSrc[x];
			}
		}
        default:
            assert(false);
	}
	return true;
}

/**
 @brief »°Ãùº¬Ë¾ ¹¹µÍ¶µÂ×²÷ ÂÜºã

 @param kernel		[in]:	µØÁÄ±¨¿¤³º
 @param Ksize		[in]:	¿¤³ºÌ© ¿Í±¨
 @param Kfactor		[in]:	¼³°öÃùº¬ºã
 @param Koffset		[in]:	ÁÝÌ¡
 @param pmatOrg		[in]:	ÌºÃùº¬
 @param pmatDest	[out]:	°Î±áÃùº¬
 @return bool ºÂ°áËË·² 1, Ê­³£·² 0
 */
bool ipFilter::Process2(long* kernel, long Ksize, long Kfactor, long Koffset, Mat* pmatOrg, Mat* pmatDest/* = NULL*/)
{
	assert (pmatOrg->Type() == MAT_Tbyte);
	assert (Kfactor != 0);

	if (pmatDest)
	{
		assert (pmatDest->Rows() == pmatOrg->Rows());
		assert (pmatDest->Cols() == pmatOrg->Cols());
		assert (pmatDest->Type() == pmatOrg->Type());
	}

	Mat matTemp(pmatOrg->Rows(), pmatOrg->Cols(), MAT_Tbyte);

	long k2 = Ksize/2;
	long kmax= Ksize-k2;
	long b,i;

	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmatOrg->Cols();		// width
	ymax = pmatOrg->Rows();		// height			

	int iCount;
    BYTE** ppOrg;
	BYTE** ppDest;

	ppOrg = pmatOrg->data.ptr;
	ppDest = matTemp.data.ptr;

	for(long y = ymin ; y < ymax ; y++)
	{
		for(long x = xmin ; x < xmax ; x++)
		{
			if (y-k2 > 0 && (y+kmax-1) < ymax && x-k2 > 0 && (x+kmax-1) < xmax)
			{
				b = 0;
				iCount = 0;
				for(long j = -k2 ; j < kmax ; j++)
				{
					for(long k = -k2 ; k < kmax ; k++)
					{
						i = kernel[iCount];
						b += ppOrg[y+j][x+k] * i;
						iCount++;
					}
				}
				ppDest[y][x] = (BYTE)MIN(255, MAX(0,(int)(b/Kfactor + Koffset)));
			}
			else
				ppDest[y][x] = ppOrg[y][x];
		}
	}
	
	BYTE** ppbOut;
	if (pmatDest)
		ppbOut = pmatDest->data.ptr;
	else
		ppbOut = pmatOrg->data.ptr;

	for (int iRow = 0 ; iRow < matTemp.Rows() ; iRow ++)
		for (int iCol = 0 ; iCol < matTemp.Cols() ; iCol ++)
			ppbOut[iRow][iCol] = ppDest[iRow][iCol];
		
	matTemp.Release();

	return true;
}

bool ipFilter::Process2(long* kernel, long Ksize, long Kfactor, long Koffset, Rect* pFilteringRect, Mat* pmatOrg, Mat* pmatDest/* = NULL*/)
{
	assert (pmatOrg->Type() == MAT_Tbyte);
	assert (pmatOrg->Rows() > 0 && pmatOrg->Cols() > 0);
	assert (Kfactor != 0);
	assert (pFilteringRect);
	assert ((pFilteringRect->x >= 0) && (pFilteringRect->y >= 0) && 
		((pFilteringRect->x + pFilteringRect->width) < pmatOrg->Cols()) &&
		((pFilteringRect->y + pFilteringRect->height) < pmatOrg->Rows()));
	
	if (pmatDest)
	{
		assert (pmatDest->Rows() == pmatOrg->Rows());
		assert (pmatDest->Cols() == pmatOrg->Cols());
		assert (pmatDest->Type() == pmatOrg->Type());
	}
	
	int nImgWidth = pmatOrg->Cols();
	int nImgHeight = pmatOrg->Rows();
	
	Mat matTemp(pmatOrg->Rows(), pmatOrg->Cols(), MAT_Tbyte);
	matTemp.Zero();
	
	long k2 = Ksize/2;
	long kmax= Ksize-k2;
	long b,i;
	
	long xmin,xmax,ymin,ymax;
	xmin = pFilteringRect->x;
	ymin = pFilteringRect->y;
	xmax = pFilteringRect->x + pFilteringRect->width;		// width
	ymax = pFilteringRect->y + pFilteringRect->height;		// height			
	
	int iCount;
    BYTE** ppOrg;
	BYTE** ppDest;
	
	ppOrg = pmatOrg->data.ptr;
	ppDest = matTemp.data.ptr;
	
	for(long y = ymin ; y < ymax ; y++)
	{
		for(long x = xmin ; x < xmax ; x++)
		{
			if (y-k2 > 0 && (y+kmax-1) < nImgHeight && x-k2 > 0 && (x+kmax-1) < nImgWidth)
			{
				b = 0;
				iCount = 0;
				for(long j = -k2 ; j < kmax ; j++)
				{
					for(long k = -k2 ; k < kmax ; k++)
					{
						i = kernel[iCount];
						b += ppOrg[y+j][x+k] * i;
						iCount++;
					}
				}
				ppDest[y][x] = (BYTE)MIN(255, MAX(0,(int)(b/Kfactor + Koffset)));
			}
			else
				ppDest[y][x] = ppOrg[y][x];
		}
	}
	
	BYTE** ppbOut;
	if (pmatDest)
		ppbOut = pmatDest->data.ptr;
	else
		ppbOut = pmatOrg->data.ptr;
	
	for (int iRow = 0 ; iRow < matTemp.Rows() ; iRow ++)
		for (int iCol = 0 ; iCol < matTemp.Cols() ; iCol ++)
			ppbOut[iRow][iCol] = ppDest[iRow][iCol];
		
		matTemp.Release();
		
		return true;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_PUMP(ipFilterSep, ipCorePump)

ipFilterSep::ipFilterSep() :ipFilter(){m_pmTemp=NULL;}
ipFilterSep::ipFilterSep(const ipFilter& from) : ipFilter(from) {m_pmTemp=NULL;}
ipFilterSep::ipFilterSep(int* pnKernel, int nKsize) : ipFilter(pnKernel, nKsize, ipFilter::KernelFactor(pnKernel, nKsize),0, ipFilter::VEC) {m_pmTemp=NULL;}
ipFilterSep::ipFilterSep(float* pnKernel, int nKsize) : ipFilter(pnKernel, nKsize, ipFilter::KernelFactor(pnKernel, nKsize),0, ipFilter::VEC) {m_pmTemp=NULL;}
ipFilterSep::ipFilterSep(double* pnKernel, int nKsize) : ipFilter(pnKernel, nKsize, ipFilter::KernelFactor(pnKernel, nKsize),0, ipFilter::VEC) {m_pmTemp=NULL;}
ipFilterSep::~ipFilterSep() {if (m_pmTemp) delete m_pmTemp;}

bool ipFilterSep::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (pmDst && !EqualTypeSize(pmSrc, pmDst))
	{
		pmDst->Release();
		pmDst->Create (*pmSrc, FALSE);
	}
	Mat* pmTemp=NULL;
	if (pmDst==NULL)
	{
		if (!m_pmTemp)
			m_pmTemp=new Mat(*pmSrc);
		else if (!m_pmTemp->IsEqualMat(*pmSrc))
		{
			delete m_pmTemp;
			m_pmTemp=new Mat(*pmSrc);
		}
		else
		{
			for (int i=0; i<pmSrc->Rows(); i++)
				memcpy(m_pmTemp->data.ptr[i], pmSrc->data.ptr[i], pmSrc->Cols()*pmSrc->Step());
		}
	}
	pmTemp=m_pmTemp;

	bool fProcess = false;

	if (m_pnKernel)
	{
		switch (pmSrc->Type())
		{
		case MAT_Tbyte:
			{
				BYTE** ppbIn;
				BYTE** ppbOut;

				int i, j, nKSize2 = m_nKSize / 2, nRows = pmSrc->Rows(), nCols = pmSrc->Cols();
				int ii, jj, k;
				if (pmDst)
				{
					ppbIn = pmSrc->data.ptr;
					ppbOut = pmTemp->data.ptr;
				}
				else
				{
					ppbIn = pmSrc->data.ptr;
					ppbOut = pmTemp->data.ptr;
				}
				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						int nSum = 0;
						for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
							nSum += ppbIn[i][jj] * m_pnKernel[k];
						ppbOut[i][j] = nSum / m_nKFactor;
					}
				}

				if (pmDst)
				{
					ppbIn = pmTemp->data.ptr;
					ppbOut = pmDst->data.ptr;
				}
				else
				{
					ppbIn = pmTemp->data.ptr;
					ppbOut = pmSrc->data.ptr;
				}

				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						int nSum = 0;
						for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nCols-1); ii++, k++)
							nSum += ppbIn[ii][j] * m_pnKernel[k];
						ppbOut[i][j] = nSum / m_nKFactor;
					}
				}
			}
			break;
		case MAT_Tfloat:
			{
				float** pprIn;
				float** pprOut;

				int i, j, nKSize2 = m_nKSize / 2, nRows = pmSrc->Rows(), nCols = pmSrc->Cols();
				int ii, jj, k;

				if (pmDst)
				{
					pprIn = pmSrc->data.fl;
					pprOut = pmTemp->data.fl;
				}
				else
				{
					pprIn = pmSrc->data.fl;
					pprOut = pmTemp->data.fl;
				}
				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						float rSum = 0;
						for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
							rSum += (float)(pprIn[i][jj] * m_pnKernel[k]);
						pprOut[i][j] = rSum / m_nKFactor;
					}
				}

				if (pmDst)
				{
					pprIn = pmTemp->data.fl;
					pprOut = pmDst->data.fl;
				}
				else
				{
					pprIn = pmTemp->data.fl;
					pprOut = pmSrc->data.fl;
				}

				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						float rSum = 0;
						for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nCols-1); ii++, k++)
							rSum += (float)(pprIn[ii][j] * m_pnKernel[k]);
						pprOut[i][j] = rSum / m_nKFactor;
					}
				}
			}
			break;
		default:
			fProcess = false;
		}
	}
	else if (m_prKernel)
	{
		switch (pmSrc->Type())
		{
		case MAT_Tbyte:
			{
				BYTE** ppbIn;
				BYTE** ppbOut;

				int i, j, nKSize2 = m_nKSize / 2, nRows = pmSrc->Rows(), nCols = pmSrc->Cols();
				int ii, jj, k;

				if (pmDst)
				{
					ppbIn = pmSrc->data.ptr;
					ppbOut = pmTemp->data.ptr;
				}
				else
				{
					ppbIn = pmSrc->data.ptr;
					ppbOut = pmTemp->data.ptr;
				}
				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						float rSum = 0;
						for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
							rSum += ppbIn[i][jj] * m_prKernel[k];
						ppbOut[i][j] = (BYTE)(rSum / m_rKFactor);
					}
				}

				if (pmDst)
				{
					ppbIn = pmTemp->data.ptr;
					ppbOut = pmDst->data.ptr;
				}
				else
				{
					ppbIn = pmTemp->data.ptr;
					ppbOut = pmSrc->data.ptr;
				}

				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						float rSum = 0;
						for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nCols-1); ii++, k++)
							rSum += ppbIn[ii][j] * m_prKernel[k];
						ppbOut[i][j] = (BYTE)(rSum / m_rKFactor);
					}
				}
			}
			break;
		case MAT_Tfloat:
			{
				float** pprIn;
				float** pprOut;

				int i, j, nKSize2 = m_nKSize / 2, nRows = pmSrc->Rows(), nCols = pmSrc->Cols();
				int ii, jj, k;

				if (pmDst)
				{
					pprIn = pmSrc->data.fl;
					pprOut = pmTemp->data.fl;
				}
				else
				{
					pprIn = pmSrc->data.fl;
					pprOut = pmTemp->data.fl;
				}
				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						float rSum = 0;
						for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
							rSum += (float)(pprIn[i][jj] * m_prKernel[k]);
						pprOut[i][j] = rSum / m_rKFactor;
					}
				}

				if (pmDst)
				{
					pprIn = pmTemp->data.fl;
					pprOut = pmDst->data.fl;
				}
				else
				{
					pprIn = pmTemp->data.fl;
					pprOut = pmSrc->data.fl;
				}				
				for (i = 0; i < nRows; i ++)
				{
					for (j = 0; j < nCols; j ++)
					{
						float rSum = 0;
						for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nRows-1); ii++, k++)
							rSum += (float)(pprIn[ii][j] * m_prKernel[k]);
						pprOut[i][j] = rSum / m_rKFactor;
					}
				}
			}
			break;
		default:
			fProcess = false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
* 2D linear filter
* \param kernel: convolving matrix, in row format.
* \param Ksize: size of the kernel.
* \param Kfactor: normalization constant.
* \param Koffset: bias.
* \verbatim Example: the "soften" filter uses this kernel:
1 1 1
1 8 1
1 1 1
the function needs: kernel={1,1,1,1,8,1,1,1,1}; Ksize=3; Kfactor=16; Koffset=0; \endverbatim
* \return true if everything is ok
*/
int	Filter::KernelFactor(int* pnKernel, int nLen)
{
	int nRet = 0;
	for (int ii = 0; ii < nLen; ii ++)
		nRet += pnKernel[ii];
	return nRet;
}

float Filter::KernelFactor(float* prKernel, int nLen)
{
	float rRet = 0.0f;
	for (int ii = 0; ii < nLen; ii ++)
		rRet += prKernel[ii];
	return rRet;
}
bool Filter::Process2D (int* pnKernel, int nKsize, int nKFactor, 
						int nKOffset, Mat* pmatOrg, Mat* pmatDest)
{
	assert (pmatOrg->Type() == MAT_Tbyte);
	assert (pmatDest->Type() == MAT_Tshort);


	int k2 = nKsize/2;
	int kmax= nKsize-k2;
	int b,i;

	int xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmatOrg->Cols();		// width
	ymax = pmatOrg->Rows();		// height			

	int iCount;
	BYTE**	ppbOrg;
	short**	ppbTemp;
	short**	ppbDest;

	Mat matTemp(pmatOrg->Rows(), pmatOrg->Cols(), MAT_Tshort);
	ppbOrg = pmatOrg->data.ptr;
	ppbTemp = matTemp.data.s;
	ppbDest = pmatDest->data.s;

	for(int y = ymin ; y < ymax ; y++)
	{
		for(int x = xmin ; x < xmax ; x++)
		{
			if (y-k2 > 0 && (y+kmax-1) < ymax && x-k2 > 0 && (x+kmax-1) < xmax)
			{
				b = 0;
				iCount = 0;
				for(int j = -k2 ; j < kmax ; j++)
				{
					for(int k = -k2 ; k < kmax ; k++)
					{
						i = pnKernel[iCount];
						b += ppbOrg[y+j][x+k] * i;
						iCount++;
					}
				}
				ppbTemp[y][x] = (BYTE)MIN(255, MAX(0,(int)(b/nKFactor + nKOffset)));
			}
			else
				ppbTemp[y][x] = ppbOrg[y][x];
		}
	}

	for (int iRow = 0 ; iRow < matTemp.Rows() ; iRow ++)
		for (int iCol = 0 ; iCol < matTemp.Cols() ; iCol ++)
			ppbDest[iRow][iCol] = ppbTemp[iRow][iCol];

	return true;
}

bool Filter::Conv2Sep(int* pnKernel, int nKSize, int nKFactor, Mat* pmIn, Mat* pmDest)
{
	if ((pmIn->Rows() != pmDest->Rows()) ||
		(pmIn->Cols() != pmDest->Cols()))
		return false;
	if (pmIn->Type() != pmDest->Type())
		return false;

	Mat* pmTemp = new Mat (*pmIn);
	switch (pmIn->Type())
	{
	case MAT_Tbyte:
		{
			MatOp::CopyMat (pmDest, pmIn, MAT_Tbyte);

			BYTE** ppbIn;
			BYTE** ppbOut;

			int i, j, nKSize2 = nKSize / 2, nRows = pmIn->Rows(), nCols = pmIn->Cols();
			int ii, jj, k;

			ppbIn = pmDest->data.ptr;
			ppbOut = pmTemp->data.ptr;
			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					int nSum = 0;
					for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
						nSum += ppbIn[i][jj] * pnKernel[k];
					ppbOut[i][j] = nSum / nKFactor;
				}
			}

			ppbIn = pmTemp->data.ptr;
			ppbOut = pmDest->data.ptr;

			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					int nSum = 0;
					for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nCols-1); ii++, k++)
						nSum += ppbIn[ii][j] * pnKernel[k];
					ppbOut[i][j] = nSum / nKFactor;
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			MatOp::CopyMat (pmDest, pmIn, MAT_Tfloat);

			float** pprIn;
			float** pprOut;

			int i, j, nKSize2 = nKSize / 2, nRows = pmIn->Rows(), nCols = pmIn->Cols();
			int ii, jj, k;

			pprIn = pmDest->data.fl;
			pprOut = pmTemp->data.fl;
			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					float rSum = 0;
					for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
						rSum += (float)(pprIn[i][jj] * pnKernel[k]);
					pprOut[i][j] = rSum / nKFactor;
				}
			}

			pprIn = pmTemp->data.fl;
			pprOut = pmDest->data.fl;

			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					float rSum = 0;
					for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nCols-1); ii++, k++)
						rSum += (float)(pprIn[ii][j] * pnKernel[k]);
					pprOut[i][j] = rSum / nKFactor;
				}
			}
		}
		break;
        default:
            assert(false);
	}

	delete pmTemp;

	return true;
}

bool Filter::Conv2Sep(float* prKernel, int nKSize, float rKFactor, Mat* pmIn, Mat* pmDest)
{
	if ((pmIn->Rows() != pmDest->Rows()) ||
		(pmIn->Cols() != pmDest->Cols()))
		return false;
	if (pmIn->Type() != pmDest->Type())
		return false;

	Mat* pmTemp = new Mat (*pmIn);
	switch (pmIn->Type())
	{
	case MAT_Tbyte:
		{
			MatOp::CopyMat (pmDest, pmIn, MAT_Tbyte);

			BYTE** ppbIn;
			BYTE** ppbOut;

			int i, j, nKSize2 = nKSize / 2, nRows = pmIn->Rows(), nCols = pmIn->Cols();
			int ii, jj, k;

			ppbIn = pmDest->data.ptr;
			ppbOut = pmTemp->data.ptr;
			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					float rSum = 0;
					for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
						rSum += ppbIn[i][jj] * prKernel[k];
					ppbOut[i][j] = (BYTE)(rSum / rKFactor);
				}
			}

			ppbIn = pmTemp->data.ptr;
			ppbOut = pmDest->data.ptr;

			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					float rSum = 0;
					for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nCols-1); ii++, k++)
						rSum += ppbIn[ii][j] * prKernel[k];
					ppbOut[i][j] = (BYTE)(rSum / rKFactor);
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			MatOp::CopyMat (pmDest, pmIn, MAT_Tfloat);

			float** pprIn;
			float** pprOut;

			int i, j, nKSize2 = nKSize / 2, nRows = pmIn->Rows(), nCols = pmIn->Cols();
			int ii, jj, k;

			pprIn = pmDest->data.fl;
			pprOut = pmTemp->data.fl;
			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					float rSum = 0;
					for (jj = MAX(0,j-nKSize2), k = jj-(j-nKSize2); jj <= MIN(j + nKSize2, nCols-1); jj++, k++)
						rSum += (float)(pprIn[i][jj] * prKernel[k]);
					pprOut[i][j] = rSum / rKFactor;
				}
			}

			pprIn = pmTemp->data.fl;
			pprOut = pmDest->data.fl;

			for (i = 0; i < nRows; i ++)
			{
				for (j = 0; j < nCols; j ++)
				{
					float rSum = 0;
					for (ii = MAX(0,i-nKSize2), k = ii-(i-nKSize2); ii <= MIN(i + nKSize2, nRows-1); ii++, k++)
						rSum += (float)(pprIn[ii][j] * prKernel[k]);
					pprOut[i][j] = rSum / rKFactor;
				}
			}
		}
		break;
            default:
            assert(false);
	}

	delete pmTemp;

	return true;
}

namespace ip
{
static inline Point2i normalizeAnchor( Point2i anchor, Size ksize )
{
    if( anchor.x == -1 )
        anchor.x = ksize.width/2;
    if( anchor.y == -1 )
        anchor.y = ksize.height/2;
    assert( Rect(0, 0, ksize.width, ksize.height).PtInRect(anchor) );
    return anchor;
}
void sepFilter2D( const Mat& src, Mat& dst, int ddepth, Mat& kernelX, Mat& _kernelY, Point2i anchor, double delta, int borderType )
{
}
void filter2D(const Mat& src, Mat& dst, int ddepth, Mat& kernel, Point2i anchor, double delta, int borderType )
{
    dst.Create( src.SizeObject(), (TYPE)CVLIB_MAKETYPE(ddepth, src.Channels()) );
	normalizeAnchor(anchor, kernel.SizeObject());
    int dft_filter_size = 50;/*((src.Type() == MAT_Tbyte && (ddepth == MAT_Tbyte || ddepth == MAT_Tshort)) ||
        (src.Type() == MAT_Tfloat && ddepth == MAT_Tfloat)) && checkHardwareSupport(CV_CPU_SSE3)? 130 :*/
    if( kernel.Rows()*kernel.Cols() >= dft_filter_size )
    {
        Mat temp;
        if( src.data.ptr != dst.data.ptr )
            temp = dst;
        else
            temp.Create(dst.SizeObject(), dst.Type1());
/*        crossCorr( src, kernel, temp, src.size(), CVLIB_MAKETYPE(ddepth, src.Channels()),
                   anchor, delta, borderType );
        if( temp.data.ptr != dst.data.ptr )
            temp.copyTo(dst);
        return;*/
		return;
    }

/*    Ptr<FilterEngine> f = createLinearFilter(src.type(), dst.type(), kernel,
                                             anchor, delta, borderType & ~BORDER_ISOLATED );
    f->apply(src, dst, Rect(0,0,-1,-1), Point(), (borderType & BORDER_ISOLATED) != 0 );*/
}

}}