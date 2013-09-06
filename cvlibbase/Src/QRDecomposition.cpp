/*!
 * \file	QRDecomposition.cpp
 * \ingroup math
 * \brief	QR¹¤ÃÍ¶¦ ¼EÔÂ×²E¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "QRDecomposition.h"

namespace CVLib
{

static	double Hypot (double a, double b)
{
	double r;
	if (fabs(a) > fabs(b)) 
	{
		r = b/a;
		r = fabs(a) * sqrt(1 + r * r);
	} 
	else if (b != 0) 
	{
		r = a/b;
		r = fabs(b) * sqrt(1 + r * r);
	} 
	else 
	{
		r = 0.0;
	}
	return r;
}

QRDecomposition::QRDecomposition(Mat* pA, Matd* pmQR/*=NULL*/)
{
	m_pmQR = new Mat (*pA);
	// Initialize.
	QR = m_pmQR->data.db;
	m = m_pmQR->Rows();
	n = m_pmQR->Cols();
	Rdiag = new double[n];
	
	// Main loop.
	for (int k = 0; k < n; k++)
	{
		// Compute 2-norm of k-th column without under/overflow.
		double nrm = 0;
		for (int i = k; i < m; i++)
		{
			nrm = Hypot(nrm, QR[i][k]);
		}

		if (nrm != 0.0)
		{
			// Form k-th Householder vector.
			if (QR[k][k] < 0)
			{
				nrm = - nrm;
			}
			for (int i = k; i < m; i++)
			{
				QR[i][k] /= nrm;
			}
			QR[k][k] += 1.0;
			
			// Apply transformation to remaining columns.
			for (int j = k + 1; j < n; j++)
			{
				int i;
				double s = 0.0;
				for (i = k; i < m; i++)
				{
					s += QR[i][k] * QR[i][j];
				}
				s = (- s) / QR[k][k];
				for (i = k; i < m; i++)
				{
					QR[i][j] += s * QR[i][k];
				}
			}
		}
		Rdiag[k] = - nrm;
	}
}

QRDecomposition::~QRDecomposition()
{
	ReleaseMat(m_pmQR);
	if (Rdiag)
	{
		delete []Rdiag;
		Rdiag = NULL;
	}
}

bool QRDecomposition::FullRank()
{
	for (int j = 0; j < n; j++)
	{
		if (Rdiag[j] == 0)
			return false;
	}
	return true;
}

Mat* QRDecomposition::H()
{
	Mat* X = new Mat(m, n, MAT_Tdouble);
	double** H = X->data.db;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i >= j)
			{
				H[i][j] = QR[i][j];
			}
			else
			{
				H[i][j] = 0.0;
			}
		}
	}
	return X;
}

Mat* QRDecomposition::R()
{
	Mat* X = new Mat(n, n, MAT_Tdouble);
	double** R = X->data.db;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i < j)
			{
				R[i][j] = QR[i][j];
			}
			else if (i == j)
			{
				R[i][j] = Rdiag[i];
			}
			else
			{
				R[i][j] = 0.0;
			}
		}
	}
	return X;
}

Mat* QRDecomposition::Q()
{
	Mat* X = new Mat(m, n, MAT_Tdouble);
	double** Q = X->data.db;
	int i, j, k;
	for (k = n - 1; k >= 0; k--)
	{
		for (i = 0; i < m; i++)
		{
			Q[i][k] = 0.0;
		}
		Q[k][k] = 1.0;
		for (j = k; j < n; j++)
		{
			if (QR[k][k] != 0)
			{
				double s = 0.0;
				for (i = k; i < m; i++)
				{
					s += QR[i][k] * Q[i][j];
				}
				s = (- s) / QR[k][k];
				for (i = k; i < m; i++)
				{
					Q[i][j] += s * QR[i][k];
				}
			}
		}
	}
	return X;
}

Mat* QRDecomposition::Solve(Mat* pB)
{
	if (pB->Rows() != m)
	{
		return NULL;
		//assert (FALSE);//"Mat row dimensions must agree."
	}
	if (!this->FullRank())
	{
		return NULL;
		//assert (FALSE);//("Matrix is rank deficient.");
	}
	
	// Copy right hand side
	int nx = pB->Cols();
	Mat* pmRet = new Mat (n, nx, MAT_Tdouble);
	double** X = pmRet->data.db;
	
	// Compute Y = transpose(Q)*B
	int i, j, k;
	for (k = 0; k < n; k++)
	{
		for (int j = 0; j < nx; j++)
		{
			double s = 0.0;
			for (i = k; i < m; i++)
			{
				s += QR[i][k] * X[i][j];
			}
			s = (- s) / QR[k][k];
			for (i = k; i < m; i++)
			{
				X[i][j] += s * QR[i][k];
			}
		}
	}
	// Solve R*X = Y;
	for (k = n - 1; k >= 0; k--)
	{
		for (j = 0; j < nx; j++)
		{
			X[k][j] /= Rdiag[k];
		}
		for (i = 0; i < k; i++)
		{
			for (j = 0; j < nx; j++)
			{
				X[i][j] -= X[k][j] * QR[i][k];
			}
		}
	}

	//return (new Mat(X, n, nx).GetMatrix(0, n - 1, 0, nx - 1));
	return pmRet;
}

}
