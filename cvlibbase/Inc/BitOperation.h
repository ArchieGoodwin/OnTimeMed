
/*!
 * \file	BitOperation.h
 * \ingroup base
 * \brief	���⼿��˾ ̡�� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _BITOPERATION_H_
#define _BITOPERATION_H_

#include "cvlibbaseDef.h"

namespace CVLib
{

/**
 * @brief  ���⼿��˾ ̡�� �е���
 */
class CVLIB_DECLSPEC BitOperation
{
public:

	/**
	 * @brief   ���̼� �㶦 ��ɸ˺�� n���� �伭���ֳ�.
	 *
	 * @param   c    [in] :	�伭��� �� 
	 * @param   n    [in] :	�伭�����  
	 * @param   bits [in] :	˱ã����� 
	 * @return	�αᰪ 
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
	 * @brief   ���̼� �㶦 �ﶥɸ˺�� n���� �伭���ֳ�.
	 *
	 * @param   c    [in] :	�伭��� ��
	 * @param   n    [in] :	�伭����� 
	 * @param   bits [in] :	˱ã�����
	 * @return	�αᰪ 
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
	 * @brief   2����溷 1̩ ���㶦 �в� �ܺ��˳�. 
	 *
	 * @param   c    [in] :	���̼� �� 
	 * @param   bits [in] :	˱ã�����
	 * @return	1̩ ���� \n
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
	 * @brief   2����溷 0�溷 1, ���� 1�溷 0˺�� ���� ���(������)�� ���ٳ�.
	 *
	 * @param   c    [in] : ���̼� �� 
	 * @param   bits [in] : ˱ã����� 
	 * @return	�αᰪ(������)  
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
	 * @brief   2��㶦 �� �׺Ϻ�� �伭���ֳ�.
	 *
	 * @param   n    [in] : ���̼� �� 
	 * @param   bits [in] : ˱ã����� 
	 * @return	�αᰪ 
	 *
	 */	
	static unsigned int RotMin(unsigned int n, int bits = 8);

	/**
	 * @brief   �� 2��㺡��̩ �׷𰺶��� �˺��ٳ�. 
	 *
	 * @param   a    [in] : ��1
	 * @param   b    [in] : ��2
	 * @param   bits [in] : ˱ã����� 
	 * @return	�׷𰺶�  
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