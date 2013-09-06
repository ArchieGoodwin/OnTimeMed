
/*!
 * \file	cvlibmalloc.cpp
 * \ingroup base
 * \brief	CVLib����̩ ���ͱ��� �㶮�ײ� �ܺ��˾ ��̩�ٳ�.
 * \author	����˧
 */

#include "cvlibMalloc.h"
#include "cvlibmacros.h"

namespace CVLib
{

/**
 * @brief	���ͱ�������� ���� ���� �鼿��.
 */
struct SMemblockNode
{
    void	*ptr;						/**< ����� ���ͱ� �Ѻ� */
	int		objecttype;					/**< ���ͱ�̩ ���� */
	int		len;						/**< ���ͱ�̩ ���� */
	char	filename[300];				/**< ���ͱ��� ����� �����˶� */
	int		line;						/**< ���ͱ��� ����� ̺�翸��̩ �Ը��� */
    struct	SMemblockNode *prev;		/**< �˼� ���ͱ�����ͷ���̩ �Ѻ� */
    struct	SMemblockNode *next;		/**< ���� ���ͱ�����ͷ���̩ �Ѻ� */
};

//!	�͸Ҵ�� ʱ˼ ���ͱ��� ������ ̺�����α� �Ը����� ����ײ��� ��˦��� ��������̩ �˶�.
static char mszMemleakFilename[] = "c:/memoryleak.txt";

//! ���� ���ͱ�����ͷ���.
static SMemblockNode* mpMemBlocks;

//! �״ݵ� ��˦��� ���ͱ��ͱ�.
static size_t	mnMaxSize;

//! ���̩ ��˦��� ���ͱ��ͱ�.
static size_t	mnCurSize;


//! Hash��̩ �ͱ�.
static const int mnHashSize = 4999;

//! ��������.
enum MMS_FLAG
{
	MMS_None = 0,
	MMS_Init = 1,
};

//! ���ͱ��㶮��������.
static int		mnMemManagerState = MMS_None;

static int		mnLock = 0;
/**
 * @brief	Hash�� ��˦���� ����˾ �㶮�ױ� ̡�� �е���.
 */
struct MemHash
{
	/**
	 * @brief	����е����� ���� ������ �����ٳ�.
	 * @param	nHashSize [in] : Hash��̩ �ͱ�
	 */
	void Create( int nHashSize );
	/**
	 * @brief	����е����� ���� �ͽ��� �����ٳ�.
	 */
	void Release();

	/**
	 * @brief	Hash��̩ �ͱ� �б��ܺ�
	 * @param	����
	 * @return	(int) \n
	 *		Hash��̩ �ͱ�
	 */
	int GetHashSize();
	
	/**
	 * @brief	Hash�� �б��ܺ�
	 * @param	ptr [in] : ��̰�
	 * @return	(int)  \n
	 *		Hash��
	 *
	 * @see	MemHash::AddKey(), MemHash::RemoveKey(), MemHash::GetKey()
	 */
	int GetHashValue( void* ptr );

	/**
	 * @brief	�Ѻϰ��� ���� ���ͱ������ ���б��ܺ�
	 * @param	ptr [in] : ��˺�ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
	 * @return	(void*) \n
	 *		NULL : �ͳ��� ���� �ֲ� ��˧ \n
	 *		���� : �Ѻϰ�
	 *
	 * @see	MemHash::GetHashValue()
	 * @par	Example:
	 * @code
	 *
	 *	int nHashValue = 0;
	 *	MemHash ptrHash = new MemHash( 100 );
	 *
	 *	void* pHashKey1 = malloc(10);
	 *	nHashValue = ptrHash->AddKey( pHashKey1, ( void* )0x00da9b88 );
	 *
	 *	void *pPoint = ptrHash->GetKey( pHashKey1 );
	 *	ASSERT( pPoint == ( void* )0x00da9b88 );
	 *	...
	 *
	 * @endcode
	 */
	void *GetKey(void *ptr);

	/**
	 * @brief	���ͱ������ �����ܺ�
	 * @param	ptr [in] : �����׵ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
	 * @param	pValue [in] : �Ѻϰ��� �ඣ�� ��
	 * @return	(int)  \n
	 *		Hash��
	 *
	 * @see	MemHash::RemoveKey(), MemHash::GetHashValue()
	 * @par	Example:
	 * @code
	 *
	 *	int nHashValue = 0;
	 *	MemHash ptrHash = new MemHash( 100 );
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
	 */
	int AddKey( void *ptr, void *pValue );

	/**
	 * @brief	���ͱ������ �����ܺ�
	 * @param	ptr [in] : �����׵ز� ���ͱ�����Ͱ� ����� �ز� �Ѻϰ�
	 * @return	(void*)  \n
	 *		NULL : �ͳ��� ���� �ֲ� ��˧ \n
	 *		���� : �Ѻϰ�, �°�
	 *
	 * @see	MemHash::AddKey(), MemHash::GetHashValue()
	 */
	void *RemoveKey( void *ptr );

	//! Hash��̩ �ͱ�
	int m_nHashSize;
	//! ����鼿�˹ɵ��� ���� Hash��̩ �輬��
	void** m_ppHashTable;
};
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
struct SMem
{
	SMem *m_pPrev;		/**< �̷籨�ͱ�������� ���� �輬�� */
	SMem *m_pNext;		/**< �������ͱ�������� ���� �輬�� */
	void *m_ptr;		/**< �Ѻϰ� */
	void *m_pValue;		/**< �Ѻϰ��� �ඣ�� ����� ���ͱ�̩ �Ѻ� */

	/**
	 * @brief	���ͱ�����Ͱ鼿���� ���� ������ �����ٳ�.
	 */
	SMem(){ m_pPrev = m_pNext = NULL; m_ptr = m_pValue = NULL; }
};

/**
 * @brief		���ͱ�����Ͷ� �㶮�ױ� ̡�� ������ ���� �鼿��.
 */
struct SMemList
{
	int m_nCount;		/**< ������ ������ �����̩ ���� */
	SMem *m_pHead;		/**< ����̩ �� �����̩ �Ѻ� */
	SMem *m_pTail;		/**< ����̩ ���칹���̩ �Ѻ� */

	SMemList();
	bool Add( SMem *pMem );
	void *Remove( void *ptr );
	void *Get( void *ptr );
	SMem *Find( void *ptr );

};

/**
 * @brief	����鼿���� ���� ������ �����ٳ�.
 */
SMemList::SMemList()
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
bool SMemList::Add( SMem *pMem )
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
 * @return	(SMem*) \n
 *		NULL : �ͳ��� ���� �ֲ� ��˧  \n
 *		���� : ���ͱ������̩ �Ѻϰ�
 */
SMem *SMemList::Find( void *ptr )
{
	SMem *pMem = NULL;
	if ( ptr )
	{
		SMem *pNext = m_pHead;
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
void *SMemList::Get( void *ptr )
{
	void *ptrResult = NULL;
	if ( ptr )
	{
		SMem *pNext = Find( ptr );
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
void *SMemList::Remove( void *ptr )
{
	void *ptrResult = NULL;
	if ( ptr )
	{
		SMem *pMem = Find( ptr );
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

void MemHash::Create ( int nHashSize )
{
	m_nHashSize = nHashSize;
	m_ppHashTable = (void**)malloc (sizeof(SMemList*) * m_nHashSize);
	memset( ((SMemList**)m_ppHashTable), 0, sizeof( SMemList* ) * m_nHashSize );
}

void MemHash::Release()
{
	if ( ((SMemList**)m_ppHashTable) )
	{
		for ( int nIdx = 0; nIdx < m_nHashSize; nIdx++ )
		{
			SMemList *pList = ((SMemList**)m_ppHashTable)[ nIdx ];
			if ( pList )
			{
				if ( pList->m_pHead )
				{
					// ���ͱ��ͽ��� Tail�Ṣ�� �����ٳ�.
					SMem *pPrev = pList->m_pTail;
					while ( pPrev )
					{
						SMem *pTemp = pPrev;
						pPrev = pPrev->m_pPrev;
//						free(pTemp);
						delete pTemp;
						pTemp = NULL;
					}
				}
				delete pList;
			}
		}
		free ((SMemList**)m_ppHashTable);
	}
	m_ppHashTable = NULL;
}

int MemHash::GetHashValue(void *ptr)
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

int MemHash::AddKey( void *ptr, void *pValue )
{
	if ( m_ppHashTable == NULL )
		return 0;

	int nHashValue = GetHashValue( ptr );

	SMemList *pList = NULL;
	SMem* pMem = new SMem;
	pMem->m_pPrev = NULL;
	pMem->m_pNext = NULL;
	pMem->m_ptr = ptr;
	pMem->m_pValue = pValue;

	if ( m_ppHashTable[ nHashValue ] == NULL )
	{
		pList = new SMemList;
		pList->m_nCount = 0;
		pList->m_pHead = pList->m_pTail = NULL;
		((SMemList**)m_ppHashTable)[ nHashValue ] = pList;
	}
	else
		pList = ((SMemList**)m_ppHashTable)[ nHashValue ];

	pList->Add( pMem );

	return nHashValue;
}

void *MemHash::RemoveKey( void *ptr )
{
	if ( m_ppHashTable == NULL )
		return 0;
	
	int nHashValue = GetHashValue( ptr );
	void *ptrResult = NULL;

	SMemList *pList = ((SMemList**)m_ppHashTable)[ nHashValue ];
	if ( pList )
		ptrResult = pList->Remove( ptr );

	return ptrResult;
}

void *MemHash::GetKey( void *ptr )
{
	if ( m_ppHashTable == NULL )
		return 0;
	
	void *pPoint = NULL;
	int nHashValue = GetHashValue( ptr );
	SMemList *pList = ((SMemList**)m_ppHashTable)[ nHashValue ];

	if ( pList )
	{
		SMem *pNext = pList->m_pHead;
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

int MemHash::GetHashSize()
{
	return m_nHashSize;
}

//! �ѺϹɵ�˾ Hash����˺�� �㶮�ױ� ̡�� ��̺���.
static	MemHash*	mpHash = NULL;

int InitMemManager ()
{
	int nFlag = 0;
	switch (mnMemManagerState)
	{
	case MMS_None:
		{
			mpMemBlocks = NULL;
			mnMaxSize = 0;
			mnCurSize = 0;
			mnMemManagerState = MMS_Init;

			mpHash = (MemHash*)malloc (sizeof(MemHash));
			mpHash->Create( mnHashSize );

			nFlag = 1;
			break;
		}
	case MMS_Init:
		{
			nFlag = 0;
		}
	}

	return nFlag;
}

int ReleaseMemManager ()
{
	int nFlag = 0;
	switch (mnMemManagerState)
	{
	case MMS_None:
		nFlag = 0;
		break;
	case MMS_Init:
		{
			static char szLF[3] = {0x0d, 0x0a, 0x00};

			mpHash->Release();
			free (mpHash);
			mpHash = NULL;

			FILE* fp = NULL;
 			fp = fopen (mszMemleakFilename, "wb");
			SMemblockNode* ptrNode = mpMemBlocks;
			SMemblockNode* temp = mpMemBlocks;
			while(ptrNode)
			{
				if (fp)
				{
					if (ptrNode->filename[0] != 0)
						fprintf (fp, "%s\tline %d\t%p\t%d%s", ptrNode->filename, ptrNode->line, 
						ptrNode->ptr, ptrNode->len, szLF);
					else
						fprintf (fp, "%p\t%d%s", ptrNode->ptr, ptrNode->len, szLF);
				}
				temp = ptrNode->next;
				free (ptrNode->ptr);
				free (ptrNode);
				ptrNode = temp;
			}
			if (fp)
			{
 				fprintf(fp, "%smax memory size\t%d%s", szLF, mnMaxSize, szLF);
 				fclose(fp);
			}
			
			mnMemManagerState = MMS_None;
			nFlag = 1;
			break;
		}
	}
	return nFlag;
}

int GetMMState ()
{
	return mnMemManagerState;
}

void AddMemblock (void* ptr, int len, char* szFilename, int line, int objecttype)
{
	return;
#ifdef _DEBUG
	while (mnLock);
	mnLock = 1;

	switch (mnMemManagerState)
	{
	case MMS_None:
		{
			assert (false);	//���ͱ������� ������� ʱ��.
			break;	
		}
	case MMS_Init:
		{
			SMemblockNode* newNode;
			newNode = (SMemblockNode*)malloc (sizeof(SMemblockNode));
			newNode->ptr = ptr;
			newNode->len = len;
			newNode->objecttype = objecttype;
			if (szFilename)
			{
				strcpy (newNode->filename, szFilename);
				newNode->line = line;
			}
			else
			{
				newNode->filename[0] = 0;
				newNode->line = 0;
			}
			newNode->prev = NULL;
			newNode->next = mpMemBlocks;

			if (mpMemBlocks)
				mpMemBlocks->prev = newNode;

			mpMemBlocks = newNode;
			if ( mpHash )
				mpHash->AddKey( ptr, mpMemBlocks );

			mnCurSize += len;
			mnMaxSize = MAX (mnCurSize, mnMaxSize);
			break;
		}
	}
	mnLock = 0 ;
#endif //_DEBUG
	return;	
}

void FreeMembolck (void* ptr, int objecttype)
{
	free(ptr);
	return;
#ifdef _DEBUG
	while (mnLock);
	mnLock = 1;
	switch (mnMemManagerState)
	{
	case MMS_None:
		{
			assert (false);	//���ͱ������� ������� ʱ��.
			break;	
		}
	case MMS_Init:
		{
			bool bFind = false;
			void *pValue = NULL;
			SMemblockNode* ptrNode = NULL;

 			if (mpMemBlocks == NULL)
			{
				mnLock = 0;
 				return;
			}

			ptrNode = ( SMemblockNode* )mpHash->RemoveKey( ptr );
			if ( !ptrNode )
			{
				break;
			}
			if (ptrNode->objecttype != objecttype)
				assert (false);//("Cannot release a pointer which is not mine.");

			if(ptrNode->next)
				ptrNode->next->prev = ptrNode->prev;
			else
				ptrNode->next = NULL;

			if(ptrNode->prev)
				ptrNode->prev->next = ptrNode->next;
			else
				mpMemBlocks = ptrNode->next;

			free(ptrNode->ptr);

			mnCurSize -= ptrNode->len;
			free (ptrNode);

			mnMaxSize = MAX (mnCurSize, mnMaxSize);
			break;
		}
	}
	mnLock = 0;
	return;
#else
	free(ptr);
#endif // _DEBUG
}

}
