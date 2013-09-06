#pragma once

#include "cvlibbase/inc/_cvlibbase.h"
#include "Regressor.h"
#include "imagelist.h"

namespace CVLib
{
class CVLIB_DECLSPEC LineSegment
{
public:
	Point2i end1;
	Point2i end2;
	LineSegment(){}
	~LineSegment(){}
};
class CVLIB_DECLSPEC RectangleDetectFromPatternPos
{
public:
	RectangleDetectFromPatternPos(float aspect, MatDescPtrList* pMatList);
	~RectangleDetectFromPatternPos();
	bool Extract(const Mat& img, Vector<Point2i>** edges, Vector<Point2i>& border);
	bool Extract2(const Mat& img, Vector<Point2i>** edges, Vector<Point2i>& border);
	Mat* GetDebug();
	void Getlog (StringArray& log);
protected:
	class LineElem : public Point2i
	{
	public:
		int Orientation(const LineElem& other) {
			if (x==other.x)
				return 0;
			return (int)((atanf((float)(y-other.y) / (float)(x-other.x)) + CVLIB_PI/2.0f) / CVLIB_PI * 180) % 180;
		}
	};

	class RectangleEdge
	{
	public:
		RectangleEdge(int n=0){nAxis=n;nRealX=0;nRealY=0;pRegressor=NULL;}
		~RectangleEdge() {if (pRegressor) delete pRegressor;}
		RectangleEdge& operator=(const RectangleEdge& t){nAxis=t.nAxis; nRealX=t.nRealX; nRealY=t.nRealY; fX=t.fX;
				vIdx.RemoveAll(); vIdx.Append(t.vIdx); pRegressor=t.pRegressor;return *this;}

		int nAxis;
		Vector<int> vIdx;
		int nRealX;
		int nRealY;
		bool fX;
		LinearRegressor* pRegressor;
		Point2i Cross(const RectangleEdge& t)
		{
			if (fX==t.fX)
				return Point2i(-1,-1);
			float k1, k2, b1, b2;
			if (!fX)
			{
				k1=pRegressor->GetCoeffVec().data.fl[0];
				b1=pRegressor->GetCoeffVec().data.fl[1];
				k2=t.pRegressor->GetCoeffVec().data.fl[0];
				b2=t.pRegressor->GetCoeffVec().data.fl[1];
			}
			else
			{
				k2=pRegressor->GetCoeffVec().data.fl[0];
				b2=pRegressor->GetCoeffVec().data.fl[1];
				k1=t.pRegressor->GetCoeffVec().data.fl[0];
				b1=t.pRegressor->GetCoeffVec().data.fl[1];
			}
			int x=CVUtil::Round((k2*b1+b2)/(1.0-k1*k2));
			int y=CVUtil::Round(k1*x+b1);
			return Point2i(x,y);
		}
		int Angle(const RectangleEdge& t) {
			float k1, k2;
			if (!fX)
			{
				k1=pRegressor->GetCoeffVec().data.fl[0];
				k2=t.pRegressor->GetCoeffVec().data.fl[0];
			}
			else
			{
				k2=pRegressor->GetCoeffVec().data.fl[0];
				k1=t.pRegressor->GetCoeffVec().data.fl[0];
			}
			int n=(int)(((atan(k1)-atan(1.0/k2)))/CVLIB_PI*180);
			return MIN(abs(n), abs(180-n));
		}
	};
	class Rectangle
	{
	public:
		Rectangle(){edge[0]=edge[1]=edge[2]=edge[3]=0;angEval=0;raspect=0;}
		Rectangle(RectangleEdge*p1,RectangleEdge*p2,RectangleEdge*p3,RectangleEdge*p4){
			edge[0]=p1;edge[1]=p2;edge[2]=p3;edge[3]=p4;
			angEval=0; raspect=0;
		}
		~Rectangle(){}
		RectangleEdge* edge[4];
		Rectangle& operator=(const Rectangle& t)
		{
			for (int i=0; i<4; i++)
				edge[i]=t.edge[i];
			angEval=t.angEval;
			raspect=t.raspect;
			return *this;
		}
		float AspectRate()
		{
			Point2i pt1=edge[0]->Cross(*edge[1]);
			Point2i pt2=edge[1]->Cross(*edge[2]);
			Point2i pt3=edge[2]->Cross(*edge[3]);
			Point2i pt4=edge[3]->Cross(*edge[0]);
			double w=(pt1.DistTo(pt2)+pt3.DistTo(pt4))/2.0;
			double h=(pt1.DistTo(pt4)+pt2.DistTo(pt3))/2.0;
			return (float)h/(float)w;
		}
		void DrawTo(Mat& image, COLORREF color)
		{
			int nr=5;
			Point2i pt1=edge[0]->Cross(*edge[1]);
			Point2i pt2=edge[1]->Cross(*edge[2]);
			Point2i pt3=edge[2]->Cross(*edge[3]);
			Point2i pt4=edge[3]->Cross(*edge[0]);
			image.DrawEllipse(Rect(pt1.x-nr,pt1.y-nr,nr*2,nr*2),color);
			image.DrawEllipse(Rect(pt2.x-nr,pt2.y-nr,nr*2,nr*2),color);
			image.DrawEllipse(Rect(pt3.x-nr,pt3.y-nr,nr*2,nr*2),color);
			image.DrawEllipse(Rect(pt4.x-nr,pt4.y-nr,nr*2,nr*2),color);
		}
		bool IsValid(const Mat& image)
		{
			Point2i pt1=edge[0]->Cross(*edge[1]);
			Point2i pt2=edge[1]->Cross(*edge[2]);
			Point2i pt3=edge[2]->Cross(*edge[3]);
			Point2i pt4=edge[3]->Cross(*edge[0]);
			return image.IsInterior(pt1.x,pt1.y) && image.IsInterior(pt2.x,pt2.y) &&
				image.IsInterior(pt3.x,pt3.y) && image.IsInterior(pt4.x,pt4.y);
		}

		int angEval;
		float raspect;
	};

	bool SearchLine(Vector<Point2i>& points, Vector<RectangleEdge>& lines, bool fVertical);
	void FilteringCycle (const Veci& vIn, Veci& vOut, int* pnFilter, int nFilterlen);
	void Project (Vector<LineElem>& elems, RectangleEdge& projectInfo, Vector<RectangleEdge>& lines);
	void Filtering (const Veci& vIn, Veci& vOut, int* pnFilter, int nFilterlen);
	bool FittingLine (Vector<Point2i>& points, RectangleEdge& line);
	void DrawLine (RectangleEdge& line, Mat& image, COLORREF color);
protected:
	StringArray	m_log;

	Vector<RectangleEdge> lines1;
	Vector<RectangleEdge> lines2;
	Vector<RectangleEdge> lines3;
	Vector<RectangleEdge> lines4;

	static int CompareRectangle(const void* a, const void* b);
	static int CompareRectangleAspect(const void* a, const void* b);

	float m_rAspect;
	Mat* m_pDebug;
	MatDescPtrList* m_pMatList;
};

}