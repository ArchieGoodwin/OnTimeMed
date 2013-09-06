
/*!
 * \file	ipMorphology.cpp
 * \ingroup ipCore
 * \brief   »üËÁË¾ ¾¡°¡Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#include "ipMorphology.h"
#include "cvlibbase/inc/MatOperation.h"

namespace CVLib
{

// BEGIN_PARAMRANGE(ipMorphology)
// 	DEF_PARAMRANGE(radius, (int)0, (int)255, (int)2)
// END_PARAMRANGE(ipMorphology)
IMPLEMENT_PUMP(ipMorphology, ipCorePump)
IMPLEMENT_PUMP(ipErode, ipMorphology)
IMPLEMENT_PUMP(ipDilate, ipMorphology)
IMPLEMENT_PUMP(ipOpening, ipMorphology)
IMPLEMENT_PUMP(ipClosing, ipMorphology)

bool ipErode::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	BYTE r;
	
	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmSrc->Cols(); ymax=pmSrc->Rows();
	
	Mat temp;
	BYTE** ppbSrc1;
	BYTE** ppbDst1;
	if (pmDst)
	{
		if (!EqualTypeSize(pmSrc, pmDst))
		{
			pmDst->Release();
			pmDst->Create (*pmSrc, TRUE);
		}
		ppbSrc1 = pmSrc->data.ptr;
		ppbDst1 = pmDst->data.ptr;
	}
	else
	{
		temp.Create (*pmSrc, TRUE);
		ppbSrc1 = temp.data.ptr;
		ppbDst1 = pmSrc->data.ptr;
	}
	
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			r=255;
			for(long j=-k2;j<kmax;j++){
				for(long k=-k2;k<kmax;k++){
					if (pmSrc->IsInterior(x+j,y+k))
					{
						if (ppbSrc1[y+k][x+j] < r) r=ppbSrc1[y+k][x+j];
					}
				}
			}
			ppbDst1[y][x] = r;
		}
	}
	return true;
}

bool ipErode::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	BYTE r,g,b;
	
	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pSrc->GetWidth(); ymax=pSrc->GetHeight();

	CoImage temp;
	BYTE** ppbSrc1;
	BYTE** ppbSrc2;
	BYTE** ppbSrc3;
	BYTE** ppbDst1;
	BYTE** ppbDst2;
	BYTE** ppbDst3;
	if (pDst)
	{
		if (!EqualTypeSize(pSrc, pDst))
		{
			pDst->Release();
			pDst->Create (*pSrc, TRUE);
		}
		ppbSrc1 = pSrc->m_matX.data.ptr;
		ppbSrc2 = pSrc->m_matY.data.ptr;
		ppbSrc3 = pSrc->m_matZ.data.ptr;
		ppbDst1 = pDst->m_matX.data.ptr;
		ppbDst2 = pDst->m_matY.data.ptr;
		ppbDst3 = pDst->m_matZ.data.ptr;
	}
	else
	{
		temp.Create (*pSrc, TRUE);
		ppbSrc1 = temp.m_matX.data.ptr;
		ppbSrc2 = temp.m_matY.data.ptr;
		ppbSrc3 = temp.m_matZ.data.ptr;
		ppbDst1 = pSrc->m_matX.data.ptr;
		ppbDst2 = pSrc->m_matY.data.ptr;
		ppbDst3 = pSrc->m_matZ.data.ptr;
	}
		
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			r=b=g=255;
			for(long j=-k2;j<kmax;j++){
				for(long k=-k2;k<kmax;k++){
					if (pSrc->IsInterior(x+j,y+k))
					{
						if (ppbSrc1[y+k][x+j] < r) r=ppbSrc1[y+k][x+j];
						if (ppbSrc2[y+k][x+j] < g) g=ppbSrc2[y+k][x+j];
						if (ppbSrc3[y+k][x+j] < b) b=ppbSrc3[y+k][x+j];
					}
				}
			}
			ppbDst1[y][x] = r;
			ppbDst2[y][x] = g;
			ppbDst3[y][x] = b;
		}
	}

	return true;
}


bool ipDilate::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	BYTE r;
	
	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pmSrc->Cols(); ymax=pmSrc->Rows();
	
	Mat temp;
	BYTE** ppbSrc1;
	BYTE** ppbDst1;
	if (pmDst)
	{
		if (!EqualTypeSize(pmSrc, pmDst))
		{
			pmDst->Release();
			pmDst->Create (*pmSrc, TRUE);
		}
		ppbSrc1 = pmSrc->data.ptr;
		ppbDst1 = pmDst->data.ptr;
	}
	else
	{
		temp.Create (*pmSrc, TRUE);
		ppbSrc1 = temp.data.ptr;
		ppbDst1 = pmSrc->data.ptr;
	}
	
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			r=0;
			for(long j=-k2;j<kmax;j++){
				for(long k=-k2;k<kmax;k++){
					if (pmSrc->IsInterior(x+j,y+k))
					{
						if (ppbSrc1[y+k][x+j] > r) r=ppbSrc1[y+k][x+j];
					}
				}
			}
			ppbDst1[y][x] = r;
		}
	}
	return true;
}

bool ipDilate::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	long k2 = m_nKsize/2;
	long kmax= m_nKsize-k2;
	BYTE r,g,b;
	
	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pSrc->GetWidth(); ymax=pSrc->GetHeight();
	
	CoImage temp;
	BYTE** ppbSrc1;
	BYTE** ppbSrc2;
	BYTE** ppbSrc3;
	BYTE** ppbDst1;
	BYTE** ppbDst2;
	BYTE** ppbDst3;
	if (pDst)
	{
		if (!EqualTypeSize(pSrc, pDst))
		{
			pDst->Release();
			pDst->Create (*pSrc, TRUE);
		}
		ppbSrc1 = pSrc->m_matX.data.ptr;
		ppbSrc2 = pSrc->m_matY.data.ptr;
		ppbSrc3 = pSrc->m_matZ.data.ptr;
		ppbDst1 = pDst->m_matX.data.ptr;
		ppbDst2 = pDst->m_matY.data.ptr;
		ppbDst3 = pDst->m_matZ.data.ptr;
	}
	else
	{
		temp.Create (*pSrc, TRUE);
		ppbSrc1 = temp.m_matX.data.ptr;
		ppbSrc2 = temp.m_matY.data.ptr;
		ppbSrc3 = temp.m_matZ.data.ptr;
		ppbDst1 = pSrc->m_matX.data.ptr;
		ppbDst2 = pSrc->m_matY.data.ptr;
		ppbDst3 = pSrc->m_matZ.data.ptr;
	}
	
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			r=b=g=0;
			for(long j=-k2;j<kmax;j++){
				for(long k=-k2;k<kmax;k++){
					if (pSrc->IsInterior(x+j,y+k))
					{
						if (ppbSrc1[y+k][x+j] > r) r=ppbSrc1[y+k][x+j];
						if (ppbSrc2[y+k][x+j] > g) g=ppbSrc2[y+k][x+j];
						if (ppbSrc3[y+k][x+j] > b) b=ppbSrc3[y+k][x+j];
					}
				}
			}
			ppbDst1[y][x] = r;
			ppbDst2[y][x] = g;
			ppbDst3[y][x] = b;
		}
	}
	
	return true;
}
	
bool ipOpening::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	ipErode Erode(m_nKsize);
	ipDilate Dilate(m_nKsize);
	if (pmDst)
	{
		if ( !EqualTypeSize(pmSrc, pmDst) )
		{
			pmDst->Release();
			pmDst->Create (*pmSrc, TRUE);
		}
		else
			MatOp::CopyMat(pmDst, pmSrc, pmDst->Type());
		Erode.Process (pmDst, pmSrc);
		Dilate.Process (pmSrc, pmDst);
	}
	else
	{
		Mat mTemp(*pmSrc);
		Erode.Process (pmSrc, &mTemp);
		Dilate.Process (&mTemp, pmSrc);
	}
	return true;
}

bool ipOpening::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	ipErode Erode(m_nKsize);
	ipDilate Dilate(m_nKsize);
	if (pDst)
	{
		pDst->Release();
		pDst->Create (*pSrc, TRUE);
		Erode.Process (pDst, pSrc);
		Dilate.Process (pSrc, pDst);
	}
	else
	{
		CoImage temp(*pSrc);
		Erode.Process (pSrc, &temp);
		Dilate.Process (&temp, pSrc);
	}
	return true;
}
	
bool ipClosing::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	ipErode Erode(m_nKsize);
	ipDilate Dilate(m_nKsize);
	if (pmDst)
	{
		if ( !EqualTypeSize(pmSrc, pmDst) )
		{
			pmDst->Release();
			pmDst->Create (*pmSrc, TRUE);
		}
		else
			MatOp::CopyMat(pmDst, pmSrc, pmDst->Type());
		Dilate.Process (pmDst, pmSrc);
		Erode.Process (pmSrc, pmDst);
	}
	else
	{
		Mat mTemp(*pmSrc);
		Dilate.Process (pmSrc, &mTemp);
		Erode.Process (&mTemp, pmSrc);
	}
	return true;
}

bool ipClosing::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	ipErode Erode(m_nKsize);
	ipDilate Dilate(m_nKsize);
	if (pDst)
	{
		pDst->Release();
		pDst->Create (*pSrc, TRUE);
		Dilate.Process (pDst, pSrc);
		Erode.Process (pSrc, pDst);
	}
	else
	{
		CoImage temp(*pSrc);
		Dilate.Process (pSrc, &temp);
		Erode.Process (&temp, pSrc);
	}
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define	MinMaxTester( nValue, nMax, nMin ) \
	( ( ( nValue ) < ( nMin ) || ( nValue ) > ( nMax ) ) ? 0 : 1 )

/**
* ËË ÂÜºã²÷ shortÂô ¸êºã°ªÌ© ¸áÌ¡¶¦ ½£ÂÙÂÙ³Þ.
*
* @param	sValue	[In]	ËÓµÙ°ª
* @param	sMax	[In]	¾×´Ý°ª
* @param	sMin	[In]	¾×ºÏ°ª
* @return	¾×ºÏ°ª > ËÓµÙ°ªËË·² ¾×ºÏ°ª, ¾×´Ý°ª < ËÓµÙ°ªËË·² ¾×´Ý°ª, Ê­³£·² ËÓµÙ°ª.
*/
int
Morphology::MinMaxLimiter ( int nValue, int nMax, int nMin )
{
	if ( nValue < nMin )
		return nMin;
	if ( nValue > nMax )
		return nMax;

	return nValue;
}

Morphology::Morphology()
{
}

Morphology::~Morphology()
{
}

void Morphology::Opening (Mat* pImg, Mat* pFilter)
{
	assert (pImg->Type() == MAT_Tbyte);
	assert (pFilter->Type() == MAT_Tbyte);

	Mat	xTemp;
	xTemp.Create (pImg->Rows(), pImg->Cols(), MAT_Tbyte);

	Erode (pImg, &xTemp, pFilter);
	Dilate (&xTemp, pImg, pFilter);

	xTemp.Release();
}

void Morphology::Closing (Mat* pImg, Mat* pFilter)
{
	assert (pImg->Type() == MAT_Tbyte);
	assert (pFilter->Type() == MAT_Tbyte);

	Mat	xTemp;
	xTemp.Create (pImg->Rows(), pImg->Cols(), MAT_Tbyte);

	Dilate (pImg, &xTemp, pFilter);
	Erode (&xTemp, pImg, pFilter);

	xTemp.Release();
}

void Morphology::Erode (Mat* pOrg, Mat* pDest, Mat* pFilter)
{
	assert (pOrg->Type() == MAT_Tbyte);
	assert (pDest->Type() == MAT_Tbyte);
	assert (pFilter->Type() == MAT_Tbyte);

	BYTE*	pbBuffer;
	int		nY, nX, nI, nJ;
	int		nWidth;
	int		nEndX, nEndY;

	nWidth = pFilter->Rows() >> 1;
	nEndX = pDest->Cols() - 1;
	nEndY = pDest->Rows() - 1;

	for ( nY = 0; nY < pDest->Rows(); nY ++ )
	{
		pbBuffer = pDest->data.ptr[nY];
		for ( nX = 0; nX < pDest->Cols(); nX ++ )
		{
			int	nMin = GRAY_LEVEL_NUMBER;

			for ( nI = - nWidth; nI <= nWidth; nI ++ )
			{
				int	nYI = nY - nI, nIW = nI + nWidth;

				if ( !MinMaxTester ( nYI, nEndY, 0 ) )
					continue;
				for ( nJ = - nWidth; nJ <= nWidth; nJ ++ )
				{
					int	nTemp, nXJ = nX - nJ;

					if ( !MinMaxTester ( nXJ, nEndX, 0 ) )
						continue;

					nTemp = (BYTE)pOrg->data.ptr[ nYI ][ nXJ ] +
						(BYTE)pFilter->data.ptr[ nIW ][ nJ + nWidth ];
					nMin = MIN ( nMin, nTemp );
				}
			}
			nMin = MinMaxLimiter ( nMin, MAX_8BIT_VALUE, 0 );
			*pbBuffer ++ = ( BYTE )nMin;
		}
	}
}

void Morphology::Dilate (Mat* pOrg, Mat* pDest, Mat* pFilter)
{
	assert (pOrg->Type() == MAT_Tbyte);
	assert (pDest->Type() == MAT_Tbyte);
	assert (pFilter->Type() == MAT_Tbyte);

	BYTE*	pbBuffer;
	int		nY, nX, nI, nJ;
	int		nWidth;
	int		nEndX, nEndY;

	nWidth = pFilter->Rows() >> 1;
	nEndX = pDest->Cols() - 1;
	nEndY = pDest->Rows() - 1;

	for ( nY = 0; nY < pDest->Rows(); nY ++ )
	{
		pbBuffer = pDest->data.ptr[nY];
		for ( nX = 0; nX < pDest->Cols(); nX ++ )
		{
			int	nMax = 0;

			for ( nI = - nWidth; nI <= nWidth; nI ++ )
			{
				int	nYI = nY - nI, nIW = nI + nWidth;

				if ( !MinMaxTester ( nYI, nEndY, 0 ) )
					continue;
				for ( nJ = - nWidth; nJ <= nWidth; nJ ++ )
				{
					int	nTemp, nXJ = nX - nJ;

					if ( !MinMaxTester ( nXJ, nEndX, 0 ) )
						continue;

					nTemp = (BYTE)pOrg->data.ptr[ nYI ][ nXJ ] +
						(BYTE)pFilter->data.ptr[ nIW ][ nJ + nWidth ];
					nMax = MAX ( nMax, nTemp );
				}
			}
			nMax = MinMaxLimiter ( nMax, MAX_8BIT_VALUE, 0 );
			*pbBuffer ++ = ( BYTE )nMax;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
namespace ip
{

void Erode(const Mat& src, Mat& dst, int nksize)
{
	ipErode t(nksize); t.Process((Mat*)&src, &dst);
}
void Dilate(const Mat& src, Mat& dst, int nksize)
{
	ipDilate t(nksize); t.Process((Mat*)&src, &dst);
}
void Opening(const Mat& src, Mat& dst, int nksize)
{
	ipOpening t(nksize); t.Process((Mat*)&src, &dst);
}
void Closing(const Mat& src, Mat& dst, int nksize)
{
	ipClosing t(nksize); t.Process((Mat*)&src, &dst);
}

}

}
