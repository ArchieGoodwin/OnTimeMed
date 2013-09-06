/*!
 * \file	PtrHash.h
 * \ingroup base
 * \brief	List股帝司 Hash随 泰伦兽 便懂伦谗 伴伎舅 否 啃倒瑚瓷司 汲泰萝.
 * \author	懂骂借
 */

#ifndef _PTRHASH_H_
#define _PTRHASH_H_

#include "Object.h"

namespace CVLib
{
/**
 * @brief	Hash鼎 懂甩伦兽 坊碘司 便懂伦报 獭沦 啃倒瑚.
 */
class CVLIB_DECLSPEC PtrHash
{
public:

	/**
	 * @brief   坊碘啃倒瑚随 摧沦 襟报鸣鼎 缄迷沦侈.
	 *
	 * @param	nHashSize [in] : Hash领泰 客报
	 *
	 */
	PtrHash( int nHashSize );

	//!  猛舅霍 
	virtual ~PtrHash();

	/**
	 * @brief   Hash领泰 客报鼎 市报 獭沦 萝恒怂侈.
	 *
	 * @return	Hash领泰 客报
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
	 * @brief	Hash蔼 市报 獭沦 萝恒怂侈.
	 *
	 * @param	ptr [in] : 殊惶蔼
	 * @return	Hash蔼
	 *
	 * @see	 AddKey(), RemoveKey(), GetKey()
	 *
	 */
	int GetHashValue(void* ptr);

	/**
	 * @brief   佳合蔼随 摧沦 报释报构滇客 蔼司 市谗 萝恒怂侈.
	 *
	 * @param	ptr [in] : 市撕地谗 报释报构滇客啊 啊艰爸 素谗 佳合蔼
	 * @return	NULL : 猛畴沦 蔼怂 手谗 耙帅 \n
	 *		    报腊 : 佳合蔼
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
	 * @brief  	报释报构滇客鼎 尽啊伦谗 萝恒怂侈.
	 *
	 * @param	ptr    [in] : 尽啊伦地谗 报释报构滇客啊 啊艰爸 素谗 佳合蔼
	 * @param	pValue [in] : 佳合蔼随 培叮谗 蔼
	 * @return	Hash蔼
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
	 * @brief  	报释报构滇客鼎 孩剑伦谗 萝恒怂侈.
	 *
	 * @param	ptr [in] : 孩剑伦地谗 报释报构滇客啊 啊艰爸 素谗 佳合蔼
	 * @return	NULL : 猛畴沦 蔼怂 手谗 耙帅 \n
	 *		    报腊 : 佳合蔼, 郝搬
	 *
	 * @see	 AddKey(), GetHashValue()
	 *
	 */
	void *RemoveKey(void *ptr);

private:
	//! Hash领泰 客报
	int m_nHashSize;
	//! 坊碘伴伎舅股帝随 摧沦 Hash领泰 艰棘霍
	void** m_ppHashTable;
};

}

#endif // _PTRHASH_H_
