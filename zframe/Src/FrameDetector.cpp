#include "FrameDetector.h"

#include "cvlibbase/inc/_cvlibbase.h"
#include "Canny.h"
#include "ColorSpace.h"
#include "warping.h"
#include "ScaleXY.h"
#include "ipMedian.h"
#include "IntegralImage.h"
#include "FindContour.h"
#include "thining.h"
#include "thresh.h"


namespace CVLib
{

#define CARD_STD_WIDTH			800
//#define CARD_STD_WIDTH			240

#define THRESH_PATTERN			50.0f
#define STD_ASPECT				1.5333f //(23/15)

#define CARD_VERI_REGION_SIDE	75
#define CARD_DETECT_LEN         1
    
static 	float mrscale=1.5f;

FrameDetector::FrameDetector() : m_rectDetector(STD_ASPECT, 
#ifdef ENGINE_DEBUG
	&m_debugArray
#else
	0
#endif
	)
{
	m_fInit=false;
}
FrameDetector::~FrameDetector()
{
#ifdef ENGINE_DEBUG
	m_debugArray.Release();
#endif
}

bool FrameDetector::init (const Mat& image, const PackageInfo& packageInfo)
{
//    int nHeight=image.Rows();
//    int nWidth=image.Cols();

    /*region.width=nWidth*320/360;
    region.height=nWidth*427/360;
    region.x=(nWidth-region.width)/2;
    region.y=(nHeight-region.height)/2;*/
	m_searchRegion=Rect(0,0,image.Cols(),image.Rows());

	m_fInit=true;
	m_nframeIdx=0;
	m_fExistTrack = false;
    m_detectArray.SetSize (CARD_DETECT_LEN);
	memset (m_detectArray.GetData(), 0, sizeof(m_detectArray[0])*CARD_DETECT_LEN);
	m_rAspect=packageInfo.height / (float)packageInfo.width;
	m_packageInfo=packageInfo;
	return m_fInit;
}
bool FrameDetector::flagProperSize(int percent) const
{
    int widthLen = (int)m_cardcorners[0].DistTo(m_cardcorners[1]);
    if (m_searchRegion.width * percent / 100 < widthLen)
        return true;
    else
        return false;
}

bool FrameDetector::detect (const Mat& image)
{
	bool state=false;
	if (!m_fInit)
		return state;
    
	Rect region;

	m_src=image;
//    int nHeight=image.Rows();
//    int nWidth=image.Cols();
	int nregionx=(int)(image.Cols()*0.1f);
	int nregiony=(int)(image.Rows()*0.1f);
	m_image.Create (image.Rows()+nregiony*2, image.Cols()+nregionx*2, image.Type1());
	m_image=255;
	m_image.DrawMat (image, Point2i(nregionx, nregiony));

//	region=Rect(0,0,nWidth,nHeight);
   /* region.width=nWidth*170/180;
    region.height=nWidth*230/180;
    region.x=(nWidth-region.width)/2;
    region.y=(nHeight-region.height)/2;

	if (!MatOp::CropMat (&m_image, &image, region))
		return state;*/

#ifdef ENGINE_DEBUG
	m_debugArray.Release();
#endif
#ifdef TIMELOG
	char sztemp[260];
	SString szlog;
	double rtime;
	m_log.RemoveAll();
	m_timer.Reset();
	m_timer.Resume();
	m_log.RemoveAll();
#endif

#ifdef TIMELOG
	rtime=m_timer.GetElapsedTime()*1000;
	sprintf (sztemp, "rgb->gray tansformation time (%dx%d) - %.8f ms", m_gray.Rows(), m_gray.Cols(), rtime);
	m_log.Add (sztemp);
	m_timer.Resume();
#endif
    mrscale = (float)m_image.Rows() / 230.0f;
	Mat scaled;
	scaleImage (m_image, scaled, mrscale);

	if (SExtract (scaled, m_cardcorners_internal))
	{
		m_cardcorners.SetSize(m_cardcorners_internal.GetSize());
		for (int i=0; i<m_cardcorners_internal.GetSize(); i++)
		{
			m_cardcorners[i].x = (int)(m_cardcorners_internal[i].x*mrscale)-nregionx;
			m_cardcorners[i].y = (int)(m_cardcorners_internal[i].y*mrscale)-nregiony;
		}
		state = true;
		for (int i=0; i<m_cardcorners_internal.GetSize(); i++)
        {
            if (!image.IsInterior(m_cardcorners[i].x, m_cardcorners[i].y))
                state=false;
        }
        if (state)
            m_detectArray.Add(1);
        else
            m_detectArray.Add(0);
	}
	else
	{
		m_cardcorners_internal.RemoveAll();
		m_cardcorners.RemoveAll();
		state = false;
        m_detectArray.Add(0);
	}
    m_detectArray.RemoveAt(0);
#ifdef TIMELOG
	rtime=m_timer.GetElapsedTime()*1000;
	sprintf (sztemp, "border detect time %.8f ms", rtime);
	m_log.Add (sztemp);
	m_timer.Resume();
#endif

	return state;
}
bool FrameDetector::flagFocus() const
{
    int i;
    bool ffocus=true;
    for (i=0; i<m_detectArray.GetSize(); i++)
    {
        if(m_detectArray[i]==0)
        {
            ffocus=false;
            break;
        }
    }
    return ffocus;
}

void FrameDetector::getCardRegion(Vector<Point2i>& region)
{
	region.RemoveAll();
	if (!m_fInit)
		return;
	region.Append (m_cardcorners);
}
bool FrameDetector::crop (Mat& croped)
{
    int nheight=(int)(CARD_STD_WIDTH*m_rAspect);
    Matf mH;
    croped.Create (nheight, CARD_STD_WIDTH, MAT_Tbyte3);
    calcAffineTransform(m_cardcorners, mH);
    ip::WarpImage2 (m_src, croped, mH);
    extractBound (croped);
    return true;
}

bool FrameDetector::crop (const Mat& frame, Mat& croped, Vector<Point2i>& region)
{
    int nheight=(int)(CARD_STD_WIDTH*m_rAspect);
    Matf mH;
    croped.Create (nheight, CARD_STD_WIDTH, MAT_Tbyte3);
    calcAffineTransform(region, mH);
    ip::WarpImage2 (frame, croped, mH);
    extractBound (croped);
    return true;
}
void FrameDetector::calcAffineTransform (Vector<Point2i>& points, Matf& mAffine)
{
    int nheight=(int)(CARD_STD_WIDTH*m_rAspect);
	int i;
	Vector<Point2i> dsts;
	dsts.Add(Point2i(0,0));
	dsts.Add(Point2i(CARD_STD_WIDTH,0));
	dsts.Add(Point2i(CARD_STD_WIDTH,nheight));
	dsts.Add(Point2i(0,nheight));
	int _nN=points.GetSize();

	Mat mH(2, 3, MAT_Tdouble);
	mH.Zero();

	Mat A(2 * _nN, 6, MAT_Tdouble);
	Mat B(2 * _nN, 1, MAT_Tdouble);
	Mat X(6, 1, MAT_Tdouble);

	A.Zero();

	for( i = 0; i < _nN; i++ )
	{
		A.data.db[i][0] = points[i].x;
		A.data.db[i+_nN][3] = points[i].x;
		A.data.db[i][1] = points[i].y;
		A.data.db[i+_nN][4] = points[i].y;
		A.data.db[i][2] = 1.0;
		A.data.db[i+_nN][5] = 1.0;
		B.data.db[i][0] = dsts[i].x ;
		B.data.db[i+_nN][0] = dsts[i].y ;
	}

	SingularValueDecomposition SVDecomp(&A);

	int nRow = A.Rows();
	int nCol = A.Cols();
	int nMin = MIN(nRow, nCol);

	Mat PseudoIMat(nCol, nRow, MAT_Tdouble);
	Mat V(nCol, nCol,  MAT_Tdouble);
	Mat U(nMin, nRow, MAT_Tdouble);
	Mat SingularV(nCol, nMin, MAT_Tdouble);
	Mat matTemp(nCol, nMin, MAT_Tdouble);

	SingularV.Zero();
	PseudoIMat.Zero();

	Mat* pmU = SVDecomp.GetU();
	Mat* pmV = SVDecomp.GetV();
	for (i = 0; i < nRow; i ++)
	{
		for (int j = 0; j < nMin; j ++)
		{
			U.data.db[j][i] = pmU->data.db[i][j];
		}
	}

	for (i = 0; i < nCol; i ++)
	{
		for (int j = 0; j < nCol; j ++)
		{
			V.data.db[i][j] = pmV->data.db[i][j];
		}
	}

	for (i = 0; i < nCol; i ++)
		SingularV.data.db[i][i] = (float)1 / SVDecomp.SingularValues()[i];

	ReleaseMat(pmU);
	ReleaseMat(pmV);

	MatOp::Mul(&matTemp, &V, &SingularV);
	MatOp::Mul(&PseudoIMat, &matTemp, &U);
	MatOp::Mul(&X, &PseudoIMat, &B);

	//////////////////////////////////////////////////////////////////////////
	for (i = 0; i < 6; i++)
	{
		mH.data.db[i / 3][i % 3] = X.data.db[i][0];
	}

	mAffine.Release();
	mAffine.Create (3,3); mAffine.Zero(); mAffine[2][2]=1.0f;
	for (i=0; i<6; i++)
		mAffine[0][i]=(float)mH.data.db[0][i];
}

void FrameDetector::getLog (StringArray& log)
{
	log.Append(m_log);
}

void FrameDetector::scaleImage(const Mat& src, Mat& scaled, float mrscale)
{
	ScaleXY scale ;
	int nH=src.Rows();
	int nW=src.Cols();
	nH = (int)(nH/mrscale+0.5f);
	nW = (int)(nW/mrscale+0.5f);
	scaled.Create (nH, nW, src.Type1());
	scale.FastProcess( &src, &scaled ) ;
}

void FrameDetector::binaryImage(const Mat& src, Mat& bin)
{
	bin.Create (src.Rows(), src.Cols(), MAT_Tbyte);
	BYTE* pbsrc=src.data.ptr[0];
	BYTE* pbdst=bin.data.ptr[0];
	int datalen=src.Rows()*src.Cols();
	for (int i=0, k=0; i<datalen; i++, k+=3)
	{
		if (pbsrc[k]<128)
			pbdst[i]=0;
		else
			pbdst[i]=255;
	}
}
void FrameDetector::edgeImage(const Mat& src, Mat& edge)
{
	int nH = src.Rows() ;
	int nW = src.Cols() ;
	edge.Create ( nH, nW, MAT_Tbyte ) ;	
#ifdef TIMELOG
	char sztemp[260];
	double rtime=m_timer.GetElapsedTime()*1000;
	sprintf (sztemp, "median filtering time %.8f ms", rtime);
	m_log.Add (sztemp);
	m_timer.Resume();
#endif
	Canny canny ;
	canny.Create( nW, nH ) ;
	canny.m_nHighThreshold = 50 ;
	canny.m_nLowThreshold = 0 ;
  	canny.Process( (Mat*)&src, &edge );
#ifdef TIMELOG
	rtime=m_timer.GetElapsedTime()*1000;
	sprintf (sztemp, "canny edge time %.8f ms", rtime);
	m_log.Add (sztemp);
	m_timer.Resume();
#endif
}

#ifdef ENGINE_DEBUG
void FrameDetector::getDebugInfo(Mat& image)
{
	m_debugArray.GetAppend(AxeX, AT_Up, image);
}

#endif

static void Dilate (Mat& bin)
{
	Mat temp(bin);
	int ksize=2;
	int k2=ksize/2;
	int kmax=ksize-k2;
	int ymin=ksize/2;
	int ymax=bin.Rows()-ksize/2;
	int xmin=ksize/2;
	int xmax=bin.Cols()-ksize/2;
	int r;
	BYTE** ppbdata=temp.data.ptr;
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			if (ppbdata[y][x]!=0)
				continue;
			bool f=true;
			r=0;
			for(long j=-k2;j<kmax && f;j++)
			{
				for(long k=-k2;k<kmax && f;k++)
				{
					if (j==0 || k==0)
						continue;
					if (ppbdata[y+j][x+k]!=0)//255
					{
						f=false;
					}
				}
			}
			if (!f)
				bin.data.ptr[y][x]=255;
		}
	}
}
void FrameDetector::filteringVertical  (const Mat& gray, const Rect& region, Vector<Point2i>& points, Mat& work)
{
	int kx=3, ky=5;
	int kx2=kx/2, ky2=ky/2;
	work.Create (gray.SizeObject(), MAT_Tbyte);
	work.Zero();
	int x, y;
	for (y=region.y+ky2; y<region.LimY()-ky2; y++)
	{
		for (x=region.x+kx2; x<region.LimX()-kx2; x++)
		{
			if (gray.data.ptr[y][x] == 255)
				continue;
			bool fedge=true;
			for (int iy=y-ky2; iy<y+ky2; iy++)
			{
				if (gray.data.ptr[iy][x]==255)
				{
					fedge=false;
					break;
				}
			}
			if (fedge)
			{
				points.Add (Point2i(x,y));
				work.data.ptr[y][x]=1;
			}
		}
	}
	int i;
	int nThreshold=4;
	for (i=0; i<points.GetSize(); i++)
	{
		int cx=points[i].x;
		int cy=points[i].y;
		int nsum1=0, nsum2=0, nsum3=0, nsum4=0;
		for (y=cy-1; y<=cy+1; y++)
			nsum1 += work.data.ptr[y][cx-1];
		for (y=cy-1; y<=cy+1; y++)
			nsum2 += work.data.ptr[y][cx];
		for (y=cy-1; y<=cy+1; y++)
			nsum3 += work.data.ptr[y][cx+1];
		for (y=cy-1; y<=cy+1; y++)
			nsum4 += work.data.ptr[y][cx+2];
		if (nsum2*2-nsum1-nsum3 > nThreshold || nsum2+nsum3-nsum1-nsum4 > nThreshold)
			continue;
		points.RemoveAt(i);
		i--;
	}
}

void FrameDetector::filteringHorizental (const Mat& gray, const Rect& region, Vector<Point2i>& points, Mat& work)
{
	int kx=5, ky=3;
	int kx2=kx/2, ky2=ky/2;
	work.Create (gray.SizeObject(), MAT_Tbyte);
	work.Zero();
	int x, y;
	for (y=region.y+ky2; y<region.LimY()-ky2; y++)
	{
		for (x=region.x+kx2; x<region.LimX()-kx2; x++)
		{
			if (gray.data.ptr[y][x] == 255)
				continue;
			bool fedge=true;
			for (int ix=x-kx2; ix<x+kx2; ix++)
			{
				if (gray.data.ptr[y][ix]==255)
				{
					fedge=false;
					break;
				}
			}
			if (fedge)
			{
				points.Add (Point2i(x,y));
				work.data.ptr[y][x]=1;
			}
		}
	}
	int i;
	int nThreshold=4;
	for (i=0; i<points.GetSize(); i++)
	{
		int cx=points[i].x;
		int cy=points[i].y;
		int nsum1=0, nsum2=0, nsum3=0, nsum4=0, nsum5=0;
		for (x=cx-1; x<=cx+1; x++)
			nsum1 += work.data.ptr[cy-1][x];
		for (x=cx-1; x<=cx+1; x++)
			nsum2 += work.data.ptr[cy][x];
		for (x=cx-1; x<=cx+1; x++)
			nsum3 += work.data.ptr[cy+1][x];
		for (x=cx-1; x<=cx+1; x++)
			nsum4 += work.data.ptr[cy+2][x];
		for (x=cx-1; x<=cx+1; x++)
			nsum5 += work.data.ptr[cy+3][x];
		if (nsum2*2-nsum1-nsum3 > nThreshold || nsum2+nsum3-nsum1-nsum4 > nThreshold/* ||
			(nsum2+nsum3+nsum4)*2-(nsum1+nsum5)*3 > nThreshold*3/2*/)
			continue;
		points.RemoveAt(i);
		i--;
	}
}
static int surroundingLength(Vector<Point2i>& border)
{
	float len=0.0f;
	for (int i=0; i<border.GetSize(); i++)
	{
		int nnum=(i+1)%border.GetSize();
		len += (float)border[i].DistTo(border[nnum]);
	}
	return (int)len;
}
bool FrameDetector::SExtract (const Mat& gray, Vector<Point2i>& border)
{
	Mat mScaled, mBin;
	float rscale;

	int flagFirstProceess = 0;
	bool fresult=false;

	while (true)
	{
		if (flagFirstProceess==2)
		{
			rscale=1.0f;
			scaleImage (gray, mScaled, rscale);
			binaryImage (mScaled, mBin);
			Vector<Rect> rects_;
			findRects (mBin, rects_);

			int kk=0;
			for (int i=0; i<rects_.GetSize(); i++)
			{
				Rect r=rects_[i];
				if (r.width < mBin.Cols()/6 || r.height < mBin.Rows()/4)
				{
					for (int y=r.y; y<=r.LimY(); y++) for (int x=r.x; x<=r.LimX(); x++)
					{
						mBin.data.ptr[y][x]=255;
					}
					kk++;
				}
			}
		}
		else
		{
			rscale=2.0f;
			scaleImage (gray, mScaled, rscale);
			binaryImage (mScaled, mBin);
			if (flagFirstProceess==1)
				Dilate (mBin);
			Vector<Rect> rects_;
			findRects (mBin, rects_);

			int kk=0;
			for (int i=0; i<rects_.GetSize(); i++)
			{
				Rect r=rects_[i];
				if (r.width < mBin.Cols()/6 && r.height < mBin.Rows()/4)
				{
					for (int y=r.y; y<=r.LimY(); y++) for (int x=r.x; x<=r.LimX(); x++)
					{
						mBin.data.ptr[y][x]=255;
					}
					kk++;
				}
			}
		}

		Mat mwork;

		int i;
		Vector<Point2i>* points[]={&points_edge1, &points_edge2, &points_edge3, &points_edge4, 0};
		for (i=0; points[i]; i++)
			points[i]->RemoveAll();

		int nside=2;
		Rect regionLeft (nside,nside,mBin.Cols()*3/7-nside, mBin.Rows()-nside*2);
		filteringVertical (mBin, regionLeft, points_edge1, mwork);
		Rect regionUp (nside,nside,mBin.Cols()-nside*2, mBin.Rows()*3/7-nside);
		filteringHorizental (mBin, regionUp, points_edge2, mwork);
		Rect regionRight (mBin.Cols()*4/7,nside,mBin.Cols()-mBin.Cols()*4/7-nside*2, mBin.Rows()-nside*2);
		filteringVertical (mBin, regionRight, points_edge3, mwork);
		Rect regionDown (nside,mBin.Rows()*4/7-nside,mBin.Cols()-nside*2, mBin.Rows()-mBin.Rows()*4/7-nside);
		filteringHorizental (mBin, regionDown, points_edge4, mwork);

	#ifdef TIMELOG
		{
			char sztemp[260];
			double rtime=m_timer.GetElapsedTime()*1000;
			sprintf (sztemp, "search square detect time %.8f ms", rtime);
			m_log.Add (sztemp);
			sprintf (sztemp, "point %d %d %d %d", points_edge1.GetSize(),points_edge2.GetSize(),
				points_edge3.GetSize(),points_edge4.GetSize());
			m_log.Add (sztemp);
			m_timer.Resume();
		}
	#endif
	#ifdef ENGINE_DEBUG
		{
			int i;
			Vector<Point2i>* points[]={&points_edge1, &points_edge2, &points_edge3, &points_edge4, 0};
			for (i=0; points[i]; i++)
			{
				Mat* pnew=new Mat(mBin.Rows(), mBin.Cols(), MAT_Tbyte3);
				ColorSpace::GraytoRGB(mBin, *pnew);
				for (int k=0; k<points[i]->GetSize()-1; k++)
					pnew->DrawPixel(points[i]->GetAt(k), RGB(255,0,0));
				m_debugArray.Add (pnew);
			}
		}
	#endif
	
		fresult=m_rectDetector.Extract(mBin, points, border);
		if (flagFirstProceess==2)
		{
			int nlen=surroundingLength (border);
			if (nlen < (gray.Rows()+gray.Cols())*0.9f)
				fresult=false;
		}
		if (fresult)
			break;
		if (flagFirstProceess==2)
			break;
		flagFirstProceess++;
	}
#ifdef TIMELOG
	{
		double rtime=m_timer.GetElapsedTime()*1000;
		char sztemp[260];
		sprintf (sztemp, "square detect time %.8f ms", rtime);
		m_log.Add (sztemp);
		m_timer.Resume();
	}
#endif
	if (fresult)
	{
        if (border.GetSize())
        {
            for (int i=0; i<border.GetSize(); i++)
            {
                border[i].x = (int)(border[i].x*rscale);
                border[i].y = (int)(border[i].y*rscale);
            }
        }
        else{
            fresult=false;
        }
	}
	
	return fresult;
}
Point2i FrameDetector::extractCorner (const Mat& src, COLORREF backColor, COLORREF dotColor)
{
	Mat respondImage1, respondImage2;
	respondImage1.Create (src.Rows(), src.Cols(), MAT_Tint);
	respondImage2.Create (src.Rows(), src.Cols(), MAT_Tint);
	uchar backcolor[]={ GetRValue(backColor),GetGValue(backColor),GetBValue(backColor)};
	uchar dotcolor[]={ GetRValue(dotColor),GetGValue(dotColor),GetBValue(dotColor)};
	for (int iy=0; iy<src.Rows(); iy++) 
	{
		uchar* pdata=src.data.ptr[iy];
		int k=0;
		for (int ix=0; ix<src.Cols(); ix++, k+=3)
		{
			int dis1=abs(pdata[k]-backcolor[0])+abs(pdata[k+1]-backcolor[1])+abs(pdata[k+2]-backcolor[2]);
			respondImage1.data.i[iy][ix]=dis1;
			int dis2=abs(pdata[k]-dotcolor[0])+abs(pdata[k+1]-dotcolor[1])+abs(pdata[k+2]-dotcolor[2]);
			respondImage2.data.i[iy][ix]=dis2;
		}
	}
	Mat bin1;
	respondImage1.Convert (MAT_Tbyte);
	respondImage2.Convert (MAT_Tbyte);
	ip::Threshold (respondImage1, bin1, 50, 255, THRESH_BINARY);
	for (int i=0; i<bin1.Rows()*bin1.Cols(); i++)
	{
		if (bin1.data.ptr[0][i]==255)
			continue;
		respondImage2.data.ptr[0][i]=255;
	}
	ip::Threshold (respondImage2, bin1, 8, 255, THRESH_TILE);

	Point2i center;
	int count=0;
	for (int iy=0; iy<bin1.Rows(); iy++)
	for (int ix=0; ix<bin1.Cols(); ix++)
	{
		if (bin1.data.ptr[iy][ix]==255)
			continue;
		center.x += ix;
		center.y += iy;
		count++;
	}
	center.x /= count;
	center.y /= count;

#ifdef ENGINE_DEBUG
	m_debugArray.Add (new Mat(bin1));
//	m_debugArray.Add (new Mat(respondImage2));
#endif
	return center;
}

void FrameDetector::extractBound(Mat& src)
{
	int nside=src.Cols() / 20;
	int patchwidth=m_packageInfo.corners[2].x+m_packageInfo.corners[0].x;
	Rect searchRegion[4];
	float rscale=(float)src.Cols()/(float)patchwidth;

	Point2i corners4[4];
	for (int i=0; i<4; i++)
	{
		searchRegion[i]=Rect( (int)(rscale*m_packageInfo.corners[i].x+0.5f)-nside, 
			(int)(rscale*m_packageInfo.corners[i].y+0.5f)-nside, nside*2, nside*2);
		Mat subMat, respondMat1, respondMat2;
		src.SubMat(searchRegion[i], subMat);
		Point2i pt=extractCorner (subMat, m_packageInfo.backgroundColor, m_packageInfo.dotColor);
		pt.x += searchRegion[i].x;
		pt.y += searchRegion[i].y;
		corners4[i]=pt;
	}

	src.DrawLine(corners4[0], corners4[1], RGB(255,0,0));
	src.DrawLine(corners4[0], corners4[2], RGB(255,0,0));
	src.DrawLine(corners4[2], corners4[3], RGB(255,0,0));
	src.DrawLine(corners4[1], corners4[3], RGB(255,0,0));
	Point2f vx, vy;
	vx.x=(corners4[2].x-corners4[0].x)/(float)m_packageInfo.cols;
	vx.y=(corners4[2].y-corners4[0].y)/(float)m_packageInfo.cols;
	vy.x=(corners4[1].x-corners4[0].x)/(float)m_packageInfo.rows;
	vy.y=(corners4[1].y-corners4[0].y)/(float)m_packageInfo.rows;
	for (int iy=1; iy<m_packageInfo.rows; iy++)
	{
		Point2i pt1, pt2;
		pt1.x=(int)(corners4[0].x+vy.x*iy+0.5f);
		pt1.y=(int)(corners4[0].y+vy.y*iy+0.5f);
		pt2.x=(int)(corners4[2].x+vy.x*iy+0.5f);
		pt2.y=(int)(corners4[2].y+vy.y*iy+0.5f);
		src.DrawLine(pt1, pt2, RGB(255,0,0));
	}
	for (int ix=1; ix<m_packageInfo.cols; ix++)
	{
		Point2i pt1, pt2;
		pt1.x=(int)(corners4[0].x+vx.x*ix+0.5f);
		pt1.y=(int)(corners4[0].y+vx.y*ix+0.5f);
		pt2.x=(int)(corners4[1].x+vx.x*ix+0.5f);
		pt2.y=(int)(corners4[1].y+vx.y*ix+0.5f);
		src.DrawLine(pt1, pt2, RGB(255,0,0));
	}
}


}