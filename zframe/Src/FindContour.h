
/*!
 * \file
 * \brief .
 * \author Jinhyok Sin
 */


#ifndef _FINDCONTOUR_H_
#define _FINDCONTOUR_H_

#include "ImageList.h"

namespace CVLib
{

#define CV_MAGIC_MASK 0xFFFF0000

/******************************Mat Type****************************/

#define CV_MAT_CN_MASK          ((CV_CN_MAX - 1) << CV_CN_SHIFT)
#define CV_MAT_CN(flags)        ((((flags) & CV_MAT_CN_MASK) >> CV_CN_SHIFT) + 1)
#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)
#define CV_MAT_TYPE_MASK        (CV_DEPTH_MAX*CV_CN_MAX - 1)
#define CV_MAT_TYPE(flags)      ((flags) & CV_MAT_TYPE_MASK)
#define CV_MAT_CONT_FLAG_SHIFT  14
#define CV_MAT_CONT_FLAG        (1 << CV_MAT_CONT_FLAG_SHIFT)
#define CV_IS_MAT_CONT(flags)   ((flags) & CV_MAT_CONT_FLAG)
#define CV_IS_CONT_MAT          CV_IS_MAT_CONT
#define CV_MAT_TEMP_FLAG_SHIFT  15
#define CV_MAT_TEMP_FLAG        (1 << CV_MAT_TEMP_FLAG_SHIFT)
#define CV_IS_TEMP_MAT(flags)   ((flags) & CV_MAT_TEMP_FLAG)

#define CV_MAGIC_MASK       0xFFFF0000
#define CV_MAT_MAGIC_VAL    0x42420000


#define CV_ELEM_SIZE(type) \
    (CV_MAT_CN(type) << ((((sizeof(int)/4+1)*16384|0x3a50) >> CV_MAT_DEPTH(type)*2) & 3))

/* default alignment for dynamic data strucutures, resided in storages. */
#define  CV_STRUCT_ALIGN    ((int)sizeof(double))
/****************************************************************************************\
*                                    Sequence types                                      *
\****************************************************************************************/

#define CV_32FC1 CV_MAKETYPE(CV_32F,1)
#define CV_32FC2 CV_MAKETYPE(CV_32F,2)
#define CV_32FC3 CV_MAKETYPE(CV_32F,3)
#define CV_32FC4 CV_MAKETYPE(CV_32F,4)
#define CV_32FC(n) CV_MAKETYPE(CV_32F,(n))

#define CV_32SC1 CV_MAKETYPE(CV_32S,1)
#define CV_32SC2 CV_MAKETYPE(CV_32S,2)
#define CV_32SC3 CV_MAKETYPE(CV_32S,3)
#define CV_32SC4 CV_MAKETYPE(CV_32S,4)
#define CV_32SC(n) CV_MAKETYPE(CV_32S,(n))

#define CV_8SC1 CV_MAKETYPE(CV_8S,1)
#define CV_8SC2 CV_MAKETYPE(CV_8S,2)
#define CV_8SC3 CV_MAKETYPE(CV_8S,3)
#define CV_8SC4 CV_MAKETYPE(CV_8S,4)
#define CV_8SC(n) CV_MAKETYPE(CV_8S,(n))

/* IEEE754 constants and macros */
#define  CV_POS_INF       0x7f800000
#define  CV_NEG_INF       0x807fffff /* CV_TOGGLE_FLT(0xff800000) */
#define  CV_1F            0x3f800000
#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define  CV_TOGGLE_DBL(x) \
    ((x)^((int64)(x) < 0 ? CV_BIG_INT(0x7fffffffffffffff) : 0))

#define CV_SEQ_MAGIC_VAL             0x42990000

#define CV_IS_SEQ(seq) \
((seq) != NULL && (((Sequence*)(seq))->flags & CV_MAGIC_MASK) == CV_SEQ_MAGIC_VAL)

#define CV_SET_MAGIC_VAL             0x42980000
#define CV_IS_SET(set) \
((set) != NULL && (((Sequence*)(set))->flags & CV_MAGIC_MASK) == CV_SET_MAGIC_VAL)

#define CV_SEQ_ELTYPE_BITS           9
#define CV_SEQ_ELTYPE_MASK           ((1 << CV_SEQ_ELTYPE_BITS) - 1)

#define CV_SEQ_ELTYPE_POINT          CV_32SC2  /* (x,y) */
#define CV_SEQ_ELTYPE_CODE           CV_8UC1   /* freeman code: 0..7 */
#define CV_SEQ_ELTYPE_GENERIC        0
#define CV_SEQ_ELTYPE_PTR            CV_USRTYPE1
#define CV_SEQ_ELTYPE_PPOINT         CV_SEQ_ELTYPE_PTR  /* &(x,y) */
#define CV_SEQ_ELTYPE_INDEX          CV_32SC1  /* #(x,y) */
#define CV_SEQ_ELTYPE_GRAPH_EDGE     0  /* &next_o, &next_d, &vtx_o, &vtx_d */
#define CV_SEQ_ELTYPE_GRAPH_VERTEX   0  /* first_edge, &(x,y) */
#define CV_SEQ_ELTYPE_TRIAN_ATR      0  /* vertex of the binary tree   */
#define CV_SEQ_ELTYPE_CONNECTED_COMP 0  /* connected component  */
#define CV_SEQ_ELTYPE_POINT3D        CV_32FC3  /* (x,y,z)  */

#define CV_SEQ_KIND_BITS        3
#define CV_SEQ_KIND_MASK        (((1 << CV_SEQ_KIND_BITS) - 1)<<CV_SEQ_ELTYPE_BITS)
/* contour retrieval mode */
#define CV_RETR_EXTERNAL 0
#define CV_RETR_LIST     1
#define CV_RETR_CCOMP    2
#define CV_RETR_TREE     3

/* contour approximation method */
#define CV_CHAIN_CODE               0
#define CV_CHAIN_APPROX_NONE        1
#define CV_CHAIN_APPROX_SIMPLE      2
#define CV_CHAIN_APPROX_TC89_L1     3
#define CV_CHAIN_APPROX_TC89_KCOS   4
#define CV_LINK_RUNS                5

/* types of sequences */
#define CV_SEQ_KIND_GENERIC     (0 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_CURVE       (1 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_BIN_TREE    (2 << CV_SEQ_ELTYPE_BITS)

#define CV_CN_MAX     64
#define CV_CN_SHIFT   3
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)

#define CV_8U   0
#define CV_8S   1
#define CV_16U  2
#define CV_16S  3
#define CV_32S  4
#define CV_32F  5
#define CV_64F  6
#define CV_USRTYPE1 7

#define CV_MAKETYPE(depth,cn) ((depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE
#define CV_8UC1 CV_MAKETYPE(CV_8U,1)
#define CV_SEQ_ELTYPE_CODE           CV_8UC1   /* freeman code: 0..7 */

/* chain-coded curves */
#define CV_SEQ_CHAIN           (CV_SEQ_KIND_CURVE  | CV_SEQ_ELTYPE_CODE)
#define CV_SEQ_CHAIN_CONTOUR   (CV_SEQ_FLAG_CLOSED | CV_SEQ_CHAIN)

/* point sets */
#define CV_SEQ_POINT_SET       (CV_SEQ_KIND_GENERIC| CV_SEQ_ELTYPE_POINT)
#define CV_SEQ_POINT3D_SET     (CV_SEQ_KIND_GENERIC| CV_SEQ_ELTYPE_POINT3D)
#define CV_SEQ_POLYLINE        (CV_SEQ_KIND_CURVE  | CV_SEQ_ELTYPE_POINT)
#define CV_SEQ_POLYGON         (CV_SEQ_FLAG_CLOSED | CV_SEQ_POLYLINE )
#define CV_SEQ_CONTOUR         CV_SEQ_POLYGON
#define CV_SEQ_SIMPLE_POLYGON  (CV_SEQ_FLAG_SIMPLE | CV_SEQ_POLYGON  )

#define CV_SEQ_KIND_BITS			3
#define CV_SEQ_ELTYPE_BITS			9

#define CV_SEQ_FLAG_SHIFT       (CV_SEQ_KIND_BITS + CV_SEQ_ELTYPE_BITS)

/* flags for curves */
#define CV_SEQ_FLAG_CLOSED     (1 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_SIMPLE     (2 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_CONVEX     (4 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_HOLE       (8 << CV_SEQ_FLAG_SHIFT)

/* Types of thresholding */
// #define CV_THRESH_BINARY      0  /* value = value > threshold ? max_value : 0       */
// #define CV_THRESH_BINARY_INV  1  /* value = value > threshold ? 0 : max_value       */
// #define CV_THRESH_TRUNC       2  /* value = value > threshold ? threshold : value   */
// #define CV_THRESH_TOZERO      3  /* value = value > threshold ? value : 0           */
// #define CV_THRESH_TOZERO_INV  4  /* value = value > threshold ? 0 : value           */
// #define CV_THRESH_MASK        7
// 
// #define CV_THRESH_OTSU        8  /* use Otsu algorithm to choose the optimal threshold value;
//                                     combine the flag with one of the above CV_THRESH_* values */
/* sequence of the integer numbers */
#define CV_SEQ_INDEX           (CV_SEQ_KIND_GENERIC  | CV_SEQ_ELTYPE_INDEX)

#define CV_SEQ_ELTYPE( seq )   ((seq)->flags & CV_SEQ_ELTYPE_MASK)
#define CV_SEQ_KIND( seq )     ((seq)->flags & CV_SEQ_KIND_MASK )

/* flag checking */
#define CV_IS_SEQ_INDEX( seq )      ((CV_SEQ_ELTYPE(seq) == CV_SEQ_ELTYPE_INDEX) && \
(CV_SEQ_KIND(seq) == CV_SEQ_KIND_GENERIC))

#define CV_IS_SEQ_CURVE( seq )      (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE)
#define CV_IS_SEQ_CLOSED( seq )     (((seq)->flags & CV_SEQ_FLAG_CLOSED) != 0)
#define CV_IS_SEQ_CONVEX( seq )     (((seq)->flags & CV_SEQ_FLAG_CONVEX) != 0)
#define CV_IS_SEQ_HOLE( seq )       (((seq)->flags & CV_SEQ_FLAG_HOLE) != 0)
#define CV_IS_SEQ_SIMPLE( seq )     ((((seq)->flags & CV_SEQ_FLAG_SIMPLE) != 0) || \
                                    CV_IS_SEQ_CONVEX(seq))

#define CV_MEMCPY_AUTO( dst, src, len )                                             \
{                                                                                   \
    int _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    char* _icv_memcpy_dst_ = (char*)(dst);                                          \
    const char* _icv_memcpy_src_ = (const char*)(src);                              \
    if( (_icv_memcpy_len_ & (sizeof(int)-1)) == 0 )                                 \
    {                                                                               \
	assert( ((int)_icv_memcpy_src_&(sizeof(int)-1)) == 0 &&                  \
	((int)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 );                  \
	for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_;                 \
	_icv_memcpy_i_+=sizeof(int) )                                           \
        {                                                                           \
		*(int*)(_icv_memcpy_dst_+_icv_memcpy_i_) =                              \
		*(const int*)(_icv_memcpy_src_+_icv_memcpy_i_);                         \
        }                                                                           \
    }                                                                               \
    else                                                                            \
    {                                                                               \
	for(_icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++)\
	_icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_];    \
    }                                                                               \
}

/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CV_INIT_3X3_DELTAS( deltas, step, nch )            \
    ((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch),  \
	(deltas)[2] = -(step), (deltas)[3] = -(step) - (nch),	\
	(deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch),	\
	(deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))

/******************************** Memory storage ****************************************/

struct CVLIB_DECLSPEC MemBlock
{
    struct MemBlock*  prev;
    struct MemBlock*  next;
};

#define CV_STORAGE_MAGIC_VAL    0x42890000

struct CVLIB_DECLSPEC MemStorage
{
    int signature;
    MemBlock* bottom;/* first allocated block */
    MemBlock* top;   /* current memory block - top of the stack */
    MemStorage* parent; /* borrows new blocks from */
    int block_size;  /* block size */
    int free_space;  /* free space in the current block */
};

#define CV_IS_STORAGE(storage)  \
    ((storage) != NULL &&       \
(((MemStorage*)(storage))->signature & CV_MAGIC_MASK) == CV_STORAGE_MAGIC_VAL)


struct CVLIB_DECLSPEC MemStoragePos
{
    MemBlock* top;
    int free_space;
};

/*********************************** Sequence *******************************************/

struct CVLIB_DECLSPEC SeqBlock
{
    struct SeqBlock*  prev;		/* previous sequence block */
    struct SeqBlock*  next;		/* next sequence block */
	int    start_index;			/* index of the first element in the block +
								sequence->first->start_index */
	int    count;				/* number of elements in the block */
	char*  data;				/* pointer to the first element of the block */
};


#define CV_TREE_NODE_FIELDS(node_type)                          \
    int       flags;         /* micsellaneous flags */          \
    int       header_size;   /* size of sequence header */      \
    struct    node_type* h_prev; /* previous sequence */        \
    struct    node_type* h_next; /* next sequence */            \
    struct    node_type* v_prev; /* 2nd previous sequence */    \
	struct    node_type* v_next;  /* 2nd next sequence */

/*
Read/Write sequence.
Elements can be dynamically inserted to or deleted from the sequence.
*/
#define CV_SEQUENCE_FIELDS()														\
    CV_TREE_NODE_FIELDS(Sequence)													\
    int       total;          /* total number of elements */						\
    int       elem_size;      /* size of sequence element in bytes */				\
    char*     block_max;      /* maximal bound of the last block */					\
    char*     ptr;            /* current write pointer */							\
    int       delta_elems;    /* how many elements allocated when the seq grows */  \
	MemStorage* storage;    /* where the seq is stored */							\
    SeqBlock* free_blocks;  /* free blocks list */									\
    SeqBlock* first; /* pointer to the first sequence block */

struct CVLIB_DECLSPEC Sequence
{
	Sequence(){};
	CV_SEQUENCE_FIELDS()
};
// class CVLIB_DECLSPEC Sequence
// {
// public:
// 	Sequence(){}
// 
// 	int       flags;         /* micsellaneous flags */
// 	int       header_size;   /* size of sequence header */
// 	Sequence* h_prev; /* previous sequence */
// 	Sequence* h_next; /* next sequence */
// 	Sequence* v_prev; /* 2nd previous sequence */
// 	Sequence* v_next;  /* 2nd next sequence */
// 	int       total;          /* total number of elements */
// 	int       elem_size;      /* size of sequence element in bytes */
// 	char*     block_max;      /* maximal bound of the last block */
// 	char*     ptr;            /* current write pointer */
// 	int       delta_elems;    /* how many elements allocated when the seq grows */
// 	MemStorage* storage;    /* where the seq is stored */
// 	SeqBlock* free_blocks;  /* free blocks list */
// 	SeqBlock* first; /* pointer to the first sequence block */
// };

struct CVLIB_DECLSPEC Chain
{
	CV_SEQUENCE_FIELDS()
    SPoint  origin;
};

#define CV_CONTOUR_FIELDS()  \
    CV_SEQUENCE_FIELDS()     \
    Rect rect;				 \
    int color;               \
int reserved[3];

struct CVLIB_DECLSPEC Contour
{
    CV_CONTOUR_FIELDS()
};

typedef Contour Point2DSeq;

/****************************************************************************************/
/*                            Sequence writer & reader                                  */
/****************************************************************************************/

#define SEQ_WRITER_FIELDS()											\
    int          header_size;										\
    Sequence*       seq;        /* the sequence written */			\
    SeqBlock*  block;      /* current block */						\
    char*        ptr;        /* pointer to free space */			\
    char*        block_min;  /* pointer to the beginning of block*/	\
char*        block_max;  /* pointer to the end of block */

struct CVLIB_DECLSPEC SeqWriter
{
    SEQ_WRITER_FIELDS()
};


#define SEQ_READER_FIELDS()											\
    int          header_size;                                       \
    Sequence*       seq;        /* sequence, beign read */          \
    SeqBlock*  block;      /* current block */						\
    char*        ptr;        /* pointer to element be read next */  \
    char*        block_min;  /* pointer to the beginning of block */\
    char*        block_max;  /* pointer to the end of block */      \
    int          delta_index;/* = seq->first->start_index   */      \
	char*        prev_elem;  /* pointer to previous element */


struct CVLIB_DECLSPEC SeqReader
{
    SEQ_READER_FIELDS()
};

/*************************************** Set ********************************************/
/*
Set.
Order is not preserved. There can be gaps between sequence elements.
After the element has been inserted it stays in the same place all the time.
The MSB(most-significant or sign bit) of the first field (flags) is 0 iff the element exists.
*/
#define CV_SET_ELEM_FIELDS(elem_type)   \
    int  flags;                         \
struct elem_type* next_free;

struct CVLIB_DECLSPEC SetElem
{
    CV_SET_ELEM_FIELDS(SetElem)
};

#define CV_SET_FIELDS()     \
    CV_SEQUENCE_FIELDS()    \
    SetElem* free_elems;	\
	int active_count;

struct CVLIB_DECLSPEC Set
{
    CV_SET_FIELDS()
};


#define CV_SET_ELEM_IDX_MASK   ((1 << 26) - 1)
#define CV_SET_ELEM_FREE_FLAG  (1 << (sizeof(int)*8-1))

/* Checks whether the element pointed by ptr belongs to a set or not */
#define CV_IS_SET_ELEM( ptr )  (((SetElem*)(ptr))->flags >= 0)


/********************************Contour Infomations****************************************/
struct CVLIB_DECLSPEC ContourInfo 
{
	int flags;
    ContourInfo *next;        /* next contour with the same mark value */
    ContourInfo *parent;      /* information about parent contour */
    Sequence *contour;             /* corresponding contour (may be 0, if rejected) */
    Rect rect;                /* bounding rectangle */
    SPoint origin;             /* origin point (where the contour was traced from) */
    int is_hole;                /* hole flag */
};

struct CVLIB_DECLSPEC _ContourScanner
{    
	MemStorage *storage1;     /* contains fetched contours */
	MemStorage *storage2;     /* contains approximated contours
	(!=storage1 if approx_method2 != approx_method1) */
	MemStorage *cinfo_storage;        /* contains _CvContourInfo nodes */
	Set *cinfo_set;           /* set of _CvContourInfo nodes */
	MemStoragePos initial_pos;        /* starting storage pos */
	MemStoragePos backup_pos; /* beginning of the latest approx. contour */
	MemStoragePos backup_pos2;        /* ending of the latest approx. contour */
	char *img0;                 /* image origin */
	char *img;                  /* current image row */
	int img_step;               /* image step */
	Size img_size;            /* ROI size */
	SPoint offset;             /* ROI offset: coordinates, added to each contour point */
	SPoint pt;                 /* current scanner position */
	SPoint lnbd;               /* position of the last met contour */
	int nbd;                    /* current mark val */
	ContourInfo *l_cinfo;    /* information about latest approx. contour */
	ContourInfo cinfo_temp;  /* temporary var which is used in simple modes */
	ContourInfo frame_info;  /* information about frame */
	Sequence frame;                /* frame itself */
	int approx_method1;         /* approx method when tracing */
	int approx_method2;         /* final approx method */
	int mode;                   /* contour scanning mode:
								0 - external only
								1 - all the contours w/o any hierarchy
								2 - connected components (i.e. two-level structure -
	external contours and holes) */
	int subst_flag;
	int seq_type1;              /* type of fetched contours */
	int header_size1;           /* hdr size of fetched contours */
	int elem_size1;             /* elem size of fetched contours */
	int seq_type2;              /*                                       */
	int header_size2;           /*        the same for approx. contours  */
	int elem_size2;             /*                                       */
    ContourInfo *cinfo_table[126];	
};

typedef struct _ContourScanner* ContourScanner;
typedef void Arr;

typedef union Cv32suf
{
    int i;
    unsigned u;
    float f;
} Cv32suf;

struct CVLIB_DECLSPEC TreeNode
{
    int       flags;         /* micsellaneous flags */
    int       header_size;   /* size of sequence header */
    struct    TreeNode* h_prev; /* previous sequence */
    struct    TreeNode* h_next; /* next sequence */
    struct    TreeNode* v_prev; /* 2nd previous sequence */
    struct    TreeNode* v_next; /* 2nd next sequence */
};

#define _CV_FIND_CONTOURS_FLAGS_EXTERNAL_ONLY    1
#define _CV_FIND_CONTOURS_FLAGS_HIERARCHIC       2

/****************************************************************************************\
*                                  Chain Approximation                                   *
\****************************************************************************************/

struct CVLIB_DECLSPEC PtInfo
{
    SPoint pt;
    int k;                      /* support region */
    int s;                      /* curvature value */
    struct PtInfo *next;
};

/* Freeman chain reader state */

#define CV_SEQ_READER_FIELDS()                                      \
    int          header_size;                                       \
    Sequence*       seq;        /* sequence, beign read */             \
    SeqBlock*  block;      /* current block */                    \
    char*        ptr;        /* pointer to element be read next */  \
    char*        block_min;  /* pointer to the beginning of block */\
    char*        block_max;  /* pointer to the end of block */      \
    int          delta_index;/* = seq->first->start_index   */      \
    char*        prev_elem;  /* pointer to previous element */

struct CVLIB_DECLSPEC ChainPtReader
{
    CV_SEQ_READER_FIELDS()
	char      code;
    SPoint   pt;
    char      deltas[8][2];
};

#define CV_IS_SEQ_CHAIN( seq )   \
    (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE && (seq)->elem_size == 1)

#define CV_IS_SEQ_CHAIN_CONTOUR( seq ) \
    (CV_IS_SEQ_CHAIN( seq ) && CV_IS_SEQ_CLOSED( seq ))

//////////////////////////////////////////////////////////////////////////

/************************************* CvSlice ******************************************/

#define CV_WHOLE_SEQ_END_INDEX 0x3fffffff
#define CV_WHOLE_SEQ  Slice(0, CV_WHOLE_SEQ_END_INDEX)



// #if defined WIN32 || defined WIN64
// #define CV_CDECL __cdecl
// #define CV_STDCALL __stdcall
// #else
// #define CV_CDECL
// #define CV_STDCALL
// #endif

#define  CV_MALLOC_ALIGN    32

#define ICV_FREE_PTR(storage)  \
    ((char*)(storage)->top + (storage)->block_size - (storage)->free_space)

#define ICV_ALIGNED_SEQ_BLOCK_SIZE  \
    (int)Align(sizeof(SeqBlock), CV_STRUCT_ALIGN)

#define CV_WRITE_SEQ_ELEM( elem, writer )             \
{                                                     \
    assert( (writer).seq->elem_size == sizeof(elem)); \
    if( (writer).ptr >= (writer).block_max )          \
    {                                                 \
		CreateSeqBlock( &writer);                   \
    }                                                 \
    assert( (writer).ptr <= (writer).block_max - sizeof(elem));\
    memcpy((writer).ptr, &(elem), sizeof(elem));      \
    (writer).ptr += sizeof(elem);                     \
}

/* move reader position forward */
#define CV_NEXT_SEQ_ELEM( elem_size, reader )                 \
{                                                             \
    if( ((reader).ptr += (elem_size)) >= (reader).block_max ) \
    {                                                         \
		ChangeSeqBlock( &(reader), 1 );                     \
    }                                                         \
}

/* read element and move read position forward */
#define CV_READ_SEQ_ELEM( elem, reader )                       \
{                                                              \
    assert( (reader).seq->elem_size == sizeof(elem));          \
    memcpy( &(elem), (reader).ptr, sizeof((elem)));            \
    CV_NEXT_SEQ_ELEM( sizeof(elem), reader )                   \
}

#define CV_READ_CHAIN_POINT( _pt, reader )                              \
{                                                                       \
    (_pt) = (reader).pt;                                                \
    if( (reader).ptr )                                                  \
    {                                                                   \
	CV_READ_SEQ_ELEM( (reader).code, (reader));                     \
	assert( ((reader).code & ~7) == 0 );                            \
	(reader).pt.x += (reader).deltas[(int)(reader).code][0];        \
	(reader).pt.y += (reader).deltas[(int)(reader).code][1];        \
    }                                                               \
}

/* maximum size of dynamic memory buffer.
cvAlloc reports an error if a larger block is requested. */
#define  CV_MAX_ALLOC_SIZE    (((int)1 << (sizeof(int)*8-2)))

#define CV_MEMCPY_INT( dst, src, len )                                              \
{                                                                                   \
    int _icv_memcpy_i_, _icv_memcpy_len_ = (len);                                \
    int* _icv_memcpy_dst_ = (int*)(dst);                                            \
    const int* _icv_memcpy_src_ = (const int*)(src);                                \
    assert( ((int)_icv_memcpy_src_&(sizeof(int)-1)) == 0 &&                      \
	((int)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 );                      \
	\
    for(_icv_memcpy_i_=0;_icv_memcpy_i_<_icv_memcpy_len_;_icv_memcpy_i_++)          \
	_icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_];        \
}

#define CV_GET_LAST_ELEM( seq, block ) \
    ((block)->data + ((block)->count - 1)*((seq)->elem_size))

/* default storage block size */
#define  CV_STORAGE_BLOCK_SIZE   ((1<<16) - 128)

/* type checking macros */
#define CV_IS_SEQ_POINT_SET( seq ) \
    ((CV_SEQ_ELTYPE(seq) == CV_32SC2 || CV_SEQ_ELTYPE(seq) == CV_32FC2))

#define CV_POLY_APPROX_DP 0

#define cvContourPerimeter( contour ) ArcLength( contour, CV_WHOLE_SEQ, 1 )

#define CV_IS_SEQ_POLYLINE( seq )   \
    (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE && CV_IS_SEQ_POINT_SET(seq))

#define CV_IS_SEQ_POLYGON( seq )   \
    (CV_IS_SEQ_POLYLINE(seq) && CV_IS_SEQ_CLOSED(seq))

#define CV_IS_MAT_HDR(mat) \
    ((mat) != NULL && \
    (CV_MAGIC_MASK) == CV_MAT_MAGIC_VAL && \
    ((const Mat*)(mat))->Cols() > 0 && ((const Mat*)(mat))->Rows() > 0)

#define CV_IS_MAT(mat) \
    (CV_IS_MAT_HDR(mat) && ((const Mat*)(mat))->data.ptr != NULL)

#define ICV_SHIFT_TAB_MAX 32
static const char icvPower2ShiftTab[] =
{
    0, 1, -1, 2, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, 4,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5
};

//////////////////////////////////////////////////////////////////////////

inline int Align( int size, int align )
{
    assert( (align & (align-1)) == 0 && size < INT_MAX );
    return (size + align - 1) & -align;
}

inline int AlignLeft( int size, int align )
{
    return size & -align;
}

inline void* AlignPtr( const void* ptr, int align=32 )
{
    assert( (align & (align-1)) == 0 );
    return (void*)( ((unsigned int)ptr + align - 1) & ~(int)(align-1) );
}

// default <free>
inline static int DefaultFree( void* ptr, void* )
{
    // Pointer must be aligned by CV_MALLOC_ALIGN
    if( ((unsigned int)ptr & (CV_MALLOC_ALIGN-1)) != 0 )
        return -49;
    free( *((char**)ptr - 1) );
	
    return 0;
}
inline static void* DefaultAlloc( int size, void* )
{
    char *ptr, *ptr0 = (char*)malloc(
        (int)(size + CV_MALLOC_ALIGN*((size >= 4096) + 1) + sizeof(char*)));
	
    if( !ptr0 )
        return 0;
	
    // align the pointer
    ptr = (char*)AlignPtr(ptr0 + sizeof(char*) + 1, CV_MALLOC_ALIGN);
    *(char**)(ptr - sizeof(char*)) = ptr0;
	
    return ptr;
}

#define cvFree(ptr) (cvFree_(*(ptr)), *(ptr)=0)

// pointers to allocation functions, initially set to default
typedef void* (/*CV_CDECL*/ *AllocFunc)(int size, void* userdata);
typedef int (/*CV_CDECL*/ *FreeFunc)(void* pptr, void* userdata);

static AllocFunc p_cvAlloc = DefaultAlloc;
static FreeFunc p_cvFree = DefaultFree;
static void* p_cvAllocUserData = 0;

inline bool cvFree_( void* ptr )
{	
    if( ptr )
    {
        int status = p_cvFree( ptr, p_cvAllocUserData );
        if( status < 0 )
            return false ;
    }
	return true ;
}

inline double Angle( SPoint* pt1, SPoint* pt2, SPoint* pt0 )
{
    double dx1 = pt1->x - pt0->x;
    double dy1 = pt1->y - pt0->y;
    double dx2 = pt2->x - pt0->x;
    double dy2 = pt2->y - pt0->y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

//////////////////////////////////////////////////////////////////////////

CVLIB_DECLSPEC void FindSquares4( CoImage* img, Matrix<Point2i>& squares, float t_angle, bool bMultiple = FALSE, ImageDescPtrList* pImageList=NULL) ;
CVLIB_DECLSPEC void FindSquares4( Mat* pmSrc, Matrix<Point2i>& squares, float t_angle, bool bMultiple = FALSE) ;

CVLIB_DECLSPEC int FindContours( Mat* img, MemStorage* storage, Sequence** firstContour, int mode=CV_RETR_LIST, 
				 int method=CV_CHAIN_APPROX_SIMPLE, Point2i offset=Point2i() ) ;
CVLIB_DECLSPEC int FindContours( Mat* img, Matrix<Point2i>& contours, int mode=CV_RETR_LIST, 
								int method=CV_CHAIN_APPROX_SIMPLE, Point2i offset=Point2i() ) ;
CVLIB_DECLSPEC void FindSquaresFromBin( Mat* img, Matrix<Point2i>& contours ) ;

CVLIB_DECLSPEC ContourScanner StartFindContours(Mat* pmImg, MemStorage* storage, int nRetrievalMode, int nApproximageMethod, Point2i startPt);

CVLIB_DECLSPEC Sequence* FindNextContour( ContourScanner scanner ) ;

CVLIB_DECLSPEC int TraceContour( char *ptr, int step, char *stop_ptr, int is_hole ) ;

CVLIB_DECLSPEC void EndProcessContour( ContourScanner scanner ) ;

CVLIB_DECLSPEC int FetchContour( char *ptr, int step, Point2i pt, Sequence* contour, int _method ) ;

CVLIB_DECLSPEC int FetchContourEx( char* ptr, int step, Point2i pt, Sequence* contour, int  _method,  int nbd, Rect* _rect ) ;

CVLIB_DECLSPEC int ApproximateChainTC89( Chain* chain, int header_size, MemStorage* storage, 
                         Sequence** contour, int method ) ;

CVLIB_DECLSPEC Sequence* EndFindContours( ContourScanner *_scanner ) ;

CVLIB_DECLSPEC void Threshold_Contour( Mat* src, Mat* dst, double thresh, double maxval ) ;

CVLIB_DECLSPEC void EndSquares( Mat* graySrc, Matrix<Point2i>& squares, int nScale, bool bMulti = FALSE ) ;

CVLIB_DECLSPEC float GetEdgeValue( Mat* pmEdge, Vector<Point2i>& rect) ;

CVLIB_DECLSPEC Sequence* TestContour( Sequence* contours, MemStorage* storage, int method, Sequence* rst ) ;

CVLIB_DECLSPEC Sequence* ApproxPoly( const void*  array, int  header_size, MemStorage*  storage, int  method, double  parameter, int parameter2 ) ;

/* the version for integer point coordinates */
CVLIB_DECLSPEC int iApproxPolyDP_32s( Sequence* src_contour, int header_size, MemStorage* storage, Sequence** dst_contour, float eps ) ;

CVLIB_DECLSPEC double ArcLength( const void *array, Slice slice, int is_closed ) ;

/* external contour area function */
CVLIB_DECLSPEC double ContourArea( const void *array, Slice slice ) ;

CVLIB_DECLSPEC int CheckContourConvexity( const Arr* array ) ;

/* finds sequence element by its index */
CVLIB_DECLSPEC char* GetSeqElem( const Sequence *seq, int index ) ;

/* pushes element to the sequence */
CVLIB_DECLSPEC char* SeqPush( Sequence *seq, void *element ) ;

/* pops the last element out of the sequence */
CVLIB_DECLSPEC int SeqPop( Sequence *seq, void *element ) ;

/* recycles a sequence block for the further use */
CVLIB_DECLSPEC int FreeSeqBlock( Sequence *seq, int in_front_of ) ;

CVLIB_DECLSPEC Sequence* PointSeqFromMat( int seq_kind, const Arr* arr, Contour* contour_header, SeqBlock* block ) ;

/* constructs sequence from array without copying any data.
the resultant sequence can't grow above its initial size */
CVLIB_DECLSPEC Sequence* MakeSeqHeaderForArray( int seq_flags, int header_size, int elem_size, void *array, 
								int total, Sequence *seq, SeqBlock * block ) ;

CVLIB_DECLSPEC int SliceLength( Slice slice, const Sequence* seq ) ;

/* area of a whole sequence */
CVLIB_DECLSPEC int iContourArea( const Sequence* contour, double *area ) ;

/* area of a contour sector */
CVLIB_DECLSPEC int iContourSecArea( Sequence * contour, Slice slice, double *area ) ;

/* returns the current reader position */
CVLIB_DECLSPEC int GetSeqReaderPos( SeqReader* reader ) ;

/* removes several elements from the end of sequence */
CVLIB_DECLSPEC void SeqPopMulti( Sequence *seq, void *_elements, int count, int front=0 ) ;

/* sets reader position to given absolute or relative
(relatively to the current one) position */
CVLIB_DECLSPEC int SetSeqReaderPos( SeqReader* reader, int index, int is_relative = 0 ) ;

/*=====================================Memory Functions=========================================*/

/****************************************Sequence Operations*************************************/
CVLIB_DECLSPEC void* Alloc( int size ) ;

CVLIB_DECLSPEC Sequence* CreateSeq( int seq_flags, int header_size, int elem_size, MemStorage * storage ) ;

CVLIB_DECLSPEC int CreateSeqBlock( SeqWriter * writer ) ;

CVLIB_DECLSPEC int InsertNodeIntoTree( void* _node, void* _parent, void* _frame ) ;

CVLIB_DECLSPEC int SetSeqBlockSize( Sequence *seq, int delta_elements ) ;

CVLIB_DECLSPEC int StartAppendToSeq( Sequence *seq, SeqWriter * writer ) ;

CVLIB_DECLSPEC int StartReadSeq( const Sequence *seq, SeqReader * reader, int reverse=0 ) ;

CVLIB_DECLSPEC int StartWriteSeq( int seq_flags, int header_size, int elem_size, MemStorage * storage, SeqWriter * writer ) ;

CVLIB_DECLSPEC Rect BoundingRect( Arr* array, int update ) ;

//SeqWiriter funcs

/* calls icvFlushSeqWriter and finishes writing process */
CVLIB_DECLSPEC Sequence* EndWriteSeq( SeqWriter * writer ) ;

CVLIB_DECLSPEC int FlushSeqWriter( SeqWriter * writer ) ;

CVLIB_DECLSPEC int GrowSeq( Sequence *seq, int in_front_of ) ;

CVLIB_DECLSPEC int GoNextMemBlock( MemStorage * storage ) ;

CVLIB_DECLSPEC int SetAdd( Set* set, SetElem* element, SetElem** inserted_element ) ;

CVLIB_DECLSPEC int StartReadChainPoints( Chain * chain, ChainPtReader * reader ) ;

CVLIB_DECLSPEC int ChangeSeqBlock( void* _reader, int direction ) ;

//memstorage

CVLIB_DECLSPEC MemStorage* CreateMemStorage( int block_size=0 ) ;

/* creates empty set */
CVLIB_DECLSPEC Set* CreateSet( int set_flags, int header_size, int elem_size, MemStorage * storage ) ;

/* creates child memory storage */
CVLIB_DECLSPEC MemStorage* CreateChildMemStorage( MemStorage * parent ) ;

/* initializes allocated storage */
CVLIB_DECLSPEC int InitMemStorage( MemStorage* storage, int block_size ) ;

CVLIB_DECLSPEC int SaveMemStoragePos( const MemStorage * storage, MemStoragePos * pos ) ;

CVLIB_DECLSPEC int RestoreMemStoragePos( MemStorage * storage, MemStoragePos * pos ) ;

CVLIB_DECLSPEC int ClearMemStorage( MemStorage * storage ) ;

CVLIB_DECLSPEC int ReleaseMemStorage( MemStorage** storage ) ;

CVLIB_DECLSPEC void* MemStorageAlloc( MemStorage* storage, int size ) ;

CVLIB_DECLSPEC int DestroyMemStorage( MemStorage* storage ) ;

}

#endif//_FINDCONTOUR_H_

