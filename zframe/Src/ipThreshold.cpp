
/*!
 * \file	ipThreshold.cpp
 * \ingroup ipCore
 * \brief   À¿°ªËæ Ì©ÂÙ 2¼êÃùº¬Ë¾ »¶ºÂÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#include "ipThreshold.h"
#include "Histogram.h"
#include "cvlibbase/inc/MatOperation.h"

namespace CVLib
{

IMPLEMENT_PUMP(ipThreshold, ipCorePump)

bool ipThreshold::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	switch(m_nMode)
	{
	case General:
		return iProcess_General(pmSrc, pmDst);
	case Global:
		return iProcess_Global(pmSrc, pmDst);
	}
	return true;
}
bool ipThreshold::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	switch(m_nMode)
	{
	case General:
		return iProcess_General(pSrc, pDst);
	case Global:
		return iProcess_Global(pSrc, pDst);
	}
	return true;
}

bool ipThreshold::iProcess_General (Mat* pmSrc, Mat* pmDst/* = NULL*/)
{
	if ( pmDst && !EqualTypeSize(pmSrc, pmDst) )
	{
		pmDst->Release();
		pmDst->Create(*pmSrc);
	}

	int x, y;
	BYTE** ppbGray = pmSrc->data.ptr;
	BYTE** ppbDst = pmDst ? pmDst->data.ptr : ppbGray;
	for (y = 0; y < pmSrc->Rows(); y ++)
		for (x = 0; x < pmSrc->Cols(); x ++)
		{
			if (ppbGray[y][x] > m_bLevel)
				ppbDst[y][x] = 255;
			else
				ppbDst[y][x] = 0;
		}
	return true;
}

bool ipThreshold::iProcess_General (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if ( pDst && !EqualTypeSize(pSrc, pDst) )
	{
		pDst->Release();
		pDst->Create(*pSrc);
	}
	int x, y;
	pSrc->Gray();
	BYTE** ppbGray = pSrc->m_matG.data.ptr;
	CoImage* pDst1 = pDst ? pDst : pSrc;
	for (y = 0; y < pSrc->GetHeight(); y ++)
	for (x = 0; x < pSrc->GetWidth(); x ++)
	{
		if (ppbGray[y][x] > m_bLevel)
			pDst1->SetPixelColor(x,y,RGB(255,255,255));
		else
			pDst1->SetPixelColor(x,y,RGB(0,0,0));
	}
	return true;
}

bool ipThreshold::iProcess_Global (Mat* pIn, Mat* pOut/*=NULL*/)
{
	assert (pIn->Type() == MAT_Tbyte);
	
	int*	pnBuffer;
	int		nSum = 0;
	int		iPixel;
	BYTE	bTh = MAX_8BIT_VALUE;
	
	Vec			vecHistogram;
	Histogram	xHisto;
	BYTE*	pbData = pIn->data.ptr[0];
	
	int	nNumber = pIn->Rows() * pIn->Cols() * m_bLevel / 100;
	
	//	Getting the histogram of old image.
	vecHistogram.Create (GRAY_LEVEL_NUMBER, MAT_Tint);
	xHisto.Process(pIn, &vecHistogram);
	
	//	Getting the value of histogram-bins corresponding to threshold.
	pnBuffer = vecHistogram.data.i;
	for ( iPixel = 0; iPixel < GRAY_LEVEL_NUMBER; iPixel ++ )
	{
		nSum += *pnBuffer ++;
		if ( nNumber <= nSum )
		{
			bTh = ( BYTE )iPixel;
			break;
		}
	}
	if (pOut != NULL)
	{
		pOut->Release();
		pOut->Create(pIn->Rows(), pIn->Cols(), MAT_Tbyte);
		MatOp::CopyMat (pOut, pIn, MAT_Tbyte);
		pbData = pOut->data.ptr[0];
	}
	
	//	Binarization based on the bins value.
	for ( iPixel = 0; iPixel < pIn->Rows() * pIn->Cols(); iPixel ++, pbData++ )
	{
		if ( *pbData > bTh )
			*pbData = MAX_8BIT_VALUE;
		else
			*pbData = 0;
	}
	
	vecHistogram.Release();
	return true;
}

bool ipThreshold::iProcess_Global (CoImage* pSrc, CoImage* pDst/* = NULL*/)
{
	assert (false);
	return false;
}

}
