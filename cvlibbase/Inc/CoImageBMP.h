/*! 
 * \file    CoImageBMP.h
 * \ingroup base
 * \brief   BMP����̩ �����е����� ��̩��.
 * \author  ����˧
 */

#ifndef _COIMAGEBMP_H_
#define _COIMAGEBMP_H_

#include "CoImage.h"

namespace CVLib 
{

/**
 * @brief bmp����˾ �ϰ� ����ױ� ̡�� �е���
 */

#if CVLIB_IMG_SUPPORT_BMP

class CVLIB_DECLSPEC CoImageBMP : public CoImage
{
public:
	// Construction and Destruction
	//! �龢��
	CoImageBMP ();

	/**
	 * @brief  BMP����˾ �ϲ���.
	 *
	 * @param   pFile [in] : ���μ輬��
	 * @return  �°��˷� true, ��²�˷� false
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
	 * @brief  BMP����˾ ����ٳ�.
	 *
	 * @param   pFile [in] : ���μ輬��
	 * @return  �°��˷� true, ��²�˷� false
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