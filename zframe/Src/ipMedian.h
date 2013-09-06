
/*!
 * \file	ipMedian.h
 * \ingroup ipCore
 * \brief   MedianµØÁÄ¶¦ ¼êÃÔÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#ifndef _ipMedian_H_
#define _ipMedian_H_

#include "ipCoreABC.h"

namespace CVLib
{

class CVLIB_DECLSPEC ipMedian : public ipCorePump
{
	DECLARE_PUMP(ipMedian)
public:
	enum scantype{Row, Col};
	ipMedian(const ipMedian& from) {m_nKsize=from.m_nKsize;}
	ipMedian(int nKsize = 3): m_nKsize(nKsize){}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);

	bool Process (Mat* pmSrc, scantype scantype, Mat* pmDst=NULL);
	bool Process (Vec* pvSrc, Vec* pvDst=NULL);
public:
	int m_nKsize;
};

namespace ip
{
CVLIB_DECLSPEC void Median(const Mat& src, Mat& dst, int nKsize = 3);
}

}

#endif //_ipMedian_H_