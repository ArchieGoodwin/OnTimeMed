
/*! 
 * \file    CoImageCFG.h
 * \ingroup base
 * \brief   Ãùº¬¿¸´èË¾ ¼èÌºÂ×±¨ Ì¡ÂÙ ¶ë¿Íµá´ÉË¾ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */
#ifndef _COIMAGECFG_H_
#define _COIMAGECFG_H_

/////////////////////////////////////////////////////////////////////////////
// CVLIB_IMG supported features
#define CVLIB_IMG_SUPPORT_ALPHA          1
#define CVLIB_IMG_SUPPORT_INTERPOLATION  0

#define CVLIB_IMG_SUPPORT_DECODE	1
#define CVLIB_IMG_SUPPORT_ENCODE	1		//<vho><T.Peck>

/////////////////////////////////////////////////////////////////////////////
// CVLIB_IMG supported formats
#define CVLIB_IMG_SUPPORT_BMP 1
#define CVLIB_IMG_SUPPORT_GIF 1
#define CVLIB_IMG_SUPPORT_JAS 1
#define CVLIB_IMG_SUPPORT_JPG 1
#define CVLIB_IMG_SUPPORT_PNG 1
#define CVLIB_IMG_SUPPORT_MNG 1
#define CVLIB_IMG_SUPPORT_ICO 1
#define CVLIB_IMG_SUPPORT_TIF 1
#define CVLIB_IMG_SUPPORT_TGA 1
#define CVLIB_IMG_SUPPORT_PCX 1
#define CVLIB_IMG_SUPPORT_WBMP 1
#define CVLIB_IMG_SUPPORT_WMF 1
#define CVLIB_IMG_SUPPORT_J2K 0		// Beta, use JP2
#define CVLIB_IMG_SUPPORT_JBG 0		// GPL'd see ../jbig/copying.txt & ../jbig/patents.htm

#define CVLIB_IMG_SUPPORT_JP2 1
#define CVLIB_IMG_SUPPORT_JPC 1
#define CVLIB_IMG_SUPPORT_PGX 1
#define CVLIB_IMG_SUPPORT_PNM 1
#define CVLIB_IMG_SUPPORT_RAS 1


/////////////////////////////////////////////////////////////////////////////
#define	CVLIB_IMG_MAX_MEMORY 256000000

#define CVLIB_IMG_ERR_NOFILE "null file handler"
#define CVLIB_IMG_ERR_NOIMAGE "null image!!!"

/////////////////////////////////////////////////////////////////////////////
//color to grey mapping <H. Muelner> <jurgene>
//#define RGB2GRAY(r,g,b) (((b)*114 + (g)*587 + (r)*299)/1000)
#define RGB2GRAY(r,g,b) (((b)*117 + (g)*601 + (r)*306) >> 10)

#endif //_COIMAGECFG_H_
