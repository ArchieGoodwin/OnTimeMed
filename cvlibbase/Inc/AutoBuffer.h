
/*!
* \file	AutoBuffer.h
* \ingroup cvlibbase
* \brief   class for automatically new and delete memory.
* \author  Cholu Kim
*/

#pragma once

#include "cvlibmacros.h"

namespace CVLib
{

template<typename _Tp, int fixed_size=4096/sizeof(_Tp)+8> class CVLIB_DECLSPEC AutoBuffer
{
public:
	typedef _Tp value_type;

	AutoBuffer();
	AutoBuffer(int _size);
	~AutoBuffer();

	void Alloc(int _size);
	void Dealloc();
	operator _Tp* ();
	operator const _Tp* () const;

protected:
	_Tp* ptr;
	int size;
	_Tp buf[fixed_size];
};

//////////////////// Generic ref-counting pointer class for C/C++ objects ////////////////////////

template<typename _Tp> class CVLIB_DECLSPEC Ptr
{
public:
	Ptr();
	Ptr(_Tp* _obj);
	~Ptr();
	Ptr(const Ptr& ptr);
	Ptr& operator = (const Ptr& ptr);
	void addref();
	void release();
	void delete_obj();
	bool empty() const;

	_Tp* operator -> ();
	const _Tp* operator -> () const;

	operator _Tp* ();
	operator const _Tp*() const;
protected:
	_Tp* obj;
	int* refcount;
};

template<typename _Tp, int fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer()
: ptr(buf), size(fixed_size) {}

template<typename _Tp, int fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer(int _size)
: ptr(buf), size(fixed_size) { Alloc(_size); }

template<typename _Tp, int fixed_size> inline AutoBuffer<_Tp, fixed_size>::~AutoBuffer()
{ Dealloc(); }

template<typename _Tp, int fixed_size> inline void AutoBuffer<_Tp, fixed_size>::Alloc(int _size)
{
	if(_size <= size)
		return;
	Dealloc();
	if(_size > fixed_size)
	{
		ptr = new _Tp[_size];
		size = _size;
	}
}

template<typename _Tp, int fixed_size> inline void AutoBuffer<_Tp, fixed_size>::Dealloc()
{
	if( ptr != buf )
	{
		delete []ptr;
		ptr = buf;
		size = fixed_size;
	}
}

template<typename _Tp, int fixed_size> inline AutoBuffer<_Tp, fixed_size>::operator _Tp* ()
{ return ptr; }

template<typename _Tp, int fixed_size> inline AutoBuffer<_Tp, fixed_size>::operator const _Tp* () const
{ return ptr; }

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Ptr ////////////////////////////////////////
static int _XADD(int* addr, int delta)
{ int tmp = *addr; *addr += delta; return tmp; }

template<typename _Tp> inline Ptr<_Tp>::Ptr() : obj(0), refcount(0) {}
template<typename _Tp> inline Ptr<_Tp>::Ptr(_Tp* _obj) : obj(_obj)
{
	if(obj)
	{
		refcount = new int;
		*refcount = 1;
	}
	else
		refcount = 0;
}

template<typename _Tp> inline void Ptr<_Tp>::addref()
{ if( refcount ) _XADD(refcount, 1); }

template<typename _Tp> inline void Ptr<_Tp>::release()
{
	if( refcount && _XADD(refcount, -1) == 1 )
	{
		delete_obj();
		delete refcount;
	}
	refcount = 0;
	obj = 0;
}

template<typename _Tp> inline void Ptr<_Tp>::delete_obj()
{
	if( obj ) delete obj;
}

template<typename _Tp> inline Ptr<_Tp>::~Ptr() { release(); }

template<typename _Tp> inline Ptr<_Tp>::Ptr(const Ptr<_Tp>& ptr)
{
	obj = ptr.obj;
	refcount = ptr.refcount;
	addref();
}

template<typename _Tp> inline Ptr<_Tp>& Ptr<_Tp>::operator = (const Ptr<_Tp>& ptr)
{
	int* _refcount = ptr.refcount;
	if( _refcount )
		_XADD(_refcount, 1);
	release();
	obj = ptr.obj;
	refcount = _refcount;
	return *this;
}

template<typename _Tp> inline _Tp* Ptr<_Tp>::operator -> () { return obj; }
template<typename _Tp> inline const _Tp* Ptr<_Tp>::operator -> () const { return obj; }

template<typename _Tp> inline Ptr<_Tp>::operator _Tp* () { return obj; }
template<typename _Tp> inline Ptr<_Tp>::operator const _Tp*() const { return obj; }

template<typename _Tp> inline bool Ptr<_Tp>::empty() const { return obj == 0; }

}