/*!
 * \file
 * \brief ¼¬¹¤Ãùº¬Ë¾ °éÂ×±¨ Ì¡ÂÙ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÙ³Ş.
 * \author CholU Kim
 */
#ifndef _INTEGRALIMAGE_H_
#define _INTEGRALIMAGE_H_

#include "cvlibbase/inc/Mat.h"

namespace CVLib 
{

/**
 @brief  ¼¬¹¤Ãùº¬»¶ºÂ ¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC IntegralImage : public Object
{
public:
	/**
	 @brief  °é¾¢»ô
	 */
	IntegralImage (){};

	/**
	 @brief  ÁÄ±Ğ»ô
	 */
    virtual ~IntegralImage(){};

	/**
	 @brief  ¼¬¹¤Ãùº¬Ë¾ °éÂÙ³Ş.
	 @param		pmImg		[in]	ËÓµÙÃùº¬
	 @param		pmSum		[out]	±Ëº¤´ô ¼¬¹¤Ãùº¬.
	 @param		pmSquare	[out]	±Ëº¤´ô ´¸½£°ß¼¬¹¤Ãùº¬.
			ËË ÁÄµ¹¸¡À¾°¡ ËÓµÙ»¤Ëæ NULLËË·² ´¸½£°ß¼¬¹¤Ãùº¬Ë¾ °éÂ×¼è Ê±²÷³Ş.
	 */
	void	Process (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);

	/**
	 @brief  ¼¬¹¤Ãùº¬Ë¾ °éÂÙ³Ş.
	 @param		pmImg		[in]	ËÓµÙÃùº¬
	 @param		pmSum		[out]	±Ëº¤´ô ¼¬¹¤Ãùº¬.
	 @param		pmTilted	[out]	±Ëº¤´ô °Òº¡¼ê ¼¬¹¤Ãùº¬.
	 @param		pmSquare	[out]	±Ëº¤´ô ´¸½£°ß¼¬¹¤Ãùº¬.
			ËË ÁÄµ¹¸¡À¾°¡ ËÓµÙ»¤Ëæ NULLËË·² ´¸½£°ß¼¬¹¤Ãùº¬Ë¾ °éÂ×¼è Ê±²÷³Ş.
	 */
	void	ProcessEx (Mat* pmImg, Mat* pmSum, Mat* pmTilted, Mat* pmSquare = NULL);

protected:
	void	iProcess_Byte (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);
	void	iProcess_int (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);
	void	iProcess_float (Mat* pmImg, Mat* pmSum, Mat* pmSquare = NULL);
};

}

#endif //_INTEGRALIMAGE_H_