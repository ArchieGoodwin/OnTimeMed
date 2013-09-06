
/*!
 * \file    XFileMem.h
 * \ingroup base
 * \brief   ���μ���˾ �����ײ� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _XFILEMEM_H_
#define _XFILEMEM_H_

#include "XFile.h"
#include "cvlibbasedef.h"

namespace CVLib
{

/**
 * @brief  ���μ��� ̡�� �е���
 */
class CVLIB_DECLSPEC XFileMem : public XFile
{
public:
	/**
	 * @brief   �е����� �����ٳ�.
	 *
	 * @param   pBuffer [in] : ����˾ ̡�� ���ͱ��Ѻ� 
	 * @param   size    [in] : ���οͱ� 
	 *
	 */
	 XFileMem(BYTE* pBuffer = NULL, DWORD size = 0);
	 
	//! �;˻� 
	~XFileMem();

	/**
	 * @brief   ����˾ ̡�� ���ͱ��� �ۼ��ٳ�.
	 *
	 * @return  ������ �ۼ�����˺�� true, ʭ���� false
	 *
	 * @see  Close()
	 *
	 * @par Example:
	 * @code
	 *
	 *  BYTE* pBuffer = new BYTE[1000];
	 *  XFileMem xFile(pBuffer);	
	 *  xFile.Open();
	 * 
	 *  int nCount = 5;
	 *  int* pnData = new int[nCount];
	 *  for(int i = 0; i < nCount; i ++)
	 *  	pnData[i] = i;
	 *
	 *  int nWrite = xFile.Write(pnData, sizeof(int), nCount);
	 * 	assert( nWrite == nCount );
	 *  assert(xFile.Flush() == TRUE );
	 *  assert( xFile.Size() == (long)(sizeof(int) * nCount) );
	 *  
	 *  xFile.Seek(0, SEEK_SET);
	 *  int nData[5];
	 *  xFile.Read(nData, sizeof(int), 3);
	 *  for(i = 0; i < 3; i ++)
	 *  	assert( nData[i] == i );
	 *	
	 *  long  position = xFile.Tell();
	 *  assert( position == (sizeof(int) * 3) );
	 *  
	 *  xFile.Seek(4L, SEEK_CUR);
	 *  xFile.Read(&(nData[4]), sizeof(int), 1);
	 *  assert( nData[4] == 4 );
	 *  assert( xFile.Eof() == TRUE );
	 *  assert( xFile.Error() == 0 );	
	 * 
	 *  assert ( xFile.GetBuffer() == pBuffer );
     *  xFile.Close();
     *  delete []pnData;
	 *  ...
	 *  @endcode
	 * 
	 */
	bool Open();

	/**
	 * @brief   ���α��ͱ�̩ �Ѻ϶� �в���. 
	 *
	 * @param   bDetachBuffer [in] : 
	 * @return  ���μѺ� 
	 *
	 * @see  open()
	 *
	 */
	BYTE* GetBuffer(bool bDetachBuffer = true);

	/**
	 * @brief   ����˾ ̡�� ����� ���ͱ��� �͸��ٳ�.
	 *
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  open()
	 * 
	 */
	virtual bool	Close();

	/**
	 * @brief   ���α��ͱ��Ṣ�� ̰������� ���궦 ���̴��ͳ�.
	 *
	 * @param   buffer [in] : ̰�����Ѻ� 
	 * @param   size   [in] : �ϱ��ײ� ����̩ ������� 
	 * @param   count  [in] : �ϱ��ײ� ����̩ �״ݺ� 
	 * @return  ���� �ϱ� ��  ���Ϻ� 
	 *
	 * @see  Write(), open(), XFileDisk::Read()
	 *
	 */
	virtual int	Read(void *buffer, int size, int count);

	/**
	 * @brief   ̰����̩ ���궦 ������ ȸ���ٳ�.
	 *
	 * @param   buffer [in] : ̰�����Ѻ� 
	 * @param   size   [in] : ȸ���ײ� ����̩ �������  
	 * @param   count  [in] : ȸ���ײ� ����̩ �״ݺ� 
	 * @return  ���� ȸ�� �� ���Ϻ� 
	 *
	 * @see  Read(), open(), XFileDisk::Write()
	 *
	 */
	virtual int	Write(const void *buffer, int size, int count);

	/**
	 * @brief   ���γ����輬���� �輳�� ̡����� �˴����ֳ�.
	 *
	 * @param   offset [in] : �˴����Եز� ������� 
	 * @param   origin [in] : ��̡�� 
	 * @return  �°��˷� true, ��²�˷� false 
	 *
	 * @see  open()
	 *
	 */
	virtual bool	Seek(long offset, int origin);

	/**
	 * @brief   ���γ����輬��̩ ���̡���� �в���.
	 * 
	 * @return  ���̡�� 
	 *
	 * @see  Seek(), open()
	 *
	 */
	virtual long	Tell();

	/**
	 * @brief   ���α��ͱ�̩ �ͱ��� �в���.
	 *
	 * @return  ���οͱ�  
	 *
	 * @see  Seek(), Tell(), open()
	 *
	 */
	virtual long	Size();

	/**
	 * @brief   ���α��ͱ��� flush�ٳ�.
	 *
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  open()
	 *
	 */
	virtual bool	Flush();

	/**
	 * @brief   ���α��ͱ�̩ ��˾ �����ײ� �ܺ��˳�.
	 *
	 * @return  ���˷� true, ʭ���� false
	 *
	 * @see  open()
	 *
	 */
	virtual bool	Eof();

	/**
	 * @brief   ���α��ͱ���̩ ��˱�� �����ٳ�.
	 *
	 * @return  ��˱�� ��˺�� 0, ��˺�� 1
	 *
	 * @see  open()
	 *
	 */
	virtual long	Error();

	/**
	 * @brief   ���̼� �ͻ��� ���α��ͱ��� ȸ���ٳ�.
	 *
	 * @param   c [in] : ȸ�ز� �ͻ� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  GetC()
	 *
	 * @par Example:
	 * @code
	 *
	 *	BYTE* pBuffer = new BYTE[1000];
	 *	XFileMem xFile(pBuffer);
	 *	xFile.Open();
	 *	
	 *	char str[] = "test";
	 *	char* szChar = str;
	 *	while( *szChar != 0 )
	 *  	xFile.PutC( *(szChar++) );
	 *  
	 *  xFile.Seek(0, SEEK_SET);
	 *  assert( (char)xFile.GetC() == 't');
	 *  assert( (char)xFile.GetC() == 'e');
	 *  assert( (char)xFile.GetC() == 's');
     *
	 *  xFile.Close();
	 * ...
	 * @endcode
	 * 
	 */
	virtual bool	PutC(unsigned char c);

	/**
	 * @brief   ���α��ͱ��Ṣ�� �ͻ��� ���̴��ͳ�.
	 *
	 * @return  ���̴��� �ͻ� 
	 *
	 * @see  PutC()
	 *
	 */
	virtual long	GetC();

protected:
	void	Alloc(DWORD nBytes);
	void	Free();

	//!  ���α��ͱ�̩ �Ѻ� 
	BYTE*	m_pBuffer;
	//!  ���α��ͱ�̩ �ͱ� 
	DWORD	m_Size;
	//!  ������ ����ز����� �������� ���� 
	bool	m_bFreeOnClose;
	//!  ���γ����輬��̩ ���̡�� 
	long	m_Position;
	//!  ���α��ͱ�̩ �״ݿͱ�    
	long	m_Edge;	
};

}

#endif //_XFILEMEM_H_
