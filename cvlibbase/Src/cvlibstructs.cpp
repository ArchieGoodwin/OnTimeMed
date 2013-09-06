
/*! 
 * \file cvlibstructs.cpp
 * \ingroup 
 * \brief PointÌ® Rect ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */

#include "cvlibstructs.h"

namespace CVLib
{

	Rectf::Rectf()	{}
	Rectf::Rectf(float const x1_, float const y1_, float const x2_, float const y2_)
	{
		topLeft.x=x1_,topLeft.y=y1_,bottomRight.x=x2_,bottomRight.y=y2_;
	}
	Rectf::Rectf(Rectf const &p):topLeft(p.topLeft),bottomRight(p.bottomRight){}
	float Rectf::Surface() const
	{
		return (bottomRight.x-topLeft.x)*(bottomRight.y-topLeft.y);
	}
	
	Rectf Rectf::CrossSection(Rectf const &r2) const
	{
		Rectf cs;
		cs.topLeft.x=MAX(topLeft.x, r2.topLeft.x);
		cs.topLeft.y=MAX(topLeft.y, r2.topLeft.y);
		cs.bottomRight.x=MIN(bottomRight.x, r2.bottomRight.x);
		cs.bottomRight.y=MIN(bottomRight.y, r2.bottomRight.y);
		if (cs.topLeft.x<=cs.bottomRight.x && cs.topLeft.y<=cs.bottomRight.y) {
			return cs;
		} else {
			return Rectf(0,0,0,0);
		}//if
	}
	
	Point2f Rectf::Center() const
	/*
	* Returns the center point of rectangle.
	*/
	{
		return Point2f((bottomRight.x+topLeft.x)/2.0f, (bottomRight.y+topLeft.y)/2.0f);
	}
	
	float Rectf::Width() const
		//returns rectangle width
	{
		return bottomRight.x-topLeft.x;
	}
	
	float Rectf::Height() const
		//returns rectangle height
	{
		return bottomRight.y-topLeft.y;
	}
	Rectf Rectf::operator-()
	{
		Rectf xBoundary;
		xBoundary.topLeft = -topLeft;
		xBoundary.bottomRight = -bottomRight;
		return xBoundary;
	}
	void Rectf::Padding(float const rPadding)
	{
		topLeft -= Point2f(rPadding, rPadding);
		bottomRight += Point2f(rPadding, rPadding);
	}

	TermCriteria TermCriteria::Check (double default_eps, int default_max_iters) const
	{
		TermCriteria crit;

		crit.nType = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
		crit.nMaxIter = default_max_iters;
		crit.rEpsilon = (float)default_eps;

		if( (nType & ~(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER)) != 0 )
			assert(0);

		if( (nType & CV_TERMCRIT_ITER) != 0 )
		{
			if( nMaxIter <= 0 )
				assert(0);
			crit.nMaxIter = nMaxIter;
		}

		if( (nType & CV_TERMCRIT_EPS) != 0 )
		{
			if( rEpsilon < 0 )
				assert(0);

			crit.rEpsilon = rEpsilon;
		}

		if( (nType & (CV_TERMCRIT_EPS | CV_TERMCRIT_ITER)) == 0 )
			assert (0);

		crit.rEpsilon = (float)MAX( 0, crit.rEpsilon );
		crit.nMaxIter = MAX( 1, crit.nMaxIter );

		return crit;
	}


	RotatedRect::RotatedRect():angle(0.0f) {}
	RotatedRect::RotatedRect(const Point2f& _center, const Sizef& _size, float _angle) :center(_center),size(_size),angle(_angle){}
	RotatedRect::RotatedRect(const RotatedRect& box):center(box.center),size(box.size),angle(box.angle){}
	void RotatedRect::points(Point2f pts[]) const
	{
		double _angle = angle*CVLIB_PI/180.;
		float b = (float)cos(_angle)*0.5f;
		float a = (float)sin(_angle)*0.5f;

		pts[0].x = center.x - a*size.height - b*size.width;
		pts[0].y = center.y + b*size.height - a*size.width;
		pts[1].x = center.x + a*size.height - b*size.width;
		pts[1].y = center.y - b*size.height - a*size.width;
		pts[2].x = 2*center.x - pts[0].x;
		pts[2].y = 2*center.y - pts[0].y;
		pts[3].x = 2*center.x - pts[1].x;
		pts[3].y = 2*center.y - pts[1].y;
	}
	Rect RotatedRect::boundingRect() const
	{
		Point2f pt[4];
		points(pt);
		Rect r((int)floor(MIN(MIN(MIN(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
			(int)floor(MIN(MIN(MIN(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
			(int)ceil(MAX(MAX(MAX(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
			(int)ceil(MAX(MAX(MAX(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
		r.width -= r.x - 1;
		r.height -= r.y - 1;
		return r;
	}
}