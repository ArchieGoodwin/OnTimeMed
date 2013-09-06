/*!
 * \file	BitOperation.cpp
 * \ingroup base
 * \brief	¹¾Àâ¼¿»õË¾ Ì¡ÂÙ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÙ³Ş.
 * \author	±®½èË§
 */

#include "BitOperation.h"

namespace CVLib
{

//! Êú°ıºãÂô(int)¸êºãÌ© ¾×´İ°ª
#define MAXINT	( 1 << 31 )

unsigned int BitOperation::ROR(unsigned int c, int n, int bits )
{
	return (c >> n) | ((c & ((1 << n) - 1)) << (bits - n));
}

unsigned int BitOperation::ROL(unsigned int c, int n, int bits )
{
	return (c << n) | ((c & (((1 << n) - 1) << (bits - n))) >> (bits - n)) & ((1 << bits) - 1);
}

int BitOperation::OneCount(unsigned int c, int bits)
{
	int count = 0, base = 1;
	for (int i=0; i<bits;i++)
		{
			if (c & base) count++;
			base <<= 1;
		}
	return count;
}

int BitOperation::Transitions(unsigned int c, int bits)
{
	int base = 1;
	int current = c & base, current2, changes = 0;
	for (int i=1;i<bits;i++)
		{
			base <<= 1;
			current2 = (c & base) >> i;
			if (current ^ current2) changes++;
			current = current2;
		}
	return changes; //(changes <= 2)? 1 : 0;
}

unsigned int BitOperation::RotMin(unsigned int n, int bits)
{
	unsigned int tmp = n << sizeof(int)*4;
	unsigned int lowmask = MAXINT >> (sizeof(int)*4-1);
	unsigned int min = tmp;
	int minIndex=0;
	for (int i=1;i<bits;i++)
		{
			tmp >>= 1;
			tmp |= (tmp & lowmask) << bits;
			tmp &= ~lowmask;
			if (tmp<min)
				{
					min = tmp;
					minIndex = i;
				}
		}
	tmp = (n << sizeof(int)*4) - minIndex;
	tmp |= (tmp & lowmask) << bits;
	tmp &= ~lowmask;
	return tmp >> sizeof(int)*4;
}

int BitOperation::HammingDistance(unsigned int a, unsigned int b, int bits)
{
	int c = a^b;
	int dist = 0, base = 1;
	for (int i=0;i<bits;i++)
		{
			if (base & c) dist++;
			base <<= 1;
		}
	return dist;
}

}
