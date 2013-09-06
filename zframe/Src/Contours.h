
/*! 
 * \file    Contours.h
 * \ingroup ip
 * \brief   cvlib
 * \author  
 */
#ifndef _Contours_H_
#define _Contours_H_

#include "cvlibbase/inc/cvlibstructs.h"
#include "cvlibbase/inc/template.h"

namespace CVLib { namespace ip {

CVLIB_DECLSPEC void ConvexHull (Vector<Point2i>& src, Vector<Point2i>& dst, bool fClockwise);
CVLIB_DECLSPEC void Bresenham (int nX0, int nY0, int nX1, int nY1, int* &pnX, int* &pnY, int &nNum);
CVLIB_DECLSPEC void Bresenham (Point2i& first, Point2i& last, Vector<Point2i>& points);
CVLIB_DECLSPEC bool IntersectPointTwoLines (const Point2f& p1, const Point2f& p2, const Point2f& p3, const Point2f& p4, 
											Point2f& intersectPt);
CVLIB_DECLSPEC Point2f CrossPointTwoLines (const Point2f& p1, const Point2f& p2, const Point2f& p3, const Point2f& p4);
CVLIB_DECLSPEC Point2f CrossPointTwoLines (const Point2i& p1, const Point2i& p2, const Point2i& p3, const Point2i& p4);


}}

#endif //_Contours_H_