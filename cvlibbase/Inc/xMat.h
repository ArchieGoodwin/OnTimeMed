/*!
 * \file    xMat
 * \ingroup base
 * \brief   ¹ÖÀË¶£¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _xMAT_H_
#define _xMAT_H_

namespace CVLib
{

template<typename _Tp>
Mat_<_Tp>::Mat_(const Mat& m)
{
	if (DataDepth<_Tp>::value == m.Type())
	{
		Mat::operator =(m);
	}
	Mat::operator =(m);
	Convert ((TYPE)DataDepth<_Tp>::value, CT_Cast);
}

template<typename _Tp>
Mat_<_Tp>::Mat_(const Mat_<_Tp>& mat)
{
	if (this == &mat) return;
	Mat::Create (mat, TRUE);
}
template<typename _Tp>
Mat_<_Tp>::Mat_(int rows, int cols)
{
	Create (rows, cols);
}
template<typename _Tp>
Mat_<_Tp>::Mat_(_Tp** ptr, int rows, int cols)
{
	Create(ptr, rows, cols);
}
template<typename _Tp>
Mat_<_Tp>::Mat_(_Tp* ptr, int rows, int cols, BOOL fAlloc)
{
	Create (ptr, rows, cols, fAlloc);
}
template<typename _Tp>
int	Mat_<_Tp>::Create(int rows, int cols)
{
	if (this->rows==rows && this->cols==cols && 
		this->type==(TYPE)DataDepth<_Tp>::value && data.ptr)
		return 0;
	Release();
	this->rows = rows;
	this->cols = cols;
	this->type = (TYPE)DataDepth<_Tp>::value;
	step = sizeof(_Tp);
	MALLOC (data.ptr, sizeof(_Tp*) * rows, uchar*);
	MALLOC (data.ptr[0], sizeof(_Tp) * rows * cols*Channels(), uchar);
	int i;
	for (i = 1; i < rows; i ++)
		data.ptr[i] = data.ptr[0] + i*step*cols*Channels();
	depend = 0;
	return 1;
}
template<typename _Tp>
int	Mat_<_Tp>::Create(_Tp** ptr, int rows, int cols)
{
	Release();
	this->rows = rows;
	this->cols = cols;
	this->type = (TYPE)DataDepth<_Tp>::value;
	this->step = sizeof(_Tp);
	this->data.ptr = (uchar**)ptr;
	depend = 1;
	return 1;
}
template<typename _Tp>
int	Mat_<_Tp>::Create(_Tp* ptr, int rows, int cols, BOOL fAlloc)
{
	Release();
	int i;
	this->rows = rows;
	this->cols = cols;
	this->type = (TYPE)DataDepth<_Tp>::value;
	step = sizeof(_Tp);
	if (fAlloc)
	{
		MALLOC (data.ptr, sizeof(_Tp*) * rows, uchar*);
		data.ptr[0] = (uchar*)ptr;
		for (i = 1; i < rows; i ++)
			data.ptr[i] = (uchar*)(ptr + i * cols);
		depend = 2;
	}
	else
	{
		data.ptr = (uchar**)(ptr + rows * cols);
		data.ptr[0] = (uchar*)ptr;
		for (i = 1; i < rows; i ++)
			data.ptr[i] = (uchar*)(ptr + i * cols);
		depend = 4;
	}
	return 1;
}
template<typename _Tp>
int	Mat_<_Tp>::Create(const Mat_<_Tp>& mat, BOOL fCopy)
{
	int nRet;
	if (SizeObject()==mat.SizeObject()&&type==mat.Type1()&&data.ptr)
	nRet=1;
	else
	{
		Release();
		nRet = Create (mat.rows, mat.cols);
	}
	if (nRet)
	{
		if (fCopy)
		{
			for (int i = 0;  i < Rows(); i ++)
				memcpy (data.ptr[i], mat.data.ptr[i], Step() * Cols());
		}
		return 1;
	}
	return 0;
}
template<typename _Tp>
Mat_<_Tp>&	Mat_<_Tp>::operator=(const Mat_<_Tp>& other)
{
	if (this==&other)
		return *this;
	if (this->IsValid())
		memcpy (data.ptr[0], other.data.ptr[0], Step()*rows*cols*Channels());
	else
		Mat::Create(other, TRUE);
	return *this;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::operator=(double value)
{
	int i;
	if ((value == 0.0) && (rows != 0))
	{
		assert(rows > 0 && cols > 0);
		if (IsContinuous())
			memset(data.ptr[0], 0, rows*cols*sizeof(_Tp)*Channels());
		else
		{
			int nlinestep=cols*sizeof(_Tp)*Channels();
			for (i=0;i<rows;i++)
				memset(data.ptr[i], 0, nlinestep);
		}
	}
	else
	{
		int n = rows * cols;
		_Tp *p = (_Tp*)data.ptr[0];
		for (i = 0; i < n; i++)
			p[i] = (_Tp)value;
	}
	return *this;
}
template<typename _Tp>
bool Mat_<_Tp>::operator==(const Mat_<_Tp>& refmatrix) const
{
	if ((Rows() == refmatrix.Rows()) && (Cols() == refmatrix.Cols()))
	{
		if ((Rows() == 0) || (Cols() == 0))
			return true;
		int cbRow = Cols()  * sizeof(_Tp);
		for (int iRow = 0; iRow < Rows(); ++iRow)
		{
			if (memcmp(data.ptr[iRow], refmatrix.data.ptr[iRow], cbRow) != 0)
				return false;
		}
		return true;
	}
	return false; 
}
template<typename _Tp>
bool Mat_<_Tp>::operator!=(const Mat_<_Tp>& refmatrix) const
{
	return !(*this == refmatrix);
}
template<typename _Tp>
bool Mat_<_Tp>::operator<(const Mat_<_Tp>& refmatrix) const
{
	if (Rows() == refmatrix.Rows())
	{
		if (Cols() == refmatrix.Cols())
		{
			if ((Rows() == 0) || (Cols() == 0))
				return false;
			int cbRow = Cols()  * sizeof(_Tp);
			for (int iRow = 0; iRow < Rows(); ++iRow)
			{
				int wCmp = memcmp(data.ptr[iRow], refmatrix.data.ptr[iRow], cbRow);
				if (wCmp != 0)
					return (wCmp < 0);
			}
			return false;
		}
		return (Cols() < refmatrix.Cols());
	}
	return (Rows() < refmatrix.Rows());
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::operator+(const Mat_<_Tp>& refmatrix) const
{
	Mat_<_Tp> matrixRet(*this);
	return matrixRet.EqSum(*this, refmatrix);
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::operator-(void) const
{
	Mat_<_Tp> matrixRet(Rows(), Cols());
	for (int i = 0; i < Rows(); i++)
	for (int j = 0; j < Cols(); j++)
	matrixRet.data.ptr[i][j] = - data.ptr[i][j];
	return matrixRet;
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::operator-(const Mat_<_Tp>& refmatrix) const
{
	Mat_<_Tp> matrixRet(*this);
	return matrixRet.EqDiff(*this, refmatrix);
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::operator*(double dbl) const
{
	Mat_<_Tp> matrixRet(*this);
	return (matrixRet *= dbl);
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::operator*(const Mat_<_Tp>& refmatrix) const
{
	Mat_<_Tp> matrixRet(Rows(), refmatrix.Cols());
	return matrixRet.EqProd(*this, refmatrix);
}
template<typename _Tp>
Vec_<_Tp> Mat_<_Tp>::operator*(const Vec_<_Tp>& refvector) const
{
	Vec_<_Tp> vectorRet(Rows());
	return vectorRet.EqProd(*this, refvector);
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::operator/(double dbl) const
{
	Mat_<_Tp> matrixRet(*this);
	return (matrixRet /= dbl);
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::operator+=(const Mat_<_Tp>& refmatrix)
{
	assert((Rows() == refmatrix.Rows()) && (Cols() == refmatrix.Cols()));
	for (int i = 0; i < Rows(); i++)for (int j = 0; j < Cols(); j++)
	(*this)[i][j] += refmatrix[i][j];
	return *this;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::operator-=(const Mat_<_Tp>& refmatrix)
{
	assert((Rows() == refmatrix.Rows()) && (Cols() == refmatrix.Cols()));
	for (int i = 0; i < Rows(); i++)
	for (int j = 0; j < Cols(); j++)
	(*this)[i][j] -= refmatrix[i][j];
	return *this;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::operator*=(double dbl)
{
	for (int i = 0; i < Rows(); i++)
	for (int j = 0; j < Cols(); j++)
	(*this)[i][j] = (_Tp)((*this)[i][j]*dbl);
	return *this;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::operator*=(const Mat_<_Tp>& refmatrix)
{
	Mat_<_Tp> matrixT(*this);
	assert(matrixT.Cols() == refmatrix.Rows());
	if (refmatrix.Cols() != refmatrix.Rows())
	{
		ResizeRows(Rows());
		ResizeCols(refmatrix.Cols());
	}
	for (int i = 0; i < matrixT.Rows(); i++)
	{
		for (int j = 0; j < refmatrix.Cols(); j++)
		{
			double sum = 0.0;
			for (int k = 0; k < Cols(); k++)
				sum += matrixT[i][k] * refmatrix[k][j];
			(*this)[i][j] = (_Tp)sum;
		}
	}
	return *this;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::operator/=(double dbl)
{
	assert(dbl != 0);
	for (int i = 0; i < Rows(); i++)
	for (int j = 0; j < Cols(); j++)
	(*this)[i][j] = (_Tp)((*this)[i][j]/dbl);
	return *this;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::EqSum(const Mat_<_Tp> &refmatrixA, const Mat_<_Tp> &refmatrixB)
{
	int nRows = Rows();
	int nCols = Cols();
	assert(nRows == refmatrixA.Rows());
	assert(nCols == refmatrixA.Cols());
	assert(nRows == refmatrixB.Rows());
	assert(nCols == refmatrixB.Cols());
	for (int i = 0; i < nRows; i++)
	for (int j = 0; j < nCols; j++)
	(*this)[i][j] = refmatrixA[i][j] + refmatrixB[i][j];
	return (*this);
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::EqDiff(const Mat_<_Tp> &refmatrixA, const Mat_<_Tp> &refmatrixB)
{
	int nRows = Rows();
	int nCols = Cols();
	assert(nRows == refmatrixA.Rows());
	assert(nCols == refmatrixA.Cols());
	assert(nRows == refmatrixB.Rows());
	assert(nCols == refmatrixB.Cols());
	for (int i = 0; i < nRows; i++)
	for (int j = 0; j < nCols; j++)
	(*this)[i][j] = refmatrixA[i][j] - refmatrixB[i][j];
	return (*this);
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::EqProd(const Mat_<_Tp> &refmatrixA, const Mat_<_Tp> &refmatrixB)
{
	int nRows = Rows();
	int nCols = Cols();
	int nTerms = refmatrixA.Cols();
	assert(nRows == refmatrixA.Rows());
	assert(nTerms == refmatrixB.Rows());
	assert(refmatrixB.Cols() == nCols);
	for (int i = 0; i < nRows; i++) {
	for (int j = 0; j < nCols; j++) {
	double sum = 0.0;
	for (int k = 0; k < nTerms; k++)
	sum += refmatrixA[i][k] * refmatrixB[k][j];
	(*this)[i][j] = (_Tp)sum;
	}
	}
	return (*this);
}
template<typename _Tp>
Vec_<_Tp> Mat_<_Tp>::Row(int r) const
{
	assert(r < Rows());
	assert(r >= 0);
	Vec_<_Tp> row(Cols());
	memcpy(row.data.ptr, &(*this)[r][0], Cols()*Step());
	return row;
}
template<typename _Tp>
Vec_<_Tp> Mat_<_Tp>::Column(int c) const
{
	assert(c < Cols());
	assert(c >= 0);
	Vec_<_Tp> col(Rows());
	for (int i = 0; i < Rows(); i++)
		col[i] = (*this)[i][c];
	return col;
}
template<typename _Tp>
void Mat_<_Tp>::SetRow(int r, const Vec_<_Tp> &v)
{
	int N = Cols();
	if (N > v.Length())
		N = v.Length();
	memcpy(&(*this)[r][0], &v[0], N*sizeof(_Tp));
}
template<typename _Tp>
void Mat_<_Tp>::SetColumn(int c, const Vec_<_Tp> &v)
{
	int N = Rows();
	if (N > v.Length())
		N = v.Length();
	for (int i = 0; i < N; i++)
	(*this)[i][c] = v[i];
}
template<typename _Tp>
double Mat_<_Tp>::Det() const
{
	double d;
	assert( Rows()==Cols() );
	if( Rows()==3 )
	{
	d = (double)( data.ptr[0][0]*data.ptr[1][1]*data.ptr[2][2] + data.ptr[0][1]*data.ptr[1][2]*data.ptr[2][0] + data.ptr[0][2]*data.ptr[1][0]*data.ptr[2][1]
	- data.ptr[0][0]*data.ptr[1][2]*data.ptr[2][1] - data.ptr[0][1]*data.ptr[1][0]*data.ptr[2][2] - data.ptr[0][2]*data.ptr[1][1]*data.ptr[2][0]);
	}
	if ( Rows()==2 )
	{
		d = (double)(data.ptr[0][0]*data.ptr[1][1] - data.ptr[1][0]*data.ptr[0][1]);
	}
	if ( Rows()==1 )
		d = (double)data.ptr[0][0];
	if ( Rows()>3 )
	{
		assert( Type()==MAT_Tdouble );
//		LUDecomposition	lu((Mat*)this);
//		d = lu.Determinant();
	}
	return d;
}
template<typename _Tp>
void Mat_<_Tp>::Col(int i, Vec_<_Tp>& vCol) const
{
	int nr = Rows();
	if( nr!= vCol.Length() )
	vCol.Resize( nr );
	for (int r=0;r<nr;r++)
	vCol[r] = (*this)[r][i];
}
template<typename _Tp>
void Mat_<_Tp>::Row(int i, Vec_<_Tp>& vRow) const
{
	int nc = Cols();
	if( nc!= vRow.Length() )
	vRow.Resize( nc );
	memcpy(vRow.data.ptr, data.ptr[i], nc*Step());
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::Submatrix(const int newRows, const int newCols, Mat_<_Tp>& submat, const int i0, const int j0) const
{
	assert(newRows > 0);
	assert(newCols > 0);
	assert(i0 >= 0);
	assert(j0 >= 0);
	assert(i0 + newRows <= Rows());
	assert(j0 + newCols <= Cols());
	if (submat.Rows() != newRows || submat.Cols() != newCols)
	{
		submat.Release();
		submat.Create(newRows, newCols);
	}
	for (int i = 0; i < newRows; i++)
		memcpy(submat.data.ptr[i], &data.ptr[i0+i][j0*Step()], newCols*Step());
	return submat;
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::Submatrix(const int newRows, const int newCols, const int i0, const int j0) const
{
	Mat_<_Tp> submat(newRows, newCols);
	Submatrix(newRows, newCols, submat, i0, j0);
	return submat;
}
template<typename _Tp>
Mat_<_Tp> Mat_<_Tp>::Transposed(void) const
{
	int i, j, n = Rows(), m = Cols();
	Mat_<_Tp> matrixTranspose(m, n);
	assert(m > 0 && n > 0);
	for (i = 0; i < m; i++)
	for (j = 0; j < n; j++)
	matrixTranspose[i][j] = (*this)[j][i];
	return matrixTranspose;
}
template<typename _Tp>
Mat_<_Tp>& Mat_<_Tp>::Transpose(void)
{
	*this = Transposed();
	return *this;
}

}

#endif //_xMAT_H_