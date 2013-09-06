/*!
 * \file	Timer.h
 * \ingroup base
 * \brief	����˾ �����ױ� ̡�� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>
#include "Object.h"

namespace CVLib 
{

/**
 * @brief  ����˾ �����ױ� ̡�� �е���
 */
class CVLIB_DECLSPEC Timer : public Object
{
private:
#ifdef _MSC_VER
#if CVLIB_OS == CVLIB_OS_WIN32
	__int64 m_Frequency;
	__int64 m_StartTime;
	__int64 m_StopTime;
#endif
	
	//! ���󻤰�
	static time_t base_time;
#endif

	//! ���Լ��커����
	bool is_running;
	//! ��������
	double start_time;
	//! ���� ����
	double total_time;
public:

	// Construction and Destruction
	//! �龢��
	Timer();

	//! �;˻�
	virtual ~Timer();

	/**
	 * @brief   ���˶� �������ٳ�.
	 <pre> ������ ���������� ����˾ �˺��ٳ�. �񼬻���˼ �����ٳ�. </pre>
	 *
	 * @return	void
	 *
	 * @see  AscCurrentTime()
	 *
	 */
	void Reset();

	/**
	 * @brief   ���˶� �����ٰ� �񼬴� ����˾ �����ٳ�.
	 *
	 * @return	void
	 *
	 * @see  AscCurrentTime()
	 *
	 */
	void Stop();

	/**
	 * @brief   ���˶� �����ٳ�. �˺㶦 �����ٳ�.
	 *
	 * @return	void
	 *
	 * @see  AscCurrentTime(), reset()
	 *
	 */
	void Resume();

	/**
	 * @brief   ���� �񼬴� ����˾ �в���.
	 *
	 * @return	�񼬻���
	 *
	 * @see  AscCurrentTime(), stop()
	 *
	 */
	double GetElapsedTime();

	/**
	 * @brief   �ͻ�������˺�� ����˱˻���˾ �в���.
	 *
	 * @return  �ͻ���
	 *
	 * @par Example:
	 * @code
	 *	
	 *  Timer timer;
	 *	timer.Reset();
	 *	timer.Resume();
	 *	
	 *	printf("���̩ ����̮ ����˼ %s �˳�.\n", timer.AscCurrentTime());
	 *	
	 *	double rSum = 0.0f;
	 *	for(int i = 0; i < 1000000; i ++)
	 *		rSum += i;
	 *	
	 *	double rTime = timer.GetRunTime();
	 *	printf( "���Ի���˼ %.6f �˳�.\n", rTime);
	 *	timer.Stop();
	 *	rTime = timer.GetElapsedTime();
	 *	printf("���˵񼬻���˼ %.2f �˳�.\n", rTime);
	 *	
	 *	Output
	 *		���̩ ����̮ ����˼ Tue Mar 09 19:35:03 2010
	 *      �˳�.
	 *      ���Ի���˼ 7.0000000 ���˳�.
	 *      ���˵񼬻���˼ 16.00 ���˳�.
	 *
	 * @endcode
	 *
	 */
	char* AscCurrentTime ();

	/**
	 * @brief   �˱�̩���ܺ�ặ ���Ի���˾ ���ؼӳ�.
	 *
	 * @return  ���Ի���
	 *
	 * @see	 Reset(), Stop(), Resume(), GetElapsedTime()
	 *
	 */
	static double GetRunTime();

	void Sleep(int nMilliseconds);

};

}

#endif //_TIMER_H_

