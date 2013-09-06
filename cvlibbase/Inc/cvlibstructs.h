/*! 
 * \file    cvlibstructs.h
 * \ingroup base
 * \brief   cvlibbaseº·°ÖËæº· ¶®Ë¦Â×²÷ »ôµê°é¼¿´ÉË¾ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */
#ifndef _CVLIBstructs_H_
#define _CVLIBstructs_H_

#include "cvlibmacros.h"

namespace CVLib 
{

	/**
	 * @brief  ¼°Ì© °é¼¿¾Ë
	 */
	struct SPoint { int x; int y; };
	inline SPoint	PointObject (int x, int y)
	{
		SPoint	point;
		point.x = x; point.y = y;
		return point;
	}

	#define CV_TERMCRIT_ITER    1	//!¸É¸ôÃäºã
	#define CV_TERMCRIT_EPS     2	//!ºãµÜ±¨¼Ó
	class CVLIB_DECLSPEC TermCriteria
	{
	public:
		int		nType;		/**< Âô»¥ (¸É¸ôÃä?E?EEºãµÜ?E? */
		int		nMaxIter;	/**< ¾×´Ý¸É¸ôÃäºã*/
		double	rEpsilon;	/**< ºãµÜ*/
		TermCriteria () :nType(CV_TERMCRIT_ITER),nMaxIter(0),rEpsilon(0){}
		TermCriteria (int type, int iter, double epsilon) :nType(type),nMaxIter(iter),rEpsilon(epsilon){}
		TermCriteria (const TermCriteria& other) :nType(other.nType), nMaxIter(other.nMaxIter), rEpsilon(other.rEpsilon){}
		TermCriteria& operator=(const TermCriteria& other) {nType=other.nType; nMaxIter=other.nMaxIter; rEpsilon= other.rEpsilon; return *this;}

		TermCriteria Check (double default_eps, int default_max_iters) const;
	};

#define CVLIB_TITLE_LEN		64
#define CVLIB_COPYRIGHT_LEN		64
	typedef struct tagLibraryInfo 
	{ 
		char szTitle[CVLIB_TITLE_LEN];         /**< º·°ÖËË¶§ */
		char szCopyright[CVLIB_COPYRIGHT_LEN]; /**< copyright */
		int nMajor;                            /**< ±¨¸õÁÆ¸õ¸Ü?E*/
		int nMinor;                            /**< ³ÞËÁ ÁÆ¸õ¸Ü?E*/
		int nBuild;                            /**< ¼çÁÆ¸õ¸Ü?E*/ 
	} SLibraryInfo;

	/**
	 * @brief  »°°é¼¿¾Ë 
	 */
	typedef struct tagRGBQUAD1 
	{
		BYTE    rgbBlue;
		BYTE    rgbGreen;
		BYTE    rgbRed;
		BYTE    rgbReserved;
	} RGBQUAD1;

	/**
	 * @brief  BMP¼³¸óÌ© ·£¶®¹¢¶¦ ±ã¶®Â×?E°é¼¿¾Ë
	 */
	typedef struct tagBITMAPINFOHEADER1
	{
		DWORD      biSize;
		long       biWidth;
		long       biHeight;
		WORD       biPlanes;
		WORD       biBitCount;
		DWORD      biCompression;
		DWORD      biSizeImage;
		long       biXPelsPerMeter;
		long       biYPelsPerMeter;
		DWORD      biClrUsed;
		DWORD      biClrImportant;
	} BITMAPINFOHEADER1;

	/**
	 * @brief  BMPÁÄËÎÌ© ·£¶®¹¢¶¦ ±ã¶®Â×?E°é¼¿¾Ë
	 */
	typedef struct tagBITMAPFILEHEADER1 
	{
		WORD    bfType;
		DWORD   bfSize;
		WORD    bfReserved1;
		WORD    bfReserved2;
		DWORD   bfOffBits;
	} BITMAPFILEHEADER1;
	
	typedef struct tagBITMAPINFO1 { // bmi 
		BITMAPINFOHEADER1 bmiHeader; 
		RGBQUAD1          bmiColors[1]; 
	} BITMAPINFO1; 

	/**
	 * @brief  BMP¼³¸óÌ© ·£¶®¹¢¶¦ ±ã¶®Â×?E°é¼¿¾Ë
	 */
	typedef struct tagBITMAPCOREHEADER1 
	{
		DWORD   bcSize;
		WORD    bcWidth;
		WORD    bcHeight;
		WORD    bcPlanes;
		WORD    bcBitCount;
	} BITMAPCOREHEADER1;
	
	typedef struct tagRGBTRIPLE1 {
		BYTE    rgbtBlue;
		BYTE    rgbtGreen;
		BYTE    rgbtRed;
	} RGBTRIPLE1;

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

#undef GetRValue
#undef GetGValue
#undef GetBValue
#undef RGB

#define GetRValue(rgb)      ((CVLib::BYTE)(rgb))
#define GetGValue(rgb)      ((CVLib::BYTE)(((CVLib::WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((CVLib::BYTE)((rgb)>>16))
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((CVLib::WORD)((CVLib::BYTE)(g))<<8))|(((CVLib::DWORD)(CVLib::BYTE)(b))<<16)))

template <class TypeName> class Point3_;
template <class TypeName> class Point4_;

template <class TypeName>
class CVLIB_DECLSPEC Point2_
{
public:
	typedef Point2_<TypeName> _MyPoint;

	union {
		struct {
			TypeName x,y;        // standard names for components
		};
		struct {
			TypeName s,t;        // standard names for components
		};
		TypeName vec_array[2];     // array access
	};
public:
	Point2_() :x(0),y(0){}
	Point2_(const TypeName* xy) :x(xy[0]),y(xy[1]){}
	Point2_(TypeName const x_, TypeName const y_) :x(x_),y(y_){}
	Point2_(_MyPoint const &p) :x(p.x),y(p.y){}
	Point2_(Point3_<TypeName> const &p) {TypeName k=TypeName(1/p.z);x=k*p.x;y=k*p.y;}
	Point2_(SPoint const& initPt) {x=(TypeName)initPt.x, y=(TypeName)initPt.y;}

	operator SPoint() const{SPoint pt; pt.x=(int)x;pt.y=(int)y;return pt;}
	_MyPoint operator= (const _MyPoint& a) {x=a.x; y=a.y; return *this;}
	_MyPoint operator- ()        const { return _MyPoint(-x, -y); }
	_MyPoint operator+ (const _MyPoint& a) const { return _MyPoint(x + a.x, y + a.y); }
	_MyPoint operator- (const _MyPoint& a) const { return _MyPoint(x - a.x, y - a.y); }
	_MyPoint operator* (const float & lambda) const { return _MyPoint((TypeName)(x*lambda), (TypeName)(y*lambda)); }
	_MyPoint operator* (const _MyPoint &a) const { return _MyPoint((TypeName)(x*a.x), (TypeName)(y*a.y)); }
	bool  operator< (const _MyPoint& a) const { return (x <  a.x) && (y <  a.y); }
	bool  operator<=(const _MyPoint& a) const { return (x <= a.x) && (y <= a.y); }
	bool  operator> (const _MyPoint& a) const { return (x >  a.x) && (y >  a.y); }
	bool  operator>=(const _MyPoint& a) const { return (x >= a.x) && (y >= a.y); }
	bool  operator==(const _MyPoint& a) const { return (x == a.x) && (y == a.y); }
	bool  operator!=(const _MyPoint& a) const { return (x != a.x) || (y != a.y); }
	void operator+=(const _MyPoint a) { x+=a.x; y+=a.y;}
	void operator-=(const _MyPoint a) { x-=a.x; y-=a.y;}
	void Inc(const int i, const int j) { x+=i; y+=j;}
	void Dec(const int i, const int j) { x-=i; y-=j;}
	double DistTo(const _MyPoint& o) const { return sqrt(double((x-o.x)*(x-o.x) + (y-o.y)*(y-o.y))); }
	double DistTo(TypeName const x_, TypeName const y_) const { return sqrt(double((x-x_)*(x-x_) + (y-y_)*(y-y_))); }

	_MyPoint & operator*=(const float & lambda) { x=(TypeName)(x*lambda); y=(TypeName)(y*lambda); return *this; }
	TypeName & operator[](int i) { return vec_array[i]; }
	const TypeName operator[](int i) const { return vec_array[i];}
	bool FromFile(FILE* pFile){
		if ( fread(vec_array, sizeof(vec_array), 1, pFile) == 1)
			return true;
		return false;
	}
	bool ToFile(FILE* pFile) const{
		if ( fwrite(vec_array, sizeof(vec_array), 1, pFile) == 1)
			return true;
		return false;
	}
	float Norm2() const {return sqrtf(Energy());}
	float Energy() const {return (float)(x*x+y*y);}
};

/**
* @brief  µßÊßÌ© °é¼¿¾Ë
*/
template <class TypeName>
class CVLIB_DECLSPEC Size_
{
public:
	typedef Size_<TypeName> _MySize;
public:
	TypeName width;		/**< µßÊßÌ© ²¸¹¾ */
	TypeName height;		/**< µßÊßÌ© ²ÚËË */

	Size_() {width=height=0;}
	Size_(TypeName w,TypeName h) :width(w),height(h){}
	Size_(const _MySize& other) :width(other.width),height(other.height){}
	Size_(const Point2_<TypeName>& pt) {width=pt.x,height=pt.y;}

	_MySize& operator=(const _MySize& other) {width=other.width;height=other.height; return *this;}
	inline bool operator==(const Size_& o) const {return width==o.width && height==o.height;}
	inline bool operator!=(const Size_& o) const {return width!=o.width || height!=o.height;}
	inline _MySize operator*(float s) const {return _MySize((TypeName)(width*s),(TypeName)(height*s));}
	inline void operator*=(float s){ width=(TypeName)(width*s), height=(TypeName)(height*s);}
	inline TypeName Area() const{return width*height;}
};

//***bd*** simple floating point point
template <class TypeName>
class CVLIB_DECLSPEC Point3_
{
public:
	typedef Point3_<TypeName> _MyPoint3;

	union {
		struct {
			TypeName x,y,z;          // standard names for components
		};
		struct {
			TypeName s,t,r;          // standard names for components
		};
		TypeName vec_array[3];     // array access
	};
public:
	Point3_():x(0),y(0),z(0) {}
	Point3_(const TypeName *xyz):x(xyz[0]),y(xyz[1]),z(xyz[2]){}
	Point3_(TypeName const x_, TypeName const y_, TypeName const z_):x(x_),y(y_),z(z_){}
	Point3_(Point2_<TypeName> const &p)	:x(p.x),y(p.y),z((TypeName)1.0){}
	Point3_(_MyPoint3 const &p)	:x(p.x),y(p.y),z(p.z){}
	Point3_(Point4_<TypeName> const &p)	:x(p.x),y(p.y),z(p.z){}

	_MyPoint3 operator= (const _MyPoint3& a) {x=a.x; y=a.y; z=a.z; return *this;}
	_MyPoint3 operator- ()       const  { return _MyPoint3(-x, -y, -z); }
	_MyPoint3 operator+ (const _MyPoint3& a) const { return _MyPoint3(x + a.x, y + a.y, z+a.z); }
	_MyPoint3 operator- (const _MyPoint3& a) const { return _MyPoint3(x - a.x, y - a.y, z-a.z); }
	_MyPoint3 operator* (const TypeName a)const  { return _MyPoint3( (TypeName)(a*x),(TypeName)(a*y),(TypeName)(a*z)); }
//	_MyPoint3 operator* (const _MyPoint3& a) const { return _MyPoint3(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);}
	_MyPoint3 operator* (const _MyPoint3& a) const  { return _MyPoint3( (TypeName)(a.x*x),(TypeName)(a.y*y),(TypeName)(a.z*z)); }
	_MyPoint3 operator^ (const _MyPoint3& a)const  { return _MyPoint3(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x); }
	bool  operator< (const _MyPoint3& a) const { return (x <  a.x) && (y <  a.y) && (z <  a.z); }
	bool  operator<=(const _MyPoint3& a) const { return (x <= a.x) && (y <= a.y) && (z <= a.z); }
	bool  operator> (const _MyPoint3& a) const { return (x >  a.x) && (y >  a.y) && (z >  a.z); }
	bool  operator>=(const _MyPoint3& a) const { return (x >= a.x) && (y >= a.y) && (z >= a.z); }
	bool  operator==(const _MyPoint3& a) const { return (x == a.x) && (y == a.y) && (z == a.z); }
	bool  operator!=(const _MyPoint3& a) const { return (x != a.x) || (y != a.y) || (z != a.z); }
	void operator+=(const _MyPoint3 a) { x+=a.x; y+=a.y; z+=a.z;}
	void operator-=(const _MyPoint3 a) { x-=a.x; y-=a.y; z-=a.z;}
	void Inc(const int i, const int j, const int k) { x+=i; y+=j; z+=k;}
	void Dec(const int i, const int j, const int k) { x-=i; y-=j; z-=k;}
	double DistTo(const _MyPoint3& o) const { return sqrt(double((x-o.x)*(x-o.x) + (y-o.y)*(y-o.y) + (z-o.z)*(z-o.z))); }
	double DistTo(TypeName const x_, TypeName const y_, TypeName const z_) const { return sqrt(double((x-x_)*(x-x_) + (y-y_)*(y-y_) + (z-z_)*(z-z_))); }
	TypeName dot(const _MyPoint3& a) const {return x*a.x+y*a.y+z*a.z;}
	_MyPoint3 & operator*=(const TypeName & lambda) { x=(TypeName)(x*lambda); y=(TypeName)(y*lambda); z=(TypeName)(z*lambda); return *this; }
	TypeName & operator[](int i) { return vec_array[i]; }
	const TypeName operator[](int i) const { return vec_array[i];}

	TypeName normalize() {
		double norm = sqrt( (double)(x * x + y * y + z * z) );
		if (norm > 10e-6)
			norm = (1.0 / norm);
		else
			norm = 0.0;
		x = (TypeName)(x*norm);
		y = (TypeName)(y*norm);
		z = (TypeName)(z*norm);
		return (TypeName)norm;
	}
	TypeName sq_norm() const { return (TypeName)(x * x + y * y + z * z); }
	TypeName norm() const { return (TypeName)sqrt( double(x * x + y * y + z * z) ); }
	bool FromFile(FILE* pFile){
		if ( fread(vec_array, sizeof(vec_array), 1, pFile) == 1)
			return true;
		return false;
	}
	bool ToFile(FILE* pFile) const{
		if ( fwrite(vec_array, sizeof(vec_array), 1, pFile) == 1)
			return true;
		return false;
	}
};


template <class TypeName>
class CVLIB_DECLSPEC Point4_
{
public:
	typedef Point4_<TypeName> _MyPoint4;

	union {
		struct {
			TypeName x,y,z,w;          // standard names for components
		};
		struct {
			TypeName s,t,r,q;          // standard names for components
		};
		TypeName vec_array[4];     // array access
	};
public:
	Point4_() {x=y=z=w=0;}
	Point4_(const TypeName* xyzw):x(xyzw[0]),y(xyzw[1]),z(xyzw[2]),w(xyzw[3]){}
	Point4_(TypeName const x_, TypeName const y_, TypeName const z_, TypeName const w_):x(x_),y(y_),z(z_),w(w_){}
	Point4_(Point3_<TypeName> const &p)	:x(p.x),y(p.y),z(p.z),w(TypeName(1.0)){}
	Point4_(_MyPoint4 const &p)	:x(p.x),y(p.y),z(p.z),w(p.w){}

	_MyPoint4 operator= (const _MyPoint4& a) {x=a.x; y=a.y; z=a.z; w=a.w; return *this;}
	_MyPoint4 operator- ()       const  { return _MyPoint4(-x, -y, -z, -w); }
	_MyPoint4 operator+ (const _MyPoint4& a) const { return _MyPoint4(x + a.x, y + a.y, z+a.z, w+a.w); }
	_MyPoint4 operator- (const _MyPoint4& a) const { return _MyPoint4(x - a.x, y - a.y, z-a.z, w-a.w); }
	_MyPoint4 operator* (const _MyPoint4& a) const { return _MyPoint4(x*a.x, y*a.y, z*a.z, w*a.w); }
	_MyPoint4 operator* (const TypeName & lambda) const { return _MyPoint4((TypeName)(x*lambda), (TypeName)(y*lambda), (TypeName)(z*lambda), (TypeName)(w*lambda)); }
	bool  operator==(const _MyPoint4& a) const { return (x == a.x) && (y == a.y) && (z == a.z) && (w == a.w); }
	bool  operator!=(const _MyPoint4& a) const { return (x != a.x) || (y != a.y) || (z != a.z) || (w != a.w); }
	void operator+=(const _MyPoint4 a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w;}
	void operator-=(const _MyPoint4 a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w;}

	_MyPoint4 & operator*=(const TypeName & lambda) { x=(TypeName)(x*lambda); y=(TypeName)(y*lambda); z=(TypeName)(z*lambda); w=(TypeName)(w*lambda); return *this; }
	TypeName & operator[](int i) { return vec_array[i]; }
	const TypeName operator[](int i) const { return vec_array[i];}
	bool FromFile(FILE* pFile){
		if ( fread(vec_array, sizeof(vec_array), 1, pFile) == 1)
			return true;
		return false;
	}
	bool ToFile(FILE* pFile)const{
		if ( fwrite(vec_array, sizeof(vec_array), 1, pFile) == 1)
			return true;
		return false;
	}
};

#ifndef DEF_GeoTypes
#define DEF_GeoTypes
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point2_<int>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point2_<float>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point2_<double>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point3_<int>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point3_<float>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point3_<double>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point4_<int>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point4_<float>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Point4_<double>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Size_<int>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Size_<float>;
#endif

//and simple rectangle
class CVLIB_DECLSPEC Rectf
{
public:
	Rectf();
	Rectf(float const x1_, float const y1_, float const x2_, float const y2_);
	Rectf(Point2_<float> const &bl, Point2_<float> const &tr);
	Rectf(Rectf const &p);

	float Surface() const;
	Rectf CrossSection(Rectf const &r2) const;
	Point2_<float> Center() const;
	float Width() const;
	float Height() const;
	Rectf operator-();
	Rectf operator= (const Rectf& a) {topLeft=a.topLeft; bottomRight=a.bottomRight; return *this;}
	void Padding(float const rPadding);
public:
	Point2_<float> topLeft;
	Point2_<float> bottomRight;
};

template <class TypeName> class CVLIB_DECLSPEC Rect_
{
public:
	typedef Rect_<TypeName> _MyRect;
public:
	TypeName x, y, width, height;

	Rect_() {x=y=width=height=0;}
	Rect_(TypeName x_,TypeName y_,TypeName w,TypeName h) :x(x_),y(y_),width(w),height(h){}
	Rect_(const _MyRect& other) :x(other.x),y(other.y),width(other.width),height(other.height){}

	_MyRect& operator=(const _MyRect& other) {x=other.x;y=other.y;width=other.width;height=other.height; return *this;}
	inline bool operator==(const _MyRect& t) {return x==t.x&& y==t.y&&width==t.width&&height==t.height;}
	inline Point2_<TypeName> tl() const {return Point2_<TypeName>(x, y);}
	inline Point2_<TypeName> br() const {return Point2_<TypeName>(x+width, y+height);}
	inline Point2_<int> Centeri() const {return Point2_<int>((int)(x+width/2.0f), (int)(y+height/2.0f));}
	inline Point2_<float> Centerf() const {return Point2_<float>(x+width/2.0f, y+height/2.0f);}
	inline TypeName LimX () const {return x+width;}
	inline TypeName LimY () const {return y+height;}
	inline Size_<TypeName> SizeObject() const {return Size_<TypeName>(width, height);}
	inline void SetSize(const Size_<TypeName>& sz) {width=sz.width;height=sz.height;}
	inline bool PtInRect(const Point2_<TypeName>& pt) const {return pt.x>=x&&pt.y>=y&&pt.x<x+width&&pt.y<y+height;}
	inline _MyRect Scale(float sx, float sy) const {return _MyRect((TypeName)(x*sx),(TypeName)(y*sy),(TypeName)(width*sx),(TypeName)(height*sy));}
	inline TypeName Area() const{return width*height;}
	inline _MyRect CrossSection(const _MyRect&r2) const {
		Rectf rect1((float)x,(float)y,(float)(x+width),(float)(y+height));
		Rectf rect2((float)r2.x,(float)r2.y,(float)(r2.x+r2.width),(float)(r2.y+r2.height));
		Rectf cross=rect1.CrossSection(rect2);
		return _MyRect((TypeName)cross.topLeft.x, (TypeName)cross.topLeft.y, 
			(TypeName)(cross.bottomRight.x-cross.topLeft.x), (TypeName)(cross.bottomRight.y-cross.topLeft.y));
	}
	inline _MyRect Union(const _MyRect& r) const {
		TypeName brx=LimX(), bry=LimY();
		TypeName brx_=r.LimX(), bry_=r.LimY();
		TypeName ux1=MIN(x, r.x), uy1=MIN(y, r.y);
		TypeName ux2=MAX(brx, brx_), uy2=MAX(bry, bry_);
		return _MyRect(ux1, uy1, ux2-ux1, uy2-uy1);
	}
};

// class CVLIB_DECLSPEC Rect : public Rect_<int>
// {
// public:
// // 	Rect() {x=y=width=height=0;}
// // 	Rect(int x_,int y_,int w,int h) {x=x_,y=y_,width=w,height=h;}
// // 	Rect(const Rect& other) {x=other.x,y=other.y,width=other.width,height=other.height;}
// //	Rect& operator=(const Rect& other) {x=other.x;y=other.y;width=other.width;height=other.height; return *this;}
// //	inline bool operator==(const Rect& t) {return x==t.x&& y==t.y&&width==t.width&&height==t.height;}
// // 	inline Point2i Centeri() const {return Point2i(x+width/2, y+height/2);}
// // 	inline Point2f Centerf() const {return Point2f(x+width/2.0f, y+height/2.0f);}
// //	inline Rect Scale(float sx, float sy) const { return Rect((int)(x*sx+0.5f),(int)(y*sy+0.5f),(int)(width*sx+0.5f),(int)(height*sy+0.5f));}
// //	inline bool PtInRect(const Point2i& pt) const {return pt.x>=x&&pt.y>=y&&pt.x<x+width&&pt.y<y+height;}
// 	inline Rect CrossSection(const Rect&r2) const {
// 		Rectf rect1((float)x,(float)y,(float)(x+width),(float)(y+height));
// 		Rectf rect2((float)r2.x,(float)r2.y,(float)(r2.x+r2.width),(float)(r2.y+r2.height));
// 		Rectf cross=rect1.CrossSection(rect2);
// 		return Rect((int)cross.topLeft.x, (int)cross.topLeft.y, (int)(cross.bottomRight.x-cross.topLeft.x), (int)(cross.bottomRight.y-cross.topLeft.y));
// 	}
// 	inline Rect Union(const Rect& r) const {
// 		int brx=LimX(), bry=LimY();
// 		int brx_=r.LimX(), bry_=r.LimY();
// 		int ux1=MIN(x, r.x), uy1=MIN(y, r.y);
// 		int ux2=MAX(brx, brx_), uy2=MAX(bry, bry_);
// 		return Rect(ux1, uy1, ux2-ux1, uy2-uy1);
// 	}
// };

#ifndef DEF_GeoTypes_Rect
#define DEF_GeoTypes_Rect
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Rect_<int>;
EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Rect_<float>;
#endif

typedef Rect_<int>		Rect;
typedef Size_<int>		Size;
typedef Size_<float>	Sizef;

typedef Point2_<int> Point2i;
typedef Point2_<float> Point2f;
typedef Point2_<double> Point2d;

typedef Point3_<int> Point3i;
typedef Point3_<float> Point3f;
typedef Point3_<double> Point3d;

typedef Point2i vec2i;
typedef Point2f vec2f;
typedef Point2d vec2d;
typedef Point3i vec3i;
typedef Point3f vec3f;
typedef Point3d vec3d;

typedef Point4_<int> vec4i;
typedef Point4_<float> vec4f;
typedef Point4_<double> vec4d;

typedef Point2f vec2;
typedef Point3f vec3;
typedef Point4_<float> vec4;

class CVLIB_DECLSPEC RotatedRect
{
public:
	//! various constructors
	RotatedRect();
	RotatedRect(const Point2f& _center, const Sizef& _size, float _angle);
	RotatedRect(const RotatedRect& box);
	void points(Point2f pts[]) const;//! returns 4 vertices of the rectangle
	Rect boundingRect() const;//! returns the minimal up-right rectangle containing the rotated rectangle

	Point2f center; //< the rectangle mass center
	Sizef size;    //< width and height of the rectangle
	float angle;    //< the rotation angle. When the angle is 0, 90, 180, 270 etc., the rectangle becomes an up-right rectangle. 
};

template<typename _Tp> class CVLIB_DECLSPEC DataDepth {public: enum { value = -1, fmt=(int)'\0' }; };

template<> class DataDepth<bool> { public: enum { value = MAT_Tbyte, fmt=(int)'u' }; };
template<> class DataDepth<char> { public: enum { value = MAT_Tchar, fmt=(int)'u' }; };
template<> class DataDepth<uchar> { public: enum { value = MAT_Tbyte, fmt=(int)'u' }; };
template<> class DataDepth<short> { public: enum { value = MAT_Tshort, fmt=(int)'s' }; };
template<> class DataDepth<int> { public: enum { value = MAT_Tint, fmt=(int)'i' }; };
template<> class DataDepth<float> { public: enum { value = MAT_Tfloat, fmt=(int)'f' }; };
template<> class DataDepth<double> { public: enum { value = MAT_Tdouble, fmt=(int)'d' }; };
template<typename _Tp> class DataDepth<_Tp*> { public: enum { value = MAT_USRTYPE, fmt=(int)'r' }; };

template<typename _Tp> class CVLIB_DECLSPEC Scalar_
{
public:
	Scalar_();
	Scalar_(_Tp v0, _Tp v1, _Tp v2=0, _Tp v3=0);
	Scalar_(const Scalar_<_Tp>& s);
	Scalar_(_Tp v0);
	static Scalar_<_Tp> All(_Tp v0);

	template<typename T2> operator Scalar_<T2>() const;

	Scalar_<_Tp> Mul(const Scalar_<_Tp>& t, double scale=1 ) const;
	template<typename T2> void ConvertTo(T2* buf, int channels, int unroll_to=0) const;

	_Tp operator [](int i) const { return val[i]; }
	_Tp& operator[](int i) { return val[i]; }

	_Tp val[4];
};

typedef Scalar_<double> Scalar;

class CVLIB_DECLSPEC Slice
{
public:
	int  start_index, end_index;
	Slice(){start_index=0,end_index=0;}
	Slice(int start, int end){start_index=start,end_index=end;}
	Slice(const Slice& t){start_index=t.start_index,end_index=t.end_index;}
};

}

/*!
 * \defgroup math Mathematics Classes
 * \brief Mathematics Library Classes Related Definition
 */

/*!
 * \defgroup base Object Base Classes
 * \brief Object Base Library Classes Related Definition
 */

#include "cvliboperation.h"

// namespace CVLib
// {
// #ifndef DEF_Scalars
// #define DEF_Scalars
// EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Scalar_<uchar>;
// EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Scalar_<short>;
// EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Scalar_<int>;
// EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Scalar_<float>;
// EXPIMP_TEMPLATE template class CVLIB_DECLSPEC Scalar_<double>;
// #endif
// }

#endif //_CVLIBstructs_H_