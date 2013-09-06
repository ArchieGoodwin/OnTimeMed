
/*!
 * \file
 * \brief	��˧��ʯ�е����� ��?E?
 * \author	���β�
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
 * @brief �龢?E
 */
GaussianFilter::GaussianFilter()
{
	/*��������˾ �����ٳ�.*/
	m_prKernel = NULL;
	SetParam(DEFAULT_GAUSSIAN_SIGMA1, DEFAULT_GAUSSIAN_RADIUS);
}

/**
 * @brief �;�?E
 */
GaussianFilter::~GaussianFilter()
{
	if (m_prKernel)
		delete []m_prKernel;
}

void GaussianFilter::GetYBlur(Mat* pmIn, Mat* pmOut, double rSigma)
{
	/*��(��)���Ե��� ������ �ӵ��Ե��� ��ã�� ��˧ ��?Eٳ?*/
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nCols = pmIn->Cols();
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (j + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][(nCols-1)*2-(j+k)]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][j + k]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				ppbOut[i][j] = (BYTE)sum;
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (j + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				pprOut[i][j] = sum;
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
		for ( i = 0; i < nRows; i ++)
			for ( j = 0; j < nCols; j ++)
			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (j + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
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
					if (i + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[(nRows-1)*2 - (i+k)][j]);
					else						/*���̱�?Eʱ˾ ��˧*/
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
					if (i + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*���̱�?Eʱ˾ ��˧*/
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
					if (i + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				pprOut[i][j] = sum;
			}
	}
}

int GaussianFilter::Process(Mat* pmIn, Mat* pmOut)
{
	/*�ӵ��Ե��˱� �����Ե��� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pmIn || !pmOut)
		return -1;
	/*�ӵ��Ե��� ��ã�׷� ?E����Ŷ?������.*/
	if (!pmIn->IsValid())
		return -1;
	if (!pmIn->IsEqualMat(m_mTemp))
	{
		m_mTemp.Release();
		m_mTemp.Create (*pmIn);
	}
	/*Y����̩ ��˧��ʯ?E��Ե��?�в���.*/
	GetYBlur(pmIn, &m_mTemp, m_rSigma);
	GetXBlur(&m_mTemp, pmOut, m_rSigma);

	/*�°Ὸ�Ŷ� ���ؼӳ�.*/
	return 0;
}

int GaussianFilter::ProcessPixel(Mat* pmIn, Mat* pmOut, int row, int col)
{
	/*�ӵ��Ե��˱� �����Ե��� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pmIn || !pmOut)
		return -1;
	/*�ӵ��Ե��� ��ã�׷� ?E����Ŷ?������.*/
	if (!pmIn->IsValid())
		return -1;
	
	if (!pmIn->IsEqualMat(m_mTemp))
	{
		m_mTemp.Release();
		m_mTemp.Create (*pmIn);
	}
	/*Y����̩ ��˧��ʯ?E��Ե��?�в���.*/
	GetYBlurPixel(pmIn, &m_mTemp, m_rSigma, row, col);
	GetXBlurPixel(&m_mTemp, pmOut, m_rSigma, row, col);
	
	/*�°Ὸ�Ŷ� ���ؼӳ�.*/
	return 0;
}

int GaussianFilter::ProcessXPixel(Mat* pmIn, Mat* pmOut, int row, int col)
{
	/*�ӵ��Ե��˱� �����Ե��� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pmIn || !pmOut)
		return -1;
	/*�ӵ��Ե��� ��ã�׷� ?E����Ŷ?������.*/
	if (!pmIn->IsValid())
		return -1;
	
	/*Y����̩ ��˧��ʯ?E��Ե��?�в���.*/
	GetXBlurPixel(pmIn, pmOut, m_rSigma, row, col);
	
	/*�°Ὸ�Ŷ� ���ؼӳ�.*/
	return 0;
}
int GaussianFilter::ProcessYPixel(Mat* pmIn, Mat* pmOut, int row, int col)
{
	/*�ӵ��Ե��˱� �����Ե��� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pmIn || !pmOut)
		return -1;
	/*�ӵ��Ե��� ��ã�׷� ?E����Ŷ?������.*/
	if (!pmIn->IsValid())
		return -1;
	
	/*Y����̩ ��˧��ʯ?E��Ե��?�в���.*/
	GetYBlurPixel(pmIn, pmOut, m_rSigma, row, col);
	
	/*�°Ὸ�Ŷ� ���ؼӳ�.*/
	return 0;
}

int GaussianFilter::Process(CoImage* pImgSrc, CoImage* pImgDst)
{
	/*�ӵ������˱� ���������� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*�ӵ������� �ٳ������� ��˧��?E*/
	{
		/*X�ҵ�?E�����޺�?E���ͽ嶮�� ?E��ٳ?*/
		return Process(&pImgSrc->m_matX, &pImgDst->m_matX);
	}

	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*�ӵ������� �������� ��˧��?E*/
	{
		/*X, Y, Z�ҵ�?E������ ���ͽ嶮�� ?E��ٳ?*/
		nResultX = Process(&pImgSrc->m_matX, &pImgDst->m_matX);
		nResultY = Process(&pImgSrc->m_matY, &pImgDst->m_matY);
		nResultZ = Process(&pImgSrc->m_matZ, &pImgDst->m_matZ);
	}

	/*��?E�ױ�̩ ���ͽ嶮���� ��²�� ��˧ ?E����Ŷ?���ؼӳ�.*/
	if (nResultX || nResultY || nResultZ)
		return -1;

	/*�°Ὸ�Ŷ� ���ؼӳ�.*/	
	return 0;
}

int GaussianFilter::ProcessPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col)
{
	/*�ӵ������˱� ���������� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*�ӵ������� �ٳ������� ��˧��?E*/
	{
		/*X�ҵ�?E�����޺�?E���ͽ嶮�� ?E��ٳ?*/
		return ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX, row, col);
	}
	
	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*�ӵ������� �������� ��˧��?E*/
	{
		/*X, Y, Z�ҵ�?E������ ���ͽ嶮�� ?E��ٳ?*/
		nResultX = ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX,  row, col);
		nResultY = ProcessPixel(&pImgSrc->m_matY, &pImgDst->m_matY,  row, col);
		nResultZ = ProcessPixel(&pImgSrc->m_matZ, &pImgDst->m_matZ,  row, col);
	}
	
	/*��?E�ױ�̩ ���ͽ嶮���� ��²�� ��˧ ?E����Ŷ?���ؼӳ�.*/
	if (nResultX || nResultY || nResultZ)
		return -1;
	
	/*�°Ὸ�Ŷ� ���ؼӳ�.*/	
	return 0;
}
int GaussianFilter::ProcessXPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col)
{
	/*�ӵ������˱� ���������� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*�ӵ������� �ٳ������� ��˧��?E*/
	{
		/*X�ҵ�?E�����޺�?E���ͽ嶮�� ?E��ٳ?*/
		return ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX, row, col);
	}
	
	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*�ӵ������� �������� ��˧��?E*/
	{
		/*X, Y, Z�ҵ�?E������ ���ͽ嶮�� ?E��ٳ?*/
		nResultX = ProcessXPixel(&pImgSrc->m_matX, &pImgDst->m_matX,  row, col);
		nResultY = ProcessXPixel(&pImgSrc->m_matY, &pImgDst->m_matY,  row, col);
		nResultZ = ProcessXPixel(&pImgSrc->m_matZ, &pImgDst->m_matZ,  row, col);
	}
	
	/*��?E�ױ�̩ ���ͽ嶮���� ��²�� ��˧ ?E����Ŷ?���ؼӳ�.*/
	if (nResultX || nResultY || nResultZ)
		return -1;
	
	/*�°Ὸ�Ŷ� ���ؼӳ�.*/	
	return 0;
}

int GaussianFilter::ProcessYPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col)
{
	/*�ӵ������˱� ���������� ������?Eʱ˺�� ?E����Ŷ?������.*/
	if (!pImgSrc || !pImgDst)
		return -1;
	
	if (pImgSrc->GetChannel() == CoImage::enGRAY_MODE)	/*�ӵ������� �ٳ������� ��˧��?E*/
	{
		/*X�ҵ�?E�����޺�?E���ͽ嶮�� ?E��ٳ?*/
		return ProcessPixel(&pImgSrc->m_matX, &pImgDst->m_matX, row, col);
	}
	
	int nResultX = 1, nResultY = 0, nResultZ = 0;
	if (pImgSrc->GetChannel() == CoImage::enCOLOR_MODE)	/*�ӵ������� �������� ��˧��?E*/
	{
		/*X, Y, Z�ҵ�?E������ ���ͽ嶮�� ?E��ٳ?*/
		nResultX = ProcessYPixel(&pImgSrc->m_matX, &pImgDst->m_matX,  row, col);
		nResultY = ProcessYPixel(&pImgSrc->m_matY, &pImgDst->m_matY,  row, col);
		nResultZ = ProcessYPixel(&pImgSrc->m_matZ, &pImgDst->m_matZ,  row, col);
	}
	
	/*��?E�ױ�̩ ���ͽ嶮���� ��²�� ��˧ ?E����Ŷ?���ؼӳ�.*/
	if (nResultX || nResultY || nResultZ)
		return -1;
	
	/*�°Ὸ�Ŷ� ���ؼӳ�.*/	
	return 0;
}

void GaussianFilter::SetParam(double rSigma, double rRadius)
{
	/*����?E�ɰҰ���˾ �����ٳ�.*/
	m_rSigma = rSigma;
	m_rRadius = rRadius;

	/*��˧��ʯ�����ɵ�˾ �����װ�...*/
	if (m_prKernel)
		delete m_prKernel;

	/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	/*�̾�(��, ��)̩ �ɰҰ�?E��(0)��˾ �������� �ɵ�̩ �ͱ��� ���ٳ�.*/
	int nCntK = nSideR * 2 + 1;
	m_prKernel = new double[nCntK];
	/*������˾ �˺����� �ɵ�?E�Ĳ���.*/
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
	/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
	int nSideR = (int)(rSigma * rRadius + 0.5f);
	/*�̾�(��, ��)̩ �ɰҰ�?E��(0)��˾ �������� �ɵ�̩ �ͱ��� ���ٳ�.*/
	int nCntK = nSideR * 2 + 1;
	vKernel.Create (nCntK);
	double* prKernel = vKernel.data.db;
	/*������˾ �˺����� �ɵ�?E�Ĳ���.*/
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
	/*��(��)���Ե��� ������ �ӵ��Ե��� ��ã�� ��˧ ��?Eٳ?*/
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
					if (j + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][(nCols-1)*2-(j+k)]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i][j + k]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				if (ppbOut[i][j] < (BYTE)sum)
				{
					ppbOut[i][j] = (BYTE)sum;
				}
					
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (j + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (j + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][-(j+k)]);
					else if (j + k >= nCols)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][2*(nCols-1)-(j+k)]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i][j + k]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
}

void GaussianFilter::GetXBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col)
{
	/*��(��)���Ե��� ������ �ӵ��Ե��� ��ã�� ��˧ ��?Eٳ?*/
	if (!pmIn || !pmOut || !pmIn->IsValid())
		return;
	
	int nRows = pmIn->Rows();
	
	if (pmIn->Type() == MAT_Tbyte)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		unsigned char** ppbIn = pmIn->data.ptr;
		unsigned char** ppbOut = pmOut->data.ptr;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;
			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
 					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (i + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[(nRows-1)*2 - (i+k)][j]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += ( m_prKernel[k + nR] * ppbIn[i + k][j]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				if (ppbOut[i][j] < (BYTE)sum)
				{
					ppbOut[i][j] = (BYTE)sum;
				}
			}
	}
	if (pmIn->Type() == MAT_Tdouble)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		double** pprIn = pmIn->data.db;
		double** pprOut = pmOut->data.db;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				double sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (i + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += ( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
				if (pprOut[i][j] < (BYTE)sum)
				{
					pprOut[i][j] = (BYTE)sum;
				}
			}
	}
	if (pmIn->Type() == MAT_Tfloat)
	{
		/*�Ӿ����Ե۴�̩ ��?E?����?E�ˁE�ʲ���.*/
		float** pprIn = pmIn->data.fl;
		float** pprOut = pmOut->data.fl;

		int i, j, k;
		/*��˧��ʯ�ɰҰ�˾ �����ٳ�.*/
		int nR = (int)(m_rRadius * rSigma + 0.5f);
		/*�ӵ��Ե�̩ ?E̺�ϴ�?E������ ���ͽ嶮���� �����Ե�?E�����ٳ�.*/
// 		for ( i = 0; i < nRows; i ++)
// 			for ( j = 0; j < nCols; j ++)
		i = row;
		j = col;

			{
				/*k�� -r���� rĴ?E�������� K(kernel)?E���� �Ե�̩ ��?E̺�϶� ���ٰ�˾ �ݱ��ٳ�.*/
				float sum = 0.0;
				k = -nR;
				while ( k <= nR )
				{
					/*k�� ��˦���� ���� �Ե�̩ ��?E̺��̮ K�� ������ �ݱ��ٳ�.*/
					/*��˧��ʯ�ܺ㰡 ��������˾ ���̲� ��˧��?E�ұ˶� �ݾ�˺?E���� �����溷 ���Ĵ�?E��̩
 					��˾ ��˦�ٳ�.*/
					if (i + k < 0)				/*����̩ ��ɸ����˺?E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[-(i+k)][j]);
					else if (i + k >= nRows)	/*����̩ ?E?�����˺��E���̲� ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[(nRows-1)*2 - (i+k)][j]);
					else						/*���̱�?Eʱ˾ ��˧*/
						sum += (float)( m_prKernel[k + nR] * pprIn[i + k][j]);
					k ++;
				}

				/*�����Ե�̩ ��?E̺��?E�񼬰�˾ �����ٳ�.*/
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