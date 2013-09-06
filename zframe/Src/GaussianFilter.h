
/*!
 * \file
 * \brief	°¡Ë§»¤Ê¯¿Ğµ¹º÷¶¦ ººÊÎÂÜ.
 * \author	¹ÊËÎ²§
 */

#if !defined(_GaussianFilter_H_)
#define _GaussianFilter_H_

#include "cvlibbase/inc/CoImage.h"

namespace CVLib
{

/**
 * @brief °¡Ë§»¤Ê¯¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC GaussianFilter : public Object
{
public:
	//! °é¾¢»ô
	GaussianFilter();
	//! ÁÄ±Ğ»ô
	virtual ~GaussianFilter();

	/**
	 * @brief ËÓµÙÃÔµÛËæ ´İÂ×ÊŞ X, Y¸ÒÂá¹¹µÍ½å¶®¶¦ ¼êÃÔÂ×ÊŞ ¾¥µÙÃÔµÛËæ ²ÄÊÌ¼Ó³Ş.
	 *
	 * @param pmIn [in] : ËÓµÙÃÔµÛ
	 * @param pmOut [out] : ¾¥µÙÃÔµÛ
	 * @return ºÂ°á : 0 , »¨Â² : ÊïË±¿¸´Å
	 */
	int Process(Mat* pmIn, Mat* pmOut);
	/**
	 * @brief ËÓµÙÃùº¬Ëæ ´İÂ×ÊŞ X, Y¸ÒÂá¹¹µÍ½å¶®¶¦ ¼êÃÔÂ×ÊŞ ¾¥µÙÃùº¬Ëæ ²ÄÊÌ¼Ó³Ş.
	 *
	 * @param pImgSrc [in] : ËÓµÙÃùº¬
	 * @param pImgDst [out] : ¹¹µÍ½å¶®´ô ¾¥µÙÃùº¬
	 * @return ºÂ°á : 0 , »¨Â² : ÊïË±¿¸´Å
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
	 * @brief °¡Ë§»¤Ê¯µØÁÄ±¨Ì© ÁÄµ¹¸¡À¾¶¦ º¼¼³Â×°Ö °¡Ë§»¤Ê¯¿¤³º´ÉË¾ ±Ëº¤ÂÙ³Ş.
	 *			( »¥ : exp( - i^2 / (2 * sigma^2)) )
	 *
	 * @param rSigma [in] : ¹¤º¤
	 * @param rRadius [in] : ¸É°Ò
	 * @return 
	 */
	void SetParam (double rSigma, double rRadius);
	static void GetKernel(double rSigma, double rRadius, Vecd& vKernel);
	static void GetKernel2D(double rSigma1, double rSigma2, double rRadius, Matd& mKernel);
protected:
	Mat		m_mTemp;
private:
	//! °¡Ë§»¤Ê¯¹¤º¤
	double m_rSigma;
	//! °¡Ë§»¤Ê¯¸É°Ò
	double m_rRadius;
	//! °¡Ë§»¤Ê¯¿¤³º¹ÉµÛ
	double* m_prKernel;

private:
	/**
	 * @brief X¸ÒÂá ¹¹µÍ½å¶®¶¦ ¼êÃÔÂÙ³Ş.
	 *
	 * @param pmIn [in] : ¹¹µÍ½å¶®ÂÚ ÃÔµÛ
	 * @param pmOut [out] : ¹¹µÍ½å¶®´ô ÃÔµÛ
	 * @param rSigma [in] : °¡Ë§»¤Ê¯¹¤º¤
	 * @return 
	 */
	void GetXBlur(Mat* pmIn, Mat* pmOut, double rSigma);
	void GetXBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col);
	/**
	 * @brief Y¸ÒÂá ¹¹µÍ½å¶®¶¦ ¼êÃÔÂÙ³Ş.
	 *
	 * @param pmIn [in] : ¹¹µÍ½å¶®ÂÚ ÃÔµÛ
	 * @param pmOut [out] : ¹¹µÍ½å¶®´ô ÃÔµÛ
	 * @param rSigma [in] : °¡Ë§»¤Ê¯¹¤º¤
	 * @return 
	 */
	void GetYBlur(Mat* pmIn, Mat* pmOut, double rSigma);
	void GetYBlurPixel(Mat* pmIn, Mat* pmOut, double rSigma, int row, int col);
};

/**
 * @brief °¡Ë§»¤Ê¯·ç¹¤¿Ğµ¹º÷
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
