
/*!
 * \file	ipMedian.cpp
 * \ingroup ipCore
 * \brief   MedianµØÁÄ¶¦ ¼êÃÔÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#include "cvlibbase/inc/cvlibutil.h"
#include "ipMedian.h"
#include "ColorSpace.h"

namespace CVLib
{

IMPLEMENT_PUMP(ipMedian, ipCorePump)
bool ipMedian::Process (Mat* pmSrc, Mat* pmDst/* = NULL*/)
{
	if ( pmDst && !EqualTypeSize(pmSrc, pmDst) )
	{
		pmDst->Release();
		pmDst->Create(*pmSrc);
	}
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	long i,j,k;
	
	int* kernel = (int*)malloc(m_nKsize*m_nKsize*sizeof(int));
	
	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmSrc->Cols(); ymax=pmSrc->Rows();
	
	if (pmDst)
	{
		BYTE** ppbSrc = pmSrc->data.ptr;
		BYTE** ppbDst = pmDst->data.ptr;
		for(long y=ymin; y<ymax; y++)
		{
			for(long x=xmin; x<xmax; x++)
			{
				for(j=-k2, i=0;j<kmax;j++)
					for(k=-k2;k<kmax;k++, i++)
					{
						if (pmSrc->IsInterior(x+j, y+k))
							kernel[i]=ppbSrc[y+k][x+j];
						else
							i--;
					}
					CVUtil::Sort(kernel, i, CVUtil::SORT_INC);
				ppbDst[y][x] = kernel[i/2];
			}
		}
	}
	else
	{
		Mat mTemp(*pmSrc);
		BYTE** ppbSrc = mTemp.data.ptr;
		BYTE** ppbDst = pmSrc->data.ptr;
		for(long y=ymin; y<ymax; y++)
		{
			for(long x=xmin; x<xmax; x++)
			{
				for(j=-k2, i=0;j<kmax;j++)
					for(k=-k2;k<kmax;k++, i++)
					{
						if (mTemp.IsInterior(x+j, y+k))
							kernel[i]=ppbSrc[y+k][x+j];
						else
							i--;
					}
					CVUtil::Sort(kernel, i, CVUtil::SORT_INC);
					ppbDst[y][x] = kernel[i/2];
			}
		}
	}
	free(kernel);
	return true;
}

bool ipMedian::Process (CoImage* pSrc, CoImage* pDst/* = NULL*/)
{
	if ( pDst && !EqualTypeSize(pSrc, pDst) )
	{
		pDst->Release();
		pDst->Create (*pSrc);
	}
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	long i,j,k;

	int* kernel = (int*)malloc(m_nKsize*m_nKsize*sizeof(int));
	int* sortidx = (int*)malloc(m_nKsize*m_nKsize*sizeof(int));
	int* xindex = (int*)malloc(m_nKsize*m_nKsize*sizeof(int));
	int* yindex = (int*)malloc(m_nKsize*m_nKsize*sizeof(int));

	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pSrc->GetWidth(); ymax=pSrc->GetHeight();

	CoImage yuv(pSrc->GetHeight(), pSrc->GetWidth(), MAT_Tfloat, CoImage::enCOLOR_MODE);
	ColorSpace::RGBtoYUV(pSrc, &yuv);

	Mat mY(yuv.m_matX);
	Mat mU(yuv.m_matY);
	Mat mV(yuv.m_matZ);
	float** pprSrc1 = mY.data.fl;
	float** pprSrc2 = mU.data.fl;
	float** pprSrc3 = mV.data.fl;
	float** pprDst1 = yuv.m_matX.data.fl;
	float** pprDst2 = yuv.m_matY.data.fl;
	float** pprDst3 = yuv.m_matZ.data.fl;
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			for(j=-k2, i=0;j<kmax;j++)
			{
				for(k=-k2;k<kmax;k++, i++)
				{
					if (mY.IsInterior(x+j, y+k))
					{
						kernel[i]=(int)pprSrc1[y+k][x+j];
						xindex[i]=x+j;
						yindex[i]=y+k;
					}
					else
					{
						kernel[i]=(int)pprSrc1[0][0];
						xindex[i]=0;
						yindex[i]=0;
					}
				}
			}
			CVUtil::SortIdx(kernel, i, sortidx, i, CVUtil::SORT_INC);
			int x1 = xindex[sortidx[i/2]];
			int y1 = yindex[sortidx[i/2]];
			pprDst1[y][x] = pprSrc1[y1][x1];
			pprDst2[y][x] = pprSrc2[y1][x1];
			pprDst3[y][x] = pprSrc3[y1][x1];
		}
	}

	if (pDst)
		ColorSpace::YUVtoRGB(&yuv, pDst);
	else
		ColorSpace::YUVtoRGB(&yuv, pSrc);
	free(kernel);
	free(sortidx);
	free(xindex);
	free(yindex);
	return true;
}

bool ipMedian::Process (Mat* pmSrc, scantype scantype, Mat* pmDst)
{
	Mat* pvsrc=NULL;
	Mat* pvdst=NULL;
	Mat vTemp;
	if ( pmDst )
	{
		pmDst->Release();
		pmDst->Create(*pmSrc);
		pvsrc=pmSrc;
		pvdst=pmDst;
	}
	else
	{
		vTemp.Create (*pmSrc, TRUE);
		pvsrc=&vTemp;
		pvdst=pmSrc;
	}
	BYTE** ppbSrc = pvsrc->data.ptr;
	BYTE** ppbDst = pvdst->data.ptr;
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	long i,j;
	
	int* kernel = (int*)malloc(m_nKsize*sizeof(int));

	if (scantype==Row)
	{
		long xmin,xmax;
		xmin = 0;
		xmax = pvsrc->Cols();
		for (long y=0; y<pvsrc->Rows(); y++)
		{
			for(long x=xmin; x<xmax; x++)
			{
				for(j=-k2, i=0;j<kmax;j++, i++)
				{
					if (pvsrc->IsInterior(x+j, y))
						kernel[i]=ppbSrc[y][x+j];
					else
						i--;
				}
				CVUtil::Sort(kernel, i, CVUtil::SORT_INC);
				ppbDst[y][x] = kernel[i/2];
			}
		}
	}
	else
	{
		long xmin,xmax;
		xmin = 0;
		xmax = pvsrc->Rows();
		for (long y=0; y<pvsrc->Cols(); y++)
		{
			for(long x=xmin; x<xmax; x++)
			{
				for(j=-k2, i=0;j<kmax;j++, i++)
				{
					if (pvsrc->IsInterior(y, x+j))
						kernel[i]=ppbSrc[x+j][y];
					else
						i--;
				}
				CVUtil::Sort(kernel, i, CVUtil::SORT_INC);
				ppbDst[x][y] = kernel[i/2];
			}
		}
	}
	free(kernel);
	return true;
}

bool ipMedian::Process (Vec* pvSrc, Vec* pvDst)
{
	Vec* pvsrc=NULL;
	Vec* pvdst=NULL;
	Vec vTemp;
	if ( pvDst )
	{
		pvDst->Release();
		pvDst->Create(*pvSrc);
		pvsrc=pvSrc;
		pvdst=pvDst;
	}
	else
	{
		vTemp.Create (*pvSrc, TRUE);
		pvsrc=&vTemp;
		pvdst=pvSrc;
	}
	BYTE* pbSrc = pvsrc->data.ptr;
	BYTE* pbDst = pvdst->data.ptr;

	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	long i,j;
	
	int* kernel = (int*)malloc(m_nKsize*sizeof(int));
	
	long xmin,xmax;
	xmin = 0;
	xmax = pvsrc->Length();
	
	for(long x=xmin; x<xmax; x++)
	{
		for(j=-k2, i=0;j<kmax;j++, i++)
		{
			if (pvsrc->IsInterior(x+j))
				kernel[i]=pbSrc[x+j];
			else
				i--;
		}
		CVUtil::Sort(kernel, i, CVUtil::SORT_INC);
		pbDst[x] = kernel[i/2];
	}
	free(kernel);
	return true;
}

namespace ip
{
void Median(const Mat& src, Mat& dst, int nKsize)
{
	ipMedian t(nKsize);
	t.Process ((Mat*)&src, &dst);
}
}

}
