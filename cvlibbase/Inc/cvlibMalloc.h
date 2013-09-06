
/*!
 * \file	cvlibmalloc.h
 * \ingroup base
 * \brief	CVLib����̩ ���ͱ��� �㶮�ײ� �ܺ��˾ ��̩��.
 * \author	����˧
 */

#ifndef _CVLIBMALLOC_H_
#define _CVLIBMALLOC_H_

#include <stdlib.h>
#include <assert.h>
#include "cvlibbaseDef.h"

namespace CVLib
{
/**
 * @brief	���ͱ��㶮 ������ �����ٳ�.
 *
 * @return	���ͱ�������˾ ���ؼӳ�.
 *
 * @see  ReleaseMemManager()
 *
 */
int CVLIB_DECLSPEC InitMemManager ();

/**
 * @brief	���ͱ��㶮 ���뾷���� �����ٳ�.
 <pre> �ͽ��׼� ʱ˼ ���ͱ����� ������ �������� "c:\memoryleak.txt"�� ����ٳ�. </pre>
 *
 * @return	���ͱ��͸Ҵ� �αᶦ ���ؼӳ�.
 *
 * @see  InitMemManager()
 *
 */
int CVLIB_DECLSPEC ReleaseMemManager ();

/**
 * @brief	���ͱ��㶮����̩ ������ �в���.
 *
 * @return 	���ͱ��㶮����̩ ����
 *
 */
int CVLIB_DECLSPEC GetMMState ();

/**
 * @brief	�ͳ��� ���ͱ��� ���ͱ�����̮ ���ͱ������ ����,
 *		    �Ը���, ���ͱ������� ��Ů ���ͱ��㶮���� �����ٳ�.
 *
 * @param	ptr		   [in] : ����� ���ͱ�̩ �Ѻ�
 * @param	len		   [in] : ���ͱ�̩ ����
 * @param	szFilename [in] : ���ͱ��� ����� ̺������̩ �˶�
 * @param	line	   [in] : ���ͱ��� ����� ̺������̩ �Ը���
 * @param	objecttype [in] : ���ͱ� ����.	
				malloc�� ����� ��˧ 0, new�� ����� ��˧ 1.
 * @return	void
 *
 * @see  FreeMembolck()
 *
 */
void CVLIB_DECLSPEC AddMemblock (void* ptr, int len, char* szFilename, int line, int objecttype);

/**
 * @brief	�ͳ��� ���ͱ��� ���ͱ��㶮���Ṣ�� �͸��ٳ�.

 * @param	ptr		   [in] : �͸��� ���ͱ�̩ �Ѻ�
 * @param	objecttype [in]	: ���ͱ� ����.
 * @return	void
 *
 * @see  AddMemblock()
 *
 */
void CVLIB_DECLSPEC FreeMembolck (void* ptr, int objecttype);

/************************************************************************/
/*					 ���ͱ��㶮 ��͵��								*/
/************************************************************************/
/**
 * @brief	���ͱ��� �����ٳ�.
 <pre> ����� ���ͱ��� ������ ������ �����׼� ʱ����. </pre>
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
 * @brief	���ͱ��� �����ٳ�.
 <pre> ����� ���ͱ��� 0˺�� �����ٳ�. </pre>
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
 * @brief	���ͱ��� �͸��ٳ�.
 * @see	MALLOC, CALLOC
 */
#define  FREE(ptr)									\
	FreeMembolck((void*)(ptr), 0);

/**
 * @brief	�ﹶ������ ���ͱ��� �����ٳ�.
 */
#define	 MallocOBJECT(ptr, size)					\
	{							\
	assert ((size) != 0);                           \
	(ptr) = malloc (size);                          \
	AddMemblock((void*)(ptr), size, __FILE__, __LINE__, 1);	\
	}

/**
 * @brief	�ﹶ������ ���ͱ��� �͸��ٳ�.
 */
#define	 FreeOBJECT(ptr)							\
	FreeMembolck ((void*)(ptr), 1);

/**
 * @brief	2��̺�ɵ�����̩ ���ͱ��� �ڳ��ٳ�.
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
 * @brief	2��̺�ɵ�����̩ ���ͱ��� �͸��ٳ�.
 */
#define  FREE2(ptr)						\
	FreeMembolck((void*)(ptr), 0);

}


#endif //_CVLIBMALLOC_H_
