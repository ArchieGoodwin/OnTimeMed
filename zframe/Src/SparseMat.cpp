#include "SparseMat.h"

namespace CVLib
{

//////////////////////////////////////////////////////////////////////////
MatND::MatND() { msize.p=NULL,dims=0,data.ptr=0;}
MatND::MatND(int dims, const int* p, TYPE type)
{
	Create (dims, p, type);
}
MatND::MatND(const MatND& other)
{
	Create (other);
}
MatND::~MatND()
{
	Release();
}
MatND& MatND::operator=(const MatND& other)
{
	Release();
	int nflag=Create (other);
	assert(nflag);
	return *this;
}
int MatND::Create (const MatND& other)
{
	int nflag=Create (other.dims, other.msize.p, other.Type(), other.mstep.p);
	memcpy(data.ptr, other.data.ptr, other.Total()*CVLIB_ELEM_SIZE(Type()));
	return nflag;
}
int MatND::Create (int _dims, const int* _sz, TYPE type, const int* _steps, bool autosteps)
{
	int i;
	dims=_dims;
	this->mstep.p=new int[2*dims+1]; memset(this->mstep.p, 0, sizeof(int)*(2*dims+1));
	msize.p = (int*)(mstep.p + dims) + 1;
	msize.p[-1] = dims;
	this->type = type;
	if (!_sz)
		return 0;

	int esz = CVLIB_ELEM_SIZE(type), total = esz;
	for( i = dims-1; i >= 0; i-- )
	{
		int s = _sz[i];
		assert( s >= 0 );
		msize.p[i] = s;

		if( _steps )
			mstep.p[i] = i < dims-1 ? _steps[i] : esz;
		else if( autosteps )
		{
			mstep.p[i] = total;
			int64 total1 = (int64)total*s;
			if( (uint64)total1 != (int)total1 )
				assert( 0 );
			total = (size_t)total1;
		}
	}
	data.ptr=new BYTE[Total()*esz];
	return 1;
}
void MatND::Release()
{
	if (data.ptr)
	{
		delete []data.ptr; data.ptr=NULL;
		delete []mstep.p; mstep.p=NULL;
	}
}
int MatND::Total() const
{
	int p = 1;
	for( int i = 0; i < dims; i++ )
		p *= msize[i];
	return p;
}
void MatND::Zero()
{
	assert (data.ptr);
	memset(data.ptr, 0, CVLIB_ELEM_SIZE(Type())*Total());
}
typedef void (*convertFunc)(const uchar* _src, uchar* _dst, int len);
template<typename ST, typename DT> static void
	iConvert(const uchar* _src, uchar* _dst, int len)
{
	const ST* src=(const ST*)_src;
	DT* dst=(DT*)_dst;
	for (int i=0; i<len; i++)
		dst[i]=(DT)src[i];
}
void MatND::Convert(TYPE dstType)
{
	MatND src(*this);
	Release();
	Create (dims, src.msize.p, dstType, src.mstep.p);
	convertFunc funcs[6][6]=
	{
		{NULL,NULL,NULL,NULL,NULL,NULL},
		{NULL,iConvert<BYTE,BYTE>,iConvert<BYTE,short>,iConvert<BYTE,int>,iConvert<BYTE,float>,iConvert<BYTE,double>},
		{NULL,iConvert<short,BYTE>,iConvert<short,short>,iConvert<short,int>,iConvert<short,float>,iConvert<short,double>},
		{NULL,iConvert<int,BYTE>,iConvert<int,short>,iConvert<int,int>,iConvert<int,float>,iConvert<int,double>},
		{NULL,iConvert<float,BYTE>,iConvert<float,short>,iConvert<float,int>,iConvert<float,float>,iConvert<float,double>},
		{NULL,iConvert<double,BYTE>,iConvert<double,short>,iConvert<double,int>,iConvert<double,float>,iConvert<double,double>},
	};
	convertFunc func=funcs[src.Type()][Type()];
	func(src.data.ptr, data.ptr, Total());
}


//////////////////////////////////////////////////////////////////////////
typedef void (*normalizeFunc)(uchar* src, double dmin, double dmax, double smin, double smax, int len);
template<typename T> static void iNormalize(uchar* _src, double dmin, double dmax, double smin, double smax, int len)
{
	T* src=(T*)_src;
	double scale = (dmax-dmin)*( (smax-smin)>DBL_EPSILON ? 1.0/(smax-smin) : 1.0 );
	for (int i=0; i<len; i++)
		src[i]=(T)((src[i]-smin)*scale+dmin);
}
void MatND::Normalize(double rmin, double rmax)
{
	normalizeFunc func;
	normalizeFunc funcs[6]={NULL, iNormalize<uchar>, iNormalize<short>, iNormalize<int>, iNormalize<float>, iNormalize<double>};
	func=funcs[(int)type];
	double smin, smax;
	MinMax(smin, smax);
	func(data.ptr, rmin, rmax, smin, smax, Total());
}
typedef void (*minmaxFunc)(uchar* src, double& minVal, double& maxVal, int len);
template<typename T> static void iMinMax(uchar* _src, double& minVal, double& maxVal, int len)
{
	T* src=(T*)_src;
	for (int i=0; i<len; i++)
	{
		if (minVal>src[i])
			minVal=src[i];
		if (maxVal<src[i])
			maxVal=src[i];
	}
}
void MatND::MinMax(double& rmin, double& rmax)
{
	minmaxFunc func;
	minmaxFunc funcs[6]={NULL, iMinMax<uchar>, iMinMax<short>, iMinMax<int>, iMinMax<float>, iMinMax<double>};
	func=funcs[(int)type];
	if (type<=MAT_Tint)
		rmin=INT_MAX, rmax=-INT_MAX;
	else 
		rmin=MAX_FLOAT, rmax=-MAX_FLOAT;
	func(data.ptr, rmin, rmax, Total());
}
typedef void (*minmaxlocFunc)(uchar* src, double* minVal, double* maxVal, int* pnmin, int* pnmax, int len);
template<typename T> static void iMinMaxLoc(uchar* _src, double* minVal, double* maxVal, int* pnmin, int* pnmax, int len)
{
	int i;
	T* src=(T*)_src;
	if (minVal && pnmin)
	{
		double minv=-1;
		int imin=(int)*minVal;
		for (i=0; i<len; i++)
		{
			if (minv>src[i])
			{
				minv=src[i];
				imin=i;
			}
		}
		*minVal=minv;
		*pnmin=imin;
	}
	if (maxVal || pnmax)
	{
		double maxv=*maxVal;
		int imax=-1;
		for (i=0; i<len; i++)
		{
			if (maxv<src[i])
			{
				maxv=src[i];
				imax=i;
			}
		}
		*maxVal=maxv;
		*pnmax=imax;
	}
}
void MatND::MinMaxLoc(double* rmin, double* rmax, int* idxmin, int* idxmax)
{
	minmaxlocFunc func;
	minmaxlocFunc funcs[6]={NULL, iMinMaxLoc<uchar>, iMinMaxLoc<short>, iMinMaxLoc<int>, iMinMaxLoc<float>, iMinMaxLoc<double>};
	func=funcs[(int)type];
	double _rmin, _rmax;
	int _idxmin, _idxmax;
	if (type<=MAT_Tint)
		_rmin=INT_MAX, _rmax=-INT_MAX;
	else 
		_rmin=MAX_FLOAT, _rmax=-MAX_FLOAT;
	if (rmin || idxmin)
	{
		func(data.ptr, &_rmin, 0, &_idxmin, 0, Total());
		if (rmin) *rmin=_rmin;
		if (idxmin) *idxmin=_idxmin;
	}
	if (rmax || idxmax)
	{
		func(data.ptr, 0, &_rmax, 0, &_idxmax, Total());
		if (rmax) *rmax=_rmax;
		if (idxmax) *idxmax=_idxmax;
	}
}

inline MatND::MSize::MSize(int* _p) : p(_p) {}
inline Size MatND::MSize::operator()() const
{
	assert(p[-1] <= 2); 
	return Size(p[1], p[0]);
}
inline const int& MatND::MSize::operator[](int i) const { return p[i]; }
inline int& MatND::MSize::operator[](int i) { return p[i]; }
inline MatND::MSize::operator const int*() const { return p; }

inline bool MatND::MSize::operator == (const MSize& sz) const
{
	int d = p[-1], dsz = sz.p[-1];
	if( d != dsz )
		return false;
	if( d == 2 )
		return p[0] == sz.p[0] && p[1] == sz.p[1];

	for( int i = 0; i < d; i++ )
		if( p[i] != sz.p[i] )
			return false;
	return true;
}    

inline bool MatND::MSize::operator != (const MSize& sz) const
{
	return !(*this == sz);
}

inline MatND::MStep::MStep() { p = buf; p[0] = p[1] = 0; }
inline MatND::MStep::MStep(int s) { p = buf; p[0] = s; p[1] = 0; }
inline const int& MatND::MStep::operator[](int i) const { return p[i]; }
inline int& MatND::MStep::operator[](int i) { return p[i]; }
inline MatND::MStep::operator int() const
{
	assert( p == buf );
	return buf[0];
}
inline MatND::MStep& MatND::MStep::operator = (int s)
{
	assert( p == buf );
	buf[0] = s;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
DefMat::DefMat(int dim, int* sizes, int type)
{
	/* create sparse or ND matrix but not both */
	m_pSparseNode = NULL;
	m_pSparse = NULL;
// 	m_pND = NULL;
	m_Volume = 0;
	m_Max = 0;
	m_IDXs = NULL;
	m_Dim = 0;
	if(dim>0 && sizes != 0)
		Realloc(dim, sizes, type);
}
DefMat::~DefMat()
{
	if(m_pSparse)ReleaseSparseMat(&m_pSparse);
// 	if(m_pND) delete m_pND; m_pND=NULL;
	if(m_IDXs) delete []m_IDXs; m_IDXs=NULL;
}
void DefMat::Realloc(int dim, int* sizes, int type)
{
	if(m_pSparse)ReleaseSparseMat(&m_pSparse);
//	if(m_pND) delete m_pND; m_pND=NULL;

	if(type == BYSIZE )
	{
		int size = 0;
		int i;
		for(size=1,i=0;i<dim;++i)
		{
			size *= sizes[i];
		}
		size *= sizeof(int);
		if(size > (2<<20))
		{ /* if size > 1M */
			type = SPARSE;
		}
		else
		{
			type = ND;
		}
	}/* define matrix type */

	if(type == SPARSE)
	{
		m_pSparse = CreateSparseMat( dim, sizes, MAT_Tint );
		m_Dim = dim;
	}
// 	if(type == ND )
// 	{
// 		m_pND = new MatND( dim, sizes, MAT_Tint );
// 		m_pND->Zero();
// 		m_IDXs = new int[dim];
// 		m_Dim = dim;
// 	}
	m_Volume = 0;
	m_Max = 0;
}
void DefMat::AfterLoad()
{
	m_Volume = 0;
	m_Max = 0;
	if(m_pSparse)
	{/* calculate Volume of loaded hist */
		SparseMatIterator mat_iterator;
		SparseNode* node = InitSparseMatIterator( m_pSparse, &mat_iterator );

		for( ; node != 0; node = GetNextSparseNode( &mat_iterator ))
		{
			int val = *(int*)CV_NODE_VAL( m_pSparse, node ); /* get value of the element
															 (assume that the type is CV_32SC1) */
			m_Volume += val;
			if(m_Max < val)m_Max = val;
		}
	}/* calculate Volume of loaded hist */
// 	if(m_pND)
// 	{/* calculate Volume of loaded hist */
// 		Mat   mat;
// 		double  max_val;
// 		double  vol;
// 		cvGetMat( m_pND, &mat, NULL, 1 );
// 
// 		vol = mat.Sum();
// 		m_Volume = CVUtil::Round(vol);
// 		m_Max = CVUtil::Round(mat.Max());
// 	}/* calculate Volume of loaded hist */
}
int* DefMat::GetPtr(int* indx)
{
	if(m_pSparse) return (int*)PtrND( m_pSparse, indx, NULL, 1, NULL);
//	if(m_pND) return  (int*)PtrND( m_pND, indx, NULL, 1, NULL);
	return NULL;
}
int DefMat::GetVal(int* indx)
{
	int* p = GetPtr(indx);
	if(p)return p[0];
	return -1;
}
int DefMat::Add(int* indx, int val)
{
	int  NewVal;
	int* pVal = GetPtr(indx);
	if(pVal == NULL) return -1;
	pVal[0] += val;
	NewVal = pVal[0];
	m_Volume += val;
	if(m_Max < NewVal)m_Max = NewVal;
	return NewVal;
}
void DefMat::Add(DefMat* pMatAdd)
{
	int*    pIDXS = NULL;
	int     Val = 0;
	for(Val = pMatAdd->GetNext(&pIDXS, 1 );pIDXS;Val=pMatAdd->GetNext(&pIDXS, 0 ))
	{
		Add(pIDXS,Val);
	}
}
int DefMat::SetMax(int* indx, int val)
{
	int  NewVal;
	int* pVal = GetPtr(indx);
	if(pVal == NULL) return -1;
	if(val > pVal[0])
	{
		m_Volume += val-pVal[0];
		pVal[0] = val;
	}
	NewVal = pVal[0];
	if(m_Max < NewVal)m_Max = NewVal;
	return NewVal;
}
int DefMat::GetNext(int** pIDXS, int init)
{
	int     Val = 0;
	pIDXS[0] = NULL;
	if(m_pSparse)
	{
		m_pSparseNode = (init || m_pSparseNode==NULL)?
			InitSparseMatIterator( m_pSparse, &m_SparseIterator ):
		GetNextSparseNode( &m_SparseIterator );

		if(m_pSparseNode)
		{
			int* pVal = (int*)CV_NODE_VAL( m_pSparse, m_pSparseNode );
			if(pVal)Val = pVal[0];
			pIDXS[0] = CV_NODE_IDX( m_pSparse, m_pSparseNode );
		}
	}/* sparce matrix */
// 	if(m_pND)
// 	{
// 		int i;
// 		if(init)
// 		{
// 			for(i=0;i<m_Dim;++i)
// 			{
// 				m_IDXs[i] = cvGetDimSize( m_pND, i )-1;
// 			}
// 			pIDXS[0] = m_IDXs;
// 			Val = GetVal(m_IDXs);
// 		}
// 		else
// 		{
// 			for(i=0;i<m_Dim;++i)
// 			{
// 				if((m_IDXs[i]--)>0)
// 					break;
// 				m_IDXs[i] = cvGetDimSize( m_pND, i )-1;
// 			}
// 			if(i==m_Dim)
// 			{
// 				pIDXS[0] = NULL;
// 			}
// 			else
// 			{
// 				pIDXS[0] = m_IDXs;
// 				Val = GetVal(m_IDXs);
// 			}
// 		}/* get next ND */
// 	}/* sparce matrix */
	return Val;
}

//////////////////////////////////////////////////////////////////////////
SparseMat* CreateSparseMat( int dims, const int* sizes, int type )
{
	SparseMat* arr = 0;

	type = CV_MAT_TYPE( type );
	int pix_size1 = CVLIB_ELEM_SIZE(type);
	int pix_size = pix_size1/**CV_MAT_CN(type)*/;
	int i, size;
	MemStorage* storage;

	if( pix_size == 0 )
		return NULL;

	if( dims <= 0 || dims > CV_MAX_DIM_HEAP )
		return NULL;

	if( !sizes )
		return NULL;

	for( i = 0; i < dims; i++ )
	{
		if( sizes[i] <= 0 )
			return NULL;
	}

	arr = (SparseMat*)Alloc(sizeof(*arr)+MAX(0,dims-CVLIB_MAX_DIM)*sizeof(arr->size[0]));

	arr->type = CV_SPARSE_MAT_MAGIC_VAL | type;
	arr->dims = dims;
	arr->refcount = 0;
	arr->hdr_refcount = 1;
	memcpy( arr->size, sizes, dims*sizeof(sizes[0]));

	arr->valoffset = (int)Align(sizeof(SparseNode), pix_size1);
	arr->idxoffset = (int)Align(arr->valoffset + pix_size, sizeof(int));
	size = (int)Align(arr->idxoffset + dims*sizeof(int), sizeof(SetElem));

	storage = CreateMemStorage( CV_SPARSE_MAT_BLOCK );
	arr->heap = CreateSet( 0, sizeof(Set), size, storage );

	arr->hashsize = CV_SPARSE_HASH_SIZE0;
	size = arr->hashsize*sizeof(arr->hashtable[0]);

	arr->hashtable = (void**)Alloc( size );
	memset( arr->hashtable, 0, size );
	return arr;
}

// Creates MatND and underlying data
void ReleaseSparseMat( SparseMat** array )
{
	if( !array )
		assert(false);

	if( *array )
	{
		SparseMat* arr = *array;

		if( !CV_IS_SPARSE_MAT_HDR(arr) )
			return;

		*array = 0;

		ReleaseMemStorage( &arr->heap->storage );
		cvFree( &arr->hashtable );
		cvFree( &arr );
	}
}


// Creates MatND and underlying data
SparseMat* CloneSparseMat( const SparseMat* src )
{
	SparseMat* dst = 0;

	if( !CV_IS_SPARSE_MAT_HDR(src) )
		return NULL;

	dst = CreateSparseMat( src->dims, src->size, src->type );
//	cvCopy( src, dst ); 

	{
		SparseMatIterator iterator;
		SparseNode* node;

		dst->dims = src->dims;
		memcpy( dst->size, src->size, src->dims*sizeof(src->size[0]));
		dst->valoffset = src->valoffset;
		dst->idxoffset = src->idxoffset;
		ClearSet( dst->heap );

		if( src->heap->active_count >= dst->hashsize*CV_SPARSE_HASH_RATIO )
		{
			cvFree( &dst->hashtable );
			dst->hashsize = src->hashsize;
			dst->hashtable=(void**)Alloc( dst->hashsize*sizeof(dst->hashtable[0]));
		}

		memset( dst->hashtable, 0, dst->hashsize*sizeof(dst->hashtable[0]));

		for( node = InitSparseMatIterator( src, &iterator );
			node != 0; node = GetNextSparseNode( &iterator ))
		{
			SparseNode* node_copy = (SparseNode*)SetNew( dst->heap );
			int tabidx = node->hashval & (dst->hashsize - 1);
			CV_MEMCPY_AUTO( node_copy, node, dst->heap->elem_size );
			node_copy->next = (SparseNode*)dst->hashtable[tabidx];
			dst->hashtable[tabidx] = node_copy;
		}
	}
	return dst;
}

SparseNode* InitSparseMatIterator( const SparseMat* mat, SparseMatIterator* iterator )
{
	SparseNode* node = 0;

	int idx;

	if( !CV_IS_SPARSE_MAT( mat ))
		return NULL;

	if( !iterator )
		return NULL;

	iterator->mat = (SparseMat*)mat;
	iterator->node = 0;

	for( idx = 0; idx < mat->hashsize; idx++ )
		if( mat->hashtable[idx] )
		{
			node = iterator->node = (SparseNode*)mat->hashtable[idx];
			break;
		}

	iterator->curidx = idx;
	return node;
}

SparseNode* GetNextSparseNode( SparseMatIterator* mat_iterator )
{
	if( mat_iterator->node->next )
		return mat_iterator->node = mat_iterator->node->next;
	else
	{
		int idx;
		for( idx = ++mat_iterator->curidx; idx < mat_iterator->mat->hashsize; idx++ )
		{
			SparseNode* node = (SparseNode*)mat_iterator->mat->hashtable[idx];
			if( node )
			{
				mat_iterator->curidx = idx;
				return mat_iterator->node = node;
			}
		}
		return NULL;
	}
}

#define ICV_SPARSE_MAT_HASH_MULTIPLIER  33
#define CV_ZERO_CHAR( dst, len )                                                    \
{                                                                                   \
	size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
	char* _icv_memcpy_dst_ = (char*)(dst);                                          \
	\
	for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ )  \
	_icv_memcpy_dst_[_icv_memcpy_i_] = '\0';                                    \
}

static uchar* icvGetNodePtr( SparseMat* mat, const int* idx, int* _type,
			  int create_node, unsigned* precalc_hashval )
{
	uchar* ptr = 0;

	int i, tabidx;
	unsigned hashval = 0;
	SparseNode *node;
	assert( CV_IS_SPARSE_MAT( mat ));

	if( !precalc_hashval )
	{
		for( i = 0; i < mat->dims; i++ )
		{
			int t = idx[i];
			if( (unsigned)t >= (unsigned)mat->size[i] )
				return NULL;
			hashval = hashval*ICV_SPARSE_MAT_HASH_MULTIPLIER + t;
		}
	}
	else
	{
		hashval = *precalc_hashval;
	}

	tabidx = hashval & (mat->hashsize - 1);
	hashval &= INT_MAX;

	for( node = (SparseNode*)mat->hashtable[tabidx];
		node != 0; node = node->next )
	{
		if( node->hashval == hashval )
		{
			int* nodeidx = CV_NODE_IDX(mat,node);
			for( i = 0; i < mat->dims; i++ )
				if( idx[i] != nodeidx[i] )
					break;
			if( i == mat->dims )
			{
				ptr = (uchar*)CV_NODE_VAL(mat,node);
				break;
			}
		}
	}

	if( !ptr && create_node )
	{
		if( mat->heap->active_count >= mat->hashsize*CV_SPARSE_HASH_RATIO )
		{
			void** newtable;
			int newsize = MAX( mat->hashsize*2, CV_SPARSE_HASH_SIZE0);
			int newrawsize = newsize*sizeof(newtable[0]);

			SparseMatIterator iterator;
			assert( (newsize & (newsize - 1)) == 0 );

			// resize hash table
			newtable = (void**)Alloc( newrawsize );
			memset( newtable, 0, newrawsize );

			node = InitSparseMatIterator( mat, &iterator );
			while( node )
			{
				SparseNode* next = GetNextSparseNode( &iterator );
				int newidx = node->hashval & (newsize - 1);
				node->next = (SparseNode*)newtable[newidx];
				newtable[newidx] = node;
				node = next;
			}

			cvFree( &mat->hashtable );
			mat->hashtable = newtable;
			mat->hashsize = newsize;
			tabidx = hashval & (newsize - 1);
		}

		node = (SparseNode*)SetNew( mat->heap );
		node->hashval = hashval;
		node->next = (SparseNode*)mat->hashtable[tabidx];
		mat->hashtable[tabidx] = node;
		CV_MEMCPY_INT( CV_NODE_IDX(mat,node), idx, mat->dims );
		ptr = (uchar*)CV_NODE_VAL(mat,node);
		if( create_node > 0 )
			CV_ZERO_CHAR( ptr, CV_ELEM_SIZE(mat->type));
	}

	if( _type )
		*_type = CV_MAT_TYPE(mat->type);

	return ptr;
}


static void icvDeleteNode( SparseMat* mat, const int* idx, unsigned* precalc_hashval )
{
	int i, tabidx;
	unsigned hashval = 0;
	SparseNode *node, *prev = 0;
	assert( CV_IS_SPARSE_MAT( mat ));

	if( !precalc_hashval )
	{
		for( i = 0; i < mat->dims; i++ )
		{
			int t = idx[i];
			if( (unsigned)t >= (unsigned)mat->size[i] )
				return;
			hashval = hashval*ICV_SPARSE_MAT_HASH_MULTIPLIER + t;
		}
	}
	else
	{
		hashval = *precalc_hashval;
	}

	tabidx = hashval & (mat->hashsize - 1);
	hashval &= INT_MAX;

	for( node = (SparseNode*)mat->hashtable[tabidx];
		node != 0; prev = node, node = node->next )
	{
		if( node->hashval == hashval )
		{
			int* nodeidx = CV_NODE_IDX(mat,node);
			for( i = 0; i < mat->dims; i++ )
				if( idx[i] != nodeidx[i] )
					break;
			if( i == mat->dims )
				break;
		}
	}

	if( node )
	{
		if( prev )
			prev->next = node->next;
		else
			mat->hashtable[tabidx] = node->next;
		SetRemoveByPtr( mat->heap, node );
	}
}

uchar* PtrND( const void* arr, const int* idx, int* _type, int create_node, unsigned* precalc_hashval )
{
	uchar* ptr = 0;

	if( !idx )
		return NULL;

	if( CV_IS_SPARSE_MAT( arr ))
		ptr = icvGetNodePtr( (SparseMat*)arr, idx, _type, create_node, precalc_hashval );
// 	else if( CV_IS_MATND( arr ))
// 	{
// 		MatND* mat = (MatND*)arr;
// 		int i;
// 		ptr = mat->data.ptr;
// 
// 		for( i = 0; i < mat->Dims(); i++ )
// 		{
// 			if( (unsigned)idx[i] >= (unsigned)(mat->msize[i]) )
// 				return NULL;
// 			ptr += (size_t)idx[i]*mat->mstep[i];
// 		}
// 
// 		if( _type )
// 			*_type = CV_MAT_TYPE(mat->Type());
// 	}
// 	else if( CV_IS_MAT_HDR(arr) || CV_IS_IMAGE_HDR(arr) )
// 		ptr = Ptr2D( arr, idx[0], idx[1], _type );
// 	else
// 		return NULL;
	return ptr;
}

uchar* Ptr2D( const void* arr, int y, int x, int* _type )
{
	uchar* ptr = 0;

// 	if( CV_IS_MAT( arr ))
// 	{
// 		Mat* mat = (Mat*)arr;
// 		int type;
// 
// 		if( (unsigned)y >= (unsigned)(mat->Rows()) || (unsigned)x >= (unsigned)(mat->Cols()) )
// 			return NULL;
// 
// 		type = mat->Type();
// 		if( _type )
// 			*_type = type;
// 
// 		ptr = &mat->data.ptr[y][x*mat->Step()];
// 	}
// 	else if( CV_IS_IMAGE( arr ))
// 	{
// 		IplImage* img = (IplImage*)arr;
// 		int pix_size = (img->depth & 255) >> 3;
// 		int width, height;
// 		ptr = (uchar*)img->imageData;
// 
// 		if( img->dataOrder == 0 )
// 			pix_size *= img->nChannels;
// 
// 		if( img->roi )
// 		{
// 			width = img->roi->width;
// 			height = img->roi->height;
// 
// 			ptr += img->roi->yOffset*img->widthStep +
// 				img->roi->xOffset*pix_size;
// 
// 			if( img->dataOrder )
// 			{
// 				int coi = img->roi->coi;
// 				if( !coi )
// 					return NULL;
// 				ptr += (coi - 1)*img->imageSize;
// 			}
// 		}
// 		else
// 		{
// 			width = img->width;
// 			height = img->height;
// 		}
// 
// 		if( (unsigned)y >= (unsigned)height ||
// 			(unsigned)x >= (unsigned)width )
// 			return NULL;
// 
// 		ptr += y*img->widthStep + x*pix_size;
// 
// 		if( _type )
// 		{
// 			int type = icvIplToCvDepth(img->depth);
// 			if( type < 0 || (unsigned)(img->nChannels - 1) > 3 )
// 				return NULL;
// 
// 			*_type = CV_MAKETYPE( type, img->nChannels );
// 		}
// 	}
// 	else if( CV_IS_MATND( arr ))
// 	{
// 		MatND* mat = (MatND*)arr;
// 
// 		if( mat->Dims() != 2 || 
// 			(unsigned)y >= (unsigned)(mat->msize[0]) ||
// 			(unsigned)x >= (unsigned)(mat->msize[1]) )
// 			return NULL;
// 
// 		ptr = mat->data.ptr + (size_t)y*mat->mstep[0] + x*mat->mstep[1];
// 		if( _type )
// 			*_type = CV_MAT_TYPE(mat->Type());
// 	}
	/*else*/ if( CV_IS_SPARSE_MAT( arr ))
	{
		int idx[] = { y, x };
		ptr = icvGetNodePtr( (SparseMat*)arr, idx, _type, 1, 0 );
	}
	else
	{
		return NULL;
	}

	return ptr;
}

void ClearSet( Set* set )
{
	ClearSeq( (Sequence*)set );
	set->free_elems = 0;
	set->active_count = 0;
}

/* dst = src */
// void cvCopy( const void* srcarr, void* dstarr, const void* maskarr )
// {
// // 	int pix_size;
// // 	Mat srcstub, *src = (Mat*)srcarr;
// // 	Mat dststub, *dst = (Mat*)dstarr;
// // 	Size size;
// 
// 	if( !CV_IS_MAT(src) || !CV_IS_MAT(dst) )
// 	{
// 		if( CV_IS_SPARSE_MAT(src) && CV_IS_SPARSE_MAT(dst))
// 		{
// 			SparseMat* src1 = (SparseMat*)src;
// 			SparseMat* dst1 = (SparseMat*)dst;
// 			SparseMatIterator iterator;
// 			SparseNode* node;
// 
// 			dst1->dims = src1->dims;
// 			memcpy( dst1->size, src1->size, src1->dims*sizeof(src1->size[0]));
// 			dst1->valoffset = src1->valoffset;
// 			dst1->idxoffset = src1->idxoffset;
// 			ClearSet( dst1->heap );
// 
// 			if( src1->heap->active_count >= dst1->hashsize*CV_SPARSE_HASH_RATIO )
// 			{
// 				cvFree( &dst1->hashtable );
// 				dst1->hashsize = src1->hashsize;
// 				dst1->hashtable=(void**)Alloc( dst1->hashsize*sizeof(dst1->hashtable[0]));
// 			}
// 
// 			memset( dst1->hashtable, 0, dst1->hashsize*sizeof(dst1->hashtable[0]));
// 
// 			for( node = InitSparseMatIterator( src1, &iterator );
// 				node != 0; node = GetNextSparseNode( &iterator ))
// 			{
// 				SparseNode* node_copy = (SparseNode*)SetNew( dst1->heap );
// 				int tabidx = node->hashval & (dst1->hashsize - 1);
// 				CV_MEMCPY_AUTO( node_copy, node, dst1->heap->elem_size );
// 				node_copy->next = (SparseNode*)dst1->hashtable[tabidx];
// 				dst1->hashtable[tabidx] = node_copy;
// 			}
// 			return;
// 		}
// // 		else if( CV_IS_MATND(src) || CV_IS_MATND(dst) )
// // 		{
// // 			CvArr* arrs[] = { src, dst };
// // 			CvMatND stubs[3];
// // 			CvNArrayIterator iterator;
// // 
// // 			CV_CALL( cvInitNArrayIterator( 2, arrs, maskarr, stubs, &iterator ));
// // 			pix_size = CV_ELEM_SIZE(iterator.hdr[0]->type);
// // 
// // 			if( !maskarr )
// // 			{
// // 				iterator.size.width *= pix_size;
// // 				if( iterator.size.width <= CV_MAX_INLINE_MAT_OP_SIZE*(int)sizeof(double))
// // 				{
// // 					do
// // 					{
// // 						memcpy( iterator.ptr[1], iterator.ptr[0], iterator.size.width );
// // 					}
// // 					while( cvNextNArraySlice( &iterator ));
// // 				}
// // 				else
// // 				{
// // 					do
// // 					{
// // 						icvCopy_8u_C1R( iterator.ptr[0], CV_STUB_STEP,
// // 							iterator.ptr[1], CV_STUB_STEP, iterator.size );
// // 					}
// // 					while( cvNextNArraySlice( &iterator ));
// // 				}
// // 			}
// // 			else
// // 			{
// // 				CvCopyMaskFunc func = icvGetCopyMaskFunc( pix_size );
// // 				if( !func )
// // 					CV_ERROR( CV_StsUnsupportedFormat, "" );
// // 
// // 				do
// // 				{
// // 					func( iterator.ptr[0], CV_STUB_STEP,
// // 						iterator.ptr[1], CV_STUB_STEP,
// // 						iterator.size,
// // 						iterator.ptr[2], CV_STUB_STEP );
// // 				}
// // 				while( cvNextNArraySlice( &iterator ));
// // 			}
// // 			EXIT;
// // 		}
// // 		else
// // 		{
// // 			int coi1 = 0, coi2 = 0;
// // 			CV_CALL( src = cvGetMat( src, &srcstub, &coi1 ));
// // 			CV_CALL( dst = cvGetMat( dst, &dststub, &coi2 ));
// // 
// // 			if( coi1 )
// // 			{
// // 				CvArr* planes[] = { 0, 0, 0, 0 };
// // 
// // 				if( maskarr )
// // 					CV_ERROR( CV_StsBadArg, "COI + mask are not supported" );
// // 
// // 				planes[coi1-1] = dst;
// // 				CV_CALL( cvSplit( src, planes[0], planes[1], planes[2], planes[3] ));
// // 				EXIT;
// // 			}
// // 			else if( coi2 )
// // 			{
// // 				CvArr* planes[] = { 0, 0, 0, 0 };
// // 
// // 				if( maskarr )
// // 					CV_ERROR( CV_StsBadArg, "COI + mask are not supported" );
// // 
// // 				planes[coi2-1] = src;
// // 				CV_CALL( cvMerge( planes[0], planes[1], planes[2], planes[3], dst ));
// // 				EXIT;
// // 			}
// // 		}
// 	}
// 
// // 	if( !CV_ARE_TYPES_EQ( src, dst ))
// // 		return;
// // 
// // 	if( !CV_ARE_SIZES_EQ( src, dst ))
// // 		return;
// // 	if (!src->IsEqualMat(*dst))
// // 		return;
// // 
// // 	size = src->SizeObject();
// // 	pix_size = CV_ELEM_SIZE(src->type);
// // 
// // 	if( !maskarr )
// // 	{
// // 		int src_step = src->step, dst_step = dst->step;
// // 		size.width *= pix_size;
// // 		if( CV_IS_MAT_CONT( src->Type() & dst->Type() ) && (src_step == dst_step) && (src_step == src->width * pix_size))
// // 		{
// // 			size.width *= size.height;
// // 
// // 			if( size.width <= CV_MAX_INLINE_MAT_OP_SIZE*
// // 				CV_MAX_INLINE_MAT_OP_SIZE*(int)sizeof(double))
// // 			{
// // 				memcpy( dst->data.ptr, src->data.ptr, size.width );
// // 				return;
// // 			}
// // 
// // 			size.height = 1;
// // 			src_step = dst_step = CV_STUB_STEP;
// // 		}
// // 
// // 		icvCopy_8u_C1R( src->data.ptr, src_step, dst->data.ptr, dst_step, size );
// // 	}
// // 	else
// // 	{
// // 		CvCopyMaskFunc func = icvGetCopyMaskFunc(pix_size);
// // 		Mat maskstub, *mask = (Mat*)maskarr;
// // 		int src_step = src->step;
// // 		int dst_step = dst->step;
// // 		int mask_step;
// // 
// // 		if( !CV_IS_MAT( mask ))
// // 			mask = cvGetMat( mask, &maskstub );
// // 		if( !CV_IS_MASK_ARR( mask ))
// // 			assert(false);
// // 
// // 		if( !CV_ARE_SIZES_EQ( src, mask ))
// // 			assert(false);
// // 
// // 		mask_step = mask->step;
// // 
// // 		if( CV_IS_MAT_CONT( src->Type() & dst->Type() & mask->Type() ))
// // 		{
// // 			size.width *= size.height;
// // 			size.height = 1;
// // 			src_step = dst_step = mask_step = CV_STUB_STEP;
// // 		}
// // 
// // 		if( !func )
// // 			assert(false);
// // 
// // 		func( src->data.ptr, src_step, dst->data.ptr, dst_step,
// // 			size, mask->data.ptr, mask_step );
// // 	}
// }

SetElem* SetNew( Set* set_header )
{
	SetElem* elem = set_header->free_elems;
	if( elem )
	{
		set_header->free_elems = elem->next_free;
		elem->flags = elem->flags & ((1 << 26) - 1);
		set_header->active_count++;
	}
	else
		SetAdd( set_header, NULL, (SetElem**)&elem );
	return elem;
}
void ClearSeq( Sequence *seq )
{
	if( !seq )
		return;
	SeqPopMulti( seq, 0, seq->total );
}

void SetRemoveByPtr( Set* set_header, void* elem )
{
	SetElem* _elem = (SetElem*)elem;
	assert( _elem->flags >= 0 );
	_elem->next_free = set_header->free_elems;
	_elem->flags = (_elem->flags & ((1 << 26) - 1)) | (1 << (sizeof(int)*8-1));
	set_header->free_elems = _elem;
	set_header->active_count--;
}

}