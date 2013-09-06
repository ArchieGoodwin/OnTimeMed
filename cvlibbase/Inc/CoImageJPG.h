/*! 
 * \file    CoImageJPG.h
 * \ingroup base
 * \brief   JPG����̩ �����е����� ��̩��.
 * \author  ��˼��
 */

#ifndef _COIMAGEJPG_H_
#define _COIMAGEJPG_H_

#ifndef CVLIB_IMG_NOCODEC

#include "CoImage.h"

namespace CVLib 
{

/**
 * @brief jpg ����˾ �ϰ� ����ױ� ̡�� �е���
 */

#if CVLIB_IMG_SUPPORT_JPG

class CVLIB_DECLSPEC CoImageJPG : public CoImage
{
public:
	// Construction and Destruction
	//! �龢��
	CoImageJPG ();

	/**
	 * @brief   jpg����˾ �ϲ���.
	 *
	 * @param   pFile [in]  ���μ輬�� 
	 * @return  �°��˷� true, ��²�˷� false
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
	 * @brief   jpg����˾ ����ٳ�.
	 *
	 * @param   pFile [in]  ���μ輬�� 
	 * @return  �°��˷� true, ��²�˷� false
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