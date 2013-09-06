
/*!
 * \file
 * \brief	°¡Ë§»¤Ê¯¿Ğµ¹º÷¶¦ »¨?E?
 * \author	¹ÊËÎ²§
 */

#include "GaussianFilter.h"
#include "cvlibbase/inc/cvlibbaseDef.h"
#include <math.h>

namespace CVLib
{

#ifndef DEFAULT_GAUSSIAN_RADIUS
	#define DEFAULT_GAUSSIAN_RADIUS		3
#endif

#ifndef DEFAULT_GAUSSIAN_SIGMA1
	#define DEFAULT_GAUSSIAN_SIGMA1		1
#endif

#ifndef DEFAULT_GAUSSIAN_SIGMA2
	#define DEFAULT_GAUSSIAN_SIGMA2		2
#endif

#ifndef COLORNUM_8BIT
	#define COLORNUM_8BIT	256
#endif

#ifndef K
	#define K(i, sigma)		exp( - (i*i) / ( 2 * sigma * sigma ) )
#endif

/**
 * @brief °é¾¢?E
 */
GaussianFilter::GaussianFilter()
{
	/*¹¤º¤°ª´ÉË¾ ½ó±¨ÃùÂÙ³Ş.*/
	m_prKernel = NULL;
	SetParam(DEFAULT_GAUSSIAN_SIGMA1, DEFAULT_GAUSSIAN_RADIUS);
}

/**
 * @brief ÃÍ¾Ë?E
 */
GaussianFilter::~GaussianFilter()
{
	if (m_prKernel)
		delete []m_prKernel;
}

void GaussianFilter::GetYBlur(Mat* pmIn, Mat* pmOut, double rSigma)
{
	/*ËÓ(¾¥)µÙÃÔµÛËË ¹¾°º±ı ËÓµÙÃÔµÛËË ·ËÃ£ËÎ °ÒË§ ¼Ç?EÙ³?*/
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nCols = pmIn->Cols();
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (j + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][(nCols-1)*2-(j+k)]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][j + k]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				ppbOut[i][j] = (BYTE)sum;
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (j + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				pprOut[i][j] = sum;
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (j + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				pprOut[i][j] = sum;
			}
	}
}

void GaussianFilter::GetXBlur(Mat* pmIn, Mat* pmOut, double rSigma)
{
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nCols = pmIn->Cols();
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (i + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[(nRows-1)*2 - (i+k)][j]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i + k][j]);
					k ++;
				}

				ppbOut[i][j] = (BYTE)sum;
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (i + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				pprOut[i][j] = sum;
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (i + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				pprOut[i][j] = sum;
			}
	}
}

int GaussianFilter::Process(Mat* pmIn, Mat* pmOut)
{
	/*ËÓµÙÃÔµÛËË±ı ¾¥µÙÃÔµÛËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn || !pmOut)
		return -1;
	/*ËÓµÙÃÔµÛËË ·ËÃ£Â×·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn->IsValid())
		return -1;
	if (!pmIn->IsEqualMat(m_mTemp))
	{
		m_mTemp.Release();
		m_mTemp.Create (*pmIn);
	}
	/*Y¸ÒÂáÌ© °¡Ë§»¤Ê¯?EûÃÔµÛË?ÊĞ²÷³Ş.*/
	GetYBlur(pmIn, &m_mTemp, m_rSigma);
	GetXBlur(&m_mTemp, pmOut, m_rSigma);

	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/
	return 0;
}

int GaussianFilter::ProcessPixel(Mat* pmIn, Mat* pmOut, int row, int col)
{
	/*ËÓµÙÃÔµÛËË±ı ¾¥µÙÃÔµÛËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn || !pmOut)
		return -1;
	/*ËÓµÙÃÔµÛËË ·ËÃ£Â×·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn->IsValid())
		return -1;
	
	if (!pmIn->IsEqualMat(m_mTemp))
	{
		m_mTemp.Release();
		m_mTemp.Create (*pmIn);
	}
	/*Y¸ÒÂáÌ© °¡Ë§»¤Ê¯?EûÃÔµÛË?ÊĞ²÷³Ş.*/
	GetYBlurPixel(pmIn, &m_mTemp, m_rSigma, row, col);
	GetXBlurPixel(&m_mTemp, pmOut, m_rSigma, row, col);
	
	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/
	return 0;
}

int GaussianFilter::ProcessXPixel(Mat* pmIn, Mat* pmOut, int row, int col)
{
	/*ËÓµÙÃÔµÛËË±ı ¾¥µÙÃÔµÛËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn || !pmOut)
		return -1;
	/*ËÓµÙÃÔµÛËË ·ËÃ£Â×·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn->IsValid())
		return -1;
	
	/*Y¸ÒÂáÌ© °¡Ë§»¤Ê¯?EûÃÔµÛË?ÊĞ²÷³Ş.*/
	GetXBlurPixel(pmIn, pmOut, m_rSigma, row, col);
	
	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/
	return 0;
}
int GaussianFilter::ProcessYPixel(Mat* pmIn, Mat* pmOut, int row, int col)
{
	/*ËÓµÙÃÔµÛËË±ı ¾¥µÙÃÔµÛËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn || !pmOut)
		return -1;
	/*ËÓµÙÃÔµÛËË ·ËÃ£Â×·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pmIn->IsValid())
		return -1;
	
	/*Y¸ÒÂáÌ© °¡Ë§»¤Ê¯?EûÃÔµÛË?ÊĞ²÷³Ş.*/
	GetYBlurPixel(pmIn, pmOut, m_rSigma, row, col);
	
	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/
	return 0;
}

int GaussianFilter::Process(CoImage* pImgSrc, CoImage* pImgDst)
{
	/*ËÓµÙÃùº¬ËË±ı ¾¥µÙÃùº¬ËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*ËÓµÙÃùº¬ËË ²Ù³èÃùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*XÀÒµá?E´İÂ×ÊŞº·?E¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		return Process(&pImgSrc->m_matX, &pImgDst->m_matX);
	}

	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*ËÓµÙÃùº¬ËË »°Ãùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*X, Y, ZÀÒµá?E´İÂ×ÊŞ ¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		nResultX = Process(&pImgSrc->m_matX, &pImgDst->m_matX);
		nResultY = Process(&pImgSrc->m_matY, &pImgDst->m_matY);
		nResultZ = Process(&pImgSrc->m_matZ, &pImgDst->m_matZ);
	}

	/*ÊÌ?EÂ×±ıÌ© ¹¹µÍ½å¶®µ¹´ª »¨Â²ËÎ °ÒË§ ?E±¿¸´Å¶?´®µØ¼Ó³Ş.*/
	if (nResultX || nResultY || nResultZ)
		return -1;

	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/	
	return 0;
}

int GaussianFilter::ProcessPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col)
{
	/*ËÓµÙÃùº¬ËË±ı ¾¥µÙÃùº¬ËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*ËÓµÙÃùº¬ËË ²Ù³èÃùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*XÀÒµá?E´İÂ×ÊŞº·?E¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		return ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX, row, col);
	}
	
	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*ËÓµÙÃùº¬ËË »°Ãùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*X, Y, ZÀÒµá?E´İÂ×ÊŞ ¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		nResultX = ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX,  row, col);
		nResultY = ProcessPixel(&pImgSrc->m_matY, &pImgDst->m_matY,  row, col);
		nResultZ = ProcessPixel(&pImgSrc->m_matZ, &pImgDst->m_matZ,  row, col);
	}
	
	/*ÊÌ?EÂ×±ıÌ© ¹¹µÍ½å¶®µ¹´ª »¨Â²ËÎ °ÒË§ ?E±¿¸´Å¶?´®µØ¼Ó³Ş.*/
	if (nResultX || nResultY || nResultZ)
		return -1;
	
	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/	
	return 0;
}
int GaussianFilter::ProcessXPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col)
{
	/*ËÓµÙÃùº¬ËË±ı ¾¥µÙÃùº¬ËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*ËÓµÙÃùº¬ËË ²Ù³èÃùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*XÀÒµá?E´İÂ×ÊŞº·?E¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		return ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX, row, col);
	}
	
	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*ËÓµÙÃùº¬ËË »°Ãùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*X, Y, ZÀÒµá?E´İÂ×ÊŞ ¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		nResultX = ProcessXPixel(&pImgSrc->m_matX, &pImgDst->m_matX,  row, col);
		nResultY = ProcessXPixel(&pImgSrc->m_matY, &pImgDst->m_matY,  row, col);
		nResultZ = ProcessXPixel(&pImgSrc->m_matZ, &pImgDst->m_matZ,  row, col);
	}
	
	/*ÊÌ?EÂ×±ıÌ© ¹¹µÍ½å¶®µ¹´ª »¨Â²ËÎ °ÒË§ ?E±¿¸´Å¶?´®µØ¼Ó³Ş.*/
	if (nResultX || nResultY || nResultZ)
		return -1;
	
	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/	
	return 0;
}

int GaussianFilter::ProcessYPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col)
{
	/*ËÓµÙÃùº¬ËË±ı ¾¥µÙÃùº¬ËË ¼Á¼õÂ×?EÊ±Ëº·² ?E±¿¸´Å¶?´®¶°³Ş.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*ËÓµÙÃùº¬ËË ²Ù³èÃùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*XÀÒµá?E´İÂ×ÊŞº·?E¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		return ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX, row, col);
	}
	
	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*ËÓµÙÃùº¬ËË »°Ãùº¬ËÎ °ÒË§Ëæ?E*/
	{
		/*X, Y, ZÀÒµá?E´İÂ×ÊŞ ¹¹µÍ½å¶®¶¦ ?EÔÂÙ³?*/
		nResultX = ProcessYPixel(&pImgSrc->m_matX, &pImgDst->m_matX,  row, col);
		nResultY = ProcessYPixel(&pImgSrc->m_matY, &pImgDst->m_matY,  row, col);
		nResultZ = ProcessYPixel(&pImgSrc->m_matZ, &pImgDst->m_matZ,  row, col);
	}
	
	/*ÊÌ?EÂ×±ıÌ© ¹¹µÍ½å¶®µ¹´ª »¨Â²ËÎ °ÒË§ ?E±¿¸´Å¶?´®µØ¼Ó³Ş.*/
	if (nResultX || nResultY || nResultZ)
		return -1;
	
	/*ºÂ°á¿¸´Å¶¦ ´®µØ¼Ó³Ş.*/	
	return 0;
}

void GaussianFilter::SetParam(double rSigma, double rRadius)
{
	/*¹¤º¤?E¸É°Ò°ª´ÉË¾ º¼¼³ÂÙ³Ş.*/
	m_rSigma = rSigma;
	m_rRadius = rRadius;

	/*°¡Ë§»¤Ê¯¿¤³º¹ÉµÛË¾ Ãú¸óÂ×°Ö...*/
	if (m_prKernel)
		delete m_prKernel;

	/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	/*µÌ¾°(¹¢, ¼³)Ì© ¸É°Ò°ª?Eµß(0)°ªË¾ À°º¤Â×ÊŞ ¹ÉµÛÌ© ¿Í±¨¶¦ ¼³ÂÙ³Ş.*/
	int nCntK = nSideR * 2 + 1;
	m_prKernel = new double[nCntK];
	/*¿¤³º´ÉË¾ ±Ëº¤Â×ÊŞ ¹ÉµÛ?E²Ä²÷³Ş.*/
	double sum = 0.0;
	int idxK;
	for (idxK = 0; idxK < nCntK; idxK ++)
	{
		//m_prKernel[idxK] = K(idxK - nSideR, rSigma);
		double rBoy = (idxK - nSideR) * (idxK - nSideR);
		double rMom = 2 * rSigma * rSigma;
		double rShoulder = - rBoy / rMom;
		m_prKernel[idxK] = exp(rShoulder);
		sum += m_prKernel[idxK];
	}
	for (idxK = 0; idxK < nCntK; idxK ++)
		m_prKernel[idxK] /= sum;
}

void GaussianFilter::GetKernel(double rSigma, double rRadius, Vecd& vKernel)
{
	vKernel.Release();
	/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	/*µÌ¾°(¹¢, ¼³)Ì© ¸É°Ò°ª?Eµß(0)°ªË¾ À°º¤Â×ÊŞ ¹ÉµÛÌ© ¿Í±¨¶¦ ¼³ÂÙ³Ş.*/
	int nCntK = nSideR * 2 + 1;
	vKernel.Create (nCntK);
	double* prKernel = vKernel.data.db;
	/*¿¤³º´ÉË¾ ±Ëº¤Â×ÊŞ ¹ÉµÛ?E²Ä²÷³Ş.*/
	double sum = 0.0;
	int idxK;
	for (idxK = 0; idxK < nCntK; idxK ++)
	{
		//m_prKernel[idxK] = K(idxK - nSideR, rSigma);
		double rBoy = (idxK - nSideR) * (idxK - nSideR);
		double rMom = 2 * rSigma * rSigma;
		double rShoulder = - rBoy / rMom;
		prKernel[idxK] = exp(rShoulder);
		sum += prKernel[idxK];
	}
	for (idxK = 0; idxK < nCntK; idxK ++)
		prKernel[idxK] /= sum;
}

void GaussianFilter::GetKernel2D(double rSigma1, double rSigma2, double rRadius, Matd& mKernel)
{
	Vecd vK1, vK2;
	GetKernel(rSigma1, rRadius, vK1);
	GetKernel(rSigma2, rRadius, vK2);
	mKernel.Release();
	mKernel.Create(vK2.Length(), vK1.Length());
	for (int i=0; i<mKernel.Rows(); i++)
	{
		for (int j=0; j<mKernel.Cols(); j++)
		{
			mKernel[i][j] = vK1[j]*vK2[i];
		}
	}
}

void GaussianFilter::GetYBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col)
{
	/*ËÓ(¾¥)µÙÃÔµÛËË ¹¾°º±ı ËÓµÙÃÔµÛËË ·ËÃ£ËÎ °ÒË§ ¼Ç?EÙ³?*/
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nCols = pmIn->Cols();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;

		int nR = (int)(m_rRadius * rSigma + 0.5f);
		i = row;
		j = col;

			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (j + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][(nCols-1)*2-(j+k)]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][j + k]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				if (ppbOut[i][j] < (BYTE)sum)
				{
					ppbOut[i][j] = (BYTE)sum;
				}
					
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (j + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (j + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
}

void GaussianFilter::GetXBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col)
{
	/*ËÓ(¾¥)µÙÃÔµÛËË ¹¾°º±ı ËÓµÙÃÔµÛËË ·ËÃ£ËÎ °ÒË§ ¼Ç?EÙ³?*/
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;
			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
 					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (i + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[(nRows-1)*2 - (i+k)][j]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * ppbIn[i + k][j]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				if (ppbOut[i][j] < (BYTE)sum)
				{
					ppbOut[i][j] = (BYTE)sum;
				}
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (i + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += ( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		/*ËÓ¾¥µÙÃÔµÛ´ÉÌ© »ô?E?¶²»¤?EãËE¸Ê²÷³Ş.*/
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		/*°¡Ë§»¤Ê¯¸É°Ò°ªË¾ º¼¼³ÂÙ³Ş.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*ËÓµÙÃÔµÛÌ© ?EÌººÏ´É?E´İÂ×ÊŞ ¹¹µÍ½å¶®Â×ÊŞ ¾¥µÙÃÔµÛ?E´İËÓÂÙ³Ş.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k¶¦ -r¹¢À¾ rÄ´?EºåÃû»¤¿° K(kernel)?E¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏ¶¦ °ßÂÙ°ÃË¾ Âİ±ËÂÙ³Ş.*/
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k¶¦ ¶®Ë¦Â×ÊŞ ¸õ¶· ÃÔµÛÌ© ÃÍ?EÌººÏÌ® K¶¦ °ßÂ×ÊŞ Âİ±ËÂÙ³Ş.*/
					/*°¡Ë§»¤Ê¯ÂÜºã°¡ Ãùº¬µßÊßË¾ ¸ãÊÌ²¤ °ÒË§Ëæ?E°Ò±Ë¶¦ ´İ¾ÀËº?EÂ×ÊŞ Ãùº¬Ëæº· ´İËÄ´ó?E¼°Ì©
 					°ªË¾ ¶®Ë¦ÂÙ³Ş.*/
					if (i + k < 0)				/*Ãùº¬Ì© ËùÉ¸¸ÒÂáËº?E¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*Ãùº¬Ì© ?E?¸¸ÒÂáËºµE¸ãÊÌ²¤ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*¸ãÊÌ±ı?EÊ±Ë¾ °ÒË§*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				/*¾¥µÙÃÔµÛÌ© ÃÍ?EÌººÏ?Eµñ¼¬°ªË¾ ´İËÓÂÙ³Ş.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
}

int GaussianFilter::ProcessCircle(CoImage* pImgSrc, CoImage* pImgDst, int nCircleRadius, int nSigma,  int nSmoothRaduis, int row, int col)
{
	int nCurrentrow, nCurrentcol;
	for (int noffsetY = -nCircleRadius; noffsetY <= nCircleRadius; noffsetY++)
	{
		double dLimitX = sqrt((double)SQR(nCircleRadius) - SQR(noffsetY) );
		for (int noffsetX = - (int)dLimitX; noffsetX < (int)dLimitX; noffsetX++)
		{
			nCurrentrow = row + noffsetY;
			nCurrentcol = col + noffsetX;
			double dRadius = sqrt((double)SQR(noffsetY)+SQR(noffsetX));
			double dCurrentSmoothRaduis = (double)nSmoothRaduis * (nCircleRadius - dRadius) / nCircleRadius;
			double dCurrentSmoothSigma = (double)nSigma * (nCircleRadius - dRadius) / nCircleRadius +0.1f ;
			SetParam(dCurrentSmoothSigma, dCurrentSmoothRaduis);
			ProcessPixel(pImgSrc, pImgDst,nCurrentrow, nCurrentcol);
		}
	}
	return 1;
}

int GaussianFilter::ProcessXCircle(CoImage* pImgSrc, CoImage* pImgDst, int nCircleRadius, int nSigma,  int nSmoothRaduis, int row, int col)
{
	int nCurrentrow, nCurrentcol;
	for (int noffsetY = -nCircleRadius; noffsetY <= nCircleRadius; noffsetY++)
	{
		double dLimitX = sqrt((double)SQR(nCircleRadius) - SQR(noffsetY) );
		for (int noffsetX = - (int)dLimitX; noffsetX < (int)dLimitX; noffsetX++)
		{
			nCurrentrow = row + noffsetY;
			nCurrentcol = col + noffsetX;
			double dRadius = sqrt((double)SQR(noffsetY)+SQR(noffsetX));
			double dCurrentSmoothRaduis = (double)nSmoothRaduis * (nCircleRadius - dRadius) / nCircleRadius;
			SetParam(nSigma, dCurrentSmoothRaduis);
			ProcessXPixel(pImgSrc, pImgDst,nCurrentrow, nCurrentcol);
		}
	}
	return 1;
}

int GaussianFilter::ProcessYCircle(CoImage* pImgSrc, CoImage* pImgDst, int nCircleRadius, int nSigma,  int nSmoothRaduis, int row, int col)
{
	int nCurrentrow, nCurrentcol;
	for (int noffsetY = -nCircleRadius; noffsetY <= nCircleRadius; noffsetY++)
	{
		double dLimitX = sqrt((double)SQR(nCircleRadius) - SQR(noffsetY) );
		for (int noffsetX = - (int)dLimitX; noffsetX < (int)dLimitX; noffsetX++)
		{
			nCurrentrow = row + noffsetY;
			nCurrentcol = col + noffsetX;
			double dRadius = sqrt((double)SQR(noffsetY)+SQR(noffsetX));
			double dCurrentSmoothRaduis = (double)nSmoothRaduis * (nCircleRadius - dRadius) / nCircleRadius;
			double dCurrentSmoothSigma = (double)nSigma * (nCircleRadius - dRadius) / nCircleRadius +0.1f ;
			SetParam(dCurrentSmoothSigma, dCurrentSmoothRaduis);
			ProcessYPixel(pImgSrc, pImgDst,nCurrentrow, nCurrentcol);
		}
	}
	return 1;
}

GaussianDiffFilter::GaussianDiffFilter()
{
	m_prKernel=NULL;
	m_prDiffKernel=NULL;
}

GaussianDiffFilter::~GaussianDiffFilter()
{
	if (m_prKernel)
		delete []m_prKernel;
	if (m_prDiffKernel)
		delete []m_prDiffKernel;
}
	
void GaussianDiffFilter::SetParam (double rSigma, double rRadius)
{
	m_rSigma = rSigma;
	m_rRadius = rRadius;

	if (m_prKernel)
		delete []m_prKernel;
	if (m_prDiffKernel)
		delete []m_prDiffKernel;
	
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	int nCntK = nSideR * 2 + 1;
	m_prKernel = new double[nCntK];
	m_prDiffKernel = new double[nCntK];
	double sum = 0.0;
	double rk=-1./sqrt(2*CVLIB_PI) / pow(rSigma, 3.0);
	int idxK;
	for (idxK = 0; idxK < nCntK; idxK ++)
	{
		double rBoy = (idxK - nSideR) * (idxK - nSideR);
		double rMom = 2 * rSigma * rSigma;
		double rShoulder = - rBoy / rMom;
		m_prKernel[idxK] = exp(rShoulder);
		m_prDiffKernel[idxK] = rk * (idxK-nSideR) * m_prKernel[idxK];
		sum += m_prKernel[idxK];
	}
	for (idxK = 0; idxK < nCntK; idxK ++)
		m_prKernel[idxK] /= sum;
}

void GaussianDiffFilter::GetKernel(double rSigma, double rRadius, Vecd& vKernel)
{
	vKernel.Release();
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	int nCntK = nSideR * 2 + 1;
	vKernel.Create (nCntK);
	double* prKernel = vKernel.data.db;
	double sum = 0.0;
	int idxK;
	for (idxK = 0; idxK < nCntK; idxK ++)
	{
		double rBoy = (idxK - nSideR) * (idxK - nSideR);
		double rMom = 2 * rSigma * rSigma;
		double rShoulder = - rBoy / rMom;
		prKernel[idxK] = exp(rShoulder);
		sum += prKernel[idxK];
	}
	for (idxK = 0; idxK < nCntK; idxK ++)
		prKernel[idxK] /= sum;
}
	
void GaussianDiffFilter::GetDiffKernel(double rSigma, double rRadius, Vecd& vKernel)
{
	vKernel.Release();
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	int nCntK = nSideR * 2 + 1;
	vKernel.Create (nCntK);
	double* prKernel = vKernel.data.db;
	double rk=1./sqrt(2*CVLIB_PI) / pow(rSigma, 3.0);
	int idxK;
	for (idxK = 0; idxK < nCntK; idxK ++)
	{
		double rBoy = (idxK - nSideR) * (idxK - nSideR);
		double rMom = 2 * rSigma * rSigma;
		double rShoulder = - rBoy / rMom;
		prKernel[idxK] = rk * (idxK-nSideR) * exp(rShoulder);
	}
}

int GaussianDiffFilter::ProcessX(Mat* pmIn, Mat* pmOut)
{
	if (!pmIn || !pmOut)
		return -1;
	if (!pmIn->IsValid())
		return -1;
	
	if (!pmIn->IsEqualMat(m_mTemp))
	{
		m_mTemp.Release();
		m_mTemp.Create (*pmIn);
	}
	GetXProcess(pmIn, &m_mTemp, m_prDiffKernel);
	GetYProcess(&m_mTemp, pmOut, m_prKernel);
	return 0;
}

int GaussianDiffFilter::ProcessY(Mat* pmIn, Mat* pmOut)
{
	if (!pmIn || !pmOut)
		return -1;
	if (!pmIn->IsValid())
		return -1;
	
	if (!pmIn->IsEqualMat(m_mTemp))
	{
		m_mTemp.Release();
		m_mTemp.Create (*pmIn);
	}
	GetYProcess(pmIn, &m_mTemp, m_prDiffKernel);
	GetXProcess(&m_mTemp, pmOut, m_prKernel);
	return 0;
}

void GaussianDiffFilter::GetXProcess(Mat* pmIn, Mat* pmOut, double* prKernel)
{
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nCols = pmIn->Cols();
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;
		int nR = (int)(m_rRadius * m_rSigma + 0.5f);
		for ( i = 0; i < nRows; i ++)
		{
			for ( j = 0; j < nCols; j ++)
			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (j + k < 0)
						sum += ( prKernel[k + nR] * ppbIn[i][-(j+k)]);
					else if (j + k >= nCols)
						sum += ( prKernel[k + nR] * ppbIn[i][(nCols-1)*2-(j+k)]);
					else
						sum += ( prKernel[k + nR] * ppbIn[i][j + k]);
					k ++;
				}

				ppbOut[i][j] = (BYTE)sum;
			}
		}
	}
	else if (pmIn->Type() == MAT_Tfloat)
	{
		float** ppbIn = pmIn->data.fl;
		float** ppbOut = pmOut->data.fl;
		
		int i, j, k;
		int nR = (int)(m_rRadius * m_rSigma + 0.5f);
		for ( i = 0; i < nRows; i ++)
		{
			for ( j = 0; j < nCols; j ++)
			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (j + k < 0)
						sum += ( prKernel[k + nR] * ppbIn[i][-(j+k)]);
					else if (j + k >= nCols)
						sum += ( prKernel[k + nR] * ppbIn[i][(nCols-1)*2-(j+k)]);
					else
						sum += ( prKernel[k + nR] * ppbIn[i][j + k]);
					k ++;
				}
				
				ppbOut[i][j] = (float)sum;
			}
		}
	}
}

void GaussianDiffFilter::GetYProcess(Mat* pmIn, Mat* pmOut, double* prKernel)
{
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nCols = pmIn->Cols();
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;
		
		int i, j, k;
		int nR = (int)(m_rRadius * m_rSigma + 0.5f);
		for ( j = 0; j < nCols; j ++)
		{
			for ( i = 0; i < nRows; i ++)
			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (i + k < 0)
						sum += ( prKernel[k + nR] * ppbIn[-(i+k)][j]);
					else if (i + k >= nRows)
						sum += ( prKernel[k + nR] * ppbIn[(nRows-1)*2 - (i+k)][j]);
					else
						sum += ( prKernel[k + nR] * ppbIn[i + k][j]);
					k ++;
				}
				
				ppbOut[i][j] = (BYTE)sum;
			}
		}
	}
	else if (pmIn->Type() == MAT_Tfloat)
	{
		float** ppbIn = pmIn->data.fl;
		float** ppbOut = pmOut->data.fl;
		
		int i, j, k;
		int nR = (int)(m_rRadius * m_rSigma + 0.5f);
		for ( j = 0; j < nCols; j ++)
		{
			for ( i = 0; i < nRows; i ++)
			{
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					if (i + k < 0)
						sum += ( prKernel[k + nR] * ppbIn[-(i+k)][j]);
					else if (i + k >= nRows)
						sum += ( prKernel[k + nR] * ppbIn[(nRows-1)*2 - (i+k)][j]);
					else
						sum += ( prKernel[k + nR] * ppbIn[i + k][j]);
					k ++;
				}
				
				ppbOut[i][j] = (float)sum;
			}
		}
	}

}

}