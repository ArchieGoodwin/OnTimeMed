/*! 
 * \file    Contours.h
 * \ingroup ip
 * \brief   cvlib
 * \author  
 */
#include "Contours.h"
#include "cvlibbase/inc/_cvlibbase.h"

namespace CVLib { namespace ip {
	
static int sklansky( Point2i* array, int start, int end, int* stack, int nsign, int sign2 )
{
	int incr = end > start ? 1 : -1;
	/* prepare first triangle */
	int pprev = start, pcur = pprev + incr, pnext = pcur + incr;
	int stacksize = 3;
	
	if( start == end ||
		(array[start].x == array[end].x &&
		array[start].y == array[end].y) )
	{
		stack[0] = start;
		return 1;
	}
	
	stack[0] = pprev;
	stack[1] = pcur;
	stack[2] = pnext;
	
	end += incr; /* make end = afterend */
	
	while( pnext != end )
	{
		/* check the angle p1,p2,p3 */
		int cury = array[pcur].y;
		int nexty = array[pnext].y;
		int by = nexty - cury;
		
		if( CVLIB_SIGN(by) != nsign )
		{
			int ax = array[pcur].x - array[pprev].x;
			int bx = array[pnext].x - array[pcur].x;
			int ay = cury - array[pprev].y;
			int convexity = ay*bx - ax*by;/* if >0 then convex angle */
			
			if( CVLIB_SIGN(convexity) == sign2 && (ax != 0 || ay != 0) )
			{
				pprev = pcur;
				pcur = pnext;
				pnext += incr;
				stack[stacksize] = pnext;
				stacksize++;
			}
			else
			{
				if( pprev == start )
				{
					pcur = pnext;
					stack[1] = pcur;
					pnext += incr;
					stack[2] = pnext;
				}
				else
				{
					stack[stacksize-2] = pnext;
					pcur = pprev;
					pprev = stack[stacksize-4];
					stacksize--;
				}
			}
		}
		else
		{
			pnext += incr;
			stack[stacksize-1] = pnext;
		}
	}
	
	return --stacksize;
}

static int CompareX(const void *p1,const void *p2)
{
	Point2i *x = (Point2i*)p1;
	Point2i *y = (Point2i*)p2;
	
	if(x->x > y->x)
		return 1; //sort in assending order
	if(x->x < y->x)
		return -1;
	return  y->y-x->x;
}

void ConvexHull (Vector<Point2i>& src, Vector<Point2i>& dst, bool fClockwise)
{
	qsort (src.GetData(), src.GetSize(), sizeof(Point2i), CompareX);

	int nTotal=src.GetSize();
	int miny_ind=0, maxy_ind=0;
	int i;
    // sort the point set by x-coordinate, find min and max y
	for( i = 0; i < nTotal; i++ )
	{
		int y = src[i].y;
		if( src[miny_ind].y > y )
			miny_ind = i;
		if( src[maxy_ind].y < y )
			maxy_ind = i;
	}
	if (src[0].x==src[nTotal-1].x && src[0].y==src[nTotal-1].y)
	{
		dst.Add(src[0]);
		return;
	}

    int* t_stack;
    int t_count;
	int stop_idx;
	int* stack = new int[nTotal+2];
	//upper half
	{
		int *tl_stack = stack;
        int tl_count = sklansky( src.GetData(), 0, maxy_ind, tl_stack, -1, 1 );
        int *tr_stack = tl_stack + tl_count;
        int tr_count = sklansky( src.GetData(), nTotal - 1, maxy_ind, tr_stack, -1, -1 );
		
        /* gather upper part of convex hull to output */
        if( !fClockwise )
        {
            SWAP( tl_stack, tr_stack, t_stack );
            SWAP( tl_count, tr_count, t_count );
        }
		
        for( i = 0; i < tl_count - 1; i++ )
            dst.Add(src[tl_stack[i]]);
		
        for( i = tr_count - 1; i > 0; i-- )
            dst.Add(src[tr_stack[i]]);
        stop_idx = tr_count > 2 ? tr_stack[1] : tl_count > 2 ? tl_stack[tl_count - 2] : -1;
 
	}

	//lower half
	{
        int *bl_stack = stack;
        int bl_count = sklansky( src.GetData(), 0, miny_ind, bl_stack, 1, -1 );
        int *br_stack = stack + bl_count;
        int br_count = sklansky( src.GetData(), nTotal - 1, miny_ind, br_stack, 1, 1 );
		
        if( fClockwise )
        {
            SWAP( bl_stack, br_stack, t_stack );
            SWAP( bl_count, br_count, t_count );
        }
		
        if( stop_idx >= 0 )
        {
            int check_idx = bl_count > 2 ? bl_stack[1] :
			bl_count + br_count > 2 ? br_stack[2-bl_count] : -1;
            if( check_idx == stop_idx || (check_idx >= 0 &&
                src[check_idx].x == src[stop_idx].x &&
                src[check_idx].y == src[stop_idx].y) )
            {
			/* if all the points lie on the same line, then
			the bottom part of the convex hull is the mirrored top part
				(except the exteme points).*/
                bl_count = MIN( bl_count, 2 );
                br_count = MIN( br_count, 2 );
            }
        }
		
        for( i = 0; i < bl_count - 1; i++ )
            dst.Add( src[bl_stack[i]] );
		
        for( i = br_count - 1; i > 0; i-- )
            dst.Add( src[br_stack[i]]);
	}
	delete []stack;
}

static void SWAP_func(int &nA,int &nB)
{
	int nTmpswap;
	nTmpswap = nA; nA = nB; nB = nTmpswap;
}

static void iBresenham (int nX0, int nY0, int nX1, int nY1, int* pnX, int* pnY, int &nNum)
{
	int i;
	int steep = 1;
	int sx, sy;  /* step positive or negative (1 or -1) */
	int dx, dy;  /* delta (difference in X and Y between points) */
	int e;
	nNum = 0;
	/*
	* optimize for vertical and horizontal lines here
	*/       
	dx = abs(nX1 - nX0);
	sx = ((nX1 - nX0) > 0) ? 1 : -1;
	dy = abs(nY1 - nY0);
	sy = ((nY1 - nY0) > 0) ? 1 : -1;
	if (dy > dx)
	{
		steep = 0;
		SWAP_func(nX0, nY0);
		SWAP_func(dx, dy);
		SWAP_func(sx, sy);
	}
	e = (dy << 1) - dx;
	for (i = 0; i < dx; i++) 
	{
		if (steep) 
		{
			pnX[nNum] = nX0; pnY[nNum] = nY0;
			nNum++;
		}
		else
		{
			pnX[nNum] = nY0; pnY[nNum] = nX0;
			nNum++;
		}
		while (e >= 0)
		{
			nY0 += sy;
			e -= (dx << 1);
		}
		nX0 += sx;
		e += (dy << 1);
	}
}

void Bresenham (int nX0, int nY0, int nX1, int nY1, int* &pnX, int* &pnY, int &nNum)
{
	int alloc_num = (int)(sqrt((double)((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0))) + 3);
	pnX = new int[alloc_num];
	pnY = new int[alloc_num];
	iBresenham (nX0, nY0, nX1, nY1, pnX, pnY, nNum);
}

CVLIB_DECLSPEC void Bresenham (Point2i& first, Point2i& last, Vector<Point2i>& points)
{
	int* pnx = NULL;
	int* pny = NULL;
	int nnum=0;
	Bresenham (first.x, first.y, last.x, last.y, pnx, pny, nnum);
	points.SetSize(nnum);
	for (int i=0; i<nnum; i++)
	{
		points[i].x = pnx[i];
		points[i].y = pny[i];
	}
	delete []pnx;
	delete []pny;
}

bool IntersectPointTwoLines (const Point2f& p1, const Point2f& p2, const Point2f& p3, const Point2f& p4, Point2f& intersectPt)
{
	float ua, ub;
	ua=((p4.x-p3.x)*(p1.y-p3.y)-(p4.y-p3.y)*(p1.x-p3.x))/((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
	ub=((p2.x-p1.x)*(p1.y-p3.y)-(p2.y-p1.y)*(p1.x-p3.x))/((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
	if (ua<0.0f || ua>1.0f)
		return false;
	if (ub<0.0f || ub>1.0f)
		return false;
	intersectPt.x=p1.x+ua*(p2.x-p1.x);
	intersectPt.y=p1.y+ua*(p2.y-p1.y);
	return true;
}
Point2f CrossPointTwoLines (const Point2f& p1, const Point2f& p2, const Point2f& p3, const Point2f& p4)
{
	float ua, ub;
	ua=((p4.x-p3.x)*(p1.y-p3.y)-(p4.y-p3.y)*(p1.x-p3.x))/((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
	ub=((p2.x-p1.x)*(p1.y-p3.y)-(p2.y-p1.y)*(p1.x-p3.x))/((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
	Point2f pt;
	pt.x=p1.x+ua*(p2.x-p1.x);
	pt.y=p1.y+ua*(p2.y-p1.y);
	return pt;
}
Point2f CrossPointTwoLines (const Point2i& p1, const Point2i& p2, const Point2i& p3, const Point2i& p4)
{
	float ua, ub;
	ua=((p4.x-p3.x)*(p1.y-p3.y)-(p4.y-p3.y)*(p1.x-p3.x))/(float)((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
	ub=((p2.x-p1.x)*(p1.y-p3.y)-(p2.y-p1.y)*(p1.x-p3.x))/(float)((p4.y-p3.y)*(p2.x-p1.x)-(p4.x-p3.x)*(p2.y-p1.y));
	Point2f pt;
	pt.x=p1.x+ua*(p2.x-p1.x);
	pt.y=p1.y+ua*(p2.y-p1.y);
	return pt;
}


}}
