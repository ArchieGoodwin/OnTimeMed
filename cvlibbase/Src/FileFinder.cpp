#include "FileFinder.h"

namespace CVLib
{

#if CVLIB_OS == CVLIB_OS_WIN32
FileFinder::FileFinder()
{
	m_pFoundInfo = NULL;
	m_pNextInfo = NULL;
	m_hContext = NULL;
	m_chDirSeparator = '\\';
}

FileFinder::~FileFinder()
{
	Close();
}

void FileFinder::Close()
{
	if (m_pFoundInfo != NULL)
	{
		delete m_pFoundInfo;
		m_pFoundInfo = NULL;
	}

	if (m_pNextInfo != NULL)
	{
		delete m_pNextInfo;
		m_pNextInfo = NULL;
	}

	if (m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
	{
		CloseContext();
		m_hContext = NULL;
	}
}

void FileFinder::CloseContext()
{
	::FindClose(m_hContext);
	return;
}

BOOL FileFinder::FindFile(const char* pstrName /* = NULL */,
	DWORD dwUnused /* = 0 */)
{
	Close();
	m_pNextInfo = new WIN32_FIND_DATA;
	m_bGotLast = FALSE;

	if (pstrName == NULL)
		pstrName = "*.*";
	lstrcpy(((WIN32_FIND_DATA*) m_pNextInfo)->cFileName, pstrName);

	m_hContext = ::FindFirstFile(pstrName, (WIN32_FIND_DATA*) m_pNextInfo);

	if (m_hContext == INVALID_HANDLE_VALUE)
	{
		DWORD dwTemp = ::GetLastError();
		Close();
		::SetLastError(dwTemp);
		return FALSE;
	}

	LPTSTR pstrRoot = m_strRoot.GetBufferSetLength(CVLIB_PATH_MAX);
	LPCTSTR pstr = _fullpath(pstrRoot, pstrName, CVLIB_PATH_MAX);

	// passed name isn't a valid path but was found by the API
	assert(pstr != NULL);
	if (pstr == NULL)
	{
		m_strRoot.ReleaseBuffer(-1);
		Close();
		::SetLastError(ERROR_INVALID_NAME);
		return FALSE;
	}
	else
	{
		// find the last forward or backward whack
		LPTSTR pstrBack  = strrchr(pstrRoot, '\\');
		LPTSTR pstrFront = strrchr(pstrRoot, '/');

		if (pstrFront != NULL || pstrBack != NULL)
		{
			if (pstrFront == NULL)
				pstrFront = pstrRoot;
			if (pstrBack == NULL)
				pstrBack = pstrRoot;

			// from the start to the last whack is the root

			if (pstrFront >= pstrBack)
				*pstrFront = '\0';
			else
				*pstrBack = '\0';
		}
		m_strRoot.ReleaseBuffer(-1);
	}

	return TRUE;
}

BOOL FileFinder::MatchesMask(DWORD dwMask) const
{
	assert(m_hContext != NULL);

	if (m_pFoundInfo != NULL)
		return (!!(((LPWIN32_FIND_DATA) m_pFoundInfo)->dwFileAttributes & dwMask));
	else
		return FALSE;
}

BOOL FileFinder::GetLastAccessTime(FILETIME* pTimeStamp) const
{
	assert(m_hContext != NULL);
	assert(pTimeStamp != NULL);

	if (m_pFoundInfo != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastAccessTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL FileFinder::GetLastWriteTime(FILETIME* pTimeStamp) const
{
	assert(m_hContext != NULL);
	assert(pTimeStamp != NULL);

	if (m_pFoundInfo != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastWriteTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL FileFinder::GetCreationTime(FILETIME* pTimeStamp) const
{
	assert(m_hContext != NULL);

	if (m_pFoundInfo != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftCreationTime;
		return TRUE;
	}
	else
		return FALSE;
}

// BOOL FileFinder::GetLastAccessTime(CTime& refTime) const
// {
// 	assert(m_hContext != NULL);
// 	ASSERT_VALID(this);
// 
// 	if (m_pFoundInfo != NULL)
// 	{
// 		refTime = CTime(((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastAccessTime);
// 		return TRUE;
// 	}
// 	else
// 		return FALSE;
// }
// 
// BOOL FileFinder::GetLastWriteTime(CTime& refTime) const
// {
// 	assert(m_hContext != NULL);
// 	ASSERT_VALID(this);
// 
// 	if (m_pFoundInfo != NULL)
// 	{
// 		refTime = CTime(((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastWriteTime);
// 		return TRUE;
// 	}
// 	else
// 		return FALSE;
// }
// 
// BOOL FileFinder::GetCreationTime(CTime& refTime) const
// {
// 	assert(m_hContext != NULL);
// 	ASSERT_VALID(this);
// 
// 	if (m_pFoundInfo != NULL)
// 	{
// 		refTime = CTime(((LPWIN32_FIND_DATA) m_pFoundInfo)->ftCreationTime);
// 		return TRUE;
// 	}
// 	else
// 		return FALSE;
// }

BOOL FileFinder::IsDots() const
{
	assert(m_hContext != NULL);

	// return TRUE if the file name is "." or ".." and
	// the file is a directory

	BOOL bResult = FALSE;
	if (m_pFoundInfo != NULL && IsDirectory())
	{
		LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA) m_pFoundInfo;
		if (pFindData->cFileName[0] == '.')
		{
			if (pFindData->cFileName[1] == '\0' ||
				(pFindData->cFileName[1] == '.' &&
				 pFindData->cFileName[2] == '\0'))
			{
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

BOOL FileFinder::FindNextFile()
{
	assert(m_hContext != NULL);

	if (m_hContext == NULL)
		return FALSE;
	if (m_pFoundInfo == NULL)
		m_pFoundInfo = new WIN32_FIND_DATA;


	void* pTemp = m_pFoundInfo;
	m_pFoundInfo = m_pNextInfo;
	m_pNextInfo = pTemp;

	return ::FindNextFile(m_hContext, (LPWIN32_FIND_DATA) m_pNextInfo);
}

SString FileFinder::GetFileURL() const
{
	assert(m_hContext != NULL);

	SString strResult("file://");
	strResult += GetFilePath();
	return strResult;
}

SString FileFinder::GetRoot() const
{
	assert(m_hContext != NULL);

	return m_strRoot;
}

SString FileFinder::GetFilePath() const
{
	assert(m_hContext != NULL);

	SString strResult = m_strRoot;
	if (strResult[strResult.GetLength()-1] != '\\' &&
		strResult[strResult.GetLength()-1] != '/')
		strResult += m_chDirSeparator;
	strResult += GetFileName();
	return strResult;
}

SString FileFinder::GetFileTitle() const
{
	assert(m_hContext != NULL);

	SString strFullName = GetFileName();
	SString strResult;

	_splitpath(strFullName, NULL, NULL, strResult.GetBuffer(CVLIB_PATH_MAX), NULL);
	strResult.ReleaseBuffer();
	return strResult;
}

SString FileFinder::GetFileName() const
{
	assert(m_hContext != NULL);

	SString ret;

	if (m_pFoundInfo != NULL)
		ret = ((LPWIN32_FIND_DATA) m_pFoundInfo)->cFileName;
	return ret;
}

DWORD FileFinder::GetLength() const
{
	assert(m_hContext != NULL);

	if (m_pFoundInfo != NULL)
		return ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow;
	else
		return 0;
}

void FileFinder::AssertValid() const
{
	// if you trip the assert in the else side, you've called
	// a Get() function without having done at least one
	// FindNext() call

	if (m_hContext == NULL)
		assert(m_pFoundInfo == NULL && m_pNextInfo == NULL);
	else
		assert(m_pFoundInfo != NULL && m_pNextInfo != NULL);

}

#endif

}
