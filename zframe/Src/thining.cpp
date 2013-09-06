#include "thining.h"
#include "FindContour.h"

namespace CVLib
{

/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * @param  im    Binary image with range = 0-1
 * @param  iter  0=even, 1=odd
 */
void thinningIteration(Mat& im, int iter)
{
    Mat marker(im.SizeObject(), MAT_Tbyte);
	marker.Zero();

	int i;
	uchar** ppbD=im.data.ptr;
    for (i = 1; i < im.Rows()-1; i++)
    {
        for (int j = 1; j < im.Cols()-1; j++)
        {
            uchar p2 = ppbD[i-1][j];
            uchar p3 = ppbD[i-1][j+1];
            uchar p4 = ppbD[i][j+1];
            uchar p5 = ppbD[i+1][j+1];
            uchar p6 = ppbD[i+1][j];
            uchar p7 = ppbD[i+1][j-1];
            uchar p8 = ppbD[i][j-1];
            uchar p9 = ppbD[i-1][j-1];

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + 
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) + 
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				marker.data.ptr[i][j] = 1;
        }
    }
	for (i=0; i<im.Rows()*im.Cols(); i++)
	{
		if (im.data.ptr[0][i] == 0)
			continue;
		im.data.ptr[0][i] &= (~marker.data.ptr[0][i]);
	}
//    im &= ~marker;
}

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void thinning(Mat& im)
{
	int i;
	int datalen=im.Rows()*im.Cols();
	for (i=0; i<datalen; i++)
	{
		if (im.data.ptr[0][i]==0)
			im.data.ptr[0][i]=1;
		else
			im.data.ptr[0][i]=0;
	}

	Mat prev(im.SizeObject(), MAT_Tbyte);
	prev.Zero();

    Mat diff;

	bool fcontinue=false;
    do {
        thinningIteration(im, 0);
        thinningIteration(im, 1);
		fcontinue=false;
		for (i=0; i<datalen; i++)
		{
			if (im.data.ptr[0][i]!=prev.data.ptr[0][i])
			{
				fcontinue=true;
				break;
			}
		}
        im=prev;
    } 
    while (fcontinue);

	for (i=0; i<datalen; i++)
	{
		if (im.data.ptr[0][i]==0)
			im.data.ptr[0][i]=255;
		else
			im.data.ptr[0][i]=0;
	}
}

void findRects(const Mat& mask, Vector<Rect>& rects)
{
	int i;
	Mat*       pIB = NULL;
	Matrix<Point2i> contours;

	pIB = new Mat(mask);
	for (i=0; i<mask.Rows()*mask.Cols(); i++)
		pIB->data.ptr[0][i] = mask.data.ptr[0][i]==0?255:0;
	FindContours(pIB, contours, CV_RETR_EXTERNAL);
	delete pIB;

	for (i=0; i<contours.GetSize(); i++)
	{
		Vector<Point2i>& contour=contours[i];
		Rect      rect_res;
		int x0=mask.Cols(),x1=-1,y0=mask.Rows(),y1=-1;
		for(int k=0;k<contour.GetSize();++k)
		{
			Point2i& pt=contour[k];
			x0 = MIN(x0,pt.x);
			y0 = MIN(y0,pt.y);
			x1 = MAX(x1,pt.x);
			y1 = MAX(y1,pt.y);
		}
		rect_res=Rect(x0,y0,x1-x0,y1-y0);
		rects.Add(rect_res);
	}
}

}