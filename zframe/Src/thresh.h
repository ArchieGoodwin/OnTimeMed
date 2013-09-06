/*!
 * \file	ipThreshold.h
 * \ingroup ipCore
 * \brief   JitterÃùº¬Ë¾ »¶ºÂÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#pragma once

#include "cvlibbase/inc/Mat.h"

namespace CVLib
{

namespace ip
{
	/* Types of thresholding */
#define THRESH_BINARY      0  /* value = value > threshold ? max_value : 0       */
#define THRESH_BINARY_INV  1  /* value = value > threshold ? 0 : max_value       */
#define THRESH_TRUNC       2  /* value = value > threshold ? threshold : value   */
#define THRESH_TOZERO      3  /* value = value > threshold ? value : 0           */
#define THRESH_TOZERO_INV  4  /* value = value > threshold ? 0 : value           */
#define THRESH_MASK        7

#define THRESH_TILE      5  /* value = value > {thresh*imagesize} ? maxval : 0           */
#define THRESH_TILE_INV  6  /* value = value > {thresh*imagesize} ? 0 : maxval           */


#define THRESH_OTSU        8  /* use Otsu algorithm to choose the optimal threshold value;
	combine the flag with one of the above THRESH_* values */

/* Applies fixed-level threshold to grayscale image. This is a basic operation applied before retrieving contours */
CVLIB_DECLSPEC double Threshold( const Mat& src, Mat& dst, double thresh, double maxval, int type );
CVLIB_DECLSPEC BYTE OtsuThreshold(const Mat& img);
CVLIB_DECLSPEC int thresholdOfGray(const Mat& gray);

// void AdaptiveThreshold( const Mat& src, Mat& dst, double maxValue, int method, int type, int blockSize, double delta );

}

}