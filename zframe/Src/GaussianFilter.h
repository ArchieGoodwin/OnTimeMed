
/*!
 * \file
 * \brief	��˧��ʯ�е����� ������.
 * \author	���β�
 */

#if !defined(_GaussianFilter_H_)
#define _GaussianFilter_H_

#include "cvlibbase/inc/CoImage.h"

namespace CVLib
{

/**
 * @brief ��˧��ʯ�е���
 */
class CVLIB_DECLSPEC GaussianFilter : public Object
{
public:
	//! �龢��
	GaussianFilter();
	//! �ıл�
	virtual ~GaussianFilter();

	/**
	 * @brief �ӵ��Ե��� ������ X, Y���ṹ�ͽ嶮�� �������� �����Ե��� ���̼ӳ�.
	 *
	 * @param pmIn [in] : �ӵ��Ե�
	 * @param pmOut [out] : �����Ե�
	 * @return �°� : 0 , ��² : ��˱����
	 */
	int Process(Mat* pmIn, Mat* pmOut);
	/**
	 * @brief �ӵ������� ������ X, Y���ṹ�ͽ嶮�� �������� ���������� ���̼ӳ�.
	 *
	 * @param pImgSrc [in] : �ӵ�����
	 * @param pImgDst [out] : ���ͽ嶮�� ��������
	 * @return �°� : 0 , ��² : ��˱����
	 */
	int ProcessPixel(Mat* pmIn, Mat* pmOut, int row, int col);
	int ProcessXPixel(Mat* pmIn, Mat* pmOut, int row, int col);
	int ProcessYPixel(Mat* pmIn, Mat* pmOut, int row, int col);


	int Process(CoImage* pImgSrc, CoImage* pImgDst);

	int ProcessPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col);

	int ProcessXPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col);

	int ProcessYPixel(CoImage* pImgSrc, CoImage* pImgDst, int row, int col);


	int ProcessCircle(CoImage* pImgSrc, CoImage* pImgDst, int nCircleRadius, int nSigma,  int nSmoothRaduis, int row, int col);

	int ProcessXCircle(CoImage* pImgSrc, CoImage* pImgDst, int nCircleRadius, int nSigma,  int nSmoothRaduis, int row, int col);
	int ProcessYCircle(CoImage* pImgSrc, CoImage* pImgDst, int nCircleRadius, int nSigma,  int nSmoothRaduis, int row, int col);

	/**
	 * @brief ��˧��ʯ���ı�̩ �ĵ������� �����װ� ��˧��ʯ������˾ �˺��ٳ�.
	 *			( �� : exp( - i^2 / (2 * sigma^2)) )
	 *
	 * @param rSigma [in] : ����
	 * @param rRadius [in] : �ɰ�
	 * @return 
	 */
	void SetParam (double rSigma, double rRadius);
	static void GetKernel(double rSigma, double rRadius, Vecd& vKernel);
	static void GetKernel2D(double rSigma1, double rSigma2, double rRadius, Matd& mKernel);
protected:
	Mat		m_mTemp;
private:
	//! ��˧��ʯ����
	double m_rSigma;
	//! ��˧��ʯ�ɰ�
	double m_rRadius;
	//! ��˧��ʯ�����ɵ�
	double* m_prKernel;

private:
	/**
	 * @brief X���� ���ͽ嶮�� �����ٳ�.
	 *
	 * @param pmIn [in] : ���ͽ嶮�� �Ե�
	 * @param pmOut [out] : ���ͽ嶮�� �Ե�
	 * @param rSigma [in] : ��˧��ʯ����
	 * @return 
	 */
	void GetXBlur(Mat* pmIn, Mat* pmOut, double rSigma);
	void GetXBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col);
	/**
	 * @brief Y���� ���ͽ嶮�� �����ٳ�.
	 *
	 * @param pmIn [in] : ���ͽ嶮�� �Ե�
	 * @param pmOut [out] : ���ͽ嶮�� �Ե�
	 * @param rSigma [in] : ��˧��ʯ����
	 * @return 
	 */
	void GetYBlur(Mat* pmIn, Mat* pmOut, double rSigma);
	void GetYBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col);
};

/**
 * @brief ��˧��ʯ�繤�е���
 */
class CVLIB_DECLSPEC GaussianDiffFilter : public GaussianFilter
{
public:
	GaussianDiffFilter();
	virtual ~GaussianDiffFilter();

	void SetParam (double rSigma, double rRadius);
	static void GetDiffKernel(double rSigma, double rRadius, Vecd& vKernel);
	static void GetKernel(double rSigma, double rRadius, Vecd& vKernel);

	int ProcessX(Mat* pmIn, Mat* pmOut);
	int ProcessY(Mat* pmIn, Mat* pmOut);
private:
	double m_rSigma;
	double m_rRadius;
	double* m_prKernel;
	double* m_prDiffKernel;

	void GetXProcess(Mat* pmIn, Mat* pmOut, double* prKernel);
	void GetYProcess(Mat* pmIn, Mat* pmOut, double* prKernel);
};


}

#endif // !defined(_GaussianFilter_H_)
