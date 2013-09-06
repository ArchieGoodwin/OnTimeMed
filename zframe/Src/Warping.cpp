#include "Warping.h"
#include "cvlibbase/inc/matoperation.h"

namespace CVLib{ namespace ip{

float TriangleArea (float rX1, float rY1, 
					float rX2, float rY2,
					float rX3, float rY3)
{
	float rArea = 0.0f;
	rArea = (rX3 - rX2) * (rY1 - rY3) + (rX3 - rX1) * (rY3 - rY2);
	rArea = (float)fabs (rArea);
	return rArea;
}

static float BilinearInterpolation ( BYTE** ppbImage, int nX, int nY, float rX, float rY )
{
	float	rTemp;
	
	rTemp = ppbImage [ nY ][ nX ];
	if ( rX )
		rTemp += rX * ( ppbImage [ nY ][ nX + 1 ] - ppbImage [ nY ][ nX ] );
	
	if ( rY )
		rTemp += rY * ( ppbImage [ nY + 1 ][ nX ] - ppbImage [ nY ][ nX ] );
	
	if ( rX && rY )
		rTemp += rX * rY * (
		ppbImage [ nY + 1 ][ nX + 1 ] + ppbImage [ nY ][ nX ] -
		ppbImage [ nY ][ nX + 1 ] - ppbImage [ nY + 1 ][ nX ] );
	
	return rTemp;
}

bool Warping (CoImage* pInImage, CoImage* pOutImage, Rect* pWarpingRect, 
			  Point2f* pSrcPoints, Point2f* pDstPoints, TriangleModel* pTriangleModel)
{
	if (!pOutImage->IsValid())
		pOutImage->Create(*pInImage, TRUE);

	pWarpingRect->x = pInImage->GetWidth() - 1;
	pWarpingRect->y = pInImage->GetHeight() - 1;
	pWarpingRect->width = 1;
	pWarpingRect->height = 1;

	float rX1, rY1, rX2, rY2, rX3, rY3;
	float rX, rY;
	
	int nTriangleCount = pTriangleModel->m_TriangleList.GetSize();
	int iTri;
	int nLeft, nTop, nRight, nBottom;

	for(iTri = 0; iTri < nTriangleCount; iTri ++)
	{
		Triangle* pTriangle = (Triangle*)pTriangleModel->m_TriangleList.GetAt(iTri);
		rX1 = (float)pDstPoints[pTriangle->n1].x, rY1 = (float)pDstPoints[pTriangle->n1].y;
		rX2 = (float)pDstPoints[pTriangle->n2].x, rY2 = (float)pDstPoints[pTriangle->n2].y;
		rX3 = (float)pDstPoints[pTriangle->n3].x, rY3 = (float)pDstPoints[pTriangle->n3].y;
		
		nLeft = (int)(MIN(rX1, MIN(rX2, rX3)));
		nRight = (int)(MAX(rX1, MAX(rX2, rX3)));
		nTop = (int)(MIN(rY1, MIN(rY2, rY3)));
		nBottom = (int)(MAX(rY1, MAX(rY2, rY3)));
		nTop = MAX(0,nTop);
		nLeft = MAX(0,nLeft);
		nTop = MIN(nTop,pOutImage->GetHeight());
		nLeft = MIN(nLeft,pOutImage->GetWidth());
		nBottom = MIN(nBottom,pOutImage->GetHeight()-1);
		nRight = MIN(nRight,pOutImage->GetWidth()-1);
		for (int iY = nTop ; iY <= nBottom ; iY ++)
		{
			for (int iX = nLeft ; iX <= nRight ; iX ++)
			{
				int nPtX = iX;
				int nPtY = iY;
				rX = (float)iX;
				rY = (float)iY;
				
				float rS = TriangleArea (rX1, rY1, rX2, rY2, rX3, rY3);
				float rS1 = TriangleArea (rX, rY, rX2, rY2, rX3, rY3);
				float rS2 = TriangleArea (rX, rY, rX1, rY1, rX3, rY3);
				float rS3 = TriangleArea (rX, rY, rX1, rY1, rX2, rY2);
				if ((rS1 + rS2 + rS3) - rS > 0.001f) // 3°¢?EÆþÔ§?¼°
					continue;
				// ¡¡EÌ©Ðú?°Óºã¶¦ ½Ü²÷³Þ.
				float rA = 0.1f, rB = 0.5f, rC = 1.0f - rA - rB;
				ComputeCoefficients (pDstPoints, pTriangle, (float)nPtX, (float)nPtY, &rA, &rB, &rC);
				float rX1, rY1, rX2, rY2, rX3, rY3;
				rX1 = (float)pSrcPoints[pTriangle->n1].x, rY1 = (float)pSrcPoints[pTriangle->n1].y;
				rX2 = (float)pSrcPoints[pTriangle->n2].x, rY2 = (float)pSrcPoints[pTriangle->n2].y;
				rX3 = (float)pSrcPoints[pTriangle->n3].x, rY3 = (float)pSrcPoints[pTriangle->n3].y;
				
				float rWarpX = rX1 * rA + rX2 * rB + rX3 * rC;
				float rWarpY = rY1 * rA + rY2 * rB + rY3 * rC;
				int nWarpX = (int)rWarpX; rWarpX -=  nWarpX;
				int nWarpY = (int)rWarpY; rWarpY -=  nWarpY;

				if (nWarpX < 0 || nWarpY < 0 || nWarpX >= pInImage->GetWidth()-1 || nWarpY >= pInImage->GetHeight()-1)
				{
					pOutImage->m_matX.data.ptr[nPtY][nPtX] = 128;
					pOutImage->m_matY.data.ptr[nPtY][nPtX] = 128;
					pOutImage->m_matZ.data.ptr[nPtY][nPtX] = 128;
				}
				else
				{
					BYTE byteR = (BYTE)BilinearInterpolation (pInImage->m_matX.data.ptr, nWarpX, nWarpY, rWarpX, rWarpY);
					BYTE byteG = (BYTE)BilinearInterpolation (pInImage->m_matY.data.ptr, nWarpX, nWarpY, rWarpX, rWarpY);
					BYTE byteB = (BYTE)BilinearInterpolation (pInImage->m_matZ.data.ptr, nWarpX, nWarpY, rWarpX, rWarpY);
					pOutImage->m_matX.data.ptr[nPtY][nPtX] = byteR;
					pOutImage->m_matY.data.ptr[nPtY][nPtX] = byteG;
					pOutImage->m_matZ.data.ptr[nPtY][nPtX] = byteB;
				}
			}
		}

		pWarpingRect->x = MIN(pWarpingRect->x, nLeft);
		pWarpingRect->y = MIN(pWarpingRect->y, nTop);
		pWarpingRect->width = MAX(pWarpingRect->width, nRight - pWarpingRect->x + 1);
		pWarpingRect->height = MAX(pWarpingRect->height, nBottom - 	pWarpingRect->y  + 1);
	}
	
	return true;
}

void ComputeCoefficients (Point2f* pPoints, Triangle* pTriangle, float rX, float rY, float* prA, float* prB, float* prC)
{
	if(!pPoints || ! pTriangle)
		return;
	
	float rX1, rY1, rX2, rY2, rX3, rY3;
	rX1 = (float)pPoints[pTriangle->n1].x, rY1 = (float)pPoints[pTriangle->n1].y;
	rX2 = (float)pPoints[pTriangle->n2].x, rY2 = (float)pPoints[pTriangle->n2].y;
	rX3 = (float)pPoints[pTriangle->n3].x, rY3 = (float)pPoints[pTriangle->n3].y;
	float rA1, rA2, rB1, rB2, rC1, rC2;
	rA1 = rX1 - rX3; rB1 = rX2 - rX3; rC1 = rX - rX3;
	rA2 = rY1 - rY3; rB2 = rY2 - rY3; rC2 = rY - rY3;
	float rTemp = rA1 * rB2 - rA2 * rB1;
	*prA = (rC1 * rB2 - rC2 * rB1) / rTemp;
	*prB = (rA1 * rC2 - rA2 * rC1) / rTemp;
	*prC = 1.0f - (*prA) - (*prB);
	//	assert ((*prA) >= 0.0f);
	//	assert ((*prB) >= 0.0f);
	//	assert ((*prC) >= 0.0f);
}

void InitWarp (Mat& W, float a, float b, float c, float d, float tx, float ty)
{
	float data[9]={1.0f+a, b, tx,
					c, 1.0f+d, ty,
					0.0f, 0.0f, 1.0f};
	memcpy (W.data.fl[0], data, sizeof(float)*9);
}
typedef void (*BilinearInterpolationFunc)(uchar** _dst, int x, int y, uchar** const _src, float rX, float rY, int channel);
template<typename T> static void
BilinearInterpolation_( uchar** _dst, int x, int y, uchar** const _src, float rX, float rY, int channel)
{
	T** const src=(T** const)_src;
	T** dst=(T**)_dst;
	int nX = (int)rX; rX -=  nX;
	int nY = (int)rY; rY -=  nY;
	int n=nX*channel;
	int dstx=x*channel;
    T* prcur=(T*)src [ nY ];
    T* prnext=(T*)src [ nY+1 ];
    T* pdst=(T*)dst[y];
	for (int i=0; i<channel; i++, n++)
	{
		int n1=n+channel;
		float rTemp = (float)prcur[ n ];

		if ( rX )
			rTemp += rX * (float)( prcur[n1] - prcur[n] );
		if ( rY )
			rTemp += rY * (float)( prnext[n] - prcur[n] );
		if ( rX && rY )
			rTemp += rX * rY * (float)(prnext[n1] + prcur[n] -prcur[n1] - prnext[n] );
		pdst[dstx++]=(T)rTemp;
	}
}
void WarpImage (const Mat& src, Mat& dst, const Mat& W)
{
	BilinearInterpolationFunc Tab[] =
	{
		NULL, 
		BilinearInterpolation_<BYTE>, 
		BilinearInterpolation_<short>, 
		BilinearInterpolation_<int>, 
		BilinearInterpolation_<float>, 
		BilinearInterpolation_<double> 
	};
	BilinearInterpolationFunc func=Tab[(int)src.Type()];

	Vector<Point2i> vertexSrc, vertexDst;
	vertexSrc.Add (Point2i(0,0));
	vertexSrc.Add (Point2i(0,src.Rows()));
	vertexSrc.Add (Point2i(src.Cols(),src.Rows()));
	vertexSrc.Add (Point2i(src.Cols(),0));
	WarpPoint (vertexSrc, vertexDst, W);

	int left=(int)1e7, right=(int)-1e7, top=(int)1e7, bottom=(int)-1e7;
	for (int i=0; i<vertexDst.Length(); i++)
	{
		left=MIN(left, vertexDst[i].x);
		right=MAX(right, vertexDst[i].x);
		top=MIN(top, vertexDst[i].y);
		bottom=MAX(bottom, vertexDst[i].y);
	}
	dst.Release();
	dst.Create (bottom-top, right-left, src.Type1());
	dst.Zero();

	Matf X(3,1), Z(3,1);

	int x, y;
    float az[2];

	Mat mInvert=W.Inverted();
 	int cn=dst.Channels();
   float aT[9]={
        mInvert.data.fl[0][0], mInvert.data.fl[0][1], mInvert.data.fl[0][2],
        mInvert.data.fl[1][0], mInvert.data.fl[1][1], mInvert.data.fl[1][2],
        mInvert.data.fl[2][0], mInvert.data.fl[2][1], mInvert.data.fl[2][2] };

    int nsrcW=src.Cols()-1;
    int nsrcH=src.Rows()-1;
	for(x=0;x<dst.Cols(); x++)
	{
        az[0]=aT[0]*x+aT[2];
        az[1]=aT[3]*x+aT[5];
		for(y=0;y<dst.Rows(); y++)
		{
            if (az[0] >= 0 && az[0] < nsrcW && az[1] >= 0 && az[1] < nsrcH)
                func (dst.data.ptr, x, y, src.data.ptr, az[0], az[1], cn);
        
            az[0]+=aT[1];
            az[1]+=aT[4];	
		}
	}
}
void WarpImage2 (const Mat& src, Mat& dst, const Mat& W)
{
	assert (src.Channels() == dst.Channels());
	BilinearInterpolationFunc Tab[] =
	{
		NULL, 
		BilinearInterpolation_<BYTE>, 
		BilinearInterpolation_<short>, 
		BilinearInterpolation_<int>, 
		BilinearInterpolation_<float>,
		BilinearInterpolation_<double> 
	};
	BilinearInterpolationFunc func=Tab[(int)src.Type()];
	dst.Zero();

	int x, y;
    float az[2];

	int cn=dst.Channels();
	Mat mInvert=W.Inverted();
    float aT[9]={
        mInvert.data.fl[0][0], mInvert.data.fl[0][1], mInvert.data.fl[0][2],
        mInvert.data.fl[1][0], mInvert.data.fl[1][1], mInvert.data.fl[1][2],
        mInvert.data.fl[2][0], mInvert.data.fl[2][1], mInvert.data.fl[2][2] };

    int nsrcW=src.Cols()-1;
    int nsrcH=src.Rows()-1;
	for(x=0;x<dst.Cols(); x++)
	{
        az[0]=aT[0]*x+aT[2];
        az[1]=aT[3]*x+aT[5];
		for(y=0;y<dst.Rows(); y++)
		{
            if (az[0] >= 0 && az[0] < nsrcW && az[1] >= 0 && az[1] < nsrcH)
                func (dst.data.ptr, x, y, src.data.ptr, az[0], az[1], cn);
        
            az[0]+=aT[1];
            az[1]+=aT[4];	
		}
	}
}
void WarpPoint (const Vector<Point2i>& src, Vector<Point2i>& dst, const Mat& W)
{
	Matf X(3,1);
	Matf Z(3,1);
	dst.RemoveAll();
	for(int i=0; i<src.Length(); i++)
	{
		X[0][0]=(float)src[i].x; X[1][0]=(float)src[i].y; X[2][0]=1.0f;
		MatOp::GEMM(&W, &X, 1, 0, 0, &Z);
		dst.Add (Point2i((int)Z[0][0], (int)Z[1][0]));
	}
}
void DrawWarpRect (Mat& image, const Rect& rect, const Mat& W)
{
	Point2i lt, lb, rt, rb;
	
	Matf X(3,1);
	Matf Z(3,1);

	// left-top point
	X[0][0]=(float)rect.x; X[1][0]=(float)rect.y; X[2][0]=1.0f;
	MatOp::GEMM(&W, &X, 1, 0, 0, &Z);
	lt.x=(int)Z[0][0], lt.y=(int)Z[1][0];

	// left-bottom point
	X[0][0]=(float)rect.x; X[1][0]=(float)rect.LimY(); X[2][0]=1.0f;
	MatOp::GEMM(&W, &X, 1, 0, 0, &Z);
	lb.x=(int)Z[0][0], lb.y=(int)Z[1][0];

	// right-top point
	X[0][0]=(float)rect.LimX(); X[1][0]=(float)rect.y; X[2][0]=1.0f;
	MatOp::GEMM(&W, &X, 1, 0, 0, &Z);
	rt.x=(int)Z[0][0], rt.y=(int)Z[1][0];

	// right-bottom point
	X[0][0]=(float)rect.LimX(); X[1][0]=(float)rect.LimY(); X[2][0]=1.0f;
	MatOp::GEMM(&W, &X, 1, 0, 0, &Z);
	rb.x=(int)Z[0][0], rb.y=(int)Z[1][0];

	// draw rectangle
	image.DrawLine (lt, rt, RGB(255,255,255));
	image.DrawLine (rt, rb, RGB(255,255,255));
	image.DrawLine (rb, lb, RGB(255,255,255));
	image.DrawLine (lb, lt, RGB(255,255,255));
}


}}
