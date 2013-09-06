#ifndef _WARPING_H_
#define _WARPING_H_

#include "cvlibbase/inc/CoImage.h"
#include "cvlibbase/inc/template.h"
#include "TriangleModel.h"

namespace CVLib { namespace ip{

CVLIB_DECLSPEC bool Warping(CoImage* pInImage, CoImage* pOutImage, Rect* pWarpingRect, 
		Point2f* pSrcPoints, Point2f* pDstPoints, TriangleModel* pTriangleModel);
CVLIB_DECLSPEC void ComputeCoefficients (Point2f* pPoints, Triangle* pTriangle, float rX, float rY, float* prA, float* prB, float* prC);
CVLIB_DECLSPEC float TriangleArea (float rX1, float rY1, float rX2, float rY2, float rX3, float rY3);

 /*
 *        | 1+a  b   tx |
 * W(p) = | c   1+d   ty |
 *        | 0     0     1 |
 */
CVLIB_DECLSPEC void InitWarp (Mat& W, float a, float b, float c, float d, float tx, float ty);
CVLIB_DECLSPEC void WarpImage (const Mat& src, Mat& dst, const Mat& W);
CVLIB_DECLSPEC void WarpImage2 (const Mat& src, Mat& dst, const Mat& W);
CVLIB_DECLSPEC void WarpPoint (const Vector<Point2i>& src, Vector<Point2i>& dst, const Mat& W);
CVLIB_DECLSPEC void DrawWarpRect (Mat& image, const Rect& rect, const Mat& W);

}}

#endif // _WARPING_H_
