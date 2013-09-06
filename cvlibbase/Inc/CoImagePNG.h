
/*! 
 * \file    CoImagePNG.h
 * \ingroup base
 * \brief   PNGÂô»¥Ì© Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ¼³Ë¼Êé
 */

#ifndef _COIMAGEPNG_H_
#define _COIMAGEPNG_H_

// #include <setjmp.h>
// #include <stdlib.h>
#ifndef CVLIB_IMG_NOCODEC

#include "CoImage.h"
#include "XFile.h"
#include "XFileDisk.h"

namespace CVLib 
{

/**
 * @brief png Ãùº¬Ë¾ ËÏ°Ö ¸ó±ãÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷
 */

#if CVLIB_IMG_SUPPORT_PNG

class CVLIB_DECLSPEC CoImagePNG : public CoImage
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	CoImagePNG ();

	/**
	 * @brief   pngÃùº¬Ë¾ ËÏ²÷³Þ.
	 *
	 * @param   pFile [in]  ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Encode()
	 *
	 * @par Example:
	 * @code
	 *
	 *  char* szFilenamelist[] = {
	 *  	"c:/Test/png/BRDRL1A",
	 *		"c:/Test/png/doxygen",
	 *  	"c:/Test/png/glmda",
	 *		"c:/Test/png/ms34_03",
	 *  	"c:/Test/png/msfree",
	 *  	"c:/Test/png/news",
	 *  	"c:/Test/png/thanks10",
	 *		"c:/Test/png/webalizer"
	 *  };
	 *
	 *  for (int i = 0; i < sizeof(szFilenamelist) / sizeof(szFilenamelist[0]); i ++)
	 *  {
	 *  	char* szFileName = szFilenamelist[i];
	 *		char szLoadName[260];
	 *  	char szSaveName[260];
	 *		sprintf (szLoadName, "%s.png", szFileName);
	 *  	sprintf (szSaveName, "%s_100.png", szFileName);
	 *  
	 *  	FILE* pFile = fopen(szLoadName, "rb");
	 *		CoImage testImg;
	 *  	if ( testImg.Decode (pFile, CVLIB_IMG_FORMAT_PNG) ) 
	 *		{
	 *  		testImg.Save(szSaveName, CVLIB_IMG_FORMAT_PNG);
	 * 		}
	 *		fclose(pFile);endcode
	 *  }
	 *
	 * @endcode
	 *
	 */
	bool Decode (FILE* pFile);

#if CVLIB_IMG_SUPPORT_ENCODE
	/**
	 * @brief   pngÃùº¬Ë¾ ¸ó±ãÂÙ³Þ.
	 *
	 * @param   pFile [in]  ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Decode()
	 *
	 */
	bool Encode(FILE* pFile);
#endif // CVLIB_IMG_SUPPORT_ENCODE

protected:
	void expand2to4bpp(BYTE* prow);
};

#endif

}
#endif //CVLIB_IMG_NOCODEC

#endif //_COIMAGEPNG_H_