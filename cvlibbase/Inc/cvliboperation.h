/*! 
 * \file    cvliboperation.h
 * \ingroup base
 * \brief   cvlibbaseº·°ÖËæº· ¶®Ë¦Â×²÷ »ôµê°é¼¿´ÉË¾ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */
#ifndef _CVLIBoperation_H_
#define _CVLIBoperation_H_

#include "cvlibmacros.h"

namespace CVLib
{

template<typename _Tp> static inline _Tp saturate_cast(uchar v) { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(short v) { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(int v) { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(float v) { return _Tp(v); }
template<typename _Tp> static inline _Tp saturate_cast(double v) { return _Tp(v); }

template<typename _Tp> inline Scalar_<_Tp> Scalar_<_Tp>::Mul(const Scalar_<_Tp>& t, double scale ) const
{
	return Scalar_<_Tp>( saturate_cast<_Tp>(this->val[0]*t.val[0]*scale),
		saturate_cast<_Tp>(this->val[1]*t.val[1]*scale),
		saturate_cast<_Tp>(this->val[2]*t.val[2]*scale),
		saturate_cast<_Tp>(this->val[3]*t.val[3]*scale));
}

template<typename _Tp> template<typename T2> inline void Scalar_<_Tp>::ConvertTo(T2* buf, int cn, int unroll_to) const
{
	int i;
	assert(cn <= 4);
	for( i = 0; i < cn; i++ )
		buf[i] = saturate_cast<T2>(this->val[i]);
	for( ; i < unroll_to; i++ )
		buf[i] = buf[i-cn];
}
template<typename _Tp> inline Scalar_<_Tp>::Scalar_()
{ this->val[0] = this->val[1] = this->val[2] = this->val[3] = 0; }

template<typename _Tp> inline Scalar_<_Tp>::Scalar_(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
{ this->val[0] = v0; this->val[1] = v1; this->val[2] = v2; this->val[3] = v3; }

template<typename _Tp> inline Scalar_<_Tp>::Scalar_(_Tp v0)
{ this->val[0] = v0; this->val[1] = this->val[2] = this->val[3] = 0; }

template<typename _Tp> inline Scalar_<_Tp> Scalar_<_Tp>::All(_Tp v0)
{ return Scalar_<_Tp>(v0, v0, v0, v0); }

template<typename _Tp> template<typename T2> inline Scalar_<_Tp>::operator Scalar_<T2>() const
{
	return Scalar_<T2>(saturate_cast<T2>(this->val[0]),
		saturate_cast<T2>(this->val[1]),
		saturate_cast<T2>(this->val[2]),
		saturate_cast<T2>(this->val[3]));
}

//////////////////////////////////////////////////////////////////////////
template<typename _Tp> static inline bool operator == ( const Scalar_<_Tp>& a, const Scalar_<_Tp>& b )
{
	return a.val[0] == b.val[0] && a.val[1] == b.val[1] &&
		a.val[2] == b.val[2] && a.val[3] == b.val[3];
}

template<typename _Tp> static inline bool operator != ( const Scalar_<_Tp>& a, const Scalar_<_Tp>& b )
{
	return a.val[0] != b.val[0] || a.val[1] != b.val[1] ||
		a.val[2] != b.val[2] || a.val[3] != b.val[3];
}

template<typename _Tp> static inline Scalar_<_Tp> operator + (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
	return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] + b.val[0]),
		saturate_cast<_Tp>(a.val[1] + b.val[1]),
		saturate_cast<_Tp>(a.val[2] + b.val[2]),
		saturate_cast<_Tp>(a.val[3] + b.val[3]));
}

template<typename _Tp> static inline Scalar_<_Tp> operator - (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
	return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] - b.val[0]),
		saturate_cast<_Tp>(a.val[1] - b.val[1]),
		saturate_cast<_Tp>(a.val[2] - b.val[2]),
		saturate_cast<_Tp>(a.val[3] - b.val[3]));
}

template<typename _Tp> static inline Scalar_<_Tp> operator * (const Scalar_<_Tp>& a, _Tp alpha)
{
	return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] * alpha),
		saturate_cast<_Tp>(a.val[1] * alpha),
		saturate_cast<_Tp>(a.val[2] * alpha),
		saturate_cast<_Tp>(a.val[3] * alpha));
}

template<typename _Tp> static inline Scalar_<_Tp> operator * (_Tp alpha, const Scalar_<_Tp>& a)
{
	return a*alpha;
}

template<typename _Tp> static inline Scalar_<_Tp> operator - (const Scalar_<_Tp>& a)
{
	return Scalar_<_Tp>(saturate_cast<_Tp>(-a.val[0]), saturate_cast<_Tp>(-a.val[1]),
		saturate_cast<_Tp>(-a.val[2]), saturate_cast<_Tp>(-a.val[3]));
}

template<typename _Tp> static inline Scalar_<_Tp>& operator += (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
	a.val[0] = saturate_cast<_Tp>(a.val[0] + b.val[0]);
	a.val[1] = saturate_cast<_Tp>(a.val[1] + b.val[1]);
	a.val[2] = saturate_cast<_Tp>(a.val[2] + b.val[2]);
	a.val[3] = saturate_cast<_Tp>(a.val[3] + b.val[3]);
	return a;
}

template<typename _Tp> static inline Scalar_<_Tp>& operator -= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
	a.val[0] = saturate_cast<_Tp>(a.val[0] - b.val[0]);
	a.val[1] = saturate_cast<_Tp>(a.val[1] - b.val[1]);
	a.val[2] = saturate_cast<_Tp>(a.val[2] - b.val[2]);
	a.val[3] = saturate_cast<_Tp>(a.val[3] - b.val[3]);
	return a;
}

template<typename _Tp> static inline Scalar_<_Tp>& operator *= ( Scalar_<_Tp>& a, _Tp v )
{
	a.val[0] = saturate_cast<_Tp>(a.val[0] * v);
	a.val[1] = saturate_cast<_Tp>(a.val[1] * v);
	a.val[2] = saturate_cast<_Tp>(a.val[2] * v);
	a.val[3] = saturate_cast<_Tp>(a.val[3] * v);
	return a;
}

}

#endif //_CVLIBoperation_H_