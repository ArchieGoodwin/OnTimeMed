
#include "RectangleDetectFromPatternPos.h"

#include "integralimage.h"
#include "ipThreshold.h"
#include "colorspace.h"
#include "regressorTrainer.h"
#include "cvlibbase/inc/cvlibutil.h"
#include "cvlibbase/inc/MatOperation.h"
#include "cvlibbase/inc/cvlibutil.h"
#include "cvlibbase/inc/timer.h"
#include "cvlibbase/inc/random.h"
#include "ImageList.h"
#include "FindContour.h"

namespace CVLib
{
//#define DRAWRESULT
//#define TIMELOG
#define ORIENT_DIR 17
#define ORIENT_QUANT 10

RectangleDetectFromPatternPos::RectangleDetectFromPatternPos(float aspect, 
	MatDescPtrList* pMatList)
{
	m_rAspect=aspect;m_pDebug=0;
	m_pMatList = pMatList;
}
RectangleDetectFromPatternPos::~RectangleDetectFromPatternPos(){}

int RectangleDetectFromPatternPos::CompareRectangleAspect(const void* a, const void* b)
{
	const RectangleDetectFromPatternPos::Rectangle* p1=(const RectangleDetectFromPatternPos::Rectangle*)a;
	const RectangleDetectFromPatternPos::Rectangle* p2=(const RectangleDetectFromPatternPos::Rectangle*)b;
	{
		if (p1->raspect > p2->raspect)
			return 1;
		else 
			return -1;
	}
}
int RectangleDetectFromPatternPos::CompareRectangle(const void* a, const void* b)
{
	const RectangleDetectFromPatternPos::Rectangle* p1=(const RectangleDetectFromPatternPos::Rectangle*)a;
	const RectangleDetectFromPatternPos::Rectangle* p2=(const RectangleDetectFromPatternPos::Rectangle*)b;
	{
		if (p1->angEval > p2->angEval)
			return 1;
		else 
			return -1;
	}
}
void RectangleDetectFromPatternPos::Getlog (StringArray& log)
{
	log.Append(m_log);
}

Mat* RectangleDetectFromPatternPos::GetDebug()
{
	return m_pDebug;
}

bool RectangleDetectFromPatternPos::Extract (const Mat& img, Vector<Point2i>** edges, Vector<Point2i>& border)
{
	int i, k;
	if (!SearchLine (*edges[0], lines1, true))
		return false;
	if (!SearchLine (*edges[1], lines2, false))
		return false;
	if (!SearchLine (*edges[2], lines3, true))
		return false;
	if (!SearchLine (*edges[3], lines4, false))
		return false;

	Vector<RectangleEdge>* lines[]={&lines1, &lines2, &lines3, &lines4, 0};
	for (k=0; lines[k]; k++)
	{
		for (i=0; i<lines[k]->GetSize(); i++)
		{
			RectangleEdge& line=lines[k]->GetAt(i);
			if (!FittingLine(*edges[k], line))
				return false;
		}
	}
	// confirm last rectangle
	Vector<Rectangle> rectangles;
	int n1=lines1.GetSize();
	int n2=lines2.GetSize();
	int n3=lines3.GetSize();
	int n4=lines4.GetSize();
	if (n1<1 || n2<1 || n3<1 || n4<1)
		return false;

	for (int i0=0; i0<lines1.GetSize(); i0++)
		for (int i1=0; i1<lines2.GetSize(); i1++)
			for (int i2=0; i2<lines3.GetSize(); i2++)
	for (int i3=0; i3<lines4.GetSize(); i3++)
	{
		Rectangle r(&lines1[i0],&lines2[i1],&lines3[i2],&lines4[i3]);
		if (!r.IsValid(img))
			continue;
		rectangles.Add(r);
	}
	if (rectangles.GetSize() == 0)
		return false;
	int iR;
	Mat * pnew=0;
	if (m_pMatList)
	{
		pnew=new Mat();
		ColorSpace::GraytoRGB (img, *pnew);
	}
	for (iR=0; iR<rectangles.GetSize(); iR++)
	{
		Rectangle& r=rectangles[iR];
		r.angEval=0;
		for (int k=0; k<4; k++)
			r.angEval += abs(r.edge[k]->Angle(*r.edge[(k+1)%4])-90);
		rectangles[iR].raspect=(float)fabs(rectangles[iR].AspectRate()-m_rAspect);
		if (m_pMatList)
		{
			Point2i pt0, pt1, pt2, pt3;
			pt0=r.edge[0]->Cross(*r.edge[1]);
			pt1=r.edge[1]->Cross(*r.edge[2]);
			pt2=r.edge[2]->Cross(*r.edge[3]);
			pt3=r.edge[3]->Cross(*r.edge[0]);
			pnew->DrawLine (pt0, pt1, RGB(0,255,0));
			pnew->DrawLine (pt1, pt2, RGB(0,255,0));
			pnew->DrawLine (pt2, pt3, RGB(0,255,0));
			pnew->DrawLine (pt3, pt0, RGB(0,255,0));
		}
	}
	if (m_pMatList)
		m_pMatList->Add(pnew);
	qsort (rectangles.GetData(), rectangles.GetSize(), sizeof (Rectangle), CompareRectangleAspect);
	int effRects=0;
	for (iR=0; iR<rectangles.GetSize(); iR++)
	{
		if (rectangles[iR].raspect > 0.05f)
			break;
	}
	if (iR==0)
		return false;
	effRects=iR;
	qsort (rectangles.GetData(), effRects, sizeof (Rectangle), CompareRectangle);
	int iniAng=rectangles[0].angEval;
	for (iR=0; iR<effRects; iR++)
	{
		if (rectangles[iR].angEval>iniAng+2)
			break;
	}
	{
		if (rectangles[0].angEval > 10)
			return false;
		int neffR=iR;
		int iAs=0;
		Vecf aspects(neffR);
		for (iR=0; iR<neffR; iR++)
		{
			aspects[iR]=rectangles[iR].AspectRate();
		}
		for (iR=1; iR<neffR; iR++)
		{
			if (fabs(aspects[iR]-m_rAspect) < fabs(aspects[iAs]-m_rAspect))
			{
				iAs=iR;
			}
		}
		if (fabs(aspects[iAs]-m_rAspect) > 0.05f)
			return false;
		border.SetSize(4);
		Rectangle& t=rectangles[iAs];
		border[0]=t.edge[0]->Cross(*t.edge[1]);
		border[1]=t.edge[1]->Cross(*t.edge[2]);
		border[2]=t.edge[2]->Cross(*t.edge[3]);
		border[3]=t.edge[3]->Cross(*t.edge[0]);
	}
	return true;
}

typedef enum PatternType
{
	PT_Corner1,
	PT_Corner2,
	PT_Corner3,
	PT_Corner4,
	PT_Edge1,
	PT_Edge2,
	PT_Edge3,
	PT_Edge4,
} PatternType;

#define EDGE_PIXEL 0
#define NON_EDGE_PIXEL 255

static const int nPatternSize=20;
static const int nPatternSize2=nPatternSize/2;
static const int nPixels=nPatternSize*nPatternSize;
static const int nSubPixels=nPatternSize2*nPatternSize2;
static const float rThresh=45;

bool RectangleDetectFromPatternPos::SearchLine(Vector<Point2i>& points, Vector<RectangleEdge>& lines, bool fVertical)
{
	int nPointNum=points.GetSize();
	if (nPointNum<=2)
		return false;
	Vector<LineElem>	elems;
	int nNeighbors = MIN(2, nPointNum);
	int i, j;
#ifdef TIMELOG
	Timer timer;
	timer.Resume();
	char szLog[1000];
	sprintf (szLog, "\tsearch line: point num %d", nPointNum);
	m_log.Add(szLog);
	double rtime;
#endif
	Matf mDist(nPointNum, nPointNum);
	Mati mOrient(nPointNum, nPointNum);

	elems.SetSize(nPointNum);
	for (i=0; i<nPointNum; i++)
	{
		elems[i].x=points[i].x;
		elems[i].y=points[i].y;
	}

	mDist=0;
	mOrient=0;
	for (i=0; i<nPointNum; i++)
	{
		for (j=i+1; j<nPointNum; j++)
		{
			mDist[i][j] = (float)elems[i].DistTo(elems[j]);
			if (mDist[i][j] < 4)
				mDist[j][i] = mDist[i][j] = 1000;
			else
				mDist[j][i] = mDist[i][j];

			mOrient[i][j] = elems[i].Orientation(elems[j]);
			mOrient[j][i] = mOrient[i][j];
		}
	}
#ifdef TIMELOG
	rtime=timer.GetElapsedTime();
	sprintf (szLog, "\tsearch line: construct dist ang mat %.4fms", rtime*1000);
	m_log.Add(szLog);
	timer.Resume();
#endif
	Veci vIdx(nNeighbors+1);
	Veci vOrientHistogram(180);
	vOrientHistogram=0;
	for (i=0; i<nPointNum; i++)
	{
		CVUtil::SortIdx (mDist.data.fl[i], nPointNum, vIdx, nNeighbors+1, CVUtil::SORT_INC);
		for (int k=1; k<nNeighbors+1; k++)
		{
			vOrientHistogram[mOrient[i][vIdx[k]]] ++;
		}
	}
	vIdx.Release();
#ifdef TIMELOG
	timer.Stop();
	rtime=timer.GetElapsedTime();
	sprintf (szLog, "\tsearch line: ori histogram %.4fms", rtime*1000);
	m_log.Add(szLog);
	timer.Resume();
#endif
	Veci vFiltered(180);
	Veci vTemp(180);
	int anFilter[] = {1, 6, 15, 20, 15, 6, 1};
	int nFilterlen=sizeof(anFilter)/sizeof(anFilter[0]);
	FilteringCycle (vOrientHistogram, vFiltered, anFilter, nFilterlen);

	for (i=0; i<180; i++)
	{
		int nPrev=(i-1+180)%180;
		int nNext=(i+1)%180;
		if (vFiltered[i] <= vFiltered[nPrev] || vFiltered[i] <= vFiltered[nNext])
			vTemp[i]=0;
		else
			vTemp[i]=vFiltered[i];
	}

	Veci vOIdx(180);
	CVUtil::SortIdx (vTemp, vTemp.Length(), vOIdx, 180, CVUtil::SORT_DEC);
	Vector<RectangleEdge> orientlines;
	RectangleEdge line;
	line.nAxis = vOIdx[0];
	line.fX = !fVertical;
	orientlines.Add (line);
				/*
	int nMaxOrient=vTemp[vOIdx[0]]/10;
	for (i=1; i<180; i++)
	{
		if (vTemp[vOIdx[i]] < nMaxOrient)
		{
			vOIdx[i]=-1;
			continue;
		}
		for (int k=0; k<i; k++)
		{
			if (vOIdx[k] < 0)
				continue;
			int dis=MIN(MIN(abs(vOIdx[i]-vOIdx[k]), abs(vOIdx[i]-vOIdx[k]+180)), abs(vOIdx[i]-vOIdx[k]-180));
			if ( dis < ORIENT_QUANT)
			{
				vOIdx[i]=-1;
				break;
			}
		}
	}
#ifdef TIMELOG
	timer.Stop();
	rtime=timer.GetElapsedTime();
	sprintf (szLog, "\tsearch line: filtering %.4fms", rtime*1000);
	m_log.Add(szLog);
	timer.Resume();
#endif

	for (i=0; i<180; i++)
	{
		if (vOIdx[i]<0) continue;
		if (vTemp[vOIdx[i]] > 10)
		{
			if (orientlines.GetSize() < 3)
			{
				if (fVertical)
				{
					if ( vOIdx[i] > ORIENT_DIR &&  vOIdx[i] < 180-ORIENT_DIR)
						continue;
				}
				else
				{
					if ( abs(vOIdx[i]-90) > ORIENT_DIR)
						continue;
				}
				RectangleEdge line;
				line.nAxis = vOIdx[i];
				line.fX = !fVertical;
				orientlines.Add (line);
			}
		}
	}
	*/
	lines.RemoveAll();
	for (i=0; i<orientlines.GetSize(); i++)
		Project (elems, orientlines[i], lines);
 #ifdef TIMELOG
	timer.Stop();
	rtime=timer.GetElapsedTime();
	sprintf (szLog, "\tsearch line: project %.4fms", rtime*1000);
	m_log.Add(szLog);
	timer.Resume();
#endif
	for (i=0; i<lines.GetSize(); i++)
	{
		if (fVertical)
		{
			if ( lines[i].nAxis > ORIENT_DIR &&  lines[i].nAxis < 180-ORIENT_DIR)
			{
				lines.RemoveAt(i);
				i--;
			}
		}
		else
		{
			if ( abs(lines[i].nAxis-90) > ORIENT_DIR)
			{
				lines.RemoveAt(i);
				i--;
			}
		}
	}
	return true;
}

void RectangleDetectFromPatternPos::FilteringCycle (const Veci& vIn, Veci& vOut, int* pnFilter, int nFilterlen)
{
	int nVecLen=vIn.Length();
	int i;
	vOut=0;
	int nSide=nFilterlen/2;
	for (i=0; i<nFilterlen/2; i++)
	{
		for (int k=0; k<nFilterlen; k++)
			vOut[i]+=pnFilter[k]*vIn[ (nVecLen+i-nSide+k) % nVecLen];
	}
	for (i=nFilterlen/2; i<nVecLen-nFilterlen/2; i++)
	{
		for (int k=0; k<nFilterlen; k++)
			vOut[i]+=pnFilter[k]*vIn[i-nSide+k];
	}
	for (i=nVecLen-nFilterlen/2; i<nVecLen; i++)
	{
		for (int k=0; k<nFilterlen; k++)
			vOut[i]+=pnFilter[k]*vIn[ (nVecLen+i-nSide+k) % nVecLen];
	}
}
void RectangleDetectFromPatternPos::Project (Vector<LineElem>& elems, RectangleEdge& projectInfo, Vector<RectangleEdge>& lines)
{
	projectInfo.fX=true;
	if (abs(projectInfo.nAxis-90) < MIN(abs(projectInfo.nAxis-180),abs(projectInfo.nAxis-0) ) )
		projectInfo.fX=false;

	int nAxis=projectInfo.nAxis;
	int nElemSize=elems.GetSize();
	int i;
	Veci ys(nElemSize);

	if (!projectInfo.fX)
	{
		for (i=0; i<nElemSize; i++)
			ys[i] = (int)(elems[i].y-tanf( (float)(nAxis*CVLIB_PI/180-CVLIB_PI/2.0f) ) * (float)(elems[i].x-0));
	}
	else
	{
		for (i=0; i<nElemSize; i++)
			ys[i] = (int)(elems[i].x-(float)(elems[i].y-0)/tanf( (float)(nAxis*CVLIB_PI/180-CVLIB_PI/2.0f) ));
	}

	int nMinY=(int)ys.Min();
	int nMaxY=(int)ys.Max();

	int nHistSide=10;
	Veci vHistogram(nMaxY-nMinY+1+nHistSide*2);
	Veci vFiltered(nMaxY-nMinY+1+nHistSide*2);
	vHistogram.Zero();
	for (i=0; i<nElemSize; i++)
	{
		vHistogram[ys[i]-nMinY+nHistSide]++;
	}

	int anFilter[] = {1, 6, 15, 20, 15, 6, 1};
	Filtering (vHistogram, vFiltered, anFilter, sizeof(anFilter)/sizeof(anFilter[0]));

	Vector<int> posAray;
	int imaxpos=-1;
	int nmaxFiltered=0;
	for (i=1; i<vFiltered.Length()-1; i++)
	{
		if (vFiltered[i]>=vFiltered[i-1] && vFiltered[i]>vFiltered[i+1])
		{
			posAray.Add(i+nMinY-nHistSide);
			if (vFiltered[i]>nmaxFiltered)
			{
				nmaxFiltered = vFiltered[i];
				imaxpos=i+nMinY-nHistSide;
			}
		}
	}
	posAray.RemoveAll();
	for (i=1; i<vFiltered.Length()-1; i++)
	{
		if (vFiltered[i]>=vFiltered[i-1] && vFiltered[i]>vFiltered[i+1])
		{
			if (vFiltered[i]>nmaxFiltered/2)
				posAray.Add (i+nMinY-nHistSide);
		}
	}
	int idx=imaxpos;
	for (int k=0; k<posAray.GetSize(); k++)
	{
		idx = posAray[k];

		RectangleEdge temp=projectInfo;
		if (!projectInfo.fX)
		{
			temp.nRealY=idx;
			for (i=0; i<nElemSize; i++)
			{
				if (abs(temp.nRealY-ys[i]) < 4)
					temp.vIdx.Add(i);
			}
		}
		else
		{
			temp.nRealX=idx;
			for (i=0; i<nElemSize; i++)
			{
				if (abs(temp.nRealX-ys[i]) < 4)
					temp.vIdx.Add(i);
			}
		}
		if (temp.vIdx.GetSize() > elems.GetSize()/6)
			lines.Add (temp);
	}
}
void RectangleDetectFromPatternPos::Filtering (const Veci& vIn, Veci& vOut, int* pnFilter, int nFilterlen)
{
	int nVecLen=vIn.Length();
	int i;
	vOut=0;
	int nSide=nFilterlen/2;
	for (i=nFilterlen/2; i<nVecLen-nFilterlen/2; i++)
	{
		for (int k=0; k<nFilterlen; k++)
			vOut[i]+=pnFilter[k]*vIn[i-nSide+k];
	}
}

void RectangleDetectFromPatternPos::DrawLine (RectangleEdge& line, Mat& image, COLORREF color)
{
	if (!line.pRegressor)
	{
		if (!line.fX)
		{
			float rk=tanf( (float)(line.nAxis*CVLIB_PI/180-CVLIB_PI/2.0f) );
			int nY1=CVUtil::Round(line.nRealY+rk*(image.Cols()-1));
			image.DrawLine( Point2i(0, line.nRealY), Point2i(image.Cols()-1, nY1), color);
		}
		else
		{
			float rk=tanf( (float)(line.nAxis*CVLIB_PI/180-CVLIB_PI/2.0f) );
			int nX1=CVUtil::Round(line.nRealX+(image.Rows()-1)/rk);
			image.DrawLine( Point2i(line.nRealX, 0), Point2i(nX1, image.Rows()-1), color);
		}
	}
	else
	{
		if (!line.fX)
		{
			Vecf v(1);
			for (int i=0; i<image.Cols(); i++)
			{
				v[0]=(float)i;
				int nY1=(int)line.pRegressor->Eval(&v);
				image.DrawPixel(i, nY1, color);
			}
		}
		else
		{
			Vecf v(1);
			for (int i=0; i<image.Rows(); i++)
			{
				v[0]=(float)i;
				int nX1=(int)line.pRegressor->Eval(&v);
				image.DrawPixel(nX1, i, color);
			}
		}
	}
}

bool RectangleDetectFromPatternPos::FittingLine (Vector<Point2i>& points, RectangleEdge& line)
{
	LinearRegressorTrainer trainer;	

	DataSet data(1, line.vIdx.GetSize());
	int i;
	if (!line.fX)
	{
		for (i=0; i<line.vIdx.GetSize(); i++)
		{
			Point2i& point=points[line.vIdx[i]];
			data.m_pprData[i][0]=point.x;
			data.m_prCls[i]=point.y;
		}
	}
	else
	{
		for (i=0; i<line.vIdx.GetSize(); i++)
		{
			Point2i& point=points[line.vIdx[i]];
			data.m_pprData[i][0]=point.y;
			data.m_prCls[i]=point.x;
		}
	}
	line.pRegressor = (LinearRegressor*)trainer.Train(&data);
	if (!line.pRegressor)
		return false;
	if (!line.fX)
	{
		Vecf vt(1);vt[0]=0;
		line.nRealY=(int)line.pRegressor->Eval(&vt);
	}
	else
	{
		Vecf vt(1);vt[0]=0;
		line.nRealX=(int)line.pRegressor->Eval(&vt);
	}
	return true;
}

}