/*!
 * \file	PtrHash.cpp
 * \ingroup base
 * \brief	List�ɵ�˾ Hash�� ̩���� �㶮�ײ� �ܺ��˾ ��̩�ٳ�.
 * \author	�����
 */

#include "ptrhash.h"

namespace CVLib
{

#if !defined LOWORD
#define LOWORD(l)			((WORD)(l))
#endif //LOWORD

#if !defined HIWORD
#define HIWORD(l)			((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif //HIWORD

#if !defined LOBYTE
#define LOBYTE(w)			((BYTE)(w))
#endif //LOBYTE

#if !defined HIBYTE
#define HIBYTE(w)			((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif //HIBYTE

/**
 * @brief		���ͱ�������� ���� ���� �鼿��.
 */
struct SMemHash
{
	SMemHash *m_pPrev;		/**< �̷籨�ͱ�������� ���� �輬�� */
	SMemHash *m_pNext;		/**< �������ͱ�������� ���� �輬�� */
	void *m_ptr;		/**< �Ѻϰ� */
	void *m_pValue;		/**< �Ѻϰ��� �ඣ�� ����� ���ͱ�̩ �Ѻ� */

	/**
	 * @brief	���ͱ�����Ͱ鼿���� ���� ������ �����ٳ�.
	 */
	SMemHash(){ m_pPrev = m_pNext = NULL; m_ptr = m_pValue = NULL; }
};

/**
 * @brief		���ͱ�����Ͷ� �㶮�ױ� ̡�� ������ ���� �鼿��.
 */
struct SMemHashList
{
	int m_nCount;		/**< ������ ������ �����̩ ���� */
	SMemHash *m_pHead;		/**< ����̩ �� �����̩ �Ѻ� */
	SMemHash *m_pTail;		/**< ����̩ ���칹���̩ �Ѻ� */

	SMemHashList();
	bool Add( SMemHash *pMem );
	void *Remove( void *ptr );
	void *Get( void *ptr );
	SMemHash *Find( void *ptr );

};

/**
 * @brief	����鼿���� ���� ������ �����ٳ�.
 */
SMemHashList::SMemHashList()
{
	m_nCount = 0;
	m_pHead = m_pTail = NULL;
}

/**
 * @brief	���ͱ������ �����ܺ�
 * @param	pMem [in] : �����׵ز� ���ͱ�������� ���� �輬��
 * @return	(bool)  \n
 *		true : �°�, false : ��²
 */
bool SMemHashList::Add( SMemHash *pMem )
{
	bool bResult = false;
	if ( pMem )
	{
		if ( m_pHead == NULL )
			m_pHead = m_pTail = pMem;
		else
		{
			// �� ���ͱ��� �����ײ� ��˧ Tail�� ���� ������ �ٳ�.
			m_pTail->m_pNext = pMem;
			pMem->m_pPrev = m_pTail;
			m_pTail = pMem;
		}
		m_nCount++;
		bResult = true;
	}
	return bResult;
}

/**
 * @brief	�Ѻϰ��� ���� ���ͱ������ �б��ܺ�
 * @param	ptr [in] : ��˺�ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
 * @return	(SMemHash*) \n
 *		NULL : �ͳ��� ���� �ֲ� ��˧  \n
 *		���� : ���ͱ������̩ �Ѻϰ�
 */
SMemHash *SMemHashList::Find( void *ptr )
{
	SMemHash *pMem = NULL;
	if ( ptr )
	{
		SMemHash *pNext = m_pHead;
		while ( pNext )
		{
			if ( ptr == pNext->m_ptr )
			{
				pMem = pNext;
				break;
			}
			pNext = pNext->m_pNext;
		}
	}
	return pMem;
}

/**
 * @brief	�Ѻϰ��� ���� ���ͱ������ ���б��ܺ�
 * @param	ptr [in] : ��˺�ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
 * @return	(void*) \n
 *		NULL : �ͳ��� ���� �ֲ� ��˧ \n
 *		���� : �Ѻϰ�
 */
void *SMemHashList::Get( void *ptr )
{
	void *ptrResult = NULL;
	if ( ptr )
	{
		SMemHash *pNext = Find( ptr );
		if ( pNext )
			ptrResult = pNext->m_pValue;
	}
	return ptrResult;
}

/**
 * @brief	���ͱ������ �����ܺ�
 * @param	ptr [in] : �����׵ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
 * @return	(void*) \n
 *		NULL : �ͳ��� ���� �ֲ� ��˧ \n
 *		���� : �Ѻϰ�, �°�
 */
void *SMemHashList::Remove( void *ptr )
{
	void *ptrResult = NULL;
	if ( ptr )
	{
		SMemHash *pMem = Find( ptr );
		if ( pMem )
		{
			ptrResult = pMem->m_pValue;

			// �����׵ز� ���ͱ��� Head̮ ��˼ ��˧
			if ( pMem == m_pHead )
			{
				// ���ͱ��� �ױ��� �ֲ� ��˧
				if ( pMem->m_pNext == NULL )
				{
					m_pHead = m_pTail = NULL;
				}
				else
				{
					m_pHead = pMem->m_pNext;
					m_pHead->m_pPrev = NULL;
				}
			}
			// �����׵ز� ���ͱ��� Tail�� ��˼ ��˧
			else if ( pMem == m_pTail )
			{
				m_pTail = pMem->m_pPrev;
				m_pTail->m_pNext = NULL;
			}
			else // ���� �θɼ��� ��˧
			{
				pMem->m_pNext->m_pPrev = pMem->m_pPrev;
				pMem->m_pPrev->m_pNext = pMem->m_pNext;
			}
		
			delete pMem;
			m_nCount--;
		}
	}
	return ptrResult;
}

PtrHash::PtrHash( int nHashSize )
{
	m_nHashSize = nHashSize;
	m_ppHashTable = (void**)new SMemHashList* [ m_nHashSize ];
	memset( ((SMemHashList**)m_ppHashTable), 0, sizeof( SMemHashList* ) * m_nHashSize );
}

PtrHash::~PtrHash()
{
	if ( ((SMemHashList**)m_ppHashTable) )
	{
		for ( int nIdx = 0; nIdx < m_nHashSize; nIdx++ )
		{
			SMemHashList *pList = ((SMemHashList**)m_ppHashTable)[ nIdx ];
			if ( pList )
			{
				if ( pList->m_pHead )
				{
					// ���ͱ��ͽ��� Tail�Ṣ�� �����ٳ�.
					SMemHash *pPrev = pList->m_pTail;
					while ( pPrev )
					{
						SMemHash *pTemp = pPrev;
						pPrev = pPrev->m_pPrev;
						delete pTemp;
						pTemp = NULL;
					}
				}
				delete pList;
			}
		}
		delete [] ((SMemHashList**)m_ppHashTable);
	}
	m_ppHashTable = NULL;
}

int PtrHash::GetHashValue(void *ptr)
{
	unsigned int nHashValue = 0;
	unsigned int nValue = ( unsigned int )ptr;
	unsigned int nCode = 0;

	// nHashValue = Sigma( byte(i) * byte(i) )
	nCode = (int)HIBYTE( HIWORD( nValue ) );
	nHashValue += nCode * nCode;

	nCode = LOBYTE( HIWORD( nValue ) );
	nHashValue += nCode * nCode;

	nCode = HIBYTE( LOWORD( nValue ) );
	nHashValue += nCode * nCode;

	nCode = LOBYTE( LOWORD( nValue ) );
	nHashValue += nCode * nCode;

	nHashValue %= m_nHashSize;
	return nHashValue;
}

int PtrHash::AddKey( void *ptr, void *pValue )
{
	if ( m_ppHashTable == NULL )
		return 0;

	int nHashValue = GetHashValue( ptr );

	SMemHashList *pList = NULL;
	SMemHash *pMem = new SMemHash;
	pMem->m_ptr = ptr;
	pMem->m_pValue = pValue;

	if ( m_ppHashTable[ nHashValue ] == NULL )
	{
		pList = new SMemHashList;
		((SMemHashList**)m_ppHashTable)[ nHashValue ] = pList;
	}
	else
		pList = ((SMemHashList**)m_ppHashTable)[ nHashValue ];

	pList->Add( pMem );

	return nHashValue;
}

void *PtrHash::RemoveKey( void *ptr )
{
	if ( m_ppHashTable == NULL )
		return 0;
	
	int nHashValue = GetHashValue( ptr );
	void *ptrResult = NULL;

	SMemHashList *pList = ((SMemHashList**)m_ppHashTable)[ nHashValue ];
	if ( pList )
		ptrResult = pList->Remove( ptr );

	return ptrResult;
}

void *PtrHash::GetKey( void *ptr )
{
	if ( m_ppHashTable == NULL )
		return 0;
	
	void *pPoint = NULL;
	int nHashValue = GetHashValue( ptr );
	SMemHashList *pList = ((SMemHashList**)m_ppHashTable)[ nHashValue ];

	if ( pList )
	{
		SMemHash *pNext = pList->m_pHead;
		while ( pNext )
		{
			if ( ptr == pNext->m_ptr )
			{
				pPoint = pNext->m_pValue;
				break;
			}
			pNext = pNext->m_pNext;
		}
	}

	return pPoint;
}

int PtrHash::GetHashSize()
{
	return m_nHashSize;
}

}
