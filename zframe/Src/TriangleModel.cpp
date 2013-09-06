#include "TriangleModel.h"
#include "cvlibbase/inc/cvlibMalloc.h"

namespace CVLib
{

TriangleModel::TriangleModel()
{
	m_pEdges = NULL;
	m_nNum = NULL;
}
	
TriangleModel::~TriangleModel()
{
	Release();
}

int	TriangleModel::Load (FILE* pFile)
{
	int nRead = 0;
	nRead = fread (&m_nNum, sizeof(m_nNum), 1, pFile);
	if (nRead == 0)	return 0;
	Create (m_nNum);
	nRead = fread (m_pEdges, sizeof(m_pEdges[0]), m_nNum, pFile);
	if (nRead == 0) return 0;
	GenerateTriangles ();
	return 1;
}

int	TriangleModel::Save (FILE* pFile)
{
	fwrite (&m_nNum, sizeof(m_nNum), 1, pFile);
	fwrite (m_pEdges, sizeof(m_pEdges[0]), m_nNum, pFile);
	return 1;
}

int	TriangleModel::Create(int nNum)
{
	m_nNum = nNum;
	MALLOC (m_pEdges, sizeof(m_pEdges[0]) * m_nNum, SEdge);
	return 1;
}

void	TriangleModel::Release()
{
	if (m_pEdges)
		FREE (m_pEdges);
	m_pEdges = NULL;

	m_TriangleList.RemoveAllTriangle();
	m_nNum = 0;
}

void	TriangleModel::GenerateTriangles()
{
	m_TriangleList.RemoveAllTriangle();

	/* ¡¡E¡¡¼³¼°¡¡EÊÐ¡¡E*/
	int	nTotalNum = 0;
	int i;
	for (i = 0; i < m_nNum; i ++)
	{
		nTotalNum = MAX(m_pEdges[i].nFrom, nTotalNum);
		nTotalNum = MAX(m_pEdges[i].nTo, nTotalNum);
	}
	nTotalNum ++;
	/* ·ô ¼³¼°Ëæ ¡¡EÑ§¡¡ËËË¯ÂÙ ¼³¼°´ÉË¾ ÊÐ²÷³Þ. */
	int	nNeighborNum = 0;
	int* pnNeighborVertexs;
	MALLOC (pnNeighborVertexs, sizeof(pnNeighborVertexs[0]) * m_nNum, int);
	for (int iVertex = 0; iVertex < nTotalNum; iVertex ++)
	{
		nNeighborNum = 0;
		for (i = 0; i < m_nNum; i ++)
		{
			if (m_pEdges[i].nFrom == iVertex)
			{
				int k;
				for (k=0; k<nNeighborNum; k ++)
				{
					if (pnNeighborVertexs[k] == m_pEdges[i].nTo)
						break;
				}
				if (k == nNeighborNum)
				{
					pnNeighborVertexs[nNeighborNum] = m_pEdges[i].nTo;
					nNeighborNum ++;
				}
			}
			else if (m_pEdges[i].nTo == iVertex)
			{
				int k;
				for (k=0; k<nNeighborNum; k ++)
				{
					if (pnNeighborVertexs[k] == m_pEdges[i].nFrom)
						break;
				}
				if (k == nNeighborNum)
				{
					pnNeighborVertexs[nNeighborNum] = m_pEdges[i].nFrom;
					nNeighborNum ++;
				}
			}
		}
		for (i = 0; i < nNeighborNum - 1; i ++)
		{
			for (int k = i + 1; k < nNeighborNum; k ++)
			{
				if (FindEdge(pnNeighborVertexs[i], pnNeighborVertexs[k]))
				{
					int nFind = m_TriangleList.FindTriangle(iVertex, pnNeighborVertexs[i], pnNeighborVertexs[k]);
					if (nFind == 0)
					{
						Triangle* pTriangle = new Triangle;
						pTriangle->n1 = iVertex;
						pTriangle->n2 = pnNeighborVertexs[i];
						pTriangle->n3 = pnNeighborVertexs[k];
						m_TriangleList.Add(pTriangle);
					}
				}
			}
		}
	}
	FREE (pnNeighborVertexs);
}

int TriangleModel::FindEdge(int nFrom, int nTo)
{
	int i;
	for (i = 0; i < m_nNum; i ++)
	{
		if (m_pEdges[i].nFrom == nFrom && m_pEdges[i].nTo == nTo)
			break;
		if (m_pEdges[i].nTo == nFrom && m_pEdges[i].nFrom == nTo)
			break;
	}
	if (i == m_nNum)
		return 0;
	else
		return 1;
}

void	TriangleModel::AdjacentTriangles (int nVertex, TriangleList* pList)
{
	pList->RemoveAll();
	for (int i = 0; i < m_TriangleList.GetSize(); i ++)
	{
		Triangle* pTriangle = (Triangle*)m_TriangleList.GetAt(i);
		if (pTriangle->n1 == nVertex || 
			pTriangle->n2 == nVertex ||
			pTriangle->n3 == nVertex)
		{
			pList->Add(pTriangle);
		}
	}
}

int	TriangleModel::LoadTriangleList (FILE* pFile)
{
	int nNum;
	fread (&nNum, sizeof(nNum), 1, pFile);
	for (int i = 0; i < nNum; i ++)
	{
		Triangle* pTriangle = new Triangle;
		fread (pTriangle, sizeof(Triangle), 1, pFile);
		m_TriangleList.Add(pTriangle);
	}
	m_pEdges = NULL;
	m_nNum = 0;
	return 1;
}

int	TriangleModel::SaveTriangleList (FILE* pFile)
{
	int nNum = m_TriangleList.GetSize();
	fwrite (&nNum, sizeof(nNum), 1, pFile);
	for (int i = 0; i < nNum; i ++)
	{
		Triangle* pTriangle = (Triangle*)m_TriangleList.GetAt(i);
		fwrite (pTriangle, sizeof(Triangle), 1, pFile);
	}
	return 1;
}


}
