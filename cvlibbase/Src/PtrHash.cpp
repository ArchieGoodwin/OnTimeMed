/*!
 * \file	PtrHash.cpp
 * \ingroup base
 * \brief	List¹ÉµÛË¾ HashËæ Ì©Â×ÊÞ ±ã¶®Â×²÷ ÂÜºã´ÉË¾ ¼³Ì©ÂÙ³Þ.
 * \author	¶®Âî½è
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
 * @brief		±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ·ô´Î °é¼¿¾Ë.
 */
struct SMemHash
{
	SMemHash *m_pPrev;		/**< ÊÌ·ç±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ¼è¼¬»ô */
	SMemHash *m_pNext;		/**< »ô»¥±¨ÊÍ±¨¹¹µá¿ÍËæ ´ÝÂÙ ¼è¼¬»ô */
	void *m_ptr;		/**< ¼ÑºÏ°ª */
	void *m_pValue;		/**< ¼ÑºÏ°ªËæ Åà¶£²÷ Ãú¸ó´ô ±¨ÊÍ±¨Ì© ¼ÑºÏ */

	/**
	 * @brief	±¨ÊÍ±¨¹¹µá¿Í°é¼¿¾ËËæ ´ÝÂÙ ½ó±¨Ãù¶¦ ¼êÃÔÂÙ³Þ.
	 */
	SMemHash(){ m_pPrev = m_pNext = NULL; m_ptr = m_pValue = NULL; }
};

/**
 * @brief		±¨ÊÍ±¨¹¹µá¿Í¶¦ ±ã¶®Â×±¨ Ì¡ÂÙ ·»µâËæ ´ÝÂÙ °é¼¿¾Ë.
 */
struct SMemHashList
{
	int m_nCount;		/**< ·»µâËæ ¾¡°¡´ô ¹¹µá¿ÍÌ© ±¶ºã */
	SMemHash *m_pHead;		/**< ·»µâÌ© ½ë ¹¹µá¿ÍÌ© ¼ÑºÏ */
	SMemHash *m_pTail;		/**< ·»µâÌ© ¶ë¼è¶ì¹¹µá¿ÍÌ© ¼ÑºÏ */

	SMemHashList();
	bool Add( SMemHash *pMem );
	void *Remove( void *ptr );
	void *Get( void *ptr );
	SMemHash *Find( void *ptr );

};

/**
 * @brief	·»µâ°é¼¿¾ËËæ ´ÝÂÙ ½ó±¨Ãù¶¦ ¼êÃÔÂÙ³Þ.
 */
SMemHashList::SMemHashList()
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
bool SMemHashList::Add( SMemHash *pMem )
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
 * @return	(SMemHash*) \n
 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§  \n
 *		±¨À° : ±¨ÊÍ±¨¹¹µá¿ÍÌ© ¼ÑºÏ°ª
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
 * @brief	¼ÑºÏ°ªËæ ´ÝÂÙ ±¨ÊÍ±¨¹¹µá¿Í °ªÊÐ±¨ÂÜºã
 * @param	ptr [in] : ÊÐËºµØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
 * @return	(void*) \n
 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§ \n
 *		±¨À° : ¼ÑºÏ°ª
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
 * @brief	±¨ÊÍ±¨¹¹µá¿Í º¢½£ÂÜºã
 * @param	ptr [in] : º¢½£Â×µØ²÷ ±¨ÊÍ±¨¹¹µá¿Í°¡ °¡¼è°Ö ËØ²÷ ¼ÑºÏ°ª
 * @return	(void*) \n
 *		NULL : ÃÍ³ëÂÙ °ªËË ÊÖ²÷ °ÒË§ \n
 *		±¨À° : ¼ÑºÏ°ª, ºÂ°á
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
					// ±¨ÊÍ±¨ÃÍ½£¶¦ Tailµá¹¢À¾ ¼êÃÔÂÙ³Þ.
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
