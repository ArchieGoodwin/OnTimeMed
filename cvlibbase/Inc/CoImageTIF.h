
/*! 
 * \file    CoImageTIF.h
 * \ingroup base
 * \brief   TIFFÂô»¥Ì© Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ¼³Ë¼Êé
 */

#ifndef _COIMAGETIF_H_
#define _COIMAGETIF_H_

#ifndef CVLIB_IMG_NOCODEC

#include "CoImage.h"

namespace CVLib 
{

/**
 * @brief png Ãùº¬Ë¾ ËÏ°Ö ¸ó±ãÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷
 */

#if CVLIB_IMG_SUPPORT_TIF

class CVLIB_DECLSPEC CoImageTIF : public CoImage
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	CoImageTIF () {m_tif2=NULL; m_multipage=false; m_pages=0;}
	//! ÃÍ¾Ë»ô 
	~CoImageTIF ();

	/**
	 * @brief   tiffÃùº¬Ë¾ ËÏ²÷³Þ.
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
	 *		"c:/Test/tif/Dunning Notice",
	 *  	"c:/Test/tif/E-Ticket",
	 *		"c:/Test/tif/lego",
	 *  	"c:/Test/tif/Tax Receipt"
	 *  };
	 *  
	 *	for (int i = 0; i < sizeof(szFilenamelist) / sizeof(szFilenamelist[0]); i ++)
	 *  {
	 *		char* szFileName = szFilenamelist[i];
	 * 		char szLoadName[260];
	 *  	char szSaveName[260];
	 *  	sprintf (szLoadName, "%s.tif", szFileName);
	 *		sprintf (szSaveName, "%s_200.tif", szFileName);
	 *  	
	 *		FILE* pFile = fopen(szLoadName, "rb");
	 *  	CoImage testImg;
	 *  	if ( testImg.Decode (pFile, CVLIB_IMG_FORMAT_TIF) ) 
	 *  	{
	 *			testImg.Save(szSaveName, CVLIB_IMG_FORMAT_TIF);
	 *  	}
	 *		fclose(pFile);
	 *  }
	 * 
	 * @endcode
	 * 
	 */
	bool Decode (FILE* pFile);

// 	TIFF* TIFFOpenEx(XFileDisk* hFile);
// 	void  TIFFCloseEx(TIFF* tif);

#if CVLIB_IMG_SUPPORT_ENCODE
	/**
	 * @brief   tiffÃùº¬Ë¾ ¸ó±ãÂÙ³Þ.
	 *
	 * @param   pFile   [in] : ÁÄËÎ¼è¼¬»ô 
	 * @param   bAppend [in] : ¾¡°¡¸Ò»¥º¼¼³ 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Decode()
	 *
	 */
	bool Encode(FILE* pFile, bool bAppend = false);
#endif // CVLIB_IMG_SUPPORT_ENCODE

protected:
	void TileToStrip(unsigned char* out, unsigned char* in,	unsigned int rows, unsigned int cols, int outskew, int inskew);
	bool EncodeBody(void *m_tif, bool multipage=false, int page=0, int pagecount=0);
	void* m_tif2;
	bool  m_multipage;
	int   m_pages;
};

#endif

}

#endif //CVLIB_IMG_NOCODEC

#endif //_COIMAGETIF_H_