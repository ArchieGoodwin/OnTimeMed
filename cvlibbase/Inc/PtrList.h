/*!
 * \file	PtrList.h
 * \ingroup base
 * \brief	�ڰη���˾ �㶮�ײ� �е����� ��̩��.
 * \author	���β�, ����˧
 */

#ifndef _PTRLIST_H_
#define _PTRLIST_H_

#include "Object.h"

namespace CVLib
{

/**
 * @brief �ڰη���˾ �㶮�ײ� �е���
 */
class CVLIB_DECLSPEC PtrList : public Object
{
public:
	
	/**
	 * @brief �ڰη��⾡���һ�˾ �輳�ײ� �۰���
	 */
	enum LISTAddMode
	{
		ADD_BEFORE = 0,     /**< ����� ����̩ �� ̡�� �Ƶ� ʼ�� ��������.*/
		ADD_AFTER  = 1,     /**< ����� ����̩ �� ̡�� �Ƶ� ���� ��������.*/
	};

	/**
	 * @brief  ����̩ ̡���� �輳�ײ� �۰��� 
	 */
	enum LISTPositionType
	{
		POS_BEGIN   = 0,    /**< ����̩ ����̡�� */
		POS_CURRENT = 1,    /**< ����̩ ���̡�� */
		POS_END     = 2  	/**< ����̩ ��̡��   */ 
	};

public:
	
	// Constructor and Destructor
	//! �龢��
	PtrList();
	//! �;˻� 
	virtual ~PtrList();
	
	// Check if the instance was created
	/**
	 * @brief   ����̩ ˱ã��˾ �Ƽ��ٳ�.
	 *
	 * @return	˱ã�׷� true, ʭ���� false
	 *
	 * @see PtrList::Create()
	 *
	 */
	bool FlagCreate(void) { return m_pData != NULL; }

	// Initialize the list.
	/**
	 * @brief   ����˾ �����ٳ�.
	 *
	 * @return	�°��˷� true, ��²�˷� false
	 *
	 * @see  FlagCreate(), Release()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *	...
	 *	listDB.Release();
	 *
	 * @endcode
	 *
	 */
	bool Create(void);
	
	// Destroy the list and deletes all nodes.
	/**
	 * @brief   ����˾ �ı��ٳ�.
	 *
	 * @return  void
	 *
	 * @see  Create(), FlagCreate()
	 *
	 */
	virtual void Release(void);

	// Get the size of data.
	/**
	 * @brief   ����ʯ̩ ���̩ �㶦 �󴮶���.
	 *
	 * @return  ��Ѻ�
	 *
	 * @see  Add(), Remove()
	 *
	 */
	unsigned long Count(void) const;
	
	// Get the data at the current list position. 
	/**
	 * @brief   ��� �軤���溷̩ ���궦 �󴮶���.
	 * @brief   ��� �軤���溷̩ ���� �󴮶���.
	 *
	 * @return  �Ѻ�, �ۼ���� ʱʾ˺�� NULL˾ �󴮶���.
	 *
	 * @see  SetPosition()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *	assert( 2 == listDB.Count() );
	 *
	 *	listDB.SetPosition(PtrList::POS_BEGIN, 0);	//Set element 0
	 *	assert( pVec1 == listDB.Data() );
	 *	...
	 * @endcode
	 *
	 */
	void* Data(void) const;

	// Adds data to the list.
	/**
	 * @brief   �һ��� �൹ ���궦 ����/���� �ٳ�.
	 *
	 * @param   pData [in] : ����
	 * @param   nMode [in] : �һ� : ����/���� ( ����˺�� ������� �����˳� : ADD_AFTER )
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see  Count(), Remove()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagpVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *	assert( 2 == listDB.Count() );
	 *
	 *	listDB.Remove ();	//Remove element 1
	 *	assert( 1 == listDB.Count() );
	 *	...
	 * @endcode
	 *
	 */
	bool Add(const void* pData, int nMode = ADD_AFTER);
	
	// Search the list.
	/**
	 * @brief   ����̩ �輬���� �ܲ���.
	 <pre>
		����輬���� ����ʯ�� �����׷� ��� ̡���軤���� 
 		����輬���� �����ײ� ������ ���� �����ٳ�.
	 </pre>
	 *
	 * @param   pData [in] : ��˺�ز� ����̩ �Ѻ�
	 * @return  ���궦 ��˺�� true, ʭ���� false�� �󴮶���.
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *
	 *	assert( listDB.Find(pVec1) == true );
	 *	...
	 * @endcode
	 *
	 */
	bool Find(const void* pData) const;
	
	// Set current position of the list.
	/**
	 * @brief   �Ѻ϶� �����ٳ�.
	 *
	 * @param   nWhence	[in] : ����̡��
	 * @param   nIndex	[in] : ����̡���溷̩ ��̡��
	 * @return  �°��׷� true, ��²�׷� false
	 *
	 * @see  Data()
	 *
	 * @par Example:
	 * @code
	 *
	 *	PtrList listDB;
	 *	if ( !listDB.FlagCreate() )
	 *		listDB.Create();
	 *
	 *	Vec* pVec1 = new Vec;
	 *	Vec* pVec2 = new Vec;
	 *	listDB.Add (pVec1);	//Element 0
	 *	listDB.Add (pVec2);	//Element 1
	 *	assert( 2 == listDB.Count() );
	 *
	 *	listDB.SetPosition(PtrList::POS_BEGIN, 0);	//Set element 0
	 *	assert( pVec1 == listDB.Data() );
	 *	...
	 * @endcode
	 *
	 */
	bool SetPosition(int nWhence, int nIndex);
	
	// Remove an element from the list.
	/**
	 * @brief	��� ̡���溷̩ ̺�϶� �����ٳ�.
	 *
     * @return  void
	 *
	 * @see  Add()
	 *
	 */
	void Remove(void);

private:
	//! �ڰη�������� ���� �輬��
	void*	m_pData;
};

}

#endif // _PTRLIST_H_
