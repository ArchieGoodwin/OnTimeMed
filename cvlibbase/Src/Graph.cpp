/*!
 * \file	PumpABC.cpp
 * \ingroup base
 * \brief	·º´Ç Ê²°Ö¶®´ÍÌ© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include <assert.h>
#include "Graph.h"

namespace CVLib
{

bool Vertex::IsConnected(Vertex* pV)
{
	int i;
	for (i=0; i<m_Edges.GetSize(); i++)
	{
		if (m_Edges[i] == pV)
			break;
	}
	if (i==m_Edges.GetSize())
		return false;
	return true;
}

bool Graph::BuildOrder()
{
	return true;
}

}
