
/*!
 * \file	Statistics.h
 * \ingroup math
 * \brief	�ұ˵�˾ ���ױ� ̡�� �ܺ��˾ ��̩��.
 * \author	����˧
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "Mat.h"

namespace CVLib 
{

/** 
 * @brief   ���˶���̩ ������˶��� ���ٳ�.
 *
 * @param	pvecExamples [in]  : ���˶���̩ �����Ѻ�
 * @param	nNum		 [in]  : ���˶�����
 * @param	pvecMean	 [out] : ������˶�
 * @param	pvecWeight	 [in]  : ���˶���̩ �˱¶� ������ ���˶� \n
 *				Null�� ��˧ ���˶���̩ �˱¶� �ֵ��׼� ʱ����.
 * @return	void
 *
 * @see	Covariance(), Variation()
 *
 */
void	CVLIB_DECLSPEC MeanVector (Vec* pvecExamples, int nNum, Vec* pvecMean, Vec* pvecWeight = NULL);

/** 
 * @brief	���˶���̩ �Ṥ���Ե�˾ �˺��ٳ�.
 *
 * @param   pvecExamples [in]     : ���˶���̩ �����Ѻ�
 * @param	nNum		 [in]     : ���˶�����
 * @param	pmatCov		 [out]    : �Ṥ���Ե�
 * @param	pvecMean	 [in/out] : ������˶� \n
 *				null�� ��˧ ������˶��˺�˾ �����׷� ʭ�� ��˧ �����׼� ʱ����.
 * @param	pvecWeight	 [in]     : ���˶���̩ �˱¶� ������ ���˶� \n
 *				null�� ��˧ ���˶���̩ �˱¶� �ֵ��׼� ʱ����.
 * @return	void
 *
 * @see	MeanVector(), Variation()
 *
 */
void	CVLIB_DECLSPEC Covariance (Vec* pvecExamples, int nNum, Mat* pmatCov, Vec* pvecMean = NULL, Vec* pvecWeight = NULL);

/** 
 * @brief   ���˶���̩ �괴�Ե�˾ �˺��ٳ�.
 *
 * @param	pvecExamples [in]     : ���˶���̩ �����Ѻ�
 * @param	nNum		 [in]     : ���˶�����
 * @param	pmatVar		 [out]    : �괴�Ե�
 * @param	pvecMean	 [in/out] : ������˶� \n
 *				null�� ��˧ ������˶��˺�˾ �����׷� ʭ�� ��˧ �����׼� ʱ����.
 * @param	pvecWeight	 [in]     : ���˶���̩ �˱¶� ������ ���˶� \n
 *				null�� ��˧ ���˶���̩ �˱¶� �ֵ��׼� ʱ����.
 *
 * @return	void
 *
 * @see	MeanVector(), Covariance()
 *
 */
void	CVLIB_DECLSPEC Variation (Vec* pvecExamples, int nNum, Mat* pmatVar, Vec* pvecMean = NULL, Vec* pvecWeight = NULL);

}

#endif //_STATISTICS_H_
