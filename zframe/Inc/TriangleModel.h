#ifndef _TRIANGLEMODEL_H_
#define _TRIANGLEMODEL_H_

#include "cvlibbase/inc/PtrArray.h"

namespace CVLib {

struct Triangle
{
	int n1;
	int n2;
	int n3;
};

class CVLIB_DECLSPEC TriangleModel : public Object
{
public:
	TriangleModel ();
	virtual ~TriangleModel ();

	struct SEdge
	{
		int nFrom;
		int nTo;
	} *m_pEdges;
	
	int		m_nNum;
public:
	class CVLIB_DECLSPEC TriangleList : public PtrArray
	{
	public:
		int FindTriangle(int n1, int n2, int n3)
		{
			int i;
			for (i = 0; i < GetSize(); i ++)
			{
				Triangle* pTriangle = (Triangle*)GetAt(i);
				if (pTriangle->n1 == n1 && pTriangle->n2 == n2 && pTriangle->n3 == n3)
					break;
				if (pTriangle->n1 == n1 && pTriangle->n2 == n3 && pTriangle->n3 == n2)
					break;
				if (pTriangle->n1 == n2 && pTriangle->n2 == n1 && pTriangle->n3 == n3)
					break;
				if (pTriangle->n1 == n2 && pTriangle->n2 == n3 && pTriangle->n3 == n1)
					break;
				if (pTriangle->n1 == n3 && pTriangle->n2 == n1 && pTriangle->n3 == n2)
					break;
				if (pTriangle->n1 == n3 && pTriangle->n2 == n2 && pTriangle->n3 == n1)
					break;
			}
			if (i == GetSize())
				return 0;
			else
				return 1;
		}
		void RemoveAllTriangle()
		{
			for (int i = 0 ; i < GetSize() ; i ++)
			{
				delete (Triangle*)GetAt(i);
			}
			RemoveAll();
		}
	};
	TriangleList	m_TriangleList;
	
	int		Load (FILE* pFile);
	int		Save (FILE* pFile);
	int		Create(int nNum);
	void	Release();

	void	GenerateTriangles();
	void	AdjacentTriangles (int nVertex, TriangleList* pList);

	int		LoadTriangleList (FILE* pFile);
	int		SaveTriangleList (FILE* pFile);
	
protected:
		int		FindEdge(int nFrom, int nTo);
};

}

#endif // _TRIANGLEMODEL_H_
