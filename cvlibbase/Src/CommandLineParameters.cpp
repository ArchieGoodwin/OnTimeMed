/*!
 * \file CommandLineParameters.cpp
 * \brief CommandLineÌ© ÁÄµ¹¸¡À¾´ÉË¾ »Ú±Â ±ã¶®Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷
 * \author ±®½èË§
 */

#include "cvlibmacros.h"
#if CVLIB_OS == CVLIB_OS_WIN32
#include <windows.h>
#undef GetRValue
#undef GetGValue
#undef GetBValue
#else
using namespace std;
#endif

#include "CommandLineParameters.h"
#include <string.h>

namespace CVLib
{

static char mszSwitchChars[] = "-/";

CommandLineParameters::CommandLineParameters(char* szCommandLine /*= 0*/)
{
#if CVLIB_OS == CVLIB_OS_WIN32
	if (szCommandLine)
		m_pszCmdLineDup = _strdup(szCommandLine);
	else
		m_pszCmdLineDup = _strdup(GetCommandLine());
#else
	if (szCommandLine)
	{
		m_pszCmdLineDup = (char*)malloc (strlen(szCommandLine) + 1);
		strcpy (m_pszCmdLineDup, szCommandLine);
	}
	else
		assert (false);
#endif
	m_nMaxParams = 100;
	m_nParamCount = CreateParameterFromString(m_pszCmdLineDup, m_pszParams, m_nMaxParams);

	m_szSwitchChars = mszSwitchChars;
}

CommandLineParameters::~CommandLineParameters()
{
	if (m_pszCmdLineDup)
	{
		free (m_pszCmdLineDup);
		m_pszCmdLineDup = NULL;
	}
}

int CommandLineParameters::CreateParameterFromString(char *pszParams, char *argv[], int max)
{
    int argc = 0;
    if (pszParams) 
	{
        char *p = pszParams;
        while (*p && (argc < max)) 
		{
            while (*p == ' ') 
			{
                p++;
            }
            if (!*p) 
			{
                break;
            }
            if (*p == '"') 
			{
                p++;
                argv[argc++] = p;
                while (*p && *p != '"') 
				{
                    p++;
                }
            } else 
			{
                argv[argc++] = p;
                while (*p && *p != ' ') 
				{
                    p++;
                }
            }
            if (*p) 
			{
                *p++ = 0;
            }
        }
    }
    return argc;
}

BOOL CommandLineParameters::CheckHelp(const BOOL fNoSwitches /*= FALSE */)
{
     if (fNoSwitches && (m_nParamCount < 2)) return TRUE;
     if (m_nParamCount < 2) return FALSE;
     if (strcmp(ParamStr(1).GetBuffer(0),"-?") == 0) return TRUE;
     if (strcmp(ParamStr(1).GetBuffer(0),"/?") == 0) return TRUE;
     if (strcmp(ParamStr(1).GetBuffer(0),"?") == 0) return TRUE;
     return FALSE;
}

SString CommandLineParameters::ParamStr(const int index, const BOOL fGetAll /* = FALSE */)
{
    if ((index < 0) || (index >= m_nParamCount)) {
        return SString("");
    }
    SString s = SString(m_pszParams[index]);
    if (fGetAll) {
        for (int i = index+1;i < m_nParamCount; i++) {
              s += " ";
              s += m_pszParams[i];
        }
    }
    return s;
}

int CommandLineParameters::ParamInt(const int index)
{
    return atoi(ParamStr(index));
}

SString CommandLineParameters::ParamLine()
{
    SString s;
#if CVLIB_OS == CVLIB_OS_WIN32
    char *p = strchr(GetCommandLine(),' ');
    if (p) {
        s.Format("%s",p+1);
    }
#endif
    return s;
}

SString CommandLineParameters::CommandLine()
{
    SString s;
#if CVLIB_OS == CVLIB_OS_WIN32
    s.Format("%s",GetCommandLine());
#endif
    return s;
}

BOOL CommandLineParameters::IsSwitch(const char *sz)
{
    return (strchr(m_szSwitchChars,sz[0]) != NULL);
}

int CommandLineParameters::SwitchCount()
{
    int count = 0;
    for (int i = 1;i < m_nParamCount; i++) {
        if (IsSwitch(m_pszParams[i])) count++;
    }
    return count;
}

int CommandLineParameters::FirstNonSwitchIndex()
{
    for (int i = 1;i < m_nParamCount; i++) {
        if (!IsSwitch(m_pszParams[i])) {
            return i;
        }
    }
    return 0;
}

SString CommandLineParameters::FirstNonSwitchStr()     // 499.5 04/16/01 12:17 am
{
    // returns the first non-switch, handles lines such as:
    // [options] file [specs]
    return GetNonSwitchStr(FALSE,TRUE);
}

//////////////////////////////////////////////////////////////////////////
// Switch() will return the parameter index if the switch exist.
// Return 0 if not found.  The logic will allow for two types of
// switches:
//
//          /switch value
//          /switch:value
//
// DO NOT PASS THE COLON. IT IS AUTOMATICALLY CHECKED.  In other
// words, the following statements are the same:
//
//         Switch("server");
//         Switch("-server");
//         Switch("/server");
//
// to handle the possible arguments:
//
//         /server:value
//         /server value
//         -server:value
//         -server value
//

int CommandLineParameters::Switch(const char *sz,
                                   const BOOL fCase /* = FALSE */
                                   )
{
    if (!sz || !sz[0]) {
        return 0;
    }

    char sz2[255];
    strncpy(sz2,sz,sizeof(sz2)-1);
    sz2[sizeof(sz2)-1] = 0;

    char *p = sz2;
    if (strchr(m_szSwitchChars,*p) != NULL) p++;

    // check for abbrevation

    int amt = 0;
    char *abbr = strchr(p,'*');
    if (abbr) {
        *abbr = 0;
        amt = strlen(p);
        strcpy(abbr,abbr+1);
    }

    for (int i = 1;i < m_nParamCount; i++) {
      if (!IsSwitch(m_pszParams[i])) continue;
      char *pColon = strchr(&m_pszParams[i][1],':');
      if (pColon && (amt == 0)) { amt = strlen(p); }

      if (fCase)
	  {
        if (amt > 0) {
          if (strncmp(p,&m_pszParams[i][1],strlen(p)) != 0) continue; // 450.6b20
          if (strncmp(p,&m_pszParams[i][1],amt) == 0) return i;
        } else {
          if (strcmp(p,&m_pszParams[i][1]) == 0) return i;
        }
      }
	  else
	  {
#if CVLIB_OS == CVLIB_OS_WIN32
        if (amt > 0) {
          if (_strnicmp(p,&m_pszParams[i][1],strlen(p)) != 0) continue; // 450.6b20
          if (_strnicmp(p,&m_pszParams[i][1],amt) == 0) return i;
        } else {
          if (_stricmp(p,&m_pszParams[i][1]) == 0) return i;
        }
#else
		  if (amt > 0) {
			  if (strncmp(p,&m_pszParams[i][1],strlen(p)) != 0) continue; // 450.6b20
			  if (strncmp(p,&m_pszParams[i][1],amt) == 0) return i;
		  } else {
			  if (strcmp(p,&m_pszParams[i][1]) == 0) return i;
		  }
#endif
      }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// GetSwitchStr() will return the string for the given switch. The logic
// will allow for two types of switches:
//
//   /switch value
//   /switch:value
//

SString CommandLineParameters::GetSwitchStr(const char *sz,
                                             const char *szDefault, /* = "" */
                                             const BOOL fCase /* = FALSE */
                                            )
{
    int idx = Switch(sz,fCase);
    if (idx > 0) {
        SString s = ParamStr(idx);
        int n = s.Find(':');
        if (n > -1) {
            return s.Mid(n+1);
        } else {
          if ((idx+1) < m_nParamCount) {
              if (!IsSwitch(m_pszParams[idx+1])) {
                  return SString(m_pszParams[idx+1]);
              }
          }
        }
        //return szDefault;
    }
    return SString(szDefault);
}

int CommandLineParameters::GetSwitchInt(const char *sz,
                                          const int iDefault, /* = 0 */
                                          const BOOL fCase /* = FALSE */
                                         )
{
    char szDefault[25];
    sprintf(szDefault,"%d",iDefault);
    return atoi(GetSwitchStr(sz,szDefault,fCase));
}

SString CommandLineParameters::GetNonSwitchStr(
                                const BOOL bBreakAtSwitch, /* = TRUE */
                                const BOOL bFirstOnly /* = FALSE */)
{
    SString sLine = SString("");
    int i = 1;
    while (i < m_nParamCount) 
	{
        if (IsSwitch(m_pszParams[i])) 
		{
            if (bBreakAtSwitch) 
				break;
        } 
		else 
		{
            sLine += m_pszParams[i];
            if (bFirstOnly) break;
            sLine += " ";
        }
        i++;
    }
    sLine.TrimRight();
    return sLine;
}

}
