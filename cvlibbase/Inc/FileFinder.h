/*!
 * \file    XFile.h
 * \ingroup base
 * \brief   ÁÄËÎ¼¿»õË¾ Ì¡ÂÙ ±¨½ó¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#if !defined(_FileFinder_H_)
#define _FileFinder_H_

#include "cvlibmacros.h"

#if CVLIB_OS == CVLIB_OS_WIN32
#include <windows.h>
#undef GetRValue
#undef GetGValue
#undef GetBValue
#endif

#include "cvlibbasedef.h"
#include "Object.h"
#include "SString.h"

namespace CVLib
{

#if CVLIB_OS == CVLIB_OS_WIN32
class CVLIB_DECLSPEC FileFinder : public Object
{
public:
	FileFinder();
	virtual ~FileFinder();
	
	// Attributes
public:
	DWORD GetLength() const;
	virtual SString GetFileName() const;
	virtual SString GetFilePath() const;
	virtual SString GetFileTitle() const;
	virtual SString GetFileURL() const;
	virtual SString GetRoot() const;
	
	virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp) const;
	virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp) const;
	virtual BOOL GetCreationTime(FILETIME* pTimeStamp) const;
// 	virtual BOOL GetLastWriteTime(CTime& refTime) const;
// 	virtual BOOL GetLastAccessTime(CTime& refTime) const;
// 	virtual BOOL GetCreationTime(CTime& refTime) const;
	
	virtual BOOL MatchesMask(DWORD dwMask) const;
	
	virtual BOOL IsDots() const;
	// these aren't virtual because they all use MatchesMask(), which is
	BOOL IsReadOnly() const;
	BOOL IsDirectory() const;
	BOOL IsCompressed() const;
	BOOL IsSystem() const;
	BOOL IsHidden() const;
	BOOL IsTemporary() const;
	BOOL IsNormal() const;
	BOOL IsArchived() const;
	
	// Operations
	void Close();
	virtual BOOL FindFile(const char* pstrName = NULL, DWORD dwUnused = 0);
	virtual BOOL FindNextFile();
	
protected:
	virtual void CloseContext();
	
	// Implementation
protected:
	void* m_pFoundInfo;
	void* m_pNextInfo;
	HANDLE m_hContext;
	BOOL m_bGotLast;
	SString m_strRoot;
	char m_chDirSeparator;     // not '\\' for Internet classes
	
	void AssertValid() const;
};

inline BOOL FileFinder::IsReadOnly() const
{ return MatchesMask(FILE_ATTRIBUTE_READONLY); }
inline BOOL FileFinder::IsDirectory() const
{ return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
inline BOOL FileFinder::IsCompressed() const
{ return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
inline BOOL FileFinder::IsSystem() const
{ return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
inline BOOL FileFinder::IsHidden() const
{ return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
inline BOOL FileFinder::IsTemporary() const
{ return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
inline BOOL FileFinder::IsNormal() const
{ return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
inline BOOL FileFinder::IsArchived() const
{ return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }

#endif

}

#endif //_FileFinder_H_