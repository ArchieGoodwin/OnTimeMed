/*!
 * \file	ipTransformation.cpp
 * \ingroup ipCore
 * \brief   Ãùº¬¸êÃûË¾ ¼êÃÔÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§ 
 */
#include "ipTransformation.h"
#include "cvlibbase/inc/MatOperation.h"
#include "cvlibbase/inc/cvlibutil.h"
#include "cvlibbase/inc/LUDecomposition.h"

namespace CVLib
{

IMPLEMENT_PUMP(ipTransformation, ipCorePump)
IMPLEMENT_PUMP(ipResample, ipTransformation);
IMPLEMENT_PUMP(ipCrop, ipTransformation);
IMPLEMENT_PUMP(ipSkew, ipTransformation)
IMPLEMENT_PUMP(ipExpand, ipTransformation)
IMPLEMENT_PUMP(ipThumbnail, ipTransformation)
IMPLEMENT_PUMP(ipTransGeo, ipTransformation)
IMPLEMENT_PUMP(ipRotate, ipTransformation);

/**
 * Recalculates coordinates according to specified overflow method.
 * If pixel (x,y) lies within image, nothing changes.
 *
 *  \param x, y - coordinates of pixel
 *  \param ofMethod - overflow method
 * 
 *  \return x, y - new coordinates (pixel (x,y) now lies inside image)
 *
 *  \author ***bd*** 2.2004
 */
void ipTransformation::OverflowCoordinates(CoImage* pSrc, long &x, long &y, OverflowMethod const ofMethod)
{
  if (pSrc->IsInterior(x,y)) return;  //if pixel is within bounds, no change
  switch (ofMethod)
  {
    case OM_REPEAT:
      //clip coordinates
      x=MAX(x,0); x=MIN(x, pSrc->GetWidth()-1);
      y=MAX(y,0); y=MIN(y, pSrc->GetHeight()-1);
      break;
    case OM_WRAP:
      //wrap coordinates
      x = x % pSrc->GetWidth();
      y = y % pSrc->GetHeight();
      if (x<0) x = pSrc->GetWidth() + x;
      if (y<0) y = pSrc->GetHeight() + y;
      break;
    case OM_MIRROR:
      //mirror pixels near border
      if (x<0) x=((-x) % pSrc->GetWidth());
      else if (x>=pSrc->GetWidth()) x=pSrc->GetWidth()-(x % pSrc->GetWidth() + 1);
      if (y<0) y=((-y) % pSrc->GetHeight());
      else if (y>=pSrc->GetHeight()) y=pSrc->GetHeight()-(y % pSrc->GetHeight() + 1);
      break;
    default:
      return;
  }//switch
}
void ipTransformation::OverflowCoordinates(Mat* pSrc, long &x, long &y, OverflowMethod const ofMethod)
{
	if (pSrc->IsInterior(x,y)) return;  //if pixel is within bounds, no change
	switch (ofMethod)
	{
    case OM_REPEAT:
		//clip coordinates
		x=MAX(x,0); x=MIN(x, pSrc->Cols()-1);
		y=MAX(y,0); y=MIN(y, pSrc->Rows()-1);
		break;
    case OM_WRAP:
		//wrap coordinates
		x = x % pSrc->Cols();
		y = y % pSrc->Rows();
		if (x<0) x = pSrc->Cols() + x;
		if (y<0) y = pSrc->Rows() + y;
		break;
    case OM_MIRROR:
		//mirror pixels near border
		if (x<0) x=((-x) % pSrc->Cols());
		else if (x>=pSrc->Cols()) x=pSrc->Cols()-(x % pSrc->Cols() + 1);
		if (y<0) y=((-y) % pSrc->Rows());
		else if (y>=pSrc->Rows()) y=pSrc->Rows()-(y % pSrc->Rows() + 1);
		break;
    default:
		return;
	}//switch
}

////////////////////////////////////////////////////////////////////////////////
/**
 * See OverflowCoordinates for integer version 
 * \author ***bd*** 2.2004
 */
void ipTransformation::OverflowCoordinates(CoImage* pSrc, float &x, float &y, OverflowMethod const ofMethod)
{
  if (x>=0 && x<pSrc->GetWidth() && y>=0 && y<pSrc->GetHeight()) return;  //if pixel is within bounds, no change
  switch (ofMethod)
  {
    case OM_REPEAT:
      //clip coordinates
      x=MAX(x,0); x=MIN(x, pSrc->GetWidth()-1);
      y=MAX(y,0); y=MIN(y, pSrc->GetHeight()-1);
      break;
    case OM_WRAP:
      //wrap coordinates
      x = (float)fmod(x, (float) pSrc->GetWidth());
      y = (float)fmod(y, (float) pSrc->GetHeight());
      if (x<0) x = pSrc->GetWidth() + x;
      if (y<0) y = pSrc->GetHeight() + y;
      break;
    case OM_MIRROR:
      //mirror pixels near border
      if (x<0) x=(float)fmod(-x, (float) pSrc->GetWidth());
      else if (x>=pSrc->GetWidth()) x=pSrc->GetWidth()-((float)fmod(x, (float) pSrc->GetWidth()) + 1);
      if (y<0) y=(float)fmod(-y, (float) pSrc->GetHeight());
      else if (y>=pSrc->GetHeight()) y=pSrc->GetHeight()-((float)fmod(y, (float) pSrc->GetHeight()) + 1);
      break;
    default:
      return;
  }//switch
}
void ipTransformation::OverflowCoordinates(Mat* pSrc, float &x, float &y, OverflowMethod const ofMethod)
{
	if (x>=0 && x<pSrc->Cols() && y>=0 && y<pSrc->Rows()) return;  //if pixel is within bounds, no change
	switch (ofMethod)
	{
    case OM_REPEAT:
		//clip coordinates
		x=MAX(x,0); x=MIN(x, pSrc->Cols()-1);
		y=MAX(y,0); y=MIN(y, pSrc->Rows()-1);
		break;
    case OM_WRAP:
		//wrap coordinates
		x = (float)fmod(x, (float) pSrc->Cols());
		y = (float)fmod(y, (float) pSrc->Rows());
		if (x<0) x = pSrc->Cols() + x;
		if (y<0) y = pSrc->Rows() + y;
		break;
    case OM_MIRROR:
		//mirror pixels near border
		if (x<0) x=(float)fmod(-x, (float) pSrc->Cols());
		else if (x>=pSrc->Cols()) x=pSrc->Cols()-((float)fmod(x, (float) pSrc->Cols()) + 1);
		if (y<0) y=(float)fmod(-y, (float) pSrc->Rows());
		else if (y>=pSrc->Rows()) y=pSrc->Rows()-((float)fmod(y, (float) pSrc->Rows()) + 1);
		break;
    default:
		return;
	}//switch
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Method return pixel color. Different methods are implemented for out of bounds pixels.
 * If an image has alpha channel, alpha value is returned in .RGBReserved.
 *
 *  \param x,y : pixel coordinates
 *  \param ofMethod : out-of-bounds method:
 *    - OF_WRAP - wrap over to pixels on other side of the image
 *    - OF_REPEAT - repeat last pixel on the edge
 *    - OF_COLOR - return input value of color
 *    - OF_BACKGROUND - return background color (if not set, return input color)
 *    - OF_TRANSPARENT - return transparent pixel
 *
 *  \param rplColor : input color (returned for out-of-bound coordinates in OF_COLOR mode and if other mode is not applicable)
 *
 * \return color : color of pixel
 * \author ***bd*** 2.2004
 */
COLORREF ipTransformation::GetPixelColorWithOverflow(CoImage* pSrc, long x, long y, OverflowMethod const ofMethod, COLORREF* const rplColor)
{
  COLORREF color;          //color to return
  if ((!pSrc->IsInterior(x,y)))
  {     //is pixel within bouns?:
    //pixel is out of bounds or no DIB
    if (rplColor!=NULL)
      color=*rplColor;
    else
	{
      color = RGB(255, 255, 255); //default replacement colour: white transparent
    }//if

    //pixel is out of bounds:
    switch (ofMethod)
	{
      case OM_TRANSPARENT:
          //no alpha transparency
		  if (rplColor == NULL)
		  {
			  color = m_transColor;
		  }//if
		  return color;
      case OM_BACKGROUND:
		  //return background color (if it exists, otherwise input value)
		  if (rplColor == NULL)
		  {
			  color = m_bkColor;
		  }//if
		  return color;
      case OM_REPEAT:
      case OM_WRAP:
      case OM_MIRROR:
        OverflowCoordinates(pSrc, x,y,ofMethod);
        break;
      default:
        //simply return replacement color (OM_COLOR and others)
        return color;
    }//switch
  }//if
  //just return specified pixel (it's within bounds)
  return BlindGetPixelColor(pSrc, x,y);
}
BYTE ipTransformation::GetPixelColorWithOverflow(Mat* pSrc, long x, long y, OverflowMethod const ofMethod, BYTE* const rplColor)
{
	BYTE color;          //color to return
	if ((!pSrc->IsInterior(x,y)))
	{     //is pixel within bouns?:
		//pixel is out of bounds or no DIB
		if (rplColor!=NULL)
			color=*rplColor;
		else
		{
			color = 255; //default replacement colour: white transparent
		}//if
		
		//pixel is out of bounds:
		switch (ofMethod)
		{
		case OM_TRANSPARENT:
			//no alpha transparency
			if (rplColor == NULL)
			{
				color = m_btransColor;
			}//if
			return color;
		case OM_BACKGROUND:
			//return background color (if it exists, otherwise input value)
			if (rplColor == NULL)
			{
				color = m_bkGray;
			}//if
			return color;
		case OM_REPEAT:
		case OM_WRAP:
		case OM_MIRROR:
			OverflowCoordinates(pSrc, x,y,ofMethod);
			break;
		default:
			//simply return replacement color (OM_COLOR and others)
			return color;
		}//switch
	}//if
	//just return specified pixel (it's within bounds)
	return BlindGetPixelColor(pSrc, x,y);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * This method reconstructs image according to chosen interpolation method and then returns pixel (x,y).
 * (x,y) can lie between actual image pixels. If (x,y) lies outside of image, method returns value
 * according to overflow method.
 * This method is very useful for geometrical image transformations, where destination pixel
 * can often assume color value lying between source pixels.
 *
 *  \param (x,y) - coordinates of pixel to return
 *           GPCI method recreates "analogue" image back from digital data, so x and y
 *           are float values and color value of point (1.1,1) will generally not be same
 *           as (1,1). Center of first pixel is at (0,0) and center of pixel right to it is (1,0).
 *           (0.5,0) is half way between these two pixels.
 *  \param inMethod - interpolation (reconstruction) method (kernel) to use:
 *    - IM_NEAREST_NEIGHBOUR - returns colour of nearest lying pixel (causes stairy look of 
 *                            processed images)
 *    - IM_BILINEAR - interpolates colour from four neighbouring pixels (softens image a bit)
 *    - IM_BICUBIC - interpolates from 16 neighbouring pixels (can produce "halo" artifacts)
 *    - IM_BICUBIC2 - interpolates from 16 neighbouring pixels (perhaps a bit less halo artifacts 
                     than IM_BICUBIC)
 *    - IM_BSPLINE - interpolates from 16 neighbouring pixels (softens image, washes colours)
 *                  (As far as I know, image should be prefiltered for this method to give 
 *                   good results... some other time :) )
 *                  This method uses bicubic interpolation kernel from CXImage 5.99a and older
 *                  versions.
 *    - IM_LANCZOS - interpolates from 12*12 pixels (slow, ringing artifacts)
 *
 *  \param ofMethod - overflow method (see comments at GetPixelColorWithOverflow)
 *  \param rplColor - pointer to color used for out of borders pixels in OM_COLOR mode
 *              (and other modes if colour can't calculated in a specified way)
 *
 *  \return interpolated color value (including interpolated alpha value, if image has alpha layer)
 * 
 *  \author ***bd*** 2.2004
 */
COLORREF ipTransformation::GetPixelColorInterpolated(
	CoImage* pSrc, float x,float y, 
	InterpolationMethod const inMethod, 
	OverflowMethod const ofMethod, 
	COLORREF* const rplColor)
{
  //calculate nearest pixel
  int xi=(int)(x); if (x<0) xi--;   //these replace (incredibly slow) floor (Visual c++ 2003, AMD Athlon)
  int yi=(int)(y); if (y<0) yi--;
  COLORREF color;                    //calculated colour

  switch (inMethod)
  {
    case IM_NEAREST_NEIGHBOUR:
		return GetPixelColorWithOverflow(pSrc, (long)(x+0.5f), (long)(y+0.5f), ofMethod, rplColor);
    default:
	{
      //bilinear interpolation
      if (xi<-1 || xi>=pSrc->GetWidth() || yi<-1 || yi>=pSrc->GetHeight()) 
	  {  //all 4 points are outside bounds?:
        switch (ofMethod) 
		{
          case OM_COLOR: case OM_TRANSPARENT: case OM_BACKGROUND:
            //we don't need to interpolate anything with all points outside in this case
            return GetPixelColorWithOverflow(pSrc, -999, -999, ofMethod, rplColor);
          default:
            //recalculate coordinates and use faster method later on
            OverflowCoordinates(pSrc, x,y,ofMethod);
            xi=(int)(x); if (x<0) xi--;   //x and/or y have changed ... recalculate xi and yi
            yi=(int)(y); if (y<0) yi--;
        }//switch
      }//if
      //get four neighbouring pixels
      if ((xi+1)<pSrc->GetWidth() && xi>=0 && (yi+1)<pSrc->GetHeight() && yi>=0)
	  {
        //all pixels are inside RGB24 image... optimize reading (and use fixed point arithmetic)
        WORD wt1=(WORD)((x-xi)*256.0f), wt2=(WORD)((y-yi)*256.0f);
        WORD wd=wt1*wt2>>8;
        WORD wb=wt1-wd;
        WORD wc=wt2-wd;
        WORD wa=256-wt1-wc;
        WORD wrr,wgg,wbb;

		BYTE** ppbSrc = pSrc->m_matX.data.ptr;
		wrr = wa*ppbSrc[yi][xi] + wb*ppbSrc[yi][xi+1] + wc*ppbSrc[yi+1][xi] + wd*ppbSrc[yi+1][xi+1];
		ppbSrc = pSrc->m_matY.data.ptr;
		wgg = wa*ppbSrc[yi][xi] + wb*ppbSrc[yi][xi+1] + wc*ppbSrc[yi+1][xi] + wd*ppbSrc[yi+1][xi+1];
		ppbSrc = pSrc->m_matZ.data.ptr;
		wbb = wa*ppbSrc[yi][xi] + wb*ppbSrc[yi][xi+1] + wc*ppbSrc[yi+1][xi] + wd*ppbSrc[yi+1][xi+1];
		color = RGB(wrr>>8, wgg>>8, wbb>>8);
        return color;
      } 
	  else 
	  {
        //default (slower) way to get pixels (not RGB24 or some pixels out of borders)
        float t1=x-xi, t2=y-yi;
        float d=t1*t2;
        float b=t1-d;
        float c=t2-d;
        float a=1-t1-c;
        COLORREF rgb11,rgb21,rgb12,rgb22;
        rgb11=GetPixelColorWithOverflow(pSrc, xi, yi, ofMethod, rplColor);
        rgb21=GetPixelColorWithOverflow(pSrc, xi+1, yi, ofMethod, rplColor);
        rgb12=GetPixelColorWithOverflow(pSrc, xi, yi+1, ofMethod, rplColor);
        rgb22=GetPixelColorWithOverflow(pSrc, xi+1, yi+1, ofMethod, rplColor);
        //calculate linear interpolation
        color = RGB((BYTE) (a*GetRValue(rgb11)+b*GetRValue(rgb21)+c*GetRValue(rgb12)+d*GetRValue(rgb22)),
					(BYTE) (a*GetGValue(rgb11)+b*GetGValue(rgb21)+c*GetGValue(rgb12)+d*GetGValue(rgb22)),
					(BYTE) (a*GetBValue(rgb11)+b*GetBValue(rgb21)+c*GetBValue(rgb12)+d*GetBValue(rgb22)));
        return color;
      }//if
    }//default
    case IM_BICUBIC: 
    case IM_BICUBIC2:
    case IM_BSPLINE:
	case IM_BOX:
	case IM_HERMITE:
	case IM_HAMMING:
	case IM_SINC:
	case IM_BLACKMAN:
	case IM_BESSEL:
	case IM_GAUSSIAN:
	case IM_QUADRATIC:
	case IM_MITCHELL:
	case IM_CATROM:
      //bicubic interpolation(s)
      if (((xi+2)<0) || ((xi-1)>=pSrc->GetWidth()) || ((yi+2)<0) || ((yi-1)>=pSrc->GetHeight()))
	  { //all points are outside bounds?:
        switch (ofMethod)
		{
          case OM_COLOR: case OM_TRANSPARENT: case OM_BACKGROUND:
            //we don't need to interpolate anything with all points outside in this case
            return GetPixelColorWithOverflow(pSrc, -999, -999, ofMethod, rplColor);
            break;
          default:
            //recalculate coordinates and use faster method later on
            OverflowCoordinates(pSrc, x,y,ofMethod);
            xi=(int)(x); if (x<0) xi--;   //x and/or y have changed ... recalculate xi and yi
            yi=(int)(y); if (y<0) yi--;
        }//switch
      }//if

      //some variables needed from here on
      int xii,yii;                      //x any y integer indexes for loops
      float kernel, kernelyc;           //kernel cache
      float kernelx[12], kernely[4];    //precalculated kernel values
      float rr,gg,bb,aa;                //accumulated color values
      //calculate multiplication factors for all pixels
	  int i;
      switch (inMethod) 
	  {
        case IM_BICUBIC:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelCubic((float)(xi+i-1-x));
            kernely[i]=KernelCubic((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BICUBIC2:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelGeneralizedCubic((float)(xi+i-1-x), -0.5);
            kernely[i]=KernelGeneralizedCubic((float)(yi+i-1-y), -0.5);
          }//for i
          break;
        case IM_BSPLINE:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelBSpline((float)(xi+i-1-x));
            kernely[i]=KernelBSpline((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BOX:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelBox((float)(xi+i-1-x));
            kernely[i]=KernelBox((float)(yi+i-1-y));
          }//for i
          break;
        case IM_HERMITE:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelHermite((float)(xi+i-1-x));
            kernely[i]=KernelHermite((float)(yi+i-1-y));
          }//for i
          break;
        case IM_HAMMING:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelHamming((float)(xi+i-1-x));
            kernely[i]=KernelHamming((float)(yi+i-1-y));
          }//for i
          break;
        case IM_SINC:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelSinc((float)(xi+i-1-x));
            kernely[i]=KernelSinc((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BLACKMAN:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelBlackman((float)(xi+i-1-x));
            kernely[i]=KernelBlackman((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BESSEL:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelBessel((float)(xi+i-1-x));
            kernely[i]=KernelBessel((float)(yi+i-1-y));
          }//for i
          break;
        case IM_GAUSSIAN:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelGaussian((float)(xi+i-1-x));
            kernely[i]=KernelGaussian((float)(yi+i-1-y));
          }//for i
          break;
        case IM_QUADRATIC:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelQuadratic((float)(xi+i-1-x));
            kernely[i]=KernelQuadratic((float)(yi+i-1-y));
          }//for i
          break;
        case IM_MITCHELL:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelMitchell((float)(xi+i-1-x));
            kernely[i]=KernelMitchell((float)(yi+i-1-y));
          }//for i
          break;
        case IM_CATROM:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelCatrom((float)(xi+i-1-x));
            kernely[i]=KernelCatrom((float)(yi+i-1-y));
          }//for
              break;
         default:
              assert(false);;
      }//switch
      rr=gg=bb=aa=0;
      if (((xi+2)<pSrc->GetWidth()) && xi>=1 && ((yi+2)<pSrc->GetHeight()) && (yi>=1))
	  {
        //optimized interpolation (faster pixel reads) for RGB24 images with all pixels inside bounds
			BYTE** ppbR = pSrc->m_matX.data.ptr;
			BYTE** ppbG = pSrc->m_matY.data.ptr;
			BYTE** ppbB = pSrc->m_matZ.data.ptr;
//        BYTE *pxptr, *pxptra;
        for (yii=yi-1; yii<yi+3; yii++)
		{
//          pxptr=(BYTE *)BlindGetPixelPointer(xi-1, yii);    //calculate pointer to first byte in row
          kernelyc=kernely[yii-(yi-1)];
          //alpha not supported or valid (optimized bicubic int. for no alpha channel)
          {
            kernel=kernelyc*kernelx[0];
            bb+=kernel*ppbB[yii][xi-1]; gg+=kernel*ppbG[yii][xi-1]; rr+=kernel*ppbR[yii][xi-1];
            kernel=kernelyc*kernelx[1];
            bb+=kernel*ppbB[yii][xi]; gg+=kernel*ppbG[yii][xi]; rr+=kernel*ppbR[yii][xi];
            kernel=kernelyc*kernelx[2];
            bb+=kernel*ppbB[yii][xi+1]; gg+=kernel*ppbG[yii][xi+1]; rr+=kernel*ppbR[yii][xi+1];
            kernel=kernelyc*kernelx[3];
            bb+=kernel*ppbB[yii][xi+2]; gg+=kernel*ppbG[yii][xi+2]; rr+=kernel*ppbR[yii][xi+2];
          }
        }//yii
      }
	  else
	  {
        //slower more flexible interpolation for border pixels and paletted images
        COLORREF rgbs;
        for (yii=yi-1; yii<yi+3; yii++)
		{
          kernelyc=kernely[yii-(yi-1)];
          for (xii=xi-1; xii<xi+3; xii++)
		  {
            kernel=kernelyc*kernelx[xii-(xi-1)];
            rgbs=GetPixelColorWithOverflow(pSrc, xii, yii, ofMethod, rplColor);
            rr+=kernel*GetRValue(rgbs);
            gg+=kernel*GetGValue(rgbs);
            bb+=kernel*GetBValue(rgbs);
          }//xii
        }//yii
      }//if
      //for all colors, clip to 0..255 and assign to COLORREF
      if (rr>255) rr=255; if (rr<0) rr=0;
      if (gg>255) gg=255; if (gg<0) gg=0;
      if (bb>255) bb=255; if (bb<0) bb=0;
	  color = RGB(rr,gg,bb);
      return color;
    case IM_LANCZOS:
      //lanczos window (16*16) sinc interpolation
      if (((xi+6)<0) || ((xi-5)>=pSrc->GetWidth()) || ((yi+6)<0) || ((yi-5)>=pSrc->GetHeight()))
	  {
        //all points are outside bounds
        switch (ofMethod)
		{
          case OM_COLOR: case OM_TRANSPARENT: case OM_BACKGROUND:
            //we don't need to interpolate anything with all points outside in this case
            return GetPixelColorWithOverflow(pSrc, -999, -999, ofMethod, rplColor);
            break;
          default:
            //recalculate coordinates and use faster method later on
            OverflowCoordinates(pSrc, x,y,ofMethod);
            xi=(int)(x); if (x<0) xi--;   //x and/or y have changed ... recalculate xi and yi
            yi=(int)(y); if (y<0) yi--;
        }//switch
      }//if

      for (xii=xi-5; xii<xi+7; xii++) 
		  kernelx[xii-(xi-5)]=KernelLanczosSinc((float)(xii-x), 6.0f);
      rr=gg=bb=aa=0;

      if (((xi+6)<pSrc->GetWidth()) && ((xi-5)>=0) && ((yi+6)<pSrc->GetHeight()) && ((yi-5)>=0))
	  {
        //optimized interpolation (faster pixel reads) for RGB24 images with all pixels inside bounds
        BYTE *pbR, *pbG, *pbB;
        for (yii=yi-5; yii<yi+7; yii++)
		{
			pbR = &pSrc->m_matX.data.ptr[yii][xi-5];
			pbG = &pSrc->m_matY.data.ptr[yii][xi-5];
			pbB = &pSrc->m_matZ.data.ptr[yii][xi-5];
	          kernelyc=KernelLanczosSinc((float)(yii-y),6.0f);
			for (xii=0; xii<12; xii++) 
			{
			  kernel=kernelyc*kernelx[xii];
			  bb+=kernel*pbB[xii]; gg+=kernel*pbG[xii]; rr+=kernel*pbR[xii];
			}//for xii
        }//yii
      }
	  else
	  {
        //slower more flexible interpolation for border pixels and paletted images
        COLORREF rgbs;
        for (yii=yi-5; yii<yi+7; yii++)
		{
          kernelyc=KernelLanczosSinc((float)(yii-y),6.0f);
          for (xii=xi-5; xii<xi+7; xii++)
		  {
            kernel=kernelyc*kernelx[xii-(xi-5)];
            rgbs=GetPixelColorWithOverflow(pSrc, xii, yii, ofMethod, rplColor);
            rr+=kernel*GetRValue(rgbs);
            gg+=kernel*GetGValue(rgbs);
            bb+=kernel*GetBValue(rgbs);
          }//xii
        }//yii
      }//if
      //for all colors, clip to 0..255 and assign to COLORREF
      if (rr>255) rr=255; if (rr<0) rr=0;
      if (gg>255) gg=255; if (gg<0) gg=0;
      if (bb>255) bb=255; if (bb<0) bb=0;
	  color = RGB(rr,gg,bb);
      return color;
  }//switch
}


BYTE ipTransformation::GetPixelColorInterpolated(
	Mat* pSrc, float x,float y, 
	InterpolationMethod const inMethod, 
	OverflowMethod const ofMethod, 
	BYTE* const rplColor)
{
  //calculate nearest pixel
  int xi=(int)(x); if (x<0) xi--;   //these replace (incredibly slow) floor (Visual c++ 2003, AMD Athlon)
  int yi=(int)(y); if (y<0) yi--;
  BYTE color;                    //calculated colour

  switch (inMethod)
  {
    case IM_NEAREST_NEIGHBOUR:
      return GetPixelColorWithOverflow(pSrc, (long)(x+0.5f), (long)(y+0.5f), ofMethod, rplColor);
    default:
	{
      //bilinear interpolation
      if (xi<-1 || xi>=pSrc->Cols() || yi<-1 || yi>=pSrc->Rows()) 
	  {  //all 4 points are outside bounds?:
        switch (ofMethod) 
		{
          case OM_COLOR: case OM_TRANSPARENT: case OM_BACKGROUND:
            //we don't need to interpolate anything with all points outside in this case
            return GetPixelColorWithOverflow(pSrc, -999, -999, ofMethod, rplColor);
          default:
            //recalculate coordinates and use faster method later on
            OverflowCoordinates(pSrc, x,y,ofMethod);
            xi=(int)(x); if (x<0) xi--;   //x and/or y have changed ... recalculate xi and yi
            yi=(int)(y); if (y<0) yi--;
        }//switch
      }//if
      //get four neighbouring pixels
      if ((xi+1)<pSrc->Cols() && xi>=0 && (yi+1)<pSrc->Rows() && yi>=0)
	  {
        //all pixels are inside RGB24 image... optimize reading (and use fixed point arithmetic)
        WORD wt1=(WORD)((x-xi)*256.0f), wt2=(WORD)((y-yi)*256.0f);
        WORD wd=wt1*wt2>>8;
        WORD wb=wt1-wd;
        WORD wc=wt2-wd;
        WORD wa=256-wt1-wc;
        WORD wrr;

		BYTE** ppbSrc = pSrc->data.ptr;
		wrr = wa*ppbSrc[yi][xi] + wb*ppbSrc[yi][xi+1] + wc*ppbSrc[yi+1][xi] + wd*ppbSrc[yi+1][xi+1];
        return wrr>>8;
      } 
	  else 
	  {
        //default (slower) way to get pixels (not RGB24 or some pixels out of borders)
        float t1=x-xi, t2=y-yi;
        float d=t1*t2;
        float b=t1-d;
        float c=t2-d;
        float a=1-t1-c;
        BYTE rgb11,rgb21,rgb12,rgb22;
        rgb11=GetPixelColorWithOverflow(pSrc, xi, yi, ofMethod, rplColor);
        rgb21=GetPixelColorWithOverflow(pSrc, xi+1, yi, ofMethod, rplColor);
        rgb12=GetPixelColorWithOverflow(pSrc, xi, yi+1, ofMethod, rplColor);
        rgb22=GetPixelColorWithOverflow(pSrc, xi+1, yi+1, ofMethod, rplColor);
        //calculate linear interpolation
        color = (BYTE) (a*rgb11+b*rgb21+c*rgb12+d*rgb22);
        return color;
      }//if
    }//default
    case IM_BICUBIC: 
    case IM_BICUBIC2:
    case IM_BSPLINE:
	case IM_BOX:
	case IM_HERMITE:
	case IM_HAMMING:
	case IM_SINC:
	case IM_BLACKMAN:
	case IM_BESSEL:
	case IM_GAUSSIAN:
	case IM_QUADRATIC:
	case IM_MITCHELL:
	case IM_CATROM:
      //bicubic interpolation(s)
      if (((xi+2)<0) || ((xi-1)>=pSrc->Cols()) || ((yi+2)<0) || ((yi-1)>=pSrc->Rows()))
	  { //all points are outside bounds?:
        switch (ofMethod)
		{
          case OM_COLOR: case OM_TRANSPARENT: case OM_BACKGROUND:
            //we don't need to interpolate anything with all points outside in this case
            return GetPixelColorWithOverflow(pSrc, -999, -999, ofMethod, rplColor);
            break;
          default:
            //recalculate coordinates and use faster method later on
            OverflowCoordinates(pSrc, x,y,ofMethod);
            xi=(int)(x); if (x<0) xi--;   //x and/or y have changed ... recalculate xi and yi
            yi=(int)(y); if (y<0) yi--;
        }//switch
      }//if

      //some variables needed from here on
      int xii,yii;                      //x any y integer indexes for loops
      float kernel, kernelyc;           //kernel cache
      float kernelx[12], kernely[4];    //precalculated kernel values
      float rr;                //accumulated color values
      //calculate multiplication factors for all pixels
	  int i;
      switch (inMethod) 
	  {
        case IM_BICUBIC:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelCubic((float)(xi+i-1-x));
            kernely[i]=KernelCubic((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BICUBIC2:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelGeneralizedCubic((float)(xi+i-1-x), -0.5);
            kernely[i]=KernelGeneralizedCubic((float)(yi+i-1-y), -0.5);
          }//for i
          break;
        case IM_BSPLINE:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelBSpline((float)(xi+i-1-x));
            kernely[i]=KernelBSpline((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BOX:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelBox((float)(xi+i-1-x));
            kernely[i]=KernelBox((float)(yi+i-1-y));
          }//for i
          break;
        case IM_HERMITE:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelHermite((float)(xi+i-1-x));
            kernely[i]=KernelHermite((float)(yi+i-1-y));
          }//for i
          break;
        case IM_HAMMING:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelHamming((float)(xi+i-1-x));
            kernely[i]=KernelHamming((float)(yi+i-1-y));
          }//for i
          break;
        case IM_SINC:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelSinc((float)(xi+i-1-x));
            kernely[i]=KernelSinc((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BLACKMAN:
          for (i=0; i<4; i++) 
		  {
            kernelx[i]=KernelBlackman((float)(xi+i-1-x));
            kernely[i]=KernelBlackman((float)(yi+i-1-y));
          }//for i
          break;
        case IM_BESSEL:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelBessel((float)(xi+i-1-x));
            kernely[i]=KernelBessel((float)(yi+i-1-y));
          }//for i
          break;
        case IM_GAUSSIAN:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelGaussian((float)(xi+i-1-x));
            kernely[i]=KernelGaussian((float)(yi+i-1-y));
          }//for i
          break;
        case IM_QUADRATIC:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelQuadratic((float)(xi+i-1-x));
            kernely[i]=KernelQuadratic((float)(yi+i-1-y));
          }//for i
          break;
        case IM_MITCHELL:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelMitchell((float)(xi+i-1-x));
            kernely[i]=KernelMitchell((float)(yi+i-1-y));
          }//for i
          break;
        case IM_CATROM:
          for (i=0; i<4; i++) {
            kernelx[i]=KernelCatrom((float)(xi+i-1-x));
            kernely[i]=KernelCatrom((float)(yi+i-1-y));
          }//for i
          break;
          default:
          assert(false);
      }//switch
      rr=0;
      if (((xi+2)<pSrc->Cols()) && xi>=1 && ((yi+2)<pSrc->Rows()) && (yi>=1))
	  {
        //optimized interpolation (faster pixel reads) for RGB24 images with all pixels inside bounds
		BYTE** ppbR = pSrc->data.ptr;
//        BYTE *pxptr, *pxptra;
        for (yii=yi-1; yii<yi+3; yii++)
		{
//          pxptr=(BYTE *)BlindGetPixelPointer(xi-1, yii);    //calculate pointer to first byte in row
          kernelyc=kernely[yii-(yi-1)];
          //alpha not supported or valid (optimized bicubic int. for no alpha channel)
          {
            kernel=kernelyc*kernelx[0];
            rr+=kernel*ppbR[yii][xi-1];
            kernel=kernelyc*kernelx[1];
            rr+=kernel*ppbR[yii][xi];
            kernel=kernelyc*kernelx[2];
            rr+=kernel*ppbR[yii][xi+1];
            kernel=kernelyc*kernelx[3];
            rr+=kernel*ppbR[yii][xi+2];
          }
        }//yii
      }
	  else
	  {
        //slower more flexible interpolation for border pixels and paletted images
        COLORREF rgbs;
        for (yii=yi-1; yii<yi+3; yii++)
		{
          kernelyc=kernely[yii-(yi-1)];
          for (xii=xi-1; xii<xi+3; xii++)
		  {
            kernel=kernelyc*kernelx[xii-(xi-1)];
            rgbs=GetPixelColorWithOverflow(pSrc, xii, yii, ofMethod, rplColor);
            rr+=kernel*GetRValue(rgbs);
          }//xii
        }//yii
      }//if
      //for all colors, clip to 0..255 and assign to COLORREF
      if (rr>255) rr=255; if (rr<0) rr=0;
      return (BYTE)rr;
    case IM_LANCZOS:
      //lanczos window (16*16) sinc interpolation
      if (((xi+6)<0) || ((xi-5)>=pSrc->Cols()) || ((yi+6)<0) || ((yi-5)>=pSrc->Rows()))
	  {
        //all points are outside bounds
        switch (ofMethod)
		{
          case OM_COLOR: case OM_TRANSPARENT: case OM_BACKGROUND:
            //we don't need to interpolate anything with all points outside in this case
            return GetPixelColorWithOverflow(pSrc, -999, -999, ofMethod, rplColor);
            break;
          default:
            //recalculate coordinates and use faster method later on
            OverflowCoordinates(pSrc, x,y,ofMethod);
            xi=(int)(x); if (x<0) xi--;   //x and/or y have changed ... recalculate xi and yi
            yi=(int)(y); if (y<0) yi--;
        }//switch
      }//if

      for (xii=xi-5; xii<xi+7; xii++) 
		  kernelx[xii-(xi-5)]=KernelLanczosSinc((float)(xii-x), 6.0f);
      rr=0;

      if (((xi+6)<pSrc->Cols()) && ((xi-5)>=0) && ((yi+6)<pSrc->Rows()) && ((yi-5)>=0))
	  {
        //optimized interpolation (faster pixel reads) for RGB24 images with all pixels inside bounds
        BYTE *pbR;
        for (yii=yi-5; yii<yi+7; yii++)
		{
			pbR = &pSrc->data.ptr[yii][xi-5];
	        kernelyc=KernelLanczosSinc((float)(yii-y),6.0f);
			for (xii=0; xii<12; xii++) 
			{
			  kernel=kernelyc*kernelx[xii];
			  rr+=kernel*pbR[xii];
			}//for xii
        }//yii
      }
	  else
	  {
        //slower more flexible interpolation for border pixels and paletted images
        for (yii=yi-5; yii<yi+7; yii++)
		{
          kernelyc=KernelLanczosSinc((float)(yii-y),6.0f);
          for (xii=xi-5; xii<xi+7; xii++)
		  {
            kernel=kernelyc*kernelx[xii-(xi-5)];
            rr+=kernel*GetPixelColorWithOverflow(pSrc, xii, yii, ofMethod, rplColor);
          }//xii
        }//yii
      }//if
      //for all colors, clip to 0..255 and assign to COLORREF
      if (rr>255) rr=255; if (rr<0) rr=0;
      return (BYTE)rr;
  }//switch
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Helper function for GetAreaColorInterpolated.
 * Adds 'surf' portion of image pixel with color 'color' to (rr,gg,bb,aa).
 */
void ipTransformation::AddAveragingCont(COLORREF const &color, float const surf, float &rr, float &gg, float &bb)
{
  rr+=GetRValue(color)*surf;
  gg+=GetGValue(color)*surf;
  bb+=GetBValue(color)*surf;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * This method is similar to GetPixelColorInterpolated, but this method also properly handles 
 * subsampling.
 * If you need to sample original image with interval of more than 1 pixel (as when shrinking an image), 
 * you should use this method instead of GetPixelColorInterpolated or aliasing will occur.
 * When area width and height are both less than pixel, this method gets pixel color by interpolating
 * color of frame center with selected (inMethod) interpolation by calling GetPixelColorInterpolated. 
 * If width and height are more than 1, method calculates color by averaging color of pixels within area.
 * Interpolation method is not used in this case. Pixel color is interpolated by averaging instead.
 * If only one of both is more than 1, method uses combination of interpolation and averaging.
 * Chosen interpolation method is used, but since it is averaged later on, there is little difference
 * between IM_BILINEAR (perhaps best for this case) and better methods. IM_NEAREST_NEIGHBOUR again
 * leads to aliasing artifacts.
 * This method is a bit slower than GetPixelColorInterpolated and when aliasing is not a problem, you should
 * simply use the later. 
 *
 * \param  xc, yc - center of (rectangular) area
 * \param  w, h - width and height of area
 * \param  inMethod - interpolation method that is used, when interpolation is used (see above)
 * \param  ofMethod - overflow method used when retrieving individual pixel colors
 * \param  rplColor - replacement colour to use, in OM_COLOR
 *
 * \author ***bd*** 2.2004
 */
COLORREF ipTransformation::GetAreaColorInterpolated(
  CoImage* pSrc,
  float const xc, float const yc, float const w, float const h, 
  InterpolationMethod const inMethod, 
  OverflowMethod const ofMethod, 
  COLORREF* const rplColor)
{
	COLORREF color;      //calculated colour
	
	if (h<=1 && w<=1)
	{
		//both width and height are less than one... we will use interpolation of center point
		return GetPixelColorInterpolated(pSrc, xc, yc, inMethod, ofMethod, rplColor);
	}
	else 
	{
		//area is wider and/or taller than one pixel:
		Rectf area(xc-w/2.0f, yc-h/2.0f, xc+w/2.0f, yc+h/2.0f);   //area
		int xi1=(int)(area.topLeft.x+0.49999999f);                //low x
		int yi1=(int)(area.topLeft.y+0.49999999f);                //low y
		
		
		int xi2=(int)(area.bottomRight.x+0.5f);                      //top x
		int yi2=(int)(area.bottomRight.y+0.5f);                      //top y (for loops)
		
		float rr,gg,bb,aa;                                        //red, green, blue and alpha components
		rr=gg=bb=aa=0;
		int x,y;                                                  //loop counters
		float s=0;                                                //surface of all pixels
		float cps;                                                //surface of current crosssection
		if (h>1 && w>1) 
		{
			//width and height of area are greater than one pixel, so we can employ "ordinary" averaging
			Rectf intBL, intTR;     //bottom left and top right intersection
			intBL=area.CrossSection(Rectf(((float)xi1)-0.5f, ((float)yi1)-0.5f, ((float)xi1)+0.5f, ((float)yi1)+0.5f));
			intTR=area.CrossSection(Rectf(((float)xi2)-0.5f, ((float)yi2)-0.5f, ((float)xi2)+0.5f, ((float)yi2)+0.5f));
			float wBL, wTR, hBL, hTR;
			wBL=intBL.Width();            //width of bottom left pixel-area intersection
			hBL=intBL.Height();           //height of bottom left...
			wTR=intTR.Width();            //width of top right...
			hTR=intTR.Height();           //height of top right...
			
			AddAveragingCont(GetPixelColorWithOverflow(pSrc, xi1,yi1,ofMethod,rplColor), wBL*hBL, rr, gg, bb);    //bottom left pixel
			AddAveragingCont(GetPixelColorWithOverflow(pSrc, xi2,yi1,ofMethod,rplColor), wTR*hBL, rr, gg, bb);    //bottom right pixel
			AddAveragingCont(GetPixelColorWithOverflow(pSrc, xi1,yi2,ofMethod,rplColor), wBL*hTR, rr, gg, bb);    //top left pixel
			AddAveragingCont(GetPixelColorWithOverflow(pSrc, xi2,yi2,ofMethod,rplColor), wTR*hTR, rr, gg, bb);    //top right pixel
			//bottom and top row
			for (x=xi1+1; x<xi2; x++)
			{
				AddAveragingCont(GetPixelColorWithOverflow(pSrc, x,yi1,ofMethod,rplColor), hBL, rr, gg, bb);    //bottom row
				AddAveragingCont(GetPixelColorWithOverflow(pSrc, x,yi2,ofMethod,rplColor), hTR, rr, gg, bb);    //top row
			}
			//leftmost and rightmost column
			for (y=yi1+1; y<yi2; y++)
			{
				AddAveragingCont(GetPixelColorWithOverflow(pSrc, xi1,y,ofMethod,rplColor), wBL, rr, gg, bb);    //left column
				AddAveragingCont(GetPixelColorWithOverflow(pSrc, xi2,y,ofMethod,rplColor), wTR, rr, gg, bb);    //right column
			}
			for (y=yi1+1; y<yi2; y++)
			{
				for (x=xi1+1; x<xi2; x++) 
				{ 
					color=GetPixelColorWithOverflow(pSrc, x,y,ofMethod,rplColor);
					rr+=GetRValue(color);
					gg+=GetGValue(color);
					bb+=GetBValue(color);
				}//for x
			}//for y
		} 
		else 
		{
			//width or height greater than one:
			Rectf intersect;                                          //intersection with current pixel
			Point2f center;
			for (y=yi1; y<=yi2; y++) 
			{
				for (x=xi1; x<=xi2; x++) 
				{
					intersect=area.CrossSection(Rectf(((float)x)-0.5f, ((float)y)-0.5f, ((float)x)+0.5f, ((float)y)+0.5f));
					center=intersect.Center();
					color=GetPixelColorInterpolated(pSrc, center.x, center.y, inMethod, ofMethod, rplColor);
					cps=intersect.Surface();
					rr+=GetRValue(color)*cps;
					gg+=GetGValue(color)*cps;
					bb+=GetBValue(color)*cps;
				}//for x
			}//for y      
		}//if
		
		s=area.Surface();
		rr/=s; gg/=s; bb/=s; aa/=s;
		if (rr>255) rr=255; if (rr<0) rr=0;
		if (gg>255) gg=255; if (gg<0) gg=0;
		if (bb>255) bb=255; if (bb<0) bb=0;
		color = RGB(rr,gg,bb);
	}//if
	return color;
}
/**
* This is (a bit) faster version of GetPixelColor. 
* It tests bounds only in debug mode (_DEBUG defined).
* 
* It is an error to request out-of-borders pixel with this method. 
* In DEBUG mode an exception will be thrown, and data will be violated in non-DEBUG mode. 
* \author ***bd*** 2.2004
  */
COLORREF ipTransformation::BlindGetPixelColor(CoImage* pSrc, const long x,const long y)
{
  COLORREF rgb;
#ifdef _DEBUG
  if (!pSrc->IsInterior(x,y)) throw 0;
#endif
  rgb = RGB(pSrc->m_matX.data.ptr[y][x], pSrc->m_matY.data.ptr[y][x], pSrc->m_matZ.data.ptr[y][x]);
  return rgb;
}

BYTE ipTransformation::BlindGetPixelColor(Mat* pSrc, const long x,const long y)
{
	return pSrc->data.ptr[y][x];
}

float ipTransformation::KernelBSpline(const float x)
{
	if (x>2.0f) return 0.0f;
	// thanks to Kristian Kratzenstein
	float a, b, c, d;
	float xm1 = x - 1.0f; // Was calculatet anyway cause the "if((x-1.0f) < 0)"
	float xp1 = x + 1.0f;
	float xp2 = x + 2.0f;

	if ((xp2) <= 0.0f) a = 0.0f; else a = xp2*xp2*xp2; // Only float, not float -> double -> float
	if ((xp1) <= 0.0f) b = 0.0f; else b = xp1*xp1*xp1;
	if (x <= 0) c = 0.0f; else c = x*x*x;  
	if ((xm1) <= 0.0f) d = 0.0f; else d = xm1*xm1*xm1;

	return (0.16666666666666666667f * (a - (4.0f * b) + (6.0f * c) - (4.0f * d)));

	/* equivalent <Vladim?r Kloucek>
	if (x < -2.0)
		return(0.0f);
	if (x < -1.0)
		return((2.0f+x)*(2.0f+x)*(2.0f+x)*0.16666666666666666667f);
	if (x < 0.0)
		return((4.0f+x*x*(-6.0f-3.0f*x))*0.16666666666666666667f);
	if (x < 1.0)
		return((4.0f+x*x*(-6.0f+3.0f*x))*0.16666666666666666667f);
	if (x < 2.0)
		return((2.0f-x)*(2.0f-x)*(2.0f-x)*0.16666666666666666667f);
	return(0.0f);
	*/
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Bilinear interpolation kernel:
  \verbatim
          /
         | 1-t           , if  0 <= t <= 1
  h(t) = | t+1           , if -1 <= t <  0
         | 0             , otherwise
          \
  \endverbatim
 * ***bd*** 2.2004
 */
float ipTransformation::KernelLinear(const float t)
{
//  if (0<=t && t<=1) return 1-t;
//  if (-1<=t && t<0) return 1+t;
//  return 0;
	
	//<Vladim?r Kloucek>
	if (t < -1.0f)
		return 0.0f;
	if (t < 0.0f)
		return 1.0f+t;
	if (t < 1.0f)
		return 1.0f-t;
	return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Bicubic interpolation kernel (a=-1):
  \verbatim
          /
         | 1-2|t|**2+|t|**3          , if |t| < 1
  h(t) = | 4-8|t|+5|t|**2-|t|**3     , if 1<=|t|<2
         | 0                         , otherwise
          \
  \endverbatim
 * ***bd*** 2.2004
 */
float ipTransformation::KernelCubic(const float t)
{
  float abs_t = (float)fabs(t);
  float abs_t_sq = abs_t * abs_t;
  if (abs_t<1) return 1-2*abs_t_sq+abs_t_sq*abs_t;
  if (abs_t<2) return 4 - 8*abs_t +5*abs_t_sq - abs_t_sq*abs_t;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Bicubic kernel (for a=-1 it is the same as BicubicKernel):
  \verbatim
          /
         | (a+2)|t|**3 - (a+3)|t|**2 + 1     , |t| <= 1
  h(t) = | a|t|**3 - 5a|t|**2 + 8a|t| - 4a   , 1 < |t| <= 2
         | 0                                 , otherwise
          \
  \endverbatim
 * Often used values for a are -1 and -1/2.
 */
float ipTransformation::KernelGeneralizedCubic(const float t, const float a)
{
  float abs_t = (float)fabs(t);
  float abs_t_sq = abs_t * abs_t;
  if (abs_t<1) return (a+2)*abs_t_sq*abs_t - (a+3)*abs_t_sq + 1;
  if (abs_t<2) return a*abs_t_sq*abs_t - 5*a*abs_t_sq + 8*a*abs_t - 4*a;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Lanczos windowed sinc interpolation kernel with radius r.
  \verbatim
          /
  h(t) = | sinc(t)*sinc(t/r)       , if |t|<r
         | 0                       , otherwise
          \
  \endverbatim
 * ***bd*** 2.2004
 */
float ipTransformation::KernelLanczosSinc(const float t, const float r)
{
  if (fabs(t) > r) return 0;
  if (t==0) return 1;
  float pit=(float)(CVLIB_PI*t);
  float pitd=pit/r;
  return (float)((sin(pit)/pit) * (sin(pitd)/pitd));
}

////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBox(const float x)
{
	if (x < -0.5f)
		return 0.0f;
	if (x < 0.5f)
		return 1.0f;
	return 0.0f;
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelHermite(const float x)
{
	if (x < -1.0f)
		return 0.0f;
	if (x < 0.0f)
		return (-2.0f*x-3.0f)*x*x+1.0f;
	if (x < 1.0f)
		return (2.0f*x-3.0f)*x*x+1.0f;
	return 0.0f;
//	if (fabs(x)>1) return 0.0f;
//	return(0.5f+0.5f*(float)cos(CVLIB_PI*x));
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelHamming(const float x)
{
	if (x < -1.0f)
		return 0.0f;
	if (x < 0.0f)
		return 0.92f*(-2.0f*x-3.0f)*x*x+1.0f;
	if (x < 1.0f)
		return 0.92f*(2.0f*x-3.0f)*x*x+1.0f;
	return 0.0f;
//	if (fabs(x)>1) return 0.0f;
//	return(0.54f+0.46f*(float)cos(CVLIB_PI*x));
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelSinc(const float x)
{
	if (x == 0.0)
		return(1.0);
	return((float)sin( (float)(CVLIB_PI*x) )/(float)(CVLIB_PI*x));
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBlackman(const float x)
{
	//if (fabs(x)>1) return 0.0f;
	return (0.42f+0.5f*(float)cos(CVLIB_PI*x)+0.08f*(float)cos(2.0f*CVLIB_PI*x));
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBessel_J1(const float x)
{
	double p, q;
	
	register long i;
	
	static const double
	Pone[] =
	{
		0.581199354001606143928050809e+21,
		-0.6672106568924916298020941484e+20,
		0.2316433580634002297931815435e+19,
		-0.3588817569910106050743641413e+17,
		0.2908795263834775409737601689e+15,
		-0.1322983480332126453125473247e+13,
		0.3413234182301700539091292655e+10,
		-0.4695753530642995859767162166e+7,
		0.270112271089232341485679099e+4
	},
	Qone[] =
	{
		0.11623987080032122878585294e+22,
		0.1185770712190320999837113348e+20,
		0.6092061398917521746105196863e+17,
		0.2081661221307607351240184229e+15,
		0.5243710262167649715406728642e+12,
		0.1013863514358673989967045588e+10,
		0.1501793594998585505921097578e+7,
		0.1606931573481487801970916749e+4,
		0.1e+1
	};
		
	p = Pone[8];
	q = Qone[8];
	for (i=7; i >= 0; i--)
	{
		p = p*x*x+Pone[i];
		q = q*x*x+Qone[i];
	}
	return (float)(p/q);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBessel_P1(const float x)
{
	double p, q;
	
	register long i;
	
	static const double
	Pone[] =
	{
		0.352246649133679798341724373e+5,
		0.62758845247161281269005675e+5,
		0.313539631109159574238669888e+5,
		0.49854832060594338434500455e+4,
		0.2111529182853962382105718e+3,
		0.12571716929145341558495e+1
	},
	Qone[] =
	{
		0.352246649133679798068390431e+5,
		0.626943469593560511888833731e+5,
		0.312404063819041039923015703e+5,
		0.4930396490181088979386097e+4,
		0.2030775189134759322293574e+3,
		0.1e+1
	};
		
	p = Pone[5];
	q = Qone[5];
	for (i=4; i >= 0; i--)
	{
		p = p*(8.0/x)*(8.0/x)+Pone[i];
		q = q*(8.0/x)*(8.0/x)+Qone[i];
	}
	return (float)(p/q);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBessel_Q1(const float x)
{
	double p, q;
	
	register long i;
	
	static const double
	Pone[] =
	{
		0.3511751914303552822533318e+3,
		0.7210391804904475039280863e+3,
		0.4259873011654442389886993e+3,
		0.831898957673850827325226e+2,
		0.45681716295512267064405e+1,
		0.3532840052740123642735e-1
	},
	Qone[] =
	{
		0.74917374171809127714519505e+4,
		0.154141773392650970499848051e+5,
		0.91522317015169922705904727e+4,
		0.18111867005523513506724158e+4,
		0.1038187585462133728776636e+3,
		0.1e+1
	};
		
	p = Pone[5];
	q = Qone[5];
	for (i=4; i >= 0; i--)
	{
		p = p*(8.0/x)*(8.0/x)+Pone[i];
		q = q*(8.0/x)*(8.0/x)+Qone[i];
	}
	return (float)(p/q);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBessel_Order1(float x)
{
	float p, q;
	
	if (x == 0.0)
		return (0.0f);
	p = x;
	if (x < 0.0)
		x=(-x);
	if (x < 8.0)
		return(p*KernelBessel_J1(x));
	q = (float)sqrt(2.0f/(CVLIB_PI*x))*(float)(KernelBessel_P1(x)*(1.0f/sqrt(2.0f)*(sin(x)-cos(x)))-8.0f/x*KernelBessel_Q1(x)*
		(-1.0f/sqrt(2.0f)*(sin(x)+cos(x))));
	if (p < 0.0f)
		q = (-q);
	return (q);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelBessel(const float x)
{
	if (x == 0.0f)
		return (float)(CVLIB_PI/4.0f);
	return(KernelBessel_Order1((float)(CVLIB_PI*x))/(2.0f*x));
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelGaussian(const float x)
{
	return (float)(exp(-2.0f*x*x)*0.79788456080287f/*sqrt(2.0f/CVLIB_PI)*/);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelQuadratic(const float x)
{
	if (x < -1.5f)
		return(0.0f);
	if (x < -0.5f)
		return(0.5f*(x+1.5f)*(x+1.5f));
	if (x < 0.5f)
		return(0.75f-x*x);
	if (x < 1.5f)
		return(0.5f*(x-1.5f)*(x-1.5f));
	return(0.0f);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelMitchell(const float x)
{
#define KM_B (1.0f/3.0f)
#define KM_C (1.0f/3.0f)
#define KM_P0 ((  6.0f - 2.0f * KM_B ) / 6.0f)
#define KM_P2 ((-18.0f + 12.0f * KM_B + 6.0f * KM_C) / 6.0f)
#define KM_P3 (( 12.0f - 9.0f  * KM_B - 6.0f * KM_C) / 6.0f)
#define KM_Q0 ((  8.0f * KM_B + 24.0f * KM_C) / 6.0f)
#define KM_Q1 ((-12.0f * KM_B - 48.0f * KM_C) / 6.0f)
#define KM_Q2 ((  6.0f * KM_B + 30.0f * KM_C) / 6.0f)
#define KM_Q3 (( -1.0f * KM_B -  6.0f * KM_C) / 6.0f)
	
	if (x < -2.0)
		return(0.0f);
	if (x < -1.0)
		return(KM_Q0-x*(KM_Q1-x*(KM_Q2-x*KM_Q3)));
	if (x < 0.0f)
		return(KM_P0+x*x*(KM_P2-x*KM_P3));
	if (x < 1.0f)
		return(KM_P0+x*x*(KM_P2+x*KM_P3));
	if (x < 2.0f)
		return(KM_Q0+x*(KM_Q1+x*(KM_Q2+x*KM_Q3)));
	return(0.0f);
}
////////////////////////////////////////////////////////////////////////////////
float ipTransformation::KernelCatrom(const float x)
{
	if (x < -2.0)
		return(0.0f);
	if (x < -1.0)
		return(0.5f*(4.0f+x*(8.0f+x*(5.0f+x))));
	if (x < 0.0)
		return(0.5f*(2.0f+x*x*(-5.0f-3.0f*x)));
	if (x < 1.0)
		return(0.5f*(2.0f+x*x*(-5.0f+3.0f*x)));
	if (x < 2.0)
		return(0.5f*(4.0f+x*(-8.0f+x*(5.0f-x))));
	return(0.0f);
}
//////////////////////////////////////////////////////////////////////////
#define DEF_ColorVariables \
	CoImage tmp;\
	BYTE **ppbSrc1, **ppbSrc2, **ppbSrc3, **ppbDst1, **ppbDst2, **ppbDst3;\
	if (pDst)\
{\
	ppbSrc1 = pSrc->m_matX.data.ptr;\
	ppbSrc2 = pSrc->m_matY.data.ptr;\
	ppbSrc3 = pSrc->m_matZ.data.ptr;\
	ppbDst1 = pDst->m_matX.data.ptr;\
	ppbDst2 = pDst->m_matY.data.ptr;\
	ppbDst3 = pDst->m_matZ.data.ptr;\
}\
	else\
{\
	tmp.Create (*pSrc, TRUE);\
	ppbSrc1 = tmp.m_matX.data.ptr;\
	ppbSrc2 = tmp.m_matY.data.ptr;\
	ppbSrc3 = tmp.m_matZ.data.ptr;\
	ppbDst1 = pSrc->m_matX.data.ptr;\
	ppbDst2 = pSrc->m_matY.data.ptr;\
	ppbDst3 = pSrc->m_matZ.data.ptr;\
}

//////////////////////////////////////////////////////////////////////////
//ipRotate


ipRotate::ipRotate(const ipRotate& from)
{
	m_type = from.m_type;
	m_rAngle = from.m_rAngle;
	m_inMethod = from.m_inMethod;
	m_ofMethod = from.m_ofMethod;
	m_pRepColor = from.m_pRepColor;
	m_fOptimizeRightAngles = from.m_fOptimizeRightAngles;
	m_fKeepOriginalSize = from.m_fKeepOriginalSize;
	m_nAngle = from.m_nAngle;
	m_repColor = from.m_repColor;
}

ipRotate::ipRotate(RotateType nType)
{
	m_type = nType;
	m_nAngle = 0;
}

ipRotate::ipRotate(float rAngle)
{
	m_nAngle = 1;
	m_rAngle = rAngle;
}

ipRotate::ipRotate(float rAngle, InterpolationMethod inMethod/*=IM_BILINEAR*/,
		 OverflowMethod ofMethod/*=OM_BACKGROUND*/, COLORREF *replColor/*=0*/,
		 bool const optimizeRightAngles/*=true*/, bool const bKeepOriginalSize/*=false*/)
{
	m_nAngle = 2;
	m_rAngle = rAngle;
	m_inMethod = inMethod;
	m_ofMethod = ofMethod;
	if (replColor)
	{
		m_pRepColor = &m_repColor;
		m_repColor = *replColor;
	}
	else
		m_pRepColor = NULL;
	m_fOptimizeRightAngles = optimizeRightAngles;
	m_fKeepOriginalSize = bKeepOriginalSize;
}

bool ipRotate::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (m_nAngle == 0)
	{
		switch(m_type)
		{
		case RLeft:
			{
				Mat tempMat(pmSrc->Cols(), pmSrc->Rows(), pmSrc->Type1());
				BYTE **ppbSrc, **ppbDst;
				ppbSrc = pmSrc->data.ptr;
				ppbDst = tempMat.data.ptr;
				if (pmSrc->Channels() == 1)
                {
				for (int y= 0; y < pmSrc->Rows(); y ++)
				{
					for (int x= 0; x < pmSrc->Cols(); x ++)
					{
						ppbDst[pmSrc->Cols()-x-1][y] = ppbSrc[y][x];
					}
				}
                }
                else{
                    int cn=pmSrc->Channels();
					for (int y=0; y<pmSrc->Rows(); y++)for (int x=0; x<pmSrc->Cols(); x++)
					{
                        for (int k=0; k<cn; k++)
                            ppbDst[pmSrc->Cols()-x-1][y*cn+k] = ppbSrc[y][x*cn+k];
					}
                }
				if (pmDst) 
				{
					pmDst->Release();
					pmDst->Create (tempMat, TRUE);
				}
				else
				{
					pmSrc->Release();
					pmSrc->Create (tempMat, TRUE);
				}
			}
			break;
		case RRight:
			{
				Mat tempMat(pmSrc->Cols(), pmSrc->Rows(), pmSrc->Type1());
				BYTE **ppbSrc, **ppbDst;
				ppbSrc = pmSrc->data.ptr;
				ppbDst = tempMat.data.ptr;
				if (pmSrc->Channels() == 1)
				{
					for (int y= 0; y < pmSrc->Rows(); y ++)for (int x= 0; x < pmSrc->Cols(); x ++)
						ppbDst[x][pmSrc->Rows()-y-1] = ppbSrc[y][x];
				}
				else
				{
                    int cn=pmSrc->Channels();
					for (int y=0; y<pmSrc->Rows(); y++)for (int x=0; x<pmSrc->Cols(); x++)
					{
                        for (int k=0; k<cn; k++)
                            ppbDst[x][(pmSrc->Rows()-y-1)*cn+k] = ppbSrc[y][x*cn+k];
					}
				}
				if (pmDst) 
				{
					pmDst->Release();
					pmDst->Create (tempMat, TRUE);
				}
				else
				{
					pmSrc->Release();
					pmSrc->Create (tempMat, TRUE);
				}
			}
			break;
		case R180:
			{
				Mat tempMat(pmSrc->Rows(), pmSrc->Cols(), pmSrc->Type1());
				BYTE **ppbSrc, **ppbDst;
				ppbSrc = pmSrc->data.ptr;
				ppbDst = tempMat.data.ptr;
				
				if (pmSrc->Channels() == 1)
                {
				for (int y= 0; y < pmSrc->Rows(); y ++)
				{
					for (int x= 0; x < pmSrc->Cols(); x ++)
					{
						ppbDst[pmSrc->Rows()-y-1][pmSrc->Cols()-x-1] = ppbSrc[y][x];
					}
				}
                }
                else{
                    int cn=pmSrc->Channels();
                    for (int y= 0; y < pmSrc->Rows(); y ++)for (int x= 0; x < pmSrc->Cols(); x ++)
                    {
                        for (int k=0; k<cn; k++)
                            ppbDst[pmSrc->Rows()-y-1][(pmSrc->Cols()-x-1)*cn+k] = ppbSrc[y][x*cn+k];
                    }
                }
				if (pmDst) 
				{
					pmDst->Release();
					pmDst->Create (tempMat, TRUE);
				}
				else
				{
					pmSrc->Release();
					pmSrc->Create (tempMat, TRUE);
				}
			}
			break;
		}
	}
	else if (m_nAngle == 1)
	{
		double ang = m_rAngle*acos((float)0)/90;
		int newWidth, newHeight;
		int nWidth = pmSrc->Cols();
		int nHeight= pmSrc->Rows();
		double cos_angle = cos(ang);
		double sin_angle = sin(ang);
		
		// Calculate the size of the new bitmap
		SPoint p1={0,0};
		SPoint p2={nWidth,0};
		SPoint p3={0,nHeight};
		SPoint p4={nWidth-1,nHeight};
		SPoint newP1,newP2,newP3,newP4, leftTop, rightTop, leftBottom, rightBottom;
		
		newP1.x = p1.x;
		newP1.y = p1.y;
		newP2.x = (long)CVUtil::Round(p2.x*cos_angle - p2.y*sin_angle);
		newP2.y = (long)CVUtil::Round(p2.x*sin_angle + p2.y*cos_angle);
		newP3.x = (long)CVUtil::Round(p3.x*cos_angle - p3.y*sin_angle);
		newP3.y = (long)CVUtil::Round(p3.x*sin_angle + p3.y*cos_angle);
		newP4.x = (long)CVUtil::Round(p4.x*cos_angle - p4.y*sin_angle);
		newP4.y = (long)CVUtil::Round(p4.x*sin_angle + p4.y*cos_angle);
		
		leftTop.x = MIN(MIN(newP1.x,newP2.x),MIN(newP3.x,newP4.x));
		leftTop.y = MIN(MIN(newP1.y,newP2.y),MIN(newP3.y,newP4.y));
		rightBottom.x = MAX(MAX(newP1.x,newP2.x),MAX(newP3.x,newP4.x));
		rightBottom.y = MAX(MAX(newP1.y,newP2.y),MAX(newP3.y,newP4.y));
		leftBottom.x = leftTop.x;
		leftBottom.y = 2+rightBottom.y;
		rightTop.x = 2+rightBottom.x;
		rightTop.y = leftTop.y;
		
		newWidth = rightTop.x - leftTop.x;
		newHeight= leftBottom.y - leftTop.y;

		Mat tempMat(newHeight, newWidth, pmSrc->Type1());
		
		int x,y,newX,newY,oldX,oldY;

		BYTE **ppbSrc, **ppbDst;
		ppbSrc = pmSrc->data.ptr;
		ppbDst = tempMat.data.ptr;
		
		if (pmSrc->Channels()==1)
		{
			for (y = leftTop.y, newY = 0; y<leftBottom.y; y++,newY++)
			{
				for (x = leftTop.x, newX = 0; x<rightTop.x; x++,newX++)
				{
					oldX = CVUtil::Round(x*cos_angle + y*sin_angle);
					oldY = CVUtil::Round(y*cos_angle - x*sin_angle);
					oldX = oldX < 0 ? 0 : oldX < pmSrc->Cols() ? oldX : pmSrc->Cols()-1;
					oldY = oldY < 0 ? 0 : oldY < pmSrc->Rows() ? oldY : pmSrc->Rows()-1;

					ppbDst[newY][newX] = ppbSrc[oldY][oldX];
				}
			}
		}
		else
		{
			int cn=pmSrc->Channels();
			for (y = leftTop.y, newY = 0; y<leftBottom.y; y++,newY++)
			{
				for (x = leftTop.x, newX = 0; x<rightTop.x; x++,newX++)
				{
					oldX = CVUtil::Round(x*cos_angle + y*sin_angle);
					oldY = CVUtil::Round(y*cos_angle - x*sin_angle);
					oldX = oldX < 0 ? 0 : oldX < pmSrc->Cols() ? oldX : pmSrc->Cols()-1;
					oldY = oldY < 0 ? 0 : oldY < pmSrc->Rows() ? oldY : pmSrc->Rows()-1;

					memcpy(&ppbDst[newY][newX*cn], &ppbSrc[oldY][oldX*cn], cn);
				}
			}
		}
		//select the destination
		if (pmDst) 
		{
			pmDst->Release();
			pmDst->Create (tempMat, TRUE);
		}
		else
		{
			pmSrc->Release();
			pmSrc->Create (tempMat, TRUE);
		}
	}
	else if (m_nAngle == 2)
	{
		double ang = -m_rAngle*acos(0.0f)/90.0f;		//convert angle to radians and invert (positive angle performs clockwise rotation)
		float cos_angle = (float) cos(ang);			//these two are needed later (to rotate)
		float sin_angle = (float) sin(ang);
		
		float cx = (float)(pmSrc->Cols() / 2);
		float cy = (float)(pmSrc->Rows() / 2);

		int i;

		Point2f p[4];
		p[0] = Point2f(-cx, cy);
		p[1] = Point2f(cx, cy);
		p[2] = Point2f(cx, -cy);
		p[3] = Point2f(-cx, -cy);

		Point2f newp[4];								//rotated positions of corners
		for (i = 0; i < 4; i++) 
		{
			newp[i].x = (p[i].x*cos_angle - p[i].y*sin_angle);
			newp[i].y = (p[i].x*sin_angle + p[i].y*cos_angle);
		}

		if (m_rAngle >= 0)
		{
			newp[0].x = newp[0].x / newp[0].y * cy;
			newp[0].y = cy;
			
			newp[1].y = newp[1].y / newp[1].x * cx;
			newp[1].x = cx;
			
			newp[2].x = -newp[2].x / newp[2].y * cy;
			newp[2].y = -cy;
			
			newp[3].y = -newp[3].y / newp[3].x * cx;
			newp[3].x = -cx;

		}
		else
		{
			newp[0].y = -newp[0].y / newp[0].x * cx;
			newp[0].x = -cx;

			newp[1].x = newp[1].x / newp[1].y * cy;
			newp[1].y = cy;

			newp[2].y = newp[2].y / newp[2].x * cx;
			newp[2].x = cx;

			newp[3].x = -newp[3].x / newp[3].y * cy;
			newp[3].y = -cy;

		}

		for (i = 0; i < 4; i++) 
		{
			p[i].x = (newp[i].x*cos_angle + newp[i].y*sin_angle);
			p[i].y = (-newp[i].x*sin_angle + newp[i].y*cos_angle);
		}

		float startx = (float)MAX(p[0].x, p[3].x);
		float starty = (float)MIN(p[0].y, p[1].y);

		float endx = (float)MIN(p[1].x, p[2].x);
		float endy = (float)MAX(p[2].y, p[3].y);

		int newWidth, newHeight;
		newWidth = (int)floor(endx - startx);
		newHeight = (int)floor(starty - endy);
		Mat matTemp(newHeight, newWidth, pmSrc->Type1());

		int y, x;
		int yy, xx;
		int yy1, xx1;
		if (pmSrc->Channels() == 1)
		{
			for(y = 0; y < newHeight; y++)
			{
				for(x = 0; x < newWidth; x++)
				{
					xx = (int)(x + startx);
					yy = (int)(starty - y);
				
					xx1 = (int)floor(cos_angle * xx - sin_angle * yy);
					yy1 = (int)floor(sin_angle * xx + cos_angle * yy);
				
					xx = (int)(xx1 + cx);
					yy = (int)(-yy1 + cy);
				
					matTemp.data.ptr[y][x] = pmSrc->data.ptr[yy][xx];
				}
			}
		}
		else
		{
			int cn=pmSrc->Channels();
			for(y = 0; y < newHeight; y++)
			{
				for(x = 0; x < newWidth; x++)
				{
					xx = (int)(x + startx);
					yy = (int)(starty - y);
				
					xx1 = (int)floor(cos_angle * xx - sin_angle * yy);
					yy1 = (int)floor(sin_angle * xx + cos_angle * yy);
				
					xx = (int)(xx1 + cx);
					yy = (int)(-yy1 + cy);
				
					memcpy (&matTemp.data.ptr[y][x*cn], &pmSrc->data.ptr[yy][xx*cn], cn);
				}
			}
		}

		if (pmDst) 
		{
			pmDst->Release();
			pmDst->Create (matTemp, TRUE);
		}
		else
		{
			pmSrc->Release();
			pmSrc->Create (matTemp, TRUE);
		}
	}
	return true;
}

bool ipRotate::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if (m_nAngle == 0)
	{
		switch(m_type)
		{
		case RLeft:
			{
				CoImage tempImage(pSrc->GetWidth(), pSrc->GetHeight(), pSrc->GetType(), CoImage::enCOLOR_MODE);
				BYTE **ppbSrc1, **ppbSrc2, **ppbSrc3, **ppbDst1, **ppbDst2, **ppbDst3;
				ppbSrc1 = pSrc->m_matX.data.ptr;
				ppbSrc2 = pSrc->m_matY.data.ptr;
				ppbSrc3 = pSrc->m_matZ.data.ptr;
				ppbDst1 = tempImage.m_matX.data.ptr;
				ppbDst2 = tempImage.m_matY.data.ptr;
				ppbDst3 = tempImage.m_matZ.data.ptr;
				
				for (int y= 0; y < pSrc->GetHeight(); y ++)
				{
					for (int x= 0; x < pSrc->GetWidth(); x ++)
					{
						ppbDst1[pSrc->GetWidth()-x-1][y] = ppbSrc1[y][x];
						ppbDst2[pSrc->GetWidth()-x-1][y] = ppbSrc2[y][x];
						ppbDst3[pSrc->GetWidth()-x-1][y] = ppbSrc3[y][x];
					}
				}
				if (pDst) 
				{
					pDst->Release();
					pDst->Create (tempImage, TRUE);
				}
				else
				{
					pSrc->Release();
					pSrc->Create (tempImage, TRUE);
				}
			}
			break;
		case RRight:
			{
				CoImage tempImage(pSrc->GetWidth(), pSrc->GetHeight(), pSrc->GetType(), CoImage::enCOLOR_MODE);
				BYTE **ppbSrc1, **ppbSrc2, **ppbSrc3, **ppbDst1, **ppbDst2, **ppbDst3;
				ppbSrc1 = pSrc->m_matX.data.ptr;
				ppbSrc2 = pSrc->m_matY.data.ptr;
				ppbSrc3 = pSrc->m_matZ.data.ptr;
				ppbDst1 = tempImage.m_matX.data.ptr;
				ppbDst2 = tempImage.m_matY.data.ptr;
				ppbDst3 = tempImage.m_matZ.data.ptr;
					
				for (int y= 0; y < pSrc->GetHeight(); y ++)
				{
					for (int x= 0; x < pSrc->GetWidth(); x ++)
					{
						ppbDst1[x][pSrc->GetHeight()-y-1] = ppbSrc1[y][x];
						ppbDst2[x][pSrc->GetHeight()-y-1] = ppbSrc2[y][x];
						ppbDst3[x][pSrc->GetHeight()-y-1] = ppbSrc3[y][x];
					}
				}
				if (pDst) 
				{
					pDst->Release();
					pDst->Create (tempImage, TRUE);
				}
				else
				{
					pSrc->Release();
					pSrc->Create (tempImage, TRUE);
				}
			}
			break;
		case R180:
			{
				if (pDst && !EqualTypeSize(pDst, pSrc))
				{
					pDst->Release();
					pDst->Create(*pSrc);
				}
				DEF_ColorVariables
					
				for (int y= 0; y < pSrc->GetHeight(); y ++)
				{
					for (int x= 0; x < pSrc->GetWidth(); x ++)
					{
						ppbDst1[pSrc->GetHeight()-y-1][pSrc->GetWidth()-x-1] = ppbSrc1[y][x];
						ppbDst2[pSrc->GetHeight()-y-1][pSrc->GetWidth()-x-1] = ppbSrc2[y][x];
						ppbDst3[pSrc->GetHeight()-y-1][pSrc->GetWidth()-x-1] = ppbSrc3[y][x];
					}
				}
			}
			break;
		}
	}
	else if (m_nAngle == 1)
	{
		double ang = m_rAngle*acos((float)0)/90;
		int newWidth, newHeight;
		int nWidth = pSrc->GetWidth();
		int nHeight= pSrc->GetHeight();
		double cos_angle = cos(ang);
		double sin_angle = sin(ang);
		
		// Calculate the size of the new bitmap
		SPoint p1={0,0};
		SPoint p2={nWidth,0};
		SPoint p3={0,nHeight};
		SPoint p4={nWidth-1,nHeight};
		SPoint newP1,newP2,newP3,newP4, leftTop, rightTop, leftBottom, rightBottom;
		
		newP1.x = p1.x;
		newP1.y = p1.y;
		newP2.x = (long)CVUtil::Round(p2.x*cos_angle - p2.y*sin_angle);
		newP2.y = (long)CVUtil::Round(p2.x*sin_angle + p2.y*cos_angle);
		newP3.x = (long)CVUtil::Round(p3.x*cos_angle - p3.y*sin_angle);
		newP3.y = (long)CVUtil::Round(p3.x*sin_angle + p3.y*cos_angle);
		newP4.x = (long)CVUtil::Round(p4.x*cos_angle - p4.y*sin_angle);
		newP4.y = (long)CVUtil::Round(p4.x*sin_angle + p4.y*cos_angle);
		
		leftTop.x = MIN(MIN(newP1.x,newP2.x),MIN(newP3.x,newP4.x));
		leftTop.y = MIN(MIN(newP1.y,newP2.y),MIN(newP3.y,newP4.y));
		rightBottom.x = MAX(MAX(newP1.x,newP2.x),MAX(newP3.x,newP4.x));
		rightBottom.y = MAX(MAX(newP1.y,newP2.y),MAX(newP3.y,newP4.y));
		leftBottom.x = leftTop.x;
		leftBottom.y = 2+rightBottom.y;
		rightTop.x = 2+rightBottom.x;
		rightTop.y = leftTop.y;
		
		newWidth = rightTop.x - leftTop.x;
		newHeight= leftBottom.y - leftTop.y;

		CoImage imgDest;
		imgDest.Create(newHeight, newWidth, MAT_Tbyte, CoImage::enCOLOR_MODE);
		
		int x,y,newX,newY,oldX,oldY;

		BYTE **ppbSrc1, **ppbSrc2, **ppbSrc3, **ppbDst1, **ppbDst2, **ppbDst3;
		ppbSrc1 = pSrc->m_matX.data.ptr;
		ppbSrc2 = pSrc->m_matY.data.ptr;
		ppbSrc3 = pSrc->m_matZ.data.ptr;
		ppbDst1 = imgDest.m_matX.data.ptr;
		ppbDst2 = imgDest.m_matY.data.ptr;
		ppbDst3 = imgDest.m_matZ.data.ptr;
		
		for (y = leftTop.y, newY = 0; y<leftBottom.y; y++,newY++)
		{
			for (x = leftTop.x, newX = 0; x<rightTop.x; x++,newX++)
			{
				oldX = CVUtil::Round(x*cos_angle + y*sin_angle);
				oldY = CVUtil::Round(y*cos_angle - x*sin_angle);
				oldX = oldX < 0 ? 0 : oldX < pSrc->GetWidth() ? oldX : pSrc->GetWidth()-1;
				oldY = oldY < 0 ? 0 : oldY < pSrc->GetHeight() ? oldY : pSrc->GetHeight()-1;

				ppbDst1[newY][newX] = ppbSrc1[oldY][oldX];
				ppbDst2[newY][newX] = ppbSrc2[oldY][oldX];
				ppbDst3[newY][newX] = ppbSrc3[oldY][oldX];
			}
		}
		//select the destination
		if (pDst) 
		{
			pDst->Release();
			pDst->Create (imgDest, TRUE);
		}
		else
		{
			pSrc->Release();
			pSrc->Create (imgDest, TRUE);
		}
	}
	else if (m_nAngle == 2)
	{
		double ang = -m_rAngle*acos(0.0f)/90.0f;		//convert angle to radians and invert (positive angle performs clockwise rotation)
		float cos_angle = (float) cos(ang);			//these two are needed later (to rotate)
		float sin_angle = (float) sin(ang);
		
		float cx = (float)(pSrc->GetWidth() / 2);
		float cy = (float)(pSrc->GetHeight() / 2);

		int i;

		Point2f p[4];
		p[0] = Point2f(-cx, cy);
		p[1] = Point2f(cx, cy);
		p[2] = Point2f(cx, -cy);
		p[3] = Point2f(-cx, -cy);

		Point2f newp[4];								//rotated positions of corners
		for (i = 0; i < 4; i++) 
		{
			newp[i].x = (p[i].x*cos_angle - p[i].y*sin_angle);
			newp[i].y = (p[i].x*sin_angle + p[i].y*cos_angle);
		}

		if (m_rAngle >= 0)
		{
			newp[0].x = newp[0].x / newp[0].y * cy;
			newp[0].y = cy;
			
			newp[1].y = newp[1].y / newp[1].x * cx;
			newp[1].x = cx;
			
			newp[2].x = -newp[2].x / newp[2].y * cy;
			newp[2].y = -cy;
			
			newp[3].y = -newp[3].y / newp[3].x * cx;
			newp[3].x = -cx;

		}
		else
		{
			newp[0].y = -newp[0].y / newp[0].x * cx;
			newp[0].x = -cx;

			newp[1].x = newp[1].x / newp[1].y * cy;
			newp[1].y = cy;

			newp[2].y = newp[2].y / newp[2].x * cx;
			newp[2].x = cx;

			newp[3].x = -newp[3].x / newp[3].y * cy;
			newp[3].y = -cy;

		}

		for (i = 0; i < 4; i++) 
		{
			p[i].x = (newp[i].x*cos_angle + newp[i].y*sin_angle);
			p[i].y = (-newp[i].x*sin_angle + newp[i].y*cos_angle);
		}

		float startx = (float)MAX(p[0].x, p[3].x);
		float starty = (float)MIN(p[0].y, p[1].y);

		float endx = (float)MIN(p[1].x, p[2].x);
		float endy = (float)MAX(p[2].y, p[3].y);

		int newWidth, newHeight;
		newWidth = (int)floor(endx - startx);
		newHeight = (int)floor(starty - endy);
		CoImage destImage(newHeight, newWidth, pSrc->GetType(), CoImage::enCOLOR_MODE);

		int y, x;
		int yy, xx;
		int yy1, xx1;
		for(y = 0; y < newHeight; y++)
			for(x = 0; x < newWidth; x++)
			{
				xx = (int)(x + startx);
				yy = (int)(starty - y);
				
				xx1 = (int)floor(cos_angle * xx - sin_angle * yy);
				yy1 = (int)floor(sin_angle * xx + cos_angle * yy);
				
				xx = (int)(xx1 + cx);
				yy = (int)(-yy1 + cy);
				
				destImage.m_matX.data.ptr[y][x] = pSrc->m_matX.data.ptr[yy][xx];
				destImage.m_matY.data.ptr[y][x] = pSrc->m_matY.data.ptr[yy][xx];
				destImage.m_matZ.data.ptr[y][x] = pSrc->m_matZ.data.ptr[yy][xx];				
 
			}
/*		
		//Calculate the size of the new bitmap (rotate corners of image)
		Point2f p[4];								//original corners of the image
		p[0]=Point2f(-0.5f,-0.5f);
		p[1]=Point2f(pSrc->GetWidth()-0.5f,-0.5f);
		p[2]=Point2f(-0.5f,pSrc->GetHeight()-0.5f);
		p[3]=Point2f(pSrc->GetWidth()-0.5f,pSrc->GetHeight()-0.5f);
		Point2f newp[4];								//rotated positions of corners
		//(rotate corners)
		if (m_fKeepOriginalSize)
		{
			for (int i=0; i<4; i++) 
			{
				newp[i].x = p[i].x;
				newp[i].y = p[i].y;
			}//for
		} 
		else 
		{
			for (int i=0; i<4; i++) 
			{
				newp[i].x = (p[i].x*cos_angle - p[i].y*sin_angle);
				newp[i].y = (p[i].x*sin_angle + p[i].y*cos_angle);
			}//for i
			
			if (m_fOptimizeRightAngles) 
			{ 
				//For rotations of 90, -90 or 180 or 0 degrees, call faster routines
				if (newp[3].Distance(Point2f(pSrc->GetHeight()-0.5f, 0.5f-pSrc->GetWidth())) < 0.25) 
				{
					//rotation right for circa 90 degrees (diagonal pixels less than 0.25 pixel away from 90 degree rotation destination)
					ipRotate rot(RRight);
					return rot.Process(pSrc, pDst);
				}
				if (newp[3].Distance(Point2f(0.5f-pSrc->GetHeight(), -0.5f+pSrc->GetWidth())) < 0.25) 
				{
					//rotation left for ~90 degrees
					ipRotate rot(RLeft);
					return rot.Process(pSrc, pDst);
				}
				if (newp[3].Distance(Point2f(0.5f-pSrc->GetWidth(), 0.5f-pSrc->GetHeight())) < 0.25) 
				{
					//rotation left for ~180 degrees
					ipRotate rot(R180);
					return rot.Process(pSrc, pDst);
				}
				if (newp[3].Distance(p[3]) < 0.25)
				{
					//rotation not significant
					if (pDst)
					{
						pDst->Release();
						pDst->Create (*pSrc, TRUE);
					}
					return true;						//and we're done
				}//if
			}//if
		}//if

		//(read new dimensions from location of corners)
		float minx = (float) MIN(MIN(newp[0].x,newp[1].x),MIN(newp[2].x,newp[3].x));
		float miny = (float) MIN(MIN(newp[0].y,newp[1].y),MIN(newp[2].y,newp[3].y));
		float maxx = (float) MAX(MAX(newp[0].x,newp[1].x),MAX(newp[2].x,newp[3].x));
		float maxy = (float) MAX(MAX(newp[0].y,newp[1].y),MAX(newp[2].y,newp[3].y));


		int newWidth = (int) floor(maxx-minx+0.5f);
		int newHeight= (int) floor(maxy-miny+0.5f);
		float ssx=((maxx+minx)- ((float) newWidth-1))/2.0f;   //start for x
		float ssy=((maxy+miny)- ((float) newHeight-1))/2.0f;  //start for y

		float newxcenteroffset = 0.5f * newWidth;
		float newycenteroffset = 0.5f * newHeight;
		if (m_fKeepOriginalSize)
		{
			ssx -= 0.5f * pSrc->GetWidth();
			ssy -= 0.5f * pSrc->GetHeight();
		}

		//create destination image
		CoImage imgDest (newHeight, ne0wWidth, MAT_Tbyte, CoImage::enCOLOR_MODE);
		
		COLORREF rgb;			//pixel colour
		COLORREF rc;
		if (m_pRepColor!=0) 
			rc=*m_pRepColor; 
		else
		{
			rgb = RGB(255,255,255);
		}//if
		float x,y;              //destination location (float, with proper offset)
		float origx, origy;     //origin location
		int destx, desty;       //destination location
		
		y=ssy;                  //initialize y

		{
			//optimized RGB24 implementation (direct write to destination):
			BYTE *pbR, *pbG, *pbB;
			for (desty=0; desty<newHeight; desty++)
			{
				//initialize x
				x=ssx;
				//calculate pointer to first byte in row
				pbR = imgDest.m_matX.data.ptr[desty];
				pbG = imgDest.m_matY.data.ptr[desty];
				pbB = imgDest.m_matZ.data.ptr[desty];
				for (destx=0; destx<newWidth; destx++)
				{
					//get source pixel coordinate for current destination point
					//origx = (cos_angle*(x-pSrc->GetWidth()/2)+sin_angle*(y-pSrc->GetHeight()/2))+newWidth/2;
					//origy = (cos_angle*(y-pSrc->GetHeight()/2)-sin_angle*(x-pSrc->GetWidth()/2))+newHeight/2;
					origx = cos_angle*x+sin_angle*y;
					origy = cos_angle*y-sin_angle*x;
					if (m_fKeepOriginalSize)
					{
						origx += newxcenteroffset;
						origy += newycenteroffset;
					}
					rgb = GetPixelColorInterpolated(pSrc, origx, origy, m_inMethod, m_ofMethod, &rc);   //get interpolated colour value
					//copy alpha and colour value to destination
					*pbB++ = GetBValue(rgb);
					*pbG++ = GetGValue(rgb);
					*pbR++ = GetRValue(rgb);
					x++;
				}//for destx
				y++;
			}//for desty
		}
		//select the destination
*/		
		if (pDst) 
		{
			pDst->Release();
			pDst->Create (destImage, TRUE);
		}
		else
		{
			pSrc->Release();
			pSrc->Create (destImage, TRUE);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//ipResample
ipResample::ipResample(long newx, long newy, int mode /*= 1*/)
{
	m_newx = newx;
	m_newy = newy;
	m_nMode = mode;
	m_nMethod = 1;
}

ipResample::ipResample(long newx, long newy, InterpolationMethod const inMethod/*=IM_BICUBIC2*/,
		   OverflowMethod const ofMethod/*=OM_REPEAT*/, bool const disableAveraging/*=false*/)
{
	m_newx = newx;
	m_newy = newy;
	m_inMethod = inMethod;
	m_ofMethod = ofMethod;
	m_fDisableAveraging = disableAveraging;
	m_nMethod = 2;
}

ipResample::ipResample(const ipResample& from)
{
	m_newx = from.m_newx;
	m_newy = from.m_newy;
	m_inMethod = from.m_inMethod;
	m_ofMethod = from.m_ofMethod;
	m_fDisableAveraging = from.m_fDisableAveraging;
	m_nMethod = from.m_nMethod;
}

bool ipResample::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	CoImage srcImage(*pmSrc, TRUE);
	Process (&srcImage);
	if (pmSrc->Channels()==1)
	{
		if (pmDst)
		{
			pmDst->Release();
			pmDst->Create(srcImage.m_matX, TRUE);
		}
		else
		{
			pmSrc->Release();
			pmSrc->Create(srcImage.m_matX, TRUE);
		}
	}
	else if (pmSrc->Channels()==3)
	{
		if (pmDst)
			*pmDst = (Mat)srcImage;
		else
			*pmSrc = (Mat)srcImage;
	}

	return true;
}

bool ipResample::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if ( pDst && !EqualType(pSrc, pDst) )
	{
		pDst->Release();
		pDst->Create(*pSrc);
	}

	if (m_nMethod == 1)
	{
		if (m_newx==0 || m_newy==0) return false;

		if (pSrc->GetWidth()==m_newx && pSrc->GetHeight()==m_newy)
		{
			if (pDst)
			{
				pDst->Release();
				pDst->Create (*pSrc, TRUE);
			}
			return true;
		}

		float xScale, yScale, fX, fY;
		xScale = (float)pSrc->GetWidth()  / (float)m_newx;
		yScale = (float)pSrc->GetHeight() / (float)m_newy;

		CoImage newImage(m_newy, m_newx, MAT_Tbyte, CoImage::enCOLOR_MODE);
		if (!newImage.IsValid()) 
			return false;

		switch (m_nMode) 
		{
		case 1: // nearest pixel
		{ 
			for(long y=0; y<m_newy; y++)
			{
				fY = y * yScale;
				for(long x=0; x<m_newx; x++)
				{
					fX = x * xScale;
					newImage.SetPixelColor(x,y,pSrc->GetPixelColor((long)fX,(long)fY));
				}
			}
			break;
		}
		case 2: // bicubic interpolation by Blake L. Carlson <blake-carlson(at)uiowa(dot)edu
		{
			float f_x, f_y, a, b, rr, gg, bb, r1, r2;
			int   i_x, i_y, xx, yy;
			BYTE** ppbSrcR = pSrc->m_matX.data.ptr;
			BYTE** ppbSrcG = pSrc->m_matY.data.ptr;
			BYTE** ppbSrcB = pSrc->m_matZ.data.ptr;
			BYTE** ppbDstR = newImage.m_matX.data.ptr;
			BYTE** ppbDstG = newImage.m_matY.data.ptr;
			BYTE** ppbDstB = newImage.m_matZ.data.ptr;

			for(long y=0; y<m_newy; y++)
			{
				f_y = (float) y * yScale - 0.5f;
				i_y = (int) floor(f_y);
				a   = f_y - (float)floor(f_y);
				for(long x=0; x<m_newx; x++)
				{
					f_x = (float) x * xScale - 0.5f;
					i_x = (int) floor(f_x);
					b   = f_x - (float)floor(f_x);

					rr = gg = bb = 0.0f;
					for(int m=-1; m<3; m++) 
					{
						r1 = KernelBSpline((float) m - a);
						yy = i_y+m;
						if (yy<0) 
							yy=0;
						if (yy>=pSrc->GetHeight()) 
							yy = pSrc->GetHeight()-1;
						for(int n=-1; n<3; n++) 
						{
							r2 = r1 * KernelBSpline(b - (float)n);
							xx = i_x+n;
							if (xx<0) xx=0;
							if (xx>=pSrc->GetWidth()) 
								xx=pSrc->GetWidth()-1;

							rr += ppbSrcR[yy][xx] * r2;
							gg += ppbSrcG[yy][xx] * r2;
							bb += ppbSrcB[yy][xx] * r2;
						}
					}

					ppbDstR[y][x] = (BYTE)rr;
					ppbDstG[y][x] = (BYTE)gg;
					ppbDstB[y][x] = (BYTE)bb;
				}
			}
			break;
		}
		default: // bilinear interpolation
			if (!(pSrc->GetWidth()>m_newx && pSrc->GetHeight()>m_newy)) 
			{
				//?1999 Steve McMahon (steve@dogma.demon.co.uk)
				long ifX, ifY, ifX1, ifY1, xmax, ymax;
				float ir1, ir2, ig1, ig2, ib1, ib2, dx, dy;
				xmax = pSrc->GetWidth()-1;
				ymax = pSrc->GetHeight()-1;
				BYTE** ppbR = pSrc->m_matX.data.ptr;
				BYTE** ppbG = pSrc->m_matY.data.ptr;
				BYTE** ppbB = pSrc->m_matZ.data.ptr;
				BYTE** ppbDstR = newImage.m_matX.data.ptr;
				BYTE** ppbDstG = newImage.m_matY.data.ptr;
				BYTE** ppbDstB = newImage.m_matZ.data.ptr;
				for(long y=0; y<m_newy; y++)
				{
					fY = y * yScale;
					ifY = (int)fY;
					ifY1 = MIN(ymax, ifY+1);
					dy = fY - ifY;
					for(long x=0; x<m_newx; x++)
					{
						fX = x * xScale;
						ifX = (int)fX;
						ifX1 = MIN(xmax, ifX+1);
						dx = fX - ifX;
						// Interpolate using the four nearest pixels in the source
						// Interplate in x direction:
						ir1 = ppbR[ifY][ifX]  * (1 - dy) + ppbR[ifY1][ifX] * dy;
						ig1 = ppbG[ifY][ifX]  * (1 - dy) + ppbG[ifY1][ifX] * dy;
						ib1 = ppbB[ifY][ifX]  * (1 - dy) + ppbB[ifY1][ifX] * dy;
						ir2 = ppbR[ifY][ifX1]  * (1 - dy) + ppbR[ifY1][ifX1] * dy;
						ig2 = ppbG[ifY][ifX1]  * (1 - dy) + ppbG[ifY1][ifX1] * dy;
						ib2 = ppbB[ifY][ifX1]  * (1 - dy) + ppbB[ifY1][ifX1] * dy;
						// Interpolate in y:
						ppbDstR[y][x] = (BYTE)(ir1 * (1 - dx) + ir2 * dx);
						ppbDstG[y][x] = (BYTE)(ig1 * (1 - dx) + ig2 * dx);
						ppbDstB[y][x] = (BYTE)(ib1 * (1 - dx) + ib2 * dx);
					}
				} 
			}
			else 
			{
// 				//high resolution shrink, thanks to Henrik Stellmann <henrik.stellmann@volleynet.de>
// 				const long ACCURACY = 1000;
// 				long i,j; // index for faValue
// 				long x,y; // coordinates in  source image
// 				BYTE* pSource;
// 				BYTE* pDest = newImage.m_Info.pImage;
// 				long* naAccu  = new long[3 * m_newx + 3];
// 				long* naCarry = new long[3 * m_newx + 3];
// 				long* naTemp;
// 				long  nWeightX,nWeightY;
// 				float fEndX;
// 				long nScale = (long)(ACCURACY * xScale * yScale);
// 
// 				memset(naAccu,  0, sizeof(long) * 3 * m_newx);
// 				memset(naCarry, 0, sizeof(long) * 3 * m_newx);
// 
// 				int u, v = 0; // coordinates in dest image
// 				float fEndY = yScale - 1.0f;
// 				for (y = 0; y < pSrc->GetHeight(); y++)
// 				{
// 					pSource = info.pImage + y * info.dwEffWidth;
// 					u = i = 0;
// 					fEndX = xScale - 1.0f;
// 					if ((float)y < fEndY) 
// 					{       // complete source row goes into dest row
// 						for (x = 0; x < pSrc->GetWidth(); x++)
// 						{
// 							if ((float)x < fEndX)
// 							{       // complete source pixel goes into dest pixel
// 								for (j = 0; j < 3; j++)	
// 									naAccu[i + j] += (*pSource++) * ACCURACY;
// 							} 
// 							else 
// 							{       // source pixel is splitted for 2 dest pixels
// 								nWeightX = (long)(((float)x - fEndX) * ACCURACY);
// 								for (j = 0; j < 3; j++)
// 								{
// 									naAccu[i] += (ACCURACY - nWeightX) * (*pSource);
// 									naAccu[3 + i++] += nWeightX * (*pSource++);
// 								}
// 								fEndX += xScale;
// 								u++;
// 							}
// 						}
// 					}
// 					else 
// 					{       // source row is splitted for 2 dest rows       
// 						nWeightY = (long)(((float)y - fEndY) * ACCURACY);
// 						for (x = 0; x < pSrc->GetWidth(); x++)
// 						{
// 							if ((float)x < fEndX)
// 							{       // complete source pixel goes into 2 pixel
// 								for (j = 0; j < 3; j++)
// 								{
// 									naAccu[i + j] += ((ACCURACY - nWeightY) * (*pSource));
// 									naCarry[i + j] += nWeightY * (*pSource++);
// 								}
// 							} 
// 							else 
// 							{       // source pixel is splitted for 4 dest pixels
// 								nWeightX = (int)(((float)x - fEndX) * ACCURACY);
// 								for (j = 0; j < 3; j++) 
// 								{
// 									naAccu[i] += ((ACCURACY - nWeightY) * (ACCURACY - nWeightX)) * (*pSource) / ACCURACY;
// 									*pDest++ = (BYTE)(naAccu[i] / nScale);
// 									naCarry[i] += (nWeightY * (ACCURACY - nWeightX) * (*pSource)) / ACCURACY;
// 									naAccu[i + 3] += ((ACCURACY - nWeightY) * nWeightX * (*pSource)) / ACCURACY;
// 									naCarry[i + 3] = (nWeightY * nWeightX * (*pSource)) / ACCURACY;
// 									i++;
// 									pSource++;
// 								}
// 								fEndX += xScale;
// 								u++;
// 							}
// 						}
// 						if (u < m_newx)
// 						{ // possibly not completed due to rounding errors
// 							for (j = 0; j < 3; j++) *pDest++ = (BYTE)(naAccu[i++] / nScale);
// 						}
// 						naTemp = naCarry;
// 						naCarry = naAccu;
// 						naAccu = naTemp;
// 						memset(naCarry, 0, sizeof(int) * 3);    // need only to set first pixel zero
// 						pDest = newImage.info.pImage + (++v * newImage.info.dwEffWidth);
// 						fEndY += yScale;
// 					}
// 				}
// 				if (v < m_newy)
// 				{	// possibly not completed due to rounding errors
// 					for (i = 0; i < 3 * m_newx; i++) *pDest++ = (BYTE)(naAccu[i] / nScale);
// 				}
// 				delete [] naAccu;
// 				delete [] naCarry;
			}
		}

		//select the destination
		if (pDst)
		{
			pDst->Release();
			pDst->Create(newImage, TRUE);
		}
		else 
		{
			pSrc->Release();
			pSrc->Create (newImage, TRUE);
		}

		return true;
	}
	else if (m_nMethod == 2)
	{
		if (m_newx<=0 || m_newy<=0 )
			return false;
		
		if (pSrc->GetWidth()==m_newx && pSrc->GetHeight()==m_newy) 
		{
			//image already correct size (just copy and return)
			if (pDst)
			{
				pDst->Release();
				pDst->Create (*pSrc, TRUE);
			}
			return true;
		}//if
		
		//calculate scale of new image (less than 1 for enlarge)
		float xScale, yScale;
		xScale = (float)pSrc->GetWidth()  / (float)m_newx;    
		yScale = (float)pSrc->GetHeight() / (float)m_newy;
		
		//create temporary destination image
		CoImage newImage(m_newy,m_newx,MAT_Tbyte,CoImage::enCOLOR_MODE);
		if (!newImage.IsValid()) return false;
		
		float sX, sY;         //source location
		long dX,dY;           //destination pixel (int value)
		if ((xScale<=1 && yScale<=1) || m_fDisableAveraging) 
		{
			//image is being enlarged (or interpolation on demand)
			//RGB24 image (optimized version with direct writes)
			for(dY=0; dY<m_newy; dY++)
			{
				sY = (dY + 0.5f) * yScale - 0.5f;
				BYTE* pbR = newImage.m_matX.data.ptr[dY];
				BYTE* pbG = newImage.m_matY.data.ptr[dY];
				BYTE* pbB = newImage.m_matZ.data.ptr[dY];
				for(dX=0; dX<m_newx; dX++)
				{
					sX = (dX + 0.5f) * xScale - 0.5f;
					COLORREF q=GetPixelColorInterpolated(pSrc, sX,sY,m_inMethod,m_ofMethod,0);
					pbR[dX] = GetRValue(q);
					pbG[dX] = GetGValue(q);
					pbB[dX] = GetBValue(q);
				}//for dX
			}//for dY
		} 
		else 
		{
			//image size is being reduced (averaging enabled)
			for(dY=0; dY<m_newy; dY++)
			{
				sY = (dY+0.5f) * yScale - 0.5f;
				for(dX=0; dX<m_newx; dX++)
				{
					sX = (dX+0.5f) * xScale - 0.5f;
					newImage.SetPixelColor(dX,dY,GetAreaColorInterpolated(pSrc, sX, sY, xScale, yScale, m_inMethod, m_ofMethod,0));
				}//for x
			}//for y
		}//if
		
		//copy new image to the destination
		if (pDst) 
		{
			pDst->Release();
			pDst->Create (newImage, TRUE);
		}
		else 
		{
			pSrc->Release();
			pSrc->Create(newImage, TRUE);
		}
		return true;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//ipCrop

ipCrop::ipCrop(const ipCrop& from)
{
	m_rect=from.m_rect;
	m_nMethod=from.m_nMethod;
	m_rAngle=from.m_rAngle;
	m_fAngle=from.m_fAngle;
}

ipCrop::ipCrop(long left, long top, long right, long bottom)
{
	m_rect.x = left;
	m_rect.y = top;
	m_rect.width = right - left;
	m_rect.height = bottom - top;
	m_nMethod = 1;
}

ipCrop::ipCrop(const Rect& rect)
{
	m_rect = rect;
	m_nMethod = 1;
}

ipCrop::ipCrop(long topx, long topy, long width, long height, float angle)
{
	m_rect.x = topx;
	m_rect.y = topy;
	m_rect.width = width;
	m_rect.height = height;
	m_rAngle = angle;
	m_nMethod = 2;
}

bool ipCrop::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (m_nMethod == 1)
	{
		int i;
		int nRows = m_rect.height;
		int nCols = m_rect.width;
		int topx = m_rect.x;
		int topy = m_rect.y;
		
		if ( pmSrc->IsInterior( topx, topy )==false)
			return false;		
		if ( pmSrc->IsInterior( topx + nCols, topy )==false)
			return false;		
		if ( pmSrc->IsInterior( topx, topy + nRows )==false)
			return false;		
		if ( pmSrc->IsInterior( topx + nCols, topy + nRows )==false)
			return false;		

		if (nRows <= 0 || nCols <= 0)
			return false;

		Mat destImage(nRows, nCols, pmSrc->Type());
		
		for (i = 0; i < nRows; i ++)
		{
			memcpy (destImage.data.ptr[i], 
				pmSrc->data.ptr[i+m_rect.y] + m_rect.x * pmSrc->Step(), pmSrc->Step() * nCols);
		}	
		if (pmDst) 
		{
			pmDst->Release();
			pmDst->Create (destImage, TRUE);
		}
		else 
		{
			pmSrc->Release();
			pmSrc->Create(destImage, TRUE);
		}
	}
	else
	{
		m_rAngle = m_rAngle * (float)(acos(float(0)) / 90);

		double cos_angle = cos(m_rAngle/*/57.295779513082320877*/);
		double sin_angle = sin(m_rAngle/*/57.295779513082320877*/);
		
		// if there is nothing special, call the original Crop():
		if ( fabs(m_rAngle)<0.0002 )
		{
			ipCrop crop(m_rect);
			return crop.Process(pmSrc);
		}

		long cx = m_rect.x + m_rect.width / 2;
		long cy = m_rect.y + m_rect.height / 2;
		
		SPoint p1 = {-m_rect.width / 2, m_rect.height / 2};
		SPoint p2 = {m_rect.width / 2, m_rect.height / 2};
		SPoint p3 = {m_rect.width / 2, -m_rect.height / 2};
		SPoint p4 = {-m_rect.width / 2, -m_rect.height / 2};
	
		SPoint np1, np2, np3, np4;
		np1.x = (long)floor(cos_angle * p1.x - sin_angle * p1.y);
		np1.y = (long)floor(sin_angle * p1.x + cos_angle * p1.y);
		
		np1.x += cx;
		np1.y = -np1.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pmSrc->IsInterior( np1.x, np1.y )==false)
			return false;		

		np2.x = (long)floor(cos_angle * p2.x - sin_angle * p2.y);
		np2.y = (long)floor(sin_angle * p2.x + cos_angle * p2.y);

		np2.x += cx;
		np2.y = -np2.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pmSrc->IsInterior( np2.x, np2.y )==false)
			return false;		

		np3.x = (long)floor(cos_angle * p3.x - sin_angle * p3.y);
		np3.y = (long)floor(sin_angle * p3.x + cos_angle * p3.y);

		np3.x += cx;
		np3.y = -np3.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pmSrc->IsInterior( np3.x, np3.y )==false)
			return false;		

		np4.x = (long)floor(cos_angle * p4.x - sin_angle * p4.y);
		np4.y = (long)floor(sin_angle * p4.x + cos_angle * p4.y);

		np4.x += cx;
		np4.y = -np4.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pmSrc->IsInterior( np4.x, np4.y )==false)
			return false;		

		Mat destImage(m_rect.height, m_rect.width , pmSrc->Type());

		long y, x;
		long xx, yy;
		long xx1, yy1;
		for(y = 0; y < m_rect.height; y++)
			for(x = 0; x < m_rect.width; x++)
			{
				xx = x + m_rect.x - cx;
				yy = cy - y - m_rect.y;

				xx1 = (long)floor(cos_angle * xx - sin_angle * yy);
				yy1 = (long)floor(sin_angle * xx + cos_angle * yy);

				xx = xx1 + cx;
				yy = -yy1 + cy;

				destImage.data.ptr[y][x] = pmSrc->data.ptr[yy][xx];
			}
		m_rAngle = m_rAngle *  (float)(90 / acos(float(0)));
		if ( m_rAngle  > 45 && m_rAngle < 135 )
		{
			ipRotate rot(90, (CVLib::InterpolationMethod)1, (CVLib::OverflowMethod)2, NULL, TRUE, FALSE);
			rot.Process(&destImage);
		}

		if (pmDst) 
		{
			pmDst->Release();
			pmDst->Create (destImage, TRUE);
		}
		else 
		{
			pmSrc->Release();
			pmSrc->Create(destImage, TRUE);
		}

/*	
		startx = MIN(m_rect.x, m_rect.x - (long)(sin_angle*(double)m_rect.height));
		endx   = m_rect.x + (long)(cos_angle*(double)m_rect.width);
		endy   = m_rect.y + (long)(cos_angle*(double)m_rect.height + sin_angle*(double)m_rect.width);
		// check: corners of the rectangle must be inside
 		if ( pSrc->IsInterior( startx, m_rect.y )==false || pSrc->IsInterior( endx, endy ) == false )
 			return false;
		
		// first crop to bounding rectangle
		CoImage tmp;
		ipCrop crop(startx, m_rect.y, endx, endy);
		if (false == crop.Process (pSrc, &tmp))
			return false;
		
		// the midpoint of the image now became the same as the midpoint of the rectangle
		// rotate new image with minus angle amount
		ipRotate rot((float)(-m_rAngle*57.295779513082320877));
		if ( false == rot.Process (&tmp) )
			return false;

		// crop rotated image to the original selection rectangle
		endx   = (tmp.GetWidth() + pSrc->GetWidth())/2;
		startx = (tmp.GetWidth() - pSrc->GetWidth())/2;
		starty = (tmp.GetHeight() + pSrc->GetHeight())/2;
		endy   = (tmp.GetHeight() - pSrc->GetHeight())/2;
		ipCrop crop1(startx, starty, endx, endy);
		if ( false == crop1.Process( &tmp ) )
			return false;
		
		if (pDst) 
		{
			pDst->Release();
			pDst->Create (tmp, TRUE);
		}
		else 
		{
			pSrc->Release();
			pSrc->Create(tmp, TRUE);
		}
*/		
		return true;
	}

	return true;
}

bool ipCrop::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if (m_nMethod == 1)
	{
		int i;
		int nRows = m_rect.height;
		int nCols = m_rect.width;
		int topx = m_rect.x;
		int topy = m_rect.y;
		
		if ( pSrc->IsInterior( topx, topy )==false)
			return false;		
		if ( pSrc->IsInterior( topx + nCols, topy )==false)
			return false;		
		if ( pSrc->IsInterior( topx, topy + nRows )==false)
			return false;		
		if ( pSrc->IsInterior( topx + nCols, topy + nRows )==false)
			return false;		

		if (nRows <= 0 || nCols <= 0)
			return false;

		CoImage destImage(nRows, nCols, pSrc->GetType(), CoImage::enCOLOR_MODE);
		
		for (i = 0; i < nRows; i ++)
		{
			memcpy (destImage.m_matX.data.ptr[i], 
				pSrc->m_matX.data.ptr[i+m_rect.y] + m_rect.x * pSrc->m_matX.Step(), pSrc->m_matX.Step() * nCols);
			memcpy (destImage.m_matY.data.ptr[i], 
				pSrc->m_matY.data.ptr[i+m_rect.y] + m_rect.x * pSrc->m_matX.Step(), pSrc->m_matX.Step() * nCols);
			memcpy (destImage.m_matZ.data.ptr[i], 
				pSrc->m_matZ.data.ptr[i+m_rect.y] + m_rect.x * pSrc->m_matX.Step(), pSrc->m_matX.Step() * nCols);
		}	
		if (pDst) 
		{
			pDst->Release();
			pDst->Create (destImage, TRUE);
		}
		else 
		{
			pSrc->Release();
			pSrc->Create(destImage, TRUE);
		}
	}
	else
	{
		m_rAngle = m_rAngle * (float)(acos(float(0)) / 90);

		double cos_angle = cos(m_rAngle/*/57.295779513082320877*/);
		double sin_angle = sin(m_rAngle/*/57.295779513082320877*/);
		
		// if there is nothing special, call the original Crop():
		if ( fabs(m_rAngle)<0.0002 )
		{
			ipCrop crop(m_rect);
			return crop.Process(pSrc);
		}

		long cx = m_rect.x + m_rect.width / 2;
		long cy = m_rect.y + m_rect.height / 2;
		
		SPoint p1 = {-m_rect.width / 2, m_rect.height / 2};
		SPoint p2 = {m_rect.width / 2, m_rect.height / 2};
		SPoint p3 = {m_rect.width / 2, -m_rect.height / 2};
		SPoint p4 = {-m_rect.width / 2, -m_rect.height / 2};
	
		SPoint np1, np2, np3, np4;
		np1.x = (long)floor(cos_angle * p1.x - sin_angle * p1.y);
		np1.y = (long)floor(sin_angle * p1.x + cos_angle * p1.y);
		
		np1.x += cx;
		np1.y = -np1.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pSrc->IsInterior( np1.x, np1.y )==false)
			return false;		

		np2.x = (long)floor(cos_angle * p2.x - sin_angle * p2.y);
		np2.y = (long)floor(sin_angle * p2.x + cos_angle * p2.y);

		np2.x += cx;
		np2.y = -np2.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pSrc->IsInterior( np2.x, np2.y )==false)
			return false;		

		np3.x = (long)floor(cos_angle * p3.x - sin_angle * p3.y);
		np3.y = (long)floor(sin_angle * p3.x + cos_angle * p3.y);

		np3.x += cx;
		np3.y = -np3.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pSrc->IsInterior( np3.x, np3.y )==false)
			return false;		

		np4.x = (long)floor(cos_angle * p4.x - sin_angle * p4.y);
		np4.y = (long)floor(sin_angle * p4.x + cos_angle * p4.y);

		np4.x += cx;
		np4.y = -np4.y + cy;
		
		// check: corners of the rectangle must be inside
		if ( pSrc->IsInterior( np4.x, np4.y )==false)
			return false;		

		CoImage destImage(m_rect.height, m_rect.width , pSrc->GetType(), CoImage::enCOLOR_MODE);

		long y, x;
		long xx, yy;
		long xx1, yy1;
		for(y = 0; y < m_rect.height; y++)
			for(x = 0; x < m_rect.width; x++)
			{
				xx = x + m_rect.x - cx;
				yy = cy - y - m_rect.y;

				xx1 = (long)floor(cos_angle * xx - sin_angle * yy);
				yy1 = (long)floor(sin_angle * xx + cos_angle * yy);

				xx = xx1 + cx;
				yy = -yy1 + cy;

				destImage.m_matX.data.ptr[y][x] = pSrc->m_matX.data.ptr[yy][xx];
				destImage.m_matY.data.ptr[y][x] = pSrc->m_matY.data.ptr[yy][xx];
				destImage.m_matZ.data.ptr[y][x] = pSrc->m_matZ.data.ptr[yy][xx];				
			}
		m_rAngle = m_rAngle *  (float)(90 / acos(float(0)));
		if ( m_rAngle  > 45 && m_rAngle < 135 )
		{
			ipRotate rot(90, (CVLib::InterpolationMethod)1, (CVLib::OverflowMethod)2, NULL, TRUE, FALSE);
			rot.Process(&destImage);
		}

			if (pDst) 
			{
				pDst->Release();
				pDst->Create (destImage, TRUE);
			}
			else 
			{
				pSrc->Release();
				pSrc->Create(destImage, TRUE);
			}

/*	
		startx = MIN(m_rect.x, m_rect.x - (long)(sin_angle*(double)m_rect.height));
		endx   = m_rect.x + (long)(cos_angle*(double)m_rect.width);
		endy   = m_rect.y + (long)(cos_angle*(double)m_rect.height + sin_angle*(double)m_rect.width);
		// check: corners of the rectangle must be inside
 		if ( pSrc->IsInterior( startx, m_rect.y )==false || pSrc->IsInterior( endx, endy ) == false )
 			return false;
		
		// first crop to bounding rectangle
		CoImage tmp;
		ipCrop crop(startx, m_rect.y, endx, endy);
		if (false == crop.Process (pSrc, &tmp))
			return false;
		
		// the midpoint of the image now became the same as the midpoint of the rectangle
		// rotate new image with minus angle amount
		ipRotate rot((float)(-m_rAngle*57.295779513082320877));
		if ( false == rot.Process (&tmp) )
			return false;

		// crop rotated image to the original selection rectangle
		endx   = (tmp.GetWidth() + pSrc->GetWidth())/2;
		startx = (tmp.GetWidth() - pSrc->GetWidth())/2;
		starty = (tmp.GetHeight() + pSrc->GetHeight())/2;
		endy   = (tmp.GetHeight() - pSrc->GetHeight())/2;
		ipCrop crop1(startx, starty, endx, endy);
		if ( false == crop1.Process( &tmp ) )
			return false;
		
		if (pDst) 
		{
			pDst->Release();
			pDst->Create (tmp, TRUE);
		}
		else 
		{
			pSrc->Release();
			pSrc->Create(tmp, TRUE);
		}
*/		
		return true;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//ipSkew


ipSkew::ipSkew(const ipSkew& from)
{
	m_rxgain = from.m_rxgain;
	m_rygain = from.m_rygain;
	m_nxpivot=from.m_nxpivot;
	m_nypivot=from.m_nypivot;
	m_fEnableInterpolation = from.m_fEnableInterpolation;
}

ipSkew::ipSkew(float xgain, float ygain, long xpivot/*=0*/, long ypivot/*=0*/, bool bEnableInterpolation /*= false*/)
{
	m_rxgain = xgain;
	m_rygain = ygain;
	m_nxpivot = xpivot;
	m_nypivot = ypivot;
	m_fEnableInterpolation = bEnableInterpolation;
}

bool ipSkew::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	CoImage srcImage(*pmSrc);
	Process (&srcImage);
	if (pmDst)
	{
		pmDst->Release();
		pmDst->Create(srcImage.m_matX, TRUE);
	}
	else
	{
		pmSrc->Release();
		pmSrc->Create(srcImage.m_matX, TRUE);
	}
	return true;
}

bool ipSkew::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if ( pDst && !EqualTypeSize(pSrc, pDst) )
	{
		pDst->Release();
		pDst->Create(*pSrc);
	}
	float nx,ny;
	
	CoImage tmp(*pSrc);
	if (!tmp.IsValid())
		return false;
	
	long xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pSrc->GetWidth(); ymax=pSrc->GetHeight();
	for(long y=ymin; y<ymax; y++)
	{
		for(long x=xmin; x<xmax; x++)
		{
			nx = x + (m_rxgain*(y - m_nypivot));
			ny = y + (m_rygain*(x - m_nxpivot));
			if (m_fEnableInterpolation)
			{
				tmp.SetPixelColor(x,y,GetPixelColorInterpolated(pSrc, nx, ny, 
					IM_BILINEAR, OM_BACKGROUND));
			}
			else
			{
				tmp.SetPixelColor(x,y,pSrc->GetPixelColor((long)nx,(long)ny));
			}
		}
	}
	//copy new image to the destination
	if (pDst) 
	{
		pDst->Release();
		pDst->Create (tmp, TRUE);
	}
	else 
	{
		pSrc->Release();
		pSrc->Create(tmp, TRUE);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//ipExpand


ipExpand::ipExpand(const ipExpand& from)
{
	m_nLeft=from.m_nLeft;
	m_nTop=from.m_nTop;
	m_nRight=from.m_nRight;
	m_nBottom=from.m_nBottom;
	m_newx=from.m_newx;
	m_newy=from.m_newy;
	m_color=from.m_newy;
}

ipExpand::ipExpand(long left, long top, long right, long bottom, COLORREF canvascolor)
{
}

ipExpand::ipExpand(long newx, long newy, COLORREF canvascolor)
{
}

bool ipExpand::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if ( pmDst && !EqualType(pmSrc, pmDst) )
		return false;
	return true;
}

bool ipExpand::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if ( pDst && !EqualTypeSize(pSrc, pDst) )
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//ipThumbnail

ipThumbnail::ipThumbnail(const ipThumbnail& from)
{
	m_newx=from.m_newx;
	m_newy=from.m_newy;
}

ipThumbnail::ipThumbnail(long newx, long newy, COLORREF canvascolor)
{
	m_newx=newx;
	m_newy=newy;
	m_canvasColor=canvascolor;
}

bool ipThumbnail::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if ( pmDst && !EqualType(pmSrc, pmDst) )
		return false;
	return true;
}

bool ipThumbnail::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if ( pDst && !EqualTypeSize(pSrc, pDst) )
		return false;
	if (!pSrc->IsValid()) return false;

    if ((m_newx <= 0) || (m_newy <= 0)) return false;

    CoImage tmp(*pSrc);
	if (!tmp.IsValid()) return false;

    // determine whether we need to shrink the image
    if ((pSrc->GetWidth() > m_newx) || (pSrc->GetHeight() > m_newy)) {
        float fScale;
        float fAspect = (float) m_newx / (float) m_newy;
        if (fAspect * pSrc->GetHeight() > pSrc->GetWidth()) {
            fScale = (float) m_newy / pSrc->GetHeight();
        } else {
            fScale = (float) m_newx / pSrc->GetWidth();
        }
		ipResample resampler((long) (fScale * pSrc->GetWidth()), (long) (fScale * pSrc->GetHeight()), 0);
		resampler.Process (&tmp);
    }

    // expand the frame
	ipExpand expander(m_newx, m_newy, m_canvasColor);
	if (pDst)
		expander.Process (&tmp, pDst);
	else
		expander.Process (&tmp, pSrc);
	return true;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//ipTransGeo


ipTransGeo::ipTransGeo(const ipTransGeo& from)
{
	m_mTransform=from.m_mTransform;
}

ipTransGeo::ipTransGeo()
{
	m_mTransform.Create (3, 3, MAT_Tdouble);
	m_mTransform.Identity();
}

ipTransGeo::~ipTransGeo()
{
	m_mTransform.Release();		
}

ipTransGeo&	ipTransGeo::Rotate(double theta)
{
	Mat mTran(3, 3, MAT_Tdouble);
	mTran.Identity();
	
	mTran.data.db[0][0] = cos(theta);
	mTran.data.db[1][1] = cos(theta);
	mTran.data.db[0][1] = -sin(theta);
	mTran.data.db[1][0] = sin(theta);
	ApplyTrans(&mTran);
	return *this;
}

ipTransGeo&	ipTransGeo::Scale(double sx, double sy)
{
	Mat mTran(3, 3, MAT_Tdouble);
	mTran.Identity();
	mTran.data.db[0][0] = sx;
	mTran.data.db[1][1] = sy;
	ApplyTrans(&mTran);
	return *this;
}

ipTransGeo&	ipTransGeo::Translate(double dx, double dy)
{
	Mat mTran(3, 3, MAT_Tdouble);
	mTran.Identity();
	mTran.data.db[0][2] = dx;
	mTran.data.db[1][2] = dy;
	ApplyTrans(&mTran);
	return *this;
}

void ipTransGeo::ApplyTrans(Mat* A)
{
	int i, j, k;                                   /* Variables used as indices */
	Mat* P = new Mat(3, 3, MAT_Tdouble);	/* Product A * B */
	
	/** Build the product matrix P */
	for ( j = 0; j < m_mTransform.Cols(); j++)
	{
		for ( i = 0; i < A->Rows(); i++)
		{
			P->data.db[i][j] = 0;
			for (k = 0; k < A->Cols(); k ++)
				P->data.db[i][j] += A->data.db[i][k] * m_mTransform.data.db[k][j];
		}
	}
	for ( j = 0; j < m_mTransform.Cols(); j++)
	{
		for ( i = 0; i < m_mTransform.Rows(); i++)
			m_mTransform.data.db[i][j] = P->data.db[i][j];
	}
	delete P;
}

const Mat*	ipTransGeo::GetTransformation() const
{
	return &m_mTransform;
}

const Mat*	ipTransGeo::GetInverseTransformation() const
{
	assert (false);
	return &m_mInverseTransform;
}

#define INT_FLOOR(a)	((int)(a))
#define INT_CEIL(a)		((int)(a)+1)

double ipTransGeo::InterpLinear(Mat* pmImage, double x, double y)
{
	double xfrac = (x - floor(x));
	double yfrac = (y - floor(y));
	int xLower = INT_FLOOR(x); xLower = MAX(xLower, 0); xLower = MIN(xLower, pmImage->Cols() - 1);
	int xUpper = INT_CEIL(x); xUpper = MAX(xUpper, 0); xUpper = MIN(xUpper, pmImage->Cols() - 1);
	int yLower = INT_FLOOR(y); yLower = MAX(yLower, 0); yLower = MIN(yLower, pmImage->Rows() - 1);
	int yUpper = INT_CEIL(y); yUpper = MAX(yUpper, 0); yUpper = MIN(yUpper, pmImage->Rows() - 1);
	double valUpper, valLower;
	
	switch (pmImage->Type())
	{
	case MAT_Tdouble:
		{
			double** pprImage = pmImage->data.db;
			valUpper = pprImage[yUpper][xLower]*(1.0-xfrac) + pprImage[yUpper][xUpper]*(xfrac);
			valLower = pprImage[yLower][xLower]*(1.0-xfrac) + pprImage[yLower][xUpper]*(xfrac);
		}
		break;
	case MAT_Tbyte:
		{
			BYTE** ppbImage = pmImage->data.ptr;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
		}
		break;
        default:
        assert(false);	}
	
	return valLower*(1.0-yfrac) + valUpper*(yfrac);
}

void ipTransGeo::InterpLinearImage(CoImage* pSrc, double x, double y, CoImage* pDst, int dstx, int dsty)
{
	double xfrac = (x - floor(x));
	double yfrac = (y - floor(y));
	int xLower = INT_FLOOR(x); xLower = MAX(xLower, 0); xLower = MIN(xLower, pSrc->GetWidth() - 1);
	int xUpper = INT_CEIL(x); xUpper = MAX(xUpper, 0); xUpper = MIN(xUpper, pSrc->GetWidth() - 1);
	int yLower = INT_FLOOR(y); yLower = MAX(yLower, 0); yLower = MIN(yLower, pSrc->GetHeight() - 1);
	int yUpper = INT_CEIL(y); yUpper = MAX(yUpper, 0); yUpper = MIN(yUpper, pSrc->GetHeight() - 1);
	double valUpper, valLower;
	
	switch (pSrc->m_matX.Type())
	{
	case MAT_Tdouble:
		{
			double** ppbImage = pSrc->m_matX.data.db;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
			pDst->m_matX.data.db[dsty][dstx]=(double)(valLower*(1.0-yfrac) + valUpper*(yfrac));
			
			ppbImage = pSrc->m_matY.data.db;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
			pDst->m_matY.data.db[dsty][dstx]=(double)(valLower*(1.0-yfrac) + valUpper*(yfrac));
			
			ppbImage = pSrc->m_matZ.data.db;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
			pDst->m_matZ.data.db[dsty][dstx]=(double)(valLower*(1.0-yfrac) + valUpper*(yfrac));
		}
		break;
	case MAT_Tbyte:
		{
			BYTE** ppbImage = pSrc->m_matX.data.ptr;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
			pDst->m_matX.data.ptr[dsty][dstx]=(BYTE)(valLower*(1.0-yfrac) + valUpper*(yfrac));

			ppbImage = pSrc->m_matY.data.ptr;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
			pDst->m_matY.data.ptr[dsty][dstx]=(BYTE)(valLower*(1.0-yfrac) + valUpper*(yfrac));

			ppbImage = pSrc->m_matZ.data.ptr;
			valUpper = ppbImage[yUpper][xLower]*(1.0-xfrac) + ppbImage[yUpper][xUpper]*(xfrac);
			valLower = ppbImage[yLower][xLower]*(1.0-xfrac) + ppbImage[yLower][xUpper]*(xfrac);
			pDst->m_matZ.data.ptr[dsty][dstx]=(BYTE)(valLower*(1.0-yfrac) + valUpper*(yfrac));
		}
		break;
        default:
        assert(false);	}
}

bool ipTransGeo::Process (Point2f* pSrc, Point2f* pDst /*= NULL*/)
{
	LUDecomposition lu(&m_mTransform);
	Mat mB(m_mTransform.Rows(), m_mTransform.Cols(), m_mTransform.Type());
	mB.Identity();
	
	Mat pointMat(3, 1, MAT_Tdouble);
	Matd mpt(3, 1);
	double** pprPoint = pointMat.data.db;
	pprPoint[0][0] = (double)pSrc->x;
	pprPoint[1][0] = (double)pSrc->y;
	pprPoint[2][0] = 1.0;
	MatOp::Mul(&mpt, &m_mTransform, &pointMat);
	double** pt1 = mpt.data.db;
	pt1[0][0] = pt1[0][0] / pt1[2][0];
	pt1[1][0] = pt1[1][0] / pt1[2][0];
	if (pDst)
	{
		pDst->x = (float)pt1[0][0];
		pDst->y = (float)pt1[1][0];
	}
	else
	{
		pSrc->x = (float)pt1[0][0];
		pSrc->y = (float)pt1[1][0];
	}

	return true;
}

bool ipTransGeo::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	LUDecomposition lu(&m_mTransform);
	Mat mB(m_mTransform.Rows(), m_mTransform.Cols(), m_mTransform.Type());
	mB.Identity();
	Mat* pmInverse = lu.Solve(&mB);

	Mat pointMat(3, 1, MAT_Tdouble);
	Matd mpt(3, 1);
	double** pt1 = mpt.data.db;
	double** pprPoint = pointMat.data.db;
	pprPoint[2][0] = 1;
	if (pmDst)
	{
		pmDst->Zero();
		for (int i = 0; i < pmDst->Rows(); i ++)
		{
			for (int j = 0; j < pmDst->Cols(); j ++)
			{
				pprPoint[0][0] = (double)j;
				pprPoint[1][0] = (double)i;
				pprPoint[2][0] = 1.0;
				MatOp::Mul(&mpt, pmInverse, &pointMat);
				pt1[0][0] = pt1[0][0] / pt1[2][0];
				pt1[1][0] = pt1[1][0] / pt1[2][0];
				if (pt1[0][0] < 0 || pt1[0][0] >= pmSrc->Cols() || pt1[1][0] < 0 || pt1[1][0] >= pmSrc->Rows())
					pmDst->data.ptr[i][j] = 0;
				else
					pmDst->data.ptr[i][j] = (BYTE)InterpLinear(pmSrc, pt1[0][0], pt1[1][0]);
			}
		}
	}
	else
	{
		Mat temp(*pmSrc);
		temp.Zero();
		for (int i = 0; i < temp.Rows(); i ++)
		{
			for (int j = 0; j < temp.Cols(); j ++)
			{
				pprPoint[0][0] = (double)j;
				pprPoint[1][0] = (double)i;
				pprPoint[2][0] = 1.0;
				MatOp::Mul(&mpt, pmInverse, &pointMat);
				pt1[0][0] = pt1[0][0] / pt1[2][0];
				pt1[1][0] = pt1[1][0] / pt1[2][0];
				if (pt1[0][0] < 0 || pt1[0][0] >= pmSrc->Cols() || pt1[1][0] < 0 || pt1[1][0] >= pmSrc->Rows())
					temp.data.ptr[i][j] = 0;
				else
					temp.data.ptr[i][j] = (BYTE)InterpLinear(pmSrc, pt1[0][0], pt1[1][0]);
			}
		}
		pmSrc->Release();
		pmSrc->Create (temp, TRUE);
	}
	delete pmInverse;
	return true;
}

bool ipTransGeo::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/, Rect* pRect/*=NULL*/)
{
	LUDecomposition lu(&m_mTransform);
	Mat mB(m_mTransform.Rows(), m_mTransform.Cols(), m_mTransform.Type());
	mB.Identity();
	Mat* pmInverse = lu.Solve(&mB);
	
	Mat pointMat(3, 1, MAT_Tdouble);
	Matd mpt(3, 1);
	double** pt1 = mpt.data.db;
	double** pprPoint = pointMat.data.db;
	pprPoint[2][0] = 1;

	if (pDst)
	{
		int minx=pRect?pRect->x:0;
		int miny=pRect?pRect->y:0;
		int maxx=pRect?pRect->x+pRect->width:pDst->GetWidth();
		int maxy=pRect?pRect->y+pRect->height:pDst->GetHeight();
		for (int i = miny; i < maxy; i ++)
		{
			for (int j = minx; j < maxx; j ++)
			{
				pprPoint[0][0] = (double)j;
				pprPoint[1][0] = (double)i;
				pprPoint[2][0] = 1.0;
				MatOp::Mul(&mpt, pmInverse, &pointMat);
				pt1[0][0] = pt1[0][0] / pt1[2][0];
				pt1[1][0] = pt1[1][0] / pt1[2][0];
				if (pt1[0][0] < 0 || pt1[0][0] >= pSrc->GetWidth() || pt1[1][0] < 0 || pt1[1][0] >= pSrc->GetHeight())
				{
					pDst->SetPixelColor(j, i, m_bkColor);
				}
				else
				{
					InterpLinearImage(pSrc, pt1[0][0], pt1[1][0], pDst, j, i);
				}
			}
		}
	}
	else
	{
		CoImage temp(*pSrc);
		for (int i = 0; i < temp.GetHeight(); i ++)
		{
			for (int j = 0; j < temp.GetWidth(); j ++)
			{
				pprPoint[0][0] = (double)j;
				pprPoint[1][0] = (double)i;
				pprPoint[2][0] = 1.0;
				MatOp::Mul(&mpt, pmInverse, &pointMat);
				pt1[0][0] = pt1[0][0] / pt1[2][0];
				pt1[1][0] = pt1[1][0] / pt1[2][0];
				if (pt1[0][0] < 0 || pt1[0][0] >= pSrc->GetWidth() || pt1[1][0] < 0 || pt1[1][0] >= pSrc->GetHeight())
				{
					temp.SetPixelColor(j, i, RGB(0,0,0));
				}
				else
				{
					InterpLinearImage (pSrc, pt1[0][0], pt1[1][0], &temp, j, i);
				}
			}
		}
		pSrc->Release();
		pSrc->Create (temp, TRUE);
	}
	delete pmInverse;
	return true;
}

bool ipTransGeo::InverseProcess (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	assert (false);
	return false;
}
bool ipTransGeo::InverseProcess (Point2f* pSrc, Point2f* pDst /*= NULL*/)
{
	LUDecomposition lu(&m_mTransform);
	Mat mB(m_mTransform.Rows(), m_mTransform.Cols(), m_mTransform.Type());
	mB.Identity();
	
	Mat* pmInverse = lu.Solve(&mB);
	Mat pointMat(3, 1, MAT_Tdouble);
	double** pprPoint = pointMat.data.db;
	pprPoint[0][0] = (double)pSrc->x;
	pprPoint[1][0] = (double)pSrc->y;
	pprPoint[2][0] = 1.0;
	Matd mpt(3, 1);
	MatOp::Mul(&mpt, pmInverse, &pointMat);
	double** pt1 = mpt.data.db;
	pt1[0][0] = pt1[0][0] / pt1[2][0];
	pt1[1][0] = pt1[1][0] / pt1[2][0];
	if (pDst)
	{
		pDst->x = (float)pt1[0][0];
		pDst->y = (float)pt1[1][0];
	}
	else
	{
		pSrc->x = (float)pt1[0][0];
		pSrc->y = (float)pt1[1][0];
	}
	
	delete pmInverse;
	return true;
}
bool ipTransGeo::InverseProcess (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	assert (false);
	return false;
}

namespace ip
{
void Rotate(const Mat& src, Mat& dst, RotateType type)
{
	ipRotate t(type); t.Process ((Mat*)&src, &dst);
}
void Rotate(const Mat& src, Mat& dst, float rAngle)
{
	ipRotate t(rAngle); t.Process ((Mat*)&src, &dst);
}
void Rotate(const Mat& src, Mat& dst, float rAngle, InterpolationMethod inMethod,
		   OverflowMethod ofMethod, COLORREF *replColor,
		   bool const optimizeRightAngles, bool const bKeepOriginalSize)
{
	ipRotate t(rAngle, inMethod, ofMethod, replColor, optimizeRightAngles, bKeepOriginalSize); t.Process ((Mat*)&src, &dst);
}
void Resample(const Mat& src, Mat& dst, long newx, long newy, int mode)
{
	ipResample t(newx, newy, mode); t.Process ((Mat*)&src, &dst);
}
void Resample(const CoImage& src, CoImage& dst, long newx, long newy, int mode)
{
	ipResample t(newx, newy, mode); t.Process ((CoImage*)&src, &dst);
}
void Resample(const Mat& src, Mat& dst, long newx, long newy, InterpolationMethod const inMethod,
			 OverflowMethod const ofMethod, bool const disableAveraging)
{
	ipResample t(newx, newy, inMethod, ofMethod, disableAveraging); t.Process ((Mat*)&src, &dst);
}
void Crop(const Mat& src, Mat& dst, long left, long top, long right, long bottom)
{
	ipCrop t(left, top, right, bottom); t.Process ((Mat*)&src, &dst);
}
void Crop(const Mat& src, Mat& dst, const Rect& rect)
{
	ipCrop t(rect); t.Process ((Mat*)&src, &dst);
}
void Crop(const Mat& src, Mat& dst, long topx, long topy, long width, long height, float angle)
{
	ipCrop t(topx, topy, width, height, angle); t.Process ((Mat*)&src, &dst);
}
void Skew(const Mat& src, Mat& dst, float xgain, float ygain, long xpivot, long ypivot, bool bEnableInterpolation)
{
	ipSkew t(xgain, ygain, xpivot, ypivot, bEnableInterpolation); t.Process ((Mat*)&src, &dst);
}
void Expand(const Mat& src, Mat& dst, long left, long top, long right, long bottom, COLORREF canvascolor)
{
	ipExpand t(left, top, right, bottom, canvascolor); t.Process ((Mat*)&src, &dst);
}
void Expand(const Mat& src, Mat& dst, long newx, long newy, COLORREF canvascolor)
{
	ipExpand t(newx, newy, canvascolor); t.Process ((Mat*)&src, &dst);
}
void Thumbnail(const Mat& src, Mat& dst, long newx, long newy, COLORREF canvascolor)
{
	ipThumbnail t(newx, newy, canvascolor); t.Process ((Mat*)&src, &dst);
}
void Thumbnail(const CoImage& src, CoImage& dst, long newx, long newy, COLORREF canvascolor)
{
	ipThumbnail t(newx, newy, canvascolor); t.Process ((CoImage*)&src, &dst);
}

}

}
