
/*!
 * \file	Object.h
 * \ingroup base
 * \brief	���� �е���̩ ����е����� ��˦��� Object�е����� ��̩��.
 * \author	����˧
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "cvlibbaseDef.h"

namespace CVLib 
{

struct _CVLIB_POSITION { };
typedef _CVLIB_POSITION* CVLIB_POSITION;
#define CVLIB_BEFORE_START_POSITION ((CVLIB_POSITION)-1L)
	
/**
 * @brief	CVLib Library̩ ����е���.
 <pre> �� �е����� ���ͱ��ڳ�� �͸�̩ �ݶ���˾ �����ױ� ̡���� ���˴����. </pre>
 */
class CVLIB_DECLSPEC Object
{
public:
	// Construction and Destruction
	//! �龢��
	Object ();
	//! �;˻�
    virtual ~Object();

	/**
	 * @brief   �������εṢ�� �����ٳ�. 
	 *
	 * @param   szFilename [in] : ���ΰҵ�  
	 * @return  �°��׷� 1, ��²�׷� 0
	 *
	 * @see  ToFile(char* szFilename), FromFile(FILE* pFile)
	 *
	 */
	virtual int	FromFile (const char* szFilename);
	/**
	 * @brief   ���������� ����ٳ�. 
	 *
	 * @param   szFilename [in] : ���ΰҵ� 
	 * @return  �°��׷� 1, ��²�׷� 0
	 *
	 * @see  FromFile(char* szFilename), ToFile(FILE* pFile)
	 *
	 */
	virtual int	ToFile (const char* szFilename) const;
	/**
	 * @brief   �輳�� ���εṢ�� �����ٳ� 
	 *
	 * @param   pFile [in] : ���μ輬�� 
	 * @return  �°��׷� 1, ��²�׷� 0   
	 *
	 * @see  ToFile(FILE* pFile)
	 *
	 */
	virtual int	FromFile (FILE* pFile);
// 	virtual int FromFile (XFile* pFile);
	/**
	 * @brief   �輳�� ������ ����ٳ�. 
	 *
	 * @param   pFile [in]  ���μ輬�� 
	 * @return  �°��׷� 1, ��²�׷� 0
	 *
	 * @see  FromFile (FILE* pFile)
	 *
	 */
	virtual int	ToFile (FILE* pFile) const;
// 	virtual int ToFile (XFile* pFile) const;
	// new and delete operations
	/**
	 * @brief   ���ͱ��� �����ٳ�.
	 *
	 * @param   nSize [in] : ������ ���ͱ�̩ �ͱ� 
	 * @return  �ڳ�� ���ͱ��Ѻ�
	 *
	 */
	void* operator new(size_t nSize);

	/**
	 * @brief   ���ͱ��� �����ٳ�.
	 *
	 * @param   nSize      [in] : ������ ���ͱ�̩ �ͱ� 
	 * @param   szFilename [in] : ���ͱ��� ����� ̺������̩ �˶� 
	 * @param   nLine      [in] : ���ͱ��� ����� ̺������̩ �Ը��� 
	 * @return  �ڳ�� ���ͱ��Ѻ� 
	 *
	 */
	void* operator new(size_t nSize, char* szFilename, int nLine);
// 	void* operator new(size_t nSize, void* ptr);
// 	void* operator new(size_t nSize, void* ptr, char* szFilename, int nLine);

	/**
	 * @brief   ���ͱ��� �����ٳ�.
	 *
	 * @param   nSize [in] : ������ ���ͱ�̩ �ͱ� 
	 * @return  �ڳ�� ���ͱ��Ѻ�
	 *
	 */
	void* operator new[](size_t nSize);

	/**
	 * @brief   ���ͱ��� �����ٳ�.
	 *
	 * @param   nSize      [in] : ������ ���ͱ�̩ �ͱ� 
	 * @param   szFilename [in] : ���ͱ��� ����� ̺������̩ �˶� 
	 * @param   nLine      [in] : ���ͱ��� ����� ̺������̩ �Ը��� 
	 * @return  �ڳ�� ���ͱ��Ѻ� 
	 *
	 */
	void* operator new[](size_t nSize, char* szFilename, int nLine);

	/**
	 * @brief   ���ͱ��� �͸��ٳ�.
	 *
	 * @param   ptr [in] : �͸��� ���ͱ��Ѻ�
	 * @return  void
	 *
	 */
	void  operator delete(void *ptr);

	/**
	 * @brief   ���ͱ��� �͸��ٳ�.
	 *
	 * @param   ptr        [in] : �͸��� ���ͱ��Ѻ� 
	 * @param   szFilename [in] : ���ͱ��� ����� ̺������̩ �˶� 
	 * @param   nLine      [in] : ���ͱ��� ����� ̺������̩ �Ը��� 
	 * @return  void
	 *
	 */
	void  operator delete(void *ptr, char* szFilename, int nLine);
// 	void  operator delete(void *ptr, void *ptr1);
// 	void  operator delete(void *ptr, void *ptr1, char* szFilename, int nLine);

	/**
	 * @brief   ���ͱ��� �͸��ٳ�.
	 *
	 * @param   ptr [in] : �͸��� ���ͱ��Ѻ�
	 * @return  void
	 */
	void  operator delete[](void *ptr);

	/**
	 * @brief   ���ͱ��� �͸��ٳ�.
	 *
	 * @param   ptr        [in] : �͸��� ���ͱ��Ѻ� 
	 * @param   szFilename [in] : ���ͱ��� ����� ̺������̩ �˶� 
	 * @param   nLine      [in] : ���ͱ��� ����� ̺������̩ �Ը��� 
	 * @return  void
	 *
	 */
	void  operator delete[](void *ptr, char* szFilename, int nLine);

};

}

#endif //_OBJECT_H_
