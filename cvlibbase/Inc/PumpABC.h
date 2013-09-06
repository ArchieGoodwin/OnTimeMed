
/*!
 * \file	PumpABC.h
 * \ingroup base
 * \brief	·º´Ç Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _PUMPABC_H_
#define _PUMPABC_H_


#include "PtrArray.h"
#include "SString.h"
#include "CoImage.h"
#include "Graph.h"

namespace CVLib
{

#pragma warning( disable : 4172 )

enum PipePassword { PIPE_PASSWORD };

enum PipeStatus {
	PS_ZERO,
	PS_COMPLETE,
	PS_CANTDO,
	PS_OK,
	PS_BREAK,
	PS_ABORT
};

#define MAX_LINKS 8
enum LinkType { LT_NONE, LT_INPUT, LT_INOUT, LT_OUTPUT, LT_MAX };

enum PumpRet {
	PR_NOW, 	// the pump can be called once again immediately
	PR_AFTER,	// the pump can be called once again
	PR_NEVER,	// the pump has completed operation, and it cannot be called once more
	PR_BREAK,	// the pump has interrupted operation of the pipeline
	PR_ABORT	// the pump has interrupted operation of the pipeline for its restarting
};

enum PipeLineRet {
	PLR_FALSE = 0,
	PLR_TRUE = 1,
	PLR_ABORT = 2
};

class Pin;
class PumpABC;
class CompoundPump;

class CVLIB_DECLSPEC Pin
{
public:
	Pin():m_fMany(false),m_data(0),m_pPump(0),m_LinkType(LT_MAX){}
	PumpABC* GetPump();

	inline void SetData(void* data) {m_data=data;}
	inline void* Data() const {return m_data;}
	inline const SString& Type() const {return m_sztype;}
	inline void SetType (const SString& szType) {m_sztype=szType;}
	inline LinkType& Link () {return m_LinkType;}
	inline void SetPump(PumpABC* pPump) {m_pPump=pPump;}
private:
	bool m_fMany;
	SString m_sztype;
	void* m_data;
	PumpABC*	m_pPump;
public:
	LinkType	m_LinkType;
};

struct CVLIB_DECLSPEC PumpData
{
	Pin* pPin;
	PumpABC* pSrc;
	PumpABC* pSink;
	int nSrcLink;
};

#define CVLIB_MAX_PUMP_IDLEN 64
// struct PumpClass
// {
// 	// Attributes
// 	const SString m_lpszClassName;
// 	int m_nObjectSize;
// //	UINT m_wSchema; // schema number of the loaded class
// 	PumpABC* (*m_pfnNew)(); // NULL => abstract class
// 	PumpClass* m_pBaseClass;
// 	
// 	// Operations
// //	PumpABC* CreateObject();
// 	BOOL IsDerivedFrom(const PumpClass* pBaseClass) const;
// 	
// 	// Implementation
// // 	void Store(CArchive& ar) const;
// // 	static PumpClass* Load(CArchive& ar, UINT* pwSchemaNum);
// 	
// 	// PumpClass objects linked together in simple list
// 	PumpClass* m_pNextClass;       // linked list of registered classes
// };

/**
 * @brief  Ãùº¬¸êÃû ±¨½ó¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC PumpABC
{
	friend class Pipe;
	friend class CompoundPump;
	friend class BranchPump;
private:
	static const char PumpABC_id_[CVLIB_MAX_PUMP_IDLEN];
public:
	typedef SString id_t;
	static const char* id ();

	PumpABC();
	virtual ~PumpABC() {PumpRelease();}

// 	virtual PumpClass* GetPumpClass() const;
// 	virtual const id_t& id() const=0;
// 	virtual PumpABC* New() const=0;
	virtual PumpABC* Clone() const {return NULL;}

	virtual CoImage* PumpProcess(CoImage* pImage) {return pImage;}

// 	virtual void PumpInitialize();
	virtual PumpRet PumpProcess() {return PR_AFTER;}
	void PumpRelease();

 	virtual Pin* GetPin( int l );
	virtual BOOL IsConnected( int l );
 	virtual BOOL IsConnected( int l, LinkType type );
	virtual BOOL Connect (int to, PumpABC* pfrom, int from);
	virtual BOOL Disconnect(int to);

	void* GetData() { return GetDataEx(0);}
	void* GetDataEx(int l);
	const SString& GetDataType() { return GetDataTypeEx(0);}
	const SString& GetDataTypeEx(int l);
 
 	void RegData( void* pdata, const SString& type="") { RegDataEx(0, pdata, type); }
 	void RegDataEx( int l, void* pdata, const SString& type="");

	void ResetLinks();

	int GetOutputNum();
	int GetParameterNum() const {return m_numInOut;}

// public:
// 	// Attributes
// 	BOOL IsKindOf(const PumpClass* pPump) const;
// 	// Implementation
// 	static const PumpClass classPump;

protected:
	virtual void releaseData(){}
	virtual PumpABC* RequestJoin(int& to, PumpABC* pfrom);
//	virtual void RequestAdd(Array<PumpABC*>& pumpAray/*, Array<Pin*>& pinAray*/);
	virtual void RequestAdd(PtrArray& pumpAray/*, Array<Pin*>& pinAray*/);

	int m_numInOut; // total number of inputs/outputs
	LinkType m_linktypes[MAX_LINKS];
	PumpData m_pumpDatas[MAX_LINKS];
};


//////////////////////////////////////////////////////////////////////////////
// Helper macros for declaring PumpClass compatible classes

typedef PumpABC* (*FuncCreator) ();

struct CVLIB_DECLSPEC _register_creator {
    _register_creator(const PumpABC::id_t&, const PumpABC::id_t&, FuncCreator);
};

/// Use a prefix @a p to allow several classes in a same .cpp file
#define DECLARE_PUMP(cls)                             \
private:\
	static const char cls##_id_[CVLIB_MAX_PUMP_IDLEN];\
public:\
	static const char* id ();\
	static PumpABC* New () { return new cls; }					\
	virtual PumpABC* Clone() const {return new cls(*this);}		\
private:


#define IMPLEMENT_PUMP(cls, basecls)\
    const char cls::cls##_id_[CVLIB_MAX_PUMP_IDLEN] = #cls;\
	const char* cls::id () { return cls##_id_;}

//#define IMPLEMENT_PUMP(cls, basecls)
//    const PumpABC::id_t cls::cls##_id_ = #cls;
//	const _register_creator cls::registercreator(cls::cls##_id_, basecls::basecls##_id_, cls::New);



/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class TYPE>
class CVLIB_DECLSPEC InputDataPump : public PumpABC 
{
public:
	InputDataPump(const TYPE* pdata, const SString& szType);
	virtual ~InputDataPump(){PumpRelease();}
protected:
	virtual void releaseData();
};

template<class TYPE>
InputDataPump<TYPE>::InputDataPump(const TYPE* pdata, const SString& szType) 
{
	m_numInOut=1;
	m_linktypes[0]=LT_OUTPUT; 
	memset (m_pumpDatas, 0, sizeof(m_pumpDatas[0])*MAX_LINKS);
	ResetLinks(); 
	m_pumpDatas[0].pPin->SetData((void*)pdata); 
	m_pumpDatas[0].pPin->SetType(szType);
}

template<class TYPE>
void InputDataPump<TYPE>::releaseData()
{
	if (GetPin(0)->Data())
	{
		delete (TYPE*)GetPin(0)->Data();
		GetPin(0)->SetData(NULL);
	}
}


class CVLIB_DECLSPEC MatToImagePump : public PumpABC
{
	DECLARE_PUMP(MatToImagePump)
public:
	MatToImagePump();
	virtual ~MatToImagePump(){PumpRelease();}
	virtual PumpRet PumpProcess();
protected:
	virtual void releaseData();
};

class CVLIB_DECLSPEC BranchPump : public PumpABC
{
	DECLARE_PUMP(BranchPump)
public:
	BranchPump() {}
	BranchPump(PumpABC* pfrom, int from);
	virtual PumpRet PumpProcess();
	
	virtual BOOL Connect (int to, PumpABC* pfrom, int from);
protected:
	virtual PumpABC* RequestJoin(int& to, PumpABC* pfrom);
};

class CVLIB_DECLSPEC CompoundPump : public PumpABC
{
	DECLARE_PUMP(CompoundPump)
public:
	CompoundPump();
	virtual ~CompoundPump();

	CompoundPump& Add(PumpABC* pump);
	CompoundPump& DetachAt(int at);
	CompoundPump& DetachAll();
	PumpABC*	GetAt(int at) {return (PumpABC*)m_Pumps[at];}
	int	GetPumpCount() const {return m_Pumps.GetSize();}

	virtual PumpRet PumpProcess();
	virtual Pin* GetPin( int l );
	virtual BOOL IsConnected( int l );
	virtual BOOL IsConnected( int l, LinkType type );
	virtual BOOL Connect (int to, PumpABC* pfrom, int from);
	virtual BOOL Disconnect(int to);
protected:
	virtual PumpABC* RequestJoin(int& to, PumpABC* pfrom);
//	virtual void RequestAdd(Array<PumpABC*>& pumpAray/*, Array<Pin*>& pinAray*/);
	virtual void RequestAdd(PtrArray& pumpAray/*, Array<Pin*>& pinAray*/);
	virtual void releaseData();
	// to reset the status:
	// 	void ResetStatus() { status = PS_ZERO; }
	
	BOOL m_fBuild; // flag handling the construction of a pipe
//	Array<Pin*> m_Pins;
//	Array<PumpABC*> m_Pumps;
//	Array<PumpABC*> m_OrderPumps;
	PtrArray m_Pumps;
	PtrArray m_OrderPumps;
	
	PtrArray m_Srcs;
	PtrArray m_Dsts;
// 	Array<PumpData*> m_Srcs;
// 	Array<PumpData*> m_Dsts;
	
	bool BuildOrder();
	bool iBuildOrder();
};

class CVLIB_DECLSPEC Pipe
{
public:
	Pipe(){m_fBuild=FALSE;}
	virtual ~Pipe(); // deletes of Pin
	
	Pipe& Add(PumpABC* pump);
	PumpABC* GetAt(int at)	{return (PumpABC*)m_Pumps[at];}
	void DetachAt(int at)	{m_Pumps.RemoveAt(at); m_fBuild=FALSE;}
	void DetachAll()	{m_Pumps.RemoveAll(); m_fBuild=FALSE;}
	
 	int GetPumpCount() {return m_Pumps.GetSize();}
// 	int GetPinCount() {return m_Pins.GetSize();}
	
	BOOL CallPumps(PipePassword psw);  // to call processes of pumps one after another
	void FreePipe();
	CoImage* GetOutputImage();
protected:
	// to reset the status:
	// 	void ResetStatus() { status = PS_ZERO; }
	
	BOOL m_fBuild; // flag handling the construction of a pipe
// 	Array<Pin*> m_Pins;
// 	Array<PumpABC*> m_Pumps;
// 	Array<PumpABC*> m_OrderPumps;
// 	
// 	Array<PumpData*> m_Srcs;
// 	Array<PumpData*> m_Dsts;
	PtrArray m_Pumps;
	PtrArray m_OrderPumps;
	
	PtrArray m_Srcs;
	PtrArray m_Dsts;
	
	virtual bool BuildOrder();
};// class Pipe

}

#pragma warning( disable : 4172 )

#endif //_PUMPABC_H_