/*!
 * \file    Mat.h
 * \ingroup base
 * \brief   ÃÔµÛ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 * \last update date	2010/12/09
 */

#ifndef _MAT_H_
#define _MAT_H_

#include "Vec.h"
#include "LUDecomposition.h"

namespace CVLib 
{

class CoImage;

enum ENUM_CVLIB_MATIMG_CONVERT_FORMATS
{
	CVLIB_MATIMG_DEFAULT,
	CVLIB_MATIMG_EXPANSION
};

enum { NORM_INF=1, NORM_L1=2, NORM_L2=4, NORM_L2SQR=5, NORM_HAMMING=6, NORM_HAMMING2=7, NORM_TYPE_MASK=7, NORM_RELATIVE=8, NORM_MINMAX=32 };

/**
 * @brief matrix class
 */
class CVLIB_DECLSPEC Mat : public Object
{
public:
	union
	{
		unsigned char**	ptr;
		short** s;
		int** i;
		float** fl;
		double** db;
	} data;

public:
	// Construction and Destruction
	Mat() {data.ptr = NULL; rows = 0; cols = 0;};
	Mat(int rows, int cols, TYPE type);
	Mat(int rows, int cols, TYPE type, const Scalar& _s);
	Mat(const Size& size, TYPE type);
	Mat(void** ptr, int rows, int cols, TYPE type);
	Mat(void* ptr, int rows, int cols, TYPE type, BOOL fAlloc = TRUE);
	Mat(const Mat& mat);
	Mat(const Mat& mat, const Rect& roi);
	virtual ~Mat ();

	// creation Operations
	int		Create(int rows, int cols, TYPE type);
	int		Create(const Size& size, TYPE type);
	int		Create(void** ptr, int rows, int cols, TYPE type);
	int		Create(void* ptr, int rows, int cols, TYPE type, BOOL fAlloc = TRUE);
	int		Create(Vec& vec);
	int		Create(const Mat& mA, BOOL fCopy = FALSE);
	int		Create(BITMAPINFOHEADER1* pBI);
	int		CreateFromArray (BYTE* pbData, int nRows, int nCols, int nBitsperpixel, int nBytesperline, bool bFlipImage=0);
	void	Release ();

	//! Attributes
	inline TYPE	Type () const {return (TYPE)CVLIB_MAT_DEPTH((int)type);};
	inline TYPE Type1() const {return (TYPE)CVLIB_MAT_TYPE((int)type);}
	inline int Rows () const {return rows;};
	inline int Cols () const {return cols;};
	inline int Step () const {return step;}
	inline int Channels() const {return CVLIB_MAT_CN((int)type);}
	inline int RowByteSize() const {return cols*step*CVLIB_MAT_CN((int)type);}
	inline Size SizeObject() const {return Size(cols, rows);}

	//! valid check operations
	inline bool	IsInterior (int nX, int nY) const { return (nX < cols && nX >= 0 && nY < rows && nY >= 0); }
	inline bool IsValid () const { return data.ptr!=NULL; }
	inline bool IsEqualSize(const Size& sz) const {return rows==sz.height && cols==sz.width;}
	inline bool IsEqualSize(const Mat& mat) const {return rows==mat.Rows() && cols==mat.Cols();}
	inline bool IsEqualMat(const Mat& mat) const {return IsEqualSize(mat) && type==mat.type;}
	inline bool IsContinuous() const { return data.ptr[1] == (data.ptr[0]+cols*CVLIB_ELEM_SIZE(type)*Channels()); }

	//! import and export, conversion operations
	virtual int FromFile (const char* szFilename);
	virtual int FromFile (FILE* fp);
	virtual int ToFile (const char* szFilename) const;
	virtual int ToFile (FILE* fp) const;
	void	FromMatlab(const SString& sFilename,const SString& sName);
	void	ToMatlab(const SString& sFilename,const SString& sName,const SString& sComment="",bool fAppend=true) const;
	bool	FromImage(const char* szFilename, DWORD nImageType=0);
	bool	ToImage (CoImage& image) const;
	bool	ToImage (const char* szFilename, DWORD nImageType=1/*bmp*/) const;
	bool	ToImage (const char* szFilename, DWORD nImageType, BOOL fExpand, float rMin[]/*={0.0f}*/, float rMax[]/*={1.0f}*/) const;
	bool	ToImage (const char* szFilename, DWORD nImageType, BOOL fExpand, float rMin=0.0f, float rMax=1.0f) const;
	void	ToC(const SString& szFilename, const SString& szVarName, bool fAppend=true) const;
	SString ToString() const;
    void	ToVector( Vec &v, bool rowWise = true ) const;

	void	Convert(TYPE type, CASTTYPE casttype=CT_Default);
	void	Convert(TYPE type, double alpha, double beta);
	void	ConvertTo(Mat& dst, TYPE type, double alpha=1.0, double beta=0.0);
	void	CopyTo(Mat& dst, const Mat& mask) const;
	void	ReverseChannel();

	//! Element Access
	double	Value (int i, int j, int cn=0) const;
	void	SetValue (double rVal);
	Mat*	SubMat(int nRow1, int nCol1, int nRow2, int nCol2) const;
	Mat*	SubMat(const Rect& region) const;
    Mat*	SubRefMat(int nRow1, int nCol1, int nRow2, int nCol2) const;
	void	SubRefMat(const Rect& region, Mat& mRef) const;
	void	SubMat(const Rect& region, Mat& mRef) const;

	void	ResizeRows (int rows) {this->rows = rows;}
	void	ResizeCols (int cols) {this->cols = cols;}

	void	Zero ();
	void	Identity(double val);
	void	Identity();
	double	Min(int cn=0) const;
	double	Max(int cn=0) const;
	double	Mean() const;
	double	Min(int& row, int& col) const;
	double	Max(int& row, int& col) const;
	void	MinMaxLoc(double* minVal, double* maxVal, Point2i* minLoc, Point2i* maxLoc, const Mat& mask) const;
	double	Norm(int norm_type, const Mat& mask) const;
	double	NormDiff(int norm_type, const Mat& other, const Mat& mask) const;

	Mat& operator=(const Mat& matSrc);
	Mat& operator=(const Scalar& value);
 	Mat& operator=(double value);
	bool IsSymmetric(void) const;

    // Statistical functions
	void MeanCol(Vec& vMean) const;
	double Std() const;
	void StdCol(Vec& vStd) const;
	double Sum() const;
	void SumCol(Vec& vSum) const;
	double Var() const;
	void VarCol(Vec& vVar) const;

    // matrix manipulation
	void ColVec(int i, Vec& vCol) const;
	void RowVec(int i, Vec& vRow) const;	
	void ToVec(Vec& v) const;

	void CombVert(Mat& Top,Mat& Bottom);

	void Diag(const Vec& vec);

	void ElementDivide(const Mat& matrix);
	void ElementMultiply(const Mat& matrix);

	// math functions
	void Sqr();
	void Sqrt();
    void Log();

    // misc
	void Kron(Mat& mX,Mat& mY);	
    void Rand();
    void OuterProduct(const Vec &v1, const Vec &v2);

    // Extract upper triangular part of matrix.
	void TriU(Mat& matrix, const int K) const;

	// Extract lower triangular part of matrix.
	void TriL(Mat& matrix, const int K) const;

    // Flips
    void FlipLR();
    void FlipUD();

	// Elementwise equal to
	void Eq(const double B, Mat& C) const;
	void Eq(const Mat& B, Mat& C) const;
	// Elementwise not equal to
	void Ne(const double B, Mat& C) const;
	void Ne(const Mat& B, Mat& C) const;
	// Elementwise less than
	void Lt(const double B, Mat& C) const;
	void Lt(const Mat& B, Mat& C) const;
	// Elementwise less than or equal
	void Le(const double B, Mat& C) const;
	void Le(const Mat& B, Mat& C) const;
	// Elementwise greater than
	void Gt(const double B, Mat& C) const;
	void Gt(const Mat& B, Mat& C) const;
	// Elementwise greater than or equal
	void Ge(const double B, Mat& C) const;
	void Ge(const Mat& B, Mat& C) const;

    // Determinant, Trace
    double Trace() const;
	Mat Inverted(void) const;
	Mat& Invert(void);
	Mat Transposed(void) const;
	Mat& Transpose(void);

	void Swap(Mat& other);

	// Simple Drawing Functions
	void	DrawMat(const Mat& img, const Point2i& pt=Point2i(), const float rOpacity=1);
	void	DrawRect(const Rect& rect, COLORREF color, int thick=1, const float rOpacity=1);
	void	DrawEllipse (const Rect& rect, COLORREF color, const float rOpacity=1);
	void	DrawEllipse (const RotatedRect& rect, COLORREF color, const float rOpacity=1);
	void	DrawPixel (int x, int y, COLORREF color, const float rOpacity=1);
	void	DrawPixel (const Point2i& point, COLORREF color, const float rOpacity=1);
	void	DrawLine (const Point2i& pt1, const Point2i& pt2, COLORREF color, const float rOpacity=1);
	void	DrawCross (const Point2i& pt, int nSide, COLORREF color, const float rOpacity=1);
	void	DrawPolygon (Point2i* pts, int num, COLORREF color, bool open=true, const float rOpacity=1);
	void	DrawString(const Point2i& pt, const char* szText, COLORREF color, const char* szFont,
		long lSize, long lWeight, BYTE bItalic, BYTE bUnderline, const float rOpacity = 1);
#if CVLIB_OS==CVLIB_OS_WIN32
	BYTE*	GetBMPInfoHeader() const;
	void	Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY) const;
	void	Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY,
		int nSrcX, int nSrcY, int nSrcCX, int nSrcCY) const;
	void	Draw(DWORD hDC) const;
	int		DrawString(DWORD hdc, long x, long y, const char* szText, COLORREF color, const char* szFont,
		long lSize, long lWeight, BYTE bItalic, BYTE bUnderline, const float rOpacity = 1);
	int		CreateFromHBITMAP(DWORD hBitmap, DWORD hPalette = 0);
	bool	LoadResource(DWORD hRes, DWORD imagetype, DWORD hModule=NULL);
	bool	Tile(DWORD hdc, Rect* rc);
	bool	CreateFromHANDLE(DWORD hMem);
	DWORD	CopyToHandle();
#endif

protected:
	
	TYPE type;	//! ÃÔµÛÌ© ÌººÏÌ© Âô
	int rows;	//! ÃÔÌ© ±¶ºã
	int cols;	//! µÛÌ© ±¶ºã
	int step;	//! ÂôËæ Åà¶£²÷ ÌººÏÌ© ¿Í±¨ (ÂôËæ Åàµ¹º· intÂôËÍ °ÒË§²÷ 4, byteÂôËÍ °ÒË§²÷ 1...)
	int depend;	//! ¸ôº¡¸õËÍ°¡¶¦ ±ýÀ°³­²÷ ±¨¸Ë
};

template <typename _Tp>class CVLIB_DECLSPEC Mat_ : public Mat
{
public:
	typedef Mat_<_Tp> _MyMat;
	typedef Vec_<_Tp> _MyVec;
	Mat_(){};
	Mat_(int rows, int cols);
	Mat_(_Tp** ptr, int rows, int cols);
	Mat_(_Tp* ptr, int rows, int cols, BOOL fAlloc = TRUE);
	Mat_(const Mat& mat);
	Mat_(const _MyMat& mat);
	int		Create(int rows, int cols);
	int		Create(_Tp** ptr, int rows, int cols);
	int		Create(_Tp* ptr, int rows, int cols, BOOL fAlloc = TRUE);
	int		Create(const _MyMat& mat, BOOL fCopy = FALSE);

	_MyMat&	operator=(const _MyMat& other);
	_Tp*	operator[](int index) { return (_Tp*)data.ptr[index]; }
	const _Tp*	operator[](int index) const { return (const _Tp*)data.ptr[index]; }
	_MyMat& operator=(double value);
	bool operator==(const _MyMat& refmatrix) const;
	bool operator!=(const _MyMat& refmatrix) const;
	bool operator<(const _MyMat& refmatrix) const;
	
	_MyMat operator+(const _MyMat& A) const;
	
	_MyMat operator-(void) const;
	_MyMat operator-(const _MyMat& A) const;
	
	_MyMat operator*(double dbl) const;
	_MyMat operator*(const _MyMat& refmatrix) const;
	_MyVec operator*(const _MyVec& refvector) const;
	
	_MyMat operator/(double dbl) const;
	
	_MyMat& operator+=(const _MyMat& A);
	_MyMat& operator-=(const _MyMat& A);
	_MyMat& operator*=(double dbl);
	_MyMat& operator*=(const _MyMat& A);
	_MyMat& operator/=(double dbl);
	
	_MyMat& EqSum(const _MyMat &refmatrixA, const _MyMat &refmatrixB);
	_MyMat& EqDiff(const _MyMat &refmatrixA, const _MyMat &refmatrixB);
	_MyMat& EqProd(const _MyMat &refmatrixA, const _MyMat &refmatrixB);
	
	_MyMat Transposed(void) const;
	_MyMat& Transpose(void);
	
	_MyVec Row(int r) const;
	_MyVec Column(int c) const;
	void Col(int i, _MyVec& vCol) const;
	void Row(int i, _MyVec& vRow) const;
	void SetRow(int r, const _MyVec &v);
	void SetColumn(int c, const _MyVec &v);
	
	double Det() const;
	_MyMat& Submatrix(const int newRows, const int newCols, _MyMat& submat, const int i0, const int j0) const;
	_MyMat Submatrix(const int newRows, const int newCols, const int i0 = 0, const int j0 = 0) const;
};

/**
 * @brief   ½Û¼¿´ô ÃÔµÛË¾ ÃÍ¸ÒÂÙ³Þ.
 *
 * @param   pmA [in] : ÃÔµÛ¼è¼¬»ô
 * @return  void 
 *
 * @see  Create(int rows, int cols, TYPE type)
 * 
 */
inline void CVLIB_DECLSPEC ReleaseMat (Mat* pmA)
{
	pmA->Release();
	delete pmA;
}


}

#include "xMat.h"

namespace CVLib
{
typedef Mat_<uchar>		Matb;
typedef Mat_<short>		Mats;
typedef Mat_<int>		Mati;
typedef Mat_<float>		Matf;
typedef Mat_<double>	Matd;
}

#endif //_MAT_H_
