/*!
 * \file	PtrArray.h
 * \ingroup base
 * \brief	�Ѻϴ�̩ �ɵ�˾ �㶮�ױ� ̡�� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _PTRARRAY_H_
#define _PTRARRAY_H_

#include "Object.h"

namespace CVLib 
{

/**
 * @brief  �ѺϹɵۼ���˾ �����ױ� ̡�� �е���.
 */
class CVLIB_DECLSPEC PtrArray : public Object
{
public:
	// Construction and Destruction
	//! �龢��
	PtrArray();
	//! �;˻�
	virtual ~PtrArray();

	// Attributes
	/**
	 * @brief   ̺�ϴ�̩ �㶦 �в���.
	 *
	 * @return  �ɵ�̩ �ͱ�, ����˼ 0�� �����װ� ��˺��� �ɵ�̩ �ͱ��� \n
	 *			���� �� ����� 1 �ͳ�.
	 *
	 * @see  GetUpperBound(), SetSize()
	 *
	 * @par Example:
	 * @code
	 *  
	 *	PtrArray array;
	 *	Vec* pVec = new Vec;
	 *	array.Add (pVec);
	 *	assert( 1 == array.GetSize() );
	 *  ...
	 * @endcode
	 *
	 */
	int GetSize() const;

	/**
	 * @brief   ˱ã�� ���� �� ��㶦 �󴮶���.
	 *
	 * @return  ���� �� ���(0�� ������)
	 *
	 * @see  GetSize(), SetSize()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	assert( 1 == array.GetUpperBound() );	//Largest index
	 *  ...
	 * @endcode
	 *
	 */
	int GetUpperBound() const;

	/**
	 * @brief   �ɵ��� ���ܴ� ̺�ϴ�̩ �㶦 �����ٳ�.
	 *
	 * @param   nNewSize [in] :	�� �ɵ�̩ �ͱ�, 0�˺�����ʿ �ٳ�.
	 * @param   nGrowBy	 [in] :	�ɵ�̩ �ͱ��簡�� ­���׷� �ڳ��ױ� ̡�� ̺�ϴ�̩ �׺Ϻ�
	 * @return  void 
	 *
	 * @see  GetSize()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array1;
	 *	PtrArray array2;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array1.Add (pVec1);	
	 *	array2.Add (pVec2);	
	 *	
	 *  array1.SetSize(array1.m_nSize + array2.m_nSize);
	 *  assert(array1.m_nSize == 2);
	 *  ...
	 * @endcode
	 *
	 */
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations for Cleaning up
	/**
	 * @brief   ��� ����˧̩ ��˦�׼� ʱ�� ���ͱ��� ���� �͸��ٳ�.
	 *
	 * @return  void 
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	
	 *	array.Add (pVec2);	
	 *	...
	 *  array.FreeExtra();
	 *  assert(array.m_nSize == array.m_nMaxSize);
	 *  ...
	 * @endcode
	 *
	 */
	void FreeExtra();

	/**
	 * @brief   �� �ɵ۵Ṣ�� ���� ̺�ϴ�˾ �����ٳ�.
	 *
	 * @return  void 
     *
	 * @see  GetSize(), SetSize()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	assert( 2 == array.GetSize() );
	 *	array.RemoveAll(); // Pointers removed but objects not deleted.
	 *	assert( 0 == array.GetSize() );
	 *	delete pVec1;
	 *	delete pVec2;	// Cleans up memory.
	 *
	 * @endcode
	 *
	 */
	void RemoveAll();

	// Operations for Accessing elements
	/**
	 * @brief   ���̼� ����溷̩ ��˾ �в���.
	 *
	 * @param   nIndex [in]	: 0�˺��˰� GetUpperBound�� �󴮶��� �������� ��������
	 * @return  �� ����� �ز� �Ѻ�
	 *
	 * @see  SetAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	assert( array.GetAt(0) == pVec1 );
	 *	...
	 * @endcode
	 *
	 */
	void* GetAt(int nIndex) const;

	/**
	 * @brief   ���̼� ����� ������ ��˾ �����ٳ�. �ɵ�˼ �����׼� ʱ����.
	 *
	 * @param   nIndex	   [in]	: 0�˺��˰� GetUpperBound�� �󴮶��� �������� ��������
	 * @param   newElement [in] : �ɵ��� ���Ӵ� �Ѻ�
	 * @return  void 
	 *
	 * @see  GetAt(), SetAtGrow(), ElementAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.SetAt( 0, pVec3 );
	 *	assert( array.GetAt(0) == pVec3 );
	 *	...
	 * @endcode
	 *
	 */
	void SetAt(int nIndex, void* newElement);

	/**
	 * @brief   �ɵ۳���̩ ̺�ϼѺ��� ���� �ؼ��Ѻ϶� �в���.
	 *
	 * @param   nIndex [in]	: 0�˺��˰� GetUpperBound�� �󴮶��� �������� ��������
	 * @return  �Ѻ�̩ �ؼ�
	 *
	 * @see  operator[]
	 *
	 */
	void*& ElementAt(int nIndex);

	// Operations for Direct Access to the element data (may return NULL)
	/**
	 * @brief   �ɵ�ʯ̩ ̺�ϴ��� ���� �鼱���� ����˾ �����ٳ�.
	 <pre> NULL�κ� �س�. </pre>
	 *
	 * @return  �Ѻϴ�̩ �ɵ��� ���� �Ѻ�
	 *
	 * @see  GetAt(), SetAt(), ElementAt()
	 *
	 */
	const void** GetData() const;

	/**
	 * @brief   �ɵ�ʯ̩ ̺�ϴ��� ���� �鼱���� ����˾ �����ٳ�.
	 <pre> NULL�κ� �س�. </pre>
	 *
	 * @return  �Ѻϴ�̩ �ɵ��� ���� �Ѻ�
	 *
	 * @see  GetData()
	 *
	 */
	void** GetData();

	// Operations for Potentially growing the array
	/**
	 * @brief   ���̼� ����� ������ ��˾ �����ٳ�. ­���׳޷� �ɵ�˾ ���»��ֳ�.
	 *
	 * @param   nIndex	   [in]	: 0�˺��� ������
	 * @param   newElement [in]	: �� �ɵ��� ������ �Ѻ��˳�. NULL���� ��˦�ٳ�.
	 * @return  void 
	 *
	 * @see  GetAt(), ElementAt(), operator[]
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.SetAt( 3, pVec3 );
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	SetAtGrow example: A PtrArray with 4 elements
	 *		[0] = pVec1
	 *		[1] = pVec2
	 *		[2] = NULL
	 *		[3] = pVec3
	 *
	 * @endcode
	 *
	 */
	void SetAtGrow(int nIndex, void* newElement);

	/**
	 * @brief   �ɵ�̩ ���� ̺�϶� �����ٳ�. ­���׳޷� �ɵ�˾ ���»��ֳ�.
	 *
	 * @param   newElement [in]	: �� �ɵ��� ������ ̺��
	 * @return  ������ ̺��̩ ���
	 *
	 * @see  SetAt(), SetAtGrow(), InsertAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	Add example: A PtrArray with 2 elements
	 *		[0] = pVec1
	 *		[1] = pVec2
	 *
	 * @endcode
	 *
	 */
	int Add(void* newElement);

	/**
	 * @brief   �ɵ��� �޶� �ɵ�˾ �����ٳ�. ­���׳޷� �ɵ�˾ ���»�����.
	 *
	 * @param   src [in] : �� �ɵ��� ������ ̺�ϴ�̩ ̺��
	 * @return  ����˺�� ������ ̺��̩ ���
	 *
	 * @see PtrArray::Copy()
	 *
	 */
	int Append(const PtrArray& src);

	/**
	 * @brief   �ɵ��� �޶� �ɵ�˾ �����ٳ�. ­���׳޷� �ɵ�˾ ���»�����.
	 *
	 * @param   src [in] : �ɵ��� ������ ̺�ϴ�̩ ̺��
	 * @return  void 
	 *
	 * @see PtrArray::Append()
	 *
	 */
	void Copy(const PtrArray& src);

	// overloaded operator helpers
	/**
	 * @brief   ���̼� ����� �ز� ̺�϶� �в���.
	 *
	 * @param   nIndex [in]	: 0�˺��˰� GetUpperBound()�� �󴮶��� �������� ��������
	 * @return  �� ����� �ز� �Ѻ� 
	 *
	 * @see  GetAt(), SetAt()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	Vec* pA = (Vec*)array[0];
	 *	assert( *pA == pVec1 ); // Get element 0
	 *	array[0] = pVec3; // Replace element 0
	 *	delete pA;
	 *	assert( array[0] == pVec3 ); // Get new element 0
	 *  ...
	 * @endcode
	 *
	 */
	void* operator[](int nIndex) const;

	/**
	 * @brief   �ɵ۳���̩ ̺�ϼѺ��� ���� �ؼ��Ѻ϶� �в���.
	 *
	 * @param   nIndex [in]	: 0�˺��˰� GetUpperBound()�� �󴮶��� �������� �������� 
	 * @return  �Ѻ�̩ �ؼ� 
	 *
	 * @see	 operator[]
	 *
	 */
	void*& operator[](int nIndex);

	// Operations that move elements around
	/**
	 * @brief   �㼳�� ̡���� ̺�϶� �����ٳ�.
	 *
	 * @param   nIndex	   [in]	: GetUpperBound�� �󴮶� ����� �к㴪 �ز� ��������
	 * @param   newElement [in]	: �� �ɵ��� ̡���� ̺��, NULL�κ� �س�.
	 * @param   nCount	   [in]	: �� ̺�Ϻ��� ���Ӵ���. default�� 1�˳�.
	 * @return  void 
	 *
	 * @see  SetAt(), RemoveAt()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	Vec* pVec3 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.InsertAt( 1, pVec3 );
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	InsertAt example: A PtrArray with 3 elements
	 *		[0] = pVec1
	 *		[1] = pVec3
	 *		[2] = pVec2
	 *
	 * @endcode
	 *
	 */
	void InsertAt(int nIndex, void* newElement, int nCount = 1);

	/**
	 * @brief   �㼳�� ̡���� ̺�϶� �����ٳ�.
	 *
	 * @param   nStartIndex	[in] : GetUpperBound�� �󴮶� ����� �к㴪 �ز� ��������
	 * @param   pNewArray	[in] : �� �ɵ��� ������ ̺�ϴ�˾ �����ײ� �޶� �ɵ�
	 * @return  void
	 *
	 * @see  InsertAt()
	 *
	 */
	void InsertAt(int nStartIndex, PtrArray* pNewArray);

	/**
	 * @brief   �㼳�� ̡���� �ز� ̺�ϴ�˾ �����ٳ�.
	 *
	 * @param   nIndex [in]	: 0�˺��˷��� GetUpperBound�� �󴮶� �������� ��������
	 * @param   nCount [in]	: ������ ̺�ϴ�̩ ��
	 * @return  void 
	 *
	 * @see  GetAt(), SetAtGrow(), ElementAt(), operator[]
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrArray array;
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	array.Add (pVec1);	//Element 0
	 *	array.Add (pVec2);	//Element 1
	 *	array.RemoveAt( 0 );
	 *	...
	 *	The results from this program are as follows:
	 *
	 *	RemoveAt example: A PtrArray with 1 elements
	 *		[0] = pVec2
	 *
	 * @endcode
	 *
	 */
	void RemoveAt(int nIndex, int nCount = 1);

// Implementation
protected:
	//! ����ɵ� 
	void** m_pData;
	//! �ڳ�� ���Ϻ� 
	int m_nSize;
	//! �״����Ϻ� 
	int m_nMaxSize;
	//! �ɵ�̩ ���º� 
	int m_nGrowBy;

protected:
	// local typedefs for class templates
	typedef void* BASE_TYPE;
	typedef void* BASE_ARG_TYPE;
};

}

#endif //_PTRARRAY_H_
