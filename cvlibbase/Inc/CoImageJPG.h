/*! 
 * \file    CoImageJPG.h
 * \ingroup base
 * \brief   JPGÂô»¥Ì© Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ¼³Ë¼Êé
 */

#ifndef _COIMAGEJPG_H_
#define _COIMAGEJPG_H_

#ifndef CVLIB_IMG_NOCODEC

#include "CoImage.h"

namespace CVLib 
{

/**
 * @brief jpg Ãùº¬Ë¾ ËÏ°Ö ¸ó±ãÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷
 */

#if CVLIB_IMG_SUPPORT_JPG

class CVLIB_DECLSPEC CoImageJPG : public CoImage
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	CoImageJPG ();

	/**
	 * @brief   jpgÃùº¬Ë¾ ËÏ²÷³Þ.
	 *
	 * @param   pFile [in]  ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Encode()
	 *
	 * @par Example:
	 * @code
	 *
	 *  char szFileName[] = "c:/Test/TestImg6.JPG"; 
	 *  char szFileName1[] = "c:/Test/SaveImg6.bmp"; 
	 *
	 *  FILE* pFile = fopen(szFileName, "rb");
	 *
	 *  CoImage testImg;
	 *  if ( testImg.Decode (pFile, CVLIB_IMG_FORMAT_JPG) ) 
	 *  {
	 *		testImg.Save(szFileName1, CVLIB_IMG_FORMAT_BMP);
	 *  }
	 *
	 *  fclose(pFile);
	 * ...
	 * @endcode
	 * 
	 */
	bool Decode (FILE* pFile);

#if CVLIB_IMG_SUPPORT_ENCODE
	/**
	 * @brief   jpgÃùº¬Ë¾ ¸ó±ãÂÙ³Þ.
	 *
	 * @param   pFile [in]  ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Encode()
	 *
	 * @par Example:
	 * @code
	 *
	 *  char szFileName[] = "c:/Test/TestImg6.JPG"; 
	 *  char szFileName1[] = "c:/Test/SaveImg6.JPG"; 
	 *
	 *  FILE* pFile = fopen(szFileName, "rb");
	 *
	 *  CoImageJPG testImg, Img; 
	 *  if ( testImg.Decode (pFile) ) 
	 *  {
	 *		Img.Create (testImg, TRUE);
	 *  	FILE* pFile1 = fopen(szFileName1, "wb");
	 *		Img.Encode(pFile1);
	 *  	fclose(pFile1);
	 *  }
	 *  fclose(pFile);
	 * ...
	 * @endcode
	 * 
	 */
	bool Encode(FILE* pFile);
#endif // CVLIB_IMG_SUPPORT_ENCODE
	
};

#endif

}

#endif //CVLIB_IMG_NOCODEC

#endif //_COIMAGEJPG_H_