/*!
 * \file	MatOperation.h
 * \ingroup base
 * \brief	ÃÔµÛÊàº¤Ë¾ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */
#ifndef _MATOPERATION_H_
#define _MATOPERATION_H_

#include "CoImage.h"

namespace CVLib 
{

#define CVLIB_C            1
#define CVLIB_L1           2
#define CVLIB_L2           4
#define CVLIB_NORM_MASK    7
#define CVLIB_RELATIVE     8
#define CVLIB_DIFF         16
#define CVLIB_MINMAX       32

class CVLIB_DECLSPEC MatOp
{
public:
	
	// ÃÔµÛËæº· ¼è¼³´ô 4°¢Âô¹¢¹¤Ë¾ »ùµ¹ Â×±ıÌ© ÃÔµÛµá ÂÙ³Ş.
	static bool CropMat (Mat* pDest, const Mat* pMat, const Rect& rect);
	//ÃÔµÛË¾ ¸ÊÊ­ 0 - 255º¡ËËµá ¼³°öÃùÂÙ³Ş.
	static int Normalize(Mat* pMat, float rMin, float rMax);

	// ÃÔµÛË¾ ¸ôº¡ÂÙ³Ş.
	static void CopyMat (Mat* pDst, const Mat* pSrc, TYPE dsttype);
	static void CopyMat(Mat* pDst, const Mat* pSrc, const Rect& srcRect, const Point2i& dstpt=Point2i());
	static void CopyMat(Mat* pDst, const Mat* pSrc, int nLeft1, int nTop1, int nRight1, int nBottom1, int nLeft2, int nTop2);

	// ¹ÖÀË¶£¶¦ ¸ôº¡ÂÙ³Ş.
	static bool CopyVec (Vec* pDst, const Vec* pSrc, TYPE type);

	//  ÃÔµÛÌ© ³óÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. A = B + C 
	static void Add (Mat* pA, const Mat* pB, const Mat* pC);

	//	ÃÔµÛÌ© ³÷±¨¶¦ ¼êÃÔÂÙ³Ş. A = B - C  
	static void Sub (Mat* pA, const Mat* pB, const Mat* pC);

	// ÃÔµÛ±á ¹ÖÀË¶£Ì© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. A = B * C  
	static void Mul (Vec* pA, const Mat* pB, const Vec* pC);

	// ¹ÖÀË¶£Ì® ÃÔµÛÌ© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. A = B * C 
	static void Mul (Vec* pA, const Vec* pB, const Mat* pC);

	// x' * A * x ¶¦ ±Ëº¤Â×²÷ ÂÜºãËË³Ş.
	static float TrXAX (const Vec* pvX, const Mat* pmA);

	// ¼ÑÊÌ¼ê ÃÔµÛÌ© ¼­Ì¡ÃÔµÛË¾ °éÂÙ³Ş. A = B'  
	static void Transpose(Mat* pA, const Mat* pB);

	// ¼ÑÊÌ¼ê ÃÔµÛÌ© ¼­Ì¡ÃÔµÛË¾ °éÂÙ³Ş. A = A'
	static void Transpose(Mat* pA);

	// A = B  
	static void Equal(Vec* pA, const Vec* pB);

	// ¹ÖÀË¶£ ³óÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. A = B + C  
	static void Add (Vec* pA, const Vec* pB, const Vec* pC);

	// ¹ÖÀË¶£Ì© ³÷±¨¶¦ ¼êÃÔÂÙ³Ş. A = B - C  
	static void Sub (Vec* pA, const Vec* pB, const Vec* pC);

	// ¹ÖÀË¶£Ì© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. C = A * B'  
	static void Mul (Mat* pC, const Vec* pA, const Vec* pB);

	// ¼­Ì¡¹Öµá¶£Ì® ¹ÖÀË¶£Ì®Ì© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. A' * B  
	static double DotProduct (const Vec* pA, const Vec* pB);

	// C = A * B * A'  
	static void ABTrA (Mat* pmC, const Mat* pmA, const Mat* pmB);

	// C = A' * B * A  
	static void TrABA (Mat* pmC, const Mat* pmA, const Mat* pmB);

	// ÃÔµÛÌ© »¨ºã°ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş. B = r * A  
	static void Mul (Mat* pB, double rReal, const Mat* pA);

	// »¨ºãÌ® ¹ÖÀË¶£Ì© °ßÂ×±¨¶¦ ¼êÃÔÂÙ³Ş.
	static void Mul (Vec* pB, double rReal, const Vec* pA);

	// ÃÔµÛÌ© °ßÂ×±¨Êàº¤Ë¾ ¼êÃÔÂÙ³Ş. C = A * B  
	static void Mul (Mat* pC, const Mat* pA, const Mat* pB);

	// ¼ÑÊÌ¼ê ÃÔµÛË¾ ´İ°¢ºº ÃÔµÛµá ¶í´Ç³Ş.
	static void Diag(Mat* pA, double rVal);

	// B = A' * A. 
	static void TrAA(Mat* pmB, const Mat* pmA);

	// B = A * A'. 
	static void ATrA(Mat* pmB, const Mat* pmA);

	// Trace (A) 
	static double Trace(const Mat* pmA);

	static double Sum(const Vec* pvA);
	static double Sum(const Mat* pmA);
	static void SumRows(const Mat* pmA, Vec* pvB);
	static void SumCols(const Mat* pmA, Vec* pvB);
	
	static void inRange(const Mat& src, const Mat& lb, const Mat& rb, Mat& dst);
	static void inRange(const Mat& src, const double lb, const double rb, Mat& dst);
	static void inRange(const CoImage& src, const double lb[3], const double rb[3], Mat& dst, int dims);

	static void Normalize( const Mat& src, Mat& dst, double alpha=1, double beta=0,
					int norm_type=NORM_L2, TYPE dtype=MAT_Tbyte, const Mat& mask=Mat());

	static void AddWeighted(const Mat& src1, double alpha, const Mat& src2, double beta, double gamma, Mat& dst);
	static void GEMM(const Mat* matA, const Mat* matB, double alpha,
		const Mat* matC, double beta, Mat* matD, int flags=0);
	/* Matrix transform: dst = A*B + C, C is optional */
	static void MatMulAdd(const Mat* src1, const Mat* src2, const Mat* src3, Mat* dst );
	static double ScalarProduct(Mat* pmA, Mat *pmB);
};

void CVLIB_DECLSPEC CartToPolar( const Mat& X, const Mat& Y, Mat& Mag, Mat& Angle, bool angleInDegrees );

}

#endif //_MATOPERATION_H_