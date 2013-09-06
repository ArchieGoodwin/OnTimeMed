/*!
 * \file	Plex.cpp
 * \ingroup base
 * \brief	»ôµê¸ó±ã¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "Plex.h"
#include "Template.h"

namespace CVLib
{

Plex* Plex::Create(Plex*& head, UINT nMax, UINT cbElement)
{
	Plex* pNewPlex = (Plex*)malloc(nMax*cbElement+sizeof(Plex));
	pNewPlex->pNext = NULL;

	Plex* pLastPlex=head;
	if (pLastPlex)
	{
		while (pLastPlex->pNext)
			pLastPlex=pLastPlex->pNext;
		pLastPlex->pNext=pNewPlex;
	}
	else
		head=pNewPlex;

	return pNewPlex;
}
	
void Plex::FreeDataChain()
{
	Array<Plex*> plexAray;
	Plex* pLastPlex=this;
	while (pLastPlex->pNext)
	{
		pLastPlex=pLastPlex->pNext;
		plexAray.Add(pLastPlex);
	}
	for (int i=0; i<plexAray.GetSize(); i++)
		free((void*)plexAray[i]);
	free(this);
}

}
