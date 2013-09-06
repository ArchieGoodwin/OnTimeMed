/*!
 * \file mat.cpp
 * \ingroup base
 * \brief ÃÔµÛ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */

#include "Mat.h"
#include "CoImage.h"
#include "Inifile.h"
#include "Exception.h"
#include "LUDecomposition.h"
#include "cvlibutil.h"
#include "MatOperation.h"

namespace CVLib 
{

Mat::Mat(int rows, int cols, TYPE type)
{
	this->rows=this->cols=0; data.ptr=NULL;
	Create (rows, cols, type);
}
Mat::Mat(int _rows, int _cols, TYPE _type, const Scalar& _s)
: rows(0), cols(0)
{
	data.ptr=0;
	if(_rows > 0 && _cols > 0)
	{
		Create(_rows, _cols, _type);
		*this = _s;
	}
}

Mat::Mat(void** ptr, int rows, int cols, TYPE type)
{
	this->rows=this->cols=0; data.ptr=NULL;
	Create (ptr, rows, cols, type);
}

Mat::Mat(void* ptr, int rows, int cols, TYPE type, BOOL fAlloc /*= TRUE*/)
{
	this->rows=this->cols=0; data.ptr=NULL;
	Create (ptr, rows, cols, type, fAlloc);
}

Mat::Mat(const Mat& mat)
{
	if (this == &mat)
		return;
	rows=cols=0; data.ptr=NULL;
	Create (mat, TRUE);
}

Mat::Mat(const Mat& mat, const Rect& roi)
{
	rows=cols=0; data.ptr=NULL;
	mat.SubRefMat(roi, *this);
}
Mat::Mat(const Size& size, TYPE type) 
{
	rows=cols=0; data.ptr=NULL;
	Create(size, type);
}

Mat::~Mat ()
{
	Release();
}

int Mat::Create(int rows, int cols, TYPE type)
{
	if (this->rows==rows && this->cols==cols && this->type==type && data.ptr)
		return 1;
	Release();
	int i;
	this->rows = rows;
	this->cols = cols;
	this->type = type;
	int cn=Channels();
	step = CVLIB_ELEM_SIZE(Type());
	int nlineStep=cols * step* cn;
	MALLOC (data.ptr, 4 * rows, uchar*);
	MALLOC (data.ptr[0], rows * nlineStep, uchar);
	for (i = 1; i < rows; i ++)
		data.ptr[i] = data.ptr[0]+i*nlineStep;
	depend = 0;
	return 1;
}

int Mat::Create(const Size& size, TYPE type)
{
	return Create (size.height, size.width, type);
}

int Mat::Create(void** ptr, int rows, int cols, TYPE type)
{
	Release();
	this->rows = rows;
	this->cols = cols;
	this->type = type;
	step = CVLIB_ELEM_SIZE(Type());
	data.ptr=(uchar**)ptr;
	depend = 1;
	return 1;
}

int Mat::Create(void* ptr, int rows, int cols, TYPE type, BOOL fAlloc /*= TRUE*/)
{
	Release();
	int i;
	this->rows = rows;
	this->cols = cols;
	this->type = type;
	int cn=Channels();

	if (fAlloc)
	{
		step = CVLIB_ELEM_SIZE(Type());
		MALLOC (data.ptr, 4 * rows, uchar*);
		data.ptr[0] = (uchar*)ptr;
		int nlinestep=step*cols*cn;
		for (i = 1; i < rows; i ++)
			data.ptr[i] = (uchar*)ptr + i * nlinestep;
		depend = 2;
	}
	else
	{
		step = CVLIB_ELEM_SIZE(Type());
		int nlinestep=cols * cn * step;
		data.ptr = (uchar**)( ((uchar*)ptr) + rows * nlinestep);
		data.ptr[0] = (uchar*)ptr;
		for (i = 1; i < rows; i ++)
			data.ptr[i] = (uchar*)ptr + i * nlinestep;
		depend = 4;
	}
	return 1;
}

int	Mat::Create(Vec& vec)
{
	Release();
	MALLOC (data.ptr, sizeof(BYTE*), BYTE*);
	data.ptr[0] = vec.data.ptr;
	rows = 1;
	cols = vec.Length();
	this->type = vec.type;
	
	depend = 3;
	return 1;
}

int	Mat::Create(const Mat& mA, BOOL fCopy /*= FALSE*/)
{
	Release();
	int nRet = Create (mA.rows, mA.cols, mA.type);
	if (nRet)
	{
		if (fCopy)
		{
			int nlinestep=Step() * Cols() * Channels();
			for (int i = 0;  i < Rows(); i ++)
				memcpy (data.ptr[i], mA.data.ptr[i], nlinestep);
		}
		return 1;
	}
	return 0;
}

void Mat::Release ()
{
	if (data.ptr)
	{
		if (depend == 0)
		{
			FREE (data.ptr[0]);
			FREE (data.ptr);
		}
		else if (depend == 2)
		{
			FREE (data.ptr);
		}
		else if (depend == 3)		//¹ÖÀË¶£Ëæ Ì©ÃÍ ½Û¼¿´ó²÷ °ÒË§
		{
			FREE (data.ptr);
		}
		else if (depend == 4)
		{
		}
		else if (depend == 5)
		{
			FREE (data.ptr);
		}
		data.ptr = 0;
		rows=0;
		cols=0;
	}
}

Mat& Mat::operator=(const Mat& matSrc)
{
	if (this == &matSrc)
		return *this;
	if (rows == matSrc.Rows() && cols == matSrc.Cols() && type == matSrc.type)
	{
		int nlinestep=Cols() * Step() * Channels();
		for (int i = 0; i < Rows(); i ++)
			memcpy(data.ptr[i], matSrc.data.ptr[i], nlinestep);
	}
	else
	{
		Release();
		Create (matSrc, TRUE);
	}
	return *this;
}
static inline void scalarToRawData(const Scalar& s, void* buf, int type, int unroll_to=0)
{
	int depth = CVLIB_MAT_DEPTH(type), cn = CVLIB_MAT_CN(type);
	switch(depth)
	{
	case MAT_Tbyte:
		s.ConvertTo((uchar*)buf, cn, unroll_to);
		break;
	case MAT_Tshort:
		s.ConvertTo((short*)buf, cn, unroll_to);
		break;
	case MAT_Tint:
		s.ConvertTo((int*)buf, cn, unroll_to);
		break;
	case MAT_Tfloat:
		s.ConvertTo((float*)buf, cn, unroll_to);
		break;
	case MAT_Tdouble:
		s.ConvertTo((double*)buf, cn, unroll_to);
		break;
	default:
		assert(false);
	}
}
Mat& Mat::operator=(const Scalar& s)
{
	Size sz = SizeObject();
	uchar** dst = data.ptr;

	sz.width *= (step*Channels());
	if( IsContinuous() )
	{
		sz.width *= sz.height;
		sz.height = 1;
	}

	if( s[0] == 0 && s[1] == 0 && s[2] == 0 && s[3] == 0 )
	{
		for(int i=0; i<sz.height; i++)
			memset( dst[i], 0, sz.width );
	}
	else
	{
		int t = type, esz1 = (int)Step();
		double scalar[12];
		scalarToRawData(s, scalar, t, 12);
		int copy_len = 12*esz1;
		uchar* dst2=dst[0];
		uchar* dst_limit = dst2 + sz.width;

		if( sz.height-- )
		{
			while( dst2 + copy_len <= dst_limit )
			{
				memcpy( dst2, scalar, copy_len );
				dst2 += copy_len;
			}
			memcpy( dst2, scalar, dst_limit - dst2 );
		}

		if( sz.height > 0 )
		{
			int nlinestep=sz.width*rows;
			dst2 = dst_limit - sz.width + step;
			for( ; sz.height--; dst2 += step )
				memcpy( dst2, data.ptr[0], sz.width );
		}
	}
	return *this;
}

void Mat::Zero ()
{
	int nsize=step * Cols() * Channels();
	for (int i = 0; i < Rows(); i ++)
		memset (data.ptr[i], 0, nsize);
}

double	Mat::Value (int i, int j, int cn/*=0*/) const
{
	switch (Type())
	{
	case MAT_Tbyte:
		return data.ptr[i][j*Channels()+cn];
	case MAT_Tshort:
		return data.s[i][j*Channels()+cn];
	case MAT_Tint:
		return data.i[i][j*Channels()+cn];
	case MAT_Tfloat:
		return data.fl[i][j*Channels()+cn];
	case MAT_Tdouble:
		return data.db[i][j*Channels()+cn];
	}
	return 0.0f;
}

Mat* Mat::SubMat(int nRow1, int nCol1, int nRow2, int nCol2) const
{
	int nNewRows = nRow2 - nRow1 + 1;
	int nNewCols = nCol2 - nCol1 + 1;
	if (nRow1 < 0 || nRow1 >= Rows() || nRow2 < 0 || nRow2 >= Rows() ||
		nCol1 < 0 || nCol1 >= Cols() || nCol2 < 0 || nCol2 >= Cols() || 
		nNewRows <= 0 || nNewCols <= 0)
		return 0;
	Mat* pmRet = new Mat(nNewRows, nNewCols, type);
	int cn=Channels();
	int nlinestep=Step()*nNewCols*cn;
	for (int i = 0; i < nNewRows; i ++)
		memcpy(pmRet->data.ptr[i], data.ptr[i+nRow1]+nCol1*Step()*cn, nlinestep);
	return pmRet;
}
Mat* Mat::SubMat(const Rect& region) const
{
	Mat* psub=new Mat(region.SizeObject(), Type1());
	int datalength=CVLIB_ELEM_SIZE(Type())*Channels()*region.width;
	int offset=region.x*CVLIB_ELEM_SIZE(Type())*Channels();
	for (int i=0; i<region.height; i++)
		memcpy(psub->data.ptr[i], &data.ptr[i+region.y][offset], datalength);
	return psub;
}
Mat* Mat::SubRefMat(int nRow1, int nCol1, int nRow2, int nCol2) const
{
	if (nRow1 > nRow2 || nCol1 > nCol2)
		return NULL;
	int nNewRows = nRow2 - nRow1 + 1;
	int nNewCols = nCol2 - nCol1 + 1;
	Mat* pmRet = new Mat;
	MALLOC (pmRet->data.ptr, sizeof(BYTE*) * nNewRows, BYTE*);

	for (int i = 0; i < nNewRows; i ++)
		pmRet->data.ptr[i] = &data.ptr[i+nRow1][nCol1*Step()*Channels()];
	pmRet->ResizeRows(nNewRows);
	pmRet->ResizeCols(nNewCols);
	pmRet->depend = 5;
	pmRet->type = type;

	return pmRet;
}
void Mat::SubRefMat(const Rect& region, Mat& mRef) const
{
	if (mRef.SizeObject()!=region.SizeObject() || mRef.depend!=5)
	{
		mRef.Release();
		MALLOC (mRef.data.ptr, sizeof(BYTE*) * region.height, BYTE*);
		mRef.ResizeRows(region.height);
		mRef.ResizeCols(region.width);
		mRef.depend = 5;
		mRef.type = type;
		mRef.step = step;
	}
	for (int i = 0; i < region.height; i ++)
		mRef.data.ptr[i] = &data.ptr[i+region.y][region.x*Step()*Channels()];
}
void Mat::SubMat(const Rect& region, Mat& mRef) const
{
	if (mRef.SizeObject()!=region.SizeObject())
	{
		mRef.Release();
		mRef.Create(region.SizeObject(), type);
	}
	int nlinestep=Step()*Channels()*region.width;
	for (int i = 0; i < region.height; i ++)
		memcpy(mRef.data.ptr[i], &data.ptr[i+region.y][region.x*Step()*Channels()], nlinestep);
}

int Mat::FromFile (const char* szFilename)
{
	return Object::FromFile(szFilename);
}

int Mat::ToFile (const char* szFilename) const
{
	return Object::ToFile(szFilename);
}

int Mat::FromFile (FILE* pFile)
{
	if (IsValid())
		Release ();
	if (pFile == NULL)
		return 0;
	int nRows, nCols, nType;
	fread (&nRows, sizeof(int), 1, pFile);
	fread (&nCols, sizeof(int), 1, pFile);
	fread (&nType, sizeof(int), 1, pFile);
	
	Create(nRows, nCols, (TYPE)nType);
	int nlinestep=nCols * Step() * Channels();
	for (int i = 0; i < nRows; i ++)
		fread (data.ptr[i], nlinestep, 1, pFile);
	
	return 1;
}

int Mat::ToFile (FILE* pFile) const
{
	assert (IsValid());
	if (pFile != NULL)
	{
		int nRows, nCols, nType;
		nRows = Rows();
		nCols = Cols();
		fwrite (&nRows, sizeof(int), 1, pFile);
		fwrite (&nCols, sizeof(int), 1, pFile);
		nType = type;
		fwrite (&nType, sizeof(int), 1, pFile);
		
		int nlinestep=nCols * Step() * Channels();
		for (int i = 0; i < nRows; i ++)
			fwrite (data.ptr[i], nlinestep, 1, pFile);
	}
	
	return 1;
}

void Mat::Identity(double val)
{
	Zero();
	int ii, k;
	int t = MIN(rows, cols);
	int cn=Channels();
	if (cn==1)
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (ii = 0; ii < t; ii ++)
				data.ptr[ii][ii] = (BYTE)val;
			break;
		case MAT_Tshort:
			for (ii = 0; ii < t; ii ++)
				data.s[ii][ii] = (short)val;
			break;
		case MAT_Tint:
			for (ii = 0; ii < t; ii ++)
				data.i[ii][ii] = (int)val;
			break;
		case MAT_Tfloat:
			for (ii = 0; ii < t; ii ++)
				data.fl[ii][ii] = (float)val;
			break;
		case MAT_Tdouble:
			for (ii = 0; ii < t; ii ++)
				data.db[ii][ii] = (double)val;
			break;
		}
	}
	else
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.ptr[ii][ii*cn+k] = (BYTE)val;
			break;
		case MAT_Tshort:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.s[ii][ii*cn+k] = (short)val;
			break;
		case MAT_Tint:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.i[ii][ii*cn+k] = (int)val;
			break;
		case MAT_Tfloat:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.fl[ii][ii*cn+k] = (float)val;
			break;
		case MAT_Tdouble:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.db[ii][ii*cn+k] = (double)val;
			break;
		}
	}
}
void Mat::Identity()
{
	Zero();
	int ii, k;
	int t = MIN(rows, cols);
	int cn=Channels();

	if (cn==1)
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (ii = 0; ii < t; ii ++)
				data.ptr[ii][ii] = (BYTE)1;
			break;
		case MAT_Tshort:
			for (ii = 0; ii < t; ii ++)
				data.s[ii][ii] = (short)1;
			break;
		case MAT_Tint:
			for (ii = 0; ii < t; ii ++)
				data.i[ii][ii] = (int)1;
			break;
		case MAT_Tfloat:
			for (ii = 0; ii < t; ii ++)
				data.fl[ii][ii] = (float)1;
			break;
		case MAT_Tdouble:
			for (ii = 0; ii < t; ii ++)
				data.db[ii][ii] = (double)1;
			break;
		}
	}
	else
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.ptr[ii][ii*cn+k] = (BYTE)1;
			break;
		case MAT_Tshort:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.s[ii][ii*cn+k] = (short)1;
			break;
		case MAT_Tint:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.i[ii][ii*cn+k] = (int)1;
			break;
		case MAT_Tfloat:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.fl[ii][ii*cn+k] = (float)1;
			break;
		case MAT_Tdouble:
			for (ii = 0; ii < t; ii ++) for(k=0;k<cn;k++)
				data.db[ii][ii*cn+k] = (double)1;
			break;
		}
	}
}
double Mat::Min(int cn) const
{
	double rMin = 1e10;
	int i = 0, j = 0;
	int cnsize=Channels();
	if (cnsize==1)
	{
		switch(Type())
		{
		case MAT_Tbyte:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.ptr[i][j])
						rMin = data.ptr[i][j];
			return rMin;
		case MAT_Tshort:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.s[i][j])
						rMin = data.s[i][j];
			return rMin;
		case MAT_Tint:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.i[i][j])
						rMin = data.i[i][j];
			return rMin;
		case MAT_Tfloat:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.fl[i][j])
						rMin = data.fl[i][j];
			return rMin;
		case MAT_Tdouble:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.db[i][j])
						rMin = data.db[i][j];
			return rMin;
		}
	}
	else
	{
		switch(Type())
		{
		case MAT_Tbyte:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.ptr[i][j*cnsize+cn])
						rMin = data.ptr[i][j*cnsize+cn];
			return rMin;
		case MAT_Tshort:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.s[i][j*cnsize+cn])
						rMin = data.s[i][j*cnsize+cn];
			return rMin;
		case MAT_Tint:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.i[i][j*cnsize+cn])
						rMin = data.i[i][j*cnsize+cn];
			return rMin;
		case MAT_Tfloat:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.fl[i][j*cnsize+cn])
						rMin = data.fl[i][j*cnsize+cn];
			return rMin;
		case MAT_Tdouble:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMin > data.db[i][j*cnsize+cn])
						rMin = data.db[i][j*cnsize+cn];
			return rMin;
		}
	}

	return rMin;
}

double Mat::Max(int cn) const
{
	double rMax = -1e10;
	int i = 0, j = 0;
	int cnsize=Channels();
	if (cnsize==1)
	{
		switch(Type())
		{
		case MAT_Tbyte:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.ptr[i][j])
						rMax = data.ptr[i][j];
			return rMax;
		case MAT_Tshort:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.s[i][j])
						rMax = data.s[i][j];
			return rMax;
		case MAT_Tint:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.i[i][j])
						rMax = data.i[i][j];
			return rMax;
		case MAT_Tfloat:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.fl[i][j])
						rMax = data.fl[i][j];
			return rMax;
		case MAT_Tdouble:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.db[i][j])
						rMax = data.db[i][j];
			return rMax;
		}	
	}
	else
	{
		switch(Type())
		{
		case MAT_Tbyte:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.ptr[i][j*cnsize+cn])
						rMax = data.ptr[i][j*cnsize+cn];
			return rMax;
		case MAT_Tshort:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.s[i][j*cnsize+cn])
						rMax = data.s[i][j*cnsize+cn];
			return rMax;
		case MAT_Tint:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.i[i][j*cnsize+cn])
						rMax = data.i[i][j*cnsize+cn];
			return rMax;
		case MAT_Tfloat:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.fl[i][j*cnsize+cn])
						rMax = data.fl[i][j*cnsize+cn];
			return rMax;
		case MAT_Tdouble:
			for (i = 0; i < Rows(); i ++)
				for (j = 0; j < Cols(); j ++)
					if (rMax < data.db[i][j*cnsize+cn])
						rMax = data.db[i][j*cnsize+cn];
			return rMax;
		}	
	}
	return rMax;
}

double Mat::Min(int& row, int& col) const
{
	double rMin = 1e10;
	int i = 0, j = 0;
	row=col=0;
	switch(Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
			{
				if (rMin > data.ptr[i][j])
				{
					rMin = data.ptr[i][j];
					row=i;
					col=j;
				}
			}
		return rMin;
	case MAT_Tshort:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMin > data.s[i][j])
				{
					rMin = data.s[i][j];
					row=i;
					col=j;
				}
		return rMin;
	case MAT_Tint:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMin > data.i[i][j])
				{
					rMin = data.i[i][j];
					row=i;
					col=j;
				}
		return rMin;
	case MAT_Tfloat:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMin > data.fl[i][j])
				{
					rMin = data.fl[i][j];
					row=i;
					col=j;
				}
		return rMin;
	case MAT_Tdouble:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMin > data.db[i][j])
				{
					rMin = data.db[i][j];
					row=i;
					col=j;
				}
		return rMin;
	}
	return rMin;
}

double Mat::Max(int& row, int& col) const
{
	double rMax = -1e10;
	int i = 0, j = 0;
	row=col=0;
	switch(Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < Rows(); i ++) for (j = 0; j < Cols(); j ++)
		if (rMax < data.ptr[i][j]) 
		{
			rMax = data.ptr[i][j];
			row=i;
			col=j;
		}
		return rMax;
	case MAT_Tshort:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMax < data.s[i][j])
				{
					rMax = data.s[i][j];
					row=i;
					col=j;
				}
		return rMax;
	case MAT_Tint:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMax < data.i[i][j])
				{
					rMax = data.i[i][j];
					row=i;
					col=j;
				}
		return rMax;
	case MAT_Tfloat:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMax < data.fl[i][j])
				{
					rMax = data.fl[i][j];
					row=i;
					col=j;
				}
		return rMax;
	case MAT_Tdouble:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				if (rMax < data.db[i][j])
				{
					rMax = data.db[i][j];
					row=i;
					col=j;
				}
		return rMax;
	}	
	return rMax;
}

double Mat::Mean() const
{
	double rMean = 0.0;
	double rF = 1.0 / (Rows() * Cols());
	int i = 0, j = 0;
	switch(Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				rMean += data.ptr[i][j];
		return rMean * rF;
	case MAT_Tshort:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				rMean += data.s[i][j];
		return rMean * rF;
	case MAT_Tint:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				rMean += data.i[i][j];
		return rMean * rF;
	case MAT_Tfloat:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				rMean += data.fl[i][j];
		return rMean * rF;
	case MAT_Tdouble:
		for (i = 0; i < Rows(); i ++)
			for (j = 0; j < Cols(); j ++)
				rMean += data.db[i][j];
		return rMean * rF;
	}	
	return rMean;
}

Mat& Mat::operator=(double value)
{
	if (fabs(value) < 1e-10)
		Zero();
	else
		SetValue(value);
	return *this;
}

void Mat::SetValue (double rVal)
{
	int i, j;
	int nelems=Cols()*Channels();
	switch (Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < Rows(); i ++)
		{
			for (j = 0; j < nelems; j ++)
				data.ptr[i][j] = (BYTE)rVal;
		}
		break;
	case MAT_Tshort:
		for (i = 0; i < Rows(); i ++)
		{
			for (j = 0; j < nelems; j ++)
				data.s[i][j] = (short)rVal;
		}
		break;
	case MAT_Tint:
		for (i = 0; i < Rows(); i ++)
		{
			for (j = 0; j < nelems; j ++)
				data.i[i][j] = (int)rVal;
		}
		break;
	case MAT_Tfloat:
		for (i = 0; i < Rows(); i ++)
		{
			for (j = 0; j < nelems; j ++)
				data.fl[i][j] = (float)rVal;
		}
		break;
	case MAT_Tdouble:
		for (i = 0; i < Rows(); i ++)
		{
			for (j = 0; j < nelems; j ++)
				data.db[i][j] = (double)rVal;
		}
		break;
	}
}

bool Mat::ToImage(CoImage& image) const
{
	int i, j;
	int cn=Channels();
	image.Release();
	if (cn==1)
	{
		if (Type() == MAT_Tbyte)
		{
			image.Create (*this, *this, *this);
		}
		else
		{
			image.Create (rows, cols, MAT_Tbyte);
			double rMin1;
			double rMax1;
			rMin1 = Min();
			rMax1 = Max();
			float rExpand = 255.0f / (float)(rMax1 - rMin1);
			for (i = 0; i < Rows(); i ++)
			{
				for (j = 0; j < Cols(); j ++)
				{
					double rV = (Value(i,j) - rMin1) * rExpand;
					image.m_matX.data.ptr[i][j] = (BYTE)MIN(MAX(rV, 0),255);
					image.m_matY.data.ptr[i][j]=image.m_matZ.data.ptr[i][j]=image.m_matX.data.ptr[i][j];
				}
			}
		}
	}
	else if (cn==3)
	{
		if (Type() == MAT_Tbyte)
		{
			image.Create (rows, cols, MAT_Tbyte);
			for (i=0;i<rows; i++) for (j=0;j<cols;j++)
			{
				image.m_matX.data.ptr[i][j]=data.ptr[i][j*3];
				image.m_matY.data.ptr[i][j]=data.ptr[i][j*3+1];
				image.m_matZ.data.ptr[i][j]=data.ptr[i][j*3+2];
			}
		}
		else
		{
			image.Create (rows, cols, MAT_Tbyte);
			Mat** pmats=image.GetBand();
			double rMin1[3];
			double rMax1[3];
			for (int k=0; k<cn; k++)
			{
				Mat* pmBand=pmats[k];
				rMin1[k] = Min(k);
				rMax1[k] = Max(k);
				float rExpand = 255.0f / (float)(rMax1[k] - rMin1[k]);
				for (i = 0; i < rows; i ++)
				{
					for (j = 0; j < cols; j ++)
					{
						double rV = (Value(i,j,k) - rMin1[k]) * rExpand;
						pmBand->data.ptr[i][j] = (BYTE)MIN(MAX(rV, 0),255);
					}
				}
			}
		}
	}

	return true;
}

bool Mat::FromImage(const char* szFilename, DWORD nImageType)
{
	CoImage image;
	image.Load(szFilename, nImageType);
	if (image.IsValid())
	{
		TYPE type= (TYPE)((int)image.m_matX.Type()+(2<<CVLIB_CN_SHIFT));
		Create(image.GetHeight(),image.GetWidth(),type);
		int i,j,d=0;
		switch (image.m_matX.Type())
		{
		case MAT_Tbyte:
			{
				uchar* pdata=data.ptr[0];
				for (i=0;i<rows; i++) 
				{
					for (j=0;j<cols;j++,d+=3)
					{
						pdata[d]=image.m_matX.data.ptr[i][j];
						pdata[d+1]=image.m_matY.data.ptr[i][j];
						pdata[d+2]=image.m_matZ.data.ptr[i][j];
					}
				}
			}
			break;
		case MAT_Tshort:
			{
				short* pdata=data.s[0];
				for (i=0;i<rows; i++)
				{
					for (j=0;j<cols;j++, d+=3)
					{
						pdata[d]=image.m_matX.data.s[i][j];
						pdata[d+1]=image.m_matY.data.s[i][j];
						pdata[d+2]=image.m_matZ.data.s[i][j];
					}
				}
			}
			break;
		case MAT_Tint:
			{
				int* pdata=data.i[0];
				for (i=0;i<rows; i++)
				{
					for (j=0;j<cols;j++, d+=3)
					{
						pdata[d]=image.m_matX.data.i[i][j];
						pdata[d+1]=image.m_matY.data.i[i][j];
						pdata[d+2]=image.m_matZ.data.i[i][j];
					}
				}
			}
			break;
		case MAT_Tfloat:
			{
				float* pdata=data.fl[0];
				for (i=0;i<rows; i++)
				{
					for (j=0;j<cols;j++, d+=3)
					{
						pdata[d]=image.m_matX.data.fl[i][j];
						pdata[d+1]=image.m_matY.data.fl[i][j];
						pdata[d+2]=image.m_matZ.data.fl[i][j];
					}
				}	
			}
			break;
		case MAT_Tdouble:
			{
				double* pdata=data.db[0];
				for (i=0;i<rows; i++)
				{
					for (j=0;j<cols;j++, d+=3)
					{
						pdata[d]=image.m_matX.data.db[i][j];
						pdata[d+1]=image.m_matY.data.db[i][j];
						pdata[d+2]=image.m_matZ.data.db[i][j];
					}
				}	
			}
			break;
		}
	}
	return image.IsValid();
}

bool Mat::ToImage (const char* szFilename, DWORD nImageType) const
{
	int cn=Channels();
	int i, j, k;
	CoImage image(rows, cols, MAT_Tbyte);
	Mat** pmbands=image.GetBand();
	if (cn==1)
	{
		for (i=0;i<rows;i++) for (j=0; j<cols; j++)
		{
			double rV = Value(i,j);
			pmbands[0]->data.ptr[i][j] = (BYTE)MIN(MAX(rV,0),255);
			pmbands[1]->data.ptr[i][j]=pmbands[0]->data.ptr[i][j];
			pmbands[2]->data.ptr[i][j]=pmbands[0]->data.ptr[i][j];
		}
	}
	else
	{
		for (i=0;i<rows;i++) for (j=0; j<cols; j++) for(k=0;k<cn;k++)
		{
			double rV = Value(i,j,k);
			pmbands[k]->data.ptr[i][j] = (BYTE)MIN(MAX(rV,0),255);
		}
	}
	return image.Save (szFilename, nImageType);
}
bool Mat::ToImage (const char* szFilename, DWORD nImageType, 
				   BOOL fExpand, float rMin, float rMax) const
{
	int i, j;
	CoImage image(rows, cols, MAT_Tbyte);
	Mat** pmbands=image.GetBand();
	double rMin1;
	double rMax1;
	if (fExpand)
	{
		rMin1 = Min();
		rMax1 = Max();
	}
	else
	{
		rMin1 = rMin;
		rMax1 = rMax;
	}
	float rExpand = 255.0f / (float)(rMax1 - rMin1);
	for (i=0;i<rows;i++) for (j=0; j<cols; j++) 
	{
		double rV = (Value(i,j) - rMin1) * rExpand;
		pmbands[0]->data.ptr[i][j] = (BYTE)MIN(MAX(rV, 0),255);
		pmbands[1]->data.ptr[i][j] = pmbands[0]->data.ptr[i][j];
		pmbands[2]->data.ptr[i][j] = pmbands[0]->data.ptr[i][j];
	}	
	return image.Save (szFilename, nImageType);
}
bool Mat::ToImage (const char* szFilename, DWORD nImageType, 
					   BOOL fExpand, float rMin[]/*={0.0f}*/, float rMax[]/*={1.0f}*/) const
{
	int i, j, k;
	int cn=Channels();
	CoImage image(rows, cols, MAT_Tbyte);
	Mat** pmbands=image.GetBand();
	for(k=0;k<cn;k++) 
	{
		if (cn==3)
		{
			Mat* pm=pmbands[k];
			double rMin1;
			double rMax1;
			if (fExpand)
			{
				rMin1 = Min(k);
				rMax1 = Max(k);
			}
			else
			{
				rMin1 = rMin[k];
				rMax1 = rMax[k];
			}
			float rExpand = 255.0f / (float)(rMax1 - rMin1);
			for (i=0;i<rows;i++) for (j=0; j<cols; j++) 
			{
				double rV = (Value(i,j,k) - rMin1) * rExpand;
				pm->data.ptr[i][j] = (BYTE)MIN(MAX(rV, 0),255);
			}
		}
		else
		{
			Mat* pm=pmbands[k];
			if (k==0)
			{
				double rMin1;
				double rMax1;
				if (fExpand)
				{
					rMin1 = Min(k);
					rMax1 = Max(k);
				}
				else
				{
					rMin1 = rMin[k];
					rMax1 = rMax[k];
				}
				float rExpand = 255.0f / (float)(rMax1 - rMin1);
				for (i=0;i<rows;i++) for (j=0; j<cols; j++) 
				{
					double rV = (Value(i,j,k) - rMin1) * rExpand;
					pm->data.ptr[i][j] = (BYTE)MIN(MAX(rV, 0),255);
				}	
			}
			else
			{
				for (i=0;i<rows;i++) for (j=0; j<cols; j++) 
					pm->data.ptr[i][j] = pmbands[0]->data.ptr[i][j];
			}
		}
	}
	return image.Save (szFilename, nImageType);
}

void Mat::ToMatlab(const SString& sFilename, const SString& sName, const SString& sComment, bool fAppend) const
{
	char		strTmp[1024];
	FILE* pFile = NULL;
	if(fAppend)
		pFile = fopen(sFilename, "a++");
	else 
		pFile = fopen(sFilename, "w");
	
	if(sComment!="")
		fprintf (pFile, "%%%s\n", (const char*)sComment);
	fprintf(pFile, "%s=[\n", (const char*)sName);
	for(int i=0;i<Rows();i++)
	{
		for(int j=0;j<Cols();j++)
		{
			double rValue=Value(i, j);
			sprintf (strTmp, "%6.6f ", rValue);
			fprintf(pFile, "%s", strTmp);
		}
		fprintf(pFile, ";\n");
	}
	fprintf(pFile, "];\n");
	fclose (pFile);
}

void Mat::FromMatlab(const SString& sFilename, const SString& sName)
{
	TokenFile	DIVABuf;					//Buffer or stream to the file.
	SString		sTokenString;				//String for retriving tokings, or element entities from DIVAbuf.
	bool		bFound=false;				//Indicator for the soughtafter matrix has been found.
	Array<double,double>	Entry;			//Container for the entries or numbers untile size om matrix is known.
	int			cCols=0;					//Number of columns in the matrix.
	int			cRows=0;					//Number of rows in the matrix.
	int			cEntry=0;					//Counter for wich entry is to be writen to the matrix.
	
	//Open stream/buffer.
	if(! DIVABuf.Open(sFilename,"w"))
		throw Exception("The requested file does not exist.",0,"FromMatlab","Mat.cpp", __LINE__);
	
	
	//Find the sought after matrix or fail.
	do{
		if(!DIVABuf.NextToken(sTokenString))
		{
			throw Exception("The requested variable is not in the requested file.",0,"FromMatlab","Mat.cpp", __LINE__);
		}
	}while(sTokenString!=sName+"=[");
	
	
	//Parse the first row of the matrix, and there by retrive the number of columns.
	//Get a token from the stream.
	if(!DIVABuf.NextToken(sTokenString))
	{
		throw Exception("Error reading from file.",0,"FromMatlab","Mat.cpp", __LINE__);
	}
	while(sTokenString!=SString(";"))				//";" marks the end of a line.
	{
		Entry.Add(atof((const char*)(sTokenString)));		
		//Get a token from the stream.
		if(!DIVABuf.NextToken(sTokenString))
		{
			throw Exception("Error reading from file.",0,"FromMatlab","Mat.cpp", __LINE__);
		}
		cCols++;
	}
	
	//Parse the rest of the matrix 
	while(sTokenString!=SString("];"))				//"];" marks the end of a matrix.
	{
		if(sTokenString==SString(";"))				//";" marks the end of a line.
		{
			cRows++;
		}
		else
		{
			Entry.Add(atof((const char*)(sTokenString)));
		}
		//Get a token from the stream.
		if(!DIVABuf.NextToken(sTokenString))
		{
			throw Exception("Error reading from file.",0,"FromMatlab","Mat.cpp", __LINE__);
		}
	}
	
	//Adjust the size of the (output)matrix.
	Create(cRows,cCols, MAT_Tdouble);
	
	//Transfere the values.
	for(int cI=0;cI<cRows;cI++)
	{
		for(int cJ=0;cJ<cCols;cJ++)
		{
			data.db[cI][cJ]=Entry[cEntry];
			cEntry++;
		}
	}
}

Mat Mat::Inverted(void) const
{
	assert (Type() == MAT_Tfloat || Type() == MAT_Tdouble);
	assert (Rows() == Cols());
	int nDim = Rows();
	Mat mInverted;
	switch (Type())
	{
	case MAT_Tfloat:
		{
			Mat mtemp(*this);
			mtemp.Convert(MAT_Tdouble);
			LUDecomposition lu(&mtemp);
			double rDet = lu.Determinant();
			if( lu.IsNonSingular() )
			{
				Mat mB(nDim, nDim, MAT_Tdouble);
				mB.Identity();
				Mat* pmI = lu.Solve(&mB);
				mInverted.Create (*pmI, TRUE);
				mInverted.Convert(MAT_Tfloat);
				delete pmI;
			}
			else
			{
				assert(false);
			}
		}
		break;
	case MAT_Tdouble:
		{
			LUDecomposition lu(this);
			double rDet = lu.Determinant();
			if( lu.IsNonSingular() )
			{
				Mat mB(nDim, nDim, MAT_Tdouble);
				mB.Identity();
				Mat* pmI = lu.Solve(&mB);
				mInverted.Create (*pmI, TRUE);
				delete pmI;
			}
			else
			{
				assert(false);
			}
		}
		break;
	}
	return mInverted;
}

Mat& Mat::Invert(void)
{
	assert (Type() == MAT_Tfloat || Type() == MAT_Tdouble);
	*this = Inverted();
	return *this;
}

void Mat::Diag(const Vec& vec)
{
	int stop=MIN(MIN(Rows(),Cols()),vec.Length() );

	int i;
	Zero();
	switch(Type())
	{
	case MAT_Tbyte:
		for(i=0;i<stop;i++)
			data.ptr[i][i]=vec.data.ptr[i];
		break;
	case MAT_Tshort:
		for(i=0;i<stop;i++)
			data.s[i][i]=vec.data.s[i];
		break;
	case MAT_Tint:
		for(i=0;i<stop;i++)
			data.i[i][i]=vec.data.i[i];
		break;
	case MAT_Tfloat:
		for(i=0;i<stop;i++)
			data.fl[i][i]=vec.data.fl[i];
		break;
	case MAT_Tdouble:
		for(i=0;i<stop;i++)
			data.db[i][i]=vec.data.db[i];
		break;
	}
}

void	Mat::ToC(const SString& szFilename, const SString& szVarName, bool fAppend) const
{
	if (Rows() == 0)
		return;
	FILE* pFile = NULL;
	int* pan = new int[Cols()];
	if(fAppend)
		pFile = fopen(szFilename, "a++");
	else 
		pFile = fopen(szFilename, "w");
	
	fprintf(pFile, "int %s[%d][%d]={", (const char*)szVarName, Rows(), Cols());
	
	int i;
	for(i=0;i<Rows();i++)
	{
		fprintf (pFile, "\n\t{");
		int j;
		for (j=0; j<Cols(); j++)
		{
			switch(Type())
			{
			case MAT_Tbyte:
				pan[j] = data.ptr[i][j];
				break;
			case MAT_Tshort:
				pan[j] = data.s[i][j];
				break;
			case MAT_Tint:
				pan[j] = data.i[i][j];
				break;
			case MAT_Tfloat:
				pan[j] = CVUtil::Round(data.fl[i][j] * 1048576);
				break;
			case MAT_Tdouble:
				pan[j] = CVUtil::Round(data.db[i][j] * 1048576);
				break;
			}
		}
		for (j=0; j<Cols()-1; j++)
			fprintf (pFile, "%d,", pan[j]);
		fprintf (pFile, "%d},", pan[Cols()-1]);
	}
	fprintf (pFile, "};\n");
	fclose (pFile);
	delete []pan;
}

void Mat::ToVec(Vec& v) const
{
	if (!v.IsValid())
		v.Create (Rows()*Cols(), Type());
	memcpy (v.data.ptr, data.ptr[0], Step()* Rows()*Cols());
}
Mat Mat::Transposed(void) const
{
	Mat mRet;
	mRet.Create(this->Cols(),this->Rows(),this->Type());
	MatOp::Transpose(&mRet, this);
	return mRet;
}
Mat& Mat::Transpose(void)
{
	MatOp::Transpose(this);
	return *this;
}
bool Mat::IsSymmetric(void) const
{
	if (Rows() != Cols())
		return false;
	int i, j;
	switch (Type())
	{
	case MAT_Tbyte:
		for (i = 0; i < Rows(); i++)
			for (j = i+1; j < Cols(); j++)
				if (data.ptr[i][j] != data.ptr[j][i])
					return false;
		break;
	case MAT_Tshort:
		for (i = 0; i < Rows(); i++)
			for (j = i+1; j < Cols(); j++)
				if (data.s[i][j] != data.s[j][i])
					return false;
		break;
	case MAT_Tint:
		for (i = 0; i < Rows(); i++)
			for (j = i+1; j < Cols(); j++)
				if (data.i[i][j] != data.i[j][i])
					return false;
		break;
	case MAT_Tfloat:
		for (i = 0; i < Rows(); i++)
			for (j = i+1; j < Cols(); j++)
				if (data.fl[i][j] != data.fl[j][i])
					return false;
		break;
	case MAT_Tdouble:
		for (i = 0; i < Rows(); i++)
			for (j = i+1; j < Cols(); j++)
				if (data.db[i][j] != data.db[j][i])
					return false;
		break;
	}
	return true;
}


void Mat::MeanCol(Vec& vMean) const
{
	assert (false);
}

double Mat::Std() const
{
	return sqrt(Var());
}

void Mat::StdCol(Vec& vStd) const
{
	assert (false);
}

double Mat::Sum() const
{
	int i, j;
	double dSum=0;
	if (IsContinuous())
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (i=0;i < Rows()*Cols(); i++)
				dSum+=(double)data.ptr[0][i];
			break;
		case MAT_Tshort:
			for (i=0;i < Rows()*Cols(); i++)
				dSum+=(double)data.s[0][i];
			break;
		case MAT_Tint:
			for (i=0;i < Rows()*Cols(); i++)
				dSum+=(double)data.i[0][i];
			break;
		case MAT_Tfloat:
			for (i=0;i < Rows()*Cols(); i++)
				dSum+=(double)data.fl[0][i];
			break;
		case MAT_Tdouble:
			for (i=0;i < Rows()*Cols(); i++)
				dSum+=(double)data.db[0][i];
			break;
		}
	}
	else
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dSum+=(double)data.ptr[i][j];
			break;
		case MAT_Tshort:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dSum+=(double)data.s[i][j];
			break;
		case MAT_Tint:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dSum+=(double)data.i[i][j];
			break;
		case MAT_Tfloat:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dSum+=(double)data.fl[i][j];
			break;
		case MAT_Tdouble:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dSum+=(double)data.db[i][j];
			break;
		}
	}
	return dSum;
}

void Mat::SumCol(Vec& vSum) const
{
	assert (false);
}

double Mat::Var() const
{
	int i, j;
	double dVar = 0, dMean = Mean();
	if (IsContinuous())
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (i=0;i < Rows()*Cols(); i++)
				dVar+=SQR((double)data.ptr[0][i]-dMean);
			break;
		case MAT_Tshort:
			for (i=0;i < Rows()*Cols(); i++)
				dVar+=SQR((double)data.s[0][i]-dMean);
			break;
		case MAT_Tint:
			for (i=0;i < Rows()*Cols(); i++)
				dVar+=SQR((double)data.i[0][i]-dMean);
			break;
		case MAT_Tfloat:
			for (i=0;i < Rows()*Cols(); i++)
				dVar+=SQR((double)data.fl[0][i]-dMean);
			break;
		case MAT_Tdouble:
			for (i=0;i < Rows()*Cols(); i++)
				dVar+=SQR((double)data.db[0][i]-dMean);
			break;
		}
	}
	else
	{
		switch (Type())
		{
		case MAT_Tbyte:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dVar+=SQR((double)data.ptr[i][j]-dMean);
			break;
		case MAT_Tshort:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dVar+=SQR((double)data.ptr[i][j]-dMean);
			break;
		case MAT_Tint:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dVar+=SQR((double)data.ptr[i][j]-dMean);
			break;
		case MAT_Tfloat:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dVar+=SQR((double)data.ptr[i][j]-dMean);
			break;
		case MAT_Tdouble:
			for (i=0;i < Rows(); i++) for (j=0; j<Cols(); j++)
				dVar+=SQR((double)data.ptr[i][j]-dMean);
			break;
		}
	}
	return dVar;
}

void Mat::VarCol(Vec& vVar) const
{
	assert (false);
}

// matrix manipulation
void Mat::ColVec(int i, Vec& vCol) const
{
	assert (false);
}

void Mat::RowVec(int i, Vec& vRow) const
{
	assert (false);
}

void Mat::CombVert(Mat& Top,Mat& Bottom)
{
	assert (false);
}

SString Mat::ToString() const
{
	SString strOut = "";
	
	SString strTmp;
	int i, k;
	switch (Type())
	{
	case MAT_Tbyte:
		for(i=0;i<Rows();i++)
		{
			for(k=0;k<Cols();k++)
			{
				strTmp.Format("%d ",data.ptr[i][k]);
				strOut += strTmp;
			}
			strOut += "\r\n";
		}
		break;
	case MAT_Tshort:
		for(i=0;i<Rows();i++)
		{
			for(k=0;k<Cols();k++)
			{
				strTmp.Format("%d ",data.s[i][k]);
				strOut += strTmp;
			}
			strOut += "\r\n";
		}
		break;
	case MAT_Tint:
		for(i=0;i<Rows();i++)
		{
			for(k=0;k<Cols();k++)
			{
				strTmp.Format("%d ",data.i[i][k]);
				strOut += strTmp;
			}
			strOut += "\r\n";
		}
		break;
	case MAT_Tfloat:
		for(i=0;i<Rows();i++)
		{
			for(k=0;k<Cols();k++)
			{
				strTmp.Format("%.6f ",data.fl[i][k]);
				strOut += strTmp;
			}
			strOut += "\r\n";
		}
		break;
	case MAT_Tdouble:
		for(i=0;i<Rows();i++)
		{
			for(k=0;k<Cols();k++)
			{
				strTmp.Format("%.6f ",data.db[i][k]);
				strOut += strTmp;
			}
			strOut += "\r\n";
		}
		break;
	}
	
    return strOut;
}

void Mat::ElementDivide(const Mat& matrix)
{
	assert (false);
}

void Mat::ElementMultiply(const Mat& matrix)
{
	assert (false);
}

// math functions
void Mat::Sqr()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Rows()*Cols();j++)
			data.ptr[0][j] = (BYTE)SQR(data.ptr[0][j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Rows()*Cols();j++)
			data.s[0][j] = (short)SQR(data.s[0][j]);
		break;
	case MAT_Tint:
		for(j=0;j<Rows()*Cols();j++)
			data.i[0][j] = (int)SQR(data.i[0][j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Rows()*Cols();j++)
			data.fl[0][j] = (float)SQR(data.fl[0][j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Rows()*Cols();j++)
			data.db[0][j] = (double)SQR(data.db[0][j]);
		break;
	}
}

void Mat::Sqrt()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Rows()*Cols();j++)
			data.ptr[0][j] = (BYTE)sqrtf(data.ptr[0][j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Rows()*Cols();j++)
			data.s[0][j] = (short)sqrtf(data.s[0][j]);
		break;
	case MAT_Tint:
		for(j=0;j<Rows()*Cols();j++)
			data.i[0][j] = (int)sqrtf((float)data.i[0][j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Rows()*Cols();j++)
			data.fl[0][j] = (float)sqrtf(data.fl[0][j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Rows()*Cols();j++)
			data.db[0][j] = (double)sqrt(data.db[0][j]);
		break;
	}
}

void Mat::Log()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Rows()*Cols();j++)
			data.ptr[0][j] = (BYTE)logf(data.ptr[0][j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Rows()*Cols();j++)
			data.s[0][j] = (short)logf(data.s[0][j]);
		break;
	case MAT_Tint:
		for(j=0;j<Rows()*Cols();j++)
			data.i[0][j] = (int)logf((float)data.i[0][j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Rows()*Cols();j++)
			data.fl[0][j] = (float)logf(data.fl[0][j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Rows()*Cols();j++)
			data.db[0][j] = (double)log(data.db[0][j]);
		break;
	}
}

// misc
void Mat::Kron(Mat& mX,Mat& mY)
{
	assert (false);
}

void Mat::Rand()
{
	assert (false);
}

void Mat::OuterProduct(const Vec &v1, const Vec &v2)
{
	assert (false);
}

// Extract upper triangular part of matrix.
void Mat::TriU(Mat& matrix, const int K) const
{
	assert (false);
}

// Extract lower triangular part of matrix.
void Mat::TriL(Mat& matrix, const int K) const
{
	assert (false);
}

// Flips
void Mat::FlipLR()
{
	int i, k;
	switch(Type())
	{
	case MAT_Tbyte:
		{
			BYTE temp;
			for (i=0; i<Rows(); i++)
			{
				for (k=0; k<Cols(); k++)
				{
					temp=data.ptr[i][k]; data.ptr[i][k]=data.ptr[i][Cols()-k-1]; data.ptr[i][Cols()-k-1]=temp;
				}
			}
		}
		break;
	case MAT_Tshort:
		{
			short temp;
			for (i=0; i<Rows(); i++)
			{
				for (k=0; k<Cols(); k++)
				{
					temp=data.s[i][k]; data.s[i][k]=data.s[i][Cols()-k-1]; data.s[i][Cols()-k-1]=temp;
				}
			}
		}
		break;
	case MAT_Tint:
		{
			int temp;
			for (i=0; i<Rows(); i++)
			{
				for (k=0; k<Cols(); k++)
				{
					temp=data.i[i][k]; data.i[i][k]=data.i[i][Cols()-k-1]; data.i[i][Cols()-k-1]=temp;
				}
			}
		}
		break;
	case MAT_Tfloat:
		{
			float temp;
			for (i=0; i<Rows(); i++)
			{
				for (k=0; k<Cols(); k++)
				{
					temp=data.fl[i][k]; data.fl[i][k]=data.fl[i][Cols()-k-1]; data.fl[i][Cols()-k-1]=temp;
				}
			}
		}
		break;
	case MAT_Tdouble:
		{
			double temp;
			for (i=0; i<Rows(); i++)
			{
				for (k=0; k<Cols(); k++)
				{
					temp=data.db[i][k]; data.db[i][k]=data.db[i][Cols()-k-1]; data.db[i][Cols()-k-1]=temp;
				}
			}
		}
		break;
	}
}

void Mat::FlipUD()
{
	int i;
	Mat mTemp(*this);
	int nlinestep=Step()*Cols()*Channels();
	for (i=0; i<Rows(); i++)
		memcpy(data.ptr[i], mTemp.data.ptr[Rows()-i-1], nlinestep);
}

// Conversions and manipulations
void Mat::ToVector( Vec &v, bool rowWise /*= true */) const 
{
	assert (false);
}

template<typename T>
static void CompareEq(int w, int h, T** A, const double B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] == B));
}
// Elementwise equal to
void Mat::Eq(const double B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareEq<BYTE>(Cols(), Rows(), this->data.ptr, B, C.data.ptr); break;
	case MAT_Tshort:
		CompareEq<short>(Cols(), Rows(), this->data.s, B, C.data.s); break;
	case MAT_Tint:
		CompareEq<int>(Cols(), Rows(), this->data.i, B, C.data.i); break;
	case MAT_Tfloat:
		CompareEq<float>(Cols(), Rows(), this->data.fl, B, C.data.fl); break;
	case MAT_Tdouble:
		CompareEq<double>(Cols(), Rows(), this->data.db, B, C.data.db); break;
	}
}
template<typename T>
static void CompareEq(int w, int h, T** A, T** B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] == B[i][j]));
}
void Mat::Eq(const Mat& B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareEq<BYTE>(Cols(), Rows(), this->data.ptr, B.data.ptr, C.data.ptr); break;
	case MAT_Tshort:
		CompareEq<short>(Cols(), Rows(), this->data.s, B.data.s, C.data.s); break;
	case MAT_Tint:
		CompareEq<int>(Cols(), Rows(), this->data.i, B.data.i, C.data.i); break;
	case MAT_Tfloat:
		CompareEq<float>(Cols(), Rows(), this->data.fl, B.data.fl, C.data.fl); break;
	case MAT_Tdouble:
		CompareEq<double>(Cols(), Rows(), this->data.db, B.data.db, C.data.db); break;
	}
}
template<typename T>
static void CompareNe(int w, int h, T** A, const double B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] != B));
}
void Mat::Ne(const double B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareNe<BYTE>(Cols(), Rows(), this->data.ptr, B, C.data.ptr); break;
	case MAT_Tshort:
		CompareNe<short>(Cols(), Rows(), this->data.s, B, C.data.s); break;
	case MAT_Tint:
		CompareNe<int>(Cols(), Rows(), this->data.i, B, C.data.i); break;
	case MAT_Tfloat:
		CompareNe<float>(Cols(), Rows(), this->data.fl, B, C.data.fl); break;
	case MAT_Tdouble:
		CompareNe<double>(Cols(), Rows(), this->data.db, B, C.data.db); break;
	}
}
template<typename T>
static void CompareNe(int w, int h, T** A, T** B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] != B[i][j]));
}
void Mat::Ne(const Mat& B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareNe<BYTE>(Cols(), Rows(), this->data.ptr, B.data.ptr, C.data.ptr); break;
	case MAT_Tshort:
		CompareNe<short>(Cols(), Rows(), this->data.s, B.data.s, C.data.s); break;
	case MAT_Tint:
		CompareNe<int>(Cols(), Rows(), this->data.i, B.data.i, C.data.i); break;
	case MAT_Tfloat:
		CompareNe<float>(Cols(), Rows(), this->data.fl, B.data.fl, C.data.fl); break;
	case MAT_Tdouble:
		CompareNe<double>(Cols(), Rows(), this->data.db, B.data.db, C.data.db); break;
	}
}
// Elementwise less than
template<typename T>
static void CompareLt(int w, int h, T** A, const double B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] < B));
}
void Mat::Lt(const double B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareLt<BYTE>(Cols(), Rows(), this->data.ptr, B, C.data.ptr); break;
	case MAT_Tshort:
		CompareLt<short>(Cols(), Rows(), this->data.s, B, C.data.s); break;
	case MAT_Tint:
		CompareLt<int>(Cols(), Rows(), this->data.i, B, C.data.i); break;
	case MAT_Tfloat:
		CompareLt<float>(Cols(), Rows(), this->data.fl, B, C.data.fl); break;
	case MAT_Tdouble:
		CompareLt<double>(Cols(), Rows(), this->data.db, B, C.data.db); break;
	}
}
template<typename T>
static void CompareLt(int w, int h, T** A, T** B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] < B[i][j]));
}
void Mat::Lt(const Mat& B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareLt<BYTE>(Cols(), Rows(), this->data.ptr, B.data.ptr, C.data.ptr); break;
	case MAT_Tshort:
		CompareLt<short>(Cols(), Rows(), this->data.s, B.data.s, C.data.s); break;
	case MAT_Tint:
		CompareLt<int>(Cols(), Rows(), this->data.i, B.data.i, C.data.i); break;
	case MAT_Tfloat:
		CompareLt<float>(Cols(), Rows(), this->data.fl, B.data.fl, C.data.fl); break;
	case MAT_Tdouble:
		CompareLt<double>(Cols(), Rows(), this->data.db, B.data.db, C.data.db); break;
	}
}
// Elementwise less than or equal
template<typename T>
static void CompareLe(int w, int h, T** A, const double B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] <= B));
}
void Mat::Le(const double B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareLe<BYTE>(Cols(), Rows(), this->data.ptr, B, C.data.ptr); break;
	case MAT_Tshort:
		CompareLe<short>(Cols(), Rows(), this->data.s, B, C.data.s); break;
	case MAT_Tint:
		CompareLe<int>(Cols(), Rows(), this->data.i, B, C.data.i); break;
	case MAT_Tfloat:
		CompareLe<float>(Cols(), Rows(), this->data.fl, B, C.data.fl); break;
	case MAT_Tdouble:
		CompareLe<double>(Cols(), Rows(), this->data.db, B, C.data.db); break;
	}
}
template<typename T>
static void CompareLe(int w, int h, T** A, T** B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] <= B[i][j]));
}
void Mat::Le(const Mat& B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareLe<BYTE>(Cols(), Rows(), this->data.ptr, B.data.ptr, C.data.ptr); break;
	case MAT_Tshort:
		CompareLe<short>(Cols(), Rows(), this->data.s, B.data.s, C.data.s); break;
	case MAT_Tint:
		CompareLe<int>(Cols(), Rows(), this->data.i, B.data.i, C.data.i); break;
	case MAT_Tfloat:
		CompareLe<float>(Cols(), Rows(), this->data.fl, B.data.fl, C.data.fl); break;
	case MAT_Tdouble:
		CompareLe<double>(Cols(), Rows(), this->data.db, B.data.db, C.data.db); break;
	}
}
// Elementwise greater than
template<typename T>
static void CompareGt(int w, int h, T** A, const double B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] > B));
}
void Mat::Gt(const double B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareGt<BYTE>(Cols(), Rows(), this->data.ptr, B, C.data.ptr); break;
	case MAT_Tshort:
		CompareGt<short>(Cols(), Rows(), this->data.s, B, C.data.s); break;
	case MAT_Tint:
		CompareGt<int>(Cols(), Rows(), this->data.i, B, C.data.i); break;
	case MAT_Tfloat:
		CompareGt<float>(Cols(), Rows(), this->data.fl, B, C.data.fl); break;
	case MAT_Tdouble:
		CompareGt<double>(Cols(), Rows(), this->data.db, B, C.data.db); break;
	}
}
template<typename T>
static void CompareGt(int w, int h, T** A, T** B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] > B[i][j]));
}
void Mat::Gt(const Mat& B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareGt<BYTE>(Cols(), Rows(), this->data.ptr, B.data.ptr, C.data.ptr); break;
	case MAT_Tshort:
		CompareGt<short>(Cols(), Rows(), this->data.s, B.data.s, C.data.s); break;
	case MAT_Tint:
		CompareGt<int>(Cols(), Rows(), this->data.i, B.data.i, C.data.i); break;
	case MAT_Tfloat:
		CompareGt<float>(Cols(), Rows(), this->data.fl, B.data.fl, C.data.fl); break;
	case MAT_Tdouble:
		CompareGt<double>(Cols(), Rows(), this->data.db, B.data.db, C.data.db); break;
	}
}
// Elementwise greater than or equal
template<typename T>
static void CompareGe(int w, int h, T** A, const double B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] >= B));
}
void Mat::Ge(const double B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareGe<BYTE>(Cols(), Rows(), this->data.ptr, B, C.data.ptr); break;
	case MAT_Tshort:
		CompareGe<short>(Cols(), Rows(), this->data.s, B, C.data.s); break;
	case MAT_Tint:
		CompareGe<int>(Cols(), Rows(), this->data.i, B, C.data.i); break;
	case MAT_Tfloat:
		CompareGe<float>(Cols(), Rows(), this->data.fl, B, C.data.fl); break;
	case MAT_Tdouble:
		CompareGe<double>(Cols(), Rows(), this->data.db, B, C.data.db); break;
	}
}
template<typename T>
static void CompareGe(int w, int h, T** A, T** B, T** C)
{
	for (int i=0; i<h; i++) for (int j=0; j<w; j++)
		C[i][j]= (T)(BYTE)(-(A[i][j] >= B[i][j]));
}
void Mat::Ge(const Mat& B, Mat& C) const
{
	if (!IsEqualMat(C))
	{
		C.Release();
		C.Create (Rows(), Cols(), type);
	}
	switch (Type())
	{
	case MAT_Tbyte:
		CompareGe<BYTE>(Cols(), Rows(), this->data.ptr, B.data.ptr, C.data.ptr); break;
	case MAT_Tshort:
		CompareGe<short>(Cols(), Rows(), this->data.s, B.data.s, C.data.s); break;
	case MAT_Tint:
		CompareGe<int>(Cols(), Rows(), this->data.i, B.data.i, C.data.i); break;
	case MAT_Tfloat:
		CompareGe<float>(Cols(), Rows(), this->data.fl, B.data.fl, C.data.fl); break;
	case MAT_Tdouble:
		CompareGe<double>(Cols(), Rows(), this->data.db, B.data.db, C.data.db); break;
	}
}

// Determinant, Trace
double Mat::Trace() const
{
	assert (false);
	return 0;
}
void Mat::Swap(Mat& other)
{
	int ntemp;
	BYTE** pbtemp;
	TYPE ntype;
	SWAP(step, other.step, ntemp);
	SWAP(depend, other.depend, ntemp);
	SWAP(rows, other.rows, ntemp);
	SWAP(cols, other.cols, ntemp);
	SWAP(type, other.type, ntype);
	SWAP(data.ptr, other.data.ptr, pbtemp);
}


#define MATCONVERT_NAME(from,to) matconvert_##from##_##to
#define MATCONVERT(from,to) \
static void MATCONVERT_NAME(from,to)(const Mat& mFrom, Mat& mTo)\
{\
	if (mTo.SizeObject()!=mFrom.SizeObject() || mTo.Type()!=MAT_T##to)\
	{\
		mTo.Release();\
		mTo.Create(mFrom.Rows(), mFrom.Cols(), MAT_T##to);\
	}\
	int i;\
	to* todata=((to*)(mTo.data.ptr[0]));\
	from* fromdata=((from*)(mFrom.data.ptr[0]));\
	for (i=0; i<mFrom.Rows()*mFrom.Cols(); i++)\
	{\
		todata[i] = (to) ((from*)fromdata)[i];\
	}\
}

MATCONVERT(BYTE, short)
MATCONVERT(BYTE, int)
MATCONVERT(BYTE, float)
MATCONVERT(BYTE, double)
MATCONVERT(short, BYTE)
MATCONVERT(short, int)
MATCONVERT(short, float)
MATCONVERT(short, double)
MATCONVERT(int, BYTE)
MATCONVERT(int, short)
MATCONVERT(int, float)
MATCONVERT(int, double)
MATCONVERT(float, BYTE)
MATCONVERT(float, short)
MATCONVERT(float, int)
MATCONVERT(float, double)
MATCONVERT(double, BYTE)
MATCONVERT(double, short)
MATCONVERT(double, int)
MATCONVERT(double, float)

#define MATCONVERTTOBYTE_NAME(from) matconvertToByte_##from
#define MATCONVERTTOBYTE(from) \
static void MATCONVERTTOBYTE_NAME(from) (const Mat& mFrom, Mat& mTo)\
{\
	double rMin=mFrom.Min();\
	double rMax=mFrom.Max();\
	int i;\
	if (mTo.SizeObject()!=mFrom.SizeObject() || mTo.Type()!=MAT_Tbyte)\
	{\
		mTo.Release();\
		mTo.Create(mFrom.Rows(), mFrom.Cols(), MAT_Tbyte);\
	}\
	BYTE* todata=mTo.data.ptr[0];\
	from* fromdata=((from*)(mFrom.data.ptr[0]));\
	for (i=0; i<mFrom.Rows()*mFrom.Cols(); i++)\
	todata[i] = (BYTE)( ( fromdata[i] - rMin) / (rMax-rMin)*255);\
}

MATCONVERTTOBYTE(short)
MATCONVERTTOBYTE(int)
MATCONVERTTOBYTE(float)
MATCONVERTTOBYTE(double)

typedef void (*matconvertfunc)(const Mat&, Mat&);

void Mat::Convert(TYPE type, CASTTYPE casttype/*=CT_Default*/)
{
	if (Type() == type)
		return;
  	matconvertfunc expandFuncs[]={NULL, NULL, MATCONVERTTOBYTE_NAME(short), MATCONVERTTOBYTE_NAME(int), MATCONVERTTOBYTE_NAME(float), MATCONVERTTOBYTE_NAME(double)};
	matconvertfunc castFuncs[6][6]=
	{
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, NULL, MATCONVERT_NAME(BYTE, short), MATCONVERT_NAME(BYTE, int), MATCONVERT_NAME(BYTE, float), MATCONVERT_NAME(BYTE, double)},
		{NULL, MATCONVERT_NAME(short, BYTE), NULL, MATCONVERT_NAME(short, int), MATCONVERT_NAME(short, float), MATCONVERT_NAME(short, double)},
		{NULL, MATCONVERT_NAME(int, BYTE), MATCONVERT_NAME(int, short), NULL, MATCONVERT_NAME(int, float), MATCONVERT_NAME(int, double)},
		{NULL, MATCONVERT_NAME(float, BYTE), MATCONVERT_NAME(float, short), MATCONVERT_NAME(float, int), NULL, MATCONVERT_NAME(float, double)},
		{NULL, MATCONVERT_NAME(double, BYTE), MATCONVERT_NAME(double, short), MATCONVERT_NAME(double, int), MATCONVERT_NAME(double, float), NULL},
	};

	Mat mClone(*this);
	this->Release();
	if (casttype == CT_Default)
	{
		if (type == MAT_Tbyte)
		{
 			expandFuncs[mClone.Type()](mClone, *this);
		}
		else
		{
			castFuncs[mClone.Type()][type](mClone, *this);
		}
	}
	else if (casttype == CT_Cast)
	{
		castFuncs[mClone.Type()][type](mClone, *this);
	}
}

typedef void (*BinaryFunc)(const uchar** src, uchar** dst, Size size, double scale, double shift);

template<typename T, typename DT, typename WT> static void
cvtScale_( const uchar** _src, uchar** _dst, Size size, WT scale, WT shift )
{
	const T** src=(const T**)_src;
	DT** dst=(DT**)_dst;
	for(int y=0; y<size.height; y++)
	{
		const T* s=src[y];
		DT* d=dst[y];
		int x = 0;
// #if CV_ENABLE_UNROLLED
		for( ; x <= size.width - 4; x += 4 )
		{
			DT t0, t1;
			t0 = (DT)(s[x]*scale + shift);
			t1 = (DT)(s[x+1]*scale + shift);
			d[x] = t0; d[x+1] = t1;
			t0 = (DT)(s[x+2]*scale + shift);
			t1 = (DT)(s[x+3]*scale + shift);
			d[x+2] = t0; d[x+3] = t1;
		}
// #endif

		for( ; x < size.width; x++ )
			d[x] = (DT)(s[x]*scale + shift);
	}
}
void Mat::Convert(TYPE _type, double alpha, double beta)
{
	bool noScale = fabs(alpha-1) < DBL_EPSILON && fabs(beta) < DBL_EPSILON;

	if( _type < 0 )
		_type = Type1();

	if( noScale )
		return;

	Mat src = *this;

#define CoefT double
	BinaryFunc cvtFuncs[6][6]=
	{
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, cvtScale_<BYTE,BYTE,CoefT>, cvtScale_<BYTE,short,CoefT>, cvtScale_<BYTE,int,CoefT>, cvtScale_<BYTE,float,CoefT>, cvtScale_<BYTE,double,CoefT>},
		{NULL, cvtScale_<short,BYTE,CoefT>, cvtScale_<short,short,CoefT>, cvtScale_<short,int,double>, cvtScale_<short,float,CoefT>, cvtScale_<short,double,CoefT>},
		{NULL, cvtScale_<int,BYTE,CoefT>, cvtScale_<int,short,CoefT>, cvtScale_<int,int,CoefT>, cvtScale_<int,float,CoefT>, cvtScale_<int,double,CoefT>},
		{NULL, cvtScale_<float,BYTE,CoefT>, cvtScale_<float,short,CoefT>, cvtScale_<float,int,CoefT>, cvtScale_<float,float,CoefT>, cvtScale_<float,double,CoefT>},
		{NULL, cvtScale_<double,BYTE,CoefT>, cvtScale_<double,short,CoefT>, cvtScale_<double,int,CoefT>, cvtScale_<double,float,CoefT>, cvtScale_<double,double,CoefT>},
	};

	BinaryFunc func = cvtFuncs[(int)src.Type()][(int)this->Type()];
	this->Release();
	this->Create (src.SizeObject(), _type);
	Size size=src.SizeObject();
	size.width *= Channels();
	func((const uchar**)src.data.ptr, (uchar**)this->data.ptr, this->SizeObject(), alpha, beta);
}
void Mat::ConvertTo(Mat& dst, TYPE _type, double alpha, double beta)
{
	bool noScale = fabs(alpha-1) < DBL_EPSILON && fabs(beta) < DBL_EPSILON;

	if( _type < 0 )
		_type = Type1();

	if( noScale )
		return;

	dst.Create (rows, cols, _type);

#define CoefT double
	BinaryFunc cvtFuncs[6][6]=
	{
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, cvtScale_<BYTE,BYTE,CoefT>, cvtScale_<BYTE,short,CoefT>, cvtScale_<BYTE,int,CoefT>, cvtScale_<BYTE,float,CoefT>, cvtScale_<BYTE,double,CoefT>},
		{NULL, cvtScale_<short,BYTE,CoefT>, cvtScale_<short,short,CoefT>, cvtScale_<short,int,double>, cvtScale_<short,float,CoefT>, cvtScale_<short,double,CoefT>},
		{NULL, cvtScale_<int,BYTE,CoefT>, cvtScale_<int,short,CoefT>, cvtScale_<int,int,CoefT>, cvtScale_<int,float,CoefT>, cvtScale_<int,double,CoefT>},
		{NULL, cvtScale_<float,BYTE,CoefT>, cvtScale_<float,short,CoefT>, cvtScale_<float,int,CoefT>, cvtScale_<float,float,CoefT>, cvtScale_<float,double,CoefT>},
		{NULL, cvtScale_<double,BYTE,CoefT>, cvtScale_<double,short,CoefT>, cvtScale_<double,int,CoefT>, cvtScale_<double,float,CoefT>, cvtScale_<double,double,CoefT>},
	};

	BinaryFunc func = cvtFuncs[(int)Type()][(int)Type()];
	this->Release();
	this->Create (SizeObject(), _type);
	Size size=SizeObject();
	size.width *= Channels();
	func((const uchar**)data.ptr, (uchar**)dst.data.ptr, this->SizeObject(), alpha, beta);
}
typedef double (*NormFunc)(uchar** const src1, uchar** const src2, uchar** const mask, Size size);
template<typename T> static double 
iNormDiffL1_( uchar** const _src1, uchar** const _src2, uchar** const mask, Size size )
{
	double result=0;
	T** const src1=(T** const)_src1;
	T** const src2=(T** const)_src2;
	if (src2)
	{
		if (!mask)
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				const T* s2=src2[y];
				int x = 0;
				// #if CVLIB_ENABLE_UNROLLED
				for( ; x <= size.width - 4; x += 4 )
				{
					result += (double)fabs((double)(s1[x]-s2[x]));
					result += (double)fabs((double)(s1[x+1]-s2[x+1]));
					result += (double)fabs((double)(s1[x+2]-s2[x+2]));
					result += (double)fabs((double)(s1[x+3]-s2[x+3]));
				}
				// #endif

				for( ; x < size.width; x++ )
					result += (double)fabs((double)(s1[x]-s2[x]));
			}
		}
		else
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				const T* s2=src2[y];
				for(int x=0 ; x < size.width; x++ )
					if (mask[y][x])
						result += (double)fabs((double)(s1[x]-s2[x]));
			}
		}
	}
	else
	{
		if (!mask)
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				int x = 0;
				// #if CVLIB_ENABLE_UNROLLED
				for( ; x <= size.width - 4; x += 4 )
				{
					result += (double)fabs((double)(s1[x]));
					result += (double)fabs((double)(s1[x+1]));
					result += (double)fabs((double)(s1[x+2]));
					result += (double)fabs((double)(s1[x+3]));
				}
				// #endif

				for( ; x < size.width; x++ )
					result += (double)fabs((double)(s1[x]));
			}
		}
		else
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				for(int x=0 ; x < size.width; x++ )
					if (mask[y][x])
						result += (double)fabs((double)(s1[x]));
			}
		}
	}
	return result;
}
template<typename T> static double 
iNormDiffL2_( uchar** const _src1, uchar** const _src2, uchar** const mask, Size size )
{
	double result=0;
	T** const src1=(T** const)_src1;
	T** const src2=(T** const)_src2;
	if (src2)
	{
		if (!mask)
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				const T* s2=src2[y];
				int x = 0;
				// #if CVLIB_ENABLE_UNROLLED
				for( ; x <= size.width - 4; x += 4 )
				{
					result += (double)SQR(s1[x]-s2[x]);
					result += (double)SQR(s1[x+1]-s2[x+1]);
					result += (double)SQR(s1[x+2]-s2[x+2]);
					result += (double)SQR(s1[x+3]-s2[x+3]);
				}
				// #endif

				for( ; x < size.width; x++ )
					result += (double)SQR(s1[x]-s2[x]);
			}
		}
		else
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				const T* s2=src2[y];
				for(int x=0 ; x < size.width; x++ )
					if (mask[y][x])
						result += (double)SQR(s1[x]-s2[x]);
			}
		}
	}
	else
	{
		if (!mask)
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				int x = 0;
				// #if CVLIB_ENABLE_UNROLLED
				for( ; x <= size.width - 4; x += 4 )
				{
					result += (double)SQR(s1[x]);
					result += (double)SQR(s1[x+1]);
					result += (double)SQR(s1[x+2]);
					result += (double)SQR(s1[x+3]);
				}
				// #endif

				for( ; x < size.width; x++ )
					result += (double)SQR(s1[x]);
			}
		}
		else
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				for(int x=0 ; x < size.width; x++ )
					if (mask[y][x])
						result += (double)SQR(s1[x]);
			}
		}
	}
	return result;
}
template<typename T> static double 
iNormDiffInf_( uchar** const _src1, uchar** const _src2, uchar** const mask, Size size )
{
	double result=0;
	T** const src1=(T** const)_src1;
	T** const src2=(T** const)_src2;
	if (src2)
	{
		if (!mask)
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				const T* s2=src2[y];
				int x = 0;
				// #if CVLIB_ENABLE_UNROLLED
				for( ; x <= size.width - 4; x += 4 )
				{
					result = MAX(result, fabs((double)(s1[x]-s2[x])));
					result = MAX(result, fabs((double)(s1[x+1]-s2[x+1])));
					result = MAX(result, fabs((double)(s1[x+2]-s2[x+2])));
					result = MAX(result, fabs((double)(s1[x+3]-s2[x+3])));
				}
				// #endif

				for( ; x < size.width; x++ )
					if (mask[y][x])
						result = MAX(result, fabs((double)(s1[x]-s2[x])));
			}
		}
		else
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				const T* s2=src2[y];
				for( int x=0; x < size.width; x++ )
					if (mask[y][x])
						result = MAX(result, fabs((double)(s1[x]-s2[x])));
			}
		}
	}
	else
	{
		if (!mask)
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				int x = 0;
				// #if CVLIB_ENABLE_UNROLLED
				for( ; x <= size.width - 4; x += 4 )
				{
					result = MAX(result, fabs((double)(s1[x])));
					result = MAX(result, fabs((double)(s1[x+1])));
					result = MAX(result, fabs((double)(s1[x+2])));
					result = MAX(result, fabs((double)(s1[x+3])));
				}
				// #endif

				for( ; x < size.width; x++ )
					result = MAX(result, fabs((double)(s1[x])));
			}
		}
		else
		{
			for(int y=0; y<size.height; y++)
			{
				const T* s1=src1[y];
				for(int x=0 ; x < size.width; x++ )
					if (mask[y][x])
						result = MAX(result, fabs((double)(s1[x])));
			}
		}
	}
	return result;
}
double Mat::Norm(int norm_type, const Mat& _mask) const
{
	uchar** mask=_mask.IsValid()?_mask.data.ptr:NULL;
	Size size=SizeObject();
	NormFunc func;
	NormFunc normFuncs[5][6]=
	{
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, iNormDiffInf_<uchar>, iNormDiffInf_<short>, iNormDiffInf_<int>, iNormDiffInf_<float>, iNormDiffInf_<double>},
		{NULL, iNormDiffL1_<uchar>, iNormDiffL1_<short>, iNormDiffL1_<int>, iNormDiffL1_<float>, iNormDiffL1_<double>},
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, iNormDiffL2_<uchar>, iNormDiffL2_<short>, iNormDiffL2_<int>, iNormDiffL2_<float>, iNormDiffL2_<double>},
	};
	func=normFuncs[norm_type][(int)Type()];
	return func(data.ptr, 0, mask, size);
}

double Mat::NormDiff(int norm_type, const Mat& other, const Mat& _mask) const
{
	assert (IsEqualMat(other));
	uchar** mask=_mask.IsValid()?_mask.data.ptr:NULL;
	Size size=SizeObject();
	NormFunc func;
	NormFunc normFuncs[5][6]=
	{
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, iNormDiffInf_<uchar>, iNormDiffInf_<short>, iNormDiffInf_<int>, iNormDiffInf_<float>, iNormDiffInf_<double>},
		{NULL, iNormDiffL1_<uchar>, iNormDiffL1_<short>, iNormDiffL1_<int>, iNormDiffL1_<float>, iNormDiffL1_<double>},
		{NULL, NULL, NULL, NULL, NULL, NULL},
		{NULL, iNormDiffL2_<uchar>, iNormDiffL2_<short>, iNormDiffL2_<int>, iNormDiffL2_<float>, iNormDiffL2_<double>},
	};
	func=normFuncs[norm_type][(int)Type()];
	return func(data.ptr, other.data.ptr, mask, size);
}

typedef void (*CopyToFunc)(uchar** const src, uchar** dst, uchar** const mask, Size size);
template<typename T> static void
iCopyTo( uchar** const _src, uchar** _dst, uchar** const mask, Size size )
{
	T** const src=(T** const)_src;
	T** dst=(T**)_dst;
	if (!mask)
	{
		for (int y=0; y<size.height; y++) for (int x=0; x<size.width; x++)
			dst[y][x]=src[y][x];
	}
	else
	{
		for (int y=0; y<size.height; y++) for (int x=0; x<size.width; x++)
		{
			if (mask[y][x])
				dst[y][x]=src[y][x];
		}
	}
}
void Mat::CopyTo(Mat& dst, const Mat& _mask) const
{
	uchar** const mask=_mask.data.ptr;
	Size size=SizeObject();
	CopyToFunc func;
	CopyToFunc copytoFuncs[6]={NULL,iCopyTo<uchar>,iCopyTo<short>,iCopyTo<int>,iCopyTo<float>,iCopyTo<double>};
	func=copytoFuncs[(int)Type()];
	if (!IsEqualMat(dst))
	{
		dst.Release();
		dst.Create (*this, FALSE);
	}
	func(data.ptr, dst.data.ptr, mask, size);
}

typedef void (*minmaxLocFunc)(uchar** const src, double* minVal, double* maxVal, Point2i* minLoc, Point2i* maxLoc, uchar** const mask, Size size);
template<typename T> static void
iminmaxLoc(uchar** const _src, double* minVal, double* maxVal, Point2i* minLoc, Point2i* maxLoc, uchar** const mask, Size size)
{
	T** const src=(T** const)_src;
	double rMin = DBL_MAX, rMax = -DBL_MAX;
	int iminX, iminY, imaxX, imaxY;
	if (!mask)
	{
		for (int y=0; y<size.height; y++) for (int x=0; x<size.width; x++)
		{
			if (rMin > src[y][x])
			{
				rMin = src[y][x];
				iminX=x;
				iminY=y;
			}
			if (rMax < src[y][x])
			{
				rMax = src[y][x];
				imaxX=x;
				imaxY=y;
			}
		}
	}
	else
	{
		for (int y=0; y<size.height; y++) for (int x=0; x<size.width; x++)
		{
			if (!mask[y][x])
				continue;
			if (rMin > src[y][x])
			{
				rMin = src[y][x];
				iminX=x;
				iminY=y;
			}
			if (rMax < src[y][x])
			{
				rMax = src[y][x];
				imaxX=x;
				imaxY=y;
			}
		}
	}
	if (minVal) *minVal=rMin;
	if (maxVal) *maxVal=rMax;
	if (minLoc) *minLoc=Point2i(iminX, iminY);
	if (maxLoc) *maxLoc=Point2i(imaxX, imaxY);
}
void Mat::MinMaxLoc(double* minVal, double* maxVal, Point2i* minLoc, Point2i* maxLoc, const Mat& mask) const
{
	minmaxLocFunc func;
	minmaxLocFunc minmaxFuncs[6]={NULL,iminmaxLoc<uchar>,iminmaxLoc<short>,iminmaxLoc<int>,iminmaxLoc<float>,iminmaxLoc<double>};
	func=minmaxFuncs[(int)Type()];
	func(data.ptr, minVal, maxVal, minLoc, maxLoc, mask.data.ptr, SizeObject());
}
int Mat::CreateFromArray (BYTE* pbData, int nRows, int nCols, int nBitsperpixel, int nBytesperline, bool bFlipImage)
{
	BYTE* pBMPData = pbData;
	int nH = nRows;
	int nW = nCols;

	if ((nCols == 0) || (nRows == 0))
		return 0;
	if (nBitsperpixel==enARGB || nBitsperpixel==enBGRA)
		return 0;

	int wBpp=nBitsperpixel;
	// Make sure bits per pixel is valid
	if		(wBpp <= 1)	wBpp = 1;
	else if (wBpp <= 4)	wBpp = 4;
	else if (wBpp <= 8)	wBpp = 8;
	else				wBpp = 24;

	if (IsValid())
	{
		if (Cols() != nCols || Rows() != nRows)
		{
			Release();
			Create (nRows, nCols, MAT_Tbyte3);
		}
	}
	else
	{
		Create (nRows, nCols, MAT_Tbyte3);
	}

	BYTE* ptr=data.ptr[0];
	for (int i = 0; i < nH; i ++)
	{
		int nDst = (bFlipImage ? (nH - i - 1) : i) * nBytesperline;
		for (int j = 0; j < nW; j ++)
		{
			int nTemp;
			WORD wRGB;
			switch(nBitsperpixel)
			{
			case enARGB:
				nTemp = nDst + j * 4;
				*ptr++ = pBMPData[nTemp];
				*ptr++ = pBMPData[nTemp + 1];
				*ptr++ = pBMPData[nTemp + 2];
				*ptr++ = pBMPData[nTemp + 3];
				break;
			case enBGRA:
				nTemp = nDst + j * 4;
				*ptr++ = pBMPData[nTemp];
				*ptr++ = pBMPData[nTemp + 1];
				*ptr++ = pBMPData[nTemp + 2];
				*ptr++ = pBMPData[nTemp + 3];
				break;
			case enBGR888:
				nTemp = nDst + j * 3;
				*ptr++ = pBMPData[nTemp];
				*ptr++ = pBMPData[nTemp + 1];
				*ptr++ = pBMPData[nTemp + 2];
				break;
			case enBGR555:
				nTemp = nDst + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				*ptr++ = (wRGB & 0x1F) << 3;
				*ptr++ = ((wRGB >> 5) & 0x1F) << 3;
				*ptr++ = ((wRGB >> 10) & 0x1F) << 3;
				break;
			case enBGR565:
				nTemp = nDst + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				*ptr++ = (wRGB & 0x1F) << 3;
				*ptr++ = ((wRGB >> 5) & 0x3F) << 2;
				*ptr++ = ((wRGB >> 11) & 0x1F) << 3;
				break;
			case 12:
				nTemp = nDst + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				*ptr++ = (wRGB & 0x1F) << 3;
				*ptr++ = ((wRGB >> 5) & 0x1F) << 3;
				*ptr++ = ((wRGB >> 10) & 0x1F) << 3;
				break;
			default:
				break;
			}
		}
	}
	return 1;
}
void Mat::ReverseChannel()
{
	if (Channels()!=3)
		return;
	uchar temp;
	uchar* pbtemp;
	for (int ih=0; ih<rows; ih++)
	{
		pbtemp=data.ptr[ih];
		for (int iw=0; iw<cols; iw++, pbtemp+=3)
		{
			temp=*pbtemp;
			*pbtemp=*(pbtemp+2);
			*(pbtemp+2)=temp;
		}
	}
}

}