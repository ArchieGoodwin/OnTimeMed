/*!
 * \file    MathLib.cpp
 * \ingroup math
 * \brief   ÃÔµÛº·°Ö¶¦ ¼³Ì©ÂÙ³Þ.
 * \author  ±®½èË§
 */

#include "MathLib.h"

namespace CVLib
{

int Rounding ( int nValue )
{
	int nAbsValue;

	nAbsValue = Abs( nValue );

	nAbsValue = ( ( nAbsValue + 512 ) >> 10 );

	if ( nValue < 0 )
 		nValue = - nAbsValue;
 	else
 		nValue = nAbsValue;

	return nValue;/* Q0 */
}

int	RightShift(int nValue, int nShiftBits)
{
	int	nTemp = nValue >> nShiftBits;
	nValue = (nValue + (1 << (nShiftBits - 1))) >> nShiftBits;
	if ((nTemp ^ nValue) & 0x80000000)
		nValue = nTemp;
	return nValue;
}

int	DivisionProcess(int nNumerator, int nDenominator, int* pnQuotient)
{
	int	sign = (nNumerator ^ nDenominator) >> 31;
	/* ¹¤·º°¡ µßËÍ °ÒË§ */
	if (nDenominator == 0)
	{
		if (nNumerator >= 0)
		{
			(*pnQuotient) = 0x7fffffff;
			return 0x7fffffff;
		}
		else
		{
			(*pnQuotient) = 0x80000000;
			return 0x7fffffff;
		}
	}
	/* ¹¤»ô°¡ µßËÍ °ÒË§ */
	else if (nNumerator == 0)
	{
		(*pnQuotient) = 0;
		return 0;
	}

	nNumerator = (nNumerator >= 0) ? nNumerator : -nNumerator;
	int	nDenominator1 = nDenominator = (nDenominator >= 0) ? nDenominator : -nDenominator;
	int	nQuotient = 0;
	if (nNumerator > nDenominator)
	{
		while (nNumerator >= nDenominator1 && nDenominator1 < 0x40000000)
			nDenominator1 <<= 1;
		if (nDenominator1 > nNumerator)
			nDenominator1 >>= 1;

		while (nDenominator1 > nDenominator)
		{
			nQuotient <<= 1;
			if (nNumerator > nDenominator1)
			{
				nQuotient |= 1;
				nNumerator -= nDenominator1;
			}
			nDenominator1 >>= 1;
		}
		nQuotient <<= 1;
		if (nNumerator > nDenominator1)
		{
			nQuotient |= 1;
			nNumerator -= nDenominator1;
		}
	}

	int	n = 0;
	if (nNumerator > 0)
	{
		unsigned int	nNumerator1 = (unsigned int)nNumerator;
		unsigned int	nDenominator1 = (unsigned int)nDenominator;
		while (nQuotient < 0x40000000)
		{
			nNumerator1 <<= 1;
			nQuotient <<= 1;
			n++;
			if (nNumerator1 > nDenominator1)
			{
				nQuotient |= 1;
				nNumerator1 -= nDenominator1;
			}
		}
	}

	(*pnQuotient) = (sign) ? -nQuotient : nQuotient;

	return n;
}

int64	MultiplyProcess(int nValue1, int nValue2)
{
	int64	nProduct;

	if ((nValue1 == 0) || (nValue2 == 0))
#if defined(_WIN32) || defined(_CE)
		nProduct = (int64)0x0000000000000000L;
#else
		nProduct = (int64)0x0000000000000000LL;
#endif
	else
	{
		int	sign = (nValue1 ^ nValue2) >> 31;
		nValue1 = (nValue1 >= 0) ? nValue1 : -nValue1;
		nValue2 = (nValue2 >= 0) ? nValue2 : -nValue2;
		unsigned int	nLowValue1 = (unsigned int)nValue1 & 0x0000ffff;
		unsigned int	nHighValue1 = (unsigned int)(nValue1 >> 16) & 0x0000ffff;
		unsigned int	nLowValue2 = (unsigned int)nValue2 & 0x0000ffff;
		unsigned int	nHighValue2 = (unsigned int)(nValue2 >> 16) & 0x0000ffff;

		uint64	nTmp1 = (uint64)(nLowValue1 * nLowValue2);
		uint64	nTmp2 = (uint64)(nLowValue1 * nHighValue2 + nLowValue2 * nHighValue1);
		uint64	nTmp3 = (uint64)(nHighValue1 * nHighValue2);
		nProduct = (int64)(nTmp1 + (nTmp2 << 16) + (nTmp3 << 32));

		if (sign)
			nProduct = -nProduct;
	}

	return nProduct;
}

int	LogarithmProcess(int nValue)
{
	int	nLogValue;	/* Q16 */

	if (nValue <= 1)
		nLogValue = 0;
	else
	{
		nLogValue = 30;
		while (nValue < 0x40000000)
		{
			nValue <<= 1;
			nLogValue--;
		}
		nValue -= 0x40000000;	/* Q30 */

		int	n = nValue >> (30 - LOG_SCALE_BITS);	/* µá°ûÁìÌ© »¤»õ¸ÜÂö */
		nValue -= (n << (30 - LOG_SCALE_BITS));		/* µá°ûÁìËæº· ¸ÜÂöÊï½Ó (Q30) */
		int64	nTempValue64 = MultiplyProcess((pnLogTable[n + 1] - pnLogTable[n]), nValue);	/* Q(46 - LOG_SCALE_BITS) */
		nValue = (int)(nTempValue64 >> (30 - LOG_SCALE_BITS));	/* Q16 */
		nLogValue = (nLogValue << 16) + pnLogTable[n] + nValue;
	}

	return nLogValue;	/* 32bit Êú°ýºãÌ© 2Ì© µá°û°ª ±ÙÃû */
}

int	DivisionProcess64(int64 nNumerator, int64 nDenominator, int64* pnQuotient)
{
	int	sign = (int)((nNumerator ^ nDenominator) >> 63);

	if (nDenominator == 0)
	{
		if (nNumerator >= 0)
		{
			(*pnQuotient) = 0x7fffffff;
			return 0x7fffffff;
		}
		else
		{
			(*pnQuotient) = 0x80000000;
			return 0x7fffffff;
		}
	}
	else if (nNumerator == 0)
	{
		(*pnQuotient) = 0;
		return 0;
	}

	nNumerator = (nNumerator >= 0) ? nNumerator : -nNumerator;
	int64	nDenominator1 = nDenominator = (nDenominator >= 0) ? nDenominator : -nDenominator;

#if defined(_WIN32) || defined(_CE)
	int64	nQuotient = (int64)0x0000000000000000L;
#else
	int64	nQuotient = (int64)0x0000000000000000LL;
#endif
	if (nNumerator > nDenominator)
	{
#if defined(_WIN32) || defined(_CE)
		while (nNumerator >= nDenominator1 && nDenominator1 < (int64)0x4000000000000000L)
#else
		while (nNumerator >= nDenominator1 && nDenominator1 < (int64)0x4000000000000000LL)
#endif
			nDenominator1 <<= 1;
		if (nDenominator1 > nNumerator)
			nDenominator1 >>= 1;

		while (nDenominator1 > nDenominator)
		{
			nQuotient <<= 1;
			if (nNumerator > nDenominator1)
			{
				nQuotient |= 1;
				nNumerator -= nDenominator1;
			}
			nDenominator1 >>= 1;
		}
		nQuotient <<= 1;
		if (nNumerator > nDenominator1)
		{
			nQuotient |= 1;
			nNumerator -= nDenominator1;
		}
	}

	int	n = 0;
	if (nNumerator > 0)
	{
		uint64 nNumerator1 = (uint64)nNumerator;
		uint64 nDenominator1 = (uint64)nDenominator;
#if defined(_WIN32) || defined(_CE)
		while (nQuotient < (int64)0x4000000000000000L)
#else
		while (nQuotient < (int64)0x4000000000000000LL)
#endif
		{
			nNumerator1 <<= 1;
			nQuotient <<= 1;
			n++;
			if (nNumerator1 > nDenominator1)
			{
				nQuotient |= 1;
				nNumerator1 -= nDenominator1;
			}
		}
	}

	(*pnQuotient) = (sign) ? -nQuotient : nQuotient;

	return n;
}

int	SquareRootProcess(int64 nPowerValue, int* pnRootValue)
{
	if (nPowerValue < 0)
	{
		*pnRootValue = -1;
		return 0;
	}
	else if (nPowerValue == 0)
	{
		*pnRootValue = 0;
		return 0;
	}
	
	int	point = 0;
#if defined(_WIN32) || defined(_CE)
	if (nPowerValue >= (int64)0x4000000000000000L)
#else
	if (nPowerValue >= (int64)0x4000000000000000LL)
#endif
	{
		nPowerValue = (nPowerValue + 2) >> 2;
		point--;
	}

	int	nShiftBits = 60;
	do {
#if defined(_WIN32) || defined(_CE)
		if (((int64)0x0000000000000003L << nShiftBits) & nPowerValue)
#else
		if (((int64)0x0000000000000003LL << nShiftBits) & nPowerValue)
#endif
			break;
		nShiftBits -= 2;		
	} while(1);

	int	nRootValue;
	int	nValidBitPoint;
	int	nNumerator;
	int	nDenominator;
	{
		nNumerator = ((int)(nPowerValue >> nShiftBits) & 0x0003) - 1;
		nRootValue = 1;
		nValidBitPoint = 0;
		nDenominator = 2;
		nShiftBits -= 2;
	}
	while (nShiftBits >= 0)
	{
		nNumerator = (nNumerator << 2) | ((int)(nPowerValue >> nShiftBits) & 0x0003);
		nRootValue <<= 1;
		nValidBitPoint++;
		nDenominator <<= 1;
		if ((nDenominator + 1) <= nNumerator)
		{
			nDenominator++;
			nNumerator -= nDenominator;
			nDenominator++;
			nRootValue++;
		}
		nShiftBits -= 2;
	}
	while (nValidBitPoint < 30)
	{
		nNumerator <<= 2;
		nRootValue <<= 1;
		nValidBitPoint++;
		nDenominator <<= 1;
		if ((nDenominator + 1) <= nNumerator)
		{
			nDenominator++;
			nNumerator -= nDenominator;
			nDenominator++;
			nRootValue++;
		}
		point++;
	}
	*pnRootValue = nRootValue;

	return point;
}

int	SineProcess(int nPhase)
{
	int	nSign;
	int	nTwicePI = (PI_Q15 << 1);	/* (2 * PI) */
	int	nHalfPI = (PI_Q15 >> 1);	/* (PI / 2) */
	/* ¹¢Âöº¼¼³ */
	if ((nPhase % nTwicePI) == 0)
		return 0;
	else if (nPhase > 0)
		nSign = 1;
	else
	{
		nPhase = -nPhase;
		nSign = -1;
	}
	/* ËÓµÙÌ¡º¬°ªË¾ 0¹¢À¾ (PI / 2)º¡ËËÌ© °ªËºµá ¼³°öÃù */
	while (nPhase > nTwicePI)
		nPhase -= nTwicePI;
	if (nPhase > PI_Q15)
	{
		nSign = -nSign;
		nPhase -= PI_Q15;
	}
	if (nPhase > nHalfPI)
		nPhase = PI_Q15 - nPhase;
	/* ÁìÌ© »¤»õ¼° Àµ»° */
	int	nStartPoint;
	int	point = DivisionProcess((nPhase << 9), PI_Q15, &nStartPoint);
	nStartPoint >>= point;
	if (nStartPoint >= TRIANGLE_SCALE_SIZE)
		return (nSign << 15);
	/* Áì»¤»õ¼°±áÌ© ÁÝ½Ó */
	int	nViasValue = nPhase - (nStartPoint * PI_Q15 >> 9);	/* Q15 */
	/* Áì»¤»õ¼°±áÌ© ÂÜºã°ª ÁÝ½Ó */
	int	nErrorValue;
	point = DivisionProcess(((pnSineTable[nStartPoint + 1] - pnSineTable[nStartPoint]) * nViasValue), PI_Q15, &nErrorValue);
	if (point > 9)
		nErrorValue = RightShift(nErrorValue, (point - 9));
	else
		nErrorValue <<= (9 - point);

	return (nSign * (pnSineTable[nStartPoint] + nErrorValue));	/* sin°ª ±ÙÃû */
}

int	ArcsineProcess(int nPhase, int* pnResult)
{
	/* ËÓµÙ°ªËË arcsinÂÜºãÌ© Æ·°éÊßË¾ ¸ãÊÌ±ý·² <»¨Â²> */
	if ((nPhase < (-1 << 15)) || (nPhase > (1 << 15)))
		return -1;
	/* ËÓµÙ°ªËË µßËË·² µß°ª ±ÙÃû */
	else if (nPhase == 0)
	{
		*pnResult = 0;
		return 0;
	}
	int	nSign = 1;
	if (nPhase < 0)
	{
		nPhase = -nPhase;
		nSign = -1;
	}
	/* ÁìÌ© »¤»õ¼° Àµ»° */
	int	nStartPoint = nPhase >> (15 - TRIANGLE_SCALE_BITS);
	if (nStartPoint >= TRIANGLE_SCALE_SIZE)
	{
		*pnResult = (PI_Q15 >> 1);
		return 0;
	}
	/* Áì»¤»õ¼°±áÌ© ÁÝ½Ó */
	int	nViasValue = nPhase - (nStartPoint << (15 - TRIANGLE_SCALE_BITS));	/* Q15 */
	/* Áì»¤»õ¼°±áÌ© ÂÜºã°ª ÁÝ½Ó */
	int	nErrorValue = RightShift((pnArcsineTable[nStartPoint + 1] - pnArcsineTable[nStartPoint]) * nViasValue, 7);	/* Q15 */
	/* arcsin°ª ±Ëº¤ */
	*pnResult = nSign * (pnArcsineTable[nStartPoint] + nErrorValue);

	return 0;
}

int	CosineProcess(int nPhase)
{
	return SineProcess((PI_Q15 >> 1) - nPhase);
}

int	ArccosineProcess(int nPhase, int* pnResult)
{
	/* ËÓµÙ°ªËË arccosÂÜºãÌ© Æ·°éÊßË¾ ¸ãÊÌ±ý·² <»¨Â²> */
	if ((nPhase < (-1 << 15)) || (nPhase > (1 << 15)))
		return -1;
	if (ArcsineProcess(nPhase, pnResult) < 0)
		return -1;
	*pnResult = (PI_Q15 >> 1) - (*pnResult);
	return 0;
}

int	TangentProcess(int nPhase, int* pnResult)
{
	int	nHalfPI = (PI_Q15 >> 1);	/* (PI / 2) */
	/* ËÓµÙÌ¡º¬Ì© ¼³°öÃù */
	while (nPhase > nHalfPI)
		nPhase -= PI_Q15;
	while (nPhase < -nHalfPI)
		nPhase += PI_Q15;
	if (nPhase == 0)
	{
		*pnResult = 0;
		return 0;
	}
	int	nSign = 1;
	if (nPhase < 0)
	{
		nPhase = -nPhase;
		nSign = -1;
	}
	/* ËÓµÙ°ªËË tanÂÜºã Æ·°éÊßÌ© Ë±Ã£¸áÌ¡¶¦  ¸ãÊÌ±ý·² <»¨Â²> */
	if (nPhase > pnTangentInputTable[TRIANGLE_SCALE_SIZE])
		return -1;
	/* ÁìÌ© »¤»õ¼° Àµ»° */
	int	nStartPoint = 1;
	while (pnTangentInputTable[nStartPoint] <= nPhase)
	{
		if (++nStartPoint > TRIANGLE_SCALE_SIZE)
			break;
	}
	/* ËÓµÙ°ªËË tanÂÜºã Æ·°éÊßÌ© Ë±Ã£¸áÌ¡Ì© ÂÙ±Ë°ªËÍ °ÒË§ */
	if (--nStartPoint >= TRIANGLE_SCALE_SIZE)
	{
		*pnResult = nSign * pnTangentOutputTable[nStartPoint];
		return 0;
	}
	/* Áì»¤»õ¼°±áÌ© ÁÝ½Ó */
	int	nViasValue = nPhase - pnTangentInputTable[nStartPoint];	/* Q15 */
	/* Áì»¤»õ¼°±áÌ© ÂÜºã°ª ÁÝ½Ó */
	int	nErrorValue;
	int	point = DivisionProcess(((pnTangentOutputTable[nStartPoint + 1] - pnTangentOutputTable[nStartPoint]) * nViasValue), (pnTangentInputTable[nStartPoint + 1] - pnTangentInputTable[nStartPoint]), &nErrorValue);
	nErrorValue = RightShift(nErrorValue, point);
	/* tan°ª ±Ëº¤ */
	*pnResult = nSign * (pnTangentOutputTable[nStartPoint] + nErrorValue);

	return 0;
}

int	ArctangentProcess(int nPhase, int* pnResult)
{
	/* ËÓµÙ°ªËË arctanÂÜºã Æ·°éÊßÌ© Ë±Ã£¸áÌ¡¶¦  ¸ãÊÌ±ý·² <»¨Â²> */
	if ((nPhase < (-16 << 11)) || (nPhase > (16 << 11)))
		return -1;
	/* ËÓµÙ°ªËË µßËË·² µß°ª ±ÙÃû */
	else if (nPhase == 0)
	{
		*pnResult = 0;
		return 0;
	}
	int	nSign = 1;
	if (nPhase < 0)
	{
		nPhase = -nPhase;
		nSign = -1;
	}
	/* ÁìÌ© »¤»õ¼° Àµ»° */
	int	nStartPoint = 1;
	while (pnTangentOutputTable[nStartPoint] <= nPhase)
	{
		if (++nStartPoint > TRIANGLE_SCALE_SIZE)
			break;
	}
	/* ËÓµÙ°ªËË arctanÂÜºã Æ·°éÊßÌ© Ë±Ã£¸áÌ¡Ì© ÂÙ±Ë°ªËÍ °ÒË§ */
	if (--nStartPoint >= TRIANGLE_SCALE_SIZE)
	{
		*pnResult = nSign * pnTangentInputTable[nStartPoint];
		return 0;
	}
	/* Áì»¤»õ¼°±áÌ© ÁÝ½Ó */
	int	nViasValue = nPhase - pnTangentOutputTable[nStartPoint];	/* Q11 */
	/* Áì»¤»õ¼°±áÌ© ÂÜºã°ª ÁÝ½Ó */
	int	nErrorValue;
	int	point = DivisionProcess(((pnTangentInputTable[nStartPoint + 1] - pnTangentInputTable[nStartPoint]) * nViasValue), (pnTangentOutputTable[nStartPoint + 1] - pnTangentOutputTable[nStartPoint]), &nErrorValue);
	nErrorValue = RightShift(nErrorValue, point);
	/* tan°ª ±Ëº¤ */
	*pnResult = nSign * (pnTangentInputTable[nStartPoint] + nErrorValue);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * @brief  µù¼°ºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤ 2½Ó³ÞÂß»¥Ì© 2½Ó¶ë´Ñ°Óºã	
 */
#define _MC_ATAN_CF0  (-15.8131890796f)
/** 
 * @brief  µù¼°ºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤ 2½Ó³ÞÂß»¥Ì© 1½Ó¶ë´Ñ°Óºã	
 */
#define _MC_ATAN_CF1  (61.0941945596f)
/** 
 * @brief  µù¼°ºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤ 2½Ó³ÞÂß»¥Ì© 0½Ó¶ë´Ñ°Óºã	
 */
#define _MC_ATAN_CF2  0.f /*(-0.140500406322f)*/

/* µù¼°ºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤Ëæº· ¹¢Âö¼¿ÂÝÁì */
static const unsigned int mcAtanSign[8] =
{ 0, (unsigned int)0x80000000, (unsigned int)0x80000000, 0, (unsigned int)0x80000000, 0, 0, (unsigned int)0x80000000 };

/** 
 * @brief  °Ö¼°Êàº¤Ëæº· ¹¾Àâ·ë±¨º¬ºã	
 */
#define	 BIT_SHIFT	10
/** 
 * @brief  Êú°ýºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤ 2½Ó³ÞÂß»¥Ì© 2½Ó¶ë´Ñ°Óºã	
 */
#define _MC_INI_ATAN_CF0	(-16193)
/** 
 * @brief  Êú°ýºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤ 2½Ó³ÞÂß»¥Ì© 1½Ó¶ë´Ñ°Óºã	
 */
#define _MC_INI_ATAN_CF1	(62560)	
/** 
 * @brief  Êú°ýºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤ 2½Ó³ÞÂß»¥Ì© 0½Ó¶ë´Ñ°Óºã	
 */
#define _MC_INI_ATAN_CF2	(-144)

/*	Êú°ýºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤Ëæº· 10¹¾Àâ·ë±¨ÂÚ ÆÁÌ© ¹¤°é¸ó¼³Áì */
static const int mcIntAtanTab[8] = { 0 + _MC_INI_ATAN_CF2,    92160 - _MC_INI_ATAN_CF2,
									184320 - _MC_INI_ATAN_CF2,  92160 + _MC_INI_ATAN_CF2,
									368640 - _MC_INI_ATAN_CF2, 276480 + _MC_INI_ATAN_CF2,
									184320 + _MC_INI_ATAN_CF2, 276480 - _MC_INI_ATAN_CF2
};

/* Êú°ýºã °ÖºÐÊ­¿ÍÀ¸±Äº÷±Ëº¤Ëæº· ¹¢Âö¼¿ÂÝÁì */
static const int64 mcIntAtanSign[8] =
{ 0, 0xffffffff, 0xffffffff, 0, 0xffffffff, 0, 0, 0xffffffff };

int FastAtan2( int y, int x )
{
	if ( x == 0 && y < 1024 )
	{
		x <<= 10;
		y <<= 10;
	}

    int ix = x, iy = y;
    int idx = (ix < 0) * 2 + (iy < 0) * 4; /* idxÌ© 1,2¹¾ÀâËæ²÷ ix,iyÌ© ¹¢Âö¶¦ ¸ÉÊé */
    int ygx;

	/*¼®´Ý°ªÊÐ±¨*/
	ix = Abs(x);
	iy = Abs(y);
	
    /* idxÌ© 0¹¾ÀâËæ²÷ ix,iyÌ© ¼®´Ý°ª¿Í±¨±ã±ËÌ® 0±ã±Ë¶¦ ¸ÉÊé */
	ygx = (iy <= ix) - 1;
    idx -= ygx;	 
    idx &= ((ix == 0) - 1) | ((iy == 0) - 1);/* ix != 0ËË°Ö iy != 0 ËË·² idx¹¢ÂöË±¼è, 
												Ê­³£·² idxÌ© 0¹¾Àâ°ªË¾ 1µá º¼¼³ */

    /* swap ix and iy if ix < iy */
    ix ^= iy & ygx;
    iy ^= ix & ygx;
    ix ^= iy & ygx;
    iy ^= mcIntAtanSign[idx];

	iy <<= BIT_SHIFT;
	ix <<= BIT_SHIFT;
    ix |= 1;	// 0±ý²ä±¨¶¦ ÂªÂ×±¨ Ì¡Â×ÊÞ
    {
        int64 z = iy;
        z <<= BIT_SHIFT;
        z <<= BIT_SHIFT;/* Êú°ýºã±ý²ä±¨Ëæ Ì©ÂÙ 0±Ëº¤Ë¾ ÂªÂ×±¨ Ì¡Â×ÊÞ */
        z = z / ix;
        int64 thita = (z > 0) ? z * _MC_INI_ATAN_CF0 :  -z * _MC_INI_ATAN_CF0;
		thita >>= BIT_SHIFT;
		thita >>= BIT_SHIFT;
		thita += _MC_INI_ATAN_CF1;
		thita *= z;
		thita >>= BIT_SHIFT;
		thita >>= BIT_SHIFT;
        thita += mcIntAtanTab[idx];
		thita += 511;
		thita >>= BIT_SHIFT;
        return (int) thita;
	}
}

int Abs( int nVal )
{
	return nVal < 0 ? -(nVal) : nVal;	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**
 * @brief CosÂÜºãÌ© °ªË¾ 1´ª ³àÌ¡µá Êú°ýºãÃù ÂÙ °ª
 */
static int gnCos[91] = 
{
	1048576,	1048416,	1047937,	1047139,	1046022,	1044586,	1042832,	1040760,	1038371,	1035666,	
	1032646,	1029311,	1025662,	1021701,	1017429,	1012847,	1007956,	1002758,	997255,		991448,	
	985339,		978930,		972223,		965219,		957922,		950333,		942454,		934288,		925838,		917105,	
	908093,     898805,		889243,		879410,		869309,		858943,		848316,		837430,		826289,		814897,	
	803256,		791370,		779244,		766880,		754282,		741455,		728402,		715127,		701634,		687928,	
	674012,		659890,		645568,		631049,		616338,		601438,		586356,		571095,		555661,		540057,	
	524288,		508360,		492277,		476044,		459665,		443147,		426494,		409711,		392803,		375776,	
	358634,		341383,		324028,		306574,		289027,		271391,		253673,		235878,		218011,		200078,	
	182083,		164033,		145934,		127789,		109606,		91389,		73145,		54878,		36595,		18300,
	0
}; /* Q20 */ 

int CosineAlpha( int rTheta )
{
	int c;
	rTheta = Abs( rTheta );
	if ( rTheta <= 90 )
	{
		c = gnCos[ rTheta ]; /* Q20 */
	}
	else
	{
		c = -gnCos[ 180 - rTheta ]; /* Q20 */
	}
	return c;
}

int SineAlpha( int rTheta )
{
	return CosineAlpha( 90 - rTheta );
}

/** 
 * @brief AtanÂÜºãÌ© °ªË¾ 1´ª ³àÌ¡µá Êú°ýºãÃù ÂÙ °ª
 */
static int atn[90] = 
{
	0,			18303,		36617,		54954,		73324,		91739,		110210,		128749,		147368,		166078,	
	184892,		203823,		222882,		242083,		261439,		280965,		300674,		320582,		340703,		361054,	
	381650,		402511,		423652,		445094,		466856,		488959,		511425,		534276,		557538,		581235,	
	605396,		630048,		655223,		680953,		707273,		734221,		761835,		790159,		819237,		849120,	
	879860,		911513,		944142,		977813,		1012598,	1048576,	1085832,	1124460,	1164562,	1206249,	
	1249644,	1294884,	1342116,	1391507,	1443241,	1497522,	1554578,	1614665,	1678072,	1745124,	
	1816187,	1891681,	1972085,	2057946,	2149899,	2248678,	2355140,	2470290,	2595317,	2731634,	
	2880939,	3045286,	3227185,	3429738,	3656819,	3913339,	4205609,	4541882,	4933162,	5394456,	
	5946770,	6620448,	7461006,	8539966,	9976534,	11985279,	14995335,	20008022,	30027288,	60072879
}; /* Q20 */

/**
 * @brief °é°£³­Ëæ ËÓ°é¸êºã dwVal°¡ ¼Á¼õÂ×²÷°¡¶¦ °Áº¡ÂÙ³Þ.
 *
 * @param	dwVal		[in]: ËÓ°é°ª
 * @param	iStart		[in]: °é°£Ì© »¤»õÌ© Index
 * @param	iEnd		[in]: °é°£Ì© ÄùÌ© Index
 * @param	pnIndex		[out]: ËÓ°é°ªÌ© ¼Á¼õÌ¡¾·
 * @return	ÊÖËÁ
 */
static void 
FindIndex( int dwVal, int iStart, int iEnd, int* pnIndex )
{
	int iMid;
	int eps;
	int nEven;

	eps = 18300;
	if ( iStart + 1 >= iEnd )
	{
		*pnIndex = 
			Abs( atn[ iStart ] - dwVal ) < Abs( atn[ iEnd ] - dwVal ) ? iStart : iEnd; 
		return; 
	}

	iMid = (iStart + iEnd) / 2;

	nEven = (iStart + iEnd) % 2;
	if( nEven )
	{
		iMid = Abs( atn[ iMid ] - dwVal ) < Abs( atn[ iMid + nEven ] - dwVal ) ? iMid : iMid + nEven; 
	}
	if ( atn[ iMid ] < dwVal )
	{
		iStart = iMid ;//+ 1;
		FindIndex( dwVal, iStart, iEnd, pnIndex );
	}
	else
	{
		iEnd = iMid ;//- 1;
		FindIndex( dwVal, iStart, iEnd, pnIndex );
	}
}

int Atan( int dwVal )//Q20
{
	int iStart = 0, iEnd = 90;
	int nRetVal;
	unsigned char fNum = true;

	if ( Abs( dwVal ) > 60072879 )
		return dwVal > 0 ? 90 : -90;

	if ( Abs( dwVal ) < 9152 )
		return 0;

	if ( dwVal < 0 )
	{
		fNum = false;
		dwVal = -dwVal;
	}
	FindIndex( dwVal, iStart, iEnd, &nRetVal );	

	return fNum ? nRetVal : -nRetVal;
};

int Atan2( int nYVal, int nXVal )
{

	if ( nXVal == 0 && nYVal > 0 )
		return 90;

 	if ( nXVal == 0 && nYVal < 0 )
		return -90;

	if ( nXVal < 0 && nYVal == 0 )
		return -180;

    if ( nXVal > 0 && nYVal < 0 )
	{
		return Atan( nYVal * 1048576 / nXVal );
	}
	if ( nXVal > 0 && nYVal > 0 )
	{
		return Atan( nYVal * 1048576 / nXVal );
	}
	if ( nXVal < 0 && nYVal < 0 )
	{
		return -180 + Atan( nYVal * 1048576 / nXVal );
	}
	if ( nXVal < 0 && nYVal > 0 )
	{
		return 180 + Atan( nYVal * 1048576 / nXVal );
	}
	return 0;
}

int SquareRootProcess32( int PowerValue )
{
	int		RootValue = 0x0000;		/* 2½£°ßÇµ¶®°Î±á°ª µß ½ó±¨Ãù */
	int		ScaleValue = 0x4000;	/* ±Ëº¤½æ´ª°ª ½ó±¨Ãù */
	int		PowerValue0;	/* ´¸½£°ß°ª */
	int		PowerValue1;	/* ´¸½£°ß°ª */

	/* ±Ëº¤½æ´ª°ªËË ½ó±¨°ªËºµá¹¢À¾ »¤»õÂ×ÊÞ 1 ¸ó³Þ ¿Ð ´´Ê¯ */
	while ( ScaleValue>0x0001 )
	{
		PowerValue0 = (RootValue | ScaleValue)*(RootValue | ScaleValue);	/* Âï¼õ ´¸½£°ßÇµ¶®°ª±á ±Ëº¤½æ´ª¶¦ ³óÂÙ °ªÌ© ´¸½£°ßË¾ ±Ëº¤Â×°Ö */
		/* °û °ªËË ËÓ°é°ª±á °¯Ëº·² */
		if ( PowerValue0==PowerValue )
		{
			RootValue |= ScaleValue;	/* Âï¼õ ´¸½£°ßÇµ¶®°ªËæ ±Ëº¤½æ´ª°ªË¾ ³óÂÙ Ã¨ */
			break;	/* whileºåÃûËæº· À³Á¬ */
		} /* if */
		/* °û °ªËË ËÓ°é°ª¸ó³Þ »õËº·² */
		else if ( PowerValue0<PowerValue )
			RootValue |= ScaleValue;	/* Âï¼õ ´¸½£°ßÇµ¶®°ªËæ ±Ëº¤½æ´ª°ªË¾ ³óÂÙ³Þ. */
		ScaleValue >>= 1;	/* ±Ëº¤½æ´ª°ªË¾ ±½»¦ */
	} /* while */
	PowerValue0 = RootValue*RootValue;	/* Âï¼õ ´¸½£°ßÇµ¶®°ªÌ© ´¸½£°ß±á */
	PowerValue1 = (RootValue | ScaleValue)*(RootValue | ScaleValue);	/* Âï¼õ ´¸½£°ßÇµ¶®°ª±á ±Ëº¤½æ´ª°ªË¾ ³óÂÙ °ªÌ© ´¸½£°ßË¾ ¹¾°äÂ×ÊÞ */
	/* Ë§Ì© ´¸ °ª¼Ú ËÓ°é°ªËæ °¡Ä´Ë© */
	if ( (PowerValue-PowerValue0)>(PowerValue1-PowerValue) )
		RootValue |= ScaleValue;	/* ´¸½£°ßÇµ¶®¶¦ ½Ü°Ö */

	return RootValue;	/* ±Ëº¤´ô ´¸½£°ßÇµ¶®°ªË¾ ±ÙÃû */

}

}