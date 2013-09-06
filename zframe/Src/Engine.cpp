#include "Engine.h"
#include <assert.h>
#include "cvlibbase/inc/timer.h"
#include "cvlibbase/inc/cvlibMalloc.h"
#include "cvlibbase/inc/cvlibUtil.h"
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

namespace CVLib {
	
Engine::Engine()
{ 
	m_pFile = NULL;
}

Engine::~Engine()
{
	if (m_pFile)		fclose(m_pFile);
}

void Engine::SetLogFile (char* szFileName)
{
	if (szFileName != NULL)
	{
		CVUtil::MkDir(szFileName);
		if (m_pFile)
			fclose (m_pFile);
		m_pFile = fopen (szFileName, "a+");
	}
}

void Engine::TransferStatus (char* szStatus)
{
	static char szLF[3] = {0x0d, 0x0a, 0x0};
	assert (szStatus != NULL);
	char* szTemp = NULL;

	if (m_pFile)
	{
		MALLOC (szTemp, strlen(szStatus) + 3, char);
		szTemp[0] = 0;
		strcpy (szTemp, szStatus);
		strcat (szTemp, szLF);
		fprintf (m_pFile, "%s", szTemp);
		FREE (szTemp);
		fflush (m_pFile);
	}
	else
	{
		printf ("%s\n", szStatus);
	}
}

void Engine::info(char* fmt, ...)
{
	va_list ap;
	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);
}

void Engine::infoflush()
{
//	fflush(stdout);
}

MachineABC*	Engine::Train (DataSet* pDataSet) 
{return NULL;}

}