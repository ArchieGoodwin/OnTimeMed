/*!
 * \file	Object.h
 * \ingroup base
 * \brief	·º´Ç ¿Ðµ¹º÷Ì© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ Object¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "Exception.h"

namespace CVLib 
{
	
// @mfunc Creates an error object with given data.
Exception::Exception()
{
	m_szMessage = 0;
	m_szFunctionName = 0;
	m_szSourceFile = 0;
	m_code=0;
	m_line=0;
}
Exception::Exception(
     const char *szMessage,				// @parm Error message.
	 long lErrorCode,					// @parm Error code.
	 const char *szFunctionName,		// @parm Name of function that caused an exception.
	 const char *szSourceFile,			// @parm Source file.
	 int iLine)	throw()					// @parm Source line number.
 :  m_szMessage(szMessage),
	m_szFunctionName(szFunctionName),
	m_szSourceFile(szSourceFile),
	m_code(lErrorCode),
	m_line(iLine)
{
}

Exception::Exception(const Exception& rhs) throw()
{
}

Exception::~Exception() throw()
{
	ThrowLast();
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | Exception | FullMessage |
//
// Return a complete message.
//
////////////////////////////////////////////////////////////////////////////
inline const char *Exception::FullMessage(void)
{ 
    m_strFullMessage = "Exception: \nsource file: ";
	m_strFullMessage += m_szSourceFile;
	m_strFullMessage += "\nfunction name: ";
	m_strFullMessage += m_szFunctionName;
	m_strFullMessage += "\nmessage: ";
	m_strFullMessage += m_szMessage;

    return m_strFullMessage.GetBuffer(0);
}

}