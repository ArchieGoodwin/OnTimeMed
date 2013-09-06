/*!
 * \file
 * \brief Ãùº¬Ì© ¾¢ºÏ, Ãú´Ý¶¦ ¼êÃÔÂ×²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */
#ifndef SCALEXY
#define SCALEXY

#include "cvlibbase/inc/Mat.h"
#include "cvlibbase/inc/CoImage.h"
#include "ipCoreABC.h"

namespace CVLib {

/**
 @brief Ãùº¬Ì© ¾¢ºÏÃú´Ý¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC ScaleXY : public Object
{
public:
	//! Construction and Destruction
	ScaleXY ();
    virtual ~ScaleXY();

	//! Operations
	void Process (Mat* pDst, int nXOrgDst, int nYOrgDst, int nWidthDst, int nHeightDst,
			  const Mat* pSrc, int nXOrgSrc, int nYOrgSrc, int nWidthSrc, int nHeightSrc);
	void Process (CoImage* pDst, int nXOrgDst, int nYOrgDst, int nWidthDst, int nHeightDst,
			  const CoImage* pSrc, int nXOrgSrc, int nYOrgSrc, int nWidthSrc, int nHeightSrc);
	void Process (CoImage* pDst, const CoImage* pSrc);
	void Process (Mat* pDst, const Mat* pSrc);

	void FastProcess (const Mat* pmatSrc, Mat* pmatDest);
	void FastProcess (const CoImage* pSrc, CoImage* pDest);
	double BilinearInterpolation ( BYTE** ppbImage, int nX, int nY, double rX, double rY );
	double BilinearInterpolation ( float** pprImage, int nX, int nY, double rX, double rY );

	CoImage* Process (const CoImage* pSrc, float rScale);
	Mat* Process (const Mat* pSrc, float rScale);
};

class CVLIB_DECLSPEC ipScaleXY : public ipCorePump
{
	DECLARE_PUMP(ipScaleXY);
public:
	ipScaleXY(){}
	ipScaleXY(int nWidth, int nHeight){m_newWidth=nWidth;m_newHeight=nHeight;}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	int m_newWidth;
	int m_newHeight;
};

namespace ip
{
CVLIB_DECLSPEC void resize(const Mat& src, Mat& dst, long newx, long newy);
}

}

#endif
