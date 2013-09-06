
/*!
 * \file    SString.h
 * \ingroup base
 * \brief   �ͻ��ۼ���˾ ̡�� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _CVLIB_STRING_H_
#define _CVLIB_STRING_H_

#include "cvlibbaseDef.h"
	
namespace CVLib
{
/**
 * @brief �ͻ��ۻ���鼿��
 */
struct StringData
{
	int nRefs;              /**< �ؼ����� */
	int nDataLength;        /**< �ͻ��ۻ���̩ ���� (�ǳ�����) */
	int nAllocLength;       /**< �ڳ�� ���ͱ����� */
	// char data[nAllocLength]

	char* data()           // char* to managed data
	{ return (char *)(this+1); }
};

/**
 * @brief �ͻ��ۼ���˾ ̡�� �е���
 */
class CVLIB_DECLSPEC SString
{
public:
// Constructors
	//! �θɰ龢�� 
	SString();
	
	/**
	 * @brief   �龢��(����)
	 *
	 * @param   stringSrc [in] : ̺���ﹶ���� 
	 *
	 */
	SString(const SString& stringSrc);

	/**
	 * @brief   �龢��(�ٱ� �ͻ��� ���̼� ����̩ �ͻ���˾ �龢�ٳ�.)
	 *
	 * @param   ch      [in] : �ӵٷͻ�
	 * @param   nLength [in] : �ͻ���̩ ���� 
	 *
	 */
	SString(char ch, int nLength=1);

	/**
	 * @brief   �龢�� 
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * 
	 */
	SString(char * lpsz);
	
	/**
	 * @brief   �龢�� 
	 *
	 * @param   lpsz [in] : �ӵٷͻ���
	 *
	 */
	SString(const char * lpsz);

	/**
	 * @brief   �龢�� (�ӵٷͻ��۵Ṣ�� �����ͻ���˾ �龢�ٳ�.)
	 *
	 * @param  lpch    [in] : �ӵٷͻ���
	 * @param  nLength [in] : �����ͻ��۱��� 
	 *
	 */
	SString(char * lpch, int nLength);
	
// Attributes & Operations
	/**
	 * @brief   �ͻ���̩ ���˶� �в���.
	 *
	 * @return  �ͻ���̩ ���� 
	 *
	 */
	int GetLength() const;
	
	/**
	 * @brief   �� �ͻ����Ͱ��� �����ٳ�.
	 *
	 * @return  �� �ͻ����˷� ture, ʭ���� false
	 *
	 */
	DWORD IsEmpty() const;
	
	/**
	 * @brief   �ͻ���˾ �� �ͻ��۵� �����ٳ�.
	 *
	 * @return  void
	 *
	 */
	void Empty();

	/**
	 * @brief   �ͻ����溷 �輳�� ̡��̩ �ͻ��� �в���.(0 - ����)
	 *
	 * @param   nIndex [in] : ̡������ 
	 * @return  ���̼� �ͻ� 
	 *
	 * @see   SetAt()
	 *
	 */
	BYTE GetAt(int nIndex) const;

	/**
	 * @brief   �ͻ����溷 �輳�� ̡��̩ �ͻ��� �в���.(0 - ����)
	 *
	 * @param   nIndex [in] : ̡������ 
	 * @return  ���̼� �ͻ� 
	 *
	 * @see   GetAt(), SetAt()
	 *
	 */
	BYTE operator[](int nIndex) const;

	/**
	 * @brief   �ͻ���̩ �輳�� ̡���� �� �ͻ��� �����ٳ�.
	 *
	 * @param   nIndex [in] : ̡������ 
	 * @param   ch     [in] : �ӵٷͻ� 
	 * @return  void
	 *
	 * @see  GetAt()
	 *
	 */
	void SetAt(int nIndex, char ch);

	// overloaded assignment
	/**
	 * @brief   �ͻ��ۻ���̩ ������ �����ٳ�.
	 *
	 * @param   stringSrc [in] : ̺���ﹶ���� 
	 * @return  ������ �ﹶ����   
	 *
	 * @see  SString(SString & stringSrc)
	 *
	 */
	const SString& operator=(const SString & stringSrc);

	/**
	 * @brief   �ͻ��ۻ��궦 �����ٳ�.
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * @return  ������ ���� 
	 *
	 * @see   SString(char * lpsz)
	 *
	 */
	const SString& operator=(const char* lpsz);

	/**
	 * @brief   �� �ͻ��� �� �ͻ��ۻ��궦 �龢�ٳ�.
	 *
	 * @param   ch [in] : �ӵٷͻ� 
	 * @return  �ͻ��ۻ��� 
	 *
	 * @see   SString(char ch, int nLength)
	 *
	 */
	const SString& operator=(char ch);

	/**
	 * @brief   �ͻ���˾ �����ٳ�.
	 *
	 * @param   string [in] : �ӵٷͻ��ۻ��� ��
	 * @return  ������ �ͻ��ۻ��� 
	 *
	 */
	const SString& operator+=(const SString& string);

	/**
	 * @brief   �ͻ����� �� �ͻ��� �����ٳ�.
	 *
	 * @param   ch [in] : �ӵٷͻ� 
	 * @return  ������ �ͻ��ۻ���
	 *
	 */
	const SString& operator+=(char ch);

	/**
	 * @brief   �ͻ���˾ �����ٳ�.
	 *
	 * @param  lpsz [in] : �ӵٷͻ��� 
	 * @return ������ �ͻ��ۻ��� 
	 *
	 */
	const SString& operator+=(const char* lpsz);

	/**
	 * @brief   �ͻ���˾ �в���.
	 *
	 * @return  �ͻ��ۼ輬�� 
	 *
     */
	const char * GetLPCTSTR() const {return m_pchData;}

	/**
	 * @brief   �ͻ���˾ �в���.
	 *
	 * @return  �ͻ��ۼ輬�� 
	 *
	 */
	operator const char*() const
	{
		return m_pchData;
	}

	// string comparison
	/**
	 * @brief   �ͻ���̩ ���䶦 �����ٳ�.
	 *
	 * @param   pStr [in] : �ͻ��ۻ��� 
	 * @return  �� �ͻ����� ��˺�� 0, �ӵٷͻ��۸�� ��˺�� <0, �ӵٷͻ��۸�� �ͷ� >0
	 *
	 */
	int Compare(const SString& pStr) const;

	// straight character comparison
	/**
	 * @brief   �ͻ���̩ ���䶦 �����ٳ�. 
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * @return  �� �ͻ����� ��˺�� 0, �ӵٷͻ��۸�� ��˺�� <0, �ӵٷͻ��۸�� �ͷ� >0
	 *
	 */
	int Compare(const char * lpsz) const;

	// compare ignoring case
	/**
	 * @brief   �ͻ���̩ ���䶦 �����ٳ�. 
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * @return  �� �ͻ����� ��˺�� 0, �ӵٷͻ��۸�� ��˺�� <0, �ӵٷͻ��۸�� �ͷ� >0
	 * 
	 */
	int CompareNoCase(const char * lpsz) const;
	
	// NLS aware comparison, case sensitive
	/**
	 * @brief   �ͻ���̩ ���䶦 �����ٳ�.
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * @return  �� �ͻ����� ��˺�� 0, �ӵٷͻ��۸�� ��˺�� <0, �ӵٷͻ��۸�� �ͷ� >0
	 *
	 * @see   Compare()
	 *
	 */
	int Collate(char * lpsz) const;

	// NLS aware comparison, case insensitive
	/**
	 * @brief   �ͻ���̩ ���䶦 �����ٳ�.
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * @return  Compare(char * lpsz)
	 *
	 */
	int CollateNoCase(char * lpsz) const;

	// simple sub-string extraction

  /******************************************************/
  //MD_906 by pch
	/**
	 * @brief   �輳�� ̡���Ṣ�� �輳�� ���˶�� �����ͻ���˾ �����ٳ�.
	 *
	 * @param   nFirst [in] : ����̡�� 
	 * @param   nCount [in] : ���� 
	 * @return  �����ͻ��� 
	 *
	 */
	SString Mid(int nFirst, int nCount) const;

	/**
	 * @brief   �輳�� ̡���Ṣ�� �ͻ���̩ ��Ĵ�� �����ͻ���˾ �����ٳ�.
	 *
	 * @param   nFirst [in] : ����̡�� 
	 * @return  �����ͻ��� 
	 *
	 * @see   Mid(int nFirst, int nCount)
	 *
	 */
	SString Mid(int nFirst) const;
	
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� ����Ĵ��̩ �����ͻ���˾ ������.
	 *
	 * @param   nCount [in] : �����ͻ���̩ ���� 
	 * @return  �����ͻ��� 
	 *
	 * @see   Right()
	 *
	 */
	SString Left(int nCount) const;

	/**
	 * @brief   �ͻ���̩ ���溷���� �輳�� ����Ĵ��̩ �����ͻ���˾ ������. 
	 *
	 * @param   nCount [in] : �����ͻ���̩ ���� 
	 * @return  �����ͻ��� 
	 *
	 * @see  Left()
	 *
	 */
	SString Right(int nCount) const;
  /******************************************************/

	/**
	 * @brief   �ͻ���˾ �輳�� ������ ������ Ⱥ��.
	 *
	 * @param   lpszFormat [in] : 
	 * @return  void
	 *
	 */
	void Format(char * lpszFormat, ...);
	
	/**
	 * @brief   �ͻ���˾ �輳�� ������ ������ Ⱥ��. 
	 *
	 * @param   lpszFormat [in] : 
	 * @param   argList    [in] :
	 * @return  void
	 *
	 */
	void FormatV(char * lpszFormat, va_list argList);
  
  /******************************************************/
  //MD_906 by pch
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� lpszCharSetʯ̩ ���� �ͻ��� �����ײ� 
	 *          �����ͻ���˾ �в���. 
	 *
	 * @param   lpszCharSet [in] : �ͻ����� 
	 * @return  ���̼� �����ͻ��� 
	 *
	 * @see  SpanExcluding()
	 *
	 */
	SString SpanIncluding(const char * lpszCharSet) const;

	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� lpszCharSetʯ̩ �ͻ��� ʭ�� 
     *          �ͻ���˾ �����ײ� �����ͻ���˾ �в���. 
	 *
	 * @param   lpszCharSet [in] : �ͻ����� 
	 * @return  ���̼� �����ͻ��� 
	 *
	 * @see   SpanIncluding()
	 *
	 */
	SString SpanExcluding(const char * lpszCharSet) const;
  /******************************************************/
  
	// upper/lower/reverse conversion
	/**
	 * @brief   �ͻ���ʯ̩ ���� �ͻ���˾ �ݷͻ��� �����ٳ�. 
	 *
	 * @return  ������ �ͻ��� 
	 *
	 * @see   MakeLower()
	 *
	 */
	void MakeUpper();

	/**
	 * @brief   �ͻ���ʯ˺ ���� �ͻ���˾ �Ϸͻ��� �����ٳ�. 
	 *
	 * @return  ������ �ͻ��� 
	 *
	 * @see  MakeUpper()
	 *
	 */
	void MakeLower();

	/**
	 * @brief   �ͻ���˾ �������.
	 *
	 * @return  void
	 *
	 */
	void MakeReverse();

	// trimming whitespace (either side)
	/**
	 * @brief   �ͻ���̩ ���溷���� ���˾ �����ٳ�.
	 *
	 * @return  void 
	 *
	 * @see   TrimLeft()
	 *
	 */
	void TrimRight();

	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� ���˾ �����ٳ�.
	 *
	 * @return  void
	 *
	 * @see   TrimRight()
	 *
	 */
	void TrimLeft();

	// trimming anything (either side)
	/**
	 * @brief   �ͻ���̩ ���溷���� �輳�� �ͻ��� �����ٳ�. 
	 *
	 * @param   chTarget [in] : �����׵ز� �ͻ� 
	 * @return  void 
	 *
	 */
	void TrimRight(char chTarget);

	/**
	 * @brief   �ͻ���̩ ���溷���� �輳�� �ͻ���˾ �����ٳ�. 
	 *
	 * @param   lpszTargets [in] : �����׵ز� �ͻ��� 
	 * @return  void 
	 *
	 */
	void TrimRight(char * lpszTargets);
	
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� �ͻ��� �����ٳ�. 
	 *
	 * @param   chTarget [in] : �����׵ز� �ͻ� 
	 * @return  void
	 *
	 */
	void TrimLeft(char chTarget);
	
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� �ͻ���˾ �����ٳ�.
	 *
	 * @param   lpszTargets [in] : �����׵ز� �ͻ��� 
	 * @return  void 
	 *
	 */
	void TrimLeft(char * lpszTargets);

	// advanced manipulation
	/**
	 * @brief   �ͻ���̩ �輳�� �ͻ��� ����˩ �ͻ��� ����ٳ�. 
	 *
	 * @param   chOld [in] : ����׵ز� �ͻ� 
	 * @param   chNew [in] : �� �ͻ� 
	 * @return  ����� ���� 
	 *
	 */
	int Replace(char chOld, char chNew);
	
	/**
	 * @brief   �ͻ���̩ �輳�� �����ͻ���˾ ����˩ �ͻ��۵� ����ٳ�. 
	 *
	 * @param   lpszOld [in] : ����׵ز� �ͻ��� 
	 * @param   lpszNew [in] : �� �ͻ��� 
	 * @return  ����� ���� 
	 *
	 */
	int Replace(char * lpszOld, char * lpszNew);
	
	/**
	 * @brief   �ͻ���˾ ����˩ �ͻ��۵� ����ٳ�. 
	 *
	 * @param   Old [in] : ����׵ز� �ͻ��� 
	 * @param   New [in] : �� �ͻ��� 
	 * @return  ����� ���� 
	 *
	 */
	int Replace(SString Old, SString New);
	
	/**
	 * @brief   �ͻ����溷 �輳�� �ͻ��� �����ٳ�. 
	 *
	 * @param   chRemove [in] : �����׵ز� �ͻ� 
	 * @return  ������ ���� 
	 *
	 * @see   Insert()
	 *
	 */
	int Remove(char chRemove);

	/**
	 * @brief   �ͻ���̩ �輳�� ̡���� �輳�� �ͻ��� �����ٳ�. 
	 *
	 * @param   nIndex [in] : ̡�� 
	 * @param   ch     [in] : �����׵ز� �ͻ� 
	 * @return  �� �ͻ���̩ ���� 
	 *
	 * @see   Remove()
	 *
	 */
	int Insert(int nIndex, char ch);

	/**
	 * @brief   �ͻ���̩ �輳�� ̡���� �ͻ���˾ �����ٳ�. 
	 *
	 * @param   nIndex [in] : ̡�� 
	 * @param   pstr   [in] : �����׵ز� �ͻ��� 
	 * @return  �� �ͻ���̩ ���� 
	 *
	 */
	int Insert(int nIndex, char * pstr);
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� ���̼� ������� �ͻ��� �����ٳ�. 
	 *
	 * @param   nIndex [in] : ̡�� 
	 * @param   nCount [in] : �����׵ز� ���� 
	 * @return  �ͻ���̩ �������� 
	 *
	 */
	int Delete(int nIndex, int nCount = 1);

	// searching
	/**
	 * @brief   �ͻ���̩ ���溷���� �輳�� �ͻ��� �ܲ���. 
	 *
	 * @param   ch [in] : ��˺�ز� �ͻ� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int ReverseFind(char ch) const;
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� ��ɸ˺�� ������ �輳�� �ͻ���˾ �ܲ���. 
	 *
	 * @param   lpszSub    [in] : ��˺�ز� �ͻ��� 
	 * @param   nBackStart [in] : ����̡�� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int ReverseFind(char* lpszSub, int nBackStart = 0) const;

	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� �ﶥɸ˺�� ������ ���� �ͻ���˾ �ܲ���. 
	 *
	 * @param   pStr   [in] : ��˺�ز� �ͻ��� 
	 * @param   nStart [in] : ����̡�� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(SString *pStr, int nStart) const;
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� �ﶥɸ˺�� ������ �輳�� �ͻ���˾ �ܲ���. 
	 *
	 * @param   Str    [in] : ��˺�ز� �ͻ��� 
	 * @param   nStart [in] : ����̡�� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(SString Str, int nStart) const;
	
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� �ͻ���˾ �ܲ���. 
	 *
	 * @param   Str [in] : ��˺�ز� �ͻ��� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(SString Str) const;
	
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� �ͻ���˾ �ܲ���. 
	 *
	 * @param   pStr [in] : ��˺�ز� �ͻ��� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(SString *pStr) const;
	
	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� �ͻ��� �ܲ���. 
	 *
	 * @param   ch [in] : ��˺�ز� �ͻ� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(char ch) const;
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� �ﶥɸ˺�� ������ �輳�� �ͻ��� �ܲ���. 
	 *
	 * @param   ch     [in] : ��˺�ز� �ͻ� 
	 * @param   nStart [in] : ����̡�� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(char ch, int nStart) const;

	/**
	 * @brief   �ͻ���̩ ����˺�Ṣ�� �輳�� �ͻ���˾ �ܲ���.
	 *
	 * @param   lpszSub [in] : ��˺�ز� �ͻ��� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(char*lpszSub) const;
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� �ﶥɸ˺�� ������ �輳�� �ͻ���˾ �ܲ���. 
	 *
	 * @param   lpszSub [in] : ��˺�ز� �ͻ��� 
	 * @param   nStart  [in] : ����̡�� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int Find(char*lpszSub, int nStart) const;
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� �輳�� �ͻ�����ʯ̩ �ͻ���˾ �ܲ���. 
	 *
	 * @param   lpszCharSet [in] : �ͻ����� 
	 * @param   nStart      [in] : ����̡�� 
	 * @return  ���� ��˼ ̡�� 
	 *
	 */
	int FindOneOf(const char * lpszCharSet, int nStart = 0) const;
	
	/**
	 * @brief   �ͻ���̩ �輳�� ̡���Ṣ�� ��˼ �����ͻ���̩ ���㶦 �в���. 
	 *
	 * @param   lpszSub [in] : ��˺�ز� �����ͻ��� 
	 * @param   nStart  [in] : ����̡�� 
	 * @return  ��˼ ����  
	 *
	 */
	int GetCharCount(char *lpszSub,int nStart = 0) const;

	// get pointer to modifiable buffer at least as int as nMinBufLength
	/**
	 * @brief   �׺��� ���̼� ���˵� ��Ҵ� �ͻ���̰����̩ �Ѻ϶� �в���.    
	 *
	 * @param   nMinBufLength [in] : �ͻ��۱��� 
	 * @return  �ͻ���̰�����Ѻ� 
	 *
	 */
	char * GetBuffer(int nMinBufLength);
	
	/**
	 * @brief   �ͻ���̰������ �͸��װ� �ͻ���̩ ���˶� ���� �����ٳ�.
	 *
	 * @param   nNewLength [in] : �����׵ز� ���� 
	 * @return  void
	 *
	 * @see   FreeExtra()
	 *
	 */
	void ReleaseBuffer(int nNewLength = -1);

	// get pointer to modifiable buffer exactly as int as nNewLength
	/**
	 * @brief   ���̼� ���˵� ��Ҵ� �ͻ���̰����̩ �Ѻ϶� �в���.
	 *
	 * @param   nNewLength [in] : �ͻ��۱��� 
	 * @return  �ͻ���̰�����Ѻ� 
	 *
	 * @see   GetBuffer()
	 *
	 */
	char * GetBufferSetLength(int nNewLength);
	// release memory allocated to but unused by string
	/**
	 * @brief    �� ̰��������˾ �͸��ٳ�.
	 *
	 * @return   void
	 *
	 * @see   ReleaseBuffer()
	 *
	 */
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off

	// turn refcounting back on
	/**
	 * @brief   nRef = -1�� �����ٳ�.
	 *
	 * @return  �ͻ��ۼ輬��  
	 *
	 */
	char * LockBuffer();
	// turn refcounting off
	/**
	 * @brief   nRef = -1�� �����ٳ�.
	 *
	 * @return   void
	 *
	 */
	void UnlockBuffer();
	int FromFile(FILE* pfile);
	int ToFile(FILE* pfile);

// Implementation
public:
	//! �;˻� 
	~SString();

	/**
	 * @brief   �ڳ�� �ͻ���̰�����ͱ��� �в���.
	 *
	 * @return  ̰�����ͱ� 
	 *
	 */
	int         GetAllocLength() ;

	/**
	 * @brief   �ͻ��ۻ��궦 �в���. 
	 *
	 * @return  �ͻ��ۻ���輬�� 
	 *
	 */
	StringData* GetData() const;

	/**
	 * @brief   ���̼� �� �ͻ���˾ �ͻ���̰������ �ڰθ����ٳ�. 
	 *
	 * @param   nSrc1Len     [in] : �� �ͻ���̩ ���� 
	 * @param   lpszSrc1Data [in] : �� �ͻ��ۼѺ� 
	 * @param   nSrc2Len     [in] : ������ �ͻ���̩ ���� 
	 * @param   lpszSrc2Data [in] : ������ �ͻ��ۼѺ� 
	 * @return  void 
	 *
	 */
	void        ConcatCopy(int nSrc1Len, const char * lpszSrc1Data, int nSrc2Len, const char * lpszSrc2Data);

	/**
	 * @brief   �ͻ���̩ ���˶� �в���. 
	 *
	 * @param   lpsz [in] : �ӵٷͻ��� 
	 * @return  �ͻ���̩ ���� 
	 *
	 */
	int         SafeStrlen(const char * lpsz) const;

	//!  �ͻ��ۼ輬�� 
	char*       m_pchData;   

protected:

	// implementation helpers

	void Init();
	void AllocCopy(SString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, const char * lpszSrcData);

	void ConcatInPlace(int nSrcLen, const char * lpszSrcData);
	void CopyBeforeWrite();
	void AllocBeforeWrite(int nLen);
	void Release();
	void Release(StringData* pData);

	void  FreeData(StringData* pData);

};

// #ifdef VC_EXTRALEAN
// inline SString GetString(SString Str)
// {
// 	return (char*)const char*(Str);
// }
// #endif

inline DWORD CVLIB_DECLSPEC operator==( SString& s1,  SString& s2)
	{ return s1.Compare(s2) == 0; }
inline DWORD CVLIB_DECLSPEC operator==( SString& s1, char * s2)
	{ return s1.Compare(s2) == 0; }
inline DWORD CVLIB_DECLSPEC operator==(char * s1,  SString& s2)
	{ return s2.Compare(s1) == 0; }
inline DWORD CVLIB_DECLSPEC operator!=( SString& s1,  SString& s2)
	{ return s1.Compare(s2) != 0; }
inline DWORD CVLIB_DECLSPEC operator!=( SString& s1, char * s2)

	{ return s1.Compare(s2) != 0; }
inline DWORD CVLIB_DECLSPEC operator!=(char * s1,  SString& s2)
	{ return s2.Compare(s1) != 0; }

inline DWORD CVLIB_DECLSPEC operator<( SString& s1,  SString& s2)
	{ return s1.Compare(s2) < 0; }
inline DWORD CVLIB_DECLSPEC operator<( SString& s1, char * s2)
	{ return s1.Compare(s2) < 0; }
inline DWORD CVLIB_DECLSPEC operator<(char * s1,  SString& s2)
	{ return s2.Compare(s1) > 0; }
inline DWORD CVLIB_DECLSPEC operator>( SString& s1,  SString& s2)
	{ return s1.Compare(s2) > 0; }
inline DWORD CVLIB_DECLSPEC operator>( SString& s1, char * s2)
	{ return s1.Compare(s2) > 0; }
inline DWORD CVLIB_DECLSPEC operator>(char * s1,  SString& s2)
	{ return s2.Compare(s1) < 0; }
inline DWORD CVLIB_DECLSPEC operator<=( SString& s1,  SString& s2)
	{ return s1.Compare(s2) <= 0; }
inline DWORD CVLIB_DECLSPEC operator<=( SString& s1, char * s2)
	{ return s1.Compare(s2) <= 0; }

inline DWORD CVLIB_DECLSPEC operator<=(char * s1,  SString& s2)
	{ return s2.Compare(s1) >= 0; }
inline DWORD CVLIB_DECLSPEC operator>=( SString& s1,  SString& s2)
	{ return s1.Compare(s2) >= 0; }

inline DWORD CVLIB_DECLSPEC operator>=( SString& s1, char * s2)
	{ return s1.Compare(s2) >= 0; }
inline DWORD CVLIB_DECLSPEC operator>=(char * s1,  SString& s2)
	{ return s2.Compare(s1) <= 0; }

#define IsTCharCode(c) ((((BYTE)(c)) > 0x7F) ? (TRUE) : (FALSE))
#define IsSpaceCode(c) (((c) == 0x20 || (c) == 0xA1A1) ? (TRUE) : (FALSE))
	
/*increase the pointer to current string, appositing the multibyte string*/
CVLIB_DECLSPEC inline BYTE* cbsinc(const BYTE * current)
{
  BYTE * rslt;
  rslt = (BYTE *)current;
  if(*rslt > 0x80)
    rslt += 2;
  else
    rslt++;
  return rslt;
}

CVLIB_DECLSPEC inline BYTE* cbsstr(const BYTE *string1, const BYTE *string2)
{
	BYTE * pszFind = NULL ;

	if(string1 == NULL || string2 == NULL)
		return pszFind ;
	
	int nLen1 = (int)strlen((const char*)string1) ;
	int nLen2 = (int)strlen((const char*)string2) ;
	if(nLen1 == 0 || nLen2 == 0)
		return pszFind ;

	int nIdx1 = 0 ;
	int nIdx2 = 0 ;
	while( nIdx1 < nLen1 - nLen2 + 1 )
	{
		nIdx2 = 0 ;
		while( nIdx2 < nLen2 && string2[nIdx2] == string1[nIdx1 + nIdx2] )
		{
			nIdx2++ ;
		}
		if( nIdx2 >= nLen2 )
		{
			pszFind = (BYTE*)(string1 + nIdx1);
			break ;
		}
		if(IsTCharCode(string1[nIdx1]))
			nIdx1 += 2;
		else
			nIdx1++ ;
	}
	
	return pszFind ;
}

CVLIB_DECLSPEC inline BYTE* cbschr(const BYTE *strSrc, int nCode)
{
	BYTE * pszFind = NULL ;
	int nLen = (int)strlen((const char*)strSrc) ;
	int nIdx = 0 ;
	while( nIdx < nLen )
	{
		int nSrcCode = strSrc[nIdx] ;
		if(IsTCharCode(strSrc[nIdx]))
			nSrcCode = (nSrcCode << 8) + (strSrc[nIdx+1] & 0xFF) ;

		if(nCode == nSrcCode)
		{
			pszFind = (BYTE*)(strSrc + nIdx);
			break ;
		}

		if(IsTCharCode(strSrc[nIdx]))
			nIdx += 2;
		else
			nIdx++ ;
	}
	
	return pszFind ;
}

// Compare helpers
bool CVLIB_DECLSPEC operator==(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator==(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator==(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator!=(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator!=(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator!=(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator<(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator<(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator<(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator>(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator>(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator>(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator<=(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator<=(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator<=(const char* s1, const SString& s2);
bool CVLIB_DECLSPEC operator>=(const SString& s1, const SString& s2);
bool CVLIB_DECLSPEC operator>=(const SString& s1, const char* s2);
bool CVLIB_DECLSPEC operator>=(const char* s1, const SString& s2);
SString CVLIB_DECLSPEC operator+(const SString& string1, const SString& string2);
SString CVLIB_DECLSPEC operator+(const SString& string, char ch);
SString CVLIB_DECLSPEC operator+(char ch, const SString& string);
SString CVLIB_DECLSPEC operator+(const SString& string, const char* lpsz);
SString CVLIB_DECLSPEC operator+(const char* lpsz, const SString& string);

}

#endif //_CVLIB_STRING_H_
