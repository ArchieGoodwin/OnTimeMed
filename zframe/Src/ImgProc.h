
/*!
 * \file	ImgProc.h
 * \ingroup Engine
 * \brief	Ãùº¬½å¶®¶¦ Ì¡ÂÙ ÁÄËÎËË³Þ.
 * \author	Ê¯¼³Âö
 */

#ifndef _IMGPROC_H_
#define _IMGPROC_H_

#include "cvlibbase/inc/cvlibbaseDef.h"
#include "cvlibbase/inc/cvlibutil.h"

namespace CVLib { namespace ip{

//! Êú°ýºãÂôËºµá ´®µØ¼Ó³Þ. 
#define  PD_SCALE_INT(rX)       (((rX) + (1<<7)) >> 8)

//!  µù¼°ºãÂôËºµá ´®µØ¼Ó³Þ.
#define  PD_SCALE_FLT(rX)       ((rX)*0.00390625f)

#define  GAUSSIAN_5x5 7

//! ¾×´Ý¸É¸ôÃäºã
#define MAX_ITERS 100

/**
 * @brief  ¿Í±¨¶¦ 8Ì© ¹Éºãµá ´®µØ¼Ó³Þ.
 *
 * @param  nSize  [in]  »¨½£ ¿Í±¨
 * @param  nAlign [in]  8 
 * @return 8Ì© ¹Éºã
 */
static int Align( int nSize, int nAlign )
{
	assert( (nAlign & (nAlign-1)) == 0 && nSize < INT_MAX );
	return (nSize + nAlign - 1) & -nAlign;
};
template<typename _Tp> static inline _Tp gcd(_Tp a, _Tp b)
{
	_Tp t;
	if( a < b )
	{
		SWAP(a, b, t);
	}
	while( b > 0 )
	{
		_Tp r = a % b;
		a = b;
		b = r;
	}
	return a;
}
/**
 * @brief  ËÓµÙ°ª¸ó³Þ »õË¼ °¡»þ ¿Ï Êú°ýºã¶¦ ´®µØ¼Ó³Þ.
 *
 * @param  rValue ËÓµÙ°ª
 * @return ËÓµÙ°ª¸ó³Þ »õË¼ °¡»þ ¿Ï Êú°ýºã
 */
static int Floor (float rValue)
{
    int temp = CVUtil::Round(rValue);
    float diff = (float)(rValue - temp);
    return temp - (*(int*)&diff < 0);
};

/**
 * @brief	Ãùº¬°ûµ¹´ÑËèÀâ¶¦ °é¾¢ÂÙ³Þ.
 *
 * @param	prSrc				[in]	Ìº½çÃùº¬
 * @param	nSrc_step			[in]	Ìº½çÃùº¬Ì© ²¸¹¾  
 * @param	prDst				[out]	·»¼¬Ãùº¬
 * @param	nDst_step			[in]	·»¼¬Ãùº¬Ì© ²¸¹¾
 * @param	src_size			[in]	Ãùº¬Ì© ¿Í±¨
 * @param	prKx				[in]	x¾¢¸ÒÂá °Óºã
 * @param	prKy				[in]	y¾¢¸ÒÂá °Óºã
 * @param	prBuffer			[in]	Ãùº¬Ì°¾©±¨
 * @return  ÊÖËÁ	
 */
CVLIB_DECLSPEC void SepConvSmall( float*  prSrc, int nSrc_step, float*  prDst, int nDst_step, Size src_size,
					   const float* prKx, const float* prKy, float*  prBuffer );

/**
 * @brief	·»¼¬Ãùº¬Ì© ·ô ÃùºÏÌ© ¹¢¹¤ÃùºÏ°ªË¾ ±Ëº¤ÂÙ³Þ.
 *
 * @param	pbSrc				[in]	Ìº½çÃùº¬
 * @param	nSrc_step			[in]	Ìº½çÃùº¬Ì© ²¸¹¾  
 * @param	src_size			[in]	Ìº½çÃùº¬Ì© ¿Í±¨
 * @param	prDst				[out]	·»¼¬Ãùº¬
 * @param	nDst_step			[in]	·»¼¬Ãùº¬Ì© ²¸¹¾
 * @param	win_size			[in]	¼¬¹¤½Û·ÍÌ© ¿Í±¨
 * @param	center				[in]	¼¬¹¤½Û·ÍÌ© ¼Ú»ª
 * @return  1 	
 */
CVLIB_DECLSPEC BOOL GetRectSubPixel(const BYTE* pbSrc, int nSrc_step, Size src_size, float* prDst,
					 int nDst_step, Size win_size, Point2f center);

/**
 * @brief	µßÊß¿Í±¨¶¦ Ãú¼³Â×²÷ ÂÜºã
 *
 * @param	pSrcptr				[in]	Ìº½çÃùº¬
 * @param	nSrc_step			[in]	Ìº½çÃùº¬Ì© ²¸¹¾  
 * @param	nPix_size			[in]	ÃùºÏ¿Í±¨
 * @param	src_size			[in]	Ãùº¬Ì© ²¸¹¾
 * @param	win_size			[in]	¼¬¹¤½Û·ÍÌ© ¿Í±¨
 * @param	ip					[in]	¼¬¹¤½Û·ÍÌ© ¼Ú»ª¼°Ì¡¾·
 * @param	pRect				[out]	±Ëº¤µßÊß
 * @return 	°Î±á µßÊßÌ© left/top¼°Ì© ¼ÑºÏ
 */
CVLIB_DECLSPEC void* AdjustRect( const void* pSrcptr, int nSrc_step, int nPix_size,
				 Size src_size, Size win_size, SPoint ip, Rect* pRect );

enum { 
	BORDER_REPLICATE=1/*=IPL_BORDER_REPLICATE*/, 
	BORDER_CONSTANT=0/*=IPL_BORDER_CONSTANT*/,
	BORDER_REFLECT=2/*=IPL_BORDER_REFLECT*/, 
	BORDER_REFLECT_101=4/*=IPL_BORDER_REFLECT_101*/,
	BORDER_REFLECT101=BORDER_REFLECT_101, 
	BORDER_WRAP=3/*=IPL_BORDER_WRAP*/,
	BORDER_TRANSPARENT, 
	BORDER_DEFAULT=BORDER_REFLECT_101, 
	BORDER_ISOLATED=16 
};

CVLIB_DECLSPEC int borderInterpolate( int p, int len, int borderType );

}}

#endif // _IMGPROC_H_