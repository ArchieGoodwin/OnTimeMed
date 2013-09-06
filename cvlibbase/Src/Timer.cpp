/*!
 * \file	Timer.cpp
 * \ingroup base
 * \brief	»¤°£Ë¾ ¾°¼³Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include "cvlibmacros.h"
#if CVLIB_OS == CVLIB_OS_WIN32
#include <windows.h>
#undef GetRValue
#undef GetGValue
#undef GetBValue
#endif

#include "Timer.h"

#ifndef _MSC_VER
#include <sys/times.h>
#include <unistd.h>
#endif

namespace CVLib 
{

#ifdef _MSC_VER
time_t Timer::base_time = 0;
#endif

Timer::Timer()
{
#ifdef _MSC_VER
	while(!base_time)
		time(&base_time);
#endif

	total_time = 0;
	is_running = false;
	start_time = GetRunTime();
#if CVLIB_OS == CVLIB_OS_WIN32
	m_StartTime=0;
	m_StopTime=0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
#endif
}

Timer::~Timer()
{
}

char* Timer::AscCurrentTime()
{
	struct tm *newtime;
	time_t aclock;

	time( &aclock );                 /* Get time in seconds */

	newtime = localtime( &aclock );  /* Convert time to struct */
	return asctime(newtime);
}

double Timer::GetRunTime()
{
#ifdef _MSC_VER
	time_t truc_foireux;
	time(&truc_foireux);
	return (double)(difftime(truc_foireux, base_time));
#else
	struct tms current;
	times(&current);

	double norm = (double)sysconf(_SC_CLK_TCK);
	return(((double)current.tms_utime)/norm);
#endif
}

void Timer::Reset()
{
#if CVLIB_OS==CVLIB_OS_WIN32
	m_StartTime=0;
	m_StopTime=0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
#endif
	total_time = 0;
	start_time = GetRunTime();
}

void Timer::Stop()
{
	if(!is_running)
		return;

#if CVLIB_OS==CVLIB_OS_WIN32
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StopTime);
	total_time += static_cast<double>(m_StopTime - m_StartTime) / m_Frequency;
#else
	double current_time = GetRunTime() - start_time;
	total_time += current_time;
#endif
	is_running = false;
}

void Timer::Resume()
{
	if(is_running)
		return;

#if CVLIB_OS==CVLIB_OS_WIN32
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
#else
	start_time = GetRunTime();
#endif
	is_running = true;
}

double Timer::GetElapsedTime()
{
	if(is_running)
	{
#if CVLIB_OS==CVLIB_OS_WIN32
		__int64 curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		return total_time + static_cast<double>(curTime-m_StartTime)/m_Frequency;
#else
		double current_time = GetRunTime() - start_time;
		return(total_time+current_time);
#endif
	}
	else
		return total_time;
}

void Timer::Sleep(int nMilliseconds)
{
	Reset();
	double rSecond = (double)nMilliseconds / 1000.0;
	while (1)
	{
		double rNow=GetRunTime();
		double rDelay=rNow-start_time;
		if (rDelay > rSecond)
			break;
	}
}

// double CSProfiler::GetElapsed() const
// {
// 	const double t = static_cast<double>(m_StopTime - m_StartTime) / m_Frequency;
// 	return t;
// }
// #endif

}
