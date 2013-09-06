/*!
 * \file	Object.h
 * \ingroup base
 * \brief	·º´Ç ¿Ðµ¹º÷Ì© ±¨½ó¿Ðµ¹º÷µá ¶®Ë¦´ó²÷ Object¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _Exception_H_
#define _Exception_H_

#include "SString.h"

namespace CVLib 
{
	
/**
 * @brief	CVLib LibraryÌ© ±¨½ó¿Ðµ¹º÷.
 <pre> ËË ¿Ðµ¹º÷²÷ ÊïË±¼¿¼ÇË¾ Ì¡Â×ÊÞ º¼±Ë´óÊî³Þ. </pre>
 */
class CVLIB_DECLSPEC Exception
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	Exception ();
	//! ÃÍ¾Ë»ô
    virtual ~Exception() throw();

    Exception(const Exception& rhs) throw();
    Exception& operator=(const Exception& rhs) throw();
    virtual const char *What() const throw() {return ErrorMessage();}

	// @cmember
	// Constructor.
	Exception(const char *szMessage, long lErrorCode,   
		      const char *szFunctionName = 0, 
			  const char *szSourceFile = 0, 
			  int iLine = 0) throw();
	
	// @cmember
	// Get the function name.
	const char *FunctionName(void) const {return m_szFunctionName;}
	
	// @cmember
	// Get the error message.
	const char *ErrorMessage(void) const {return m_szMessage;}
	
	// @cmember
	// Get the source file.
	const char *SourceFile(void) const {return m_szSourceFile;}
	
	// @cmember
	// Get the line number.
	int SourceLine(void) const {return m_line;}
	
	// @cmember
	// Get the error code.
	long ErrorCode(void) const {return m_code;}
	
	// @cmember
	// Return a complete message.
	virtual const char *FullMessage(void);
	
	
	// @cmember
	// Since we don't have run-time type information, we have a virtual
	// ThrowLast() method to make sure that type information for
	// derived classes is not lost in try-catch blocks.
	virtual void ThrowLast(void) {	throw *this;}
	
	
	// @access:(IMPL) Protected members
protected:
    // @cmember:(IMPL)
	// Function name that the error occurred in.
	const char *m_szFunctionName;
	
	// @cmember:(IMPL)
	// Error message.
	const char *m_szMessage;
	
	// @cmember:(IMPL)
	// Source file that the error occurred in.
	const char *m_szSourceFile;
	
    // @cmember:(IMPL)
	// Full error message.
	SString m_strFullMessage;
	
	// @cmember:(IMPL)
	// Line number that the error occurred on.
	int  m_line;
	
	// @cmember:(IMPL)
	// Error code.
	long m_code;
};

}

#endif //_Exception_H_