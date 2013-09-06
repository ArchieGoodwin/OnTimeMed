/*!
 * \file
 * \brief Ãùº¬Ì© ¾¢ºÏ, Ãú´Ý¶¦ ¼êÃÔÂ×²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */
#include "ScaleXY.h"
#include <assert.h>


namespace CVLib
{

/**
 @brief °é¾¢»ô
 */
ScaleXY::ScaleXY()
{
}

/**
 @brief ÃÍ½£»ô
 */
ScaleXY::~ScaleXY()
{
}

/**
 @brief Ãùº¬Ì© Ãú´Ý,¾¢ºÏ¶¦ ¼êÃÔÂÙ³Þ.
 
 @param pDst		[out]: ·»¼¬Ãùº¬Ì© ÃÔµÛ
 @param nXOrgDst	[in]: ·»¼¬Ãùº¬Ì© x¾¢ ÁÝÌ¡
 @param nYOrgDst	[in]: ·»¼¬Ãùº¬Ì© y¾¢ ÁÝÌ¡
 @param nWidthDst	[in]: ·»¼¬Ãùº¬Ì© ²¸¹¾
 @param nHeightDst	[in]: ·»¼¬Ãùº¬Ì© ²ÚËË
 @param pSrc		[in]: Ìº½çÃùº¬Ì© ÃÔµÛ
 @param nXOrgSrc	[in]: Ìº½çÃùº¬Ì© x¾¢ÁÝÌ¡
 @param nYOrgSrc	[in]: Ìº½çÃùº¬Ì© y¾¢ÁÝÌ¡
 @param nWidthSrc	[in]: Ìº½çÃùº¬Ì© ²¸¹¾
 @param nHeightSrc	[in]: Ìº½çÃùº¬Ì© ²ÚËË
 */
void	ScaleXY::Process (Mat* pDst, int nXOrgDst, int nYOrgDst, int nWidthDst, int nHeightDst,
						  const Mat* pSrc, int nXOrgSrc, int nYOrgSrc, int nWidthSrc, int nHeightSrc)
{
	switch (pDst->Type())
	{
	case MAT_Tbyte:
		{
			BYTE** ppbDst = pDst->data.ptr;
			BYTE** ppbSrc = pSrc->data.ptr;
			
			int	iH, iW;
			double	rHRate, rWRate;
			
			rHRate = (double)nHeightSrc / (double)nHeightDst;
			rWRate = (double)nWidthSrc / (double)nWidthDst;
			
			int	nNewRow, nNewCol;
			double	rA = 0.0f, rB = 0.0f;
			for ( iH = 0; iH < nHeightDst; iH ++ )
			{
				double rHScale = rA;
				nNewRow = ( int )rA;
				if ( nNewRow < nHeightSrc - 1 )
					rHScale -= nNewRow;
				else
					rHScale = 0.0f;
				
				rB = 0.0f;
				for ( iW = 0; iW < nWidthDst; iW ++ )
				{
					double rWScale = rB;
					nNewCol = ( int )rB;
					if ( nNewCol < nWidthSrc - 1 )
						rWScale -= nNewCol;
					else
						rWScale = 0.0f;
					
					ppbDst [iH + nYOrgDst][iW + nXOrgDst] = ( BYTE )BilinearInterpolation (
						ppbSrc, nNewCol + nXOrgSrc, nNewRow + nYOrgSrc, rWScale, rHScale );
					rB += rWRate;
				}
				rA += rHRate;
			}
		}
		break;
	case MAT_Tfloat:
		{
			float** pprDst = pDst->data.fl;
			float** pprSrc = pSrc->data.fl;
			
			int	iH, iW;
			double	rHRate, rWRate;
			
			rHRate = (double)nHeightSrc / (double)nHeightDst;
			rWRate = (double)nWidthSrc / (double)nWidthDst;
			
			int	nNewRow, nNewCol;
			double	rA = 0.0f, rB = 0.0f;
			for ( iH = 0; iH < nHeightDst; iH ++ )
			{
				double rHScale = rA;
				nNewRow = ( int )rA;
				if ( nNewRow < nHeightSrc - 1 )
					rHScale -= nNewRow;
				else
					rHScale = 0.0f;
				
				rB = 0.0f;
				for ( iW = 0; iW < nWidthDst; iW ++ )
				{
					double rWScale = rB;
					nNewCol = ( int )rB;
					if ( nNewCol < nWidthSrc - 1 )
						rWScale -= nNewCol;
					else
						rWScale = 0.0f;
					
					pprDst [iH + nYOrgDst][iW + nXOrgDst] = ( BYTE )BilinearInterpolation (
						pprSrc, nNewCol + nXOrgSrc, nNewRow + nYOrgSrc, rWScale, rHScale );
					rB += rWRate;
				}
				rA += rHRate;
			}
		}
		break;
            default:
            assert(false);
	}
}

/**
 @brief Ãùº¬Ì© Ãú´Ý,¾¢ºÏ¶¦ ¼êÃÔÂÙ³Þ.
 
 @param pDst		[out]: ·»¼¬Ãùº¬
 @param nXOrgDst	[in]: ·»¼¬Ãùº¬Ì© x¾¢ ÁÝÌ¡
 @param nYOrgDst	[in]: ·»¼¬Ãùº¬Ì© y¾¢ ÁÝÌ¡
 @param nWidthDst	[in]: ·»¼¬Ãùº¬Ì© ²¸¹¾
 @param nHeightDst	[in]: ·»¼¬Ãùº¬Ì© ²ÚËË
 @param pSrc		[in]: Ìº½çÃùº¬
 @param nXOrgSrc	[in]: Ìº½çÃùº¬Ì© x¾¢ ÁÝÌ¡
 @param nYOrgSrc	[in]: Ìº½çÃùº¬Ì© y¾¢ ÁÝÌ¡
 @param nWidthSrc	[in]: Ìº½çÃùº¬Ì© ²¸¹¾
 @param nHeightSrc	[in]: Ìº½çÃùº¬Ì© ²ÚËË
 */
void	ScaleXY::Process (CoImage* pDst, int nXOrgDst, int nYOrgDst, int nWidthDst, int nHeightDst,
						  const CoImage* pSrc, int nXOrgSrc, int nYOrgSrc, int nWidthSrc, int nHeightSrc)
{
	assert (pDst);
	assert (pDst->GetChannel() == pSrc->GetChannel());

	int nChannel = pDst->GetChannel();

	if (nChannel == CoImage::enGRAY_MODE)
	{
		Process(&pDst->m_matX, nXOrgDst, nYOrgDst, nWidthDst, nHeightDst,
				&pSrc->m_matX, nXOrgSrc, nYOrgSrc, nWidthSrc, nHeightSrc);
	}
	else if (nChannel == CoImage::enCOLOR_MODE)
	{
		Process(&pDst->m_matX, nXOrgDst, nYOrgDst, nWidthDst, nHeightDst,
				&pSrc->m_matX, nXOrgSrc, nYOrgSrc, nWidthSrc, nHeightSrc);
		Process(&pDst->m_matY, nXOrgDst, nYOrgDst, nWidthDst, nHeightDst,
				&pSrc->m_matY, nXOrgSrc, nYOrgSrc, nWidthSrc, nHeightSrc);
		Process(&pDst->m_matZ, nXOrgDst, nYOrgDst, nWidthDst, nHeightDst,
				&pSrc->m_matZ, nXOrgSrc, nYOrgSrc, nWidthSrc, nHeightSrc);
	}
}

/**
 @brief	ËË ÂÜºã²÷ Ãùº¬¸êÃû»¤ ÇéººÂô¸ó°£Ë¾ ºãÃÔÂÙ³Þ.
 
 @param ppbImage	[In]: Ìºº¬
 @param nX			[In]: Ìºº¬Ëæº· ¸ó°£Ìº¼°Ì© X½ÁÁì
 @param nY			[In]: Ìºº¬Ëæº· ¸ó°£Ìº¼°Ì© Y½ÁÁì
 @param rX			[In]: X¾¢Ì© ·Ë±Â
 @param rY			[In]: Y¾¢Ì© ·Ë±Â
 @return double ÃÍ³ë¼°Ì© ¸ó°£´ô °ª
 */
double ScaleXY::BilinearInterpolation ( BYTE** ppbImage, int nX, int nY, double rX, double rY )
{
	double	rTemp;

	rTemp = ppbImage [ nY ][ nX ];
	if ( rX )
		rTemp += rX * ( ppbImage [ nY ][ nX + 1 ] - ppbImage [ nY ][ nX ] );

	if ( rY )
		rTemp += rY * ( ppbImage [ nY + 1 ][ nX ] - ppbImage [ nY ][ nX ] );

	if ( rX && rY )
		rTemp += rX * rY * (
			ppbImage [ nY + 1 ][ nX + 1 ] + ppbImage [ nY ][ nX ] -
			ppbImage [ nY ][ nX + 1 ] - ppbImage [ nY + 1 ][ nX ] );

	return rTemp;
}

double ScaleXY::BilinearInterpolation ( float** pprImage, int nX, int nY, double rX, double rY )
{
	double	rTemp;
	
	rTemp = pprImage [ nY ][ nX ];
	if ( rX )
		rTemp += rX * ( pprImage [ nY ][ nX + 1 ] - pprImage [ nY ][ nX ] );
	
	if ( rY )
		rTemp += rY * ( pprImage [ nY + 1 ][ nX ] - pprImage [ nY ][ nX ] );
	
	if ( rX && rY )
		rTemp += rX * rY * (
		pprImage [ nY + 1 ][ nX + 1 ] + pprImage [ nY ][ nX ] -
		pprImage [ nY ][ nX + 1 ] - pprImage [ nY + 1 ][ nX ] );
	
	return rTemp;
}

/**
 @brief Ãùº¬Ì© ¾¢ºÏ¶¦ ¼êÃÔÂÙ³Þ.
 
 @param pmatSrc		[in]:	Ìº½çÃùº¬ÃÔµÛ
 @param pmatDest	[out]:	·»¼¬Ãùº¬ÃÔµÛ
 */
void ScaleXY::FastProcess (const Mat* pmatSrc, Mat* pmatDest)
{
	assert (pmatSrc->Type() == MAT_Tbyte);
	assert (pmatDest->Type() == MAT_Tbyte);
	assert (pmatSrc->Type1() == pmatDest->Type1());

	int	i, j, k;
	int	ndstW=pmatDest->Cols();
	int	nShrink = pmatSrc->Cols() / ndstW;
	int	nRest = pmatSrc->Cols() % ndstW;
	int cn=pmatSrc->Channels();
	
	for ( i = 0; i < pmatDest->Rows(); i ++ )
	{
		int		nR, nNR;
		int		nSkip = 0;
		int		nShift = 0;
		BYTE	bNext;
		
		nR = i * pmatSrc->Rows() / pmatDest->Rows();
		nNR = ( i + 1 ) * pmatSrc->Rows() / pmatDest->Rows();
		if ( nNR - nR > 1 )
			bNext = 1;
		else
			bNext = 0;
		BYTE*	pbSrc=pmatSrc->data.ptr[nR];
		BYTE*	pbSrcNext=pmatSrc->data.ptr[nR+bNext];
		BYTE*	pbDst=pmatDest->data.ptr[i];
		if (cn==1)
		{
			for ( j = 0; j < ndstW; j ++ )
			{
				int	nC;
				int	nSum;

				nSkip += nRest;
				nC = j * nShrink + nShift;
				nSum = pbSrc[nC] + pbSrcNext[nC];
				if ( nSkip >= ndstW )
				{
					nShift ++;
					nSkip -= ndstW;
					nSum += pbSrc[nC+1] +pbSrcNext[nC+1];
					nSum >>= 2;
				}
				else
					nSum >>= 1;
				pbDst[j] = ( BYTE )nSum;
			}
		}
		else
		{
			for ( j = 0; j < ndstW; j ++ ) 
			{
				int	nC;
				int	nSum;

				nSkip += nRest;
				nC = j * nShrink + nShift;
				for (k=0;k<cn;k++)
				{
					nSum = pbSrc[ nC*cn+k ] + pbSrcNext[nC*cn+k];
					if ( nSkip >= ndstW )
					{
						nShift ++;
						nSkip -= ndstW;
						nSum += pbSrc[ (nC+1)*cn+k ] + pbSrcNext[ (nC+1)*cn+k ];
						nSum >>= 2;
					}
					else
						nSum >>= 1;
					pbDst[j*cn+k] = ( BYTE )nSum;
				}
			}
		}
	}
}

void ScaleXY::FastProcess (const CoImage* pSrc, CoImage* pDest)
{
	FastProcess(&pSrc->m_matX, &pDest->m_matX);
	FastProcess(&pSrc->m_matY, &pDest->m_matY);
	FastProcess(&pSrc->m_matZ, &pDest->m_matZ);
}

void ScaleXY::Process (CoImage* pDst, const CoImage* pSrc)
{
	Process(pDst, 0, 0, pDst->GetWidth(), pDst->GetHeight(),
		pSrc, 0, 0, pSrc->GetWidth(), pSrc->GetHeight());
}

void ScaleXY::Process (Mat* pDst, const Mat* pSrc)
{
	Process(pDst, 0, 0, pDst->Cols(), pDst->Rows(),
		pSrc, 0, 0, pSrc->Cols(), pSrc->Rows());
}

CoImage* ScaleXY::Process (const CoImage* pSrc, float rScale)
{
	CoImage* pDst = new CoImage( (int)(pSrc->GetHeight() * rScale + 0.5f), (int)(pSrc->GetWidth() * rScale + 0.5f), MAT_Tbyte, CoImage::enCOLOR_MODE);
	Process(pDst, pSrc);
	return pDst;
}

Mat* ScaleXY::Process (const Mat* pSrc, float rScale)
{
	Mat* pDst = new Mat( (int)(pSrc->Rows() * rScale + 0.5f), (int)(pSrc->Cols() * rScale + 0.5f), MAT_Tbyte);
	Process(pDst, pSrc);
	return pDst;
}

IMPLEMENT_PUMP(ipScaleXY, ipCorePump)

bool ipScaleXY::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	ScaleXY scaler;
	if (pmDst)
	{
		pmDst->Release();
		pmDst->Create (m_newHeight, m_newWidth, pmSrc->Type());
		scaler.FastProcess(pmSrc, pmDst);
	}
	else
	{
		Mat mTemp(*pmSrc);
		pmSrc->Release();
		pmSrc->Create (m_newHeight, m_newWidth, mTemp.Type());
		scaler.FastProcess(&mTemp, pmSrc);
	}
	return true;
}

bool ipScaleXY::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	ScaleXY scaler;
	if (pDst)
	{
		pDst->Release();
		pDst->Create (m_newHeight, m_newWidth, (int)pSrc->m_matX.Type(), CoImage::enCOLOR_MODE);
		scaler.FastProcess(pSrc, pDst);
	}
	else
	{
		CoImage temp(*pSrc);
		pSrc->Release();
		pSrc->Create (m_newHeight, m_newWidth, temp.m_matX.Type(), CoImage::enCOLOR_MODE);
		scaler.FastProcess(&temp, pSrc);
	}
	return true;
}

namespace ip
{

void resize(const Mat& src, Mat& dst, long newx, long newy)
{
	dst.Create (newy, newx, src.Type1());
	ScaleXY scaler;
	scaler.FastProcess(&src, &dst);
}

}


}
