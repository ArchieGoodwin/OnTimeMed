/*!
 * \file	PumpABC.cpp
 * \ingroup base
 * \brief	·º´Ç Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include <assert.h>
#include "PumpABC.h"
#include "Tree.h"

namespace CVLib
{

const char PumpABC::PumpABC_id_[CVLIB_MAX_PUMP_IDLEN] = "PumpABC";
const char* PumpABC::id () { return PumpABC_id_; } 

PumpABC::PumpABC()
{
	m_numInOut=0;
}

void PumpABC::RegDataEx( int l, void* pdata, const SString& type/*=""*/)
{
	assert (m_linktypes[l]==LT_OUTPUT);
	GetPin(l)->SetData(pdata);
	GetPin(l)->SetType(type);
}

void* PumpABC::GetDataEx(int l)
{
	return GetPin(l)->Data();
}

const SString& PumpABC::GetDataTypeEx(int l)
{
	return GetPin(l)->Type();
}

int PumpABC::GetOutputNum()
{
	int nret=0;
	for (int i=0; i<m_numInOut; i++)
	{
		if (m_linktypes[i] == LT_OUTPUT)
			nret++;
	}
	return nret;
}

void PumpABC::ResetLinks()
{
	for (int i=0; i<m_numInOut; i++)
	{
		if (m_linktypes[i] == LT_OUTPUT)
		{
			if (!m_pumpDatas[i].pPin)
				m_pumpDatas[i].pPin=new Pin();
			m_pumpDatas[i].pPin->m_LinkType=LT_OUTPUT;
			m_pumpDatas[i].pPin->SetPump(this);
			m_pumpDatas[i].pSrc = this;
			m_pumpDatas[i].pSink = NULL;
		}
		else
			m_pumpDatas[i].pPin = NULL;
		if (m_linktypes[i] < LT_OUTPUT)
			m_pumpDatas[i].pSink = this;
		else
			m_pumpDatas[i].pSink = NULL;
		m_pumpDatas[i].nSrcLink = 0;
		m_pumpDatas[i].pSrc = NULL;
	}
}

BOOL PumpABC::Connect (int to, PumpABC* pfrom, int from)
{
	assert (to>=0 && to < MAX_LINKS);
	if (m_linktypes[to] == LT_OUTPUT)
		return FALSE;
	PumpABC* pRequestPump=pfrom->RequestJoin(from, this);
	if (pRequestPump==NULL)
		return FALSE;
	m_pumpDatas[to].pSrc=pRequestPump;
	m_pumpDatas[to].nSrcLink=from;
	m_pumpDatas[to].pPin=pRequestPump->GetPin(from);
	return TRUE;
}

BOOL PumpABC::Disconnect(int to)
{
	assert (to>=0 && to < MAX_LINKS);
	if (m_linktypes[to] == LT_OUTPUT)
		return FALSE;
	m_pumpDatas[to].pSrc=NULL;
	m_pumpDatas[to].nSrcLink=0;
	m_pumpDatas[to].pPin=NULL;
	return TRUE;
}

void PumpABC::PumpRelease()
{
	releaseData();
	for (int i=0; i<m_numInOut; i++)
	{
		if (m_linktypes[i] == LT_OUTPUT && m_pumpDatas[i].pPin)
		{
			delete m_pumpDatas[i].pPin;
			m_pumpDatas[i].pPin = NULL;
		}
	}
	m_numInOut=0;
}

PumpABC* PumpABC::RequestJoin(int& to, PumpABC* pfrom)
{
	assert (to>=0 && to < MAX_LINKS);
	if (m_linktypes[to] != LT_OUTPUT)
		return NULL;
	m_pumpDatas[to].pSink = pfrom;
	return this;
}

//void PumpABC::RequestAdd(Array<PumpABC*>& pumpAray/*, Array<Pin*>& pinAray*/)
void PumpABC::RequestAdd(PtrArray& pumpAray/*, Array<Pin*>& pinAray*/)
{
	pumpAray.Add(this);
}

// whether an input/output is connected to a pipe?
BOOL PumpABC::IsConnected( int l )
{
	assert(l >= 0 && l < MAX_LINKS);
	return m_pumpDatas[l].pPin != 0;
}

BOOL PumpABC::IsConnected( int l, LinkType type )
{
	assert(l >= 0 && l < MAX_LINKS);
	return m_linktypes[l] == type;
}

// to receive Pin, to which given input/output of the pump is connected
Pin* PumpABC::GetPin( int l)
{
	assert(IsConnected(l));
	return m_pumpDatas[l].pPin;
}

typedef Map<PumpABC::id_t, FuncCreator> CreatorMap;
static CreatorMap& _creator_map () {
    static CreatorMap cmap;
    return cmap;
}

class PumpTree : public Tree
{
public:
	PumpTree(const char* id)
	{
		strcpy(m_id, id); m_nLevel=0;
	}
	~PumpTree()
	{
	}

	char m_id[CVLIB_MAX_PUMP_IDLEN];
	int m_nLevel;

	bool Search(const SString& t, PumpTree*& pPump)
	{
		if (m_id == t)
		{
			pPump=this;
			return true;
		}
		Tree* first;
		for (first=First(); first; first=first->Next())
		{
			if (((PumpTree*)first)->Search(t, pPump))
				break;
		}
		if (first)
		{
			pPump=((PumpTree*)first);
			return true;
		}
		return false;
	}

	void Output(FILE* pFile)
	{
		for (int i=0; i<m_nLevel; i++)
			fprintf (pFile, "\t");
		fprintf (pFile, "%s\n", m_id);
		Tree* first;
		for (first=First(); first; first=first->Next())
			((PumpTree*)first)->Output(pFile);
	}

	bool Add (const PumpABC::id_t& id, const PumpABC::id_t& baseid)
	{
		PumpTree* pParentTree=NULL;
		if (!Search(baseid, pParentTree))
			return false;
		PumpTree* pChild=NULL;
		if (!pParentTree->Search(id, pChild))
		{
			PumpTree* pNew=new PumpTree(id);
			pNew->m_nLevel=pParentTree->m_nLevel+1;
			pParentTree->AddLast(pNew);
		}
		else
			assert(false);
		return true;
	}
};

// static PumpTree mPumpTree(PumpABC::id());
// 
// int OutputPumpTree(const char* szFilename)
// {
// 	FILE* pFile=fopen(szFilename, "w");
// 	if (pFile)
// 	{
// 		mPumpTree.Output(pFile);
// 		fclose(pFile);
// 		return 1;
// 	}
// 	return 0;
// }

_register_creator::_register_creator(const PumpABC::id_t& id, const PumpABC::id_t& baseid, FuncCreator func)
{
//  	FuncCreator other;
//  	if (!_creator_map().Lookup(id, other))
//  		_creator_map().SetAt(id, func);
// 	FILE* pFile=fopen("c:/pump_list.txt", "a+");
// 	fprintf(pFile, "%s-->%s\n", id, baseid);
// 	fclose(pFile);
//	assert(mPumpTree.Add(id, baseid));
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
IMPLEMENT_PUMP(MatToImagePump, PumpABC)

MatToImagePump::MatToImagePump()
{
	m_numInOut=2;
	m_linktypes[0]=LT_INPUT; 
	m_linktypes[1]=LT_OUTPUT; 
	memset (m_pumpDatas, 0, sizeof(m_pumpDatas[0])*MAX_LINKS);
	ResetLinks(); 
}

PumpRet MatToImagePump::PumpProcess() 
{
	Mat* pmRet = (Mat*)GetData();
	CoImage* pImage = new CoImage;
	pmRet->ToImage(*pImage);
	RegDataEx(1, pImage, "image");
	return PR_AFTER;
}

void MatToImagePump::releaseData()
{
	if (GetPin(1)->Data())
	{
		delete ((CoImage*)GetPin(1)->Data());
		GetPin(1)->SetData(NULL);
	}
}

//--------------------------------------------------------------------------
//
//								Pipe
//
//--------------------------------------------------------------------------

Pipe::~Pipe()
{
	FreePipe();
}

void Pipe::FreePipe()
{
	for (int i=0; i<m_Pumps.GetSize(); i++)
	{
		PumpABC* pPump=(PumpABC*)m_Pumps[i];
		delete pPump;
	}
	m_Pumps.RemoveAll();
	m_OrderPumps.RemoveAll();
}

// to call processes of pumps connected to a pipe
// returns TRUE, if the pipe has got into wait state
BOOL Pipe::CallPumps(PipePassword)
{
	if (!m_fBuild)
	{
		if (!BuildOrder())
			return FALSE;
	}

	int i;
	for (i=0; i<m_OrderPumps.GetSize(); i++)
	{
		PumpABC* pPump=(PumpABC*)m_OrderPumps[i];
		PumpRet pr;
		do
		{
			pr = pPump->PumpProcess();
		} while (pr==PR_NOW);

		if (pr>=PR_NEVER)
			return FALSE;
	}

	return TRUE;
}

Pipe& Pipe::Add(PumpABC* pump)
{
	m_Pumps.Add(pump);
	m_fBuild=FALSE;
	return *this;
}

bool Pipe::BuildOrder()
{
	int i;
	int* pnFlag=new int[m_Pumps.GetSize()];
	int** ppnBuild = new int*[m_Pumps.GetSize()];

	memset (pnFlag, 0, sizeof(pnFlag[0])*m_Pumps.GetSize());
	for (i=0; i<m_Pumps.GetSize(); i++)
	{
		ppnBuild[i] = new int[((PumpABC*)m_Pumps[i])->GetParameterNum()];
		for (int j=0; j<((PumpABC*)m_Pumps[i])->GetParameterNum(); j++)
			ppnBuild[i][j] = ((PumpABC*)m_Pumps[i])->m_linktypes[j];
	}
	m_OrderPumps.RemoveAll();

	// ½å¶®ºåº· °é¾¢
	int nPrevOrder;
	do 
	{
		nPrevOrder=m_OrderPumps.GetSize();
		for (i=0; i<m_Pumps.GetSize(); i++)
		{
			if ( pnFlag[i])
				continue;
			PumpABC* pPump=(PumpABC*)m_Pumps[i];
			bool fComplete=true;
			for (int j=0; j<pPump->GetParameterNum() && fComplete; j++)
			{
				if (ppnBuild[i][j] == LT_INPUT || ppnBuild[i][j] == LT_INOUT)
				{
					PumpABC* pSrcPump=pPump->m_pumpDatas[j].pSrc;
					if (pSrcPump)
					{
						int k;
						for (k=0; k<m_Pumps.GetSize(); k++)
						{
							if (m_Pumps[k] == pSrcPump && pnFlag[k])
								break;
						}
						if (k==m_Pumps.GetSize())
							fComplete=false;
					}
				}
			}
			if (fComplete)
			{
				m_OrderPumps.Add(pPump);
				pnFlag[i]=1;
			}
		}
	} while (nPrevOrder < m_OrderPumps.GetSize());

	delete []pnFlag;
	for (i=0; i<m_Pumps.GetSize(); i++)
		delete []ppnBuild[i];
	delete []ppnBuild;

	//src, dst¼ÇÂÝ
	m_Srcs.RemoveAll();
	m_Dsts.RemoveAll();
	for (i=0; i<m_OrderPumps.GetSize(); i++)
	{
		PumpABC* pPump=(PumpABC*)m_OrderPumps[i];
		int j;
		for (j=0; j<pPump->GetParameterNum(); j++)
		{
			if ( (pPump->m_linktypes[j]==LT_INPUT || pPump->m_linktypes[j]==LT_INOUT) && pPump->m_pumpDatas[j].pSrc == NULL)
				m_Srcs.Add(&pPump->m_pumpDatas[j]);
		}
		for (j=0; j<pPump->GetParameterNum(); j++)
		{
			if (pPump->m_linktypes[j]==LT_OUTPUT && pPump->m_pumpDatas[j].pSink == NULL)
				m_Dsts.Add(&pPump->m_pumpDatas[j]);
		}
	}

	m_fBuild=FALSE;
	if (m_OrderPumps.GetSize() != m_Pumps.GetSize())
		return false;
	m_fBuild=TRUE;
	return true;
}

CoImage* Pipe::GetOutputImage()
{
	PumpABC* pPump=(PumpABC*)m_OrderPumps[m_OrderPumps.GetSize()-1];
	CoImage* image = (CoImage*)pPump->GetDataEx(pPump->GetParameterNum()-1);
	pPump->GetPin(pPump->GetParameterNum()-1)->SetData(NULL);
	return image;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
IMPLEMENT_PUMP(CompoundPump, PumpABC)
CompoundPump::CompoundPump()
{
	m_fBuild = FALSE;
}

CompoundPump::~CompoundPump()
{
	m_numInOut=0;
	PumpRelease();
}
	
PumpRet CompoundPump::PumpProcess()
{
	if (!m_fBuild)
	{
		if (!BuildOrder())
			return PR_NEVER;
	}
	int i;
	for (i=0; i<m_OrderPumps.GetSize(); i++)
	{
		PumpABC* pPump=(PumpABC*)m_OrderPumps[i];
		PumpRet pr;
		do
		{
			pr = pPump->PumpProcess();
		} while (pr==PR_NOW);
		
		if (pr>=PR_NEVER)
			return PR_NEVER;
	}

	return PR_AFTER;
}

CompoundPump& CompoundPump::Add(PumpABC* pump)
{
	pump->RequestAdd(m_Pumps);
	m_fBuild=FALSE;
	return *this;
}

CompoundPump& CompoundPump::DetachAt(int at)
{
	m_Pumps.RemoveAt(at);
	m_fBuild=FALSE;
	return *this;
}

CompoundPump& CompoundPump::DetachAll()
{
	m_Pumps.RemoveAll();
	m_fBuild=FALSE;
	return *this;
}

Pin* CompoundPump::GetPin( int l )
{
	if (!m_fBuild)
	{
		if (!BuildOrder())
			return NULL;
	}
	return PumpABC::GetPin(l);
}

BOOL CompoundPump::IsConnected( int l )
{
	if (!m_fBuild)
	{
		if (!BuildOrder())
			return FALSE;
	}
	return PumpABC::IsConnected(l);
}

BOOL CompoundPump::IsConnected( int l, LinkType type )
{
	if (!m_fBuild)
	{
		if (!BuildOrder())
			return FALSE;
	}
	return PumpABC::IsConnected(l);
}

BOOL CompoundPump::Connect (int to, PumpABC* pfrom, int from)
{
	assert (to>=0 && to < MAX_LINKS);
	assert (GetPumpCount() != 0);

	if (m_linktypes[to] == LT_OUTPUT)
		return FALSE;

	if (!m_fBuild)
	{
		if (!BuildOrder())
			return FALSE;
	}

	PumpABC* pRequestPump=pfrom->RequestJoin(from, this);
	if (pRequestPump==NULL)
		return FALSE;
	m_pumpDatas[to].pSrc = pRequestPump;
	m_pumpDatas[to].pPin = pRequestPump->GetPin(from);
	m_pumpDatas[to].nSrcLink = from;

	((PumpData*)m_Srcs[to])->pSrc = pRequestPump;
	((PumpData*)m_Srcs[to])->nSrcLink = from;
	((PumpData*)m_Srcs[to])->pPin = pRequestPump->GetPin(from);
	return TRUE;
}

BOOL CompoundPump::Disconnect(int to)
{
	assert (to>=0 && to < MAX_LINKS);
	assert (GetPumpCount() != 0);
	
	if (m_linktypes[to] == LT_OUTPUT)
		return FALSE;
	
	if (!m_fBuild)
	{
		if (!BuildOrder())
			return FALSE;
	}
	m_pumpDatas[to].pSrc = NULL;
	m_pumpDatas[to].pPin = NULL;
	m_pumpDatas[to].nSrcLink = 0;
	
	((PumpData*)m_Srcs[to])->pSrc = NULL;
	((PumpData*)m_Srcs[to])->nSrcLink = 0;
	((PumpData*)m_Srcs[to])->pPin = NULL;
	return TRUE;
}

PumpABC* CompoundPump::RequestJoin(int& to, PumpABC* pfrom)
{
	assert (to>=0 && to < MAX_LINKS);
	if (m_linktypes[to] != LT_OUTPUT)
		return NULL;
	m_pumpDatas[to].pSink = pfrom;
	((PumpData*)m_Dsts[to-m_Srcs.GetSize()])->pSink = pfrom;
	return this;
}

//void CompoundPump::RequestAdd(Array<PumpABC*>& pumpAray/*, Array<Pin*>& pinAray*/)
void CompoundPump::RequestAdd(PtrArray& pumpAray/*, Array<Pin*>& pinAray*/)
{
	pumpAray.Add(this);
}

void CompoundPump::releaseData()
{
	for (int i=0; i<m_Pumps.GetSize(); i++)
	{
		PumpABC* pPump=(PumpABC*)m_Pumps[i];
		delete pPump;
	}
	m_Pumps.RemoveAll();
	m_OrderPumps.RemoveAll();
}

bool CompoundPump::iBuildOrder()
{
	int i;
	int* pnFlag=new int[m_Pumps.GetSize()];
	int** ppnBuild = new int*[m_Pumps.GetSize()];
	
	memset (pnFlag, 0, sizeof(pnFlag[0])*m_Pumps.GetSize());
	for (i=0; i<m_Pumps.GetSize(); i++)
	{
		ppnBuild[i] = new int[((PumpABC*)m_Pumps[i])->GetParameterNum()];
		for (int j=0; j<((PumpABC*)m_Pumps[i])->GetParameterNum(); j++)
			ppnBuild[i][j] = ((PumpABC*)m_Pumps[i])->m_linktypes[j];
	}
	m_OrderPumps.RemoveAll();
	
	// ½å¶®ºåº· °é¾¢
	int nPrevOrder;
	do 
	{
		nPrevOrder=m_OrderPumps.GetSize();
		for (i=0; i<m_Pumps.GetSize(); i++)
		{
			if ( pnFlag[i])
				continue;
			PumpABC* pPump=(PumpABC*)m_Pumps[i];
			bool fComplete=true;
			for (int j=0; j<pPump->GetParameterNum() && fComplete; j++)
			{
				if (ppnBuild[i][j] == LT_INPUT || ppnBuild[i][j] == LT_INOUT)
				{
					PumpABC* pSrcPump=pPump->m_pumpDatas[j].pSrc;
					if (pSrcPump)
					{
						int k;
						for (k=0; k<m_Pumps.GetSize(); k++)
						{
							if (m_Pumps[k] == pSrcPump && pnFlag[k])
								break;
						}
						if (k==m_Pumps.GetSize())
							fComplete=false;
					}
				}
			}
			if (fComplete)
			{
				m_OrderPumps.Add(pPump);
				pnFlag[i]=1;
			}
		}
	} while (nPrevOrder < m_OrderPumps.GetSize());
	
	delete []pnFlag;
	for (i=0; i<m_Pumps.GetSize(); i++)
		delete []ppnBuild[i];
	delete []ppnBuild;
	
	//src, dst¼ÇÂÝ
	m_Srcs.RemoveAll();
	m_Dsts.RemoveAll();
	for (i=0; i<m_OrderPumps.GetSize(); i++)
	{
		PumpABC* pPump=(PumpABC*)m_OrderPumps[i];
		int j;
		for (j=0; j<pPump->GetParameterNum(); j++)
		{
			if ( (pPump->m_linktypes[j]==LT_INPUT || pPump->m_linktypes[j]==LT_INOUT) && pPump->m_pumpDatas[j].pSrc == NULL)
				m_Srcs.Add(&pPump->m_pumpDatas[j]);
		}
		for (j=0; j<pPump->GetParameterNum(); j++)
		{
			if (pPump->m_linktypes[j]==LT_OUTPUT && pPump->m_pumpDatas[j].pSink == NULL)
				m_Dsts.Add(&pPump->m_pumpDatas[j]);
		}
	}
	
	m_fBuild=FALSE;
	if (m_OrderPumps.GetSize() != m_Pumps.GetSize())
		return false;
	m_fBuild=TRUE;
	return true;
}

bool CompoundPump::BuildOrder()
{
	if (!m_fBuild)
		m_fBuild= iBuildOrder();
	if (!m_fBuild)
		return false;

	if (m_Srcs.GetSize()+m_Dsts.GetSize()>=MAX_LINKS)
		return false;

	m_numInOut=m_Srcs.GetSize()+m_Dsts.GetSize();
	int i, k;
	for (i=0; i<m_Srcs.GetSize(); i++)
		m_linktypes[i]=LT_INPUT;
	for (k=0; k<m_Dsts.GetSize(); k++, i++)
		m_linktypes[i]=((PumpData*)m_Dsts[k])->pPin->m_LinkType;
	
	for (i=0; i<m_Srcs.GetSize(); i++)
		m_pumpDatas[i]=*(PumpData*)m_Srcs[i];
	for (k=0; k<m_Dsts.GetSize(); k++, i++)
		m_pumpDatas[i]=*(PumpData*)m_Dsts[k];
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
IMPLEMENT_PUMP(BranchPump, PumpABC)
BranchPump::BranchPump(PumpABC* pfrom, int from)
{
	m_linktypes[0]=LT_INPUT; 
	for (int i=1; i<MAX_LINKS; i++)
		m_linktypes[i]=LT_OUTPUT; 
	memset (m_pumpDatas, 0, sizeof(m_pumpDatas[0])*MAX_LINKS);

	m_numInOut=1;
	ResetLinks(); 
	Connect(0, pfrom, from);
}

PumpRet BranchPump::PumpProcess()
{
	int i;
	void* pdata=GetData();
	for (i=1; i<m_numInOut; i++)
		RegDataEx(i, pdata, GetDataType());
	return PR_AFTER;
}

BOOL BranchPump::Connect (int to, PumpABC* pfrom, int from)
{
	if (to != 0)
		return FALSE;
	PumpABC* pRequestPump=pfrom->RequestJoin(from, this);
	if (pRequestPump==NULL)
		return FALSE;
	m_pumpDatas[to].pSrc=pRequestPump;
	m_pumpDatas[to].nSrcLink=from;
	m_pumpDatas[to].pPin=pRequestPump->GetPin(from);
	return TRUE;
}

PumpABC* BranchPump::RequestJoin(int& to, PumpABC* pfrom)
{
	if (to == 0)
		return NULL;
	m_numInOut ++;
	ResetLinks();
	PumpABC::RequestJoin (to, pfrom);
	return this;
}

}
