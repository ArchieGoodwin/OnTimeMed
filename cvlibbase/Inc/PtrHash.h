/*!
 * \file	PtrHash.h
 * \ingroup base
 * \brief	List�ɵ�˾ Hash�� ̩���� �㶮�ײ� �鼿�� �� �е�����˾ ��̩��.
 * \author	�����
 */

#ifndef _PTRHASH_H_
#define _PTRHASH_H_

#include "Object.h"

namespace CVLib
{
/**
 * @brief	Hash�� ��˦���� ����˾ �㶮�ױ� ̡�� �е���.
 */
class CVLIB_DECLSPEC PtrHash
{
public:

	/**
	 * @brief   ����е����� ���� ������ �����ٳ�.
	 *
	 * @param	nHashSize [in] : Hash��̩ �ͱ�
	 *
	 */
	PtrHash( int nHashSize );

	//!  �;˻� 
	virtual ~PtrHash();

	/**
	 * @brief   Hash��̩ �ͱ��� �б� ̡�� �ܺ��˳�.
	 *
	 * @return	Hash��̩ �ͱ�
	 *
	 * @par	Example:
	 * @code
	 *
	 *	PtrHash ptr = new PtrHash( 100 );
	 *
	 *  int nSize = ptr.GetHashSize();
	 *  assert(nSize == 100);
	 * 
	 * @endcode
	 *
	 */
	int GetHashSize();
	
	/**
	 * @brief	Hash�� �б� ̡�� �ܺ��˳�.
	 *
	 * @param	ptr [in] : ��̰�
	 * @return	Hash��
	 *
	 * @see	 AddKey(), RemoveKey(), GetKey()
	 *
	 */
	int GetHashValue(void* ptr);

	/**
	 * @brief   �Ѻϰ��� ���� ���ͱ������ ��˾ �в� �ܺ��˳�.
	 *
	 * @param	ptr [in] : ��˺�ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
	 * @return	NULL : �ͳ��� ���� �ֲ� ��˧ \n
	 *		    ���� : �Ѻϰ�
	 *
	 * @see	 GetHashValue()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	int nHashValue = 0;
	 *	PtrHash ptrHash = new PtrHash( 100 );
	 *
	 *	void* pHashKey1 = malloc(10);
	 *	nHashValue = ptrHash->AddKey( pHashKey1, ( void* )0x00da9b88 );
	 *
	 *	void *pPoint = ptrHash->GetKey( pHashKey1 );
	 *	assert( pPoint == ( void* )0x00da9b88 );
	 *	...
	 * @endcode
	 *
	 */
	void *GetKey(void *ptr);

	/**
	 * @brief  	���ͱ�����Ͷ� �����ײ� �ܺ��˳�.
	 *
	 * @param	ptr    [in] : �����׵ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
	 * @param	pValue [in] : �Ѻϰ��� �ඣ�� ��
	 * @return	Hash��
	 *
	 * @see	 RemoveKey(), GetHashValue()
	 *
	 * @par	Example:
	 * @code
	 *
	 *	int nHashValue = 0;
	 *	PtrHash ptrHash = new PtrHash( 100 );
	 *
	 *	void* pHashKey1 = malloc(10);
	 *	void* pHashKey2 = malloc(10);
	 *	void* pHashKey3 = malloc(10);
	 *
	 *	nHashValue = ptrHash->AddKey( pHashKey1, ( void* )0x00da9b88 );
	 *	nHashValue = ptrHash->AddKey( pHashKey2, ( void* )0x026d57a0 );
	 *	nHashValue = ptrHash->AddKey( pHashKey3, 0x00da9b88 );
	 *	...
	 *	ptrHash->RemoveKey( pHashKey1 );
	 *	ptrHash->RemoveKey( pHashKey2 );
	 *	ptrHash->RemoveKey( pHashKey3 );
	 *
	 * @endcode
	 *
	 */
	int AddKey(void *ptr, void *pValue);

	/**
	 * @brief  	���ͱ�����Ͷ� �����ײ� �ܺ��˳�.
	 *
	 * @param	ptr [in] : �����׵ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
	 * @return	NULL : �ͳ��� ���� �ֲ� ��˧ \n
	 *		    ���� : �Ѻϰ�, �°�
	 *
	 * @see	 AddKey(), GetHashValue()
	 *
	 */
	void *RemoveKey(void *ptr);

private:
	//! Hash��̩ �ͱ�
	int m_nHashSize;
	//! ����鼿�˹ɵ��� ���� Hash��̩ �輬��
	void** m_ppHashTable;
};

}

#endif // _PTRHASH_H_
