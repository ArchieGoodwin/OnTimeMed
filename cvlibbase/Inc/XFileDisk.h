
/*!
 * \file    XFileDisk.h
 * \ingroup base
 * \brief   ���μ���˾ �����ײ� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _XFILEDISK_H_
#define _XFILEDISK_H_

#include "XFile.h"

namespace CVLib
{

/**
 * @brief  ���μ���˾ ̡�� �е���
 */
class CVLIB_DECLSPEC XFileDisk : public XFile
{
public:
	//! �龢�� 
	XFileDisk(FILE* fp = NULL);
	//! �;˻� 
	~XFileDisk();

	/**
	 * @brief   ��������˾ �ⱨ�ٳ�. 
	 *
	 * @param   filename [in] :  �����˶� 
	 * @param   mode     [in] :  �ⱨ�һ� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  Close()
	 *
	 * @par Example:
	 * @code
	 *
	 *  XFileDisk xFile;
	 *  char szFileName[] = "c:/test1.bin";
	 *  assert( xFile.Open(szFileName, "wb") == TRUE );
	 * 
	 *  int nCount = 5;
	 *  int* pnData = new int[nCount];
	 *  for(int i = 0; i < nCount; i ++)
	 *      pnData[i] = i;
	 *
	 *  int nWrite = xFile.Write(pnData, sizeof(int), nCount);
	 * 	assert( nWrite == nCount );
	 * 	assert(xFile.Flush() == TRUE );
	 *  xFile.Close();
	 *
	 *  xFile.Open(szFileName, "rb");
	 *  assert( xFile.Size() == (long)sizeof(int) * nCount );
	 *
	 *  int nData[5];
	 *  xFile.Read(nData, sizeof(int), 3);
	 *  for(i = 0; i < 3; i ++)
	 *	    assert( nData[i] == i );
	 *
	 *  long  position = xFile.Tell();
	 *  assert( position == (sizeof(int) * 3) );
	 *
	 *  xFile.Seek(4L, SEEK_CUR);
	 *  xFile.Read(&(nData[4]), sizeof(int), 1);
	 *  assert( nData[4] == 4 );
	 *  assert( xFile.Eof() == FALSE );
	 * 	assert( xFile.Error() == 0 );
     *
     *  xFile.Close();
	 *  delete []pnData;
	 *  ...
	 *  @endcode
	 * 
	 */
	bool Open(const char *filename, const char *mode);

	/**
	 * @brief   ��������˾ �ᱨ�ٳ�.
	 *
	 * @return  �°��˷� true, ��²�˷� false 
	 *
	 * @see  Open()
	 *
	 */
	virtual bool Close();
  	
	/**
	 * @brief   �������εṢ�� ̰������ ���궦 ���̴��ͳ�.
	 *
	 * @param   buffer [in] : ̰�����Ѻ� 
	 * @param   size   [in] : �ϱ��ײ� ����̩ ������� 
	 * @param   count  [in] : �ϱ��ײ� ����̩ �״ݱ��� 
	 * @return  ���� �ϱ� �� ���Ϻ� 
	 *
	 * @see  Write(), Open()
	 *
	 */
	virtual int	Read(void *buffer, int size, int count);
	
	/**
	 * @brief   ���������� ̰����̩ ���궦 ȸ���ٳ�.
	 *
	 * @param   buffer [in] :  ̰�����Ѻ�
	 * @param   size   [in] :  ȸ���ײ� ����̩ ������� 
	 * @param   count  [in] :  ȸ���ײ� ����̩ �״ݱ��� 
	 * @return  ���� ȸ�� �� ���Ϻ� 
	 *
	 * @see  Read(), Open()
	 * 
	 */
	virtual int	Write(const void *buffer, int size, int count);
	
	/**
	 * @brief   ���μ輬���� �輳�� ̡����� �˴����ֳ�.
	 *
	 * @param   offset [in] : �˴����Եز� ������� 
	 * @param   origin [in] : ��̡�� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  Open()
	 *
	 */
	virtual bool Seek(long offset, int origin);
	
	/**
	 * @brief   ���μ輬��̩ ���̡���� �в� �ܺ��˳�.
	 *
	 * @return  ���̡�� 
	 *
	 * @see  Seek(), Open()
	 *
	 */
	virtual long Tell();
	
	/**
	 * @brief   ����̩ �ͱ��� �в� �ܺ��˳�.
	 *
	 * @return  ����̩ �ͱ� 
	 *
	 * @see  Seek(), Tell(), Open()
	 *
	 */
	virtual long	Size();
	
	/**
	 * @brief   ����˾ flush�ٳ�.
	 *
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  Open()
	 *
	 */
	virtual bool	Flush();
	
	/**
	 * @brief   ����̩ ��˾ �����ײ� �ܺ��˳�.
	 *
	 * @return  ����̩ ���˷� true, ���� ʭ���� false
	 *
	 * @see  Open()
	 *
	 */
	virtual bool	Eof();
	
	/**
	 * @brief   ��˱�� �����ٳ�.
	 *
	 * @return  ��˱�� ��˺�� 0, ��˺�� 1
	 *
	 * @see  Open()
	 *
	 */
	virtual long	Error();
	
	/**
	 * @brief   ���̼� �ͻ��� ������ ȸ���ٳ�.
	 *
	 * @param   c [in] : ȸ�ز� �ͻ� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 * @see  GetC()
	 *
	 * @par Example:
	 * @code
	 *
	 *	XFileDisk xFile;
	 *	char szFileName[] = "c:/test2.bin";
	 *	xFile.Open(szFileName, "wb");
	 *	
	 *	char str[] = "test";
	 *	char* szChar = str;
	 *	while( *szChar != 0 )
	 *  	xFile.PutC( *(szChar++) );
	 *  xFile.Close();
	 *  
	 *  xFile.Open(szFileName, "rb");endcode
	 *  assert( (char)xFile.GetC() == 't');
	 *  xFile.Close();
	 *  ...
	 * @endcode
	 *
	 */
	virtual bool PutC(unsigned char c);
	
	/**
	 * @brief   ���εṢ�� �ͻ��� ���̴��ͳ�.
	 *
	 * @return  ���̴��� �ͻ� 
	 *
	 * @see  PutC()
	 *
	 */
	virtual long	GetC();
protected:
	//! ���μ輬�� 
	FILE *m_fp;
	//! ������ ����ز����� �������� ���� 
	bool m_bCloseFile;
};

}

#endif //_XFILEDISK_H_