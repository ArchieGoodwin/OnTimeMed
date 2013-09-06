
/*!
 * \file
 * \brief	¹¤µù±¨Ëæ ´ÝÂÙ ±¨½ó¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */
#include "ClassifierABC.h"
#include "cvlibbase/inc/IniFile.h"

namespace CVLib 
{

/************************************************************************/
/*                                                                      */
/************************************************************************/
MachineABC::MachineABC()
{
	memset (m_szVersion, 0, sizeof(char) * VERSION_LEN);
}

MachineABC::~MachineABC()
{
	Release();
}

void MachineABC::SetVersion (const char* szVersion)
{
	memcpy (m_szVersion, szVersion, VERSION_LEN);
}

void MachineABC::GetVersion (char* szVersion) const
{
	memcpy (szVersion, m_szVersion, VERSION_LEN);
}

void MachineABC::Release ()
{
}

int	MachineABC::FromFile (FILE* pFile) 
{
	fread (m_szVersion, 1, VERSION_LEN, pFile);
	return 1;
}
int	MachineABC::FromFile (XFile* pFile) 
{
	pFile->Read(m_szVersion, 1, VERSION_LEN);
	return 1;
}

int	MachineABC::ToFile (FILE* pFile) const
{
	fwrite (m_szVersion, 1, VERSION_LEN, pFile);
	return 1;
}
int	MachineABC::ToFile (XFile* pFile) const
{
	pFile->Write(m_szVersion, 1, VERSION_LEN);
	return 1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
MachineSet::MachineSet()
{
	m_nNum = m_nActiveNum = 0;
	m_pClassifierArray = NULL;
}

MachineSet::~MachineSet()
{
	Release();
}

int MachineSet::Add (MachineABC* pClassifier)
{
	MachineABC** pClassifiers = new MachineABC*[m_nNum + 1];
	if (m_nNum != 0)
	{
		memcpy(pClassifiers, m_pClassifierArray, sizeof(MachineABC*) * m_nNum);
	}
	pClassifiers[m_nNum] = pClassifier;

	if (m_pClassifierArray)
		delete []m_pClassifierArray;
	m_pClassifierArray = pClassifiers;

	m_nNum ++;
	m_nActiveNum = m_nNum;
	return m_nNum-1;
}

void MachineSet::RemoveAt (int nIdx)
{
	if (nIdx < 0 || nIdx >= m_nNum)
		return;
	if (GetSize() == 0)
		return;

	m_pClassifierArray[nIdx]->Release();
	delete m_pClassifierArray[nIdx];

	for (int i = nIdx; i < m_nNum - 1; i ++)
		m_pClassifierArray[i] = m_pClassifierArray[i+1];
	m_nNum --;
	m_nActiveNum = m_nNum;
}

void MachineSet::DetachAt (int nIdx)
{
	if (nIdx < 0 || nIdx >= m_nNum)
		return;
	if (GetSize() == 0)
		return;
	
	for (int i = nIdx; i < m_nNum - 1; i ++)
		m_pClassifierArray[i] = m_pClassifierArray[i+1];
	m_nNum --;
	m_nActiveNum = m_nNum;
}

void MachineSet::Release ()
{
	if (m_pClassifierArray)
	{
		for (int i = 0; i < GetSize(); i ++)
		{
			m_pClassifierArray[i]->Release();
			delete m_pClassifierArray[i];
		}
		delete []m_pClassifierArray;
		m_pClassifierArray = NULL;
		m_nNum = 0;
		m_nActiveNum = m_nNum;
	}
}

void MachineSet::DetachAll()
{
	if (m_pClassifierArray)
	{
		delete []m_pClassifierArray;
		m_pClassifierArray = NULL;
		m_nNum = 0;
		m_nActiveNum = m_nNum;
	}
}

int MachineSet::FromFile (FILE* pFile)
{
	MachineABC::FromFile (pFile);

	if (strcmp(m_szVersion, CLASSIFIER_SET))
		return 0;
	fread (&m_nNum, sizeof(m_nNum), 1, pFile);
	m_pClassifierArray = new MachineABC*[m_nNum];
	for (int i = 0; i < m_nNum; i ++)
	{
//		m_pClassifierArray[i] = (MachineABC*)LoadMachine (pFile);
		assert (false);
	}
	return 1;
}

int MachineSet::ToFile (FILE* pFile) const
{
	MachineABC::ToFile (pFile);

	fwrite (&m_nNum, sizeof(m_nNum), 1, pFile);
	for (int i = 0; i < m_nNum; i ++)
		m_pClassifierArray[i]->ToFile(pFile);
	return 1;
}

int MachineSet::FromFile (XFile* pFile)
{
	return 1;
}

int MachineSet::ToFile(XFile* pFile) const
{
	pFile->Write (&m_nNum, sizeof(m_nNum), 1);
	for (int i = 0; i < m_nNum; i ++)
		m_pClassifierArray[i]->ToFile(pFile);
	return 1;
}
int	MachineSet::FromIniFile(const char* szFilename)
{
	IniFile ini(szFilename);
	if (!ini.ReadFile())
		return 0;
	if (ini.FindKey (SString("MachineSet")) == IniFile::noID)
		return 0;
	int num=ini.GetValueI(SString("MachineSet"), SString("num"));
	for (int i=0; i<num; i++)
	{
		char szvalue[260];
		sprintf (szvalue, "machine%d", i);
		SString szPath=ini.GetValue(SString("MachineSet"), szvalue);
//		Add (LoadMachine(szPath));
		assert (false);
	}
	return 1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
ClassifierSet::ClassifierSet()
{
	SetSelfID (CLASSIFIER_SET);
	strcpy (m_szVersion, CLASSIFIER_SET);
	m_nNum = m_nActiveNum = 0;
	m_pClassifierArray = NULL;
}

ClassifierSet::~ClassifierSet()
{
	Release();
}

ClassifierABC* ClassifierSet::GetAt (int nIdx) const
{
	assert (nIdx >= 0 && nIdx < m_nNum);
	return (ClassifierABC*)m_pClassifierArray[nIdx];
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
MapperSet::MapperSet()
{
	SetSelfID (CLASSIFIER_SET);
	strcpy (m_szVersion, CLASSIFIER_SET);
	m_nNum = m_nActiveNum = 0;
	m_pClassifierArray = NULL;
}

MapperSet::~MapperSet()
{
	Release();
}

MapperABC* MapperSet::GetAt (int nIdx) const
{
	assert (nIdx >= 0 && nIdx < m_nNum);
	return (MapperABC*)m_pClassifierArray[nIdx];
}

}