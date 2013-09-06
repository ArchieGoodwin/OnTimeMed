/*!
 * \file	ipCoreABC.cpp
 * \ingroup cvlibip
 * \brief   cvlibipº·°ÖÌ© Ãùº¬½å¶®¶¦ Ì¡ÂÙ ±¨½ó¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */

#include "ipCoreABC.h"
#include "cvlibbase/inc/MatOperation.h"

namespace CVLib
{

bool ipCoreABC::Process (CoImage* pSrc, CoImage* pDst/* = NULL*/)
{
#define channel1(p) p ? &p->m_matX : NULL
#define channel2(p) p ? &p->m_matY : NULL
#define channel3(p) p ? &p->m_matZ : NULL

	bool fSuccess = false;
	fSuccess = Process(&pSrc->m_matX, channel1(pDst));
	if (fSuccess)
		fSuccess = Process(&pSrc->m_matY, channel2(pDst));
	if (fSuccess)
		fSuccess = Process(&pSrc->m_matZ, channel3(pDst));
	return fSuccess;
}

bool ipBaseCore::EqualType(CoImage* p1, CoImage* p2)
{
	if (p1 == NULL || p2 == NULL)
		return true;
	if (p1->GetType() == p2->GetType())
		return true;
	return false;
}

bool ipBaseCore::EqualType(Mat* p1, Mat* p2)
{
	if (p1 == NULL || p2 == NULL)
		return true;
	if (p1->Type() == p2->Type())
		return true;
	return false;
}

bool ipBaseCore::EqualTypeSize(CoImage* p1, CoImage* p2)
{
	if (p1 == NULL || p2 == NULL)
		return true;
	if (p1->GetType() == p2->GetType() && 
		p1->GetHeight() == p2->GetHeight() &&
		p1->GetWidth() == p2->GetWidth() )
		return true;
	return false;
}

bool ipBaseCore::EqualTypeSize(Mat* p1, Mat* p2)
{
	if (p1 == NULL || p2 == NULL)
		return true;
	if (p1->Type() == p2->Type() && 
		p1->Rows() == p2->Rows() &&
		p1->Cols() == p2->Cols() )
		return true;
	return false;
}

bool ipBaseCore::Lut(BYTE* pbLut, Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	switch (pmSrc->Type())
	{
	case MAT_Tbyte:
		{
			BYTE* pbSrc = pmSrc->data.ptr[0];
			BYTE* pbDst = pmDst ? pmDst->data.ptr[0]: pbSrc;
			int cn=pmSrc->Channels();
			int size=pmSrc->Rows() * pmSrc->Cols()*cn;
			for(int i=0; i < size; i++)
				pbDst[i] = pbLut[pbSrc[i]];
		}
		break;
	case MAT_Tfloat:
		{
			float* prSrc = pmSrc->data.fl[0];
			float* prDst = pmDst ? pmDst->data.fl[0]: prSrc;
			int cn=pmSrc->Channels();
			int size=pmSrc->Rows() * pmSrc->Cols()*cn;
			for(int i=0; i < size; i++)
				prDst[i] = pbLut[(BYTE)prSrc[i]];
		}
		break;
    default:
            assert(false);
	}

	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
IMPLEMENT_PUMP(ipCorePump, PumpABC)
PumpRet ipCorePump::PumpProcess()
{
 	CoImage* pSrc=(CoImage*)GetData();
 	CoImage* pDst=new CoImage;
	pDst->Create (*pSrc);
 	Process(pSrc, pDst);
 	RegDataEx(1, pDst, "image");
	return PR_AFTER;
}

void ipCorePump::releaseData()
{
	if (GetPin(1)->Data())
	{
		delete ((CoImage*)GetPin(1)->Data());
		GetPin(1)->SetData(NULL);
	}
}
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_PUMP(ipCoreFeaturePump, PumpABC)

/************************************************************************/
/*                                                                      */
/************************************************************************/
IMPLEMENT_PUMP(ipCompoundCorePump, CompoundPump)

ipCompoundCorePump::ipCompoundCorePump()
{
// 	m_numInOut=2; m_linktypes[0]=LT_INPUT; m_linktypes[1]=LT_OUTPUT;
// 	memset (m_pumpDatas, 0, sizeof(m_pumpDatas[0])*MAX_LINKS);
}

ipCompoundCorePump::~ipCompoundCorePump()
{
	Release(); 
//	PumpRelease();
}

ipCompoundCorePump& ipCompoundCorePump::Add (ipCorePump* pCore)
{
	CompoundPump::Add(pCore);
	if (CompoundPump::m_Pumps.GetSize()>=2)
	{
		PumpABC* pPrev=(PumpABC*)CompoundPump::m_Pumps[CompoundPump::m_Pumps.GetSize()-2];
		pCore->Connect(0, pPrev, 1);
	}
	BuildOrder();
	
	return *this;
}

ipCompoundCorePump& ipCompoundCorePump::DetachAt (int at)
{
	PumpABC* pPump=GetAt(at);
	int nCount = GetPumpCount();
	if (at >= nCount)
		return *this;

	CompoundPump::DetachAt(at);
	if (at == nCount-1 || nCount == 1)
	{
		BuildOrder();
		return *this;
	}

	pPump = GetAt(at);
	if (at == 0)
	{
		pPump->Disconnect(0);
	}
	else
	{
		PumpABC* pPrev=GetAt(at-1);
		pPump->Connect(0, pPrev, 1);
	}
	BuildOrder();
	
	return *this;
}

bool ipCompoundCorePump::Process (Mat* pmSrc, Mat* pmDst /*= NULL*/)
{
	if (m_Pumps.GetSize() == 0)
		return false;

	ipCorePump* pCore=NULL;

	if (pmDst)
	{
		Mat mTemp(pmSrc->Rows(), pmSrc->Cols(), pmSrc->Type());
		Mat* pmin=pmSrc;
		Mat* pmout=&mTemp;

		pCore=(ipCorePump*)m_Pumps[0];
		pCore->Process(pmin, pmout);
		for (int i=1; i<m_Pumps.GetSize(); i++)
		{
			pmin = pmout;
			pmout = (pmin==pmDst) ? &mTemp: pmDst;
			pCore = (ipCorePump*)m_Pumps[i];
			pCore->Process(pmin, pmout);
		}
		if (pmDst != pmout)
			MatOp::CopyMat (pmDst, pmout, pmDst->Type());
	}
	else
	{
		Mat mTemp(pmSrc->Rows(), pmSrc->Cols(), pmSrc->Type());
		Mat* pmin=pmSrc;
		Mat* pmout=&mTemp;
		for (int i=0; i<m_Pumps.GetSize(); i++)
		{
			pCore = (ipCorePump*)m_Pumps[i];
			pCore->Process(pmin, pmout);
			pmin = pmout;
			pmout = (pmin==pmSrc) ? &mTemp: pmSrc;
		}
		if (pmSrc != pmout)
			MatOp::CopyMat (pmSrc, pmout, pmSrc->Type());
	}
	return true;
}

bool ipCompoundCorePump::Process (CoImage* pSrc, CoImage* pDst /*= NULL*/)
{
	if (m_Pumps.GetSize() == 0)
		return false;

	ipCorePump* pCore=NULL;
	if (pDst)
	{
		CoImage mTemp;
		CoImage* pmin=pSrc;
		CoImage* pmout=&mTemp;
		pCore=(ipCorePump*)m_Pumps[0];
		if (!pCore->Process(pmin, pmout))
			return false;
		for (int i=1; i<m_Pumps.GetSize(); i++)
		{
			pmin = pmout;
			pmout = (pmin==pDst) ? &mTemp: pDst;
			pCore = (ipCorePump*)m_Pumps[i];
			if (!pCore->Process(pmin, pmout))
				return false;
		}
		if (pDst != pmout)
		{
			pDst->Release();
			pDst->Create (*pmout, TRUE);
		}
	}
	else
	{
		CoImage mTemp;
		mTemp.Create(*pSrc);
		CoImage* pmin=pSrc;
		CoImage* pmout=&mTemp;
		for (int i=0; i<m_Pumps.GetSize(); i++)
		{
			pCore = (ipCorePump*)m_Pumps[i];
			if (!pCore->Process(pmin, pmout))
				return false;
			pmin = pmout;
			pmout = (pmin==pSrc) ? &mTemp: pSrc;
		}
		if (pSrc != pmout)
		{
			pSrc->Release();
			pSrc->Create (*pmout, TRUE);
		}
	}
	return true;
}

void ipCompoundCorePump::releaseData()
{
//	CompoundPump::releaseData();
}

void ipCompoundCorePump::Release ()
{
}

PumpRet ipCompoundCorePump::PumpProcess()
{
	CoImage* pImage=(CoImage*)GetData();
	CoImage* pDst= new CoImage;
	bool fSucess = Process(pImage, pDst);
	RegDataEx(1, pDst, "image");
	if (fSucess)
		return PR_AFTER;
	else
		return PR_ABORT;
}

}
