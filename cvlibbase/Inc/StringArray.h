
/*!
 * \file    StringArray.h
 * \ingroup base
 * \brief   �ͻ��۹ɵۼ���˾ ̡�� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _STRINGARRAY_H_
#define _STRINGARRAY_H_

#include "SString.h"
#include "Object.h"

namespace CVLib
{

/**
 * @brief �ͻ��۹ɵۿе���
 */
class CVLIB_DECLSPEC StringArray : public Object
{
public:
	//! �龢�� 
	StringArray();
	StringArray(const StringArray& t);
	//! �;˻� 
	virtual ~StringArray();
	
	// Attributes
	/**
	 * @brief   �ɵ�̩ �ͱ�(̺�Ϻ�)�� �в���.
	 *
	 * @return  ̺�Ϻ� 
	 *
	 * @see   SetSize()
	 *
	 */
	int GetSize() const;
	/**
	 * @brief   �ɵ�̩ ���˶� �в���.
	 *
	 * @return  ���� 
	 *
	 */
	int GetUpperBound() const;
	
	/**
	 * @brief   �ɵ�̩ �ͱ��� ���� �����ٳ�. 
	 *
	 * @param   nNewSize [in] : �����׵ز� �ͱ� 
	 * @param   nGrowBy  [in] : ���º� 
	 * @return  void
	 *
	 * @see  GetSize()
	 *
	 */
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	/**
	 * @brief   �ɵ�̩ �� ̰��������˾ �͸��ٳ�.
	 *
	 * @return  void
	 *
	 */
	void FreeExtra();
	
	/**
	 * @brief   �ɵ�˾ �͸��ٳ�.
	 *
	 * @return  void 
	 *
	 */
	void RemoveAll();
	
	// Accessing elements
	/**
	 * @brief   �輳�� ̡��̩ ̺�϶� �в���. 
	 *
	 * @param   nIndex [in] : ��˺�ز� ̺��̩ ̡������ 
	 * @return  ���̼� ̺�� 
	 *
	 * @see  SetAt(int nIndex, char* newElement), ElementAt()
	 *
	 */
	SString GetAt(int nIndex) const;
	
	/**
	 * @brief   �ɵ�̩ �輳�� ̡���� �� ̺�϶� �����ٳ�.
	 *
	 * @param   nIndex     [in] : ̡�� 
	 * @param   newElement [in] : �� ̺�� 
	 * @return  void
	 *
	 * @see   GetAt()
	 *
	 */
	void SetAt(int nIndex, char* newElement);
	
	/**
	 * @brief  �ɵ�̩ �輳�� ̡���� �� ̺�϶� �����ٳ�.
	 *
	 * @param   nIndex     [in] : ̡�� 
	 * @param   newElement [in] : �� ̺�� 
	 * @return  void
	 *
	 * @see   GetAt()
	 *
	 */
	void SetAt(int nIndex, SString& newElement);
	
	/**
	 * @brief   �輳�� ̡��̩ ̺�϶� �в���. 
	 *
	 * @param   nIndex [in] : ��˺�ز� ̺��̩ ̡������ 
	 * @return  ̺��̩ �Ѻ�  
	 *
	 * @see   GetAt()
	 *
	 */
	SString& ElementAt(int nIndex);
	
	// Direct Access to the element data (may return NULL)
	/**
	 * @brief   ����ɵ�˾ �в���.
	 *
	 * @return  ����ɵ� 
	 *
	 */
	const SString* GetData() const;
	
	/**
	 * @brief   ����ɵ�˾ �в���. 
	 *
	 * @return  ����ɵ� 
	 *
	 */
	SString* GetData();
	
	// Potentially growing the array
	/**
	 * @brief   �ɵ�̩ �輳�� ̡���� �� ̺�϶� �����ٳ�. 
	 *          ­���׳޷� �ɵ�˾ ���»��ֳ�.
	 *
	 * @param   nIndex     [in] : ̡�� 
	 * @param   newElement [in] : �� ̺�� 
	 * @return  void 
	 *
	 * @see  SetAt(int nIndex, char* newElement)
	 *
	 */
	void SetAtGrow(int nIndex, const char* newElement);
	
	/**
	 * @brief   �ɵ�̩ �輳�� ̡���� �� ̺�϶� �����ٳ�. 
	 *          ­���׳޷� �ɵ�˾ ���»��ֳ�. 
	 *
	 * @param   nIndex     [in] : ̡�� 
	 * @param   newElement [in] : �� ̺�� 
	 * @return  void 
	 *
	 * @see   SetAt(int nIndex, SString& newElement)
	 *
	 */
	void SetAtGrow(int nIndex, SString& newElement);
	
	/**
	 * @brief   �ɵ��� ������ ̺�϶� �����ٳ�. 
	 *
	 * @param   newElement [in] : �����׵ز� ̺�� 
	 * @return  ������ ̡������ 
	 *
	 */
	int Add(const char* newElement);
	
	/**
	 * @brief   �ɵ�̩ ������ ̺�϶� �����ٳ�.
	 *
	 * @param   newElement [in] : �����׵ز� ̺�� 
	 * @return  ������ ̡������ 
	 *
	 */
	int Add(SString& newElement);
	
	/**
	 * @brief   �ɵ��� �� �ɵ�˾ �����ٳ�.
	 *
	 * @param   src [in] : �����׵ز� �ɵ� 
	 * @return  ���� �ɵ�̩ �ͱ� 
	 *
	 */
	int Append(const StringArray& src);

	/**
	 * @brief   �ɵ�˾ �����ٳ�.
	 *
	 * @param   src [in] : ̺��ɵ� 
	 * @return  void
	 *
	 */
	void Copy(const StringArray& src);
	
	// overloaded operator helpers
	/**
	 * @brief   �輳�� ̡��̩ ̺�϶� �в���.
	 *
	 * @param   nIndex [in] : ��˺�ز� ̺��̩ ̡������
	 * @return  ���̼� ̺�� 
	 *
	 */
	SString operator[](int nIndex) const;

	/**
	 * @brief   �輳�� ̡��̩ ̺�϶� �в���.
	 *
	 * @param   nIndex [in] : ��˺�ز� ̺��̩ ̡������ 
	 * @return  ̺��̩ �Ѻ� 
	 *
	 */
	SString& operator[](int nIndex);
	
	// Operations that move elements around
	/**
	 * @brief   �輳�� ̡���Ṣ�� ���̼� ������ �� ̺�϶� �����ٳ�.
	 *
	 * @param   nIndex     [in] : �����׵ز� ̡������
	 * @param   newElement [in] : �� ̺�� 
	 * @param   nCount     [in] : ���� 
	 * @return  void
	 *
	 */
	void InsertAt(int nIndex, char* newElement, int nCount = 1);
	
	/**
	 * @brief   �輳�� ̡���Ṣ�� ���̼� ������ �� ̺�϶� �����ٳ�.
	 *
	 * @param   nIndex     [in] : �����׵ز� ̡������
	 * @param   newElement [in] : �� ̺�� 
	 * @param   nCount     [in] : ���� 
	 * @return  void
	 *
	 */
	void InsertAt(int nIndex, SString& newElement, int nCount = 1);
	
	/**
	 * @brief   �輳�� ̡���Ṣ�� ���̼� ������ ̺�ϴ�˾ �����ٳ�.
	 *
	 * @param   nIndex [in] : ̡������ 
	 * @param   nCount [in] : ���� 
	 * @return  void 
	 *
	 */
	void RemoveAt(int nIndex, int nCount = 1);

	/**
	 * @brief   �ɵ�̩ �輳�� ̡���� �޶� �ɵ�˾ �����ٳ�.
	 *
	 * @param   nStartIndex [in] : ̡������ 
	 * @param   pNewArray   [in] : �����׵ز� �ɵ� 
	 * @return  void
	 *
	 */
	void InsertAt(int nStartIndex, StringArray* pNewArray);
	
	StringArray& operator=(const StringArray& t);
	// Implementation
protected:
	//! ���� ����ɵ� 
	SString* m_pData;  
	//! �ɵ�̩ ���Ϻ� 
	int m_nSize;     
	//! �״����Ϻ� 
	int m_nMaxSize;  
	//! �ɵ�̩ ���º� 
	int m_nGrowBy;  
	
	void InsertEmpty(int nIndex, int nCount);
	
protected:
	// local typedefs for class templates
	typedef SString BASE_TYPE;
	typedef char* BASE_ARG_TYPE;
};

}

#endif //_STRINGARRAY_H_
