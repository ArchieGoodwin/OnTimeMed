
/*!
 * \file	ImgProc.h
 * \ingroup Engine
 * \brief	�����嶮�� ̡�� �����˳�.
 * \author	ʯ����
 */

#ifndef _IMGPROC_H_
#define _IMGPROC_H_

#include "cvlibbase/inc/cvlibbaseDef.h"
#include "cvlibbase/inc/cvlibutil.h"

namespace CVLib { namespace ip{

//! ��������˺�� ���ؼӳ�. 
#define  PD_SCALE_INT(rX)       (((rX) + (1<<7)) >> 8)

//!  ��������˺�� ���ؼӳ�.
#define  PD_SCALE_FLT(rX)       ((rX)*0.00390625f)

#define  GAUSSIAN_5x5 7

//! �״ݸɸ����
#define MAX_ITERS 100

/**
 * @brief  �ͱ��� 8̩ �ɺ�� ���ؼӳ�.
 *
 * @param  nSize  [in]  ���� �ͱ�
 * @param  nAlign [in]  8 
 * @return 8̩ �ɺ�
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
 * @brief  �ӵٰ���� ��˼ ���� �� �����㶦 ���ؼӳ�.
 *
 * @param  rValue �ӵٰ�
 * @return �ӵٰ���� ��˼ ���� �� ������
 */
static int Floor (float rValue)
{
    int temp = CVUtil::Round(rValue);
    float diff = (float)(rValue - temp);
    return temp - (*(int*)&diff < 0);
};

/**
 * @brief	�������������ⶦ �龢�ٳ�.
 *
 * @param	prSrc				[in]	̺������
 * @param	nSrc_step			[in]	̺������̩ ����  
 * @param	prDst				[out]	��������
 * @param	nDst_step			[in]	��������̩ ����
 * @param	src_size			[in]	����̩ �ͱ�
 * @param	prKx				[in]	x������ �Ӻ�
 * @param	prKy				[in]	y������ �Ӻ�
 * @param	prBuffer			[in]	����̰����
 * @return  ����	
 */
CVLIB_DECLSPEC void SepConvSmall( float*  prSrc, int nSrc_step, float*  prDst, int nDst_step, Size src_size,
					   const float* prKx, const float* prKy, float*  prBuffer );

/**
 * @brief	��������̩ �� ����̩ �������ϰ�˾ �˺��ٳ�.
 *
 * @param	pbSrc				[in]	̺������
 * @param	nSrc_step			[in]	̺������̩ ����  
 * @param	src_size			[in]	̺������̩ �ͱ�
 * @param	prDst				[out]	��������
 * @param	nDst_step			[in]	��������̩ ����
 * @param	win_size			[in]	�����۷�̩ �ͱ�
 * @param	center				[in]	�����۷�̩ �ڻ�
 * @return  1 	
 */
CVLIB_DECLSPEC BOOL GetRectSubPixel(const BYTE* pbSrc, int nSrc_step, Size src_size, float* prDst,
					 int nDst_step, Size win_size, Point2f center);

/**
 * @brief	���߿ͱ��� �����ײ� �ܺ�
 *
 * @param	pSrcptr				[in]	̺������
 * @param	nSrc_step			[in]	̺������̩ ����  
 * @param	nPix_size			[in]	���Ͽͱ�
 * @param	src_size			[in]	����̩ ����
 * @param	win_size			[in]	�����۷�̩ �ͱ�
 * @param	ip					[in]	�����۷�̩ �ڻ���̡��
 * @param	pRect				[out]	�˺�����
 * @return 	�α� ����̩ left/top��̩ �Ѻ�
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