#include "XFileMem.h"

namespace CVLib
{

XFileMem::XFileMem(BYTE* pBuffer, DWORD size)
{
	m_pBuffer = pBuffer;
	m_Position = 0;
	m_Size = m_Edge = size;
	m_bFreeOnClose = (bool)(pBuffer==0);
}

XFileMem::~XFileMem()
{
	Close();
}

bool XFileMem::Close()
{
	if ( (m_pBuffer) && (m_bFreeOnClose) ){
		free(m_pBuffer);
		m_pBuffer = NULL;
		m_Size = 0;
	}
	return true;
}

bool XFileMem::Open()
{
	if (m_pBuffer) return false;	// Can't re-open without closing first

	m_Position = m_Size = m_Edge = 0;
	m_pBuffer=(BYTE*)malloc(1);
	m_bFreeOnClose = true;

	return (m_pBuffer!=0);
}

BYTE* XFileMem::GetBuffer(bool bDetachBuffer)
{
	m_bFreeOnClose = !bDetachBuffer;
	return m_pBuffer;
}

int XFileMem::Read(void *buffer, int size, int count)
{
	if (buffer==NULL) return 0;

	if (m_pBuffer==NULL) return 0;
	if (m_Position >= (long)m_Size) return 0;

	long nCount = (long)(count*size);
	if (nCount == 0) return 0;

	long nRead;
	if (m_Position + nCount > (long)m_Size)
		nRead = (m_Size - m_Position);
	else
		nRead = nCount;

	memcpy(buffer, m_pBuffer + m_Position, nRead);
	m_Position += nRead;

	return (int)(nRead/size);
}

int XFileMem::Write(const void *buffer, int size, int count)
{
	if (m_pBuffer==NULL) return 0;
	if (buffer==NULL) return 0;

	long nCount = (long)(count*size);
	if (nCount == 0) return 0;

	if (m_Position + nCount > m_Edge) Alloc(m_Position + nCount);

	memcpy(m_pBuffer + m_Position, buffer, nCount);

	m_Position += nCount;

	if (m_Position > (long)m_Size) m_Size = m_Position;
	
	return count;
}

bool XFileMem::Seek(long offset, int origin)
{
	if (m_pBuffer==NULL) return false;
	long lNewPos = m_Position;

	if (origin == SEEK_SET)		 lNewPos = offset;
	else if (origin == SEEK_CUR) lNewPos += offset;
	else if (origin == SEEK_END) lNewPos = m_Size + offset;
	else return false;

	if (lNewPos < 0) lNewPos = 0;

	m_Position = lNewPos;
	return true;
}

long XFileMem::Tell()
{
	if (m_pBuffer==NULL) return -1;
	return m_Position;
}

long XFileMem::Size()
{
	if (m_pBuffer==NULL) return -1;
	return m_Size;
}

bool XFileMem::Flush()
{
	if (m_pBuffer==NULL) return false;
	return true;
}

bool XFileMem::Eof()
{
	if (m_pBuffer==NULL) return true;
	return (m_Position >= (long)m_Size);
}

long XFileMem::Error()
{
	if (m_pBuffer==NULL) return -1;
	return (m_Position > (long)m_Size);
}

bool XFileMem::PutC(unsigned char c)
{
	if (m_pBuffer==NULL) return false;
	if (m_Position + 1 > m_Edge) Alloc(m_Position + 1);

	memcpy(m_pBuffer + m_Position, &c, 1);

	m_Position += 1;

	if (m_Position > (long)m_Size) m_Size = m_Position;
	
	return true;
}

long XFileMem::GetC()
{
	if (Eof()) return EOF;
	return *(BYTE*)((BYTE*)m_pBuffer + m_Position++);
}

void XFileMem::Alloc(DWORD dwNewLen)
{
	if (dwNewLen > (DWORD)m_Edge)
	{
		// find new buffer size
		DWORD dwNewBufferSize = (DWORD)(((dwNewLen>>12)+1)<<12);

		// allocate new buffer
		if (m_pBuffer == NULL) m_pBuffer = (BYTE*)malloc(dwNewBufferSize);
		else	m_pBuffer = (BYTE*)realloc(m_pBuffer, dwNewBufferSize);
		// I own this buffer now (caller knows nothing about it)
		m_bFreeOnClose = true;

		m_Edge = dwNewBufferSize;
	}
	return;
}

void XFileMem::Free()
{
	Close();
}

}