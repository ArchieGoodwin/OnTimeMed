
/*!
 * \file CommandLineParameters.h
 * \brief CommandLineÌ© ÁÄµ¹¸¡À¾´ÉË¾ ±ã¶®Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷.
 * \author ±®½èË§
 */

#if !defined(_CommandLineParameters_H_)
#define _CommandLineParameters_H_

#include "cvlibbaseDef.h"
#include "SString.h"

namespace CVLib
{

/**
 * @brief : CommandLineÌ© ÁÄµ¹¸¡À¾´ÉË¾ ±ã¶®Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 */
class CVLIB_DECLSPEC CommandLineParameters
{
public:
	CommandLineParameters(char* szCommandLine = NULL);
	virtual ~CommandLineParameters();

    BOOL CheckHelp(const BOOL fNoSwitches = FALSE);
	
    int ParamCount() { return m_nParamCount; }
    SString ParamLine();
    SString CommandLine();
    SString ParamStr(int index, const BOOL fGetAll = FALSE);
    int ParamInt(int index);
	
    int FirstNonSwitchIndex();
    SString FirstNonSwitchStr();
    int SwitchCount();
	int Switch(const char *sz, const BOOL fCase = FALSE);
    SString GetSwitchStr(const char *sz, const char *szDefault = "", const BOOL fCase = FALSE);
    int GetSwitchInt(const char *sz, const int iDefault = -1, const BOOL fCase = FALSE);
    SString GetNonSwitchStr(const BOOL fBreakAtSwitch = TRUE, const BOOL fFirstOnly = FALSE);

private:
	char* m_pszParams[100];
	int	m_nMaxParams;
	int	m_nParamCount;
    char* m_pszCmdLineDup;
	const char* m_szSwitchChars;
	BOOL IsSwitch(const char *sz);
	int CreateParameterFromString(char *pszParams, char *argv[], int max);
};

}

#endif // !defined(_CommandLineParameters_H_)
