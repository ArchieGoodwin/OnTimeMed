
/*!
 * \file	cvlibbaseVersion.cpp
 * \ingroup cvlibbase
 * \brief   cvlibbaseº·°ÖÌ© ÁÆ¸õ±ã¶®¶¦ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.
 * \author  ¼³Ë¼Êé
 * \history	1.0.20	20110525
 * \history	1.1		20110523
 */

#include "cvlibbaseVersion.h"

#define CVLIBBASE_VERSION_MAJOR 1
#define CVLIBBASE_VERSION_MINOR 8
#define CVLIBBASE_VERSION_BUILD 2
#define CVLIBBASE_TITLE	"ChongBong cvlibbase 1.8.2"
#define CVLIBBASE_COPYRIGHT	"Copyright (C) 2007-2012 ChongBong"

namespace CVLib
{

void GetCVLIBBASEInfo(SLibraryInfo* pLibInfo)
{
	pLibInfo->nMajor = CVLIBBASE_VERSION_MAJOR;
	pLibInfo->nMinor = CVLIBBASE_VERSION_MINOR;
	pLibInfo->nBuild = CVLIBBASE_VERSION_BUILD;
	strcpy(pLibInfo->szTitle, CVLIBBASE_TITLE);
	strcpy(pLibInfo->szCopyright, CVLIBBASE_COPYRIGHT);
}

}