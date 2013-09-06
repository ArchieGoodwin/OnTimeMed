#include "Canny.h"
#include "cvlibbase/inc/_cvlibbase.h"
#include <assert.h>
#include <math.h>

namespace CVLib
{

void	Canny::Create (int nSizeX, int nSizeY)
{
	assert (!m_mGauss.IsValid());
	m_mTemp.Create (nSizeY, nSizeX, MAT_Tshort);
}

void	Canny::Release ()
{
	m_mTemp.Release();
}

/**
 @brief		ËÓµÙÃùº¬Ëæ ´ÝÂÙ CannyËæ¼è°Á¾¥Ë¾ ¼êÃÔÂÙ³Þ.
 @param		pmIn	[inout]	¾¥µÙÌ°¾©±¨Ì© ±¨ÊÍ±¨°¡ ÂÚ³ë´óÊÞ ËØ¼è Ê±³Þ·² °Î±áÃùº¬ËË ½å¶®Ã¨Ëæ ¸ó±ã´ó¼è¶í 
 @param		pmOut	[out]	¾¥µÙÃùº¬ËË ¸ó±ã´ó²÷ ±¨ÊÍ±¨Ì© ¼ÑºÏ
 */
void	Canny::Process (Mat* pmIn, Mat* pmOut)
{
	if (pmIn->Channels()==1)
		cannygray (pmIn, pmOut);
	else
		cannycolor (pmIn, pmOut);
}
void	Canny::cannygray (Mat* pmIn, Mat* pmOut)
{
	assert (pmIn->Type() == MAT_Tbyte);

	int nWidth = pmIn->Cols();
	int	nHeight = pmIn->Rows();
	BYTE**	ppbIn = pmIn->data.ptr;
	BYTE**	ppbOut = pmOut ? pmOut->data.ptr: ppbIn;

	m_mGauss.Create (nHeight, nWidth, MAT_Tshort);
	short**	ppsGauss = m_mGauss.data.s;

	int		iH, iW;
	int			nSumX, nSumY, nSUM;
	int			nLeftPixel, nRightPixel;
	float		rORIENT;
	int			nEdgeDirection;
	int			nHighThreshold = m_nHighThreshold, nLowThreshold = m_nLowThreshold;
	memset(ppbOut[0], 0, sizeof(BYTE) * nHeight * nWidth);
	for(iH = 0; iH < nHeight*nWidth*pmIn->Channels(); iH ++) 
		ppsGauss[0][iH]=ppbIn[0][iH];

	/***********************************************
	*	SOBEL GRADIENT APPROXIMATION
	***********************************************/
	float rConst = (180.0f/(float)CVLIB_PI);
	for (iH = 1; iH < nHeight-1; iH++)  
	{
		short* psG_1=ppsGauss[iH-1];
		short* psG=ppsGauss[iH];
		short* psG1=ppsGauss[iH+1];
		BYTE* pbout=ppbOut[iH];
		for(iW = 1; iW < nWidth-1; iW++)  
		{
			nSumX = 0;
			nSumY = 0;

			{
				/***********************************
				* X gradient approximation
				***********************************/
				nSumX = psG_1[iW+1]+2*psG[iW +1]+psG1[iW+1]
					-psG_1[iW -1]-2*psG[iW -1]-psG1[iW-1];
				
				/**************************
				* Y gradient approximation
				**************************/
				nSumY = psG_1[iW+1]+2*psG_1[iW]+psG_1[iW-1]
					-psG1[iW+1]-2*psG1[iW]-psG1[iW-1];

				/***********************************************
				* GRADIENT MAGNITUDE APPROXIMATION
				***********************************************/
				nSUM = abs(nSumX) + abs(nSumY);

				if(nSUM>255)    nSUM=255;
				if(nSUM<0)      nSUM=0;

				/***************************
				* Magnitude orientation
				***************************/
				/* Cannot divide by zero*/
				if(nSumX == 0)   
				{
					if(nSumY==0) rORIENT = 0.0;
					else if (nSumY<0)   
					{
						nSumY = -nSumY;
						rORIENT = 90.0;
					}
					else rORIENT = 90.0;
				}
				/* Can't take invtan of angle in 2nd Quad */
				else if( nSumX < 0 && nSumY > 0)   
				{
					nSumX = -nSumX;
					rORIENT = 180.0f - (float)((atan((float)(nSumY)/(float)(nSumX))) * rConst);
				}

				/* Can't take invtan of angle in 4th Quad */
				else if(nSumX>0 && nSumY<0)   
				{
					nSumY = -nSumY;
					rORIENT = 180.0f - (float)((atan((float)(nSumY)/(float)(nSumX))) * rConst);
				}

				/* else angle is in 1st or 3rd Quad */
				else 
					rORIENT = (float)(atan((float)(nSumY)/(float)(nSumX))) * rConst;

				/***************************************************
				* Find nEdgeDirection by assigning rORIENT a value of
				* either 0, 45, 90 or 135 degrees, depending on which
				* value rORIENT is closest to
				****************************************************/
				if(rORIENT < 22.5) nEdgeDirection = 0;
				else if(rORIENT < 67.5) nEdgeDirection = 45;
				else if(rORIENT < 112.5) nEdgeDirection = 90;
				else if(rORIENT < 157.5) nEdgeDirection = 135;
				else nEdgeDirection = 0;

				/***************************************************
				* Obtain values of 2 adjacent pixels in edge
				* direction.
				****************************************************/
				switch (nEdgeDirection)
				{
				case 0:
					nLeftPixel = (int)psG[iW - 1];
					nRightPixel = (int)psG[iW + 1];
					break;
				case 45:
					nLeftPixel = (int)psG1[iW - 1];
					nRightPixel = (int)psG_1[iW + 1];
					break;
				case 90:
					nLeftPixel = (int)psG_1[iW];
					nRightPixel = (int)psG1[iW];
					break;
				default:
					nLeftPixel = (int)psG_1[iW - 1];
					nRightPixel = (int)psG1[iW + 1];
					break;
				}

				/*********************************************
				* Compare current magnitude to both adjacent
				* pixel values.  And if it is less than either
				* of the 2 adjacent values - suppress it and make
				* a nonedge.
				*********************************************/

				if (nSUM < nLeftPixel || nSUM < nRightPixel) 
					nSUM = 0;
				else   
				{
					/**********************
					* Hysteresis
					**********************/
					if(nSUM >= nHighThreshold)
						nSUM = 255; /* edge */
					else if(nSUM < nLowThreshold)
						nSUM = 0;  /* nonedge */

					/* nSUM is between T1 & T2 */
					else   
					{

					/* Check to see if neighboring pixel values are edges */
						if(	psG_1[iW - 1] > nHighThreshold || 
							psG_1[iW] > nHighThreshold || 
							psG_1[iW - 1] > nHighThreshold || 
							psG[iW - 1] > nHighThreshold || 
							psG[iW + 1] > nHighThreshold || 
							psG1[iW - 1] > nHighThreshold || 
							psG1[iW] > nHighThreshold || 
							psG1[iW + 1] > nHighThreshold)
							nSUM = 255; /* make edge */
						else 
							nSUM = 0; /* make nonedge */
					}
				}
			} /* else loop ends here (starts after b.iW.) */
			pbout[iW] = 255 - (BYTE)(nSUM);
		}
	}
	m_mGauss.Release();
}
void	Canny::cannycolor (Mat* pmIn, Mat* pmOut)
{
	assert (pmIn->Type1()==MAT_Tbyte3);
	assert (pmOut->Type1()==MAT_Tbyte);
	int nWidth = pmIn->Cols();
	int	nHeight = pmIn->Rows();
	BYTE**	ppbIn = pmIn->data.ptr;
	BYTE**	ppbOut = pmOut ? pmOut->data.ptr: ppbIn;

	m_mGauss.Create (nHeight, nWidth, MAT_Tshort3);
	short**	ppsGauss = m_mGauss.data.s;

	int		iH, iW;
	int			nSumX1, nSumY1;
	int			nSumX2, nSumY2;
	int			nSumX3, nSumY3, nSUM;
	int			nLeftPixel, nRightPixel;
	float		rORIENT;
	int			nEdgeDirection;
	int			nHighThreshold = m_nHighThreshold, nLowThreshold = m_nLowThreshold;
	memset(ppbOut[0], 0, sizeof(BYTE) * nHeight * nWidth);

	for(iH = 0; iH < nHeight*nWidth*pmIn->Channels(); iH ++) 
		ppsGauss[0][iH]=ppbIn[0][iH];

	/***********************************************
	*	SOBEL GRADIENT APPROXIMATION
	***********************************************/
	float rConst = (180.0f/(float)CVLIB_PI);
	for (iH = 1; iH < nHeight-1; iH++)  
	{
		short* psG_1=ppsGauss[iH-1];
		short* psG=ppsGauss[iH];
		short* psG1=ppsGauss[iH+1];
		BYTE* pbout=ppbOut[iH];
		int x=3;
		for(iW = 1; iW < nWidth-1; iW++,x+=3)
		{
			nSumX1 = 0;
			nSumY1 = 0;

			{
				/***********************************
				* X gradient approximation
				***********************************/
				nSumX1 = psG_1[x+3]+2*psG[x+3]+psG1[x+3]-psG_1[x-3]-2*psG[x-3]-psG1[x-3];
				nSumX2 = psG_1[x+4]+2*psG[x+4]+psG1[x+4]-psG_1[x-2]-2*psG[x-2]-psG1[x-2];
				nSumX3 = psG_1[x+5]+2*psG[x+5]+psG1[x+5]-psG_1[x-1]-2*psG[x-1]-psG1[x-1];
				
				/**************************
				* Y gradient approximation
				**************************/
				nSumY1 = psG_1[x+3]+2*psG_1[x]+psG_1[x-3]-psG1[x+3]-2*psG1[x]-psG1[x-3];
				nSumY2 = psG_1[x+4]+2*psG_1[x+1]+psG_1[x-2]-psG1[x+4]-2*psG1[x+1]-psG1[x-2];
				nSumY3 = psG_1[x+5]+2*psG_1[x+2]+psG_1[x-1]-psG1[x+5]-2*psG1[x+2]-psG1[x-1];
				int nSUM1=abs(nSumX1)+abs(nSumY1);
				int nSUM2=abs(nSumX2)+abs(nSumY2);
				int nSUM3=abs(nSumX3)+abs(nSumY3);
				/***********************************************
				* GRADIENT MAGNITUDE APPROXIMATION
				***********************************************/
				int nSumX, nSumY, nx;
				if (nSUM1>nSUM2)
				{
					if (nSUM2>nSUM3 || nSUM1>nSUM3)
					{
						nSUM=nSUM1; nSumX=nSumX1; nSumY=nSumY1; nx=0;
					}
					else
					{
						nSUM=nSUM3; nSumX=nSumX3; nSumY=nSumY3; nx=2;
					}
				}
				else
				{
					if (nSUM3<nSUM1 || nSUM3<nSUM2)
					{
						nSUM=nSUM2; nSumX=nSumX2; nSumY=nSumY2; nx=1;
					}
					else
					{
						nSUM=nSUM3; nSumX=nSumX3; nSumY=nSumY3; nx=2;
					}
				}

				if(nSUM>255)    nSUM=255;
				if(nSUM<0)      nSUM=0;

				/***************************
				* Magnitude orientation
				***************************/
				/* Cannot divide by zero*/
				if(nSumX == 0)   
				{
					if(nSumY==0) rORIENT = 0.0;
					else if (nSumY<0)   
					{
						nSumY = -nSumY;
						rORIENT = 90.0;
					}
					else rORIENT = 90.0;
				}
				/* Can't take invtan of angle in 2nd Quad */
				else if( nSumX < 0 && nSumY > 0)   
				{
					nSumX = -nSumX;
					rORIENT = 180.0f - (float)((atan((float)(nSumY)/(float)(nSumX))) * rConst);
				}

				/* Can't take invtan of angle in 4th Quad */
				else if(nSumX>0 && nSumY<0)   
				{
					nSumY = -nSumY;
					rORIENT = 180.0f - (float)((atan((float)(nSumY)/(float)(nSumX))) * rConst);
				}

				/* else angle is in 1st or 3rd Quad */
				else 
					rORIENT = (float)(atan((float)(nSumY)/(float)(nSumX))) * rConst;

				/***************************************************
				* Find nEdgeDirection by assigning rORIENT a value of
				* either 0, 45, 90 or 135 degrees, depending on which
				* value rORIENT is closest to
				****************************************************/
				if(rORIENT < 22.5) nEdgeDirection = 0;
				else if(rORIENT < 67.5) nEdgeDirection = 45;
				else if(rORIENT < 112.5) nEdgeDirection = 90;
				else if(rORIENT < 157.5) nEdgeDirection = 135;
				else nEdgeDirection = 0;

				/***************************************************
				* Obtain values of 2 adjacent pixels in edge
				* direction.
				****************************************************/
				switch (nEdgeDirection)
				{
				case 0:
					nLeftPixel = (int)psG[x-3+nx];
					nRightPixel = (int)psG[x+3+nx];
					break;
				case 45:
					nLeftPixel = (int)psG1[x-3+nx];
					nRightPixel = (int)psG_1[x+3+nx];
					break;
				case 90:
					nLeftPixel = (int)psG_1[x+nx];
					nRightPixel = (int)psG1[x+nx];
					break;
				default:
					nLeftPixel = (int)psG_1[x-3+nx];
					nRightPixel = (int)psG1[x+3+nx];
					break;
				}

				/*********************************************
				* Compare current magnitude to both adjacent
				* pixel values.  And if it is less than either
				* of the 2 adjacent values - suppress it and make
				* a nonedge.
				*********************************************/

				if (nSUM < nLeftPixel || nSUM < nRightPixel) 
					nSUM = 0;
				else   
				{
					/**********************
					* Hysteresis
					**********************/
					if(nSUM >= nHighThreshold)
						nSUM = 255; /* edge */
					else if(nSUM < nLowThreshold)
						nSUM = 0;  /* nonedge */

					/* nSUM is between T1 & T2 */
					else   
					{

					/* Check to see if neighboring pixel values are edges */
						if(	psG_1[x-3+nx+nx] > nHighThreshold || 
							psG_1[x+nx] > nHighThreshold || 
							psG_1[x-3+nx] > nHighThreshold || 
							psG[x-3+nx] > nHighThreshold || 
							psG[x+3+nx] > nHighThreshold || 
							psG1[x-3+nx] > nHighThreshold || 
							psG1[x+nx] > nHighThreshold || 
							psG1[x+3+nx] > nHighThreshold)
							nSUM = 255; /* make edge */
						else 
							nSUM = 0; /* make nonedge */
					}
				}
			} /* else loop ends here (starts after b.iW.) */
			pbout[iW] = 255 - (BYTE)(nSUM);
		}
	}
	m_mGauss.Release();
}

namespace ip
{
void canny (const Mat& src, Mat& dst, float low_thresh, float high_thresh, int mode)
{
	bool L2gradient = false;
	int nWidth = src.Cols();
	int	nHeight = src.Rows();
	BYTE**	ppbIn = src.data.ptr;

	int			iH, iW;
	int			high = (int)high_thresh, low = (int)low_thresh;
	dst.Create (nHeight, nWidth, MAT_Tbyte);
	memset(dst.data.ptr[0], 0, sizeof(BYTE) * nHeight * nWidth);
    const int cn = src.Channels();
    Mat dx(src.Rows(), src.Cols(), (TYPE)CVLIB_MAKETYPE(MAT_Tshort, cn));
    Mat dy(src.Rows(), src.Cols(), (TYPE)CVLIB_MAKETYPE(MAT_Tshort, cn));
	if (cn==3)
	{
		for (iH = 1; iH < nHeight-1; iH++)  
		{
			short* psX=dx.data.s[iH];
			short* psY=dy.data.s[iH];
			uchar* psG_1=ppbIn[iH-1];
			uchar* psG=ppbIn[iH];
			uchar* psG1=ppbIn[iH+1];
			int x=3;
			for(iW = 1; iW < nWidth-1; iW++,x+=3)
			{
				/***********************************
				* X gradient approximation
				***********************************/
				if (mode ==0 || mode==2)
				{
					psX[x] = psG_1[x+3]+2*psG[x+3]+psG1[x+3]-psG_1[x-3]-2*psG[x-3]-psG1[x-3];
					psX[x+1] = psG_1[x+4]+2*psG[x+4]+psG1[x+4]-psG_1[x-2]-2*psG[x-2]-psG1[x-2];
					psX[x+2] = psG_1[x+5]+2*psG[x+5]+psG1[x+5]-psG_1[x-1]-2*psG[x-1]-psG1[x-1];
				}
				else
				{
					psX[x]=psX[x+1]=psX[x+2]=0;
				}
				/**************************
				* Y gradient approximation
				**************************/
				if (mode ==0 || mode==1)
				{
					psY[x] = psG_1[x+3]+2*psG_1[x]+psG_1[x-3]-psG1[x+3]-2*psG1[x]-psG1[x-3];
					psY[x+1] = psG_1[x+4]+2*psG_1[x+1]+psG_1[x-2]-psG1[x+4]-2*psG1[x+1]-psG1[x-2];
					psY[x+2] = psG_1[x+5]+2*psG_1[x+2]+psG_1[x-1]-psG1[x+5]-2*psG1[x+2]-psG1[x-1];
				}
				else
				{
					psY[x]=psY[x+1]=psY[x+2]=0;
				}
			}
		}
	}
	else
	{
		for (iH = 1; iH < nHeight-1; iH++)  
		{
			short* psX=dx.data.s[iH];
			short* psY=dy.data.s[iH];
			uchar* psG_1=ppbIn[iH-1];
			uchar* psG=ppbIn[iH];
			uchar* psG1=ppbIn[iH+1];
			int x=1;
			for(iW = 1; iW < nWidth-1; iW++,x++)
			{
				/***********************************
				* X gradient approximation
				***********************************/
				if (mode ==0 || mode==2)
				{
					psX[x] = psG_1[x+1]+2*psG[x+1]+psG1[x+1]-psG_1[x-1]-2*psG[x-1]-psG1[x-1];
				}
				else
				{
					psX[x]=0;
				}
				/**************************
				* Y gradient approximation
				**************************/
				if (mode ==0 || mode==1)
				{
					psY[x] = psG_1[x+1]+2*psG_1[x]+psG_1[x-1]-psG1[x+1]-2*psG1[x]-psG1[x-1];
				}
				else
				{
					psY[x]=0;
				}
			}
		}
	}
	for (iH=0; iH<nHeight; iH++)
	{
		dx.data.s[iH][0]=dx.data.s[iH][1];
		dx.data.s[iH][nWidth-1]=dx.data.s[iH][nWidth-2];
	}
	for (iW=0; iW<nWidth; iW++)
	{
		dx.data.s[0][iW]=dx.data.s[1][iW];
		dx.data.s[nHeight-1][iW]=dx.data.s[nHeight-2][iW];
	}

    ptrdiff_t mapstep = src.Cols() + 2;
    CVLib::AutoBuffer<uchar> buffer((src.Cols()+2)*(src.Rows()+2) + cn * mapstep * 3 * sizeof(int));

    int* mag_buf[3];
    mag_buf[0] = (int*)(uchar*)buffer;
    mag_buf[1] = mag_buf[0] + mapstep*cn;
    mag_buf[2] = mag_buf[1] + mapstep*cn;
    memset(mag_buf[0], 0, /* cn* */mapstep*sizeof(int));

    uchar* map = (uchar*)(mag_buf[2] + mapstep*cn);
    memset(map, 1, mapstep);
    memset(map + mapstep*(src.Rows() + 1), 1, mapstep);

    int maxsize = MAX(1 << 10, src.Cols() * src.Rows() / 10);
    Vector<uchar*> stack(maxsize);
    uchar **stack_top = &stack[0];
    uchar **stack_bottom = &stack[0];
	int i;
    /* sector numbers
       (Top-Left Origin)

        1   2   3
         *  *  *
          * * *
        0*******0
          * * *
         *  *  *
        3   2   1
    */

    #define CANNY_PUSH(d)    *(d) = uchar(2), *stack_top++ = (d)
    #define CANNY_POP(d)     (d) = *--stack_top

    // calculate magnitude and angle of gradient, perform non-maxima supression.
    // fill the map with one of the following values:
    //   0 - the pixel might belong to an edge
    //   1 - the pixel can not belong to an edge
    //   2 - the pixel does belong to an edge
    for ( i = 0; i <= src.Rows(); i++)
    {
        int* _norm = mag_buf[(i > 0) + 1] + 1;
        if (i < src.Rows())
        {
            short* _dx = dx.data.s[i];
            short* _dy = dy.data.s[i];

            if (!L2gradient)
            {
                for (int j = 0; j < src.Cols()*cn; j++)
                    _norm[j] = abs(int(_dx[j])) + abs(int(_dy[j]));
            }
            else
            {
                for (int j = 0; j < src.Cols()*cn; j++)
                    _norm[j] = int(_dx[j])*_dx[j] + int(_dy[j])*_dy[j];
            }

            if (cn > 1)
            {
                for(int j = 0, jn = 0; j < src.Cols(); ++j, jn += cn)
                {
                    int maxIdx = jn;
                    for(int k = 1; k < cn; ++k)
                        if(_norm[jn + k] > _norm[maxIdx]) maxIdx = jn + k;
                    _norm[j] = _norm[maxIdx];
                    _dx[j] = _dx[maxIdx];
                    _dy[j] = _dy[maxIdx];
                }
            }
            _norm[-1] = _norm[src.Cols()] = 0;
        }
        else
            memset(_norm-1, 0, /* cn* */mapstep*sizeof(int));

        // at the very beginning we do not have a complete ring
        // buffer of 3 magnitude Rows() for non-maxima suppression
        if (i == 0)
            continue;

        uchar* _map = map + mapstep*i + 1;
        _map[-1] = _map[src.Cols()] = 1;

        int* _mag = mag_buf[1] + 1; // take the central row
        ptrdiff_t magstep1 = mag_buf[2] - mag_buf[1];
        ptrdiff_t magstep2 = mag_buf[0] - mag_buf[1];

        const short* _x = dx.data.s[i-1];
        const short* _y = dy.data.s[i-1];

        if ((stack_top - stack_bottom) + src.Cols() > maxsize)
        {
            int sz = (int)(stack_top - stack_bottom);
            maxsize = maxsize * 3/2;
            stack.SetSize(maxsize);
            stack_bottom = &stack[0];
            stack_top = stack_bottom + sz;
        }

        int prev_flag = 0;
        for (int j = 0; j < src.Cols(); j++)
        {
            #define CANNY_SHIFT 15
            const int TG22 = (int)(0.4142135623730950488016887242097*(1<<CANNY_SHIFT) + 0.5);

            int m = _mag[j];

            if (m > low)
            {
                int xs = _x[j];
                int ys = _y[j];
                int x = abs(xs);
                int y = abs(ys) << CANNY_SHIFT;

                int tg22x = x * TG22;

                if (y < tg22x)
                {
                    if (m > _mag[j-1] && m >= _mag[j+1]) goto __ocv_canny_push;
                }
                else
                {
                    int tg67x = tg22x + (x << (CANNY_SHIFT+1));
                    if (y > tg67x)
                    {
                        if (m > _mag[j+magstep2] && m >= _mag[j+magstep1]) goto __ocv_canny_push;
                    }
                    else
                    {
                        int s = (xs ^ ys) < 0 ? -1 : 1;
                        if (m > _mag[j+magstep2-s] && m > _mag[j+magstep1+s]) goto __ocv_canny_push;
                    }
                }
            }
            prev_flag = 0;
            _map[j] = uchar(1);
            continue;
__ocv_canny_push:
            if (!prev_flag && m > high && _map[j-mapstep] != 2)
            {
                CANNY_PUSH(_map + j);
                prev_flag = 1;
            }
            else
                _map[j] = 0;
        }

        // scroll the ring buffer
        _mag = mag_buf[0];
        mag_buf[0] = mag_buf[1];
        mag_buf[1] = mag_buf[2];
        mag_buf[2] = _mag;
    }

    // now track the edges (hysteresis thresholding)
    while (stack_top > stack_bottom)
    {
        uchar* m;
        if ((stack_top - stack_bottom) + 8 > maxsize)
        {
            int sz = (int)(stack_top - stack_bottom);
            maxsize = maxsize * 3/2;
            stack.SetSize(maxsize);
            stack_bottom = &stack[0];
            stack_top = stack_bottom + sz;
        }

        CANNY_POP(m);

        if (!m[-1])         CANNY_PUSH(m - 1);
        if (!m[1])          CANNY_PUSH(m + 1);
        if (!m[-mapstep-1]) CANNY_PUSH(m - mapstep - 1);
        if (!m[-mapstep])   CANNY_PUSH(m - mapstep);
        if (!m[-mapstep+1]) CANNY_PUSH(m - mapstep + 1);
        if (!m[mapstep-1])  CANNY_PUSH(m + mapstep - 1);
        if (!m[mapstep])    CANNY_PUSH(m + mapstep);
        if (!m[mapstep+1])  CANNY_PUSH(m + mapstep + 1);
    }

    // the final pass, form the final image
    const uchar* pmap = map + mapstep + 1;
    for (i = 0; i < src.Rows(); i++, pmap += mapstep)
    {
	    uchar* pdst = dst.data.ptr[i];
        for (int j = 0; j < src.Cols(); j++)
            pdst[j] = (uchar)-(pmap[j] >> 1);
    }
	for (i=0; i<src.Rows(); i++)
		dst.data.ptr[i][0]=dst.data.ptr[i][src.Cols()-1]=0;
	for (i=0; i<src.Cols(); i++)
		dst.data.ptr[0][i]=dst.data.ptr[src.Rows()-1][i]=0;
}

}

}
