
/*!
 * \file
 * \brief .
 * \author Jinhyok Sin
 */

#include "FindContour.h"
#include "ipMedian.h"
#include "Canny.h"
#include "Sobel.h"
#include "Contours.h"
#include "ScaleXY.h"
#include "ipThreshold.h"
#include "ipMorphology.h"

namespace CVLib
{

/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CV_INIT_3X3_DELTAS( deltas, step, nch )            \
    ((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch),  \
	(deltas)[2] = -(step), (deltas)[3] = -(step) - (nch),  \
	(deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch),  \
(deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))

static const SPoint icvCodeDeltas[8] =
    { {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1} };

//////////////////////////////////////////////////////////////////////////
static int iMemCopy( double **buf1, double **buf2, double **buf3, int *b_max );

/*=========================FindNextContours relation functions=======================*/
void FindSquares4( CoImage* img, Matrix<Point2i>& squares, float t_angle, bool bMultiple, ImageDescPtrList* pImageList)
{
	Matrix<Point2i> temp_Squares ;
    Sequence* contoursSeq = 0;
    int i, c, l, N = 11;
    Sequence* result;
    double s, t;
	
	if( t_angle <= 0 || t_angle >= 1 )
		t_angle = 0.3f ;

	int nH = img->GetHeight() ;
	int nW = img->GetWidth() ;

	MemStorage* storage = CreateMemStorage( 0 ) ;    
    
	ipMedian median ;
	Canny canny ;
	ipDilate dilate ;
	ipErode erode ;
	ScaleXY scaler ;
	int nScale = 0 ;

	if( nH > 1000 || nW > 1000 )
		nScale = 5 ;
	else
		nScale = 1 ;

	nH /= nScale ;
	nW /= nScale ;

	Mat matSrc( nH, nW, MAT_TBYTE ) ;
	Mat matTemp( nH, nW, MAT_TBYTE ) ;
	Mat mSum( nH, nW, MAT_Tdouble ) ;
	Mat mStable( nH, nW, MAT_TBYTE ) ;

	double* prSum = mSum.data.db[0] ;
	BYTE* pbStable = mStable.data.ptr[0] ;
	BYTE* pbTemp = matTemp.data.ptr[0] ;

	memset( prSum, 0, nW * nH * sizeof(double)) ;
	memset( pbStable, 255, nW * nH * sizeof(BYTE)) ;

	scaler.FastProcess( &img->m_matX, &matSrc ) ;	
	canny.Create( nW, nH ) ;
	canny.m_nHighThreshold = 50 ;
	canny.m_nLowThreshold = 0 ;	
	    
    for( c = 0; c < 3; c++ )
    {       
		if( c == 0)
		{
 			median.Process( &matSrc );
		}
		else if( c == 1 )
		{
			scaler.FastProcess( &img->m_matY, &matSrc ) ;
 			median.Process( &matSrc ) ;
		}
		else
		{
			scaler.FastProcess( &img->m_matZ, &matSrc ) ;
 			median.Process( &matSrc, &matSrc ) ;
		}               
		if (pImageList)
		{
			char szDesc[512];
			sprintf (szDesc, "channel %d-median", c);
			pImageList->Add(new CoImage(matSrc, TRUE), szDesc);
		}
    
        for( l = 0; l < N; l++ )
        {            
            if( l == 0 )
            {                
				canny.Process( &matSrc, &matTemp ) ;
				if (pImageList)
				{
					char szDesc[512];
					sprintf (szDesc, "channel %d-canny", c);
					pImageList->Add(new CoImage(matTemp, TRUE), szDesc);
				}
				dilate.Process( &matTemp) ;
				if (pImageList)
				{
					char szDesc[512];
					sprintf (szDesc, "channel %d-dilate", c);
					pImageList->Add(new CoImage(matTemp, TRUE), szDesc);
				}
            }
            else
			{
                Threshold_Contour( &matSrc, &matTemp, (l+1)*255/N, 255 );
				if (pImageList)
				{
					char szDesc[512];
					sprintf (szDesc, "channel %d-threshold %d", c, l);
					pImageList->Add(new CoImage(matTemp, TRUE), szDesc);
				}
			}

			if( l <= N-1)
			{				
				for( i = 0 ; i < nH * nW ; i ++ )
					prSum[i] += (double)pbTemp[i] / (N-1) ;

				if( l != N-1)
					continue ;				
			}

			for ( i = 0 ; i < nH * nW ; i++ )
			{
				pbTemp[i] = (BYTE)prSum[i] ;
				prSum[i] = 0 ;
			}//end for loop

			canny.Process( &matTemp ) ;		

			for ( i = 0 ; i < nH * nW ; i ++ )
			{
				if( pbTemp[i] < 20 )
					pbStable[i] = 0 ;
			}//end for loop			
           
             FindContours( &matTemp, storage, &contoursSeq,  1, 2, Point2i(0,0) );

			 if( c == 2)				 
				 FindContours( &mStable, storage, &contoursSeq,  1, 2, Point2i(0,0) );
            
            while( contoursSeq )
            {                
                result = ApproxPoly( contoursSeq, sizeof(Contour), storage,
									 CV_POLY_APPROX_DP, cvContourPerimeter(contoursSeq)*0.023, 0 );
                
                if( result->total == 4 && fabs(ContourArea(result,CV_WHOLE_SEQ)) > 200 &&
                    CheckContourConvexity(result) )
                {
                    s = 0;
                    
                    for( i = 0; i < 5; i++ )
                    {                        
                        if( i >= 2 )
                        {
                            t = fabs(Angle(
                            (SPoint*)GetSeqElem( result, i ),
                            (SPoint*)GetSeqElem( result, i-2 ),
                            (SPoint*)GetSeqElem( result, i-1 )));
                            s = s > t ? s : t;
                        }
                    }
                    
                    if( s < t_angle )
					{
						Vector<Point2i> square;
                        for( i = 0; i < 4; i++ )
						{
							SPoint* pPt=(SPoint*)GetSeqElem( result, i );							
							square.Add(*pPt) ;
						}						
						temp_Squares.Add(square);
					}
                }                
                contoursSeq = contoursSeq->h_next;
            }
        }
    }    
	ReleaseMemStorage( &storage ) ;

	//Convert Point2i to Point2i
	Vector<Point2i> temp_s ;
	Vector<Point2i> temp_2i ;	
	Point2i sp_2i ;

	for ( i = 0 ; i < temp_Squares.GetSize() ; i ++ )
	{
		temp_s = temp_Squares[i] ;
		for( int j = 0 ; j < 4 ; j ++)
		{
			sp_2i.x = temp_s[j].x ;
			sp_2i.y = temp_s[j].y ;
			temp_2i.Add(sp_2i) ;
		}
		squares.Add( temp_2i ) ;
		temp_2i.RemoveAll() ;
	}//end for loop i

	if( !(img->m_matG).IsValid() )
		img->Gray() ;

	EndSquares( &img->m_matG, squares, nScale, bMultiple ) ;
}
/*************************************************************************************/
void FindSquaresFromBin( Mat* img, Matrix<Point2i>& squares )
{
	Matrix<Point2i> temp_Squares ;
    Sequence* contoursSeq = 0;
    int i;
    Sequence* result;
    double s, t;
	float t_angle=(float)CVLIB_PI*0.5f;

	MemStorage* storage = CreateMemStorage( 0 ) ;    
    
    {            
          FindContours( img, storage, &contoursSeq,  1, 2, Point2i(0,0) );            
        
        while( contoursSeq )
        {                
            result = ApproxPoly( contoursSeq, sizeof(Contour), storage,
								 CV_POLY_APPROX_DP, cvContourPerimeter(contoursSeq)*0.02, 0 );
            
            if( result->total == 4 && fabs(ContourArea(result,CV_WHOLE_SEQ)) > 10 &&
                CheckContourConvexity(result) )
            {
                s = 0;
                
                for( i = 0; i < 5; i++ )
                {                        
                    if( i >= 2 )
                    {
                        t = fabs(Angle(
                        (SPoint*)GetSeqElem( result, i ),
                        (SPoint*)GetSeqElem( result, i-2 ),
                        (SPoint*)GetSeqElem( result, i-1 )));
                        s = s > t ? s : t;
                    }
                }
                
                if( s < t_angle )
				{
					Vector<Point2i> square;
                    for( i = 0; i < 4; i++ )
					{
						SPoint* pPt=(SPoint*)GetSeqElem( result, i );							
						square.Add(*pPt) ;
					}						
					temp_Squares.Add(square);
				}
            }                
            contoursSeq = contoursSeq->h_next;
        }
    }        
	ReleaseMemStorage( &storage ) ;

	//Convert Point2i to Point2i
	Vector<Point2i> temp_s ;
	Vector<Point2i> temp_2i ;	
	Point2i sp_2i ;

	for ( i = 0 ; i < temp_Squares.GetSize() ; i ++ )
	{
		temp_s = temp_Squares[i] ;
		for( int j = 0 ; j < 4 ; j ++)
		{
			sp_2i.x = temp_s[j].x ;
			sp_2i.y = temp_s[j].y ;
			temp_2i.Add(sp_2i) ;
		}
		squares.Add( temp_2i ) ;
		temp_2i.RemoveAll() ;
	}//end for loop i
}
void FindSquares4( Mat* pmSrc, Matrix<Point2i>& squares, float t_angle, bool bMultiple)
{
	Matrix<Point2i> temp_Squares ;
    Sequence* contoursSeq = 0;
    int i, l, N = 11;
    Sequence* result;
    double s, t;

	if( t_angle < 0 || t_angle >= 1 )
		t_angle = 0.3f ;

	int nH = pmSrc->Rows() ;
	int nW = pmSrc->Cols() ;

	MemStorage* storage = CreateMemStorage( 0 ) ;    
    
	ipMedian median ;
	Canny canny ;
	ipDilate dilate ;
	ScaleXY scale ;
	int nScale = 0 ;

	if( nH > 1000 || nW > 1000 )
		nScale = 5 ;
	else
		nScale = 2 ;

	nH /= nScale ;
	nW /= nScale ;

	Mat matSrc( nH, nW, MAT_TBYTE ) ;
	Mat matTemp( nH, nW, MAT_TBYTE ) ;	

	scale.FastProcess( pmSrc, &matSrc ) ;
	canny.Create( nW, nH ) ;
	canny.m_nHighThreshold = 50 ;
	canny.m_nLowThreshold = 0 ;
 	median.Process( &matSrc ) ;
   
    for( l = 0; l < N; l++ )
    {            
        if( l == 0 )
        {                
			canny.Process( &matSrc, &matTemp ) ;
			dilate.Process( &matTemp) ;
        }
        else            
            Threshold_Contour( &matSrc, &matTemp, (l+1)*255/N, 255 );
		
         FindContours( &matTemp, storage, &contoursSeq,  1, 2, Point2i(0,0) );            
        
        while( contoursSeq )
        {                
            result = ApproxPoly( contoursSeq, sizeof(Contour), storage,
								 CV_POLY_APPROX_DP, cvContourPerimeter(contoursSeq)*0.02, 0 );
            
            if( result->total == 4 && fabs(ContourArea(result,CV_WHOLE_SEQ)) > 200 &&
                CheckContourConvexity(result) )
            {
                s = 0;
                
                for( i = 0; i < 5; i++ )
                {                        
                    if( i >= 2 )
                    {
                        t = fabs(Angle(
                        (SPoint*)GetSeqElem( result, i ),
                        (SPoint*)GetSeqElem( result, i-2 ),
                        (SPoint*)GetSeqElem( result, i-1 )));
                        s = s > t ? s : t;
                    }
                }
                
                if( s < t_angle )
				{
					Vector<Point2i> square;
                    for( i = 0; i < 4; i++ )
					{
						SPoint* pPt=(SPoint*)GetSeqElem( result, i );							
						square.Add(*pPt) ;
					}						
					temp_Squares.Add(square);
				}
            }                
            contoursSeq = contoursSeq->h_next;
        }
    }        
	ReleaseMemStorage( &storage ) ;

	//Convert Point2i to Point2i
	Vector<Point2i> temp_s ;
	Vector<Point2i> temp_2i ;	
	Point2i sp_2i ;

	for ( i = 0 ; i < temp_Squares.GetSize() ; i ++ )
	{
		temp_s = temp_Squares[i] ;
		for( int j = 0 ; j < 4 ; j ++)
		{
			sp_2i.x = temp_s[j].x ;
			sp_2i.y = temp_s[j].y ;
			temp_2i.Add(sp_2i) ;
		}
		squares.Add( temp_2i ) ;
		temp_2i.RemoveAll() ;
	}//end for loop i

	EndSquares( pmSrc, squares, nScale, bMultiple ) ;
}
/*************************************************************************************/
void EndSquares( Mat* graySrc, Matrix<Point2i>& squares, int nScale, bool bMulti )
{
	int i, j ;
	ScaleXY scale ;
	Sobel sobel ;
	Matrix<Point2i> temp_squares  = squares ;
	squares.RemoveAll() ;

	int nH = graySrc->Rows() / nScale ;
	int nW = graySrc->Cols() / nScale ;

	Mat matSrc( nH, nW, MAT_TBYTE ) ;	
	scale.FastProcess( graySrc, &matSrc ) ;
 	
	float rMax = 0;
	float rMean = 0 ;
	int nMaxid = -1;
	int nSqCount = temp_squares.GetSize() ;

	if( nSqCount < 2 )
		return ;

	float rTheshold = 0 ;
	Vector<float> rvalue ;
	float buf ;
	Matrix<Point2i> tempSq = temp_squares ;
	int nDelta = (int)(MAX( nW, nH) * 0.025f);
	float rDistance = (float)(nDelta * sqrtf((float)nDelta));
	Vector<Point2i> pt, tempPt ;
	Point2i src[4], comp[4] ;

	for( i = 0 ; i < nSqCount ; i ++ )
	{
		tempPt = tempSq[i] ;
		buf = GetEdgeValue( &matSrc, tempPt);
		rMean += buf ;
		rvalue.Add( buf ) ;
		if (rvalue[i] > rMax)
		{
			rMax = rvalue[i];
			nMaxid = i;
		}
	}//end for loop i
	rMean /= nSqCount ;
	rTheshold = rMean + ( rMax - rMean ) * 0.1f ;

//Single Mode Process
	int nSqH, nSqW ;
	int nPerArea = 0 ;

	if( !bMulti )
	{
		squares.RemoveAll() ;

		int nMaxArea = -1 ;
		int nAreaIdx = -1 ;
		for( i = 0 ; i < tempSq.GetSize() ; i ++ )
		{
			pt = tempSq[i] ;
			nSqH = (int)pt[0].DistTo( pt[1] ) ;
			nSqW = (int)pt[1].DistTo( pt[2]) ;
			nPerArea = (nSqH * nSqW * 100) / ( nH * nW ) ;

			if( 2 <= nPerArea && nPerArea < 85 )
			{
				if( nMaxArea < nPerArea )
				{
					nMaxArea = nPerArea ;
					nAreaIdx = i ;
					continue ;
				}				
			}
		}

		if( nAreaIdx != -1 )
		{
			pt = tempSq[nAreaIdx] ;
			for( j = 0 ; j < 4 ; j ++ ) 
				pt[j] *= (float)nScale ;
			squares.Add(pt) ;
					return ;
		}
		return ;
	}

	if(  rMax == 0 )
	{
		temp_squares.Add(tempSq[0]);
		return ;
	}

	int nSize = tempSq.GetSize() ;
	temp_squares.RemoveAll() ;

	while ( nSize > 1 )
	{
		for ( i = 0 ; i < nSize ; i ++ )
		{
			if( rvalue[i] > rMax )
			{
				rMax = rvalue[i] ;
				nMaxid = i ;
			}
		}//end for loop i

//  		if( rMax < rTheshold )
// 			break ;

		if( nMaxid != -1 )
 		{
 			temp_squares.Add(tempSq[nMaxid]) ;
			pt = tempSq.GetAt(nMaxid) ;
			tempSq.RemoveAt( nMaxid ) ;
			rvalue.RemoveAt( nMaxid ) ;
			nSize -= 1 ;
			nMaxid = -1 ;
			rMax = 0 ;
		}
		else
			break ;

		//Range Filter
		src[0] = pt[0], src[1] = pt[1], src[2] = pt[2], src[3] = pt[3] ;
		double dis_src ;
		double dis_buf = 1000 ;
		int nIdx = -1 ;		
		
		for ( i = 0 ; i < nSize ; i ++ )
		{
			tempPt = tempSq.GetAt(i) ;
			comp[0] = tempPt[0], comp[1] = tempPt[1], comp[2] = tempPt[2], comp[3] = tempPt[3] ;

			//find start point
			for ( j = 0 ; j < 4 ; j ++ )
			{
				dis_src = src[0].DistTo(comp[j]) ;
				if( dis_src < dis_buf )
				{
					dis_buf = dis_src ;
					nIdx = j ;
				}
			}//end for loop j
			
			int nState = 0 ;

			if( dis_buf == 1000 || nIdx == -1 ) continue ;
			else if( dis_buf <= rDistance )
			{
				for ( j = 1 ; j < 4 ; j ++ )
				{
					dis_buf = src[j].DistTo( comp[(j + nIdx) % 4]) ;

					if( dis_buf > rDistance )
					{
						nState = 1 ;
						break ;
					}
				}//end for loop j			
			}
			
			if( nState == 0 && dis_buf < rDistance )
			{
				tempSq.RemoveAt(i) ;
				rvalue.RemoveAt(i) ;
				nSize -= 1 ;
			}
		}//end for loop i		
	}//end while loop
	
	Point2i pBuf[4 ] ;
	Vector<Point2i> buf_2i ;

	for ( i = 0 ; i < temp_squares.GetSize() ; i ++ )
	{
		for ( int j = 0 ; j < 4 ; j ++ )
		{
			src[j] = temp_squares[i][j] ;
			pBuf[j].x = src[j].x * nScale ;
			pBuf[j].y = src[j].y * nScale ;
			buf_2i.Add(pBuf[j]) ;
		}
		squares.Add( buf_2i ) ;
		buf_2i.RemoveAll() ;
	}//end for loop i
}

/*************************************************************************************/
int FindContours( Mat* img, MemStorage* storage, Sequence** firstContour, int mode, int method, Point2i offset )
{
	ContourScanner scanner = 0;
    Sequence *contour = 0;
    int count = -1;	
    
    if( !firstContour )
        return -27 ;
	
    if( method == CV_LINK_RUNS )
    {
        if( offset.x != 0 || offset.y != 0 )
            return -211 ;
		
//        count = FindContoursInInterval( img, storage, firstContour, cntHeaderSize );
    }
    else
    {
        scanner = StartFindContours( img, storage, mode, method, offset ) ;
        assert( scanner );		
		
        do
        {
            count++;
            contour = FindNextContour( scanner );
        }
        while( contour != 0 );
		
        *firstContour = EndFindContours( &scanner );
    }    
	
    return count;
}
/*************************************************************************************/
ContourScanner StartFindContours(Mat* pmImg, MemStorage* storage, int nRetrievalMode, int nApproximageMethod, Point2i startPt)
{
	int y;
	ContourScanner scanner = 0;
	scanner = (ContourScanner)Alloc( sizeof( *scanner ));

	memset( scanner, 0, sizeof(*scanner)) ;
	int header_size = sizeof(Contour) ;
	int nH = pmImg->Rows() ;
	int nW = pmImg->Cols() ;
	
	scanner->img_size.height = nH - 1 ;
	scanner->img_size.width = nW - 1 ;
	scanner->storage1 = scanner->storage2 = storage ;
    scanner->img0 = (char *) pmImg->data.ptr[0];
    scanner->img = (char *) (pmImg->data.ptr[0] + nW);
	scanner->img_step = nW ;
    scanner->mode = nRetrievalMode ;
    scanner->offset = startPt;
    scanner->pt.x = scanner->pt.y = 1;
    scanner->lnbd.x = 0;
    scanner->lnbd.y = 1;
    scanner->nbd = 2;
	scanner->mode = nRetrievalMode;
	scanner->frame_info.contour = &(scanner->frame) ;
    scanner->frame_info.is_hole = 1;
	scanner->frame_info.next = 0 ;
	scanner->frame_info.parent = 0 ;
	scanner->frame_info.rect = Rect( 0, 0, nW, nH );
	scanner->l_cinfo = 0;
    scanner->subst_flag = 0;
	
    scanner->frame.flags = CV_SEQ_FLAG_HOLE;
    scanner->approx_method2 = scanner->approx_method1 = nApproximageMethod;
	
	if( nApproximageMethod == CV_CHAIN_APPROX_TC89_L1 || nApproximageMethod == CV_CHAIN_APPROX_TC89_KCOS )
        scanner->approx_method1 = CV_CHAIN_CODE;
	
    if( scanner->approx_method1 == CV_CHAIN_CODE )
    {
        scanner->seq_type1 = CV_SEQ_CHAIN_CONTOUR;
        scanner->header_size1 = scanner->approx_method1 == scanner->approx_method2 ?
		header_size : sizeof( Chain );
        scanner->elem_size1 = sizeof( char );
    }
    else
    {
        scanner->seq_type1 = CV_SEQ_POLYGON;
        scanner->header_size1 = scanner->approx_method1 == scanner->approx_method2 ?
								header_size : sizeof( Contour );
        scanner->elem_size1 = sizeof( Point2i );
    }
	
    scanner->header_size2 = header_size;
	
    if( scanner->approx_method2 == CV_CHAIN_CODE )
    {
        scanner->seq_type2 = scanner->seq_type1;
        scanner->elem_size2 = scanner->elem_size1;
    }
    else
    {
        scanner->seq_type2 = CV_SEQ_POLYGON;
        scanner->elem_size2 = sizeof( Point2i );
    }
	
    scanner->seq_type1 = scanner->approx_method1 == CV_CHAIN_CODE ?
	CV_SEQ_CHAIN_CONTOUR : CV_SEQ_POLYGON;
	
	scanner->seq_type2 = scanner->approx_method2 == CV_CHAIN_CODE ?
	CV_SEQ_CHAIN_CONTOUR : CV_SEQ_POLYGON;
	
    SaveMemStoragePos( storage, &(scanner->initial_pos) );
	
    if( nApproximageMethod > CV_CHAIN_APPROX_SIMPLE )
    {
        scanner->storage1 = CreateChildMemStorage( scanner->storage2 );
    }
	
    if( nRetrievalMode > CV_RETR_LIST )
    {
        scanner->cinfo_storage = CreateChildMemStorage( scanner->storage2 );
        scanner->cinfo_set = CreateSet( 0, sizeof( Set ), sizeof( ContourInfo ),
									    scanner->cinfo_storage );
        if( scanner->cinfo_storage == 0 || scanner->cinfo_set == 0 )
            return FALSE;
    }	
	
    /* make zero borders */
	unsigned char** ppImg = pmImg->data.ptr;
	
    memset( ppImg[0], 0, nW ) ;
    memset( ppImg[nH - 1], 0, nW ) ;
	
    for( y = 1; y < nH - 1; y++ )    
        ppImg[y][0] = ppImg[y][nW-1] = 0;
	
    /* converts all pixels to 0 or 1 */
	ip::Threshold( *pmImg, *pmImg, 0, 1, THRESH_BINARY );
	return scanner ;
}

/*************************************************************************************/
Sequence* FindNextContour( ContourScanner scanner )
{
	char *img0;
    char *img;
    int step;
    int width, height;
    int x, y;
    int prev;
    Point2i lnbd;
    Sequence *contour = 0;
    int nbd;
    int mode;
    int result = 1 ;

	if( !scanner )
		return NULL ;
    
	EndProcessContour( scanner );

    /* initialize local state */
	
    img0 = scanner->img0;
    img = scanner->img;
    step = scanner->img_step;
    x = scanner->pt.x;
    y = scanner->pt.y;
    width = scanner->img_size.width;
    height = scanner->img_size.height;
    mode = scanner->mode;
    lnbd = scanner->lnbd;
    nbd = scanner->nbd;

    prev = img[x - 1];

    for( ; y < height; y++, img += step )
    {
        for( ; x < width; x++ )
        {
            int p = img[x];

            if( p != prev )
            {
                ContourInfo *par_info = 0;
                ContourInfo *l_cinfo = 0;
                Sequence *seq = 0;
                int is_hole = 0;
                Point2i origin;

                if( !(prev == 0 && p == 1) )    /* if not external contour */
                {
                    /* check hole */
                    if( p != 0 || prev < 1 )
                        goto resume_scan;

                    if( prev & -2 )
                    {
                        lnbd.x = x - 1;
                    }
                    is_hole = 1;
                }

                if( mode == 0 && (is_hole || img0[lnbd.y * step + lnbd.x] > 0) )
                    goto resume_scan;

                origin.y = y;
                origin.x = x - is_hole;

                /* find contour parent */
                if( mode <= 1 || (!is_hole && mode == 2) || lnbd.x <= 0 )
                {
                    par_info = &(scanner->frame_info);
                }
                else
                {
                    int lval = img0[lnbd.y * step + lnbd.x] & 0x7f;
                    ContourInfo *cur = scanner->cinfo_table[lval - 2];

                    assert( lval >= 2 );

                    /* find the first bounding contour */
                    while( cur )
                    {
                        if( (unsigned) (lnbd.x - cur->rect.x) < (unsigned) cur->rect.width &&
                            (unsigned) (lnbd.y - cur->rect.y) < (unsigned) cur->rect.height )
                        {
                            if( par_info )
                            {
                                if( TraceContour( scanner->img0 +
                                                 par_info->origin.y * step +
                                                 par_info->origin.x, step, img + lnbd.x,
                                                 par_info->is_hole ) > 0 )
                                    break;
                            }
                            par_info = cur;
                        }
                        cur = cur->next;
                    }

                    assert( par_info != 0 );

                    /* if current contour is a hole and previous contour is a hole or
                       current contour is external and previous contour is external then
                       the parent of the contour is the parent of the previous contour else
                       the parent is the previous contour itself. */
                    if( par_info->is_hole == is_hole )
                    {
                        par_info = par_info->parent;
                        /* every contour must have a parent
                           (at least, the frame of the image) */
                        if( !par_info )
                            par_info = &(scanner->frame_info);
                    }

                    /* hole flag of the parent must differ from the flag of the contour */
                    assert( par_info->is_hole != is_hole );
                    if( par_info->contour == 0 )        /* removed contour */
                        goto resume_scan;
                }

                lnbd.x = x - is_hole;

                SaveMemStoragePos( scanner->storage2, &(scanner->backup_pos) );

                seq = CreateSeq( scanner->seq_type1, scanner->header_size1,
                                   scanner->elem_size1, scanner->storage1 );
                if( !seq )
                {
                    result = -3 ;
                    goto exit_func;
                }
                seq->flags |= is_hole ? CV_SEQ_FLAG_HOLE : 0;

                /* initialize header */
                if( mode <= 1 )
                {
                    l_cinfo = &(scanner->cinfo_temp);
                    result = FetchContour( img + x - is_hole, step,
                                          Point2i( origin.x + scanner->offset.x,
                                                   origin.y + scanner->offset.y),
                                          seq, scanner->approx_method1 );
                    if( result < 0 )
                        goto exit_func;
                }
                else
                {
                    union { ContourInfo* ci; SetElem* se; } v;
                    v.ci = l_cinfo;
                    SetAdd( scanner->cinfo_set, 0, &v.se );
                    l_cinfo = v.ci;

                    result = FetchContourEx( img + x - is_hole, step,
                                            Point2i( origin.x + scanner->offset.x,
                                                     origin.y + scanner->offset.y),
                                            seq, scanner->approx_method1,
                                            nbd, &(l_cinfo->rect) );
                    if( result < 0 )
                        goto exit_func;
                    l_cinfo->rect.x -= scanner->offset.x;
                    l_cinfo->rect.y -= scanner->offset.y;

                    l_cinfo->next = scanner->cinfo_table[nbd - 2];
                    scanner->cinfo_table[nbd - 2] = l_cinfo;

                    /* change nbd */
                    nbd = (nbd + 1) & 127;
                    nbd += nbd == 0 ? 3 : 0;
                }

                l_cinfo->is_hole = is_hole;
                l_cinfo->contour = seq;
                l_cinfo->origin = origin;
                l_cinfo->parent = par_info;

                if( scanner->approx_method1 != scanner->approx_method2 )
                {
                    result = ApproximateChainTC89( (Chain *) seq,
                                                  scanner->header_size2,
                                                  scanner->storage2,
                                                  &(l_cinfo->contour),
                                                  scanner->approx_method2 );
                    if( result < 0 )
                        goto exit_func;
                    ClearMemStorage( scanner->storage1 );
                }

                l_cinfo->contour->v_prev = l_cinfo->parent->contour;

                if( par_info->contour == 0 )
                {
                    l_cinfo->contour = 0;
                    if( scanner->storage1 == scanner->storage2 )
                    {
                        RestoreMemStoragePos( scanner->storage1, &(scanner->backup_pos) );
                    }
                    else
                    {
                        ClearMemStorage( scanner->storage1 );
                    }
                    p = img[x];
                    goto resume_scan;
                }

                SaveMemStoragePos( scanner->storage2, &(scanner->backup_pos2) );
                scanner->l_cinfo = l_cinfo;
                scanner->pt.x = x + 1;
                scanner->pt.y = y;
                scanner->lnbd = lnbd;
                scanner->img = (char *) img;
                scanner->nbd = nbd;
                contour = l_cinfo->contour;

                result = 0;
                goto exit_func;
              resume_scan:
                prev = p;
                /* update lnbd */
                if( prev & -2 )
                {
                    lnbd.x = x;
                }
            }                   /* end of prev != p */
        }                       /* end of loop on x */

        lnbd.x = 0;
        lnbd.y = y + 1;
        x = 1;
        prev = 0;

    }                           /* end of loop on y */

	exit_func:

    if( result != 0 )
        contour = 0;
    if( result < 0 )
        return FALSE ;    
    return contour;
}

/*************************************************************************************/
int TraceContour( char *ptr, int step, char *stop_ptr, int is_hole )
{
    int deltas[16];
    char *i0 = ptr, *i1, *i3, *i4;
    int s, s_end;
	
    /* initialize local state */
    CV_INIT_3X3_DELTAS( deltas, step, 1 );
    memcpy( deltas + 8, deltas, 8 * sizeof( deltas[0] ));
	
    assert( (*i0 & -2) != 0 );
	
    s_end = s = is_hole ? 0 : 4;
	
    do
    {
        s = (s - 1) & 7;
        i1 = i0 + deltas[s];
        if( *i1 != 0 )
            break;
    }
    while( s != s_end );
	
    i3 = i0;
	
    /* check single pixel domain */
    if( s != s_end )
    {
        /* follow border */
        for( ;; )
        {
            s_end = s;
			
            for( ;; )
            {
                i4 = i3 + deltas[++s];
                if( *i4 != 0 )
                    break;
            }
			
            if( i3 == stop_ptr || (i4 == i0 && i3 == i1) )
                break;
			
            i3 = i4;
            s = (s + 4) & 7;
        }                       /* end of border following loop */
    }
    return i3 == stop_ptr;
}

/*************************************************************************************/
void EndProcessContour( ContourScanner scanner )
{
    ContourInfo *l_cinfo = scanner->l_cinfo;
	
    if( l_cinfo )
    {
        if( scanner->subst_flag )
        {
            MemStoragePos temp;
			
            SaveMemStoragePos( scanner->storage2, &temp );
			
            if( temp.top == scanner->backup_pos2.top &&
                temp.free_space == scanner->backup_pos2.free_space )
            {
                RestoreMemStoragePos( scanner->storage2, &scanner->backup_pos );
            }
            scanner->subst_flag = 0;
        }
		
        if( l_cinfo->contour )
        {
            InsertNodeIntoTree( l_cinfo->contour, l_cinfo->parent->contour,
				&(scanner->frame) );
        }
        scanner->l_cinfo = 0;
    }
}
/*************************************************************************************/
int SaveMemStoragePos( const MemStorage * storage, MemStoragePos * pos )
{	
    if( !storage || !pos )
        return -27;
	
    pos->top = storage->top;
    pos->free_space = storage->free_space;
	return 1 ;
}
/*************************************************************************************/
int RestoreMemStoragePos( MemStorage * storage, MemStoragePos * pos )
{	
    if( !storage || !pos )
        return -27;
    if( pos->free_space > storage->block_size )
        return -201;		
	
    storage->top = pos->top;
    storage->free_space = pos->free_space;
	
    if( !storage->top )
    {
        storage->top = storage->bottom;
        storage->free_space = storage->top ? storage->block_size - sizeof(MemBlock) : 0;
    }
    return 1 ;
}
/*************************************************************************************/
int ClearMemStorage( MemStorage * storage )
{	
    if( !storage )
        return -27;
	
    if( storage->parent )
    {
        DestroyMemStorage( storage );
    }
    else
    {
        storage->top = storage->bottom;
        storage->free_space = storage->bottom ? storage->block_size - sizeof(MemBlock) : 0;
    }
	return 1 ;
}

/*************************************************************************************/
int DestroyMemStorage( MemStorage* storage )
{	
    int k = 0;
	
    MemBlock *block;
    MemBlock *dst_top = 0;
	
    if( !storage )
        return -27 ;
	
    if( storage->parent )
        dst_top = storage->parent->top;
	
    for( block = storage->bottom; block != 0; k++ )
    {
        MemBlock *temp = block;
		
        block = block->next;
        if( storage->parent )
        {
            if( dst_top )
            {
                temp->prev = dst_top;
                temp->next = dst_top->next;
                if( temp->next )
                    temp->next->prev = temp;
                dst_top = dst_top->next = temp;
            }
            else
            {
                dst_top = storage->parent->bottom = storage->parent->top = temp;
                temp->prev = temp->next = 0;
                storage->free_space = storage->block_size - sizeof( *temp );
            }
        }
        else
        {
            cvFree( &temp );
        }
    }
	
    storage->top = storage->bottom = 0;
    storage->free_space = 0;
	
    return 1;
}

/*************************************************************************************/
int InsertNodeIntoTree( void* _node, void* _parent, void* _frame )
{	
    TreeNode* node = (TreeNode*)_node;
    TreeNode* parent = (TreeNode*)_parent;
	
    if( !node || !parent )
        return -27 ;
	
    node->v_prev = _parent != _frame ? parent : 0;
    node->h_next = parent->v_next;
	
    assert( parent->v_next != node );
	
    if( parent->v_next )
        parent->v_next->h_prev = node;
    parent->v_next = node;
	
	return 1 ;
}
/*************************************************************************************/
/* creates empty sequence */
Sequence* CreateSeq( int seq_flags, int header_size, int elem_size, MemStorage * storage )
{
    Sequence *seq = 0;   
	
    if( !storage )
        return NULL;
    if( header_size < (int)sizeof( Sequence ) || elem_size <= 0 )
        return NULL ;
	
    /* allocate sequence header */
    seq = (Sequence*)MemStorageAlloc( storage, header_size ) ;

    memset( seq, 0, header_size );
	
    seq->header_size = header_size;
    seq->flags = (seq_flags & ~CV_MAGIC_MASK) | CV_SEQ_MAGIC_VAL;
    {
        int elemtype = CV_MAT_TYPE(seq_flags);
        int typesize = CV_ELEM_SIZE(elemtype);
		
        if( elemtype != CV_SEQ_ELTYPE_GENERIC && typesize != 0 && typesize != elem_size )
			return NULL ;
    }
    seq->elem_size = elem_size;
    seq->storage = storage;
	
    SetSeqBlockSize( seq, (1 << 10)/elem_size ) ;  
	
    return seq;
}
/*************************************************************************************/
/* adjusts <delta_elems> field of sequence. It determines how much the sequence
grows if there are no free space inside the sequence buffers */
int SetSeqBlockSize( Sequence *seq, int delta_elements )
{
    int elem_size;
    int useful_block_size;    
	
    if( !seq || !seq->storage )
        return -27 ;
    if( delta_elements < 0 )
        return -211 ;
	
    useful_block_size = AlignLeft(seq->storage->block_size - 
		sizeof(MemBlock) - sizeof(SeqBlock), CV_STRUCT_ALIGN) ;
    elem_size = seq->elem_size;
	
    if( delta_elements == 0 )
    {
        delta_elements = (1 << 10) / elem_size;
        delta_elements = MAX( delta_elements, 1 );
    }
    if( delta_elements * elem_size > useful_block_size )
    {
        delta_elements = useful_block_size / elem_size;
        if( delta_elements == 0 )
            return -211 ;
    }
	
    seq->delta_elems = delta_elements;
	
   return 1 ;
}
/*************************************************************************************/
void* MemStorageAlloc( MemStorage* storage, int size )
{
    char *ptr = 0;    
    
    if( !storage )
        return NULL ;
	
    if( size > INT_MAX )
        return NULL;
	
    assert( storage->free_space % CV_STRUCT_ALIGN == 0 );
	
    if( (int)storage->free_space < size )
    {
        int max_free_space = AlignLeft(storage->block_size - sizeof(MemBlock), CV_STRUCT_ALIGN);

        if( max_free_space < size )
            return NULL;
		
        GoNextMemBlock( storage ) ;
    }
	
    ptr = ICV_FREE_PTR(storage);
    assert( (int)ptr % CV_STRUCT_ALIGN == 0 );
    storage->free_space = AlignLeft(storage->free_space - (int)size, CV_STRUCT_ALIGN );    
	
    return ptr;
}

/*************************************************************************************/
int FetchContour( char *ptr, int step, Point2i pt, Sequence* contour, int _method )
{
    const char      nbd = 2;
    int             deltas[16];
    SeqWriter     writer;
    char            *i0 = ptr, *i1, *i3, *i4 = 0;
    int             prev_s = -1, s, s_end;
    int             method = _method - 1;

    assert( (unsigned) _method <= CV_CHAIN_APPROX_SIMPLE );

    /* initialize local state */
    CV_INIT_3X3_DELTAS( deltas, step, 1 );
    memcpy( deltas + 8, deltas, 8 * sizeof( deltas[0] ));

    /* initialize writer */
    StartAppendToSeq( contour, &writer );

    if( method < 0 )
        ((Chain *) contour)->origin = pt;

    s_end = s = CV_IS_SEQ_HOLE( contour ) ? 0 : 4;

    do
    {
        s = (s - 1) & 7;
        i1 = i0 + deltas[s];
        if( *i1 != 0 )
            break;
    }
    while( s != s_end );

    if( s == s_end )            /* single pixel domain */
    {
        *i0 = (char) (nbd | -128);
        if( method >= 0 )
        {
            CV_WRITE_SEQ_ELEM( pt, writer );
        }
    }
    else
    {
        i3 = i0;
        prev_s = s ^ 4;

        /* follow border */
        for( ;; )
        {
            s_end = s;

            for( ;; )
            {
                i4 = i3 + deltas[++s];
                if( *i4 != 0 )
                    break;
            }
            s &= 7;

            /* check "right" bound */
            if( (unsigned) (s - 1) < (unsigned) s_end )
            {
                *i3 = (char) (nbd | -128);
            }
            else if( *i3 == 1 )
            {
                *i3 = nbd;
            }

            if( method < 0 )
            {
                char _s = (char) s;

                CV_WRITE_SEQ_ELEM( _s, writer );
            }
            else
            {
                if( s != prev_s || method == 0 )
                {
                    CV_WRITE_SEQ_ELEM( pt, writer );
                    prev_s = s;
                }

                pt.x += icvCodeDeltas[s].x;
                pt.y += icvCodeDeltas[s].y;

            }

            if( i4 == i0 && i3 == i1 )
                break;

            i3 = i4;
            s = (s + 4) & 7;
        }                       /* end of border following loop */
    }

    EndWriteSeq( &writer );

    if( _method != CV_CHAIN_CODE )
        BoundingRect( contour, 1 );

    assert( writer.seq->total == 0 && writer.seq->first == 0 ||
            writer.seq->total > writer.seq->first->count ||
            (writer.seq->first->prev == writer.seq->first &&
             writer.seq->first->next == writer.seq->first) );

    return 1;
}
/*************************************************************************************/
/* initializes sequence writer */
int StartAppendToSeq( Sequence *seq, SeqWriter * writer )
{	
    if( !seq || !writer )
        return -27 ;
	
    memset( writer, 0, sizeof( *writer ));
    writer->header_size = sizeof( SeqWriter );
	
    writer->seq = seq;
    writer->block = seq->first ? seq->first->prev : 0;
    writer->ptr = seq->ptr;
    writer->block_max = seq->block_max;
	return 1 ;
}

/*************************************************************************************/
int CreateSeqBlock( SeqWriter * writer )
{    
    Sequence *seq;
	
    if( !writer || !writer->seq )
        return -27 ;
	
    seq = writer->seq;
	
    FlushSeqWriter( writer );	
    GrowSeq( seq, 0 ) ;
	
    writer->block = seq->first->prev;
    writer->ptr = seq->ptr;
    writer->block_max = seq->block_max;
	
    return 1 ;
}

/*************************************************************************************/
/* updates sequence header */
int FlushSeqWriter( SeqWriter * writer )
{
    Sequence *seq = 0;    
	
    if( !writer )
        return -27 ;
	
    seq = writer->seq;
    seq->ptr = writer->ptr;
	
    if( writer->block )
    {
        int total = 0;
        SeqBlock *first_block = writer->seq->first;
        SeqBlock *block = first_block;
		
        writer->block->count = (int)((writer->ptr - writer->block->data) / seq->elem_size);
        assert( writer->block->count > 0 );
		
        do
        {
            total += block->count;
            block = block->next;
        }
        while( block != first_block );
		
        writer->seq->total = total;
    }
	
    return 1 ;
}
/*************************************************************************************/
int GrowSeq( Sequence *seq, int in_front_of )
{
    SeqBlock *block;

    if( !seq )
        return -27;
    block = seq->free_blocks;

    if( !block )
    {
        int elem_size = seq->elem_size;
        int delta_elems = seq->delta_elems;
        MemStorage *storage = seq->storage;

        if( seq->total >= delta_elems*4 )
            SetSeqBlockSize( seq, delta_elems*2 );

        if( !storage )
            return -27 ;

        /* if there is a free space just after last allocated block
           and it's big enough then enlarge the last block
           (this can happen only if the new block is added to the end of sequence */
        if( (unsigned)(ICV_FREE_PTR(storage) - seq->block_max) < CV_STRUCT_ALIGN &&
            storage->free_space >= seq->elem_size && !in_front_of )
        {
            int delta = storage->free_space / elem_size;

            delta = MIN( delta, delta_elems ) * elem_size;
            seq->block_max += delta;
            storage->free_space = AlignLeft((int)(((char*)storage->top + storage->block_size) -
                                              seq->block_max), CV_STRUCT_ALIGN );
            return 1;
        }
        else
        {
            int delta = elem_size * delta_elems + ICV_ALIGNED_SEQ_BLOCK_SIZE;

            /* try to allocate <delta_elements> elements */
            if( storage->free_space < delta )
            {
                int small_block_size = MAX(1, delta_elems/3)*elem_size +
                                       ICV_ALIGNED_SEQ_BLOCK_SIZE;
                /* try to allocate smaller part */
                if( storage->free_space >= small_block_size + CV_STRUCT_ALIGN )
                {
                    delta = (storage->free_space - ICV_ALIGNED_SEQ_BLOCK_SIZE)/seq->elem_size;
                    delta = delta*seq->elem_size + ICV_ALIGNED_SEQ_BLOCK_SIZE;
                }
                else
                {
                    GoNextMemBlock( storage);
                    assert( storage->free_space >= delta );
                }
            }

            block = (SeqBlock*)MemStorageAlloc( storage, delta ) ;
            block->data = (char*)AlignPtr( block + 1, CV_STRUCT_ALIGN );
            block->count = delta - ICV_ALIGNED_SEQ_BLOCK_SIZE;
            block->prev = block->next = 0;
        }
    }
    else
    {
        seq->free_blocks = block->next;
    }

    if( !(seq->first) )
    {
        seq->first = block;
        block->prev = block->next = block;
    }
    else
    {
        block->prev = seq->first->prev;
        block->next = seq->first;
        block->prev->next = block->next->prev = block;
    }

    /* for free blocks the <count> field means total number of bytes in the block.
       And for used blocks it means a current number of sequence
       elements in the block */
    assert( block->count % seq->elem_size == 0 && block->count > 0 );

    if( !in_front_of )
    {
        seq->ptr = block->data;
        seq->block_max = block->data + block->count;
        block->start_index = block == block->prev ? 0 :
            block->prev->start_index + block->prev->count;
    }
    else
    {
        int delta = block->count / seq->elem_size;
        block->data += block->count;

        if( block != block->prev )
        {
            assert( seq->first->start_index == 0 );
            seq->first = block;
        }
        else
        {
            seq->block_max = seq->ptr = block->data;
        }

        block->start_index = 0;

        for( ;; )
        {
            block->start_index += delta;
            block = block->next;
            if( block == seq->first )
                break;
        }
    }

    block->count = 0;

    return 1 ;
}
/*************************************************************************************/
int GoNextMemBlock( MemStorage * storage )
{    
    if( !storage )
        return -27;
	
    if( !storage->top || !storage->top->next )
    {
        MemBlock *block;
		
        if( !(storage->parent) )
        {
            if( !(block = (MemBlock *)Alloc( storage->block_size )) )
				return -27 ;
        }
        else
        {
            MemStorage *parent = storage->parent;
            MemStoragePos parent_pos;
			
            SaveMemStoragePos( parent, &parent_pos );
            GoNextMemBlock( parent );
			
            block = parent->top;
            RestoreMemStoragePos( parent, &parent_pos );
			
            if( block == parent->top )  /* the single allocated block */
            {
                assert( parent->bottom == block );
                parent->top = parent->bottom = 0;
                parent->free_space = 0;
            }
            else
            {
                /* cut the block from the parent's list of blocks */
                parent->top->next = block->next;
                if( block->next )
                    block->next->prev = parent->top;
            }
        }
		
        /* link block */
        block->next = 0;
        block->prev = storage->top;
		
        if( storage->top )
            storage->top->next = block;
        else
            storage->top = storage->bottom = block;
    }
	
    if( storage->top->next )
        storage->top = storage->top->next;
    storage->free_space = storage->block_size - sizeof(MemBlock);
    assert( storage->free_space % CV_STRUCT_ALIGN == 0 );

    return 1 ;
}
/*************************************************************************************/
void*  Alloc( int size )
{
    void* ptr = 0;   
    
    if( (int)size > CV_MAX_ALLOC_SIZE )
        return NULL ;
	
    ptr = p_cvAlloc( size, p_cvAllocUserData );

    if( !ptr )
        return NULL ;
	
    return ptr;
}
/*************************************************************************************/
/* calls icvFlushSeqWriter and finishes writing process */
Sequence* EndWriteSeq( SeqWriter * writer )
{
    Sequence *seq = 0 ;	
    
    if( !writer )
        return NULL ;
	
    FlushSeqWriter( writer ) ;
    seq = writer->seq;
	
    /* truncate the last block */
    if( writer->block && writer->seq->storage )
    {
        MemStorage *storage = seq->storage;
        char *storage_block_max = (char *) storage->top + storage->block_size;
		
        assert( writer->block->count > 0 );
		
        if( (unsigned)((storage_block_max - storage->free_space)
            - seq->block_max) < CV_STRUCT_ALIGN )
        {
            storage->free_space = AlignLeft((int)(storage_block_max - seq->ptr), CV_STRUCT_ALIGN);
            seq->block_max = seq->ptr;
        }
    }
	
    writer->ptr = 0;   
	
    return seq;
}
/*************************************************************************************/
/* Calculates bounding rectagnle of a point set or retrieves already calculated */
Rect BoundingRect( Arr* array, int update )
{ 
    SeqReader reader;
    Rect  rect(0, 0, 0, 0);
    Sequence* ptseq = 0;    
    int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;
    int calculate = update;

    if( CV_IS_SEQ( array ))
    {
        ptseq = (Sequence*)array;
        if( !CV_IS_SEQ_POINT_SET( ptseq ))
            return rect;

        if( ptseq->header_size < (int)sizeof(Contour))
        {
            /*if( update == 1 )
                CV_ERROR( CV_StsBadArg, "The header is too small to fit the rectangle, "
                                        "so it could not be updated" );*/
            update = 0;
            calculate = 1;
        }
    }    
    if( !calculate )
    {
        rect = ((Contour*)ptseq)->rect;
        return rect ;
    }
    
    if( ptseq->total )
    {   
        int  is_float = CV_SEQ_ELTYPE(ptseq) == CV_32FC2;
        StartReadSeq( ptseq, &reader, 0 );

        if( !is_float )
        {
            Point2i pt;
            /* init values */
            CV_READ_SEQ_ELEM( pt, reader );
            xmin = xmax = pt.x;
            ymin = ymax = pt.y;

            for( i = 1; i < ptseq->total; i++ )
            {            
                CV_READ_SEQ_ELEM( pt, reader );
        
                if( xmin > pt.x )
                    xmin = pt.x;
        
                if( xmax < pt.x )
                    xmax = pt.x;

                if( ymin > pt.y )
                    ymin = pt.y;

                if( ymax < pt.y )
                    ymax = pt.y;
            }
        }
        else
        {
            Point2i pt;
            Cv32suf v;
            /* init values */
            CV_READ_SEQ_ELEM( pt, reader );
            xmin = xmax = CV_TOGGLE_FLT(pt.x);
            ymin = ymax = CV_TOGGLE_FLT(pt.y);

            for( i = 1; i < ptseq->total; i++ )
            {            
                CV_READ_SEQ_ELEM( pt, reader );
                pt.x = CV_TOGGLE_FLT(pt.x);
                pt.y = CV_TOGGLE_FLT(pt.y);
        
                if( xmin > pt.x )
                    xmin = pt.x;
        
                if( xmax < pt.x )
                    xmax = pt.x;

                if( ymin > pt.y )
                    ymin = pt.y;

                if( ymax < pt.y )
                    ymax = pt.y;
            }

            v.i = CV_TOGGLE_FLT(xmin); xmin = (int)floor(v.f);
            v.i = CV_TOGGLE_FLT(ymin); ymin = (int)floor(v.f);
            /* because right and bottom sides of
               the bounding rectangle are not inclusive
               (note +1 in width and height calculation below),
               cvFloor is used here instead of cvCeil */
            v.i = CV_TOGGLE_FLT(xmax); xmax = (int)floor(v.f);
            v.i = CV_TOGGLE_FLT(ymax); ymax = (int)floor(v.f);
        }
    }

    rect.x = xmin;
    rect.y = ymin;
    rect.width = xmax - xmin + 1;
    rect.height = ymax - ymin + 1;

    if( update )
        ((Contour*)ptseq)->rect = rect;

    return rect;
}
/*************************************************************************************/
/* adds new element to the set */
int SetAdd( Set* set, SetElem* element, SetElem** inserted_element )
{
    int id = -1;	
    SetElem *free_elem;
	
    if( !set )
        return -27 ;
	
    if( !(set->free_elems) )
    {
        int count = set->total;
        int elem_size = set->elem_size;
        char *ptr;
        GrowSeq( (Sequence *) set, 0 );
		
        set->free_elems = (SetElem*) (ptr = set->ptr);
        for( ; ptr + elem_size <= set->block_max; ptr += elem_size, count++ )
        {
            ((SetElem*)ptr)->flags = count | CV_SET_ELEM_FREE_FLAG;
            ((SetElem*)ptr)->next_free = (SetElem*)(ptr + elem_size);
        }
        assert( count <= CV_SET_ELEM_IDX_MASK+1 );
        ((SetElem*)(ptr - elem_size))->next_free = 0;
        set->first->prev->count += count - set->total;
        set->total = count;
        set->ptr = set->block_max;
    }
	
    free_elem = set->free_elems;
    set->free_elems = free_elem->next_free;
	
    id = free_elem->flags & CV_SET_ELEM_IDX_MASK;
    if( element )
        CV_MEMCPY_INT( free_elem, element, (int)set->elem_size/sizeof(int) );
	
    free_elem->flags = id;
    set->active_count++;
	
    if( inserted_element )
        *inserted_element = free_elem;   
	
    return id;
}
/*************************************************************************************/
int FetchContourEx( char* ptr, int step, Point2i pt, Sequence* contour, 
				   int  _method,  int nbd, Rect* _rect )
{
    int         deltas[16];
    SeqWriter	writer;
    char        *i0 = ptr, *i1, *i3, *i4;
    Rect		rect;
    int         prev_s = -1, s, s_end;
    int         method = _method - 1;

    assert( (unsigned) _method <= CV_CHAIN_APPROX_SIMPLE );
    assert( 1 < nbd && nbd < 128 );

    /* initialize local state */
    CV_INIT_3X3_DELTAS( deltas, step, 1 );
    memcpy( deltas + 8, deltas, 8 * sizeof( deltas[0] ));

    /* initialize writer */
    StartAppendToSeq( contour, &writer );

    if( method < 0 )
        ((Chain *)contour)->origin = pt;

    rect.x = rect.width = pt.x;
    rect.y = rect.height = pt.y;

    s_end = s = CV_IS_SEQ_HOLE( contour ) ? 0 : 4;

    do
    {
        s = (s - 1) & 7;
        i1 = i0 + deltas[s];
        if( *i1 != 0 )
            break;
    }
    while( s != s_end );

    if( s == s_end )            /* single pixel domain */
    {
        *i0 = (char) (nbd | 0x80);
        if( method >= 0 )
        {
            CV_WRITE_SEQ_ELEM( pt, writer );
        }
    }
    else
    {
        i3 = i0;

        prev_s = s ^ 4;

        /* follow border */
        for( ;; )
        {
            s_end = s;

            for( ;; )
            {
                i4 = i3 + deltas[++s];
                if( *i4 != 0 )
                    break;
            }
            s &= 7;

            /* check "right" bound */
            if( (unsigned) (s - 1) < (unsigned) s_end )
            {
                *i3 = (char) (nbd | 0x80);
            }
            else if( *i3 == 1 )
            {
                *i3 = (char) nbd;
            }

            if( method < 0 )
            {
                char _s = (char) s;
                CV_WRITE_SEQ_ELEM( _s, writer );
            }
            else if( s != prev_s || method == 0 )
            {
                CV_WRITE_SEQ_ELEM( pt, writer );
            }

            if( s != prev_s )
            {
                /* update bounds */
                if( pt.x < rect.x )
                    rect.x = pt.x;
                else if( pt.x > rect.width )
                    rect.width = pt.x;

                if( pt.y < rect.y )
                    rect.y = pt.y;
                else if( pt.y > rect.height )
                    rect.height = pt.y;
            }

            prev_s = s;
            pt.x += icvCodeDeltas[s].x;
            pt.y += icvCodeDeltas[s].y;

            if( i4 == i0 && i3 == i1 )  break;

            i3 = i4;
            s = (s + 4) & 7;
        }                       /* end of border following loop */
    }

    rect.width -= rect.x - 1;
    rect.height -= rect.y - 1;

    EndWriteSeq( &writer );

    if( _method != CV_CHAIN_CODE )
        ((Contour*)contour)->rect = rect;

    assert( writer.seq->total == 0 && writer.seq->first == 0 ||
            writer.seq->total > writer.seq->first->count ||
            (writer.seq->first->prev == writer.seq->first &&
             writer.seq->first->next == writer.seq->first) );

    if( _rect )  *_rect = rect;

    return 1;
}
/*************************************************************************************/
/* curvature: 0 - 1-curvature, 1 - k-cosine curvature. */
int ApproximateChainTC89( Chain* chain, int header_size, MemStorage* storage, 
                         Sequence** contour, int method )
{
    static const int abs_diff[] = { 1, 2, 3, 4, 3, 2, 1, 0, 1, 2, 3, 4, 3, 2, 1 };

    char            local_buffer[1 << 16];
    char*           buffer = local_buffer;
    int             buffer_size;

    PtInfo       temp;
    PtInfo       *array, *first = 0, *current = 0, *prev_current = 0;
    int             i, j, i1, i2, s, len;
    int             count;

    ChainPtReader reader;
    SeqWriter     writer;
    Point2i        pt = chain->origin;
   
    assert( chain && contour && buffer );

    buffer_size = (chain->total + 8) * sizeof( PtInfo );

    *contour = 0;

    if( !CV_IS_SEQ_CHAIN_CONTOUR( chain ))
        return -12 ;

    if( header_size < (int)sizeof(Contour) )
        return -12 ;
    
    StartWriteSeq( (chain->flags & ~CV_SEQ_ELTYPE_MASK) | CV_SEQ_ELTYPE_POINT,
                     header_size, sizeof( Point2i ), storage, &writer );
    
    if( chain->total == 0 )
    {        
        CV_WRITE_SEQ_ELEM( pt, writer );
        goto exit_function;
    }

    StartReadChainPoints( chain, &reader );

    if( method > CV_CHAIN_APPROX_SIMPLE && buffer_size > (int)sizeof(local_buffer))
    {
        buffer = (char *) Alloc( buffer_size );
        if( !buffer )
            return -3 ;
    }

    array = (PtInfo*) buffer;
    count = chain->total;

    temp.next = 0;
    current = &temp;

    /* Pass 0.
       Restores all the digital curve points from the chain code.
       Removes the points (from the resultant polygon)
       that have zero 1-curvature */
    for( i = 0; i < count; i++ )
    {
        int prev_code = *reader.prev_elem;

        reader.prev_elem = reader.ptr;
        CV_READ_CHAIN_POINT( pt, reader );

        /* calc 1-curvature */
        s = abs_diff[reader.code - prev_code + 7];

        if( method <= CV_CHAIN_APPROX_SIMPLE )
        {
            if( method == CV_CHAIN_APPROX_NONE || s != 0 )
            {
                CV_WRITE_SEQ_ELEM( pt, writer );
            }
        }
        else
        {
            if( s != 0 )
                current = current->next = array + i;
            array[i].s = s;
            array[i].pt = pt;
        }
    }

    //assert( pt.x == chain->origin.x && pt.y == chain->origin.y );

    if( method <= CV_CHAIN_APPROX_SIMPLE )
        goto exit_function;

    current->next = 0;

    len = i;
    current = temp.next;

    assert( current );

    /* Pass 1.
       Determines support region for all the remained points */
    do
    {
        Point2i pt0;
        int k, l = 0, d_num = 0;

        i = (int)(current - array);
        pt0 = array[i].pt;

        /* determine support region */
        for( k = 1;; k++ )
        {
            int lk, dk_num;
            int dx, dy;
            Cv32suf d;

            assert( k <= len );

            /* calc indices */
            i1 = i - k;
            i1 += i1 < 0 ? len : 0;
            i2 = i + k;
            i2 -= i2 >= len ? len : 0;

            dx = array[i2].pt.x - array[i1].pt.x;
            dy = array[i2].pt.y - array[i1].pt.y;

            /* distance between p_(i - k) and p_(i + k) */
            lk = dx * dx + dy * dy;

            /* distance between p_i and the line (p_(i-k), p_(i+k)) */
            dk_num = (pt0.x - array[i1].pt.x) * dy - (pt0.y - array[i1].pt.y) * dx;
            d.f = (float) (((double) d_num) * lk - ((double) dk_num) * l);

            if( k > 1 && (l >= lk || (d_num > 0 && d.i <= 0 || d_num < 0 && d.i >= 0)))
                break;

            d_num = dk_num;
            l = lk;
        }

        current->k = --k;

        /* determine cosine curvature if it should be used */
        if( method == CV_CHAIN_APPROX_TC89_KCOS )
        {
            /* calc k-cosine curvature */
            for( j = k, s = 0; j > 0; j-- )
            {
                double temp_num;
                int dx1, dy1, dx2, dy2;
                Cv32suf sk;

                i1 = i - j;
                i1 += i1 < 0 ? len : 0;
                i2 = i + j;
                i2 -= i2 >= len ? len : 0;

                dx1 = array[i1].pt.x - pt0.x;
                dy1 = array[i1].pt.y - pt0.y;
                dx2 = array[i2].pt.x - pt0.x;
                dy2 = array[i2].pt.y - pt0.y;

                if( (dx1 | dy1) == 0 || (dx2 | dy2) == 0 )
                    break;

                temp_num = dx1 * dx2 + dy1 * dy2;
                temp_num =
                    (float) (temp_num /
                             sqrt( ((double)dx1 * dx1 + (double)dy1 * dy1) *
                                   ((double)dx2 * dx2 + (double)dy2 * dy2) ));
                sk.f = (float) (temp_num + 1.1);

                assert( 0 <= sk.f && sk.f <= 2.2 );
                if( j < k && sk.i <= s )
                    break;

                s = sk.i;
            }
            current->s = s;
        }
        current = current->next;
    }
    while( current != 0 );

    prev_current = &temp;
    current = temp.next;

    /* Pass 2.
       Performs non-maxima supression */
    do
    {
        int k2 = current->k >> 1;

        s = current->s;
        i = (int)(current - array);

        for( j = 1; j <= k2; j++ )
        {
            i2 = i - j;
            i2 += i2 < 0 ? len : 0;

            if( array[i2].s > s )
                break;

            i2 = i + j;
            i2 -= i2 >= len ? len : 0;

            if( array[i2].s > s )
                break;
        }

        if( j <= k2 )           /* exclude point */
        {
            prev_current->next = current->next;
            current->s = 0;     /* "clear" point */
        }
        else
            prev_current = current;
        current = current->next;
    }
    while( current != 0 );

    /* Pass 3.
       Removes non-dominant points with 1-length support region */
    current = temp.next;
    assert( current );
    prev_current = &temp;

    do
    {
        if( current->k == 1 )
        {
            s = current->s;
            i = (int)(current - array);

            i1 = i - 1;
            i1 += i1 < 0 ? len : 0;

            i2 = i + 1;
            i2 -= i2 >= len ? len : 0;

            if( s <= array[i1].s || s <= array[i2].s )
            {
                prev_current->next = current->next;
                current->s = 0;
            }
            else
                prev_current = current;
        }
        else
            prev_current = current;
        current = current->next;
    }
    while( current != 0 );

    if( method == CV_CHAIN_APPROX_TC89_KCOS )
        goto copy_vect;

    /* Pass 4.
       Cleans remained couples of points */
    assert( temp.next );

    if( array[0].s != 0 && array[len - 1].s != 0 )      /* specific case */
    {
        for( i1 = 1; i1 < len && array[i1].s != 0; i1++ )
        {
            array[i1 - 1].s = 0;
        }
        if( i1 == len )
            goto copy_vect;     /* all points survived */
        i1--;

        for( i2 = len - 2; i2 > 0 && array[i2].s != 0; i2-- )
        {
            array[i2].next = 0;
            array[i2 + 1].s = 0;
        }
        i2++;

        if( i1 == 0 && i2 == len - 1 )  /* only two points */
        {
            i1 = (int)(array[0].next - array);
            array[len] = array[0];      /* move to the end */
            array[len].next = 0;
            array[len - 1].next = array + len;
        }
        temp.next = array + i1;
    }

    current = temp.next;
    first = prev_current = &temp;
    count = 1;

    /* do last pass */
    do
    {
        if( current->next == 0 || current->next - current != 1 )
        {
            if( count >= 2 )
            {
                if( count == 2 )
                {
                    int s1 = prev_current->s;
                    int s2 = current->s;

                    if( s1 > s2 || s1 == s2 && prev_current->k <= current->k )
                        /* remove second */
                        prev_current->next = current->next;
                    else
                        /* remove first */
                        first->next = current;
                }
                else
                    first->next->next = current;
            }
            first = current;
            count = 1;
        }
        else
            count++;
        prev_current = current;
        current = current->next;
    }
    while( current != 0 );

  copy_vect:

    /* gather points */
    current = temp.next;
    assert( current );

    do
    {
        CV_WRITE_SEQ_ELEM( current->pt, writer );
        current = current->next;
    }
    while( current != 0 );

	exit_function:

    *contour = EndWriteSeq( &writer );

    assert( writer.seq->total > 0 );

    if( buffer != local_buffer )
        cvFree( &buffer );
    return 1 ;
}

/*************************************************************************************/
int StartWriteSeq( int seq_flags, int header_size,
				int elem_size, MemStorage * storage, SeqWriter * writer )
{
    Sequence *seq = 0;
	
    if( !storage || !writer )
        return -27 ;
	
    seq = CreateSeq( seq_flags, header_size, elem_size, storage ) ;
    if( !StartAppendToSeq( seq, writer ) )
		return -27 ;

	return 1 ;
}
/*************************************************************************************/

int StartReadChainPoints( Chain * chain, ChainPtReader * reader )
{
    int i;    
	
    if( !chain || !reader )
        return -27 ;
	
    if( chain->elem_size != 1 || chain->header_size < (int)sizeof(Chain))
        return -1 ;
	
    StartReadSeq( (Sequence *) chain, (SeqReader *) reader, 0 );
//    CV_CHECK();
	
    reader->pt = chain->origin;
	
    for( i = 0; i < 8; i++ )
    {
        reader->deltas[i][0] = (char) icvCodeDeltas[i].x;
        reader->deltas[i][1] = (char) icvCodeDeltas[i].y;
    }
	return 1 ;
}
/*************************************************************************************/
/* initializes sequence reader */
int StartReadSeq( const Sequence *seq, SeqReader * reader, int reverse )
{
    SeqBlock *first_block;
    SeqBlock *last_block;
    
    if( reader )
    {
        reader->seq = 0;
        reader->block = 0;
        reader->ptr = reader->block_max = reader->block_min = 0;
    }    
	
    if( !seq || !reader )
        return -27 ;
	
    reader->header_size = sizeof( SeqReader );
    reader->seq = (Sequence*)seq;
	
    first_block = seq->first;
	
    if( first_block )
    {
        last_block = first_block->prev;
        reader->ptr = first_block->data;
        reader->prev_elem = CV_GET_LAST_ELEM( seq, last_block );
        reader->delta_index = seq->first->start_index;
		
        if( reverse )
        {
            char *temp = reader->ptr;
			
            reader->ptr = reader->prev_elem;
            reader->prev_elem = temp;
			
            reader->block = last_block;
        }
        else
        {
            reader->block = first_block;
        }
		
        reader->block_min = reader->block->data;
        reader->block_max = reader->block_min + reader->block->count * seq->elem_size;
    }
    else
    {
        reader->delta_index = 0;
        reader->block = 0;
		reader->ptr = reader->prev_elem = reader->block_min = reader->block_max = 0;
    }
	return 1 ;
}

/*************************************************************************************/
/* changes the current reading block to the previous or to the next */
int ChangeSeqBlock( void* _reader, int direction )
{	
    SeqReader* reader = (SeqReader*)_reader;
    
    if( !reader )
        return -27 ;
	
    if( direction > 0 )
    {
        reader->block = reader->block->next;
        reader->ptr = reader->block->data;
    }
    else
    {
        reader->block = reader->block->prev;
        reader->ptr = CV_GET_LAST_ELEM( reader->seq, reader->block );
    }
    reader->block_min = reader->block->data;
    reader->block_max = reader->block_min + reader->block->count * reader->seq->elem_size;
	
    return 1 ;;
}
/*===================================================================================*/
Sequence* EndFindContours( ContourScanner* _scanner )
{
    ContourScanner scanner;
    Sequence *first = 0;    
	
    if( !_scanner )
        return NULL ;
    scanner = *_scanner;
	
    if( scanner )
    {
        EndProcessContour( scanner );
		
        if( scanner->storage1 != scanner->storage2 )
            ReleaseMemStorage( &(scanner->storage1) );
		
        if( scanner->cinfo_storage )
            ReleaseMemStorage( &(scanner->cinfo_storage) );
		
        first = scanner->frame.v_next;
        cvFree( _scanner );
    }	
    return first;
}
/*************************************************************************************/
/* releases memory storage */
int ReleaseMemStorage( MemStorage** storage )
{
    MemStorage *st;
	
    if( !storage )
        return -27 ;
	
    st = *storage;
    *storage = 0;
	
    if( st )
    {
        DestroyMemStorage( st );
        cvFree( &st );
    }
    return 1 ;
}
/*************************************************************************************/
MemStorage* CreateMemStorage( int block_size )
{
    MemStorage *storage = 0;    
	
    storage = (MemStorage *)Alloc( sizeof( MemStorage ));
    InitMemStorage( storage, block_size ) ;
    
	
//     if( cvGetErrStatus() < 0 )
//         cvFree( &storage );
	
    return storage;
}
/*************************************************************************************/
/* initializes allocated storage */
int InitMemStorage( MemStorage* storage, int block_size )
{    
    if( !storage )
        return -27 ;
	
    if( block_size <= 0 )
        block_size = CV_STORAGE_BLOCK_SIZE;
	
    block_size = Align( block_size, CV_STRUCT_ALIGN );
    assert( sizeof(MemBlock) % CV_STRUCT_ALIGN == 0 );
	
    memset( storage, 0, sizeof( *storage ));
    storage->signature = CV_STORAGE_MAGIC_VAL;
    storage->block_size = block_size;
	
    return 1 ;
}
/*************************************************************************************/
MemStorage* CreateChildMemStorage( MemStorage * parent )
{
    MemStorage *storage = 0;
    
    if( !parent )
        return NULL;
	
    storage = CreateMemStorage(parent->block_size);
    storage->parent = parent;	
   
//     if( cvGetErrStatus() < 0 )
//         cvFree( &storage );
	
    return storage;
}
/*************************************************************************************/
Set* CreateSet( int set_flags, int header_size, int elem_size, MemStorage * storage )
{
    Set *set = 0;
    
    if( !storage )
        return NULL;
    if( header_size < (int)sizeof( Set ) || elem_size < (int)sizeof(void*)*2 ||
        (elem_size & (sizeof(void*)-1)) != 0 )
        return NULL ;
	
    set = (Set*) CreateSeq( set_flags, header_size, elem_size, storage );
    set->flags = (set->flags & ~CV_MAGIC_MASK) | CV_SET_MAGIC_VAL;   
	
    return set;
}

/*************************************************************************************/
Sequence* TestContour( Sequence* contours, MemStorage* storage, int method, Sequence* rst )
{
	// test each contour
	Sequence* result ;
//	Sequence* squares = CreateSeq( 0, sizeof(Sequence), sizeof(Point2i), storage );
	
	int i ;
	double s, t ;
	
	while( contours )
	{
		// approximate contour with accuracy proportional
		// to the contour perimeter
		result = ApproxPoly( contours, sizeof(Contour), storage,
			CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
		// square contours should have 4 vertices after approximation
		// relatively large area (to filter out noisy contours)
		// and be convex.
		// Note: absolute value of an area is used because
		// area may be positive or negative - in accordance with the
		// contour orientation
		if( result->total == 4 &&
			fabs(ContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
			CheckContourConvexity(result) )
		{
			s = 0;
			
			for( i = 0; i < 5; i++ )
			{
				// find minimum Angle between joint
				// edges (maximum of cosine)
				if( i >= 2 )
				{
					t = fabs(Angle(
						(SPoint*)GetSeqElem( result, i ),
						(SPoint*)GetSeqElem( result, i-2 ),
						(SPoint*)GetSeqElem( result, i-1 )));
					s = s > t ? s : t;
				}
			}
			
			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence 
			if( s < 0.3 )
				for( i = 0; i < 4; i++ )
				{
					SeqPush( rst, (Point2i*)GetSeqElem( result, i ));				
				}
		}
		
		// take the next contour
		contours = contours->h_next;
	}	
//	return squares ;
	return NULL ;
}
/*************************************************************************************/
Sequence* ApproxPoly( const void*  array, int  header_size, MemStorage*  storage, int  method, 
						double  parameter, int parameter2 )
{
	Sequence* dst_seq = 0;
    Sequence *prev_contour = 0, *parent = 0;
    Contour contour_header;
    Sequence* src_seq = 0;
    SeqBlock block;
    int recursive = 0;
    
    if( CV_IS_SEQ( array ))
    {
        src_seq = (Sequence*)array;
        if( !CV_IS_SEQ_POLYLINE( src_seq ))
            return NULL ;

        recursive = parameter2;

        if( !storage )
            storage = src_seq->storage;
    }
    else
    {
        src_seq = PointSeqFromMat( CV_SEQ_KIND_CURVE | (parameter2 ? CV_SEQ_FLAG_CLOSED : 0), 
									 array, &contour_header, &block ) ;

		if( src_seq == NULL )
			return NULL ;
    }

    if( !storage )
        return NULL ;

    if( header_size < 0 )
        return NULL ;

    if( header_size == 0 )
        header_size = src_seq->header_size;

    if( !CV_IS_SEQ_POLYLINE( src_seq ))
		return NULL ;

    if( header_size == 0 )
        header_size = src_seq->header_size;

    if( header_size < (int)sizeof(Contour) )
        return NULL ;

    if( method != CV_POLY_APPROX_DP )
        return NULL ;

    while( src_seq != 0 )
    {
        Sequence *contour = 0;

        switch (method)
        {
        case CV_POLY_APPROX_DP:
            if( parameter < 0 )
                return NULL ;

            if( CV_SEQ_ELTYPE(src_seq) == CV_32SC2 )
            {
                iApproxPolyDP_32s( src_seq, header_size, storage, &contour, (float)parameter ) ;
            }
            else
            {
                //int i = 0 ; //icvApproxPolyDP_32f( src_seq, header_size, storage, &contour, (float)parameter );
            }
            break;
        default:
            assert(0);
            return NULL ;
        }

        assert( contour );

        if( header_size >= (int)sizeof(Contour))
            BoundingRect( contour, 1 );

        contour->v_prev = parent;
        contour->h_prev = prev_contour;

        if( prev_contour )
            prev_contour->h_next = contour;
        else if( parent )
            parent->v_next = contour;
        prev_contour = contour;
        if( !dst_seq )
            dst_seq = prev_contour;

        if( !recursive )
            break;

        if( src_seq->v_next )
        {
            assert( prev_contour != 0 );
            parent = prev_contour;
            prev_contour = 0;
            src_seq = src_seq->v_next;
        }
        else
        {
            while( src_seq->h_next == 0 )
            {
                src_seq = src_seq->v_prev;
                if( src_seq == 0 )
                    break;
                prev_contour = parent;
                if( parent )
                    parent = parent->v_prev;
            }
            if( src_seq )
                src_seq = src_seq->h_next;
        }
    }    
    return dst_seq;
}
/*************************************************************************************/
double ArcLength( const void *array, Slice slice, int is_closed )
{
    double perimeter = 0;	
    int i, j = 0, count;
    const int N = 16;
    float buf[N];
    Mat buffer ;
	buffer.Create( buf, 1, N, MAT_Tfloat) ;

    SeqReader reader;
    Contour contour_header;
    Sequence* contour = 0;
    SeqBlock block;
	
    if( CV_IS_SEQ( array ))
    {
        contour = (Sequence*)array;
        if( !CV_IS_SEQ_POLYLINE( contour ))
            return -5 ;
        if( is_closed < 0 )
            is_closed = CV_IS_SEQ_CLOSED( contour );
    }
    else
    {
        is_closed = is_closed > 0;
        contour = PointSeqFromMat( CV_SEQ_KIND_CURVE | (is_closed ? CV_SEQ_FLAG_CLOSED : 0), 
								   array, &contour_header, &block ) ;
		if( contour == NULL )
			return -5 ;
    }
	
    if( contour->total > 1 )
    {
        int is_float = CV_SEQ_ELTYPE( contour ) == CV_32FC2;
        
        StartReadSeq( contour, &reader, 0 );
        SetSeqReaderPos( &reader, slice.start_index );
        count = SliceLength( slice, contour );
		
        count -= !is_closed && count == contour->total;
		
        /* scroll the reader by 1 point */
        reader.prev_elem = reader.ptr;
        CV_NEXT_SEQ_ELEM( sizeof(Point2i), reader );
		
        for( i = 0; i < count; i++ )
        {
            float dx, dy;
			
            if( !is_float )
            {
                Point2i* pt = (Point2i*)reader.ptr;
                Point2i* prev_pt = (Point2i*)reader.prev_elem;
				
                dx = (float)pt->x - (float)prev_pt->x;
                dy = (float)pt->y - (float)prev_pt->y;
            }
            else
            {
                Point2f* pt = (Point2f*)reader.ptr;
                Point2f* prev_pt = (Point2f*)reader.prev_elem;
				
                dx = pt->x - prev_pt->x;
                dy = pt->y - prev_pt->y;
            }
			
            reader.prev_elem = reader.ptr;
            CV_NEXT_SEQ_ELEM( contour->elem_size, reader );
			
            buffer.data.fl[0][j] = dx * dx + dy * dy;
            if( ++j == N || i == count - 1 )
            {
                buffer.ResizeCols( j ) ;
				buffer.Sqrt();
                for( ; j > 0; j-- )
                    perimeter += buffer.data.fl[0][j-1];
            }
        }
    }
    return perimeter;
}
/*************************************************************************************/
/* external contour area function */
double ContourArea( const void *array, Slice slice )
{
    double area = 0;
    Contour contour_header;
    Sequence* contour = 0;
    SeqBlock block;
	
    if( CV_IS_SEQ( array ))
    {
        contour = (Sequence*)array;
        if( !CV_IS_SEQ_POLYLINE( contour ))
            return  -5 ;
    }
    else
    {
        contour = PointSeqFromMat( CV_SEQ_KIND_CURVE, array, &contour_header, &block ) ;
		
		if( contour == NULL )
			return -5 ;
    }
	
    if( SliceLength( slice, contour ) == contour->total )
    {
        iContourArea( contour, &area ) ;
    }
    else
    {
        if( CV_SEQ_ELTYPE( contour ) != CV_32SC2 )
            return -210 ;

        iContourSecArea( contour, slice, &area ) ;
    }    
	
    return area;
}
/*************************************************************************************/
int CheckContourConvexity( const Arr* array )
{
    int flag = -1;
    int i;
    int orientation = 0;
    SeqReader reader;
    Contour contour_header;
    SeqBlock block;
    Sequence* contour = (Sequence*)array;

    if( CV_IS_SEQ(contour) )
    {
        if( !CV_IS_SEQ_POLYGON(contour))
            return -210 ;
    }
    else
    {
        contour = PointSeqFromMat( CV_SEQ_KIND_CURVE|CV_SEQ_FLAG_CLOSED, array, &contour_header, &block );
    }

    if( contour->total == 0 )
        return 0 ;

    StartReadSeq( contour, &reader, 0 );
    
    flag = 1;

    if( CV_SEQ_ELTYPE( contour ) == CV_32SC2 )
    {
        Point2i *prev_pt = (Point2i*)reader.prev_elem;
        Point2i *cur_pt = (Point2i*)reader.ptr;
    
        int dx0 = cur_pt->x - prev_pt->x;
        int dy0 = cur_pt->y - prev_pt->y;

        for( i = 0; i < contour->total; i++ )
        {
            int dxdy0, dydx0;
            int dx, dy;

            /*int orient; */
            CV_NEXT_SEQ_ELEM( sizeof(Point2i), reader );
            prev_pt = cur_pt;
            cur_pt = (Point2i *) reader.ptr;

            dx = cur_pt->x - prev_pt->x;
            dy = cur_pt->y - prev_pt->y;
            dxdy0 = dx * dy0;
            dydx0 = dy * dx0;

            /* find orientation */
            /*orient = -dy0 * dx + dx0 * dy;
               orientation |= (orient > 0) ? 1 : 2;
             */
            orientation |= (dydx0 > dxdy0) ? 1 : ((dydx0 < dxdy0) ? 2 : 3);

            if( orientation == 3 )
            {
                flag = 0;
                break;
            }

            dx0 = dx;
            dy0 = dy;
        }
    }
    else
    {
        assert( CV_SEQ_ELTYPE(contour) == CV_32FC2 );

        Point2f *prev_pt = (Point2f*)reader.prev_elem;
        Point2f *cur_pt = (Point2f*)reader.ptr;
    
        float dx0 = cur_pt->x - prev_pt->x;
        float dy0 = cur_pt->y - prev_pt->y;

        for( i = 0; i < contour->total; i++ )
        {
            float dxdy0, dydx0;
            float dx, dy;

            /*int orient; */
            CV_NEXT_SEQ_ELEM( sizeof(Point2f), reader );
            prev_pt = cur_pt;
            cur_pt = (Point2f*) reader.ptr;

            dx = cur_pt->x - prev_pt->x;
            dy = cur_pt->y - prev_pt->y;
            dxdy0 = dx * dy0;
            dydx0 = dy * dx0;

            /* find orientation */
            /*orient = -dy0 * dx + dx0 * dy;
               orientation |= (orient > 0) ? 1 : 2;
             */
            orientation |= (dydx0 > dxdy0) ? 1 : ((dydx0 < dxdy0) ? 2 : 3);

            if( orientation == 3 )
            {
                flag = 0;
                break;
            }

            dx0 = dx;
            dy0 = dy;
        }
    }
   
    return flag;
}

/*************************************************************************************/
char* GetSeqElem( const Sequence *seq, int index )
{
    SeqBlock *block;
    int count, total = seq->total;
	
    if( (unsigned)index >= (unsigned)total )
    {
        index += index < 0 ? total : 0;
        index -= index >= total ? total : 0;
        if( (unsigned)index >= (unsigned)total )
            return 0;
    }
	
    block = seq->first;
    if( index + index <= total )
    {
        while( index >= (count = block->count) )
        {
            block = block->next;
            index -= count;
        }
    }
    else
    {
        do
        {
            block = block->prev;
            total -= block->count;
        }
        while( index < total );
        index -= total;
    }
	
    return block->data + index * seq->elem_size;
}
/*************************************************************************************/
/* pushes element to the sequence */
char* SeqPush( Sequence *seq, void *element )
{
    char *ptr = 0;
    int elem_size;   
	
    if( !seq )
        return NULL;
	
    elem_size = seq->elem_size;
    ptr = seq->ptr;
	
    if( ptr >= seq->block_max )
    {
        GrowSeq( seq, 0 ) ;
		
        ptr = seq->ptr;
        assert( ptr + elem_size <= seq->block_max /*&& ptr == seq->block_min */  );
    }
	
    if( element )
        CV_MEMCPY_AUTO( ptr, element, elem_size );
    seq->first->prev->count++;
    seq->total++;
    seq->ptr = ptr + elem_size;    
	
    return ptr;
}
/*************************************************************************************/
/* pops the last element out of the sequence */
int SeqPop( Sequence *seq, void *element )
{
    char *ptr;
    int elem_size;   
	
    if( !seq )
        return -27 ;
    if( seq->total <= 0 )
        return -201 ;
	
    elem_size = seq->elem_size;
    seq->ptr = ptr = seq->ptr - elem_size;
	
    if( element )
        CV_MEMCPY_AUTO( element, ptr, elem_size );
    seq->ptr = ptr;
    seq->total--;
	
    if( --(seq->first->prev->count) == 0 )
    {
        FreeSeqBlock( seq, 0 );
        assert( seq->ptr == seq->block_max );
    }
	
   return 1 ;
}
/*************************************************************************************/
/* recycles a sequence block for the further use */
int FreeSeqBlock( Sequence *seq, int in_front_of )
{	
    SeqBlock *block = seq->first;
	
    assert( (in_front_of ? block : block->prev)->count == 0 );
	
    if( block == block->prev )  /* single block case */
    {
        block->count = (int)(seq->block_max - block->data) + block->start_index * seq->elem_size;
        block->data = seq->block_max - block->count;
        seq->first = 0;
        seq->ptr = seq->block_max = 0;
        seq->total = 0;
    }
    else
    {
        if( !in_front_of )
        {
            block = block->prev;
            assert( seq->ptr == block->data );
			
            block->count = (int)(seq->block_max - seq->ptr);
            seq->block_max = seq->ptr = block->prev->data +
                block->prev->count * seq->elem_size;
        }
        else
        {
            int delta = block->start_index;
			
            block->count = delta * seq->elem_size;
            block->data -= block->count;
			
            /* update start indices of sequence blocks */
            for( ;; )
            {
                block->start_index -= delta;
                block = block->next;
                if( block == seq->first )
                    break;
            }
			
            seq->first = block->next;
        }
		
        block->prev->next = block->next;
        block->next->prev = block->prev;
    }
	
    assert( block->count > 0 && block->count % seq->elem_size == 0 );
    block->next = seq->free_blocks;
    seq->free_blocks = block;
	return 1 ;
}
/*************************************************************************************/
Sequence* PointSeqFromMat( int seq_kind, const Arr* arr,
								 Contour* contour_header, SeqBlock* block )
{
    Sequence* contour = 0;    
	
    assert( arr != 0 && contour_header != 0 && block != 0 );	
    
    int eltype;
    Mat* mat = (Mat*)arr;	
    
    if( !CV_IS_MAT( mat ))
        return NULL ; 
	
//     eltype = CV_MAT_TYPE( mat->type );
//     if( eltype != CV_32SC2 && eltype != CV_32FC2 )
//         CV_ERROR( CV_StsUnsupportedFormat,
//         "The matrix can not be converted to point sequence because of "
//         "inappropriate element type" );
	
//     if( mat->width != 1 && mat->height != 1 || !CV_IS_MAT_CONT(mat->type))
//         CV_ERROR( CV_StsBadArg,
//         "The matrix converted to point sequence must be "
//         "1-dimensional and continuous" );
	
    MakeSeqHeaderForArray(
		(seq_kind & (CV_SEQ_KIND_MASK|CV_SEQ_FLAG_CLOSED)) | eltype,
		sizeof(Contour), CV_ELEM_SIZE(eltype), mat->data.ptr,
		mat->Cols() * mat->Rows(), (Sequence*)contour_header, block );
	
    contour = (Sequence*)contour_header;    
	
    return contour;
}
/*************************************************************************************/
/* constructs sequence from array without copying any data.
the resultant sequence can't grow above its initial size */
Sequence* MakeSeqHeaderForArray( int seq_flags, int header_size, int elem_size,
						void *array, int total, Sequence *seq, SeqBlock * block )
{
    Sequence* result = 0;    
	
    if( elem_size <= 0 || header_size < (int)sizeof( Sequence ) || total < 0 )
        return NULL ;
	
    if( !seq || ((!array || !block) && total > 0) )
        return NULL ;
	
    memset( seq, 0, header_size );
	
    seq->header_size = header_size;
    seq->flags = (seq_flags & ~CV_MAGIC_MASK) | CV_SEQ_MAGIC_VAL;
    {
        int elemtype = CV_MAT_TYPE(seq_flags);
        int typesize = CV_ELEM_SIZE(elemtype);
		
        if( elemtype != CV_SEQ_ELTYPE_GENERIC &&
            typesize != 0 && typesize != elem_size )
            return NULL ;
    }
    seq->elem_size = elem_size;
    seq->total = total;
    seq->block_max = seq->ptr = (char *) array + total * elem_size;
	
    if( total > 0 )
    {
        seq->first = block;
        block->prev = block->next = block;
        block->start_index = 0;
        block->count = total;
        block->data = (char *) array;
    }
	
    result = seq;    
	
    return result;
}
/*************************************************************************************/
int SliceLength( Slice slice, const Sequence* seq )
{
    int total = seq->total;
    int length = slice.end_index - slice.start_index;
    
    if( length != 0 )
    {
        if( slice.start_index < 0 )
            slice.start_index += total;
        if( slice.end_index <= 0 )
            slice.end_index += total;

        length = slice.end_index - slice.start_index;
    }

    if( length < 0 )
    {
        length += total;
        /*if( length < 0 )
            length += total;*/
    }
    else if( length > total )
        length = total;

    return length;
}
/*************************************************************************************/
/* area of a whole sequence */
int iContourArea( const Sequence* contour, double *area )
{
    if( contour->total )
    {
        SeqReader reader;
        int lpt = contour->total;
        double a00 = 0, xi_1, yi_1;
        int is_float = CV_SEQ_ELTYPE(contour) == CV_32FC2;
		
        StartReadSeq( contour, &reader, 0 );
		
        if( !is_float )
        {
            xi_1 = ((Point2i*)(reader.ptr))->x;
            yi_1 = ((Point2i*)(reader.ptr))->y;
        }
        else
        {
            xi_1 = ((Point2f*)(reader.ptr))->x;
            yi_1 = ((Point2f*)(reader.ptr))->y;
        }
        CV_NEXT_SEQ_ELEM( contour->elem_size, reader );
        
        while( lpt-- > 0 )
        {
            double dxy, xi, yi;
			
            if( !is_float )
            {
                xi = ((Point2i*)(reader.ptr))->x;
                yi = ((Point2i*)(reader.ptr))->y;
            }
            else
            {
                xi = ((Point2f*)(reader.ptr))->x;
                yi = ((Point2f*)(reader.ptr))->y;
            }
            CV_NEXT_SEQ_ELEM( contour->elem_size, reader );
			
            dxy = xi_1 * yi - xi * yi_1;
            a00 += dxy;
            xi_1 = xi;
            yi_1 = yi;
        }
		
        *area = a00 * 0.5;
    }
    else
        *area = 0;
	
    return 1;
}
/*************************************************************************************/
/* area of a contour sector */
int iContourSecArea( Sequence * contour, Slice slice, double *area )
{
    Point2i pt;                 /*  pointer to points   */
    Point2i pt_s, pt_e;         /*  first and last points  */
    SeqReader reader;         /*  points reader of contour   */

    int p_max = 2, p_ind;
    int lpt, flag, i;
    double a00;                 /* unnormalized moments m00    */
    double xi, yi, xi_1, yi_1, x0, y0, dxy, sk, sk1, t;
    double x_s, y_s, nx, ny, dx, dy, du, dv;
    double eps = 1.e-5;
    double *p_are1, *p_are2, *p_are;

    assert( contour != NULL );

    if( contour == NULL )
        return -2 ;

    if( !CV_IS_SEQ_POLYGON( contour ))
        return -12 ;

    lpt = SliceLength( slice, contour );
    /*if( n2 >= n1 )
        lpt = n2 - n1 + 1;
    else
        lpt = contour->total - n1 + n2 + 1;*/

    if( contour->total && lpt > 2 )
    {
        a00 = x0 = y0 = xi_1 = yi_1 = 0;
        sk1 = 0;
        flag = 0;
        dxy = 0;
        p_are1 = (double *) Alloc( p_max * sizeof( double ));

        if( p_are1 == NULL )
            return -3 ;

        p_are = p_are1;
        p_are2 = NULL;

        StartReadSeq( contour, &reader, 0 );
        SetSeqReaderPos( &reader, slice.start_index );
        CV_READ_SEQ_ELEM( pt_s, reader );
        p_ind = 0;
        SetSeqReaderPos( &reader, slice.end_index );
        CV_READ_SEQ_ELEM( pt_e, reader );

/*    normal coefficients    */
        nx = pt_s.y - pt_e.y;
        ny = pt_e.x - pt_s.x;
        SetSeqReaderPos( &reader, slice.start_index );

        while( lpt-- > 0 )
        {
            CV_READ_SEQ_ELEM( pt, reader );

            if( flag == 0 )
            {
                xi_1 = (double) pt.x;
                yi_1 = (double) pt.y;
                x0 = xi_1;
                y0 = yi_1;
                sk1 = 0;
                flag = 1;
            }
            else
            {
                xi = (double) pt.x;
                yi = (double) pt.y;

/****************   edges intersection examination   **************************/
                sk = nx * (xi - pt_s.x) + ny * (yi - pt_s.y);
                if( fabs( sk ) < eps && lpt > 0 || sk * sk1 < -eps )
                {
                    if( fabs( sk ) < eps )
                    {
                        dxy = xi_1 * yi - xi * yi_1;
                        a00 = a00 + dxy;
                        dxy = xi * y0 - x0 * yi;
                        a00 = a00 + dxy;

                        if( p_ind >= p_max )
                            iMemCopy( &p_are1, &p_are2, &p_are, &p_max );

                        p_are[p_ind] = a00 / 2.;
                        p_ind++;
                        a00 = 0;
                        sk1 = 0;
                        x0 = xi;
                        y0 = yi;
                        dxy = 0;
                    }
                    else
                    {
/*  define intersection point    */
                        dv = yi - yi_1;
                        du = xi - xi_1;
                        dx = ny;
                        dy = -nx;
                        if( fabs( du ) > eps )
                            t = ((yi_1 - pt_s.y) * du + dv * (pt_s.x - xi_1)) /
                                (du * dy - dx * dv);
                        else
                            t = (xi_1 - pt_s.x) / dx;
                        if( t > eps && t < 1 - eps )
                        {
                            x_s = pt_s.x + t * dx;
                            y_s = pt_s.y + t * dy;
                            dxy = xi_1 * y_s - x_s * yi_1;
                            a00 += dxy;
                            dxy = x_s * y0 - x0 * y_s;
                            a00 += dxy;
                            if( p_ind >= p_max )
                                iMemCopy( &p_are1, &p_are2, &p_are, &p_max );

                            p_are[p_ind] = a00 / 2.;
                            p_ind++;

                            a00 = 0;
                            sk1 = 0;
                            x0 = x_s;
                            y0 = y_s;
                            dxy = x_s * yi - xi * y_s;
                        }
                    }
                }
                else
                    dxy = xi_1 * yi - xi * yi_1;

                a00 += dxy;
                xi_1 = xi;
                yi_1 = yi;
                sk1 = sk;

            }
        }

        xi = x0;
        yi = y0;
        dxy = xi_1 * yi - xi * yi_1;

        a00 += dxy;

        if( p_ind >= p_max )
            iMemCopy( &p_are1, &p_are2, &p_are, &p_max );

        p_are[p_ind] = a00 / 2.;
        p_ind++;

/*     common area calculation    */
        *area = 0;
        for( i = 0; i < p_ind; i++ )
            (*area) += fabs( p_are[i] );

        if( p_are1 != NULL )
            cvFree( &p_are1 );
        else if( p_are2 != NULL )
            cvFree( &p_are2 );

        return 1 ;
    }
    else
        return -1 ;
}
/*************************************************************************************/
/* sets reader position to given absolute or relative
(relatively to the current one) position */
int SetSeqReaderPos( SeqReader* reader, int index, int is_relative )
{	
    SeqBlock *block;
    int elem_size, count, total;
	
    if( !reader || !reader->seq )
        return -27 ;
	
    total = reader->seq->total;
    elem_size = reader->seq->elem_size;
	
    if( !is_relative )
    {
        if( index < 0 )
        {
            if( index < -total )
                return -211 ;
            index += total;
        }
        else if( index >= total )
        {
            index -= total;
            if( index >= total )
                return -211 ;
        }
		
        block = reader->seq->first;
        if( index >= (count = block->count) )
        {
            if( index + index <= total )
            {
                do
                {
                    block = block->next;
                    index -= count;
                }
                while( index >= (count = block->count) );
            }
            else
            {
                do
                {
                    block = block->prev;
                    total -= block->count;
                }
                while( index < total );
                index -= total;
            }
        }
        reader->ptr = block->data + index * elem_size;
        if( reader->block != block )
        {
            reader->block = block;
            reader->block_min = block->data;
            reader->block_max = block->data + block->count * elem_size;
        }
    }
    else
    {
        char* ptr = reader->ptr;
        index *= elem_size;
        block = reader->block;
		
        if( index > 0 )
        {
            while( ptr + index >= reader->block_max )
            {
                int delta = (int)(reader->block_max - ptr);
                index -= delta;
                reader->block = block = block->next;
                reader->block_min = ptr = block->data;
                reader->block_max = block->data + block->count*elem_size;
            }
            reader->ptr = ptr + index;
        }
        else
        {
            while( ptr + index < reader->block_min )
            {
                int delta = (int)(ptr - reader->block_min);
                index += delta;
                reader->block = block = block->prev;
                reader->block_min = block->data;
                reader->block_max = ptr = block->data + block->count*elem_size;
            }
            reader->ptr = ptr + index;
        }
    }
    return 1 ;
}
/*************************************************************************************/
static int iMemCopy( double **buf1, double **buf2, double **buf3, int *b_max )
{
    int bb;

    if( *buf1 == NULL && *buf2 == NULL || *buf3 == NULL )
        return -2 ;

    bb = *b_max;
    if( *buf2 == NULL )
    {
        *b_max = 2 * (*b_max);
        *buf2 = (double *)Alloc( (*b_max) * sizeof( double ));

        if( *buf2 == NULL )
            return -3;

        memcpy( *buf2, *buf3, bb * sizeof( double ));

        *buf3 = *buf2;
        cvFree( buf1 );
        *buf1 = NULL;
    }
    else
    {
        *b_max = 2 * (*b_max);
        *buf1 = (double *) Alloc( (*b_max) * sizeof( double ));

        if( *buf1 == NULL )
            return -3 ;

        memcpy( *buf1, *buf3, bb * sizeof( double ));

        *buf3 = *buf1;
        cvFree( buf2 );
        *buf2 = NULL;
    }
    return 1;
}
/*************************************************************************************/
/* the version for integer point coordinates */
int iApproxPolyDP_32s( Sequence* src_contour, int header_size, MemStorage* storage, Sequence** dst_contour, float eps )
{
    int             init_iters = 3;
    Slice         slice, right_slice;
    SeqReader     reader, reader2;
    SeqWriter     writer;
    Point2i         start_pt(INT_MIN, INT_MIN), end_pt(0, 0), pt(0,0);
    int             i = 0, j, count = src_contour->total, new_count;
    int             is_closed = CV_IS_SEQ_CLOSED( src_contour );
    int             le_eps = 0;
    MemStorage*   temp_storage = 0;
    Sequence*          stack = 0;
    
    assert( CV_SEQ_ELTYPE(src_contour) == CV_32SC2 );
    StartWriteSeq( src_contour->flags, header_size, sizeof(pt), storage, &writer );

    if( src_contour->total == 0  )
    {
        *dst_contour = EndWriteSeq( &writer );
        return 1;
    }

    temp_storage = CreateChildMemStorage( storage );

    assert( src_contour->first != 0 );
    stack = CreateSeq( 0, sizeof(Sequence), sizeof(Slice), temp_storage );
    eps *= eps;
    StartReadSeq( src_contour, &reader, 0 );

    if( !is_closed )
    {
        right_slice.start_index = count;
        end_pt = *(Point2i*)(reader.ptr);
        start_pt = *(Point2i*)GetSeqElem( src_contour, -1 );

        if( start_pt.x != end_pt.x || start_pt.y != end_pt.y )
        {
            slice.start_index = 0;
            slice.end_index = count - 1;
            SeqPush( stack, &slice );
        }
        else
        {
            is_closed = 1;
            init_iters = 1;
        }
    }
    
    if( is_closed )
    {
        /* 1. Find approximately two farthest points of the contour */
        right_slice.start_index = 0;

        for( i = 0; i < init_iters; i++ )
        {
            int max_dist = 0;
            SetSeqReaderPos( &reader, right_slice.start_index, 1 );
            CV_READ_SEQ_ELEM( start_pt, reader );   /* read the first point */

            for( j = 1; j < count; j++ )
            {
                int dx, dy, dist;

                CV_READ_SEQ_ELEM( pt, reader );
                dx = pt.x - start_pt.x;
                dy = pt.y - start_pt.y;

                dist = dx * dx + dy * dy;

                if( dist > max_dist )
                {
                    max_dist = dist;
                    right_slice.start_index = j;
                }
            }

            le_eps = max_dist <= eps;
        }

        /* 2. initialize the stack */
        if( !le_eps )
        {
            slice.start_index = GetSeqReaderPos( &reader );
            slice.end_index = right_slice.start_index += slice.start_index;

            right_slice.start_index -= right_slice.start_index >= count ? count : 0;
            right_slice.end_index = slice.start_index;
            if( right_slice.end_index < right_slice.start_index )
                right_slice.end_index += count;

            SeqPush( stack, &right_slice );
            SeqPush( stack, &slice );
        }
        else
            CV_WRITE_SEQ_ELEM( start_pt, writer );
    }

    /* 3. run recursive process */
    while( stack->total != 0 )
    {
        SeqPop( stack, &slice );

        if( slice.end_index > slice.start_index + 1 )
        {
            int dx, dy, dist, max_dist = 0;
            
            SetSeqReaderPos( &reader, slice.end_index );
            CV_READ_SEQ_ELEM( end_pt, reader );

            SetSeqReaderPos( &reader, slice.start_index );
            CV_READ_SEQ_ELEM( start_pt, reader );

            dx = end_pt.x - start_pt.x;
            dy = end_pt.y - start_pt.y;

            assert( dx != 0 || dy != 0 );

            for( i = slice.start_index + 1; i < slice.end_index; i++ )
            {
                CV_READ_SEQ_ELEM( pt, reader );
                dist = abs((pt.y - start_pt.y) * dx - (pt.x - start_pt.x) * dy);

                if( dist > max_dist )
                {
                    max_dist = dist;
                    right_slice.start_index = i;
                }
            }

            le_eps = (double)max_dist * max_dist <= eps * ((double)dx * dx + (double)dy * dy);
        }
        else
        {
            assert( slice.end_index > slice.start_index );
            le_eps = 1;
            /* read starting point */
            SetSeqReaderPos( &reader, slice.start_index );
            CV_READ_SEQ_ELEM( start_pt, reader );
        }

        if( le_eps )
        {
            CV_WRITE_SEQ_ELEM( start_pt, writer );
        }
        else
        {
            right_slice.end_index = slice.end_index;
            slice.end_index = right_slice.start_index;
            SeqPush( stack, &right_slice );
            SeqPush( stack, &slice );
        }
    }

    is_closed = CV_IS_SEQ_CLOSED( src_contour );
    if( !is_closed )
        CV_WRITE_SEQ_ELEM( end_pt, writer );

    *dst_contour = EndWriteSeq( &writer );
    
    StartReadSeq( *dst_contour, &reader, is_closed );
    CV_READ_SEQ_ELEM( start_pt, reader );

    reader2 = reader;
    CV_READ_SEQ_ELEM( pt, reader );

    new_count = count = (*dst_contour)->total;
    for( i = !is_closed; i < count - !is_closed && new_count > 2; i++ )
    {
        int dx, dy, dist;
        CV_READ_SEQ_ELEM( end_pt, reader );

        dx = end_pt.x - start_pt.x;
        dy = end_pt.y - start_pt.y;
        dist = abs((pt.x - start_pt.x)*dy - (pt.y - start_pt.y)*dx);
        if( (double)dist * dist <= 0.5*eps*((double)dx*dx + (double)dy*dy) && dx != 0 && dy != 0 )
        {
            new_count--;
            *((Point2i*)reader2.ptr) = start_pt = end_pt;
            CV_NEXT_SEQ_ELEM( sizeof(pt), reader2 );
            CV_READ_SEQ_ELEM( pt, reader );
            i++;
            continue;
        }
        *((Point2i*)reader2.ptr) = start_pt = pt;
        CV_NEXT_SEQ_ELEM( sizeof(pt), reader2 );
        pt = end_pt;
    }

    if( !is_closed )
        *((Point2i*)reader2.ptr) = pt;

    if( new_count < count )
        SeqPopMulti( *dst_contour, 0, count - new_count, 0 );

    ReleaseMemStorage( &temp_storage );

    return 1;
}
/*************************************************************************************/
/* returns the current reader position */
int GetSeqReaderPos( SeqReader* reader )
{
    int elem_size;
    int index = -1;	
    
    if( !reader || !reader->ptr )
        return -27 ;
	
    elem_size = reader->seq->elem_size;
    if( elem_size <= ICV_SHIFT_TAB_MAX && (index = icvPower2ShiftTab[elem_size - 1]) >= 0 )
        index = (int)((reader->ptr - reader->block_min) >> index);
    else
        index = (int)((reader->ptr - reader->block_min) / elem_size);
	
    index += reader->block->start_index - reader->delta_index;    
	
    return index;
}
/*************************************************************************************/
/* removes several elements from the end of sequence */
void SeqPopMulti( Sequence *seq, void *_elements, int count, int front )
{
    char *elements = (char *) _elements;    
	
    if( !seq )
        assert(false);
    if( count < 0 )
		assert(false);
	
    count = MIN( count, seq->total );
	
    if( !front )
    {
        if( elements )
            elements += count * seq->elem_size;
		
        while( count > 0 )
        {
            int delta = seq->first->prev->count;
			
            delta = MIN( delta, count );
            assert( delta > 0 );
			
            seq->first->prev->count -= delta;
            seq->total -= delta;
            count -= delta;
            delta *= seq->elem_size;
            seq->ptr -= delta;
			
            if( elements )
            {
                elements -= delta;
                memcpy( elements, seq->ptr, delta );
            }
			
            if( seq->first->prev->count == 0 )
                FreeSeqBlock( seq, 0 );
        }
    }
    else
    {
        while( count > 0 )
        {
            int delta = seq->first->count;
			
            delta = MIN( delta, count );
            assert( delta > 0 );
			
            seq->first->count -= delta;
            seq->total -= delta;
            count -= delta;
            seq->first->start_index += delta;
            delta *= seq->elem_size;
			
            if( elements )
            {
                memcpy( elements, seq->first->data, delta );
                elements += delta;
            }
			
            seq->first->data += delta;
            if( seq->first->count == 0 )
                FreeSeqBlock( seq, 1 );
        }
    }
}
/*************************************************************************************/
void Threshold_Contour( Mat* src, Mat* dst, double thresh, double maxval )
{
	int nW = src->Cols() ;
	int nH = src->Rows() ;

	if( src->Cols() != dst->Cols() || src->Rows() != dst->Rows() )
	{
		if( dst != NULL )
			dst->Release() ;
		
		dst->Create( nH, nW, MAT_Tdouble ) ;
	}
	
	BYTE* prSrc = src->data.ptr[0] ;
	BYTE* prDst = dst->data.ptr[0] ;

	for( int i = 0 ; i < nW * nH ; i ++ )
	{
		if( prSrc[i] < thresh )
			prDst[i] = 0 ;
		else
			prDst[i] = 255 ;
	}
}

/*===================================================================================*/
//////////////////////////////////////////////////////////////////////////
//End Process Session
/*************************************************************************************/
float GetEdgeValue( Mat* pmEdge, Vector<Point2i>& rect)
{
	float rGraySum = 0;
	int nLenSum = 0;
	for(int i = 0; i <4; i++)
	{
		//CvLineIterator iterator;
        Point2i startpt = rect[i];
        Point2i endpt = rect[(i + 1) % 4];
		int nCount = 0;
		int *pnX;
		int *pnY;
		ip::Bresenham(startpt.x, startpt.y, endpt.x, endpt.y, pnX, pnY, nCount);

		BYTE** ppbEdge = pmEdge->data.ptr ;
		
        
		for(int j = 0; j < nCount; j++)
		{
			if (!pmEdge->IsInterior(pnX[j], pnY[j]))
				continue;
			rGraySum += ppbEdge[pnY[j]][pnX[j]];
		}
        
		delete pnX;
		delete pnY;
		nLenSum += nCount;
	}
    
	return rGraySum / nLenSum;
}
int FindContours( Mat* img, Matrix<Point2i>& contours, int mode, int method, Point2i offset )
{
	Sequence* contoursSeq = 0;
	MemStorage* storage = CreateMemStorage( 0 ) ;    
	int nret=FindContours(img, storage, &contoursSeq, mode, method, offset);
	Sequence* cnt=contoursSeq;
	for(;cnt;cnt=cnt->h_next)
	{
		SeqBlock* pBlock=cnt->first;
		Vector<Point2i> contour;
		contour.SetSize(pBlock->count);
		SPoint* pPoint=(SPoint*)pBlock->data;
		for (int k=0; k<pBlock->count; k++)
			contour[k]=pPoint[k];
		contours.Add(contour);
	}
	ReleaseMemStorage( &storage );
	return nret;
}

}