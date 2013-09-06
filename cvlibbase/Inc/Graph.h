/*!
 * \file	Tree.h
 * \ingroup base
 * \brief	Tree¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _Graph_H_
#define _Graph_H_

#include "PtrArray.h"
#include "template.h"

namespace CVLib
{

class Vertex;
class Graph;

class CVLIB_DECLSPEC Vertex
{
public:
	Vertex() { }
	virtual ~Vertex() { }
	void AddEdge (Vertex* pV) {m_Edges.Add(pV);}
	Graph* Parent() {return m_pParent;}

	int NeighborCount() {return m_Edges.GetSize();}
	Vertex* Neighbor(int l) {return (Vertex*)m_Edges[l];}

	void Reset() {m_Edges.RemoveAll();}
	bool IsConnected(Vertex* pV);

protected:
	PtrArray m_Edges;
private:
	Graph*	m_pParent;
	friend class Graph;
};

class CVLIB_DECLSPEC Graph
{
public:
	Graph(){m_fOrderBuild=false;}
	virtual ~Graph(){Detach();}

	Graph& Add (Vertex* pV) {m_Vertexs.Add(pV); return *this;}
	Vertex* GetVertex(int l) const {return (Vertex*)m_Vertexs[l];}
	Vertex* GetOrderVertex(int l) const {return (Vertex*)m_OrderVs[l];}
	int Size() const {return m_Vertexs.GetSize();}
	void Detach() {m_Vertexs.RemoveAll();}

	bool BuildOrder();
protected:
	PtrArray m_Vertexs;
	PtrArray m_OrderVs; // from source to destination
	
	bool m_fOrderBuild;
};

}

#endif //_Graph_H_