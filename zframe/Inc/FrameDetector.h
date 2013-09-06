#pragma once

#include "cvlibbase/inc/_cvlibbase.h"
#include "imagelist.h"
#include "RectangleDetectFromPatternPos.h"
#include "ipTransformation.h"
#include "Colorspace.h"
#include "PackageInfo.h"

namespace CVLib
{

//#define ENGINE_DEBUG
//#define TIMELOG

class CVLIB_DECLSPEC FrameDetector
{
public:
	FrameDetector();
	~FrameDetector();

	bool		init (const Mat& image, const PackageInfo& packageInfo);
	bool		detect (const Mat& image);
	void		getCardRegion(Vector<Point2i>& region);
    bool        crop (const Mat& frame, Mat& croped, Vector<Point2i>& region);
    
    bool        flagFocus() const;
	bool		flagInit() {return m_fInit;}
protected:
    bool        flagProperSize(int percent) const;
	void		getLog (StringArray& log);
    bool        crop (Mat& croped);
    
	bool	m_fInit;
	Rect	m_searchRegion;
	PackageInfo	m_packageInfo;

	Mat		m_src;
	Mat		m_image;
	Mat		m_gray;
	Mat		m_bin;
	Vector<Point2i> m_cardcorners;
	Vector<Point2i> m_cardcorners_internal;
	Vector<Point2i> m_corners;
	int		m_nframeIdx;

	void scaleImage(const Mat& src, Mat& scaled, float rscale);
	void edgeImage(const Mat& src, Mat& edge);
	void calcAffineTransform (Vector<Point2i>& points, Matf& mAffine);
	void binaryImage(const Mat& src, Mat& bin);
	void extractBound(Mat& src);
	Point2i extractCorner (const Mat& src, COLORREF backColor, COLORREF dotColor);
private:
	bool SExtract(const Mat& image, Vector<Point2i>& border);
	void filteringVertical (const Mat& gray, const Rect& region, Vector<Point2i>& points, Mat& work);
	void filteringHorizental (const Mat& gray, const Rect& region, Vector<Point2i>& points, Mat& work);

	RectangleDetectFromPatternPos	m_rectDetector;
	Vector<Point2i> points_edge1;
	Vector<Point2i> points_edge2;
	Vector<Point2i> points_edge3;
	Vector<Point2i> points_edge4;

    Vector<int>     m_detectArray;
	float m_rAspect;

private:
	StringArray m_log;

#ifdef ENGINE_DEBUG
	MatDescPtrList	m_debugArray;
public:
	void		getDebugInfo(Mat& image);
#endif
	Timer m_timer;

	// tracking
	bool m_fExistTrack;
};

}