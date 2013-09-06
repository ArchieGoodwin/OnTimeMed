/*!
 * \file    Vec.h
 * \ingroup base
 * \brief   ¹ÖÀË¶£¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _VEC_H_
#define _VEC_H_

#include "Object.h"
#include "SString.h"
#include "XFile.h"
#include "cvlibstructs.h"
#include "cvlibmalloc.h"

namespace CVLib
{

class Mat;
template<typename _Tp> class Mat_;

/**
 * @brief  ¹ÖÀË¶£¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC Vec : public Object
{
	friend class Mat;
public:
	union
	{
		unsigned char* ptr;
		short* s;
		int* i;
		float* fl;
		double* db;
	} data;

public:

	// Construction and Destruction
	Vec () :len(0),m_nMaxLen(0),step(0),depend(0){data.ptr=NULL;}
	Vec (const Vec& vec);
	Vec(int len, TYPE type);
	Vec(void* ptr, int len, TYPE type);
	~Vec ();

	//! Initialize
	int		Create(const Vec& vec, BOOL fCopy = FALSE);
	int		Create(int len, TYPE type);
	int		Create(void* ptr, int len, TYPE type);
	void	Release ();

	//! import and export, conversion operations
	virtual int FromFile (const char* szFilename);
	virtual int FromFile (FILE* pFile);
	virtual int FromFile (XFile* pFile);
	virtual int ToFile (const char* szFilename) const;
	virtual int ToFile (FILE* pFile) const;
	virtual int ToFile (XFile* pFile) const;
	void ToMatrix( const int nRows, const int nCols, Mat &m, bool rowWise =true );
	void ToMatlab(const SString& sFilename,const SString& sName,const SString& sComment="",bool fAppend=true) const;
	void FromMatlab(const SString& sFilename,const SString& sName);
	void ToC (const SString& sFilename, const SString& sVarName, const int nElement=16, bool fAppend=false) const;
	SString ToString(const bool fNewline = true) const;

	// Attribute Operations
	inline int	Length () const {return len;};
	inline int	MaxLength () const {return m_nMaxLen;};
	inline TYPE	Type () const {return type;};
	inline int Step () const {return step;}
	inline char* GetBuffer () const {return (char*)data.ptr;};

	//! valid check operations
	inline int IsValid () const {if (data.ptr)return 1;else return 0;}
	inline bool IsInterior (int nX) const { return (nX < len && nX >= 0); }

	//! Element operations
	double	Value (int i) const;
    Vec* SubVec(int nFirst, int nLast, BOOL fShare = FALSE) const;
	void	Zero ();
	void	Resize (int num);
	Vec& operator=(const Vec& vecSrc);
    Vec& operator=(double value);

    // norms
    double Norm1() const;
	double Norm2() const;
	double NormInf() const;
	double Energy() const;

	// Statistical functions
	double Unit();
	double AutoCorrelation( const int lag = 1 ) const;
	double Max() const;
	double Max(int& iPos) const;
	double Mean() const;
	double Median() const;
	Vec	Trim( const double percentage = .10 ) const;
	double TrimmedMean( const double percentage = .10 ) const;
	double TrimmedVar( const double percentage = .10 ) const;
	double TrimmedStd( const double percentage = .10 ) const;
	double Min() const;
	double Min(int& iPos) const;
	double Skewness() const;
	double Std() const;	
	double Sum() const;
	double Var() const;
	double Var( double *pMean ) const;

	void Rand();
	void Rand( const int st, const int end );
	void Ceil();
	void Floor();
	void Round();
	void Linspace( const double x1, const double x2, const int n );

	// Sorting
	void Sort( bool ascending = true );
	// radomization
	void Shuffle();

	// Fitting
	void AlignTo( const Vec &v, double *pA = NULL, double *pB = NULL );

	void ElementMultiply(const Vec& vector);
	void ElementDivide(const Vec& vector);

	// math functions
	void Abs();
	void Clamp( const double min, const double max );
	void Pow(double dP);
	void Sqrt();
	void Sqr();
	void Log();

	void CrossProduct(const Vec &v1, const Vec &v2);

	// Saving and retrieving from binary file (Much faster then the MatLab Comunication routines).
	void Reverse();

	// Conversions and manipulations
	Vec Range( const int st, const int end) const;
	Vec VecCat( const Vec &v ) const;

	// Elementwise equal to
	void Eq(const double B, Vec& C) const;
	void Eq(const Vec& B, Vec& C) const;

	// Elementwise not equal to
	void Ne(const double B, Vec& C) const;
	void Ne(const Vec& B, Vec& C) const;

	// Elementwise less than
	void Lt(const double B, Vec& C) const;
	void Lt(const Vec& B, Vec& C) const;

	// Elementwise less than or equal
	void Le(const double B, Vec& C) const;
	void Le(const Vec& B, Vec& C) const;

	// Elementwise greater than
	void Gt(const double B, Vec& C) const;
	void Gt(const Vec& B, Vec& C) const;

	// Elementwise greater than or equal
	void Ge(const double B, Vec& C) const;
	void Ge(const Vec& B, Vec& C) const;

	void Swap(Vec& other);

protected:
	TYPE type;
	int step;
	int depend;
	int len;
	int m_nMaxLen;  
};

template <typename _Tp> class CVLIB_DECLSPEC Vec_ : public Vec
{
public:
	typedef Mat_<_Tp> _MyMat;
	typedef Vec_<_Tp> _MyVec;

	Vec_ (){};
	Vec_ (const _MyVec& vec);
	Vec_ (int len);
	Vec_ (_Tp* ptr, int len);

	int		Create(int len);
	int		Create(_Tp* ptr, int len);
	_MyVec& operator=(const _MyVec& vecSrc);
	void Resize(int num);
	operator _Tp*() const {return (_Tp*)data.ptr;}
	_MyVec& operator=(_Tp value);
	bool operator==(const _MyVec& refvector) const;
	bool operator!=(const _MyVec& refvector) const;
	bool operator<(const _MyVec& refvector) const;
	_MyVec operator+(const _MyVec &refvectorB) const;
	_MyVec operator-(void) const;
	_MyVec operator-(const _MyVec &refvectorB) const;
	_MyVec operator*(double dbl) const;
	double operator*(const _MyVec &refvectorB) const;
	_MyVec operator/(double dbl) const;
	_MyVec& operator+=(const _MyVec &refvectorB);
	_MyVec& operator-=(const _MyVec &refvectorB);
	_MyVec& operator+=(double dbl);
	_MyVec& operator-=(double dbl);
	_MyVec& operator*=(double dbl);
	_MyVec& operator/=(double dbl);

	_MyVec& EqSum(const _MyVec &refvectorA, const _MyVec &refvectorB);
	_MyVec& EqDiff(const _MyVec &refvectorA, const _MyVec &refvectorB);
	_MyVec& EqProd(const _MyMat &refmatrixA, const _MyVec &refvectorB);
	_MyVec VecCat( const _MyVec &v ) const;
	bool CrossProduct(const _MyVec& v1, const _MyVec& v2);
	void AlignTo( const _MyVec& v, double *pA = NULL, double *pB = NULL );
};

inline void CVLIB_DECLSPEC ReleaseVec (Vec* pvA)
{
	pvA->Release();
	delete pvA;
}

}

#include "xVec.h"

namespace CVLib
{
typedef Vec_<uchar>		Vecb;
typedef Vec_<short>		Vecs;
typedef Vec_<int>		Veci;
typedef Vec_<float>		Vecf;
typedef Vec_<double>	Vecd;
}

#endif //_VEC_H_