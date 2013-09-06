
/*!
 * \file	ipTransformation.h
 * \ingroup ipCore
 * \brief   Ãùº¬¸êÃûË¾ ¼êÃÔÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§ 
 */
#ifndef _ipTransformation_H_
#define _ipTransformation_H_

#include "cvlibbase/inc/cvlibbaseDef.h"
#include "ipCoreABC.h"

namespace CVLib
{

	//overflow methods:
enum OverflowMethod {
	OM_COLOR=1,
 	OM_BACKGROUND=2,
 	OM_TRANSPARENT=3,
	OM_WRAP=4,
	OM_REPEAT=5,
	OM_MIRROR=6
};

	//interpolation methods:
enum InterpolationMethod {
	IM_NEAREST_NEIGHBOUR=1,
	IM_BILINEAR		=2,
	IM_BSPLINE		=3,
	IM_BICUBIC		=4,
	IM_BICUBIC2		=5,
	IM_LANCZOS		=6,
	IM_BOX			=7,
	IM_HERMITE		=8,
	IM_HAMMING		=9,
	IM_SINC			=10,
	IM_BLACKMAN		=11,
	IM_BESSEL		=12,
	IM_GAUSSIAN		=13,
	IM_QUADRATIC	=14,
	IM_MITCHELL		=15,
	IM_CATROM		=16
};

class CVLIB_DECLSPEC ipTransformation : public ipCorePump
{
	DECLARE_PUMP(ipTransformation)
public:
	ipTransformation() {m_bkColor = RGB(0,0,0);m_transColor=RGB(0,0,0);}

	static float KernelBSpline(const float x);
	static float KernelLinear(const float t);
	static float KernelCubic(const float t);
	static float KernelGeneralizedCubic(const float t, const float a=-1);
	static float KernelLanczosSinc(const float t, const float r = 3);
	static float KernelBox(const float x);
	static float KernelHermite(const float x);
	static float KernelHamming(const float x);
	static float KernelSinc(const float x);
	static float KernelBlackman(const float x);
	static float KernelBessel_J1(const float x);
	static float KernelBessel_P1(const float x);
	static float KernelBessel_Q1(const float x);
	static float KernelBessel_Order1(float x);
	static float KernelBessel(const float x);
	static float KernelGaussian(const float x);
	static float KernelQuadratic(const float x);
	static float KernelMitchell(const float x);
	static float KernelCatrom(const float x);

	void SetBkcolor(COLORREF color) {m_bkColor = color;}
	void SetTranscolor(COLORREF color) {m_transColor = color;}
	void SetBkcolor(BYTE gray) {m_bkGray= gray;}
	void SetTranscolor(BYTE gray) {m_btransColor = gray;}
protected:
	void OverflowCoordinates(CoImage* pSrc, long &x, long &y, OverflowMethod const ofMethod);
	void OverflowCoordinates(Mat* pSrc, long &x, long &y, OverflowMethod const ofMethod);
	void OverflowCoordinates(CoImage* pSrc, float &x, float &y, OverflowMethod const ofMethod);
	void OverflowCoordinates(Mat* pSrc, float &x, float &y, OverflowMethod const ofMethod);
	COLORREF GetPixelColorWithOverflow(CoImage* pSrc, long x, long y, OverflowMethod const ofMethod, COLORREF* const rplColor);
	BYTE GetPixelColorWithOverflow(Mat* pSrc, long x, long y, OverflowMethod const ofMethod, BYTE* const rplColor);
	void AddAveragingCont(COLORREF const &color, float const surf, float &rr, float &gg, float &bb);
	void AddAveragingCont(BYTE const &color, float const surf, float &rr);
	COLORREF GetAreaColorInterpolated(CoImage* pSrc, float const xc, float const yc, float const w, float const h, 
		InterpolationMethod const inMethod, OverflowMethod const ofMethod=OM_BACKGROUND, COLORREF* const rplColor = 0 );
	BYTE GetAreaColorInterpolated(Mat* pSrc, float const xc, float const yc, float const w, float const h, 
		InterpolationMethod const inMethod, OverflowMethod const ofMethod=OM_BACKGROUND, BYTE* const rplColor = 0 );
	COLORREF GetPixelColorInterpolated(CoImage* pSrc, float x,float y, InterpolationMethod const inMethod=IM_BILINEAR, 
		OverflowMethod const ofMethod=OM_BACKGROUND, COLORREF* const rplColor = 0);
	BYTE GetPixelColorInterpolated(Mat* pSrc, float x,float y, InterpolationMethod const inMethod=IM_BILINEAR, 
		OverflowMethod const ofMethod=OM_BACKGROUND, BYTE* const rplColor = 0);
	COLORREF BlindGetPixelColor(CoImage* pSrc, const long x,const long y);
	BYTE BlindGetPixelColor(Mat* pSrc, const long x,const long y);
protected:
	COLORREF m_bkColor;
	COLORREF m_transColor;
	BYTE	m_bkGray;
	BYTE	m_btransColor;
};

enum RotateType{RLeft, RRight, R180};
class CVLIB_DECLSPEC ipRotate : public ipTransformation
{
	DECLARE_PUMP(ipRotate);
public:
	ipRotate(){}
	ipRotate(const ipRotate& from);
	ipRotate(RotateType nType);
	ipRotate(float rAngle);
	ipRotate(float rAngle, InterpolationMethod inMethod/*=IM_BILINEAR*/,
		OverflowMethod ofMethod=OM_BACKGROUND, COLORREF *replColor=0,
        bool const optimizeRightAngles=true, bool const bKeepOriginalSize=false);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	RotateType m_type;
	float m_rAngle;
	InterpolationMethod	m_inMethod;
	OverflowMethod m_ofMethod;
	COLORREF* m_pRepColor;
	bool m_fOptimizeRightAngles;
	bool m_fKeepOriginalSize;
	int m_nAngle;
private:
	COLORREF m_repColor;
};

class CVLIB_DECLSPEC ipResample : public ipTransformation
{
	DECLARE_PUMP(ipResample);
public:
	ipResample(){m_newx=0;m_newy=0;}
	ipResample(const ipResample& from);
	ipResample(long newx, long newy, int mode = 1);
	ipResample(long newx, long newy, InterpolationMethod const inMethod=IM_BICUBIC2,
		OverflowMethod const ofMethod=OM_REPEAT, bool const disableAveraging=false);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	int m_newx;
	int m_newy;
	int m_nMode;
	InterpolationMethod	m_inMethod;
	OverflowMethod m_ofMethod;
	bool m_fDisableAveraging;

	int m_nMethod;
};

class CVLIB_DECLSPEC ipCrop : public ipTransformation
{
	DECLARE_PUMP(ipCrop);
public:
	ipCrop(){}
	ipCrop(const ipCrop& from);
	ipCrop(long left, long top, long right, long bottom);
	ipCrop(const Rect& rect);
	ipCrop(long topx, long topy, long width, long height, float angle);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	Rect m_rect;
	float m_rAngle;
	bool m_fAngle;

	int		m_nMethod;
};

class CVLIB_DECLSPEC ipSkew : public ipTransformation
{
	DECLARE_PUMP(ipSkew);
public:
	ipSkew(){}
	ipSkew(const ipSkew& from);
	ipSkew(float xgain, float ygain, long xpivot=0, long ypivot=0, bool bEnableInterpolation = false);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	float m_rxgain;
	float m_rygain;
	int m_nxpivot;
	int m_nypivot;
	bool m_fEnableInterpolation;
};

class CVLIB_DECLSPEC ipExpand : public ipTransformation
{
	DECLARE_PUMP(ipExpand);
public:
	ipExpand(){}
	ipExpand(const ipExpand& from);
	ipExpand(long left, long top, long right, long bottom, COLORREF canvascolor);
	ipExpand(long newx, long newy, COLORREF canvascolor);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
	int m_newx;
	int m_newy;
	COLORREF m_color;
};

class CVLIB_DECLSPEC ipThumbnail : public ipTransformation
{
	DECLARE_PUMP(ipThumbnail);
public:
	ipThumbnail(){}
	ipThumbnail(const ipThumbnail& from);
	ipThumbnail(long newx, long newy, COLORREF canvascolor);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
public:
	int m_newx;
	int m_newy;
	COLORREF m_canvasColor;
};

class CVLIB_DECLSPEC ipTransGeo : public ipTransformation
{
	DECLARE_PUMP(ipTransGeo);
public:
	ipTransGeo();
	ipTransGeo(const ipTransGeo& from);
	virtual ~ipTransGeo();
	ipTransGeo&	Rotate(double theta);
	ipTransGeo&	Scale(double sx, double sy);
	ipTransGeo&	Translate(double dx, double dy);
	const Mat*	GetTransformation() const;
	const Mat*	GetInverseTransformation() const;

	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL, Rect* pRect=NULL);
	virtual bool Process (Point2f* pSrc, Point2f* pDst = NULL);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);

	virtual bool InverseProcess (CoImage* pSrc, CoImage* pDst = NULL);
	virtual bool InverseProcess (Point2f* pSrc, Point2f* pDst = NULL);
	virtual bool InverseProcess (Mat* pmSrc, Mat* pmDst = NULL);
private:
	void	ApplyTrans(Mat* A);
	double InterpLinear(Mat* pmImage, double x, double y);
	void InterpLinearImage(CoImage* pmImage, double x, double y, CoImage* pDst, int dstx, int dsty);

	Mat		m_mTransform;
	Mat		m_mInverseTransform;
};

namespace ip
{
CVLIB_DECLSPEC void Rotate(const Mat& src, Mat& dst, RotateType type);
CVLIB_DECLSPEC void Rotate(const Mat& src, Mat& dst, float rAngle);
CVLIB_DECLSPEC void Rotate(const Mat& src, Mat& dst, InterpolationMethod inMethod/*=IM_BILINEAR*/,
						   OverflowMethod ofMethod=OM_BACKGROUND, COLORREF *replColor=0,
						   bool const optimizeRightAngles=true, bool const bKeepOriginalSize=false);
CVLIB_DECLSPEC void Resample(const Mat& src, Mat& dst, long newx, long newy, int mode = 1);
CVLIB_DECLSPEC void Resample(const Mat& src, Mat& dst, long newx, long newy, InterpolationMethod const inMethod=IM_BICUBIC2,
		   OverflowMethod const ofMethod=OM_REPEAT, bool const disableAveraging=false);
CVLIB_DECLSPEC void Crop(const Mat& src, Mat& dst, long left, long top, long right, long bottom);
CVLIB_DECLSPEC void Crop(const Mat& src, Mat& dst, const Rect& rect);
CVLIB_DECLSPEC void Crop(const Mat& src, Mat& dst, long topx, long topy, long width, long height, float angle);
CVLIB_DECLSPEC void Skew(const Mat& src, Mat& dst, float xgain, float ygain, long xpivot=0, long ypivot=0, bool bEnableInterpolation = false);
CVLIB_DECLSPEC void Expand(const Mat& src, Mat& dst, long left, long top, long right, long bottom, COLORREF canvascolor);
CVLIB_DECLSPEC void Expand(const Mat& src, Mat& dst, long newx, long newy, COLORREF canvascolor);
CVLIB_DECLSPEC void Thumbnail(const Mat& src, Mat& dst, long newx, long newy, COLORREF canvascolor);

CVLIB_DECLSPEC void Resample(const CoImage& src, CoImage& dst, long newx, long newy, int mode);
CVLIB_DECLSPEC void Thumbnail(const CoImage& src, CoImage& dst, long newx, long newy, COLORREF canvascolor);
}

}

#endif //_ipTransformation_H_