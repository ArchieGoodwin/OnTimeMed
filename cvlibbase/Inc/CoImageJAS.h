/*! 
 * \file    CoImageJAS.h
 * \ingroup base
 * \brief   Jasper����̩ �����е����� ��̩��.
 * \author  ��˼��
 */

#ifndef _COIMAGEJAS_H_
#define _COIMAGEJAS_H_


#ifndef CVLIB_IMG_NOCODEC


#include "CoImage.h"

namespace CVLib 
{

/**
 * @brief jasper ����˾ �ϰ� ����ױ� ̡�� �е���
 */

#if CVLIB_IMG_SUPPORT_JAS

class  CVLIB_DECLSPEC CoImageJAS : public CoImage
{
public:
	// Construction and Destruction
	//! �龢��
	CoImageJAS ();

	/**
	 * @brief   jasper����˾ �ϲ���.
	 *
	 * @param   pFile      [in] : ���μ輬��
	 * @param   nImagetype [in] : ����̩ ���� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  Encode()
	 *
	 * @par Example:
	 * @code
	 *
	 *  char* szFilenamelist[] = {
	 *  	"c:/Test/jas/yaleB10_P000001.pgm",
	 *		"c:/Test/jas/yaleB10_P000002.pgm",
	 *  	"c:/Test/jas/image_0004.pnm.ppm",
	 *		"c:/Test/jas/image_0005.pnm.ppm",
	 *  	"c:/Test/jas/image_0006.pnm.ppm",
	 *  	"c:/Test/jas/image_0007.pnm.ppm"
	 *  	};
	 *	
	 *  for (int i = 0; i < sizeof(szFilenamelist) / sizeof(szFilenamelist[0]); i ++)
	 *  {
	 *		char* szFileName = szFilenamelist[i];
	 *		char szLoadName[260];
	 *		char szSaveName[260];
	 *		sprintf (szLoadName, "%s", szFileName);
	 *		sprintf (szSaveName, "%s_xxx.bmp", szFileName);
	 *	
	 *		FILE* pFile = fopen(szLoadName, "rb");
	 *		CoImage testImg;
	 *		if ( testImg.Decode (pFile, CVLIB_IMG_FORMAT_PNM) ) 
	 *		{
	 *			testImg.Save(szSaveName, CVLIB_IMG_FORMAT_BMP);
	 *		}
	 *		fclose(pFile);
	 *	}
	 *
	 * @endcode
	 * 
	 */
	bool Decode (FILE* pFile, DWORD nImagetype);

#if CVLIB_IMG_SUPPORT_ENCODE
	/**
	 * @brief   jasper����˾ ����ٳ�.
	 *
	 * @param   pFile      [in] : ���μ輬��
	 * @param   nImagetype [in] : ����̩ ���� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  Decode()
	 * 
	 */
	bool Encode(FILE* pFile, DWORD nImagetype);
#endif // CVLIB_IMG_SUPPORT_ENCODE
	
};

#endif //CVLIB_IMG_SUPPORT_JAS

}


#endif //CVLIB_IMG_NOCODEC

#endif //_COIMAGEJAS_H_