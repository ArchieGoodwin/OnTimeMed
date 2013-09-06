
/*!
 * \file	Random.h
 * \ingroup base
 * \brief	˧��㻶��˾ �����ײ� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "Object.h"

namespace CVLib
{
/**
 @brief   ˧��� ���¹�.
 */
class CVLIB_DECLSPEC Random : public Object
{
public:
	// Constructor and Destructor
	//! �龢��
	Random();
	//! �;˻�
	virtual ~Random();

	// Initialization
	/**
	 * @brief	˧��㻶�±��� �����ٳ�.
	 *
	 * @return	void
	 *
	 * @see  Uniform()
	 *
	 */
	static void Init ();

	// Operations for generating random number according to different distribution.
	/**
	 * @brief   ��й����� �ඣ�� ˧��㶦 �����ٳ�. �״ݰ�˼ RAND_MAX
	 *
	 * @param	fInit [in] : �󱨺�������
	 * @return	���´� ˧���
	 *
	 * @see  Init()
	 *
	 * @par Example:
	 * @code
	 *
	 *	int i;
	 *
	 *	// Display 10 numbers.
	 *	for( i = 0; i < 10; i++ )
	 *		printf( "  %6d\n", Uniform() );
	 *
	 *	Output
	 *
	 *		6929
	 *		8026
	 *		21987
	 *		30734
	 *		20587
	 *		6699
	 *		22034
	 *		25051
	 *		7988
	 *		10104
	 *
	 * @endcode
	 *
	 */
	static int Uniform (bool fInit = true);

	/**
	 * @brief   ��й����� �ඣ�� ˧��㶦 �����ٳ�.
	 *
	 * @param   rMin  [in] : ˧���̩ �׺ϰ�
	 * @param   rMax  [in] : ˧���̩ �״ݰ�
	 * @param   fInit [in] : �󱨺�������
	 * @return	���´� ˧���
	 *
	 * @see  Uniform()
	 *
	 */
	static float BoundedUniform (float rMin = 0.0001f, float rMax = 1.0f, bool fInit = true);

	/**
	 * @brief   ���������� �ඣ�� ˧��㶦 �����ٳ�.
	 *
	 * @param   rMean [in] : �����
	 * @param	rStd  [in] : ����ݽ�
	 * @return	���´� ˧���
	 *
	 * @par Example:
	 * @code
	 *
	 *	int i;
	 *  int nNum = 10; 
	 *	float rSigma = 0.5f;
	 *	float rArray1[5];
	 *	float rArray2[3];
	 *
	 *	for (i = 0; i < 5; i ++)
	 *  {
	 *		 rArray1[i] = rSigma * Random::Normal() + 0.3f;
	 *		 printf( "%.2f\n", rArray1[i]);
	 *	}
	 *	for (i = 0; i < 3; i++)
	 *	{
	 *		 rArray2[i] = Random::Normal();
	 *		 printf( "%.1f\n", rArray2[i]);
	 *	}
	 *	...
	 *
	 *	output
	 *    0.36
	 *    0.97
	 *    -0.07
	 *    -0.87
	 *    0.15
	 *    -0.9
	 *    -0.2
	 *    -0.2
	 *
	 * @endcode
	 *
	 */
	static float Normal(float rMean = 0.0f, float rStd = 1.0f);

	/** 
	 * @brief   ��㹤���� �ඣ�� ˧��㶦 �����ٳ�.
	 *
	 * @param   rLambda	[in] : �ĵ�����
	 * @return  ���´� ˧���
	 *
	 * @see  BoundedUniform()
	 *
	 */
	static float Exponential(float rLambda);

	/**
	 * @brief   �ջչ����� �ඣ�� ˧��㶦 �����ٳ�.
	 *
	 * @param	rMedian	[in] : ��ʺ��
	 * @param	rSigma	[in] : ����ݽ�
	 * @return	���´� ˧��� 
	 *
	 * @see  BoundedUniform()
	 *
	 */
	static float Cauchy(float rMedian, float rSigma);

	/**
	 * @brief   ������������� �ඣ�� ˧��㶦 �����ٳ�.
	 *
	 * @param   rMean [in] : �����
	 * @param	rStdv [in] : ����ݽ�
	 * @return	���´� ˧���
	 *
	 * @see  Normal()
	 *
	 */
	static float LogNormal(float rMean, float rStdv);

	/**
	 * @brief   �����ؼ������� �ඣ�� ˧��㶦 �����ٳ�.
		  �����㶦 �󴮶���. p(i) = (1-rP) * rP^(i-1)$.
	 *
	 * @param	rP [in] : rP �� 0 < p < 1˾ �������ʿ �ٳ�.
	 * @return	���´� ˧���
	 *
	 * @see  BoundedUniform()
	 *
	 */
	static int Geometric(float rP);
};

}

#endif //_RANDOM_H_
