
/*!
 * \file	BitOperation.h
 * \ingroup base
 * \brief	¹¾Àâ¼¿»õË¾ Ì¡ÂÙ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _BITOPERATION_H_
#define _BITOPERATION_H_

#include "cvlibbaseDef.h"

namespace CVLib
{

/**
 * @brief  ¹¾Àâ¼¿»õË¾ Ì¡ÂÙ ¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC BitOperation
{
public:

	/**
	 * @brief   ¼ÑÊÌ¼ê ºã¶¦ ËùÉ¸Ëºµá n¹¾Àâ Ãä¼­»¤¿Ö³Ş.
	 *
	 * @param   c    [in] :	Ãä¼­´ó²÷ ºã 
	 * @param   n    [in] :	Ãä¼­¹¾Àâºã  
	 * @param   bits [in] :	Ë±Ã£¹¾Àâºã 
	 * @return	°Î±á°ª 
	 *
	 * @par Example:
	 * @code
	 *  ...
	 *	unsigned int a, b;
	 *
	 *	a = BitOperation::ROL(0xfd93, 3, 8);
	 *	assert (a == 0x7ec9c);
	 *	a = BitOperation::ROL(0xfd93, 3, 32);
	 *	assert (a == 0x7ec98);
	 *  ...
	 * @endcode
	 * 
	 */
	static unsigned int ROL(unsigned int c, int n, int bits = 8);
	
	/**
	 * @brief   ¼ÑÊÌ¼ê ºã¶¦ Êï¶¥É¸Ëºµá n¹¾Àâ Ãä¼­»¤¿Ö³Ş.
	 *
	 * @param   c    [in] :	Ãä¼­´ó²÷ ºã
	 * @param   n    [in] :	Ãä¼­¹¾Àâºã 
	 * @param   bits [in] :	Ë±Ã£¹¾Àâºã
	 * @return	°Î±á°ª 
	 *
	 * @par Example:
	 * @code
	 *  ...
	 *	unsigned int a, b;
	 *
	 *	b = BitOperation::ROR(0xfd93, 4, 8);
	 *	assert (b == 0xff9);
	 *	b = BitOperation::ROR(0xfd93, 4, 32); 
	 *	assert (b == 0x30000fd9);
	 *  ...
	 * @endcode
	 * 
	 */	
	static unsigned int ROR(unsigned int c, int n, int bits = 8);
	
	/**
	 * @brief   2¼êºãËæº· 1Ì© ±¶ºã¶¦ ÊĞ²÷ ÂÜºãËË³Ş. 
	 *
	 * @param   c    [in] :	¼ÑÊÌ¼ê ºã 
	 * @param   bits [in] :	Ë±Ã£¹¾Àâºã
	 * @return	1Ì© ±¶ºã \n
	 *
	 * @par Example:
	 * @code
	 *  ...
	 *	unsigned int a, b;
	 *
	 *	b = BitOperation::OneCount(0xfd93, 8);
	 *	assert (b == 4);
	 *	b = BitOperation::OneCount(0xfd93, 16);
	 *	assert (b == 11);
	 *  ...
	 * @endcode
	 * 
	 */	
	static int OneCount(unsigned int c, int bits = 8);

	/**
	 * @brief   2¼êºãËæº· 0Ëæº· 1, Åü²÷ 1Ëæº· 0Ëºµá ¸ÆÅÀ Ãäºã(¼­´ªºã)¶¦ °éÂÙ³Ş.
	 *
	 * @param   c    [in] : ¼ÑÊÌ¼ê ºã 
	 * @param   bits [in] : Ë±Ã£¹¾Àâºã 
	 * @return	°Î±á°ª(¼­´ªºã)  
	 *
	 * @par Example:
	 * @code
	 *  ...
	 *	unsigned int b;
	 *
	 *	b = BitOperation::Transitions(0xfd93, 8);
	 *	assert (b == 4);
	 *	b = BitOperation::Transitions(0xfd93, 16);
	 *	assert (b == 6);
	 *  ...
	 * @endcode
	 * 
	 */	
	static int Transitions(unsigned int c, int bits = 8);

	/**
	 * @brief   2¼êºã¶¦ °û ¾×ºÏºãµá Ãä¼­»¤¿Ö³Ş.
	 *
	 * @param   n    [in] : ¼ÑÊÌ¼ê ºã 
	 * @param   bits [in] : Ë±Ã£¹¾Àâºã 
	 * @return	°Î±á°ª 
	 *
	 */	
	static unsigned int RotMin(unsigned int n, int bits = 8);

	/**
	 * @brief   ´¸ 2¼êºãº¡ËËÌ© Â×·ğ°º¶®¶¦ ±Ëº¤ÂÙ³Ş. 
	 *
	 * @param   a    [in] : ºã1
	 * @param   b    [in] : ºã2
	 * @param   bits [in] : Ë±Ã£¹¾Àâºã 
	 * @return	Â×·ğ°º¶®  
	 *
	 * @par Example:
	 * @code
	 *
	 *  int i, j;
	 *  
	 *  Mat mHDist;
	 *  mHDist.Create(32, 32, MAT_Tint);
	 *  for(i = 0; i < 32; i ++)
	 *  for (j = 0; j < 32; j ++)
	 *  mHDist.data.i[i][j] = BitOperation::HammingDistance(i, j, 8);
	 *  
	 *	for(i = 0; i < 32; i ++)
	 *	   for(j = 0; j < 32; j ++)
	 *	   {
	 *	       if(i == j)
	 *	          assert( mHDist.data.i[i][j] == 0 );
	 *	       else
	 *	          assert( mHDist.data.i[i][j] == mHDist.data.i[j][i] );
	 *	   }
	 *	 
	 *  assert( mHDist.data.i[1][6] == 3 );
	 *
	 *  mHDist.Release(); 
	 *  ...
	 * @endcode
	 * 
	 */	
	static int HammingDistance(unsigned int a, unsigned int b, int bits = 8);
	
};

}

#endif // _BITOPERATION_H_