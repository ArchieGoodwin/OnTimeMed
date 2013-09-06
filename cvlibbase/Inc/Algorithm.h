/*!
 * \file	Algorithm.h
 * \ingroup base
 * \brief	���� ʲ�ֶ���̩ ����е����� ��˦��� �е����� ��̩��.
 * \author	����˧
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "PtrArray.h"
#include "SString.h"
#include "CoImage.h"

namespace CVLib
{

/**
 * @brief  ID̩ �״ݱ���
 */
#define CVLIB_IDLEN			256

/**
 * @brief  ʲ�ֶ���̩ ����е���
 */
class CVLIB_DECLSPEC Algorithm : public Object
{
public:
	//!	�龢��
	Algorithm();
	//!	�;˻�
	virtual ~Algorithm();

	/** 
	 * @brief   ʲ�ֶ���˾ �����ײ� �ܺ��˳�.
	 *
	 * @param 	pAlgorithm [in] : �����׵ز� ʲ�ֶ���
	 * @return 	ʲ�ֶ���̩ ����
	 *
	 * @see  DelAlgorithm(), RemoveAlgorithms()
	 *
	 * @par Example:
	 * @code
	 *	...
	 *  int nCount = 0;
	 *	
	 *  Algorithm total;
	 *  Algorithm sub1;
	 *  Algorithm sub2;
	 *  Algorithm sub3;
	 *  ...
	 *  total.AddAlgorithm(&sub1);
	 *  total.AddAlgorithm(&sub2);
	 *
	 *  total.SetAlgorithm(2, &sub3);  
	 *  nCount = total.GetAlgorithmCount();
	 *  assert (nCount == 3);
	 * 
	 *  temp = total.GetAlgorithm(2);
	 *  assert(temp != Null);
	 *
	 *  total.DelAlgorithm(2);
	 *  assert (nCount == 2);
	 *
	 *  total.RemoveAlgorithms();
	 *  assert (nCount == 0);
	 *  ...
	 * @endcode
	 *
	 */
	int AddAlgorithm (Algorithm* pAlgorithm);

	/** 
	 * @brief	ʲ�ֶ���˾ �б� ̡�� �ܺ��˳�.
	 * @param	nIdx [in] : ��˺�ز� ʲ�ֶ���̩ ����
	 * @return	������ �ඣ�� ʲ�ֶ���
	 *
	 * @see  AddAlgorithm()
	 *
     */
	Algorithm* GetAlgorithm (int nIdx) const;

	/**
	 * @brief   ʲ�ֶ���˾ �����ױ� ̡�� �ܺ��˳�. 
	 *
	 * @param   nIdx       [in] : �����׵ز� ʲ�ֶ���̩ ���� 
	 * @param   pAlgorithm [in] : �����׵ز� ʲ�ֶ��� 
	 * @return  void 
	 *
	 * @see  AddAlgorithm()
	 *
	 */
	void SetAlgorithm (int nIdx, Algorithm* pAlgorithm);

	/** 
	 * @brief	ʲ�ֶ���̩ ���㶦 �б� ̡�� �ܺ��˳�.
	 * 
	 * @return	ʲ�ֶ���̩ ����
	 *
	 * @see  AddAlgorithm(),  GetAlgorithm()
	 *
	 */
	int GetAlgorithmCount() const;

	/** 
	 * @brief	ʲ�ֶ���˾ ���� �����ױ� ̡�� �ܺ��˳�.
	 * 
	 * @return	void
	 *
	 * @see  AddAlgorithm(),  DelAlgorithm()
	 *
	 */
	void RemoveAlgorithms ();
	
	/** 
	 * @brief	ʲ�ֶ���˾ �����ױ� ̡�� �ܺ��˳�.
	 *
	 * @param	nIdx [in] : �����׵ز� ʲ�ֶ���̩ ����
	 * @return	void
	 *
	 * @see  AddAlgorithm()
	 *
	 */
	void DelAlgorithm(int nIdx);

	/** 
	 * @brief	ʲ�ֶ���̩ ID�� �б� ̡�� �ܺ��˳�.
	 * 
	 * @return	ʲ�ֶ��� ID
	 *
	 * @see  SetSelfID(), GetSelfID() 
	 *
	 */
	virtual const char* GetID ();

	/** 
	 * @brief	���� ID�� �����ױ� ̡�� �ܺ��˳�.
	 *
	 * @param	szID [in] : ʲ�ֶ��� ID
	 * @return	void
	 *
	 * @see  AddAlgorithm(), GetSelfID()
	 *
	 * @par Example:
	 * @code
	 *	...
	 *	char* szTotalID = NULL;
	 *  char* szSubID1 = NULL; 
	 *	
	 *  Algorithm total;
	 *  Algorithm sub1;
	 *  Algorithm sub2;
	 *  
	 *	sub1.SetSelfID("sub1");
	 *	sub2.SetSelfID("sub2");
	 *  total.SetSelfID("total");
	 *  total.AddAlgorithm(&sub1);
	 *  total.AddAlgorithm(&sub2);
     *
	 *	szTotalID = total.GetID();
	 *	assert (!strcmp(szTotalID, "total[sub1][sub2]"));
	 *
	 *	szSubID1 = sub1.GetSelfID();
	 *	assert (!strcmp(szSubID1, "sub1"));
	 *  ...
	 * @endcode
	 *
	 */
	void SetSelfID (const char* szID);

	/** 
	 * @brief	����̩ ʲ�ֶ��� ID�� �б� ̡�� �ܺ��˳�.
	 * 
	 * @return	ʲ�ֶ��� ID
	 *
	 * @see  SetSelfID()
	 *
	 */
	const char* GetSelfID() const;
protected:
	//! ����ID
	char m_szID[CVLIB_IDLEN];
	//! ����ID
	char m_szTotalID[CVLIB_IDLEN];
	//! ʲ�ֶ���̩ �ɵ�
	PtrArray	m_AlgorithmArray;
};

}

#endif //_ALGORITHM_H_