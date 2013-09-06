/*!
 * \file	Algorithm.cpp
 * \ingroup base
 * \brief	·º´Ç Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include "Algorithm.h"

namespace CVLib
{

Algorithm::Algorithm()
{
	m_szID[0] = 0;
	m_szTotalID[0] = 0;
}

Algorithm::~Algorithm()
{
	RemoveAlgorithms ();
}

const char*  Algorithm::GetID ()
{
	if (GetAlgorithmCount() == 0)
	{
		if (m_szID[0] == 0)
			strcpy (m_szID, "None");
		return GetSelfID();
	}
	else
	{
		m_szTotalID[0] = 0;
		if (m_szID[0] == 0)
			strcpy (m_szID, "Unknown");
		strcpy (m_szTotalID, m_szID);
		for (int i = 0; i < GetAlgorithmCount(); i ++)
		{
			Algorithm* pAlgorithm = GetAlgorithm(i);
			strcat (m_szTotalID, "[");
			if (pAlgorithm)
				strcat (m_szTotalID, pAlgorithm->GetID ());
			else
				strcat (m_szTotalID, "None");
			strcat (m_szTotalID, "]");
		}
	}
	return m_szTotalID;
}

void Algorithm::SetSelfID (const char* szID)
{
	assert ( strlen (szID) < CVLIB_IDLEN );
	strcpy (m_szID, szID);
}

const char* Algorithm::GetSelfID() const
{
	return m_szID;
}

Algorithm* Algorithm::GetAlgorithm (int nIdx) const
{
	return (Algorithm*)m_AlgorithmArray[nIdx];
}

int Algorithm::GetAlgorithmCount() const
{
	return m_AlgorithmArray.GetSize();
}

int Algorithm::AddAlgorithm (Algorithm* pAlgorithm)
{
	return m_AlgorithmArray.Add (pAlgorithm);
}

void Algorithm::RemoveAlgorithms ()
{
	m_AlgorithmArray.RemoveAll();
}

void Algorithm::DelAlgorithm(int nIdx)
{
	m_AlgorithmArray.RemoveAt(nIdx);
}

void Algorithm::SetAlgorithm (int nIdx, Algorithm* pAlgorithm)
{
	m_AlgorithmArray.SetAt(nIdx, pAlgorithm);
}

}
