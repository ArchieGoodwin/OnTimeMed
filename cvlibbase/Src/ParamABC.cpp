/*!
 * \file	ParamABC.cpp
 * \ingroup base
 * \brief	���� �ĵ������е���̩ ����е����� ��˦��� �е����� ��̩�ٳ�.
 * \author	����˧
 */

#include "ParamABC.h"
#include "IniFile.h"

namespace CVLib 
{

ParamABC::ParamABC()
{
}

ParamABC::~ParamABC()
{
}

int	ParamABC::Load (const char* szFilename)
{
	IniFile iniFile;
	iniFile.Path(SString(szFilename));
	if (!iniFile.ReadFile())
		return 0;
	return Load (&iniFile);
}

int	ParamABC::Save (const char* szFilename)
{
	IniFile iniFile;
	iniFile.Path(SString(szFilename));
	if (!ParamABC::Save (&iniFile))
		return 0;
	if (!iniFile.WriteFile())
		return 0;
	return 1;
}

}
