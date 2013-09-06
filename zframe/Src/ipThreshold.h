
/*!
 * \file	ipThreshold.h
 * \ingroup ipCore
 * \brief   JitterÃùº¬Ë¾ »¶ºÂÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#ifndef _ipThreshold_H_
#define _ipThreshold_H_

#include "ipCoreABC.h"

namespace CVLib
{

class CVLIB_DECLSPEC ipThreshold : public ipCorePump
{
	DECLARE_PUMP(ipThreshold)
public:
	enum ThreshType{General, Global};
	ipThreshold(const ipThreshold& from){m_bLevel=from.m_bLevel; m_nMode=from.m_nMode;}
	ipThreshold(ThreshType nMode=General, BYTE bLevel=128): m_bLevel(bLevel),m_nMode(nMode){}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	ThreshType m_nMode;
	BYTE m_bLevel;
protected:
	bool iProcess_General (Mat* pmSrc, Mat* pmDst = NULL);
	bool iProcess_General (CoImage* pSrc, CoImage* pDst = NULL);
	bool iProcess_Global (Mat* pIn, Mat* pOut = NULL);
	bool iProcess_Global (CoImage* pSrc, CoImage* pDst = NULL);
};

namespace ip
{
	/* Types of thresholding */
#define THRESH_BINARY      0  /* value = value > threshold ? max_value : 0       */
#define THRESH_BINARY_INV  1  /* value = value > threshold ? 0 : max_value       */
#define THRESH_TRUNC       2  /* value = value > threshold ? threshold : value   */
#define THRESH_TOZERO      3  /* value = value > threshold ? value : 0           */
#define THRESH_TOZERO_INV  4  /* value = value > threshold ? 0 : value           */
#define THRESH_MASK        7

#define THRESH_OTSU        8  /* use Otsu algorithm to choose the optimal threshold value;
	combine the flag with one of the above THRESH_* values */

/* Applies fixed-level threshold to grayscale image. This is a basic operation applied before retrieving contours */
CVLIB_DECLSPEC double Threshold( const Mat& src, Mat& dst, double thresh, double maxval, int type );
CVLIB_DECLSPEC BYTE OtsuThreshold(const Mat& img);
// void AdaptiveThreshold( const Mat& src, Mat& dst, double maxValue, int method, int type, int blockSize, double delta );

}

}

#endif //_ipThreshold_H_