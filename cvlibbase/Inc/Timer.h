/*!
 * \file	Timer.h
 * \ingroup base
 * \brief	»¤°£Ë¾ ¾°¼³Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>
#include "Object.h"

namespace CVLib 
{

/**
 * @brief  »¤°£Ë¾ ¾°¼³Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷
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
	
	//! ±¨½ó»¤°£
	static time_t base_time;
#endif

	//! »¨ÃÔ¼ÚÁì»¤±¨¸Ë
	bool is_running;
	//! »¤»õ»¤°£
	double start_time;
	//! ¼­¾Ë »¤°£
	double total_time;
public:

	// Construction and Destruction
	//! °é¾¢»ô
	Timer();

	//! ÃÍ¾Ë»ô
	virtual ~Timer();

	/**
	 * @brief   »¤±Ë¶¦ ¼õº¼¼³ÂÙ³Þ.
	 <pre> ¼õº¼¼³ ´ô³ÞËÁ¹¢À¾ »¤°£Ë¾ ±ËºãÂÙ³Þ. µñ¼¬»¤°£Ë¼ º¢½£ÂÙ³Þ. </pre>
	 *
	 * @return	void
	 *
	 * @see  AscCurrentTime()
	 *
	 */
	void Reset();

	/**
	 * @brief   »¤±Ë¶¦ ¼³¼èÂÙ°Ö µñ¼¬´ô »¤°£Ë¾ ±½»¦ÂÙ³Þ.
	 *
	 * @return	void
	 *
	 * @see  AscCurrentTime()
	 *
	 */
	void Stop();

	/**
	 * @brief   »¤±Ë¶¦ »¤»õÂÙ³Þ. ±Ëºã¶¦ »¤»õÂÙ³Þ.
	 *
	 * @return	void
	 *
	 * @see  AscCurrentTime(), reset()
	 *
	 */
	void Resume();

	/**
	 * @brief   ¼­¾Ë µñ¼¬´ô »¤°£Ë¾ ÊÐ²÷³Þ.
	 *
	 * @return	µñ¼¬»¤°£
	 *
	 * @see  AscCurrentTime(), stop()
	 *
	 */
	double GetElapsedTime();

	/**
	 * @brief   ·Í»ôµÛÂô»¥Ëºµá Âï¼õ¾Ë±Ë»¤°£Ë¾ ÊÐ²÷³Þ.
	 *
	 * @return  ·Í»ôµÛ
	 *
	 * @par Example:
	 * @code
	 *	
	 *  Timer timer;
	 *	timer.Reset();
	 *	timer.Resume();
	 *	
	 *	printf("Âï¼õÌ© ²¤»ôÌ® »¤°£Ë¼ %s ËË³Þ.\n", timer.AscCurrentTime());
	 *	
	 *	double rSum = 0.0f;
	 *	for(int i = 0; i < 1000000; i ++)
	 *		rSum += i;
	 *	
	 *	double rTime = timer.GetRunTime();
	 *	printf( "»¨ÃÔ»¤°£Ë¼ %.6f ËË³Þ.\n", rTime);
	 *	timer.Stop();
	 *	rTime = timer.GetElapsedTime();
	 *	printf("¼­¾Ëµñ¼¬»¤°£Ë¼ %.2f ËË³Þ.\n", rTime);
	 *	
	 *	Output
	 *		Âï¼õÌ© ²¤»ôÌ® »¤°£Ë¼ Tue Mar 09 19:35:03 2010
	 *      ËË³Þ.
	 *      »¨ÃÔ»¤°£Ë¼ 7.0000000 ½óËË³Þ.
	 *      ¼­¾Ëµñ¼¬»¤°£Ë¼ 16.00 ½óËË³Þ.
	 *
	 * @endcode
	 *
	 */
	char* AscCurrentTime ();

	/**
	 * @brief   ¾Ë±ËÌ©¼ÁÂÜºãµáº· »¨ÃÔ»¤°£Ë¾ ´®µØ¼Ó³Þ.
	 *
	 * @return  »¨ÃÔ»¤°£
	 *
	 * @see	 Reset(), Stop(), Resume(), GetElapsedTime()
	 *
	 */
	static double GetRunTime();

	void Sleep(int nMilliseconds);

};

}

#endif //_TIMER_H_

