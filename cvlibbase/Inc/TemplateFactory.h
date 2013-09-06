
/*!
 * \file	TemplateFactory.h
 * \ingroup base
 * \brief	FactoryÂôÀ°¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _TEMPLATEFACTORY_H_
#define _TEMPLATEFACTORY_H_

#include "template.h"
#include "cvlibbasedef.h"
#include "StringArray.h"
#include "PtrArray.h"

namespace CVLib
{

template<class TYPE>
class CVLIB_DECLSPEC FactoryTemplate
{
public:
	FactoryTemplate() {m_nActiveIdx = -1;m_nDefaultNum = 0;}
	virtual ~FactoryTemplate()	{Release();}
	
	virtual void	Init(){}
	virtual void	Release(){};
	virtual	int		Find (TYPE* pPrototype){return -1;}

	int				Find (const SString& szID) const {
						SString temp=szID;
						int npos=szID.Find('_', 0);
						if (npos!=-1)
							temp=szID.Left(npos);
						int i;
						for (i=0; i<m_PrototypeNames.GetSize(); i++)
						{
							if (temp == m_PrototypeNames[i])
								break;
						}
						if (i==m_PrototypeNames.GetSize())return -1;
						return i; 
					}
	TYPE*			Get(const SString& szID) { return GetPrototype(Find(szID)); }
	void			SetCurSel(int nIdx) { if (nIdx < 0 || nIdx >= m_PrototypeNames.GetSize()) return; m_nActiveIdx = nIdx; }
	int				GetCurSel() const {return m_nActiveIdx;}
	SString			GetName(int nIdx) const {
		if (nIdx < 0 || nIdx >= m_PrototypeNames.GetSize()) return "";
		else return m_PrototypeNames[nIdx];	}
	SString			GetCurrName() const {
		if (m_nActiveIdx < 0 || m_nActiveIdx >= m_PrototypeNames.GetSize()) return "";
		else return m_PrototypeNames[m_nActiveIdx];	}
	int				GetCount() const {return m_Prototypes.GetSize();}
	
	void			Add(TYPE* pPrototype, const SString& szID)	{
		m_PrototypeNames.Add (szID);
		m_Prototypes.Add(pPrototype);
	}
	TYPE*			GetPrototype(int nIdx)	{
		if (nIdx < 0 || nIdx >= m_Prototypes.GetSize()) return NULL;
		else return (TYPE*)m_Prototypes[nIdx];}
	TYPE*			GetCurrPrototype()	{
		if (m_nActiveIdx < 0 || m_nActiveIdx >= m_Prototypes.GetSize()) return NULL;
		else return (TYPE*)m_Prototypes[m_nActiveIdx];}
	int ToFile(const SString& sz)
	{
		FILE* pfile=fopen(sz, "w");
		if (pfile)
		{
			for (int i=0; i<m_PrototypeNames.GetSize(); i++)
				fprintf (pfile, "%s\n", (const char*)m_PrototypeNames[i]);
			fclose(pfile);
		}
		return 1;
	}
protected:
	StringArray		m_PrototypeNames;
	int				m_nDefaultNum;
	int				m_nActiveIdx;
	PtrArray		m_Prototypes;
};

}

#endif //_TEMPLATEFACTORY_H_