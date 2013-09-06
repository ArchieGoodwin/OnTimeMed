/*! 
 * \file    CVUtil.h
 * \ingroup base
 * \brief   cvlib
 * \author  
 */
#ifndef _CVLIBCOMMON_H_
#define _CVLIBCOMMON_H_

#include "mat.h"
#include "vec.h"
#include "SString.h"
#include "StringArray.h"
#include "template.h"

namespace CVLib 
{

class CVLIB_DECLSPEC CVUtil
{
public:
	enum enSortFlag	{SORT_INC, SORT_DEC	};
	struct SElem {double rValue;int nIdx;};

	static int ScanSortDir(const SString &path, const SString &extension, StringArray& filenames, BOOL fSubDirectory=FALSE);
	static SString RemoveExt( const SString &s ) ;
	static SString AddBackSlash( const SString &path );
	static SString GetExt( const SString &s );
	static SString GetPath( const SString &fullfilename );
	static SString GetFilename( const SString &filename );

	static int MkDir( const char* filename );
	static int Round(float a);
	static int Round(double a);
	static void Sort(int* pValues, int nNum, enSortFlag nFlag);

	static int CompareStr( const void *arg1, const void *arg2 );
	static int CompareIncIdx(const void *p1,const void *p2);
	static int CompareDecIdx(const void *p1,const void *p2);
	static int CompareIncInt(const void *p1,const void *p2);
	static int CompareDecInt(const void *p1,const void *p2);

	static void SortIdx (int* pValues, int nNum, int* pnIdx, int nK, enSortFlag nFlag);
	static void SortIdx (float* pValues, int nNum, int* pnIdx, int nK, enSortFlag nFlag);
	static void SortIdx (double* pValues, int nNum, int* pnIdx, int nK, enSortFlag nFlag);

	static int		FindMin (int* pValue, int nNum, int* pnIdx = NULL);
	static float	FindMin (float* pValue, int nNum, int* pnIdx = NULL);
	static double	FindMin (double* pValue, int nNum, int* pnIdx = NULL);
	static void		FindMinIdx (Matf* pms, int nNum, Matb* pmIdx);

	static int		FindMax (int* pValue, int nNum, int* pnIdx = NULL);
	static float	FindMax (float* pValue, int nNum, int* pnIdx = NULL);
	static double	FindMax (double* pValue, int nNum, int* pnIdx = NULL);
	static void		FindMaxIdx (Matf* pms, int nNum, Matb* pmIdx);

	static void CalcElementVar( const Array<Vecf, const Vecf&> &vVectors, Vecf &varVec, Vecf *vpMean = NULL);
};


}

#endif //_CVLIBCOMMON_H_