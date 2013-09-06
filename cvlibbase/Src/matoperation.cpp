/*!
 * \file	MatOp.cpp
 * \ingroup base
 * \brief	ÃÔµÛÊàº¤Ë¾ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "MatOperation.h"

namespace CVLib
{

bool MatOp::CropMat (Mat* pDest, const Mat* pMat, const Rect& rect)
{
	int i;

	long startx = MAX(0L,MIN(rect.x, pMat->Cols()));
	long endx = MAX(0L,MIN(rect.x + rect.width, pMat->Cols()));
	long starty = MAX(0L,MIN(rect.y, pMat->Rows()));
	long endy = MAX(0L,MIN(rect.y + rect.height, pMat->Rows()));
	int nRows = endy-starty;
	int nCols = endx-startx;

	if (nRows <= 0 || nCols <= 0)
		return false;

	pDest->Create (nRows, nCols, pMat->Type1());

	int n=pMat->Step()*pMat->Channels();
	for (i = 0; i < nRows; i ++)
	{
		memcpy (pDest->data.ptr[i],
			(char*)(pMat->data.ptr[i + starty]) + startx * n, nCols * n);
	}

	return true;
}

void	MatOp::Sub (Mat* pA, const Mat* pB, const Mat* pC)
{
	int	nRows = pA->Rows();
	int	nCols = pA->Cols();

	assert (nRows == pB->Rows());
	assert (nCols == pB->Cols());
	assert (pB->Rows() == pC->Rows());
	assert (pB->Cols() == pC->Cols());
	assert (pA->Type() == pB->Type());
	assert (pB->Type() == pC->Type());

	int i, j;
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE** pA_ = pA->data.ptr;
			for (i = 0; i < nRows; i ++)
				for (j = 0; j < nCols; j ++)
				{
					pA_[i][j] = (BYTE)(pB->data.ptr[i][j] - pC->data.ptr[i][j]);
				}
		}
		break;
	case MAT_Tshort:
		{
			short** pA_ = pA->data.s;
			for (i = 0; i < nRows; i ++)
				for (j = 0; j < nCols; j ++)
				{
					pA_[i][j] = pB->data.s[i][j] - pC->data.s[i][j];
				}
		}
		break;
	case MAT_Tint:
		{
			int** pA_ = pA->data.i;
			for (i = 0; i < nRows; i ++)
				for (j = 0; j < nCols; j ++)
				{
					pA_[i][j] = pB->data.i[i][j] - pC->data.i[i][j];
				}
		}
		break;
	case MAT_Tfloat:
		{
			float** pA_ = pA->data.fl;
			for (i = 0; i < nRows; i ++)
				for (j = 0; j < nCols; j ++)
				{
					pA_[i][j] = pB->data.fl[i][j] - pC->data.fl[i][j];
				}
		}
		break;
	case MAT_Tdouble:
		{
			double** pA_ = pA->data.db;
			for (i = 0; i < nRows; i ++)
				for (j = 0; j < nCols; j ++)
				{
					pA_[i][j] = pB->data.db[i][j] - pC->data.db[i][j];
				}
		}
		break;
	}
}

void	MatOp::CopyMat (Mat* pDst, const Mat* pSrc, TYPE dsttype)
{
	int i, j;
	int nRows = pSrc->Rows();
	int nCols = pSrc->Cols();

	if (!pDst->IsValid())
		pDst->Create (nRows, nCols, dsttype);

	assert (pDst->Rows() == nRows);
	assert (pDst->Cols() == nCols);
	
	if (dsttype == pSrc->Type())
	{
		int nlinestep=nCols * pSrc->Step()*pSrc->Channels();
		for (int i = 0; i < nRows; i ++)
			memcpy (pDst->data.ptr[i], pSrc->data.ptr[i], nlinestep);
	}
	else
	{
		switch (pDst->Type())
		{
		case MAT_Tbyte:
			for (i = 0; i < nRows; i ++)
			for (j = 0; j < nCols; j ++)
				pDst->data.ptr[i][j] = (BYTE)pSrc->Value(i, j);
			break;
		case MAT_Tshort:
			for (i = 0; i < nRows; i ++)
			for (j = 0; j < nCols; j ++)
				pDst->data.s[i][j] = (short)pSrc->Value(i, j);
			break;
		case MAT_Tint:
			for (i = 0; i < nRows; i ++)
			for (j = 0; j < nCols; j ++)
				pDst->data.i[i][j] = (int)pSrc->Value(i, j);
			break;
		case MAT_Tfloat:
			for (i = 0; i < nRows; i ++)
			for (j = 0; j < nCols; j ++)
				pDst->data.fl[i][j] = (float)pSrc->Value(i, j);
			break;
		case MAT_Tdouble:
			for (i = 0; i < nRows; i ++)
			for (j = 0; j < nCols; j ++)
				pDst->data.db[i][j] = (double)pSrc->Value(i, j);
			break;
		}
	}
}
void MatOp::CopyMat(Mat* pDst, const Mat* pSrc, const Rect& srcRect, const Point2i& dstpt)
{
	assert(pSrc && pSrc->IsValid() && pDst);
	if (!pDst->IsValid() || (pDst->IsValid() && (pDst->Rows()<srcRect.height || pDst->Cols()<srcRect.width)) )
	{
		pDst->Release();
		pDst->Create (srcRect.height, srcRect.width, pSrc->Type1());
	}

	int nRight2, nBottom2;
	int nCopyW, nCopyH;

	int nSrcW = pSrc->Cols();
	int nSrcH = pSrc->Rows();
	BYTE** ppbSrc = pSrc->data.ptr;

	int nDstW = pDst->Cols();
	int nDstH = pDst->Rows();
	BYTE** ppbDst = pDst->data.ptr;

	assert(srcRect.x >= 0 && srcRect.x < nSrcW);
	assert(srcRect.LimX() >= 0 && srcRect.LimX() <= nSrcW);
	assert(srcRect.y >= 0 && srcRect.y < nSrcH);
	assert(srcRect.LimY() >= 0 && srcRect.LimY() <= nSrcH);

	nCopyW = srcRect.width;
	nCopyH = srcRect.height;
	nRight2 = dstpt.x + nCopyW - 1;
	nBottom2 = dstpt.y + nCopyH - 1;
	assert(dstpt.x >= 0 && dstpt.x < nDstW);
	assert(nRight2 >= 0 && nRight2 < nDstW);
	assert(dstpt.y >= 0 && dstpt.y < nDstH);
	assert(nBottom2 >= 0 && nBottom2 < nDstH);

	int ncopyline=pDst->RowByteSize();
	int noffset=srcRect.x*pSrc->Step()*pSrc->Channels();
	for (int iH = dstpt.y; iH <= nBottom2; iH++)
	{
		BYTE* pbSrcLine=ppbSrc[iH-dstpt.y+srcRect.y]+noffset;
		BYTE* pbDstLine=ppbDst[iH];
		memcpy(pbDstLine, pbSrcLine, ncopyline);
	}
}
void MatOp::CopyMat(Mat* pDst, const Mat* pSrc, int nLeft1, int nTop1, int nRight1, int nBottom1, int nLeft2, int nTop2)
{
	int nRight2, nBottom2;
	int nCopyW, nCopyH;
	assert(pSrc->IsValid());
	assert(pDst->IsValid());

	int nSrcW = pSrc->Cols();
	int nSrcH = pSrc->Rows();
	BYTE* pbSrc = pSrc->data.ptr[0];

	int nDstW = pDst->Cols();
	int nDstH = pDst->Rows();
	BYTE* pbDst = pDst->data.ptr[0];

	assert(nLeft1 >= 0 && nLeft1 < nSrcW);
	assert(nRight1 >= 0 && nRight1 < nSrcW);
	assert(nTop1 >= 0 && nTop1 < nSrcH);
	assert(nBottom1 >= 0 && nBottom1 < nSrcH);

	nCopyW = nRight1 - nLeft1 + 1;
	nCopyH = nBottom1 - nTop1 + 1;
	nRight2 = nLeft2 + nCopyW - 1;
	nBottom2 = nTop2 + nCopyH - 1;
	assert(nLeft2 >= 0 && nLeft2 < nDstW);
	assert(nRight2 >= 0 && nRight2 < nDstW);
	assert(nTop2 >= 0 && nTop2 < nDstH);
	assert(nBottom2 >= 0 && nBottom2 < nDstH);

	BYTE* pbSrcLine = pbSrc + nTop1 * nSrcW + nLeft1;
	BYTE* pbDstLine = pbDst + nTop2 * nDstW + nLeft2;
	for (int iH = nTop2; iH <= nBottom2; iH++)
	{
		memcpy(pbDstLine, pbSrcLine, nCopyW);
		pbSrcLine += nSrcW;
		pbDstLine += nDstW;
	}
}

void	MatOp::Mul (Mat* pC, const Vec* pA, const Vec* pB)
{
	assert (pA->Type() == pB->Type());
	assert (pB->Type() == pC->Type());

	assert (pA->Length() == pB->Length());
	assert (pC->Rows() == pC->Cols());
	assert (pA->Length() == pC->Cols());

	switch (pA->Type())
	{
	case MAT_Tbyte:
		return;
	case MAT_Tshort:
		{
			short**	pprC = pC->data.s;
			short*	prA = pA->data.s;
			short*	prB = pB->data.s;
			int nLen = pA->Length();
			for (int i = 0; i < nLen; i ++)
			{
				for (int j = 0; j < nLen; j ++)
				{
					pprC[i][j] = prA[i] * prB[j];
				}
			}
		}
		break;
	case MAT_Tint:
		{
			int**	pprC = pC->data.i;
			int*	prA = pA->data.i;
			int*	prB = pB->data.i;
			int nLen = pA->Length();
			for (int i = 0; i < nLen; i ++)
			{
				for (int j = 0; j < nLen; j ++)
				{
					pprC[i][j] = prA[i] * prB[j];
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			float**	pprC = pC->data.fl;
			float*	prA = pA->data.fl;
			float*	prB = pB->data.fl;
			int nLen = pA->Length();
			for (int i = 0; i < nLen; i ++)
			{
				for (int j = 0; j < nLen; j ++)
				{
					pprC[i][j] = prA[i] * prB[j];
				}
			}
		}
		break;
	case MAT_Tdouble:
		{
			double** pprC = pC->data.db;
			double*	prA = pA->data.db;
			double*	prB = pB->data.db;
			int nLen = pA->Length();
			for (int i = 0; i < nLen; i ++)
			{
				for (int j = 0; j < nLen; j ++)
				{
					pprC[i][j] = prA[i] * prB[j];
				}
			}
		}
		break;
	}
}

void MatOp::Add (Mat* pA, const Mat* pB, const Mat* pC)
{
	assert ( (pA->Rows() == pB->Rows()) && (pA->Cols() == pB->Cols()) );
	assert ( (pC->Rows() == pB->Rows()) && (pC->Cols() == pB->Cols()) );
	assert ( (pA->Type() == pB->Type()));
	assert ( (pB->Type() == pC->Type()));

	int nRows = pA->Rows(), nCols = pA->Cols();
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE**	ppbA = pA->data.ptr;
			BYTE**	ppbB = pB->data.ptr;
			BYTE**	ppbC = pC->data.ptr;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					ppbA[i][j] = ppbB[i][j] + ppbC[i][j];
				}
			}
		}
		break;
	case MAT_Tshort:
		{
			short**	pprA = pA->data.s;
			short**	pprB = pB->data.s;
			short**	pprC = pC->data.s;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					pprA[i][j] = pprB[i][j] + pprC[i][j];
				}
			}
		}
		break;
	case MAT_Tint:
		{
			int**	pprA = pA->data.i;
			int**	pprB = pB->data.i;
			int**	pprC = pC->data.i;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					pprA[i][j] = pprB[i][j] + pprC[i][j];
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			float**	pprA = pA->data.fl;
			float**	pprB = pB->data.fl;
			float**	pprC = pC->data.fl;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					pprA[i][j] = pprB[i][j] + pprC[i][j];
				}
			}
		}
		break;
	case MAT_Tdouble:
		{
			double**	pprA = pA->data.db;
			double**	pprB = pB->data.db;
			double**	pprC = pC->data.db;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					pprA[i][j] = pprB[i][j] + pprC[i][j];
				}
			}
		}
		break;
	}
}

void MatOp::Mul (Mat* pB, double rReal, const Mat* pA)
{
	assert ( (pA->Rows() == pB->Rows()) && (pA->Cols() == pB->Cols()) );

	int nRows = pA->Rows(), nCols = pA->Cols();
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE** ppbA = pA->data.ptr;
			BYTE** ppbB = pB->data.ptr;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
					ppbB[i][j] = (BYTE)(rReal * ppbA[i][j]);
			}
		}
		break;
	case MAT_Tshort:
		{
			short** ppnA = pA->data.s;
			short** ppnB = pB->data.s;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
					ppnB[i][j] = (short)(rReal * ppnA[i][j]);
			}
		}
		break;
	case MAT_Tint:
		{
			int** ppnA = pA->data.i;
			int** ppnB = pB->data.i;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
					ppnB[i][j] = (int)(rReal * ppnA[i][j]);
			}
		}
		break;
	case MAT_Tfloat:
		{
			float** pprA = pA->data.fl;
			float** pprB = pB->data.fl;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
					pprB[i][j] = (float)(rReal * pprA[i][j]);
			}
		}
		break;
	case MAT_Tdouble:
		{
			double** pprA = pA->data.db;
			double** pprB = pB->data.db;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
					pprB[i][j] = (double)(rReal * pprA[i][j]);
			}
		}
		break;
	}
}

void MatOp::Sub (Vec* pA, const Vec* pB, const Vec* pC)
{
	assert (pA->Length() == pB->Length());
	assert (pA->Length() == pC->Length());
	assert (pA->Type() == pB->Type());
	assert (pA->Type() == pC->Type());
	int i;
	int		nLen = pA->Length();
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE* pA_ = pA->data.ptr;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (BYTE)(pB->data.ptr[i] - pC->data.ptr[i]);
		}
		break;
	case MAT_Tshort:
		{
			short* pA_ = pA->data.s;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (pB->data.s[i] - pC->data.s[i]);
		}
		break;
	case MAT_Tint:
		{
			int* pA_ = pA->data.i;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (pB->data.i[i] - pC->data.i[i]);
		}
		break;
	case MAT_Tfloat:
		{
			float* pA_ = pA->data.fl;
			for (i = 0; i < nLen; i ++)
				pA_[i] = pB->data.fl[i] - pC->data.fl[i];
		}
		break;
	case MAT_Tdouble:
		{
			double* pA_ = pA->data.db;
			for (i = 0; i < nLen; i ++)
				pA_[i] = pB->data.db[i] - pC->data.db[i];
		}
		break;
	}
}

void MatOp::Mul (Vec* pA, const Mat* pB, const Vec* pC)
{
	assert ( (pA->Length() == pB->Rows()) && (pB->Cols() == pC->Length()) );
	assert ( (pA->Type() == pB->Type()));
	assert ( (pB->Type() == pC->Type()));
	assert (pA->Type() != MAT_Tbyte);

	int nRows = pB->Rows(), nCols = pB->Cols();

	pA->Zero();
	
	switch (pA->Type())
	{
	case MAT_Tshort:
		{
			short*	prA = pA->data.s;
			short**	pprB = pB->data.s;
			short*	prC = pC->data.s;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					prA[i] += pprB[i][j] * prC[j];
				}
			}
		}
		break;
	case MAT_Tint:
		{
			int*	prA = pA->data.i;
			int**	pprB = pB->data.i;
			int*	prC = pC->data.i;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					prA[i] += pprB[i][j] * prC[j];
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			float*	prA = pA->data.fl;
			float**	pprB = pB->data.fl;
			float*	prC = pC->data.fl;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					prA[i] += pprB[i][j] * prC[j];
				}
			}
		}
		break;
	case MAT_Tdouble:
		{
			double*	prA = pA->data.db;
			double** pprB = pB->data.db;
			double*	prC = pC->data.db;
			for (int i = 0; i < nRows; i ++)
			{
				for (int j = 0; j < nCols; j ++)
				{
					prA[i] += pprB[i][j] * prC[j];
				}
			}
		}
		break;
	}
}

void MatOp::Mul (Vec* pA, const Vec* pB, const Mat* pC)
{
	assert ( (pA->Length() == pC->Cols()) && (pC->Rows() == pB->Length()) );
	assert ( (pA->Type() == pB->Type()) && (pB->Type() == pC->Type()) );

	int nRows = pC->Rows(), nCols = pC->Cols();

	switch(pA->Type())
	{
	case MAT_Tfloat:
		{
			float*	prA = pA->data.fl;
			float*	prB = pB->data.fl;
			float**	pprC = pC->data.fl;
			pA->Zero();
			for (int i = 0; i < nCols; i ++)
			{
				for (int j = 0; j < nRows; j ++)
				{
					prA[i] += prB[j] * pprC[j][i];
				}
			}
		}
		break;
	case MAT_Tdouble:
		{
			double*	prA = pA->data.db;
			double* prB = pB->data.db;
			double** pprC = pC->data.db;
			pA->Zero();
			for (int i = 0; i < nCols; i ++)
			{
				for (int j = 0; j < nRows; j ++)
				{
					prA[i] += prB[j] * pprC[j][i];
				}
			}
		}
		break;
	}
}

void MatOp::Diag(Mat* pA, double rVal)
{
	int i, iEnd;

	iEnd = pA->Rows();
	if ( iEnd >  pA->Cols() )
		iEnd = pA->Cols();	
	switch (pA->Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < iEnd; i++)
			pA->data.ptr[i][i]=(unsigned char)rVal;
	case MAT_Tshort:
		for (i = 0; i < iEnd; i++)
			pA->data.s[i][i]=(short)rVal;
	case MAT_Tint:
		for (i = 0; i < iEnd; i++)
			pA->data.i[i][i]=(int)rVal;
	case MAT_Tfloat:
		for (i = 0; i < iEnd; i++)
			pA->data.fl[i][i]=(float)rVal;
	case MAT_Tdouble:
        for (i = 0; i < iEnd; i++)
			pA->data.db[i][i]=(double)rVal;
	}
}

void MatOp::Transpose(Mat* pA, const Mat* pB)
{
	assert ( (pA->Rows() == pB->Cols()) && (pA->Cols() == pB->Rows()) );
	assert ( pA->Type() == pB->Type());
	
	int i, j, r = pB->Cols(), c = pB->Rows();	

	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE ** ppbA = pA->data.ptr;
			BYTE ** ppbB = pB->data.ptr;
			for (i = 0; i < r; i++)
			for (j = 0; j < c; j++)
				ppbA[i][j]=ppbB[j][i];
		}
		break;
	case MAT_Tshort:
		{
		   short ** ppsA = pA->data.s;
           short ** ppsB = pB->data.s;
		   for (i = 0; i < r; i++)
		   for (j = 0; j < c; j++)
				ppsA[i][j]=ppsB[j][i];
		}
		break;
	case MAT_Tint:
		{
		   int ** ppnA = pA->data.i;
           int ** ppnB = pB->data.i;
		   for (i = 0; i < r; i++)
		   for (j = 0; j < c; j++)
				ppnA[i][j]=ppnB[j][i];
		}
		break;
	case MAT_Tfloat:
		{
		   float ** pprA = pA->data.fl;
           float ** pprB = pB->data.fl;
		   for (i = 0; i < r; i++)
		   for (j = 0; j < c; j++)
				pprA[i][j]=pprB[j][i];
		}
		break;				 
	case MAT_Tdouble:
		{
		   double ** ppdA = pA->data.db;
           double ** ppdB = pB->data.db;
		   for (i = 0; i < r; i++)
		   for (j = 0; j < c; j++)
				ppdA[i][j]=ppdB[j][i];
		}
		break;
	}
}

void MatOp::Transpose(Mat* pA)//¼­Ì¡ÃÔµÛ °éÂ×±¨
{
	if (pA->Rows() != pA->Cols())
	{
		Mat temp(*pA); pA->Release();
		pA->Create (temp.Cols(),temp.Rows(),temp.Type());
		Transpose(pA,&temp); 
		return;
	}
	int i, j, r = pA->Rows(), c = pA->Cols();	

	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE bTemp;
			BYTE ** ppbA = pA->data.ptr;
			for (i = 0; i < r; i++)
			for (j = i+1; j < c; j++)
			{
				bTemp = ppbA[i][j];
				ppbA[i][j] = ppbA[j][i];
				ppbA[j][i] = bTemp;
			}
		}
		break;
	case MAT_Tshort:
		{
			short bTemp;
			short ** ppbA = pA->data.s;
			for (i = 0; i < r; i++)
			for (j = i+1; j < c; j++)
			{
				bTemp = ppbA[i][j];
				ppbA[i][j] = ppbA[j][i];
				ppbA[j][i] = bTemp;
			}
		}
		break;
	case MAT_Tint:
		{
			int bTemp;
			int** ppbA = pA->data.i;
			for (i = 0; i < r; i++)
			for (j = i+1; j < c; j++)
			{
				bTemp = ppbA[i][j];
				ppbA[i][j] = ppbA[j][i];
				ppbA[j][i] = bTemp;
			}
		}
		break;
	case MAT_Tfloat:
		{
			float bTemp;
			float** ppbA = pA->data.fl;
			for (i = 0; i < r; i++)
			for (j = i+1; j < c; j++)
			{
				bTemp = ppbA[i][j];
				ppbA[i][j] = ppbA[j][i];
				ppbA[j][i] = bTemp;
			}
		}
		break;				 
	case MAT_Tdouble:
		{
			double bTemp;
			double** ppbA = pA->data.db;
			for (i = 0; i < r; i++)
			for (j = i+1; j < c; j++)
			{
				bTemp = ppbA[i][j];
				ppbA[i][j] = ppbA[j][i];
				ppbA[j][i] = bTemp;
			}
		}
		break;
	}
}

void MatOp::Equal(Vec* pA, const Vec* pB)
{
	assert (pA->Length() == pB->Length());
	assert (pA->Type() == pB->Type());
	int i;
	int		nLen = pA->Length();
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE* pA_ = pA->data.ptr;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (BYTE)(pB->data.ptr[i]);			
		}
		break;
	case MAT_Tshort:
		{  
			short * pA_ = pA->data.s;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (short)(pB->data.s[i]);
		}
		break;
	case MAT_Tint:
        {  
			int * pA_ = pA->data.i;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (int)(pB->data.i[i]);
		}
		break;
	case MAT_Tfloat:
		{  
			float * pA_ = pA->data.fl;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (float)(pB->data.fl[i]);
		}
		break;
	case MAT_Tdouble:
		{  
			double * pA_ = pA->data.db;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (double)(pB->data.db[i]);
		}
		break;
	}
	
}

void MatOp::Mul (Mat* pC, const Mat* pA, const Mat* pB)
{
	assert ( pA->Cols() == pB->Rows());
	assert (pC->Rows() >= pA->Rows() && pC->Cols() >= pB->Cols());
	assert (pA->Type() == pB->Type());
	assert (pA->Type() == pC->Type());
    
    int i, j, k, r = pA->Rows(), c = pB->Cols(), m = pB->Rows();

	pC->Zero();
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{ 
		    BYTE** ppbA = pA->data.ptr;
        	BYTE** ppbB = pB->data.ptr;
			BYTE** ppbC = pC->data.ptr;

			for (i = 0; i < r; i++)
			 for (j = 0; j < c; j++)
			 {
				 for (k = 0; k < m; k++)
					ppbC[i][j]+=ppbA[i][k]*ppbB[k][j];
			 }
		}
		break;
	case MAT_Tshort:
		{
		    short** ppsA = pA->data.s;
        	short** ppsB = pB->data.s;
			short** ppsC = pC->data.s;

			for (i = 0; i < r; i++)
			 for (j = 0; j < c; j++)
			 {
				 for (k = 0; k < m; k++)
					ppsC[i][j]+=ppsA[i][k]*ppsB[k][j];
			 }
		}
		break;
	case MAT_Tint:
		{
			int** ppiA = pA->data.i;
			int** ppiB = pB->data.i;
			int** ppiC = pC->data.i;

			for (i = 0; i < r; i++)
			for (j = 0; j < c; j++)
			{
				 for (k = 0; k < m; k++)
					ppiC[i][j]+=ppiA[i][k]*ppiB[k][j];
			}
		}
		break;
	case MAT_Tfloat:
		{
			float** pprA = pA->data.fl;
        	float** pprB = pB->data.fl;
			float** pprC = pC->data.fl;

			for (i = 0; i < r; i++)
			 for (j = 0; j < c; j++)
			 {
				 for (k = 0; k < m; k++)
					pprC[i][j]+=pprA[i][k]*pprB[k][j];
			 }
		}
		break;				 
	case MAT_Tdouble:
		{
			double** ppdA = pA->data.db;
        	double** ppdB = pB->data.db;
			double** ppdC = pC->data.db;

			for (i = 0; i < r; i++)
			 for (j = 0; j < c; j++)
			 {
				 for (k = 0; k < m; k++)
					ppdC[i][j]+=ppdA[i][k]*ppdB[k][j];
			 }
		}
		break;
	}
}

void MatOp::Add (Vec* pA, const Vec* pB, const Vec* pC)
{
	assert (pB->Length() == pC->Length());
	assert (pA->Length() == pC->Length());
	assert (pA->Type() == pB->Type());
	assert (pA->Type() == pC->Type());

	int i;
	int	nLen = pA->Length();
	switch (pA->Type())
	{
	case MAT_Tbyte:
		{
			BYTE* pA_ = pA->data.ptr;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (BYTE)(pB->Value(i) + pC->Value(i));
		}
		break;
	case MAT_Tshort:
		{
			short* pA_ = pA->data.s;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (short)(pB->Value(i) + pC->Value(i));
		}
		break;
	case MAT_Tint:
		{
			int* pA_ = pA->data.i;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (int)(pB->Value(i) + pC->Value(i));
		}
		break;
	case MAT_Tfloat:
		{
			float* pA_ = pA->data.fl;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (float)(pB->Value(i) + pC->Value(i));
		}
		break;
	case MAT_Tdouble:
		{
			double* pA_ = pA->data.db;
			for (i = 0; i < nLen; i ++)
				pA_[i] = (double)(pB->Value(i) + pC->Value(i));
		}
		break;
	}
}
	
int MatOp::Normalize(Mat* pMat, float rMin, float rMax)
{
	int nRows = pMat->Rows();
	int nCols = pMat->Cols();
	
	if (rMin == rMax)
		return FALSE;
	double rBin = rMax - rMin;
	
	switch(pMat->Type())
	{
	case MAT_Tbyte:
		break;
	case MAT_Tint:
		{
			int* pnBuf = pMat->data.i[0];
			for (int i = 0 ; i < nRows * nCols; i ++)
			{
				pnBuf[i] = (BYTE)((float)(pnBuf[i] - rMin) / rBin * MAX_8BIT_VALUE);
			}
			break;
		}
	case MAT_Tfloat:
		{
			float* prBuf = pMat->data.fl[0];
			for (int i = 0 ; i < nRows * nCols; i ++)
			{
				prBuf[i] = (BYTE)((float)(prBuf[i] - rMin) / rBin * MAX_8BIT_VALUE);
			}
			break;
		}
	case MAT_Tdouble:
		{
			double* prBuf = pMat->data.db[0];
			for (int i = 0 ; i < nRows * nCols; i ++)
			{
				prBuf[i] = (BYTE)((double)(prBuf[i] - rMin) / rBin * MAX_8BIT_VALUE);
			}
			break;
		}
		break;
	}	
	return TRUE;
}

bool	MatOp::CopyVec(Vec* pDst, const Vec* pSrc, TYPE type)
{
	int i;
	int nLen = pSrc->Length();

	if (!pDst->IsValid())
		pDst->Create (nLen, type);

	assert (pDst->Length() == nLen);
	
	if (type == pSrc->Type())
	{
		memcpy (pDst->GetBuffer(), pSrc->GetBuffer(), nLen * pSrc->Step());
	}
	else
	{
		switch (pDst->Type())
		{
		case MAT_Tbyte:
			for (i = 0; i < nLen; i ++)
				pDst->data.ptr[i] = (BYTE)pSrc->Value(i);
			break;
		case MAT_Tshort:
			for (i = 0; i < nLen; i ++)
				pDst->data.s[i] = (short)pSrc->Value(i);
			break;
		case MAT_Tint:
			for (i = 0; i < nLen; i ++)
				pDst->data.i[i] = (int)pSrc->Value(i);
			break;
		case MAT_Tfloat:
			for (i = 0; i < nLen; i ++)
				pDst->data.fl[i] = (float)pSrc->Value(i);
			break;
		case MAT_Tdouble:
			for (i = 0; i < nLen; i ++)
				pDst->data.db[i] = (double)pSrc->Value(i);
			break;
		}
	}
	return true;
}

void MatOp::Mul(Vec* pB, double	rReal, const Vec* pA)
{
	assert (pA->Length() > 0);

	int nLen = pA->Length();
	switch(pA->Type())
	{
		case MAT_Tbyte:
			{
				BYTE* pbA = pA->data.ptr;
				BYTE* pbB = pB->data.ptr;
				for (int i = 0; i < nLen; i ++)
					pbB[i] = (BYTE)(rReal * pbA[i]);
				break;
			}
		case MAT_Tshort:
			{
				short* pnA = pA->data.s;
				short* pnB = pB->data.s;
				for (int i = 0; i < nLen; i ++)
					pnB[i] = (short)(rReal * pnA[i]);
				break;
			}
		case MAT_Tint:
			{
				int* pnA = pA->data.i;
				int* pnB = pB->data.i;
				for (int i = 0; i < nLen; i ++)
					pnB[i] = (int)(rReal * pnA[i]);
				break;
			}
		case MAT_Tfloat:
			{
				float* prA = pA->data.fl;
				float* prB = pB->data.fl;
				for (int i = 0; i < nLen; i ++)
					prB[i] = (float)(rReal * prA[i]);
				break;
			}
		case MAT_Tdouble:
			{
				double* prA = pA->data.db;
				double* prB = pB->data.db;
				for (int i = 0; i < nLen; i ++)
					prB[i] = rReal * prA[i];
				break;
			}
	}
}

float MatOp::TrXAX (const Vec* pvX, const Mat* pmA)
{
	// copy x into here because it's faster accessing an array than a Vec
	int nLen = pvX->Length();

	assert(pmA->Rows() == nLen);
	assert(pmA->Cols() == nLen);
	assert(pvX->Type() == MAT_Tfloat || pvX->Type() == MAT_Tdouble);
	assert(pmA->Type() == MAT_Tfloat || pmA->Type() == MAT_Tdouble);

	if (pmA->Type() == MAT_Tfloat)
	{
		float* prTemp = new float[nLen];
		float DiagResult = 0, Result = 0;

		// init prTemp and sum diag elements

		int i;
		for (i = 0; i < nLen; i++)
		{
			prTemp[i] = pvX->data.fl[i];
			DiagResult += pmA->data.fl[i][i] * prTemp[i] * prTemp[i];
		}
		// sum upper right triangle elems

		for (i = 0; i < nLen; i++)
		{
			float rXi = prTemp[i];
			for (int j = i + 1; j < nLen; j++)
				Result += pmA->data.fl[i][j] * rXi * prTemp[j];
		}
		Result *= 2;    // incorporate lower left triangle elements

		delete[] prTemp;

		return DiagResult + Result;
	}
	else if (pmA->Type()==MAT_Tdouble)
	{
		double* prTemp = new double[nLen];
		double DiagResult = 0, Result = 0;

		// init prTemp and sum diag elements

		int i;
		for (i = 0; i < nLen; i++)
		{
			prTemp[i] = pvX->data.db[i];
			DiagResult += pmA->data.db[i][i] * prTemp[i] * prTemp[i];
		}
		// sum upper right triangle elems

		for (i = 0; i < nLen; i++)
		{
			double rXi = prTemp[i];
			for (int j = i + 1; j < nLen; j++)
				Result += pmA->data.db[i][j] * rXi * prTemp[j];
		}
		Result *= 2;    // incorporate lower left triangle elements

		delete[] prTemp;

		return (float)(DiagResult + Result);
	}
	return 0.0f;
}

void MatOp::TrABA (Mat* pmC, const Mat* pmA, const Mat* pmB)
{
	assert (pmA->Type() == MAT_Tdouble);
	assert (pmB->Type() == MAT_Tdouble);
	assert (pmC->Type() == MAT_Tdouble);

	if (pmA->Type()==MAT_Tdouble)
	{
		double** pprA = pmA->data.db;
		double** pprB = pmB->data.db;
		double** pprC = pmC->data.db;
		Mat mTemp(pmA->Cols(), pmB->Cols(), MAT_Tdouble);
		double** pprT = mTemp.data.db;

		int i, j, k;

		for (i = 0; i < pmA->Cols(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprT[i][j] = 0.0;
				for (k = 0; k < pmA->Rows(); k ++)
					pprT[i][j] += pprA[k][i] * pprB[k][j];
			}
		}

		for (i = 0; i < mTemp.Rows(); i ++)
		{
			for (j = 0; j < pmA->Cols(); j ++)
			{
				pprC[i][j] = 0.0;
				for (k = 0; k < pmA->Rows(); k ++)
					pprC[i][j] += pprT[i][k] * pprA[k][j];
			}
		}
		mTemp.Release();
	}
	else if (pmA->Type() == MAT_Tfloat)
	{
		float** pprA = pmA->data.fl;
		float** pprB = pmB->data.fl;
		float** pprC = pmC->data.fl;
		Mat mTemp(pmA->Cols(), pmB->Cols(), MAT_Tfloat);
		float** pprT = mTemp.data.fl;

		int i, j, k;

		for (i = 0; i < pmA->Cols(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprT[i][j] = 0.0;
				for (k = 0; k < pmA->Rows(); k ++)
					pprT[i][j] += pprA[k][i] * pprB[k][j];
			}
		}

		for (i = 0; i < mTemp.Rows(); i ++)
		{
			for (j = 0; j < pmA->Cols(); j ++)
			{
				pprC[i][j] = 0.0;
				for (k = 0; k < pmA->Rows(); k ++)
					pprC[i][j] += pprT[i][k] * pprA[k][j];
			}
		}
		mTemp.Release();
	}
}

void MatOp::ABTrA (Mat* pmC, const Mat* pmA, const Mat* pmB)
{
	assert (pmA->Type() == MAT_Tfloat || pmA->Type() == MAT_Tdouble);
	assert (pmB->Type() == MAT_Tfloat || pmB->Type() == MAT_Tdouble);
	assert (pmC->Type() == MAT_Tfloat || pmC->Type() == MAT_Tdouble);

	if (pmA->Type() == MAT_Tdouble)
	{
		double** pprA = pmA->data.db;
		double** pprB = pmB->data.db;
		double** pprC = pmC->data.db;
		Mat mTemp(pmA->Rows(), pmB->Cols(), MAT_Tdouble);
		double** pprT = mTemp.data.db;

		int i, j, k;

		for (i = 0; i < pmA->Rows(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprT[i][j] = 0.0;
				for (k = 0; k < pmA->Cols(); k ++)
					pprT[i][j] += pprA[i][k] * pprB[k][j];
			}
		}

		for (i = 0; i < mTemp.Rows(); i ++)
		{
			for (j = 0; j < pmA->Rows(); j ++)
			{
				pprC[i][j] = 0.0;
				for (k = 0; k < pmA->Cols(); k ++)
					pprC[i][j] += pprT[i][k] * pprA[j][k];
			}
		}
		mTemp.Release();
	}
	else if (pmA->Type() == MAT_Tfloat)
	{
		float** pprA = pmA->data.fl;
		float** pprB = pmB->data.fl;
		float** pprC = pmC->data.fl;
		Mat mTemp(pmA->Rows(), pmB->Cols(), MAT_Tfloat);
		float** pprT = mTemp.data.fl;

		int i, j, k;

		for (i = 0; i < pmA->Rows(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprT[i][j] = 0.0;
				for (k = 0; k < pmA->Cols(); k ++)
					pprT[i][j] += pprA[i][k] * pprB[k][j];
			}
		}

		for (i = 0; i < mTemp.Rows(); i ++)
		{
			for (j = 0; j < pmA->Rows(); j ++)
			{
				pprC[i][j] = 0.0;
				for (k = 0; k < pmA->Cols(); k ++)
					pprC[i][j] += pprT[i][k] * pprA[j][k];
			}
		}
		mTemp.Release();
	}
}

double MatOp::DotProduct (const Vec* pA, const Vec* pB)
{
	assert (pA->Length() == pB->Length());
	assert (pA->Type() == pB->Type());

	int nLen = pA->Length();
	int i;
	double rVal = 0.0;
	switch (pA->Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < nLen; i ++)
			rVal += pA->data.ptr[i] * pB->data.ptr[i];
		break;
	case MAT_Tshort:
		for (i = 0; i < nLen; i ++)
			rVal += pA->data.s[i] * pB->data.s[i];
		break;
	case MAT_Tint:
		for (i = 0; i < nLen; i ++)
			rVal += pA->data.i[i] * pB->data.i[i];
		break;
	case MAT_Tfloat:
		for (i = 0; i < nLen; i ++)
			rVal += pA->data.fl[i] * pB->data.fl[i];
		break;
	case MAT_Tdouble:
		for (i = 0; i < nLen; i ++)
			rVal += pA->data.db[i] * pB->data.db[i];
		break;
	}
	return rVal;
}

void MatOp::TrAA(Mat* pmB, const Mat* pmA)
{
	assert (pmA->Type() == MAT_Tfloat || pmA->Type() == MAT_Tdouble);
	assert (pmB->Type() == MAT_Tfloat || pmB->Type() == MAT_Tdouble);
	assert (pmB->Rows() == pmA->Cols() && pmB->Cols() == pmA->Cols());
	
	int i, j, k;
	if (pmA->Type() == MAT_Tdouble)
	{
		double** pprA = pmA->data.db;
		double** pprB = pmB->data.db;
		for (i = 0; i < pmB->Rows(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprB[i][j] = 0.0;
				for (k = 0; k < pmA->Rows(); k ++)
					pprB[i][j] += pprA[k][i] * pprA[k][j];
			}
		}
	}
	else if (pmA->Type() == MAT_Tfloat)
	{
		float** pprA = pmA->data.fl;
		float** pprB = pmB->data.fl;
		for (i = 0; i < pmB->Rows(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprB[i][j] = 0.0;
				for (k = 0; k < pmA->Rows(); k ++)
					pprB[i][j] += pprA[k][i] * pprA[k][j];
			}
		}
	}
}

void MatOp::ATrA(Mat* pmB, const Mat* pmA)
{
	assert (pmA->Type() == MAT_Tfloat || pmA->Type() == MAT_Tdouble);
	assert (pmB->Type() == MAT_Tfloat || pmB->Type() == MAT_Tdouble);
	assert (pmB->Rows() == pmA->Rows() && pmB->Cols() == pmA->Rows());
	
	int i, j, k;

	if (pmA->Type() == MAT_Tdouble)
	{
		double** pprA = pmA->data.db;
		double** pprB = pmB->data.db;
		for (i = 0; i < pmB->Rows(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprB[i][j] = 0.0;
				for (k = 0; k < pmA->Cols(); k ++)
					pprB[i][j] += pprA[i][k] * pprA[j][k];
			}
		}
	}
	else if (pmA->Type() == MAT_Tfloat)
	{
		float** pprA = pmA->data.fl;
		float** pprB = pmB->data.fl;
		for (i = 0; i < pmB->Rows(); i ++)
		{
			for (j = 0; j < pmB->Cols(); j ++)
			{
				pprB[i][j] = 0.0;
				for (k = 0; k < pmA->Cols(); k ++)
					pprB[i][j] += pprA[i][k] * pprA[j][k];
			}
		}
	}
}

double MatOp::Trace (const Mat* pmA)
{
	double rSum = 0;
	int i;
	switch (pmA->Type())
	{
	case MAT_Tdouble:
		for (i = 0; i < MIN(pmA->Rows(), pmA->Cols()); i ++)
			rSum += pmA->data.db[i][i];
		break;
	case MAT_Tfloat:
		for (i = 0; i < MIN(pmA->Rows(), pmA->Cols()); i ++)
			rSum += pmA->data.fl[i][i];
		break;
	case MAT_Tint:
		for (i = 0; i < MIN(pmA->Rows(), pmA->Cols()); i ++)
			rSum += pmA->data.i[i][i];
		break;
	case MAT_Tshort:
		for (i = 0; i < MIN(pmA->Rows(), pmA->Cols()); i ++)
			rSum += pmA->data.s[i][i];
		break;
	case MAT_Tbyte:
		for (i = 0; i < MIN(pmA->Rows(), pmA->Cols()); i ++)
			rSum += pmA->data.ptr[i][i];
		break;
	}
	return rSum;
}

double MatOp::Sum(const Vec* pvA)
{
	double rSum = 0;
	int i = 0;
	switch(pvA->Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < pvA->Length(); i ++)
			rSum += pvA->data.ptr[i];
		return rSum;
	case MAT_Tshort:
		for (i = 0; i < pvA->Length(); i ++)
			rSum += pvA->data.s[i];
		return rSum;
	case MAT_Tint:
		for (i = 0; i < pvA->Length(); i ++)
			rSum += pvA->data.i[i];
		return rSum;
	case MAT_Tfloat:
		for (i = 0; i < pvA->Length(); i ++)
			rSum += pvA->data.fl[i];
		return rSum;
	case MAT_Tdouble:
		for (i = 0; i < pvA->Length(); i ++)
			rSum += pvA->data.db[i];
		return rSum;
	}	
	return rSum;
}

double MatOp::Sum(const Mat* pmA)
{
	double rSum = 0;
	int i = 0;
	switch(pmA->Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < pmA->Rows() * pmA->Cols(); i ++)
			rSum += pmA->data.ptr[0][i];
		return rSum;
	case MAT_Tshort:
		for (i = 0; i < pmA->Rows() * pmA->Cols(); i ++)
			rSum += pmA->data.s[0][i];
		return rSum;
	case MAT_Tint:
		for (i = 0; i < pmA->Rows() * pmA->Cols(); i ++)
			rSum += pmA->data.i[0][i];
		return rSum;
	case MAT_Tfloat:
		for (i = 0; i < pmA->Rows() * pmA->Cols(); i ++)
			rSum += pmA->data.fl[0][i];
		return rSum;
	case MAT_Tdouble:
		for (i = 0; i < pmA->Rows() * pmA->Cols(); i ++)
			rSum += pmA->data.db[0][i];
		return rSum;
	}	
	return rSum;
}

void MatOp::SumRows(const Mat* pmA, Vec* pvB)
{
	assert (pmA->Cols() == pvB->Length());
	assert (pmA->Type() == pvB->Type());
	double rSum = 0;

	int i = 0;
	switch(pmA->Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < pmA->Cols(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Rows(); j ++)
				rSum += pmA->data.ptr[j][i];
			pvB->data.ptr[i] = (BYTE)rSum;
		}
	case MAT_Tshort:
		for (i = 0; i < pmA->Cols(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Rows(); j ++)
				rSum += pmA->data.s[j][i];
			pvB->data.s[i] = (short)rSum;
		}
	case MAT_Tint:
		for (i = 0; i < pmA->Cols(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Rows(); j ++)
				rSum += pmA->data.i[j][i];
			pvB->data.i[i] = (int)rSum;
		}
	case MAT_Tfloat:
		for (i = 0; i < pmA->Cols(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Rows(); j ++)
				rSum += pmA->data.fl[j][i];
			pvB->data.fl[i] = (float)rSum;
		}
	case MAT_Tdouble:
		for (i = 0; i < pmA->Cols(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Rows(); j ++)
				rSum += pmA->data.db[j][i];
			pvB->data.db[i] = (double)rSum;
		}
	}	
}

void MatOp::SumCols(const Mat* pmA, Vec* pvB)
{
	assert (pmA->Rows() == pvB->Length());
	assert (pmA->Type() == pvB->Type());
	double rSum = 0;
	
	int i = 0;
	switch(pmA->Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < pmA->Rows(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Cols(); j ++)
				rSum += pmA->data.ptr[i][j];
			pvB->data.ptr[i] = (BYTE)rSum;
		}
	case MAT_Tshort:
		for (i = 0; i < pmA->Rows(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Cols(); j ++)
				rSum += pmA->data.s[i][j];
			pvB->data.s[i] = (short)rSum;
		}
	case MAT_Tint:
		for (i = 0; i < pmA->Rows(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Cols(); j ++)
				rSum += pmA->data.i[i][j];
			pvB->data.i[i] = (int)rSum;
		}
	case MAT_Tfloat:
		for (i = 0; i < pmA->Rows(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Cols(); j ++)
				rSum += pmA->data.fl[i][j];
			pvB->data.fl[i] = (float)rSum;
		}
	case MAT_Tdouble:
		for (i = 0; i < pmA->Rows(); i ++)
		{
			rSum = 0;
			for (int j = 0; j < pmA->Cols(); j ++)
				rSum += pmA->data.db[i][j];
			pvB->data.db[i] = (double)rSum;
		}
	}	
}

template<typename _Tp> static 
void inRange_(const _Tp* src, const _Tp* a, const _Tp* b, BYTE* dst, int total)
{
	int i;
	for( i = 0; i < total; i++ )
	{
		_Tp val = src[i];
		dst[i] = a[i] <= val && val <= b[i] ? 255 : 0;
	}
	// 	for( c = 1; c < cn; c++ )
	// 	{
	// 		for( i = 0; i < total; i++ )
	// 		{
	// 			_Tp val = src[i*cn + c];
	// 			dst[i] = a[i*cn + c] <= val && val <= b[i*cn + c] ? dst[i] : 0;
	// 		}
	// 	}
}


void MatOp::inRange(const Mat& src, const Mat& lb, const Mat& rb, Mat& dst)
{
	assert( src.IsEqualMat(lb) && src.IsEqualMat(rb) );
	dst.Release();
	dst.Create( src.SizeObject(), MAT_Tbyte );

	int total = src.Rows()*src.Cols();
	{
		const BYTE* sptr = src.data.ptr[0];
		const BYTE* aptr = lb.data.ptr[0];
		const BYTE* bptr = rb.data.ptr[0];
		BYTE* dptr = dst.data.ptr[0];

		switch( src.Type() )
		{
		case MAT_Tbyte:
			inRange_((const BYTE*)sptr, (const BYTE*)aptr, (const BYTE*)bptr, dptr, total);
			break;
		case MAT_Tshort:
			inRange_((const short*)sptr, (const short*)aptr, (const short*)bptr, dptr, total);
			break;
		case MAT_Tint:
			inRange_((const int*)sptr, (const int*)aptr, (const int*)bptr, dptr, total);
			break;
		case MAT_Tfloat:
			inRange_((const float*)sptr, (const float*)aptr, (const float*)bptr, dptr, total);
			break;
		case MAT_Tdouble:
			inRange_((const double*)sptr, (const double*)aptr, (const double*)bptr, dptr, total);
			break;
		}
	}
}

template<typename _Tp, typename _WTp> static void
inRangeS_(const _Tp* src, const _WTp a, const _WTp b, BYTE* dst, int total)
{
	int i;
	for( i = 0; i < total; i++ )
	{
		_Tp val = src[i];
		if (dst[i]!=0)
			dst[i] &= ((a <= val && val <= b) ? BYTE(255) : 0);
	}
}

void MatOp::inRange(const Mat& src, const double lb, const double rb, Mat& dst)
{
	if (!src.IsEqualSize(dst) || dst.Type() != MAT_Tbyte)
	{
		dst.Release();
		dst.Create( src.SizeObject(), MAT_Tbyte );
	}

	int total = src.Rows()*src.Cols();
	{
		const BYTE* sptr = src.data.ptr[0];
		BYTE* dptr = dst.data.ptr[0];

		switch( src.Type() )
		{
		case MAT_Tbyte:
			inRangeS_((const BYTE*)sptr, lb, rb, dptr, total);
			break;
		case MAT_Tshort:
			inRangeS_((const short*)sptr, lb, rb, dptr, total);
			break;
		case MAT_Tint:
			inRangeS_((const int*)sptr, lb, rb, dptr, total);
			break;
		case MAT_Tfloat:
			inRangeS_((const float*)sptr, lb, rb, dptr, total);
			break;
		case MAT_Tdouble:
			inRangeS_((const double*)sptr, lb, rb, dptr, total);
			break;
		}
	}
}

void MatOp::inRange(const CoImage& src, const double lb[3], const double rb[3], Mat& dst, int dims)
{
	if (src.SizeObject()!=dst.SizeObject() || dst.Type() != MAT_Tbyte)
	{
		dst.Release();
		dst.Create( src.SizeObject(), MAT_Tbyte );
	}
	int total = src.SizeObject().Area();

	for (int k=0; k<MIN(dims, src.GetChannel()); k++)
	{
		const BYTE* sptr = src.GetBand()[k]->data.ptr[0];
		BYTE* dptr = dst.data.ptr[0];

		switch( src.m_matX.Type() )
		{
		case MAT_Tbyte:
			inRangeS_((const BYTE*)sptr, lb[k], rb[k], dptr, total);
			break;
		case MAT_Tshort:
			inRangeS_((const short*)sptr, lb[k], rb[k], dptr, total);
			break;
		case MAT_Tint:
			inRangeS_((const int*)sptr, lb[k], rb[k], dptr, total);
			break;
		case MAT_Tfloat:
			inRangeS_((const float*)sptr, lb[k], rb[k], dptr, total);
			break;
		case MAT_Tdouble:
			inRangeS_((const double*)sptr, lb[k], rb[k], dptr, total);
			break;
		}
	}
}

void MatOp::Normalize( const Mat& src, Mat& dst, double a, double b,
					  int norm_type, TYPE rtype, const Mat& mask)
{
	double scale = 1, shift = 0;
	if( norm_type == CVLIB_MINMAX )
	{
		double smin = 0, smax = 0;
		double dmin = MIN( a, b ), dmax = MAX( a, b );
		src.MinMaxLoc( &smin, &smax, 0, 0, mask );
		scale = (dmax - dmin)*( (smax-smin>DBL_EPSILON) ? 1./(smax - smin) : 0);
		shift = dmin - smin*scale;
	}
	else if( norm_type == CVLIB_L2 || norm_type == CVLIB_L1 || norm_type == CVLIB_C )
	{
		scale =  src.Norm( norm_type, mask );
		scale = (scale>DBL_EPSILON) ? a/scale : 0.;
		shift = 0;
	}
	else
		assert( 0 );

	rtype = MAX(dst.Type(), rtype);

	if (src.SizeObject()!=dst.SizeObject() || rtype!=dst.Type())
	{
		dst.Release();
		dst.Create(src.SizeObject(), rtype);
	}

	if( !mask.IsValid() )
	{
		MatOp::CopyMat(&dst, &src, rtype);
		dst.Convert( rtype, scale, shift );
	}
	else
	{
		Mat temp;
		MatOp::CopyMat(&temp, &src, rtype);
		temp.Convert( rtype, scale, shift );
		temp.CopyTo( dst, mask );
	}
}

typedef void (*addWeightedFunc)(uchar** const src1, uchar** const src2, uchar** dst, Size sz, void*);
template<typename T, typename WT> static void
addWeighted_( uchar** const _src1, uchar** const _src2, uchar** _dst, Size size, void* _scalars )
{
	T** const src1=(T** const)_src1;
	T** const src2=(T** const)_src2;
	T** dst=(T**)_dst;

	const double* scalars = (const double*)_scalars;
	WT alpha = (WT)scalars[0], beta = (WT)scalars[1], gamma = (WT)scalars[2];

	for(int y=0 ; y<size.height; y++)
	{
		const T* s1=src1[y];
		const T* s2=src2[y];
		T* d=dst[y];
		int x = 0;
//#if CV_ENABLE_UNROLLED
		for( ; x <= size.width - 4; x += 4 )
		{
			T t0 = (T)(s1[x]*alpha + s2[x]*beta + gamma);
			T t1 = (T)(s1[x+1]*alpha + s2[x+1]*beta + gamma);
			d[x] = t0; d[x+1] = t1;

			t0 = (T)(s1[x+2]*alpha + s2[x+2]*beta + gamma);
			t1 = (T)(s1[x+3]*alpha + s2[x+3]*beta + gamma);
			d[x+2] = t0; d[x+3] = t1;
		}
//#endif
		for( ; x < size.width; x++ )
			d[x] = (T)(s1[x]*alpha + s2[x]*beta + gamma);
	}
}

void MatOp::AddWeighted(const Mat& src1, double alpha, const Mat& src2, double beta, double gamma, Mat& dst)
{
	assert (src1.IsEqualMat(src2));
	if (!src1.IsEqualMat(dst))
	{
		dst.Release();
		dst.Create(src1, FALSE);
	}
	addWeightedFunc addWeightedTab[] =
	{ NULL, addWeighted_<BYTE, float>, addWeighted_<short, float>, addWeighted_<int, double>, addWeighted_<float, float>, addWeighted_<double, double> };
	addWeightedFunc func=addWeightedTab[(int)dst.Type()];
	double scalars[] = {alpha, beta, gamma};
	func(src1.data.ptr, src2.data.ptr, dst.data.ptr, src1.SizeObject(), scalars);
}

void MatOp::GEMM(const Mat* matA, const Mat* matB, double alpha,
				 const Mat* matC, double beta, Mat* matD, int flags)
{
	if (!matD) return;
	Mat temp; temp.Create(*matD, FALSE);
	Mat temp1; temp1.Create(*matD, FALSE);
	if (matA && matB)
	{
		MatOp::Mul(&temp, matA, matB);
		if ( fabs(alpha-1.0) > DBL_EPSILON )
			MatOp::Mul(&temp, alpha, &temp);
	}
	if (matC && fabs(beta) > DBL_EPSILON)
	{
		MatOp::Mul(&temp1, beta, matC);
	}
	matD->Zero();
	if (matA && matB)
		MatOp::Add(matD, matD, &temp);
	if (matC && fabs(beta) > DBL_EPSILON)
		MatOp::Add(matD, matD, &temp1);
}

void MatOp::MatMulAdd(const Mat* src1, const Mat* src2, const Mat* src3, Mat* dst)
{
	GEMM( src1, src2, 1.0, src3, 1.0, dst, 0 );
}

typedef double (*ScalarProductFunc)(uchar** const src1, uchar** const src2, const Size& sz);
template<typename T> static double
ScalarProduct_( uchar** const _src1, uchar** const _src2, const Size& size )
{
	double rsum=0;
	int i, j;
	for (i=0; i<size.height; i++)
	{
		const T* src1=(const T*)_src1[i];
		const T* src2=(const T*)_src2[i];
		for (j=0; j<size.width; j++)
			rsum += src1[j]*src2[j];
	}
	return rsum;
}

double MatOp::ScalarProduct(Mat* pmA, Mat *pmB)
{
	assert (pmA->IsEqualMat(*pmB));
	ScalarProductFunc funcs[]={
		ScalarProduct_<char>,
		ScalarProduct_<uchar>,
		ScalarProduct_<short>,
		ScalarProduct_<int>,
		ScalarProduct_<float>,
		ScalarProduct_<double>
	};
	return funcs[pmA->Type()](pmA->data.ptr, pmB->data.ptr, pmA->SizeObject());
}

static void Magnitude_32f(const float* x, const float* y, float* mag, int len)
{
	int i = 0;
	for( ; i < len; i++ )
	{
		float x0 = x[i], y0 = y[i];
		mag[i] = sqrt(x0*x0 + y0*y0);
	}
}

static void Magnitude_64f(const double* x, const double* y, double* mag, int len)
{
	int i = 0;

	for( ; i < len; i++ )
	{
		double x0 = x[i], y0 = y[i];
		mag[i] = sqrt(x0*x0 + y0*y0);
	}
}

static void FastAtan2_32f(const float *Y, const float *X, float *angle, int len, bool angleInDegrees=true )
{
	int i = 0;
	float scale = angleInDegrees ? (float)(180/CVLIB_PI) : 1.f;

	for( ; i < len; i++ )
	{
		float x = X[i], y = Y[i];
		float a, x2 = x*x, y2 = y*y;
		if( y2 <= x2 )
			a = x*y/(x2 + 0.28f*y2 + (float)DBL_EPSILON) + (float)(x < 0 ? CVLIB_PI : y >= 0 ? 0 : CVLIB_PI*2);
		else
			a = (float)(y >= 0 ? CVLIB_PI*0.5 : CVLIB_PI*1.5) - x*y/(y2 + 0.28f*x2 + (float)DBL_EPSILON);
		angle[i] = a*scale;
	}
}

#define MAX_BLOCK_SIZE 1024
void CartToPolar( const Mat& X, const Mat& Y, Mat& Mag, Mat& Angle, bool angleInDegrees )
{
	float buf[2][MAX_BLOCK_SIZE];
	int i, j, k;
	TYPE type = X.Type1();

	assert( X.SizeObject() == Y.SizeObject() && type == Y.Type() && (type == MAT_Tfloat || type == MAT_Tdouble));
	Mag.Create( X.SizeObject(), type );
	Angle.Create( X.SizeObject(), type );

	int cn=X.Channels();
	Size size = X.SizeObject();
	size.width *= cn;
	bool inplace = Mag.data.ptr == X.data.ptr || Mag.data.ptr == Y.data.ptr;
	int blockSize = MIN(size.width, ((MAX_BLOCK_SIZE+cn-1)/cn)*cn);
	if( type == MAT_Tfloat )
	{
		for(k=0 ; k<size.height; k++)
		{
			const float *x = (const float*)X.data.ptr[k], *y = (const float*)Y.data.ptr[k];
			float *mag = (float*)Mag.data.fl[k], *angle = (float*)Angle.data.fl[k];
			for( i = 0; i < size.width; i += blockSize )
			{
				int block_size = MIN(blockSize, size.width - i);
				Magnitude_32f( x + i, y + i, inplace ? buf[0] : mag + i, block_size );
				FastAtan2_32f( y + i, x + i, angle + i, block_size, angleInDegrees );
				if( inplace )
					for( j = 0; j < block_size; j++ )
						mag[i + j] = buf[0][j];
			}
		}
	}
	else
	{
		for(k=0 ; k<size.height; k++)
		{
			const double *x = (const double*)X.data.ptr[k], *y = (const double*)Y.data.ptr[k];
			double *mag = (double*)Mag.data.fl[k], *angle = (double*)Angle.data.fl[k];
			for( i = 0; i < size.width; i += blockSize )
			{
				int block_size = MIN(blockSize, size.width - i);
				for( j = 0; j < block_size; j++ )
				{
					buf[0][j] = (float)x[i + j];
					buf[1][j] = (float)y[i + j];
				}
				FastAtan2_32f( buf[1], buf[0], buf[0], block_size, angleInDegrees );
				Magnitude_64f( x + i, y + i, mag + i, block_size );
				for( j = 0; j < block_size; j++ )
					angle[i + j] = buf[0][j];
			}
		}
	}
}

}