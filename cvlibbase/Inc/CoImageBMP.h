/*! 
 * \file    CoImageBMP.h
 * \ingroup base
 * \brief   BMPÂô»¥Ì© Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _COIMAGEBMP_H_
#define _COIMAGEBMP_H_

#include "CoImage.h"

namespace CVLib 
{

/**
 * @brief bmpÃùº¬Ë¾ ËÏ°Ö ¸ó±ãÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷
 */

#if CVLIB_IMG_SUPPORT_BMP

class CVLIB_DECLSPEC CoImageBMP : public CoImage
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	CoImageBMP ();

	/**
	 * @brief  BMPÃùº¬Ë¾ ËÏ²÷³Þ.
	 *
	 * @param   pFile [in] : ÁÄËÎ¼è¼¬»ô
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Encode()
	 *
	 * @par Example:
	 * @code
	 *
	 *  char szFileName[] = "c:/Test/TestImg1.bmp"; 
	 *  char szFileName1[] = "c:/Test/SaveImg1_4.bmp"; 
	 *
	 *  FILE* pFile = fopen(szFileName, "rb");
	 *
	 *  CoImageBMP testImg, Img; 
	 *  if ( testImg.Decode (pFile) ) 
	 *  {
	 *		Img.Create (testImg, TRUE);
	 *		FILE* pFile1 = fopen(szFileName1, "wb");
	 *		Img.Encode(pFile1);
	 *		fclose(pFile1);
	 *  }
	 *
	 *  fclose(pFile);
	 *
	 * @endcode
	 * 
	 */
	bool Decode (FILE* pFile);

#if CVLIB_IMG_SUPPORT_ENCODE
	/**
	 * @brief  BMPÃùº¬Ë¾ ¸ó±ãÂÙ³Þ.
	 *
	 * @param   pFile [in] : ÁÄËÎ¼è¼¬»ô
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Decode()
	 *
	 */
	bool Encode(FILE* pFile);
#endif // CVLIB_IMG_SUPPORT_ENCODE
	
protected:
	bool DibReadBitmapInfo(FILE* pFile, BITMAPINFOHEADER1 *pdib);
};

#endif

}

#endif //_COIMAGEBMP_H_