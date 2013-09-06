/*!
 * \file    MathLib.cpp
 * \ingroup math
 * \brief   �Եۺ��ֶ� ��̩�ٳ�.
 * \author  ����˧
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
	/* ������ ���� ��˧ */
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
	/* ������ ���� ��˧ */
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

		int	n = nValue >> (30 - LOG_SCALE_BITS);	/* �����̩ �������� */
		nValue -= (n << (30 - LOG_SCALE_BITS));		/* ������溷 ������� (Q30) */
		int64	nTempValue64 = MultiplyProcess((pnLogTable[n + 1] - pnLogTable[n]), nValue);	/* Q(46 - LOG_SCALE_BITS) */
		nValue = (int)(nTempValue64 >> (30 - LOG_SCALE_BITS));	/* Q16 */
		nLogValue = (nLogValue << 16) + pnLogTable[n] + nValue;
	}

	return nLogValue;	/* 32bit ������̩ 2̩ ����� ���� */
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
	/* �������� */
	if ((nPhase % nTwicePI) == 0)
		return 0;
	else if (nPhase > 0)
		nSign = 1;
	else
	{
		nPhase = -nPhase;
		nSign = -1;
	}
	/* �ӵ�̡����˾ 0���� (PI / 2)����̩ ��˺�� ������ */
	while (nPhase > nTwicePI)
		nPhase -= nTwicePI;
	if (nPhase > PI_Q15)
	{
		nSign = -nSign;
		nPhase -= PI_Q15;
	}
	if (nPhase > nHalfPI)
		nPhase = PI_Q15 - nPhase;
	/* ��̩ ������ ���� */
	int	nStartPoint;
	int	point = DivisionProcess((nPhase << 9), PI_Q15, &nStartPoint);
	nStartPoint >>= point;
	if (nStartPoint >= TRIANGLE_SCALE_SIZE)
		return (nSign << 15);
	/* �커������̩ �ݽ� */
	int	nViasValue = nPhase - (nStartPoint * PI_Q15 >> 9);	/* Q15 */
	/* �커������̩ �ܺ㰪 �ݽ� */
	int	nErrorValue;
	point = DivisionProcess(((pnSineTable[nStartPoint + 1] - pnSineTable[nStartPoint]) * nViasValue), PI_Q15, &nErrorValue);
	if (point > 9)
		nErrorValue = RightShift(nErrorValue, (point - 9));
	else
		nErrorValue <<= (9 - point);

	return (nSign * (pnSineTable[nStartPoint] + nErrorValue));	/* sin�� ���� */
}

int	ArcsineProcess(int nPhase, int* pnResult)
{
	/* �ӵٰ��� arcsin�ܺ�̩ Ʒ����˾ ���̱��� <��²> */
	if ((nPhase < (-1 << 15)) || (nPhase > (1 << 15)))
		return -1;
	/* �ӵٰ��� ���˷� �߰� ���� */
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
	/* ��̩ ������ ���� */
	int	nStartPoint = nPhase >> (15 - TRIANGLE_SCALE_BITS);
	if (nStartPoint >= TRIANGLE_SCALE_SIZE)
	{
		*pnResult = (PI_Q15 >> 1);
		return 0;
	}
	/* �커������̩ �ݽ� */
	int	nViasValue = nPhase - (nStartPoint << (15 - TRIANGLE_SCALE_BITS));	/* Q15 */
	/* �커������̩ �ܺ㰪 �ݽ� */
	int	nErrorValue = RightShift((pnArcsineTable[nStartPoint + 1] - pnArcsineTable[nStartPoint]) * nViasValue, 7);	/* Q15 */
	/* arcsin�� �˺� */
	*pnResult = nSign * (pnArcsineTable[nStartPoint] + nErrorValue);

	return 0;
}

int	CosineProcess(int nPhase)
{
	return SineProcess((PI_Q15 >> 1) - nPhase);
}

int	ArccosineProcess(int nPhase, int* pnResult)
{
	/* �ӵٰ��� arccos�ܺ�̩ Ʒ����˾ ���̱��� <��²> */
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
	/* �ӵ�̡��̩ ������ */
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
	/* �ӵٰ��� tan�ܺ� Ʒ����̩ ˱ã��̡��  ���̱��� <��²> */
	if (nPhase > pnTangentInputTable[TRIANGLE_SCALE_SIZE])
		return -1;
	/* ��̩ ������ ���� */
	int	nStartPoint = 1;
	while (pnTangentInputTable[nStartPoint] <= nPhase)
	{
		if (++nStartPoint > TRIANGLE_SCALE_SIZE)
			break;
	}
	/* �ӵٰ��� tan�ܺ� Ʒ����̩ ˱ã��̡̩ �ٱ˰��� ��˧ */
	if (--nStartPoint >= TRIANGLE_SCALE_SIZE)
	{
		*pnResult = nSign * pnTangentOutputTable[nStartPoint];
		return 0;
	}
	/* �커������̩ �ݽ� */
	int	nViasValue = nPhase - pnTangentInputTable[nStartPoint];	/* Q15 */
	/* �커������̩ �ܺ㰪 �ݽ� */
	int	nErrorValue;
	int	point = DivisionProcess(((pnTangentOutputTable[nStartPoint + 1] - pnTangentOutputTable[nStartPoint]) * nViasValue), (pnTangentInputTable[nStartPoint + 1] - pnTangentInputTable[nStartPoint]), &nErrorValue);
	nErrorValue = RightShift(nErrorValue, point);
	/* tan�� �˺� */
	*pnResult = nSign * (pnTangentOutputTable[nStartPoint] + nErrorValue);

	return 0;
}

int	ArctangentProcess(int nPhase, int* pnResult)
{
	/* �ӵٰ��� arctan�ܺ� Ʒ����̩ ˱ã��̡��  ���̱��� <��²> */
	if ((nPhase < (-16 << 11)) || (nPhase > (16 << 11)))
		return -1;
	/* �ӵٰ��� ���˷� �߰� ���� */
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
	/* ��̩ ������ ���� */
	int	nStartPoint = 1;
	while (pnTangentOutputTable[nStartPoint] <= nPhase)
	{
		if (++nStartPoint > TRIANGLE_SCALE_SIZE)
			break;
	}
	/* �ӵٰ��� arctan�ܺ� Ʒ����̩ ˱ã��̡̩ �ٱ˰��� ��˧ */
	if (--nStartPoint >= TRIANGLE_SCALE_SIZE)
	{
		*pnResult = nSign * pnTangentInputTable[nStartPoint];
		return 0;
	}
	/* �커������̩ �ݽ� */
	int	nViasValue = nPhase - pnTangentOutputTable[nStartPoint];	/* Q11 */
	/* �커������̩ �ܺ㰪 �ݽ� */
	int	nErrorValue;
	int	point = DivisionProcess(((pnTangentInputTable[nStartPoint + 1] - pnTangentInputTable[nStartPoint]) * nViasValue), (pnTangentOutputTable[nStartPoint + 1] - pnTangentOutputTable[nStartPoint]), &nErrorValue);
	nErrorValue = RightShift(nErrorValue, point);
	/* tan�� �˺� */
	*pnResult = nSign * (pnTangentInputTable[nStartPoint] + nErrorValue);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/** 
 * @brief  ������ �ֺ�ʭ�����ĺ��˺� 2�ӳ��߻�̩ 2�Ӷ�ѰӺ�	
 */
#define _MC_ATAN_CF0  (-15.8131890796f)
/** 
 * @brief  ������ �ֺ�ʭ�����ĺ��˺� 2�ӳ��߻�̩ 1�Ӷ�ѰӺ�	
 */
#define _MC_ATAN_CF1  (61.0941945596f)
/** 
 * @brief  ������ �ֺ�ʭ�����ĺ��˺� 2�ӳ��߻�̩ 0�Ӷ�ѰӺ�	
 */
#define _MC_ATAN_CF2  0.f /*(-0.140500406322f)*/

/* ������ �ֺ�ʭ�����ĺ��˺��溷 ���������� */
static const unsigned int mcAtanSign[8] =
{ 0, (unsigned int)0x80000000, (unsigned int)0x80000000, 0, (unsigned int)0x80000000, 0, 0, (unsigned int)0x80000000 };

/** 
 * @brief  �ּ��຤�溷 ����뱨����	
 */
#define	 BIT_SHIFT	10
/** 
 * @brief  ������ �ֺ�ʭ�����ĺ��˺� 2�ӳ��߻�̩ 2�Ӷ�ѰӺ�	
 */
#define _MC_INI_ATAN_CF0	(-16193)
/** 
 * @brief  ������ �ֺ�ʭ�����ĺ��˺� 2�ӳ��߻�̩ 1�Ӷ�ѰӺ�	
 */
#define _MC_INI_ATAN_CF1	(62560)	
/** 
 * @brief  ������ �ֺ�ʭ�����ĺ��˺� 2�ӳ��߻�̩ 0�Ӷ�ѰӺ�	
 */
#define _MC_INI_ATAN_CF2	(-144)

/*	������ �ֺ�ʭ�����ĺ��˺��溷 10����뱨�� ��̩ ������� */
static const int mcIntAtanTab[8] = { 0 + _MC_INI_ATAN_CF2,    92160 - _MC_INI_ATAN_CF2,
									184320 - _MC_INI_ATAN_CF2,  92160 + _MC_INI_ATAN_CF2,
									368640 - _MC_INI_ATAN_CF2, 276480 + _MC_INI_ATAN_CF2,
									184320 + _MC_INI_ATAN_CF2, 276480 - _MC_INI_ATAN_CF2
};

/* ������ �ֺ�ʭ�����ĺ��˺��溷 ���������� */
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
    int idx = (ix < 0) * 2 + (iy < 0) * 4; /* idx̩ 1,2������� ix,iy̩ ������ ���� */
    int ygx;

	/*���ݰ��б�*/
	ix = Abs(x);
	iy = Abs(y);
	
    /* idx̩ 0������� ix,iy̩ ���ݰ��ͱ����̮ 0��˶� ���� */
	ygx = (iy <= ix) - 1;
    idx -= ygx;	 
    idx &= ((ix == 0) - 1) | ((iy == 0) - 1);/* ix != 0�˰� iy != 0 �˷� idx����˱��, 
												ʭ���� idx̩ 0���Ⱚ˾ 1�� ���� */

    /* swap ix and iy if ix < iy */
    ix ^= iy & ygx;
    iy ^= ix & ygx;
    ix ^= iy & ygx;
    iy ^= mcIntAtanSign[idx];

	iy <<= BIT_SHIFT;
	ix <<= BIT_SHIFT;
    ix |= 1;	// 0���䱨�� ª�ױ� ̡����
    {
        int64 z = iy;
        z <<= BIT_SHIFT;
        z <<= BIT_SHIFT;/* ��������䱨�� ̩�� 0�˺�˾ ª�ױ� ̡���� */
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
 * @brief Cos�ܺ�̩ ��˾ 1�� ��̡�� �������� �� ��
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
 * @brief Atan�ܺ�̩ ��˾ 1�� ��̡�� �������� �� ��
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
 * @brief �鰣���� �Ӱ��� dwVal�� �����ײ����� �����ٳ�.
 *
 * @param	dwVal		[in]: �Ӱ鰪
 * @param	iStart		[in]: �鰣̩ ����̩ Index
 * @param	iEnd		[in]: �鰣̩ ��̩ Index
 * @param	pnIndex		[out]: �Ӱ鰪̩ ����̡��
 * @return	����
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
	int		RootValue = 0x0000;		/* 2����ǵ���αᰪ �� ���� */
	int		ScaleValue = 0x4000;	/* �˺��洪�� ���� */
	int		PowerValue0;	/* �����߰� */
	int		PowerValue1;	/* �����߰� */

	/* �˺��洪���� �󱨰�˺�Ṣ�� �������� 1 ��� �� ��ʯ */
	while ( ScaleValue>0x0001 )
	{
		PowerValue0 = (RootValue | ScaleValue)*(RootValue | ScaleValue);	/* ��� ������ǵ������ �˺��洪�� ���� ��̩ ������˾ �˺��װ� */
		/* �� ���� �Ӱ鰪�� ��˺�� */
		if ( PowerValue0==PowerValue )
		{
			RootValue |= ScaleValue;	/* ��� ������ǵ������ �˺��洪��˾ ���� è */
			break;	/* while�����溷 ���� */
		} /* if */
		/* �� ���� �Ӱ鰪��� ��˺�� */
		else if ( PowerValue0<PowerValue )
			RootValue |= ScaleValue;	/* ��� ������ǵ������ �˺��洪��˾ ���ٳ�. */
		ScaleValue >>= 1;	/* �˺��洪��˾ ���� */
	} /* while */
	PowerValue0 = RootValue*RootValue;	/* ��� ������ǵ����̩ �����߱� */
	PowerValue1 = (RootValue | ScaleValue)*(RootValue | ScaleValue);	/* ��� ������ǵ������ �˺��洪��˾ ���� ��̩ ������˾ �������� */
	/* ˧̩ �� ���� �Ӱ鰪�� ��Ĵ˩ */
	if ( (PowerValue-PowerValue0)>(PowerValue1-PowerValue) )
		RootValue |= ScaleValue;	/* ������ǵ���� �ܰ� */

	return RootValue;	/* �˺��� ������ǵ����˾ ���� */

}

}