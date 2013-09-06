
/*!
 * \file	cvlibmalloc.h
 * \ingroup base
 * \brief	CVLib悍爸泰 报释报鼎 便懂伦谗 萝恒瓷司 汲泰萝.
 * \author	碑借帅
 */

#ifndef _CVLIBMALLOC_H_
#define _CVLIBMALLOC_H_

#include <stdlib.h>
#include <assert.h>
#include "cvlibbaseDef.h"

namespace CVLib
{
/**
 * @brief	报释报便懂 襟报鸣鼎 缄迷沦侈.
 *
 * @return	报释报襟报鸣蔼司 串地加侈.
 *
 * @see  ReleaseMemManager()
 *
 */
int CVLIB_DECLSPEC InitMemManager ();

/**
 * @brief	报释报便懂 涅峨痉报鼎 缄迷沦侈.
 <pre> 猛剑伦艰 时思 报释报瓷随 摧伦兽 索耿聊宋 "c:\memoryleak.txt"随 阁便沦侈. </pre>
 *
 * @return	报释报猛敢呆 拔贬鼎 串地加侈.
 *
 * @see  InitMemManager()
 *
 */
int CVLIB_DECLSPEC ReleaseMemManager ();

/**
 * @brief	报释报便懂泛绰泰 含吏鼎 市谗侈.
 *
 * @return 	报释报便懂泛绰泰 含吏
 *
 */
int CVLIB_DECLSPEC GetMMState ();

/**
 * @brief	猛畴沦 报释报鼎 报释报爆怂坍 报释报铭阁呆 聊宋,
 *		    迷杠脉, 报释报爹卖贬 萝女 报释报便懂霍随 尽啊沦侈.
 *
 * @param	ptr		   [in] : 铭阁呆 报释报泰 佳合
 * @param	len		   [in] : 报释报泰 爆怂
 * @param	szFilename [in] : 报释报啊 铭阁呆 毯界聊宋泰 怂锭
 * @param	line	   [in] : 报释报啊 铭阁呆 毯界聊宋泰 迷杠脉
 * @param	objecttype [in] : 报释报 爹卖.	
				malloc滇 铭阁呆 耙帅 0, new滇 铭阁呆 耙帅 1.
 * @return	void
 *
 * @see  FreeMembolck()
 *
 */
void CVLIB_DECLSPEC AddMemblock (void* ptr, int len, char* szFilename, int line, int objecttype);

/**
 * @brief	猛畴沦 报释报鼎 报释报便懂霍滇耿谰 猛敢沦侈.

 * @param	ptr		   [in] : 猛敢纶 报释报泰 佳合
 * @param	objecttype [in]	: 报释报 爹卖.
 * @return	void
 *
 * @see  AddMemblock()
 *
 */
void CVLIB_DECLSPEC FreeMembolck (void* ptr, int objecttype);

/************************************************************************/
/*					 报释报便懂 峨客滇瓷								*/
/************************************************************************/
/**
 * @brief	报释报鼎 铭阁沦侈.
 <pre> 铭阁呆 报释报随 摧伦兽 襟报鸣鼎 缄迷伦艰 时谗侈. </pre>
 *
 * @see	FREE, CALLOC
 */
#define  MALLOC(ptr, size, type)                    \
	{							\
	assert ((size) != 0);                           \
	(ptr) = (type*)malloc ((size));                 \
	AddMemblock( (void*)(ptr), (size), __FILE__, __LINE__, 0);  \
	}

/**
 * @brief	报释报鼎 铭阁沦侈.
 <pre> 铭阁呆 报释报鼎 0撕滇 襟报鸣沦侈. </pre>
 *
 * @see	 FREE, MALLOC
 *
 * @par	Example:
 * @code
 *	int nMaxMemNum = 100000;
 *	int* pnAddresses = new int[nMaxMemNum];
 *	for (int i = 0; i < nMaxMemNum; i ++)
 *	{
 *		int* p;
 *		CALLOC (p, 1000, int);
 *		pnAddresses[i] = (int)p;
 *	}
 *
 *	for (i = 0; i < nMaxMemNum; i ++)
 *	{
 *		int* p = (int*)pnAddresses[i];
 *		FREE (p);
 *	}
 *	delete []pnAddresses;
 *  
 * @endcode
 *
 */
#define  CALLOC(ptr, nCount, type)						            \
	{									\
	assert ((nCount) != 0);                                         \
	(ptr) = (type*)calloc ((nCount), sizeof(type));                 \
	AddMemblock((void*)(ptr), (nCount)*sizeof(type), __FILE__, __LINE__, 0);	\
	}
			
/**
 * @brief	报释报鼎 猛敢沦侈.
 * @see	MALLOC, CALLOC
 */
#define  FREE(ptr)									\
	FreeMembolck((void*)(ptr), 0);

/**
 * @brief	曙苟饯棱卖 报释报鼎 铭阁沦侈.
 */
#define	 MallocOBJECT(ptr, size)					\
	{							\
	assert ((size) != 0);                           \
	(ptr) = malloc (size);                          \
	AddMemblock((void*)(ptr), size, __FILE__, __LINE__, 1);	\
	}

/**
 * @brief	曙苟饯棱卖 报释报鼎 猛敢沦侈.
 */
#define	 FreeOBJECT(ptr)							\
	FreeMembolck ((void*)(ptr), 1);

/**
 * @brief	2接毯股帝卖吏泰 报释报鼎 纶畴沦侈.
 */
#define	 MALLOC2(ptr, sizeH, sizeW, type)                           \
	{									\
	int imalloc, size, linesize;                                    \
	linesize = sizeof(type) * (sizeW);                              \
	size = sizeof(type*) * (sizeH) + (sizeH) * linesize;            \
	(ptr) = (type**)malloc(size);                                   \
	for (imalloc = 0 ; imalloc < (sizeH); imalloc ++)               \
		(ptr)[imalloc] = (type*)( (char*)(ptr) +                \
			sizeof(type*) * (sizeH) + linesize * imalloc);  \
	AddMemblock((void*)(ptr), size, __FILE__, __LINE__, 0);                  \
	}													

/**
 * @brief	2接毯股帝卖吏泰 报释报鼎 猛敢沦侈.
 */
#define  FREE2(ptr)						\
	FreeMembolck((void*)(ptr), 0);

}


#endif //_CVLIBMALLOC_H_
