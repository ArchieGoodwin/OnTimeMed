/*! 
 * \file    cvlibcommon.cpp
 * \ingroup base
 * \brief   cvlib
 * \author  
 */

#include "cvlibmacros.h"
#if CVLIB_OS == CVLIB_OS_WIN32
#include <sys/stat.h>
#include <direct.h>
#include <windows.h>
#undef GetRValue
#undef GetGValue
#undef GetBValue
#endif

#include "cvlibMalloc.h"
#include "cvlibutil.h"
#include "cvlibbasedef.h"
#include "Mat.h"

namespace CVLib 
{

	int CVUtil::CompareIncIdx(const void *p1,const void *p2)
	{
		SElem *x = (SElem*)p1;
		SElem *y = (SElem*)p2;
		
		double i = x->rValue;
		double j = y->rValue;
		if(i > j)
			return 1; //sort in assending order
		if(i < j)
			return -1;
		return  0;
	}
	
	int CVUtil::CompareDecIdx(const void *p1,const void *p2)
	{
		SElem *x = (SElem*)p1;
		SElem *y = (SElem*)p2;
		
		double i = x->rValue;
		double j = y->rValue;
		if(i < j)
			return 1; //sort in assending order
		if(i > j)
			return -1;
		return  0;
	}
	
	int CVUtil::CompareIncInt(const void *p1,const void *p2)
	{
		int x = *(int*)p1;
		int y = *(int*)p2;
		
		if(x > y)
			return 1; //sort in assending order
		if(x < y)
			return -1;
		return  0;
	}
	
	int CVUtil::CompareDecInt(const void *p1,const void *p2)
	{
		int x = *(int*)p1;
		int y = *(int*)p2;
		
		if(x < y)
			return 1; //sort in assending order
		if(x > y)
			return -1;
		return  0;
	}
	
	void CVUtil::Sort (int* pValues, int nNum, enSortFlag nFlag)
	{
		switch (nFlag)
		{
		case SORT_INC:
			qsort (pValues, nNum, sizeof(int), CompareIncInt);
			break;
		case SORT_DEC:
			qsort (pValues, nNum, sizeof(int), CompareDecInt);
			break;
		}
	}

	int CVUtil::MkDir( const char* filename )
	{
#if CVLIB_OS == CVLIB_OS_WIN32
		char path[260];
		char* p;
		int pos;
		
		struct _stat st;
		
		strcpy( path, filename );
		
		p = path;
		for( ; ; )
		{
			pos = strcspn( p, "/\\" );
			
			if( pos == (int) strlen( p ) ) break;
			if( pos != 0 )
			{
				p[pos] = '\0';
				
				if( p[pos-1] != ':' )
				{
					if( _stat( path, &st ) != 0 )
					{
						if( _mkdir( path ) != 0 ) return 0;
					}
				}
			}
			
			p[pos] = '/';
			
			p += pos + 1;
		}
		return 1;
#elif CVLIB_OS == CVLIB_OS_APPLE
		return 0;
#endif
	}

	int CVUtil::Round(float a)
	{
		if (a > 0)
			return (int)(a+0.5f);
		else
			return (int)(a-0.5f);
	}
	int CVUtil::Round(double a)
	{
		if (a > 0)
			return (int)(a+0.5);
		else
			return (int)(a-0.5);
	}
	void CVUtil::SortIdx (int* pValues, int nNum, int* pnIdx, int nK, enSortFlag nFlag)
	{
		SElem* pElems = new SElem[nNum];
		int i;
		for (i = 0; i < nNum; i ++)
		{
			pElems[i].rValue = pValues[i];
			pElems[i].nIdx = i;
		}
		switch (nFlag)
		{
		case SORT_INC:
			qsort (pElems, nNum, sizeof(SElem), CompareIncIdx);
			break;
		case SORT_DEC:
			qsort (pElems, nNum, sizeof(SElem), CompareDecIdx);
			break;
		}
		
		if (pnIdx)
		{
			for (i = 0; i < nK; i ++)
				pnIdx[i] = pElems[i].nIdx;
		}
		delete []pElems;
	}

	void CVUtil::SortIdx (float* pValues, int nNum, int* pnIdx, int nK, enSortFlag nFlag)
	{
		SElem* pElems = new SElem[nNum];
		int i;
		for (i = 0; i < nNum; i ++)
		{
			pElems[i].rValue = pValues[i];
			pElems[i].nIdx = i;
		}
		switch (nFlag)
		{
		case SORT_INC:
			qsort (pElems, nNum, sizeof(SElem), CompareIncIdx);
			break;
		case SORT_DEC:
			qsort (pElems, nNum, sizeof(SElem), CompareDecIdx);
			break;
		}
		
		if (pnIdx)
		{
			for (i = 0; i < nK; i ++)
				pnIdx[i] = pElems[i].nIdx;
		}
		delete []pElems;	}

	void CVUtil::SortIdx (double* pValues, int nNum, int* pnIdx, int nK, enSortFlag nFlag)
	{
		SElem* pElems = new SElem[nNum];
		int i;
		for (i = 0; i < nNum; i ++)
		{
			pElems[i].rValue = pValues[i];
			pElems[i].nIdx = i;
		}
		switch (nFlag)
		{
		case SORT_INC:
			qsort (pElems, nNum, sizeof(SElem), CompareIncIdx);
			break;
		case SORT_DEC:
			qsort (pElems, nNum, sizeof(SElem), CompareDecIdx);
			break;
		}
		
		if (pnIdx)
		{
			for (i = 0; i < nK; i ++)
				pnIdx[i] = pElems[i].nIdx;
		}
		delete []pElems;	
	}
	
	int		CVUtil::FindMin (int* pValue, int nNum, int* pnIdx/* = NULL*/)
	{
		if (nNum == 1)
		{
			if (pnIdx)
				*pnIdx = 0;
			return pValue[0];
		}
		int nIdx = 0;
		int rMin = pValue[0];
		for (int i = 1; i < nNum; i ++)
		{
			if (pValue[i] < rMin)
			{
				rMin = pValue[i];
				nIdx = i;
			}
		}
		if (pnIdx)
			*pnIdx = nIdx;
		return rMin;
	}

	float	CVUtil::FindMin (float* pValue, int nNum, int* pnIdx/* = NULL*/)
	{
		if (nNum == 1)
		{
			if (pnIdx)
				*pnIdx = 0;
			return pValue[0];
		}
		int nIdx = 0;
		float rMin = pValue[0];
		for (int i = 1; i < nNum; i ++)
		{
			if (pValue[i] < rMin)
			{
				rMin = pValue[i];
				nIdx = i;
			}
		}
		if (pnIdx)
			*pnIdx = nIdx;
		return rMin;
	}

	double	CVUtil::FindMin (double* pValue, int nNum, int* pnIdx/* = NULL*/)
	{
		if (nNum == 1)
		{
			if (pnIdx)
				*pnIdx = 0;
			return pValue[0];
		}
		int nIdx = 0;
		double rMin = pValue[0];
		for (int i = 1; i < nNum; i ++)
		{
			if (pValue[i] < rMin)
			{
				rMin = pValue[i];
				nIdx = i;
			}
		}
		if (pnIdx)
			*pnIdx = nIdx;
		return rMin;
	}
	
	int		CVUtil::FindMax (int* pValue, int nNum, int* pnIdx /*= NULL*/)
	{
		if (nNum == 1)
		{
			if (pnIdx)
				*pnIdx = 0;
			return pValue[0];
		}
		int nIdx = 0;
		int rMax = pValue[0];
		for (int i = 1; i < nNum; i ++)
		{
			if (pValue[i] > rMax)
			{
				rMax = pValue[i];
				nIdx = i;
			}
		}
		if (pnIdx)
			*pnIdx = nIdx;
		return rMax;
	}

	float	CVUtil::FindMax (float* pValue, int nNum, int* pnIdx /*= NULL*/)
	{
		if (nNum == 1)
		{
			if (pnIdx)
				*pnIdx = 0;
			return pValue[0];
		}
		int nIdx = 0;
		float rMax = pValue[0];
		for (int i = 1; i < nNum; i ++)
		{
			if (pValue[i] > rMax)
			{
				rMax = pValue[i];
				nIdx = i;
			}
		}
		if (pnIdx)
			*pnIdx = nIdx;
		return rMax;
	}

	double	CVUtil::FindMax (double* pValue, int nNum, int* pnIdx /*= NULL*/)
	{
		if (nNum == 1)
		{
			if (pnIdx)
				*pnIdx = 0;
			return pValue[0];
		}
		int nIdx = 0;
		double rMax = pValue[0];
		for (int i = 1; i < nNum; i ++)
		{
			if (pValue[i] > rMax)
			{
				rMax = pValue[i];
				nIdx = i;
			}
		}
		if (pnIdx)
			*pnIdx = nIdx;
		return rMax;
	}

// local (static) compare function for the qsort() call below
int CVUtil::CompareStr( const void *arg1, const void *arg2 )
{
#if CVLIB_OS == CVLIB_OS_WIN32
	return _stricmp( (const char*) ( *(SString*)arg1), (const char*) ( *(SString*)arg2) );
#elif CVLIB_OS == CVLIB_OS_APPLE
    return 1;
#endif
}

/**
  @memo     Scans and sorts a directory for files. 

  @doc      Scans and sorts a directory for files with a specfied extension.
  
  @param    path		Path to read from.
  @param    extension	The file extension to search for. ex. "hips".
  
  @return  The filenames found without any path. 
  
*/
int CVUtil::ScanSortDir(const SString &path, const SString &extension, StringArray& filenames, BOOL fSubDirectory)
{
#if CVLIB_OS == CVLIB_OS_WIN32
	WIN32_FIND_DATA fd;
	HANDLE h;
	SString searchPath;
	int nbFiles;
	
	// add terminatin backslash (if needed)
	SString pathBS = CVUtil::AddBackSlash(path);

	if (fSubDirectory)
	{
		searchPath = pathBS + SString("*.*");
		h = FindFirstFile( searchPath, &fd );
		if (h==INVALID_HANDLE_VALUE) 
			return 0;	// path does not exist
		if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != '.')
		{
			ScanSortDir(pathBS+SString(fd.cFileName), extension, filenames, fSubDirectory);
		}
		while( FindNextFile( h, &fd ) ) {
			if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != '.')
			{
				ScanSortDir(pathBS+SString(fd.cFileName), extension, filenames, fSubDirectory);
			}
		}

	}
	
	// build and sort list of filenames
	searchPath = pathBS + SString("*.") + extension;
	h = FindFirstFile( searchPath, &fd );
	if (h==INVALID_HANDLE_VALUE) 
		return 0;	// path does not exist
	filenames.Add( pathBS+SString(fd.cFileName) );
	nbFiles=1;
	while( FindNextFile( h, &fd ) ) {
		
		filenames.Add( pathBS+SString(fd.cFileName) );
		nbFiles++;
	}
	FindClose (h);
	
	// sort the filenames
	qsort(	(void *)(filenames.GetData()), (size_t)filenames.GetSize(),
		sizeof( SString ), CompareStr );
#endif	
	return filenames.GetSize();
}

/**
  @memo    Removes the extension of a file name.

  @doc      Removes the extension of a file name.
  
  @param    s	Input file name.
  
  @return   File name without extension.
  
*/
SString CVUtil::RemoveExt( const SString &s ) 
{
	return s.Left( s.ReverseFind('.') );
}


/**
  @memo    Returns the extension of a file name.

  @doc      Returns the extension of a file name.
  
  @param    s	Input filename
  
  @return   The extension.
  
*/
SString CVUtil::GetExt( const SString &s ) {
	
	return s.Right( s.GetLength()-s.ReverseFind('.')-1 );
}

/**
  @memo     Ensures that a string is terminated with a backslash

  @doc      Ensures that a string is terminated with a backslash.
			If the already has a terminating backslash, nothing
			is done.
  
  @param    path	Input string.
  
  @return   Backslash-terminated output string.
  
*/
SString CVUtil::AddBackSlash( const SString &path )
{
	int len = path.GetLength();

	if (len>0) {
		
		if (path[len-1]=='\\') {

			return path;
		}
	} else {

		return path;
	}
	return path+"\\";
}


/**
  @memo     Returns the path of a filename.

  @doc      Returns the path of a filename.
  
  @param    fullfilename	Filename including any path.
  
  @return   The path to the filename.
  
*/
SString CVUtil::GetPath( const SString &fullfilename )
{
	int slashpos = fullfilename.ReverseFind('\\');

	if (slashpos==-1) {
		
		// no path
		return "";
	}
	return fullfilename.Left( slashpos+1 );
}


/**
  @memo     Returns the file name of a path+file name string.

  @doc      Returns the file name of a path+file name string.
  
  @param    filename	Full qualified filename including path.
  
  @return   The file name including any extension, but without any path.
  
*/
SString CVUtil::GetFilename( const SString &filename )
{
	int slashpos=-1;
	int a=filename.ReverseFind('\\');
	int b=filename.ReverseFind('/');
	slashpos=MAX(a,b);
	if (slashpos==-1) {
		
		// no path
		return filename;
	}

	return filename.Right( filename.GetLength()-slashpos-1 );
}

/**
  @memo     Calculates the variance of each component in a set of vectors.
  @param    cVectors	Input set of vectors.
  @param	varVec		A vector containing the variance of each compoment
						in cVectors.
  @param    vpMean      Optional vector pointer to return the mean vector in.
  @return   Nothing.
*/
void CVUtil::CalcElementVar( const Array<Vecf, const Vecf&> &vVectors, Vecf &varVec, Vecf *vpMean ) 
{
	assert( vVectors.GetSize()>0 );	
    if (vpMean) 
        vpMean->Resize( vVectors[0].Length() );

	Vecf elemVec( vVectors.GetSize() );
	varVec.Resize( vVectors[0].Length() );

	for(int elem=0;elem<vVectors[0].Length();elem++) 
	{
		for(int vecNb=0;vecNb<vVectors.GetSize();vecNb++) 
			elemVec[vecNb] = vVectors[vecNb][elem];
        double mean;
		varVec[elem] = (float)elemVec.Var( &mean );
        if (vpMean)
            (*vpMean)[elem] = (float)mean;
	}    
}

void CVUtil::FindMaxIdx (Matf* pms, int nNum, Matb* pmIdx)
{
	int nLen = pms->Rows() * pms->Cols();
	for (int k=0; k<nLen; k++)
	{
		float rMax = -1E7;
		int nMax = -1;
		for (int i=0; i<nNum; i++)
		{
			if (rMax < pms[i].data.fl[0][k])
			{
				rMax = pms[i].data.fl[0][k];
				nMax = i;
			}
		}
		assert (nMax != -1);
		pmIdx->data.ptr[0][k] = nMax;
	}
}

void CVUtil::FindMinIdx (Matf* pms, int nNum, Matb* pmIdx)
{
	int nLen = pms->Rows() * pms->Cols();
	for (int k=0; k<nLen; k++)
	{
		float rMin = 1E7;
		int nMin = -1;
		for (int i=0; i<nNum; i++)
		{
			if (rMin > pms[i].data.fl[0][k])
			{
				rMin = pms[i].data.fl[0][k];
				nMin = i;
			}
		}
		assert (nMin != -1);
		pmIdx->data.ptr[0][k] = nMin;
	}
}

}