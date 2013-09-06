 
/*!
 * \file    ClassifierABC.h
 * \ingroup	base
 * \brief	¹¤µù±¨Ëæ ´ÝÂÙ ±¨½ó¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#if !defined (_CLASSIFIERABC_H_)
#define _CLASSIFIERABC_H_

#include "cvlibbase/inc/Algorithm.h"

namespace CVLib
{

#define CLASSIFIER_UNKNOWN	    ""
#define CLASSIFIER_SET			"CSet"
#define CLASSIFIER_FISHER		"Fisher"
	
#define CLASSIFIER_LINEAR		"Linear"
#define CLASSIFIER_LDA			"LDA"
#define CLASSIFIER_RLDA			"RLDA"
#define CLASSIFIER_DLDA			"DLDA"
#define CLASSIFIER_NLDA			"NLDA"
#define CLASSIFIER_NDA			"NDA"
#define CLASSIFIER_NNFA			"NNFA"
#define CLASSIFIER_NNSA			"NNSA"
#define CLASSIFIER_PNFA			"PNFA"
#define CLASSIFIER_PNSA			"PNSA"
#define CLASSIFIER_SSDA			"SSDA"
#define CLASSIFIER_NEDA			"NEDA"
	
#define CLASSIFIER_PCA			"PCA"
#define CLASSIFIER_KPCA			"KPCA"
	
#define CLASSIFIER_GDA			"GDA"
#define CLASSIFIER_KFA			"KFA"
#define CLASSIFIER_KDDA			"KDDA"
	
#define CLASSIFIER_MAP			"MAP"
#define CLASSIFIER_ML			"ML"
	
#define CLASSIFIER_RBOOST		"RBoost"
#define CLASSIFIER_RSTUMPBOOST	"RStumpBoost"
#define CLASSIFIER_STUMP		"StumpBoost"
#define CLASSIFIER_KNN			"KNN"

/**
 @brief ¸Ú¼¸Ì© ±¬ËË
 */
#define VERSION_LEN		16

class CVLIB_DECLSPEC MachineABC : public Algorithm
{
public:
	MachineABC();
	virtual ~MachineABC();
	virtual void	Release ();
	void	SetVersion (const char* szVersion);
	void	GetVersion (char* szVersion) const;
	virtual int		FromFile (FILE* pFile);
	virtual int		FromFile (XFile* pFile);
	virtual int		ToFile (FILE* pFile) const;
	virtual int		ToFile (XFile* pFile) const;
protected:
	//!  ¸Ú¼¸ 
	char	m_szVersion[VERSION_LEN];	
};

class CVLIB_DECLSPEC IMapperABC
{
public:
	virtual	Vec*	Project (Vec* pvIn) const=0;
	virtual	void	Project (Vec* pvIn, Vec* pvOut) const=0;
};

class CVLIB_DECLSPEC MapperABC : public MachineABC, public IMapperABC
{
public:
	virtual MapperABC* New() const=0;
	virtual int InLength() const=0;
	virtual int OutLength() const=0;
};

class CVLIB_DECLSPEC IClassifierABC
{
public:
	virtual float	Eval(Vec* pvFea)=0;
};

class CVLIB_DECLSPEC ClassifierABC : public MachineABC, public IClassifierABC
{
public:
	virtual ClassifierABC* New() const=0;
};

class CVLIB_DECLSPEC MachineSet : public MachineABC
{
public:
	MachineSet();
	virtual ~MachineSet();
	
	int Add (MachineABC* pClassifier);
	
	void RemoveAt (int nIdx);
	void DetachAt (int nIdx);
	
	int GetSize ()	const{ return m_nNum;}
	int GetActiveSize() const {return m_nActiveNum;}
	void SetActiveSize(int nNum) {m_nActiveNum=nNum;}

	virtual int		FromIniFile(const char* szFilename);
	virtual int		FromFile (FILE* pFile);
	virtual int		FromFile (XFile* pFile);
	virtual int		ToFile (FILE* pFile) const;
	virtual int		ToFile (XFile* pFile) const;
	
	virtual void	Release ();
	virtual void	DetachAll();
	
	// 	ClassifierABC* GetAt (int nIdx) const;
	// 	
	// 	ClassifierABC** GetClassifiers ()  {return m_pClassifierArray;}
protected:
	MachineABC** m_pClassifierArray;
	int	m_nNum;
	int m_nActiveNum;
};

class CVLIB_DECLSPEC ClassifierSet : public IClassifierABC, public MachineSet
{
public:
	ClassifierSet();
	virtual ~ClassifierSet();

	ClassifierABC* GetAt (int nIdx) const;
	ClassifierABC** GetClassifiers ()  {return (ClassifierABC**)m_pClassifierArray;}
	virtual float	Eval(Vec* pvFea) {return 0.0f;}
};

class CVLIB_DECLSPEC MapperSet : public IMapperABC, public MachineSet
{
public:
	MapperSet();
	virtual ~MapperSet();
	
	MapperABC* GetAt (int nIdx) const;
	MapperABC** GetClassifiers ()  {return (MapperABC**)m_pClassifierArray;}
	
	virtual	Vec*	Project (Vec* pvIn) const {return NULL;}
	virtual	void	Project (Vec* pvIn, Vec* pvOut) const {return;}
};

}

#if CVLIB_OS == CVLIB_OS_WIN32

#ifdef NDEBUG
#pragma comment(lib,"cvlibbase.lib")
#else
#pragma comment(lib,"cvlibbased.lib")
#endif

#endif

#endif //_CLASSIFIERABC_H_
