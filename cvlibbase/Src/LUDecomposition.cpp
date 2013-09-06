/*! 
 * \file LUDecomposition.cpp
 * \ingroup math
 * \brief LU¹¤ÃÍ¶¦ ¼EÔÂ×²E¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */

#include "LUDecomposition.h"

namespace CVLib
{
	
LUDecomposition::LUDecomposition(const Mat* pA, Mat* pmLU/* = NULL*/)
{
	// Use a "left-looking", dot-product, Crout/Doolittle algorithm.
	m_pmLU = new Mat (*pA);
	LU = m_pmLU->data.db;
	m = m_pmLU->Rows();
	n = m_pmLU->Cols();
	piv = new int[m];
	int i, j;
	for (i = 0; i < m; i++)
	{
		piv[i] = i;
	}
	pivsign = 1;
	double* LUrowi;
	double* LUcolj = new double[m];
	
	// Outer loop.
	
	for (j = 0; j < n; j++)
	{
		
		// Make a copy of the j-th column to localize references.
		
		for (i = 0; i < m; i++)
		{
			LUcolj[i] = LU[i][j];
		}
		
		// Apply previous transformations.
		
		for (i = 0; i < m; i++)
		{
			LUrowi = LU[i];
			
			// Most of the time is spent in the following dot product.
			
			int kmax = MIN(i, j);
			double s = 0.0;
			for (int k = 0; k < kmax; k++)
			{
				s += LUrowi[k] * LUcolj[k];
			}
			
			LUrowi[j] = LUcolj[i] -= s;
		}
		
		// Find pivot and exchange if necessary.
		
		int p = j;
		for (i = j + 1; i < m; i++)
		{
			if (fabs(LUcolj[i]) > fabs(LUcolj[p]))
			{
				p = i;
			}
		}
		if (p != j)
		{
			for (int k = 0; k < n; k++)
			{
				double t = LU[p][k]; LU[p][k] = LU[j][k]; LU[j][k] = t;
			}
			int k2 = piv[p]; piv[p] = piv[j]; piv[j] = k2;
			pivsign = - pivsign;
		}
		
		// Compute multipliers.
		
		if (j < m && LU[j][j] != 0.0)
		{
			for (int i = j + 1; i < m; i++)
			{
				LU[i][j] /= LU[j][j];
			}
		}
	}
	if (LUcolj)
	{
		delete []LUcolj;
	}
}

LUDecomposition::~LUDecomposition()
{
	ReleaseMat (m_pmLU);
	if (piv)
	{
		delete []piv;
		piv = NULL;
	}
}

bool LUDecomposition::IsNonSingular()
{
	for (int j = 0; j < n; j++)
	{
		if (LU[j][j] == 0)
			return false;
	}
	return true;
}

Mat* LUDecomposition::L()
{
	Mat* X = new Mat(m, n, MAT_Tdouble);
	double** L = X->data.db;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i > j)
			{
				L[i][j] = LU[i][j];
			}
			else if (i == j)
			{
				L[i][j] = 1.0;
			}
			else
			{
				L[i][j] = 0.0;
			}
		}
	}
	return X;
}

Mat* LUDecomposition::U()
{
	Mat* X = new Mat(n, n, MAT_Tdouble);
	double** U = X->data.db;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i <= j)
			{
				U[i][j] = LU[i][j];
			}
			else
			{
				U[i][j] = 0.0;
			}
		}
	}
	return X;
}

int* LUDecomposition::Pivot()
{
	int* p = new int[m];
	for (int i = 0; i < m; i++)
	{
		p[i] = piv[i];
	}
	return p;
}

double* LUDecomposition::DoublePivot()
{
	double* vals = new double[m];
	for (int i = 0; i < m; i++)
	{
		vals[i] = (double) piv[i];
	}
	return vals;
}

double LUDecomposition::Determinant()
{
	if (m != n)
	{
		assert (FALSE);//("Matrix must be square.");
	}
	double d = (double) pivsign;
	for (int j = 0; j < n; j++)
	{
		d *= LU[j][j];
	}
	return d;
}

Mat* LUDecomposition::Solve(Mat* pB)
{
	if (pB->Rows() != m)
	{
		assert (FALSE);//("Matrix row dimensions must agree.");
		return 0;
	}
	if (!this->IsNonSingular())
	{
//		assert (FALSE);//("Matrix is singular.");
		return 0;
	}
	
	// Copy right hand side with pivoting
	int nx = pB->Cols();
	Mat* Xmat = new Mat (m, nx, MAT_Tdouble);

	for (int iRow = 0 ; iRow < m ; iRow ++)
	{
		int nPiv = piv[iRow];
		assert (nPiv > -1 && nPiv < m);
		for (int iCol = 0; iCol < nx; iCol ++)
			Xmat->data.db[iRow][iCol] = pB->data.db[nPiv][iCol];
	}

	double** X = Xmat->data.db;
	
	// Solve L*Y = B(piv,:)
	int i, j, k;
	for (k = 0; k < n; k++)
	{
		for (i = k + 1; i < n; i++)
		{
			for (j = 0; j < nx; j++)
			{
				X[i][j] -= X[k][j] * LU[i][k];
			}
		}
	}
	// Solve U*X = Y;
	for (k = n - 1; k >= 0; k--)
	{
		for (j = 0; j < nx; j++)
		{
			X[k][j] /= LU[k][k];
		}
		for (i = 0; i < k; i++)
		{
			for (j = 0; j < nx; j++)
			{
				X[i][j] -= X[k][j] * LU[i][k];
			}
		}
	}
	return Xmat;
}

}
