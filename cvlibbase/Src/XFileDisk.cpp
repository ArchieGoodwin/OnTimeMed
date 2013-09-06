#include "XFileDisk.h"

namespace CVLib
{

XFileDisk::XFileDisk(FILE* fp /*= NULL*/)
{
	m_fp = fp;
	m_bCloseFile = (bool)(fp==0);
}

XFileDisk::~XFileDisk()
{
	Close();
}

bool XFileDisk::Open(const char *filename, const char *mode)
{
	if (m_fp) return false;	// Can't re-open without closing first

	m_fp = fopen(filename, mode);
	if (!m_fp) return false;

	m_bCloseFile = true;

	return true;
}

bool XFileDisk::Close()
{
	int iErr = 0;
	if ( (m_fp) && (m_bCloseFile) ){ 
		iErr = fclose(m_fp);
		m_fp = NULL;
	}
	return (bool)(iErr==0);
}

int	XFileDisk::Read(void *buffer, int size, int count)
{
	if (!m_fp) return 0;
	return fread(buffer, size, count, m_fp);
}

int	XFileDisk::Write(const void *buffer, int size, int count)
{
	if (!m_fp) return 0;
	return fwrite(buffer, size, count, m_fp);
}

bool XFileDisk::Seek(long offset, int origin)
{
	if (!m_fp) return false;
	return (bool)(fseek(m_fp, offset, origin) == 0);
}

long XFileDisk::Tell()
{
	if (!m_fp) return 0;
	return ftell(m_fp);
}

long XFileDisk::Size()
{
	if (!m_fp) return -1;
	long pos,size;
	pos = ftell(m_fp);
	fseek(m_fp, 0, SEEK_END);
	size = ftell(m_fp);
	fseek(m_fp, pos,SEEK_SET);
	return size;
}

bool XFileDisk::Flush()
{
	if (!m_fp) return false;
	return (bool)(fflush(m_fp) == 0);
}

bool XFileDisk::Eof()
{
	if (!m_fp) return true;
	return (bool)(feof(m_fp) != 0);
}

long XFileDisk::Error()
{
	if (!m_fp) return -1;
	return ferror(m_fp);
}

bool XFileDisk::PutC(unsigned char c)
{
	if (!m_fp) return false;
	return (bool)(fputc(c, m_fp) == c);
}

long XFileDisk::GetC()
{
	if (!m_fp) return EOF;
	return getc(m_fp);
}

}
