
/*!
 * \file	cvlibmalloc.cpp
 * \ingroup base
 * \brief	CVLibº·°ÖÌ© ±¨ÊÍ±¨¶¦ ±ã¶®Â×²÷ ÂÜºã´ÉË¾ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include "cvlibMalloc.h"
#include "cvlibmacros.h"

namespace CVLib
{

/**
 * @brief	±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ·ô´Î °é¼¿¾Ë.
 */
struct SMemblockNode
{
    void	*ptr;						/**< Ãú¸ó´ô ±¨ÊÍ±¨ ¼ÑºÏ */
	int		objecttype;					/**< ±¨ÊÍ±¨Ì© µùÂô */
	int		len;						/**< ±¨ÊÍ±¨Ì© ±¬ËË */
	char	filename[300];				/**< ±¨ÊÍ±¨°¡ Ãú¸ó´ô ÁÄËÎËË¶§ */
	int		line;						/**< ±¨ÊÍ±¨°¡ Ãú¸ó´ô Ìº½ç¿¸´ÅÌ© ÃÔ¸ÜÂö */
    struct	SMemblockNode *prev;		/**< ËË¼­ ±¨ÊÍ±¨¹¹µá¿Í·ô´ÎÌ© ¼ÑºÏ */
    struct	SMemblockNode *next;		/**< ³ÞËÁ ±¨ÊÍ±¨¹¹µá¿Í·ô´ÎÌ© ¼ÑºÏ */
};

//!	ÃÍ¸Ò´ó¼è Ê±Ë¼ ±¨ÊÍ±¨¶¦ Ãú¸óÂÙ Ìº½çÁÄËÎ±á ÃÔ¸ÜÂö¶¦ ¸ó±ãÂ×²÷´ç ¶®Ë¦´ó²÷ Ë÷¹¢ÁÄËÎÌ© ËË¶§.
static char mszMemleakFilename[] = "c:/memoryleak.txt";

//! »¤»õ ±¨ÊÍ±¨¹¹µá¿Í·ô´Î.
static SMemblockNode* mpMemBlocks;

//! ¾×´Ýµá ¶®Ë¦´ó²÷ ±¨ÊÍ±¨¿Í±¨.
static size_t	mnMaxSize;

//! Âï¼õÌ© ¶®Ë¦´ó²÷ ±¨ÊÍ±¨¿Í±¨.
static size_t	mnCurSize;


//! HashÁìÌ© ¿Í±¨.
static const int mnHashSize = 4999;

//! ½ó±¨Ãù±¨¸Ë.
enum MMS_FLAG
{
	MMS_None = 0,
	MMS_Init = 1,
};

//! ±¨ÊÍ±¨±ã¶®º¬Àô±¨¸Ë.
static int		mnMemManagerState = MMS_None;

static int		mnLock = 0;
/**
 * @brief	Hash¶¦ ¶®Ë¦Â×ÊÞ ·»µâË¾ ±ã¶®Â×±¨ Ì¡ÂÙ ¿Ðµ¹º÷.
 */
struct MemHash
{
	/**
	 * @brief	·»µâ¿Ðµ¹º÷Ëæ ´ÝÂÙ ½ó±¨Ãù¶¦ ¼êÃÔÂÙ³Þ.
	 * @param	nHashSize [in] : HashÁìÌ© ¿Í±¨
	 */
	void Create( int nHashSize );
	/**
	 * @brief	·»µâ¿Ðµ¹º÷Ëæ ´ÝÂÙ ÃÍ½£¶¦ ¼êÃÔÂÙ³Þ.
	 */
	void Release();

	/**
	 * @brief	HashÁìÌ© ¿Í±¨ ÊÐ±¨ÂÜºã
	 * @param	ÊÖËÁ
	 * @return	(int) \n
	 *		HashÁìÌ© ¿Í±¨
	 */
	int GetHashSize();
	
	/**
	 * @brief	Hash°ª ÊÐ±¨ÂÜºã
	 * @param	ptr [in] : Êâ»Ì°ª
	 * @return	(int)  \n
	 *		Hash°ª
	 *
	 * @see	MemHash::AddKey(), MemHash::RemoveKey(), MemHash::GetKey()
	 */
	int GetHashValue( void* ptr );

	/**
	 * @brief	¼ÑºÏ°ªËæ ´ÝÂÙ ±¨ÊÍ±¨¹¹µá¿Í °ªÊÐ±¨ÂÜºã
	 * @param	ptr [in] : ÊÐËºµØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
	 * @return	(void*) \n
	 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§ \n
	 *		±¨À° : ¼ÑºÏ°ª
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
	 * @brief	±¨ÊÍ±¨¹¹µá¿Í ¾¡°¡ÂÜºã
	 * @param	ptr [in] : ¾¡°¡Â×µØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
	 * @param	pValue [in] : ¼ÑºÏ°ªËæ Åà¶£²÷ °ª
	 * @return	(int)  \n
	 *		Hash°ª
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
	 * @brief	±¨ÊÍ±¨¹¹µá¿Í º¢½£ÂÜºã
	 * @param	ptr [in] : º¢½£Â×µØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
	 * @return	(void*)  \n
	 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§ \n
	 *		±¨À° : ¼ÑºÏ°ª, ºÂ°á
	 *
	 * @see	MemHash::AddKey(), MemHash::GetHashValue()
	 */
	void *RemoveKey( void *ptr );

	//! HashÁìÌ© ¿Í±¨
	int m_nHashSize;
	//! ·»µâ°é¼¿¾Ë¹ÉµÛËæ ´ÝÂÙ HashÁìÌ© ¼è¼¬»ô
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
 * @brief		±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ·ô´Î °é¼¿¾Ë.
 */
struct SMem
{
	SMem *m_pPrev;		/**< ÊÌ·ç±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ¼è¼¬»ô */
	SMem *m_pNext;		/**< »ô»¥±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ¼è¼¬»ô */
	void *m_ptr;		/**< ¼ÑºÏ°ª */
	void *m_pValue;		/**< ¼ÑºÏ°ªËæ Åà¶£²÷ Ãú¸ó´ô ±¨ÊÍ±¨Ì© ¼ÑºÏ */

	/**
	 * @brief	±¨ÊÍ±¨¹¹µá¿Í°é¼¿¾ËËæ ´ÝÂÙ ½ó±¨Ãù¶¦ ¼êÃÔÂÙ³Þ.
	 */
	SMem(){ m_pPrev = m_pNext = NULL; m_ptr = m_pValue = NULL; }
};

/**
 * @brief		±¨ÊÍ±¨¹¹µá¿Í¶¦ ±ã¶®Â×±¨ Ì¡ÂÙ ·»µâËæ ´ÝÂÙ °é¼¿¾Ë.
 */
struct SMemList
{
	int m_nCount;		/**< ·»µâËæ ¾¡°¡´ô ¹¹µá¿ÍÌ© ±¶ºã */
	SMem *m_pHead;		/**< ·»µâÌ© ½ë ¹¹µá¿ÍÌ© ¼ÑºÏ */
	SMem *m_pTail;		/**< ·»µâÌ© ¶ë¼è¶ì¹¹µá¿ÍÌ© ¼ÑºÏ */

	SMemList();
	bool Add( SMem *pMem );
	void *Remove( void *ptr );
	void *Get( void *ptr );
	SMem *Find( void *ptr );

};

/**
 * @brief	·»µâ°é¼¿¾ËËæ ´ÝÂÙ ½ó±¨Ãù¶¦ ¼êÃÔÂÙ³Þ.
 */
SMemList::SMemList()
{
	m_nCount = 0;
	m_pHead = m_pTail = NULL;
}

/**
 * @brief	±¨ÊÍ±¨¹¹µá¿Í ¾¡°¡ÂÜºã
 * @param	pMem [in] : ¾¡°¡Â×µØ²÷ ±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ¼è¼¬»ô
 * @return	(bool)  \n
 *		true : ºÂ°á, false : »¨Â²
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
			// »¯ ±¨ÊÍ±¨¶¦ ¾¡°¡Â×²÷ °ÒË§ TailËæ ´ÝÂÙ ¼¿»õ¶í ÂÙ³Þ.
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
 * @brief	¼ÑºÏ°ªËæ ´ÝÂÙ ±¨ÊÍ±¨¹¹µá¿Í ÊÐ±¨ÂÜºã
 * @param	ptr [in] : ÊÐËºµØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
 * @return	(SMem*) \n
 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§  \n
 *		±¨À° : ±¨ÊÍ±¨¹¹µá¿ÍÌ© ¼ÑºÏ°ª
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
 * @brief	¼ÑºÏ°ªËæ ´ÝÂÙ ±¨ÊÍ±¨¹¹µá¿Í °ªÊÐ±¨ÂÜºã
 * @param	ptr [in] : ÊÐËºµØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
 * @return	(void*) \n
 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§ \n
 *		±¨À° : ¼ÑºÏ°ª
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
 * @brief	±¨ÊÍ±¨¹¹µá¿Í º¢½£ÂÜºã
 * @param	ptr [in] : º¢½£Â×µØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
 * @return	(void*) \n
 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§ \n
 *		±¨À° : ¼ÑºÏ°ª, ºÂ°á
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

			// º¢½£Â×µØ²÷ ±¨ÊÍ±¨°¡ HeadÌ® °¯Ë¼ °ÒË§
			if ( pMem == m_pHead )
			{
				// ±¨ÊÍ±¨°¡ Â×±ý´ª ÊÖ²÷ °ÒË§
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
			// º¢½£Â×µØ²÷ ±¨ÊÍ±¨°¡ Tail±á °¯Ë¼ °ÒË§
			else if ( pMem == m_pTail )
			{
				m_pTail = pMem->m_pPrev;
				m_pTail->m_pNext = NULL;
			}
			else // ±¨À° ËÎ¸É¼¬ËÍ °ÒË§
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
					// ±¨ÊÍ±¨ÃÍ½£¶¦ Tailµá¹¢À¾ ¼êÃÔÂÙ³Þ.
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

//! ¼ÑºÏ¹ÉµÛË¾ HashÂô»¥Ëºµá ±ã¶®Â×±¨ Ì¡ÂÙ ºÂÌº¸êºã.
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
			assert (false);	//±¨ÊÍ±¨º¬Àô°¡ ½ó±¨Ãù´ó¼è Ê±ËÁ.
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
			assert (false);	//±¨ÊÍ±¨º¬Àô°¡ ½ó±¨Ãù´ó¼è Ê±ËÁ.
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
