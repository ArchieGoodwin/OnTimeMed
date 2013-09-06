/*!
 * \file    XFile.h
 * \ingroup base
 * \brief   ���μ���˾ ̡�� ����е����� ��̩��.
 * \author  ����˧
 */

#if !defined(_XFILE_H_)
#define _XFILE_H_

#include "cvlibbasedef.h"

namespace CVLib
{
/**
 * @brief  ���μ���˾ ̡�� ����е���
 */
class CVLIB_DECLSPEC XFile
{
public:
	//! �龢�� 
	XFile(void) { };
	//! �;˻� 
	virtual ~XFile() { };

	/**
	 * @brief   ����˾ �ᱨ�ٳ�.
	 *
	 * @return  �°��˷� true, ��²�˷� false 
	 */
	virtual bool	Close() = 0;
	
	/**
	 * @brief   ���εṢ�� ̰������ ���궦 ���̴��ͳ�.
	 *
	 * @param   buffer [in] : ̰�����Ѻ� 
	 * @param   size   [in] : �ϱ��ײ� ����̩ ������� 
	 * @param   count  [in] : �ϱ��ײ� ����̩ �״ݱ��� 
	 * @return  ���� �ϱ� �� ���Ϻ� 
	 *
	 */
	virtual int		Read(void *buffer, int size, int count) = 0;

	/**
	 * @brief   ������ ̰����̩ ���궦 ȸ���ٳ�.
	 *
	 * @param   buffer [in] :  ̰�����Ѻ�
	 * @param   size   [in] :  ȸ���ײ� ����̩ ������� 
	 * @param   count  [in] :  ȸ���ײ� ����̩ �״ݱ��� 
	 * @return  ���� ȸ�� �� ���Ϻ� 
	 *
	 */
	virtual int		Write(const void *buffer, int size, int count) = 0;
	
	/**
	 * @brief   ���μ輬���� �輳�� ̡����� �˴����ֳ�.
	 *
	 * @param   offset [in] : �˴����Եز� ������� 
	 * @param   origin [in] : ��̡�� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 */
	virtual bool	Seek(long offset, int origin) = 0;
		
	/**
	 * @brief   ���μ輬��̩ ���̡���� �в� �ܺ��˳�.
	 *
	 * @return  ���̡�� 
	 *
	 */
	virtual long	Tell() = 0;
	
	/**
	 * @brief   ����̩ �ͱ��� �в� �ܺ��˳�.
	 *
	 * @return  ����̩ �ͱ� 
	 *
	 */
	virtual long	Size() = 0;
	
	/**
	 * @brief   ����˾ flush�ٳ�.
	 *
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 */
	virtual bool	Flush() = 0;
	
	/**
	 * @brief   ����̩ ��˾ �����ײ� �ܺ��˳�.
	 *
	 * @return  ����̩ ���˷� true, ���� ʭ���� false
	 *
	 */
	virtual bool	Eof() = 0;

	/**
	 * @brief   ��˱�� �����ٳ�.
	 *
	 * @return  ��˱�� ��˺�� 0, ��˺�� 1
	 *
	 */
	virtual long	Error() = 0;
	
	/**
	 * @brief   ���̼� �ͻ��� ������ ȸ���ٳ�.
	 *
	 * @param   c [in] : ȸ�ز� �ͻ� 
	 * @return  �°��˷� true, ��²�˷� false
	 *
	 */
	virtual bool	PutC(unsigned char c)
		{
		// Default implementation
		int nWrote = Write(&c, 1, 1);
		return (bool)(nWrote == 1);
		}

	/**
	 * @brief   ���εṢ�� �ͻ��� ���̴��ͳ�.
	 *
	 * @return  ���̴��� �ͻ� 
	 *
	 */
	virtual long	GetC() = 0;
};

}

#endif //_XFILE_H_
