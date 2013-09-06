
/*!
 * \file    Vec.h
 * \ingroup base
 * \brief   ¹ÖÀË¶£¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _xVEC_H_
#define _xVEC_H_

namespace CVLib
{

#define ExternalStorage 0
#define NonExternalStorage 1

template<typename _Tp>
Vec_<_Tp>::Vec_(const Vec_& vec)
{
	if (this==&vec) return;
	Vec::Create (vec, TRUE);
}
template<typename _Tp>
Vec_<_Tp>::Vec_(int len)
{
	Create(len);
}
template<typename _Tp>
Vec_<_Tp>::Vec_(_Tp* ptr, int len)
{
	Create (ptr, len);
}
template<typename _Tp>
int Vec_<_Tp>::Create(int len)
{
	if (len==0) return 0;
	this->len = len;
	this->type = (TYPE)DataDepth<_Tp>::value;
	m_nMaxLen = len;
	step = sizeof(_Tp);
	MALLOC (data.ptr, len * step, uchar);
	depend = ExternalStorage;
	return 1;
}
template<typename _Tp>
int Vec_<_Tp>::Create(_Tp* ptr, int len)
{
	this->len = len;
	m_nMaxLen = len;
	this->type = (TYPE)DataDepth<_Tp>::value;
	step = sizeof(_Tp);
	data.ptr = (uchar*)ptr;
	depend = NonExternalStorage;
	return 1;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator=(const Vec_<_Tp>& vecSrc)
{
	if (this==&vecSrc)
		return *this;
	if (IsValid())
		memcpy (data.ptr, vecSrc.data.ptr, vecSrc.Step()*vecSrc.Length());
	else
		Vec::Create (vecSrc, TRUE);
	return *this;
}
template<typename _Tp>
bool Vec_<_Tp>::operator==(const Vec_<_Tp>& refvector) const
{
	if (Length() == refvector.Length())
	{
		if (Length() == 0) return true;
		if (memcmp(data.ptr, refvector.data.ptr, Length() * sizeof(_Tp)) == 0)
			return true;
	}
	return false;
}
template<typename _Tp>
bool Vec_<_Tp>::operator!=(const Vec_<_Tp>& refvector) const
{
	return !(*this == refvector);
}
template<typename _Tp>
bool Vec_<_Tp>::operator<(const Vec_<_Tp>& refvector) const
{
	if (Length() == refvector.Length())
	{
		if (Length() == 0) return false;
		return (memcmp(data.ptr, refvector.data.ptr, Length() * sizeof(_Tp)) < 0);
	}
	return (Length() < refvector.Length());
}
template<typename _Tp>
Vec_<_Tp> Vec_<_Tp>::operator+(const Vec_<_Tp> &refvector) const
{
	assert(Length() == refvector.Length());
	Vec_<_Tp> vectorRet(*this);
	return (vectorRet += refvector);
}
template<typename _Tp>
Vec_<_Tp> Vec_<_Tp>::operator-(void) const
{
	Vec_<_Tp> vectorRet(Length());
	for (int i=0; i < Length(); i++)
		vectorRet[i] = - ((_Tp*)data.ptr)[i];
	return vectorRet;
}
template<typename _Tp>
Vec_<_Tp> Vec_<_Tp>::operator-(const Vec_<_Tp> &refvector) const
{
	assert(Length() == refvector.Length());
	Vec_<_Tp> vectorRet(*this);
	return (vectorRet -= refvector);
}
template<typename _Tp>
Vec_<_Tp> Vec_<_Tp>::operator*(double dbl) const
{
	Vec_<_Tp> vectorRet(*this);
	return (vectorRet *= dbl);
}
template<typename _Tp>
double Vec_<_Tp>::operator*(const Vec_<_Tp> &refvector) const
{
	double sum = 0.0;
	assert(Length() == refvector.Length());
	for (int i = 0; i < Length(); i++)
		sum += ((_Tp*)data.ptr)[i] * ((_Tp*)refvector.data.ptr)[i];
	return sum;
}
template<typename _Tp>
Vec_<_Tp> Vec_<_Tp>::operator/(double dbl) const
{
	assert(dbl != 0);
	Vec_<_Tp> vectorRet(*this);
	return (vectorRet /= dbl);
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator+=(const Vec_<_Tp> &refvector)
{
	return EqSum(*this, refvector);
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator-=(const Vec_<_Tp> &refvector)
{
	return EqDiff(*this, refvector);
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator+=(double dbl)
{
	for (int i=0; i<Length(); i++)
		((_Tp*)data.ptr)[i] = (_Tp)(((_Tp*)data.ptr)[i]+dbl);
	return *this;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator-=(double dbl)
{
	for (int i=0; i<Length(); i++)
		((_Tp*)data.ptr)[i] = (_Tp)(((_Tp*)data.ptr)[i]-dbl);
	return *this;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator*=(double dbl)
{
	for (int i=0; i<Length(); i++)
		((_Tp*)data.ptr)[i] = (_Tp)(((_Tp*)data.ptr)[i]*dbl);
	return *this;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator/=(double dbl)
{
	assert(dbl != 0);
	for (int i=0; i<Length(); i++)
		((_Tp*)data.ptr)[i] = (_Tp)(((_Tp*)data.ptr)[i]/dbl);
	return *this;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::EqSum(const Vec_<_Tp> &refvectorA, const Vec_<_Tp> &refvectorB)
{
	int n = Length();
	assert(n == refvectorA.Length());
	assert(n == refvectorB.Length());
	for (int i=0; i<n; i++)
		((_Tp*)data.ptr)[i] = ((_Tp*)refvectorA.data.ptr)[i] + ((_Tp*)refvectorB.data.ptr)[i];
	return *this;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::EqDiff(const Vec_<_Tp> &refvectorA, const Vec_<_Tp> &refvectorB)
{
	int n = Length();
	assert(n == refvectorA.Length());
	assert(n == refvectorB.Length());
	for (int i=0; i<n; i++)
		((_Tp*)data.ptr)[i] = ((_Tp*)refvectorA.data.ptr)[i] - ((_Tp*)refvectorB.data.ptr)[i];
	return *this;
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::EqProd(const Mat_<_Tp> &refmatrixA, const Vec_<_Tp> &refvectorB)
{
	int nRows = refmatrixA.Rows();
	int nCols = refmatrixA.Cols();
	assert(nRows == Length());
	assert(refmatrixA.Cols() == refvectorB.Length());
	for (int i = 0; i < nRows; i++)
	{
		double dblSum = 0.0;
		for (int j = 0; j < nCols; j++)
			dblSum += refmatrixA[i][j] * refvectorB[j];
		((_Tp*)data.ptr)[i] = (_Tp)dblSum;
	}
	return (*this);
}
template<typename _Tp>
Vec_<_Tp> Vec_<_Tp>::VecCat( const Vec_<_Tp> &v ) const
{
	int tlen = this->Length();
	Vec_<_Tp> ret( tlen+v.Length() );
	for(int i=0;i<ret.Length();i++)
		ret[i] = i<tlen ? (*this)[i] : v[i-tlen];
	return ret;
}
template<typename _Tp>
bool Vec_<_Tp>::CrossProduct(const Vec_<_Tp>& v1, const Vec_<_Tp>& v2)
{
	int N = v1.Length();
	if (N < 2 || N != v2.Length())
		return false;
	int size = N * (N - 1) / 2 - 1;
	Create(size+1);
	int inc = 0;
	_Tp cross;
	for (int i=N-1; i>0; i--) for (int j=(i-1); j>=0; j--)
	{
		cross = v1[i] * v2[j] - v2[i] * v1[j];
		((_Tp*)data.ptr)[inc] = (((i-j)%2)==0) ? cross : -cross;
		inc++;
	}
	return true;
}
template<typename _Tp>
void Vec_<_Tp>::AlignTo( const Vec_<_Tp>& v, double *pA, double *pB)
{
	assert( this->Length()==v.Length() );
	int n = this->Length();
	assert(n==v.Length() );
	double x, y, a, b, Sx = .0, Sy = .0, Sxx = .0, Sxy = .0;
	int i;
	for(i=0;i<n;i++)
	{
		x    = ((_Tp*)data.ptr)[i];
		y    = v[i];
		Sx  += x;
		Sy  += y;
		Sxx += x*x;
		Sxy += x*y;
	}
	double d = n*Sxx-Sx*Sx;
	a = (n*Sxy-Sx*Sy)/d;
	b = (Sy*Sxx-Sx*Sxy)/d;
	for(i=0;i<n;i++)
		((_Tp*)data.ptr)[i] = (_Tp)(a * (*this)[i] + b);
	if (pA) { *pA = a; }
	if (pB) { *pB = b; }
}
template<typename _Tp>
Vec_<_Tp>& Vec_<_Tp>::operator=(_Tp value)
{
	if ((value == 0) && (Length() != 0))
	{
		assert(Length() > 0);
		memset(data.ptr, 0, Length()*sizeof(_Tp));
	}
	else
	{
		for (int i = 0; i < Length(); i++)
			((_Tp*)data.ptr)[i] = value;
	}
	return *this;
}
template<typename _Tp>
void Vec_<_Tp>::Resize(int num)
{
	if (m_nMaxLen >= num) len = num;
	else if (depend == ExternalStorage)
	{
		Vec_<_Tp> v(num);
		memcpy (v.data.ptr, data.ptr, len * step);
		Release();
		Vec::Create (v, TRUE);
	}
}

}

#endif //_xVEC_H_