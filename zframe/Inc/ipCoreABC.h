/*!
 * \file	ipCoreABC.h
 * \ingroup cvlibip
 * \brief   cvlibipº·°ÖÌ© Ãùº¬½å¶®¶¦ Ì¡ÂÙ ±¨½ó¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#ifndef _ipCOREABC_H_
#define _ipCOREABC_H_

#include "cvlibbase/inc/_cvlibbase.h"

namespace CVLib
{

class CVLIB_DECLSPEC ipBaseCore
{
public:
	ipBaseCore() : m_fVerbose(false) {}
	virtual ~ipBaseCore() {}
	bool Lut(BYTE* pbLut, Mat* pmSrc, Mat* pmDst = NULL);
public:
	bool	m_fVerbose;
protected:
	bool	EqualType(CoImage* p1, CoImage* p2);
	bool	EqualType(Mat* p1, Mat* p2);
	bool	EqualTypeSize(CoImage* p1, CoImage* p2);
	bool	EqualTypeSize(Mat* p1, Mat* p2);
};

class CVLIB_DECLSPEC ipCoreABC : public ipBaseCore
{
public:
	ipCoreABC() {}
	virtual ~ipCoreABC() {Release();}

	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL) {return false;}
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
	virtual void Release (){}
};

class CVLIB_DECLSPEC ipCorePump : public ipCoreABC, public PumpABC, public IParamABC
{
	DECLARE_PUMP(ipCorePump)
public:
	ipCorePump() 
	{
		m_numInOut=2; m_linktypes[0]=LT_INPUT; m_linktypes[1]=LT_OUTPUT;
		memset (m_pumpDatas, 0, sizeof(m_pumpDatas[0])*MAX_LINKS);
		ResetLinks();
	}
	virtual ~ipCorePump() {PumpRelease();}
	virtual CoImage* PumpProcess(CoImage* pImage) {Process(pImage); return pImage;}
	virtual PumpRet PumpProcess();
public:
	virtual void releaseData();
};

class CVLIB_DECLSPEC ipCoreFeaturePump : public ipBaseCore, public PumpABC
{
	DECLARE_PUMP(ipCoreFeaturePump)
public:
	ipCoreFeaturePump() {}
	virtual ~ipCoreFeaturePump() {Release();}

	virtual bool Process(CoImage* pSrc) {return false;}
	virtual bool Process(Mat* pmSrc) {return false;}
	virtual bool GetResultImage(CoImage* pSrc) {return false;}
	virtual void Release(){}
	virtual CoImage* PumpProcess(CoImage* pImage) {Process(pImage); GetResultImage(pImage); return pImage;}
};

class CVLIB_DECLSPEC ipCompoundCorePump : protected ipCoreABC, public CompoundPump
{
	DECLARE_PUMP(ipCompoundCorePump)
public:
	ipCompoundCorePump();
	virtual ~ipCompoundCorePump();

	ipCompoundCorePump& Add (ipCorePump* pCore);
	ipCompoundCorePump& DetachAt (int at);

	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
	virtual void Release ();
	
	virtual CoImage* PumpProcess(CoImage* pImage) {Process(pImage); return pImage;}
	virtual PumpRet PumpProcess();
	virtual void releaseData();
};

}


#endif //_ipCOREABC_H_