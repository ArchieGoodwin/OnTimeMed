
/*!
 * \file	ipNoise.h
 * \ingroup ipCore
 * \brief   »üËÁË¾ ¾¡°¡Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#ifndef _ipMorphology_H_
#define _ipMorphology_H_

#include "ipCoreABC.h"

namespace CVLib
{

class CVLIB_DECLSPEC ipMorphology : public ipCorePump
{
	DECLARE_PUMP(ipMorphology)

// 	DECLARE_PARAM(radius)
// 	DECLARE_PARAMRANGE
public:
	ipMorphology(int nKsize=2): m_nKsize(nKsize){/*IMPLEMENT_PARAM(radius, m_nKsize)*/}
public:
	int m_nKsize;
};

class CVLIB_DECLSPEC ipErode : public ipMorphology
{
	DECLARE_PUMP(ipErode)
public:
	ipErode(int nKsize=2){m_nKsize=nKsize;}
	ipErode(const ipErode& from) {m_nKsize=from.m_nKsize;}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
};

class CVLIB_DECLSPEC ipDilate : public ipMorphology
{
	DECLARE_PUMP(ipDilate)
public:
	ipDilate(const ipDilate& from) {m_nKsize=from.m_nKsize;}
	ipDilate(int nKsize=2){m_nKsize = nKsize;}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
};

class CVLIB_DECLSPEC ipOpening : public ipMorphology
{
	DECLARE_PUMP(ipOpening)
public:
	ipOpening(const ipOpening& from) {m_nKsize=from.m_nKsize;}
	ipOpening(int nKsize=2){m_nKsize = nKsize;}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
};

class CVLIB_DECLSPEC ipClosing : public ipMorphology
{
	DECLARE_PUMP(ipClosing)
public:
	ipClosing(const ipClosing& from) {m_nKsize=from.m_nKsize;}
	ipClosing(int nKsize=2){m_nKsize = nKsize;}
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
};

class CVLIB_DECLSPEC Morphology : public Object
{
public:
	Morphology();
	~Morphology();

	void	Opening (Mat* pImg, Mat* pFilter);
	void	Closing (Mat* pImg, Mat* pFilter);
	void	Erode (Mat* pOrg, Mat* pDest, Mat* pFilter);
	void	Dilate (Mat* pOrg, Mat* pDest, Mat* pFilter);
private:
	int		MinMaxLimiter ( int nValue, int nMax, int nMin );
};

namespace ip
{
CVLIB_DECLSPEC void Erode(const Mat& src, Mat& dst, int nksize=2);
CVLIB_DECLSPEC void Dilate(const Mat& src, Mat& dst, int nksize=2);
CVLIB_DECLSPEC void Opening(const Mat& src, Mat& dst, int nksize=2);
CVLIB_DECLSPEC void Closing(const Mat& src, Mat& dst, int nksize=2);
}

}

#endif //_ipMorphology_H_