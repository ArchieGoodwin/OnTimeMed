/*!
 * \file	MatLib.cpp
 * \ingroup base
 * \brief	ÃÔµÛÌ© °ÖË±°ª±á °ÖË±¹ÖÀË¶£ ´Ð Êàº¤Ë¾ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "MatLib.h"
#include "cvlibmalloc.h"

namespace CVLib
{

static const double EPSILON = 1e-20;			/**< ¾×ºÏ°ª¸áÌ¡ */
static const float PIE = 3.14159265358793f;		/**< Ìº¼Ñµü */
static const int	EIGEN_ITERATION = 77777;	/**< °ÖË±°ª±Ëº¤ ¾×´Ý¸É¸ôºã */
static const float	MAX_FLOAT_VALUE = 1e+15f;	/**< float¾×´Ý°ª */
static const double	MAX_DOUBLE_VALUE = 1e+30f;	/**< double */

int ComputeInverseMatrix ( int nDim, float** ppdMatrix )
{
	int			i, j, k;
	float**	ppdB;
	float**	ppdX;

	/* Making Unit Matrix.	 */

	MALLOC2 (ppdB, nDim, nDim, float)
	for ( i = 0; i < nDim; i ++ )
	{
		for ( j = 0; j < nDim; j ++ )
		{
			if ( i - j )
				ppdB [ i ][ j ] = 0;
			else
				ppdB [ i ][ i ] = 1;
		}
	}

	/*	Calculation Loop.	 */
	for ( k = 0; k < nDim; k ++ )
	{
		if ( ppdMatrix [ k ][ k ] < EPSILON )
		{
			float	Exch;

			i = k;
			while ( fabs ( ppdMatrix [ i ][ k ] ) < EPSILON && i < nDim )
			{
				i ++;
				if ( i == nDim )
				{
					FREE2 (ppdB);
					return 0;//false;
				}
			}

			for ( j = k; j < nDim; j ++ )
			{
				Exch = ppdMatrix [ k ][ j ];
				ppdMatrix [ k ][ j ] = ppdMatrix [ i ][ j ];
				ppdMatrix [ i ][ j ] = Exch;
			}

			for ( j = 0; j < nDim; j ++ )
			{
				Exch = ppdB [ k ][ j ];
				ppdB [ k ][ j ] = ppdB [ i ][ j ];
				ppdB [ i ][ j ] = Exch;
			}
		}

		for ( j = 0; j < nDim; j ++ )
			ppdB [ k ][ j ] /= ppdMatrix [ k ][ k ];

		for ( j = k + 1; j < nDim; j ++ )
			ppdMatrix [ k ][ j ] /= ppdMatrix [ k ][ k ];

		for ( i = k + 1; i < nDim; i ++ )
			for ( j = 0; j < nDim; j ++ )
				ppdB [ i ][ j ] -= ppdMatrix [ i ][ k ] * ppdB [ k ][ j ];

		for ( i = k + 1; i < nDim; i ++ )
			for ( j = k + 1; j < nDim; j ++ )
				ppdMatrix [ i ][ j ] -= ppdMatrix [ i ][ k ] * ppdMatrix [ k ][ j ];
	}

	MALLOC2 (ppdX, nDim, nDim, float);
	for ( k = 0; k < nDim; k ++ )
	{
		for ( i = nDim - 1; i >= 0; i -- )
		{
			ppdX [ i ][ k ] = ppdB [ i ][ k ];
			for ( j = nDim - 1; j >= i + 1; j -- )
				ppdX [ i ][ k ] -= ppdX [ j ][ k ] * ppdMatrix [ i ][ j ];
		}
	}

	for ( i = 0; i < nDim; i ++ )
		for ( j = 0; j < nDim; j ++ )
			ppdMatrix [ i ][ j ] = ppdX [ i ][ j ];

	FREE2 (ppdX);
	FREE2 (ppdB);

	return 1;//true;
}

int ComputeInverseMatrix ( int nDim, double** ppdMatrix )
{
	int			i, j, k;
	double**	ppdB;
	double**	ppdX;

	/* Making Unit Matrix.	 */

	MALLOC2 (ppdB, nDim, nDim, double)
	for ( i = 0; i < nDim; i ++ )
	{
		for ( j = 0; j < nDim; j ++ )
		{
			if ( i - j )
				ppdB [ i ][ j ] = 0;
			else
				ppdB [ i ][ i ] = 1;
		}
	}

	/*	Calculation Loop.	 */
	for ( k = 0; k < nDim; k ++ )
	{
		if ( ppdMatrix [ k ][ k ] < EPSILON )
		{
			double	Exch;

			i = k;
			while ( fabs ( ppdMatrix [ i ][ k ] ) < EPSILON && i < nDim )
			{
				i ++;
				if ( i == nDim )
				{
					FREE2 (ppdB);
					return 0;//false;
				}
			}

			for ( j = k; j < nDim; j ++ )
			{
				Exch = ppdMatrix [ k ][ j ];
				ppdMatrix [ k ][ j ] = ppdMatrix [ i ][ j ];
				ppdMatrix [ i ][ j ] = Exch;
			}

			for ( j = 0; j < nDim; j ++ )
			{
				Exch = ppdB [ k ][ j ];
				ppdB [ k ][ j ] = ppdB [ i ][ j ];
				ppdB [ i ][ j ] = Exch;
			}
		}

		for ( j = 0; j < nDim; j ++ )
			ppdB [ k ][ j ] /= ppdMatrix [ k ][ k ];

		for ( j = k + 1; j < nDim; j ++ )
			ppdMatrix [ k ][ j ] /= ppdMatrix [ k ][ k ];

		for ( i = k + 1; i < nDim; i ++ )
			for ( j = 0; j < nDim; j ++ )
				ppdB [ i ][ j ] -= ppdMatrix [ i ][ k ] * ppdB [ k ][ j ];

		for ( i = k + 1; i < nDim; i ++ )
			for ( j = k + 1; j < nDim; j ++ )
				ppdMatrix [ i ][ j ] -= ppdMatrix [ i ][ k ] * ppdMatrix [ k ][ j ];
	}

	MALLOC2 (ppdX, nDim, nDim, double);
	for ( k = 0; k < nDim; k ++ )
	{
		for ( i = nDim - 1; i >= 0; i -- )
		{
			ppdX [ i ][ k ] = ppdB [ i ][ k ];
			for ( j = nDim - 1; j >= i + 1; j -- )
				ppdX [ i ][ k ] -= ppdX [ j ][ k ] * ppdMatrix [ i ][ j ];
		}
	}

	for ( i = 0; i < nDim; i ++ )
		for ( j = 0; j < nDim; j ++ )
			ppdMatrix [ i ][ j ] = ppdX [ i ][ j ];

	FREE2 (ppdX);
	FREE2 (ppdB);

	return 1;//true;
}

void EigenJacobian(float** pparData, int nDim, float* parEigenValue, float** pparVector,
	  int nMaxCount)
{
	int i, j, k, p, q;
	float d, eps, rTheta, s, c;
	float rOldapj, rOldaqj, rOldaip, rOldaiq, rOldvip, rOldviq;
	float** pparTemp;
	float rTemp, rTemp1;
	int iPos;
	/* »ôµê¸ôº¡ */
	MALLOC2 (pparTemp, nDim, nDim, float);
	for (i = 0; i < nDim; i++)
	for (j = 0; j < nDim; j++)
		pparTemp[i][j] = pparData[i][j];

	/* °ÖË±¹ÖÀË¶£ ½ó±¨Ãù */
	for (i = 0; i < nDim; i++)
	for (j = 0; j < nDim; j++)
		pparVector[i][j] = (i==j)?(1.0f):(0.0f);

	/* ÃÔµÛËæº· ¼®´Ý°ªËË °¡»þ ¿Ï ÌººÏÌ© °ª °Î¼³ */
	d = 0.0f;
	for (i = 0; i < nDim; i++)
	for (j = 0; j < nDim; j++)
	{
		d = (fabs((pparTemp[i][j])) >d) ? (float)(fabs((pparTemp[i][j]))) : (d);
	}
	/* ÂâË¦Êï½Ó °Î¼³ */
	eps = d * (float)1.0e-7;

	/* ¸É¸ôÃäºã °Î¼³ */
	nMaxCount = (nMaxCount >= nDim*nDim) ? (nMaxCount):(nDim*nDim); 
	for (k = 0; k < nMaxCount; k++)
	{
		/* ÃÔµÛËæº· ¼®´Ý°ªËË °¡»þ ¿Ï ÌººÏÌ© index °Î¼³ */
		p = 0;	q= 1;
		for(i = 0; i < nDim - 1; i++)	for(j = i + 1; j < nDim; j++)
		{
			if (fabs(pparTemp[i][j]) > fabs(pparTemp[p][q]))
			{
					p = i;	q = j;
			}
		}
		/* ÂâË¦Êï½Ó¸ó³Þ »õËº·² ±Ëº¤Äù */
		if (fabs(pparTemp[p][q]) < eps) break;

		if (fabs(pparTemp[q][q]-pparTemp[p][p]) < eps)
			rTheta = (float)(PIE / 4.0);//0.7853981634
		else 
			rTheta = 0.5f * (float)atan(2.0 * pparTemp[p][q] / (pparTemp[q][q] - pparTemp[p][p]));

		/* °ÖË±¹ÖÀË¶£ ±Ëº¤ */
		s =  (float)sin(rTheta);	c = (float)cos(rTheta);
		for (j = 0; j < nDim; j++)
		{
			rOldapj = pparTemp[p][j];
			rOldaqj = pparTemp[q][j];
			pparTemp[p][j] = rOldapj * c - rOldaqj * s;
			pparTemp[q][j] = rOldapj * s + rOldaqj * c;
		}
		for (i = 0; i < nDim; i++)
		{
			rOldaip = pparTemp[i][p];
			rOldaiq = pparTemp[i][q];
			pparTemp[i][p] = rOldaip * c - rOldaiq * s;
			pparTemp[i][q] = rOldaip * s + rOldaiq * c;
			rOldvip = pparVector[i][p];
			rOldviq = pparVector[i][q];
			pparVector[i][p] = rOldvip * c - rOldviq * s;
			pparVector[i][q] = rOldvip * s + rOldviq * c;
		}
	}

	for (i = 0; i < nDim; i++)
		parEigenValue[i] = pparTemp[i][i];

	/*----------------------Ordering----------------------*/
	for (i = 0; i < nDim; i++)
	{
		iPos = i;
		rTemp = (float)fabs (parEigenValue[i]);
		for (j = i + 1; j < nDim; j ++)
		{
			rTemp1 = (float)fabs (parEigenValue[j]);
			iPos = (rTemp < rTemp1) ? j : iPos;
			rTemp = (rTemp < rTemp1) ? rTemp1 : rTemp;
		}
		if (iPos != i)
		{
			rTemp = parEigenValue[i];
			parEigenValue[i] = parEigenValue[iPos];
			parEigenValue[iPos] = rTemp;
			for (j = 0; j < nDim; j ++)
			{
				rTemp = pparVector[i][j];
				pparVector[i][j] = pparVector[iPos][j];
				pparVector[iPos][j] = rTemp;
			}
		}
	}
	FREE2 (pparTemp);
}

void EigenPower (float** pprData, int nDim, float* prEigenvalue, float** pprVector,
			int nNumEigen)
{
	int i, j, k, k_1;
	double** ppdMat;
	double** ppdEigen;
	double*	pdEigenVal;

	MALLOC2 (ppdMat, nDim, nDim, double);
	MALLOC2 (ppdEigen, nDim, nDim, double);
	MALLOC (pdEigenVal, nDim * sizeof(double), double);

	for (i = 0; i < nDim ; i ++)
	for (j = 0; j < nDim ; j ++)
		ppdMat[i][j] = (double)pprData[i][j];

	for (k = 0; k < nNumEigen; k ++)
	{
		if (k)
		{
			k_1 = k - 1;
			for (i = 0; i < nDim; i ++)
			for (j = 0; j < nDim; j ++)
			{
				ppdMat[i][j] = ppdMat[i][j] - 
					pdEigenVal[k_1] * ppdEigen[k_1][i] * ppdEigen[k_1][j];
			}
		}
		pdEigenVal[k] = FindingLargestEigenValue (ppdMat, ppdEigen[k], nDim);
	}

	for (i = 0; i < nNumEigen ; i ++)
	for (j = 0; j < nDim ; j ++)
		pprVector[i][j] = (float)ppdEigen[i][j];
	for (k = 0; k < nNumEigen; k ++)
		prEigenvalue[k] = (float)pdEigenVal[k];

	FREE2 (ppdMat);
	FREE2 (ppdEigen);
	FREE (pdEigenVal);
}

float FindMaxEigenvalue (float** pprData, int nDim, float* prVector)
{
	float	rLamda = 0.0f;
	float*	prY_1;			//	Dimention	1 * N	( k - 1 )th eigen vector
	float*	prY;		//	Dimention	1 * N	kth eigen vector
	float	rMinDiff;
	float	rLamdaK = 0;
	float	rRet = 0.0f;
	int		mlIteration;
	int		i, j, k;
	float	rTemp, rMaxElem = 0, rSquareSum;

	//	Allocation and computing of initial eigen vector and 
	//	computing of initial eigen value. 
	MALLOC (prY_1, sizeof(float) * nDim, float)

	//	½ó±¨°ÖË±¹ÖÀË¶£±Ëº¤
	for ( i = 0; i < nDim; i++ )
	{
		rTemp = 0;

		for ( j = 0; j < nDim; j++ )
			rTemp += pprData [ i ][ j ];
		prY_1 [ i ] = rTemp;
		rLamda += rTemp;
	}
	rLamda /= nDim;		// ½ó±¨°ÖË±°ª±Ëº¤
	rMinDiff = (float)fabs ( rLamda );	//	°ÖË±°ª´Éº¡ËËÌ© ½ÓËË(ºãµÜºÂÁÆ³àËæ ¶®Ë¦)

	//	Allocation of power matrix. 
	MALLOC (prY, sizeof(float) * nDim, float);

	memset ( prVector, 0, sizeof ( float ) * nDim );

	//	Iteration of proceeding computation.
	for ( mlIteration = 0; mlIteration < EIGEN_ITERATION; mlIteration++ )
	{
		rMaxElem = 0.0f;

		//	Computing of kth power matrix.
		rLamdaK = 0;
		for ( i = 0; i < nDim; i++ )
		{
			rTemp=0.0f;
			for (k=0; k<nDim; k++)
				rTemp += pprData[i][k]*prY_1[k];

			prY [ i ] = rTemp;

			if ( prY_1 [ i ] )
				rLamdaK += rTemp / prY_1 [ i ];

			rTemp = (float)fabs ( rTemp );
			if ( rMaxElem < rTemp )
				rMaxElem = rTemp;
		}
		rLamdaK /= nDim;

		if ( rMaxElem > MAX_FLOAT_VALUE/*MAX_FLOAT_VALUE*/ )
		{
			for ( i = 0; i < nDim; i++ )
				prY [ i ] /= rMaxElem;
		}

		//	Evaluation of convergence.
		if ( fabs ( rLamda - rLamdaK ) < rMinDiff )
		{
			rMinDiff = (float)fabs ( rLamda - rLamdaK );
			rRet = rLamdaK;
			memcpy ( prVector, prY, sizeof ( float ) * nDim );

			if ( rMinDiff < EPSILON )
				break;
		}

		//	Preparing of next iteration.
		rLamda = rLamdaK;
		memcpy ( prY_1, prY, sizeof ( float ) * nDim );
	}

	//	Normalization of eigenvector.
	{
		rTemp=0.0f;
		for (k=0; k<nDim; k++)
			rTemp += prVector[k]*prVector[k];
		rSquareSum = sqrtf ( rTemp );

		if ( rSquareSum )
		{
			for ( i = 0; i < nDim; i++ )
				prVector [ i ] = prVector [ i ] / rSquareSum;
		}
	}

	FREE (prY_1);
	FREE (prY);

	return rRet;
}

double FindingLargestEigenValue ( double** ppdMatrix, double* pdEigenVector, int sN )
{
	double*	Ay;			//	Dimention	1 * N	( k - 1 )th eigen vector
	double*	AAy;		//	Dimention	1 * N	kth eigen vector
	double*	mpdMin;		//	Dimention	1 * N	kth eigen vector
	double	mdMinDifference;
	double	mdLamda = 0;
	double	lamda = 0;
	double	lamdaK = 0;
	long	mlIteration;
	int	i, j;

	//	Allocation and computing of initial eigen vector and computing of initial eigen value. 
	MALLOC (Ay, sN * sizeof(double), double);
	for ( i = 0; i < sN; i++ )
	{
		double	temp = 0;

		for ( j = 0; j < sN; j++ )
			temp += ppdMatrix [ i ][ j ];
		Ay [ i ] = temp;
		lamda += temp;
	}
	lamda /= sN;
	mdMinDifference = (double)fabs ( lamda );

	//	Allocation of power matrix. 
	MALLOC (AAy, sizeof(double) * sN, double);
	MALLOC (mpdMin, sizeof(double) * sN, double);

	memset ( pdEigenVector, 0, sizeof ( double ) * sN );

	//	Iteration of proceeding computation.
	for ( mlIteration = 0; mlIteration < EIGEN_ITERATION; mlIteration++ )
	{
		double	maxElement = 0;

		//	Computing of kth power matrix.
		lamdaK = 0;
		for ( i = 0; i < sN; i++ )
		{
			double	temp = 0;

			for ( j = 0; j < sN; j++ )
				temp += ppdMatrix [ i ][ j ] * Ay [ j ];
			AAy [ i ] = temp;

			if ( Ay [ i ] )
				lamdaK += temp / Ay [ i ];

			temp = (double)fabs ( temp );
			if ( maxElement < temp )
				maxElement = temp;
		}
		lamdaK /= sN;

		if ( maxElement > MAX_DOUBLE_VALUE )
		{
			for ( i = 0; i < sN; i++ )
				AAy [ i ] /= maxElement;
		}

		//	Evaluation of convergence.
		if ( fabs ( lamda - lamdaK ) < mdMinDifference )
		{
			mdMinDifference = (double)fabs ( lamda - lamdaK );
			mdLamda = lamdaK;
			memcpy ( mpdMin, AAy, sizeof ( double ) * sN );

			if ( mdMinDifference < EPSILON )
				break;
		}

		//	Preparing of next iteration.
		lamda = lamdaK;
		memcpy ( Ay, AAy, sizeof ( double ) * sN );
	}

	//	Normalization of eigenvector.
	{
		double	squareSum = 0;

		//	Normalizing of eigen vector.
		for ( i = 0; i < sN; i++ )
			squareSum += mpdMin [ i ] * mpdMin [ i ];
		squareSum = (double)sqrt ( squareSum );

		if ( squareSum )
		{
			for ( i = 0; i < sN; i++ )
				pdEigenVector [ i ] = mpdMin [ i ] / squareSum;
		}
	}

	FREE (Ay);
	FREE (AAy);
	FREE (mpdMin);

	return mdLamda;
}

float Determinant ( float** pparMat, int nDim )
{
	int i, j, k;
	float rRet, rMul;
	for (k = 0; k < nDim - 1; k ++)
	{
		for (i = k + 1; i < nDim ; i ++)
		{
			rMul = pparMat[i][k] / pparMat[k][k];
			for (j = k; j < nDim; j ++)
				pparMat[i][j] = pparMat[i][j] - rMul * pparMat[k][j];
		}
	}
	rRet = 1.0f;
	for (k = 0; k < nDim; k ++)
		rRet = rRet * pparMat[k][k];
	return rRet;
}

void 
EigenPower_64 (double** pprData, int nDim, double* prEigenvalue, double** pprVector,
			int nNumEigen)
{
	int i, j, k, k_1;
	double** ppdMat;
	double** ppdEigen;
	double*	pdEigenVal;

	MALLOC2 (ppdMat, nDim, nDim, double);
	MALLOC2 (ppdEigen, nDim, nDim, double);
	MALLOC (pdEigenVal, nDim * sizeof(double), double);

	for (i = 0; i < nDim ; i ++)
	for (j = 0; j < nDim ; j ++)
		ppdMat[i][j] = pprData[i][j];

	for (k = 0; k < nNumEigen; k ++)
	{
		if (k)
		{
			k_1 = k - 1;
			for (i = 0; i < nDim; i ++)
			for (j = 0; j < nDim; j ++)
			{
				ppdMat[i][j] = ppdMat[i][j] - 
					pdEigenVal[k_1] * ppdEigen[k_1][i] * ppdEigen[k_1][j];
			}
		}
		pdEigenVal[k] = FindingLargestEigenValue (ppdMat, ppdEigen[k], nDim);
	}

	for (i = 0; i < nNumEigen ; i ++)
	for (j = 0; j < nDim ; j ++)
		pprVector[i][j] = ppdEigen[i][j];
	for (k = 0; k < nNumEigen; k ++)
		prEigenvalue[k] = pdEigenVal[k];

	FREE2 (ppdMat);
	FREE2 (ppdEigen);
	FREE (pdEigenVal);
}

void VecMulMat (float* prA, float** pprB, float *prC, int nDim, int nCols)
{
	int i = 0;
	int j = 0;
	memset(prC, 0, nCols * sizeof(float));
	
	for (i = 0; i < nCols; i++)
		for (j = 0; j < nDim; j++)
			prC[i] += prA[j] * pprB[j][i];
}

}