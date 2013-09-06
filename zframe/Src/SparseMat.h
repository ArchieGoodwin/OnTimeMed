
/*!
* \file	SparseMat.h
* \ingroup cvlibvp
* \brief   SparseMat¶¦ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.  
* \author  ±®½èË§
*/
#include "cvlibbase/inc/cvlibbaseDef.h"
#include "FindContour.h"

namespace CVLib
{

	/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK    (1<<12)

	/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

	/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3


#define CVLIB_MAX_DIM 32
#define CV_MAX_DIM_HEAP       (1 << 16)

class CVLIB_DECLSPEC MatND
{
public:
	union
	{
		unsigned char*	ptr;
		short* s;
		int* i;
		float* fl;
		double* db;
	} data;

	struct CVLIB_DECLSPEC MSize
	{
		MSize(int* _p=0);
		Size operator()() const;
		const int& operator[](int i) const;
		int& operator[](int i);
		operator const int*() const;
		bool operator == (const MSize& sz) const;
		bool operator != (const MSize& sz) const;

		int* p;
	};

	struct CVLIB_DECLSPEC MStep
	{
		MStep();
		MStep(int s);
		const int& operator[](int i) const;
		int& operator[](int i);
		operator int() const;
		MStep& operator = (int s);

		int* p;
		int buf[2];
	protected:
		MStep& operator = (const MStep&);
	};

	MSize msize;
	MStep mstep;

public:
	MatND();
	MatND(int dims, const int* p, TYPE type);
	MatND(const MatND& other);
	~MatND();
	int Create (int dims, const int* p, TYPE type, const int* _steps=0, bool autosteps=false);
	int Create (const MatND& other);
	void Release();
	MatND& operator=(const MatND& other);
	inline bool IsValid () const { return data.ptr!=NULL; }
	inline TYPE Type() const {return type;}
	inline int Dims() const {return dims;}
	int Total() const;
	void Normalize(double rmin, double rmax);
	void MinMax(double& rmin, double& rmax);
	void MinMaxLoc(double* rmin, double* rmax, int* idxmin, int* idxmax);
	void Zero();
	void Convert(TYPE dstType);
protected:
	TYPE type;
	int dims;
};

#define CV_SPARSE_MAT_MAGIC_VAL    0x42440000

struct CVLIB_DECLSPEC SparseMat
{
public:
	int type;
	int dims;
	int* refcount;
	int hdr_refcount;

	struct Set* heap;
	void** hashtable;
	int hashsize;
	int valoffset;
	int idxoffset;
	int size[CVLIB_MAX_DIM];
};

#define SPARSE  0
#define ND      1
#define BYSIZE  -1

struct CVLIB_DECLSPEC SparseNode
{
	unsigned hashval;
	struct SparseNode* next;
};
struct CVLIB_DECLSPEC SparseMatIterator
{
	SparseMat* mat;
	SparseNode* node;
	int curidx;
};

#define CV_IS_SPARSE_MAT_HDR(mat) \
	((mat) != NULL && \
	(((const SparseMat*)(mat))->type & CV_MAGIC_MASK) == CV_SPARSE_MAT_MAGIC_VAL)

#define CV_IS_SPARSE_MAT(mat) \
	CV_IS_SPARSE_MAT_HDR(mat)

#define CV_NODE_VAL(mat,node)   ((void*)((uchar*)(node) + (mat)->valoffset))
#define CV_NODE_IDX(mat,node)   ((int*)((uchar*)(node) + (mat)->idxoffset))


class CVLIB_DECLSPEC DefMat
{
private:
	SparseMatIterator m_SparseIterator;
	SparseNode*       m_pSparseNode;
	int*                m_IDXs;
	int                 m_Dim;

public:
	SparseMat*        m_pSparse;
//	MatND*				m_pND;
	int                 m_Volume;
	int                 m_Max;

	DefMat(int dim = 0, int* sizes = NULL, int type = SPARSE);
	~DefMat();
	void Realloc(int dim, int* sizes, int type = SPARSE);
	void AfterLoad();
	int* GetPtr(int* indx);
	int GetVal(int* indx);
	int Add(int* indx, int val);
	void Add(DefMat* pMatAdd);
	int SetMax(int* indx, int val);
	int GetNext(int** pIDXS, int init = 0);
};

CVLIB_DECLSPEC SparseMat* CreateSparseMat( int dims, const int* sizes, int type );
CVLIB_DECLSPEC void ReleaseSparseMat( SparseMat** array );
CVLIB_DECLSPEC SparseMat* CloneSparseMat( const SparseMat* src );

CVLIB_DECLSPEC SparseNode* InitSparseMatIterator( const SparseMat* mat, SparseMatIterator* iterator );
CVLIB_DECLSPEC SparseNode* GetNextSparseNode( SparseMatIterator* mat_iterator );
CVLIB_DECLSPEC uchar* PtrND( const void* arr, const int* idx, int* _type, int create_node, unsigned* precalc_hashval );
CVLIB_DECLSPEC void cvCopy( const void* srcarr, void* dstarr, const void* maskarr=NULL );

CVLIB_DECLSPEC uchar* Ptr2D( const void* arr, int y, int x, int* _type );
CVLIB_DECLSPEC void ClearSet( Set* set );
CVLIB_DECLSPEC void SetRemoveByPtr( Set* set_header, void* elem );
CVLIB_DECLSPEC SetElem* SetNew( Set* set_header );
CVLIB_DECLSPEC void ClearSeq( Sequence *seq );

}