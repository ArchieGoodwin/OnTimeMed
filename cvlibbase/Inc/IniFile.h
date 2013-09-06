
/*! 
 * \file    IniFile.h
 * \ingroup base
 * \brief   ini����˾ �ϰ� ȸ�� �����ױ� ̡�� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _INIFILE_H_
#define _INIFILE_H_

// C++ Includes
#include "StringArray.h"
#include "SString.h"
#include "XFileDisk.h"
#include "PtrArray.h"

#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048

namespace CVLib
{
/**
 * @brief ini���μ���˾ ̡�� �е���
 */
class CVLIB_DECLSPEC IniFile  
{
private:
	//!  Match case �������� 
	bool   m_fCaseInsensitive;
	//! ���ΰҵ� 
	SString m_szPath;
	/**
	 * @brief �Լ�̩ �е���
	 */
	class CVLIB_DECLSPEC Key
	{
	public:
		//!  �龢�� 
		Key ();

		/**
		 * @brief   �龢��(���̼� key�Ṣ�� �� key�� �龢�ٳ�.)
		 *
		 * @param   key [in] : �ӵ�key 
		 *
		 */
		Key (const Key& key);

		//!  �;˻� 
		virtual ~Key();

		//!  key�˶� 
		StringArray	names;
		//!  key�� 
		StringArray values; 
		//!  key������ 
		StringArray comments;
	};
	//! key�ɵ� 
//	Array<Key>	m_Keys;
	PtrArray	m_Keys;
	//! key�˶��ɵ� 
	StringArray m_Names; 
	//! key�����͹ɵ� 
	StringArray m_Comments;

	/**
	 * @brief   Match case�� �����װ� ���� ���� �ͻ���˾ �����ٳ�. 
	 *
	 * @param   s [in] : �ӵٷͻ��� 
	 * @return  �α�ͻ��� 
	 *
	 */
	SString CheckCase( SString s) const;

public:
	/**
	 * @brief ��˱�� �輳�ײ� �۰���
	 */
	enum errors
	{
		noID = -1   /**< ��˱���� */
	};

	//! �龢�� 
	IniFile(const SString iniPath = "");
	//! �;˻� 
	virtual ~IniFile() 
	{
		Reset();
	}

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.

	/**
	 * @brief   Match case�� Sensitive�� �����ٳ�.
	 *
	 * @return  void 
	 *
	 */
	void CaseSensitive() 
	{
		m_fCaseInsensitive = false;
	}
	/**
	 * @brief   Match case�� Insensitive�� �����ٳ�.
	 *
	 * @return  void 
	 *
	 */
	void CaseInsensitive()                         
	{
		m_fCaseInsensitive = true;
	}

	// Sets m_szPath of ini file to read and write from.
	/**
	 * @brief   ini����̩ �ҵᶦ �����ٳ�.
	 *
	 * @param   newPath [in] : �ҵ� 
	 * @return  void
	 *
	 */
	void Path(const SString newPath)                
	{
		m_szPath = newPath;
	}
	
	/**
	 * @brief   ini����̩ �ҵᶦ �в���. 
	 *
	 * @return  ����̩ �ҵ� 
	 *
	 */
	
	SString Path()
	{
		return m_szPath;
	}

	/**
	 * @brief   ini����̩ �ҵᶦ �����ٳ�.
	 *
	 * @param   newPath [in] : ���ΰҵ� 
	 * @return  void
	 *
	 */
	void SetPath(const SString newPath)             
	{
		Path( newPath);
	}

	/**
	 * @brief   �ҵᶦ ��˦���� ini����˾ �ϲ���.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see   WriteFile()
	 *
	 */
	bool ReadFile();

	/**
	 * @brief   ini������ ���궦 Ⱥ��.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see   ReadFile()
	 *
	 */
	bool WriteFile() const;

	// Deletes all stored ini data.

	/**
	 * @brief   ���� ���궦 �͸��ٳ�. 
	 *
	 * @return  void
	 *
	 */
	void Erase();
	
	/**
	 * @brief   ���� ���궦 �͸��ٳ�. 
	 *
	 * @return  void
	 *
	 */
	void Clear()   
	{
		Erase();
	}

	/**
	 * @brief   ���� ���궦 �͸��ٳ�. 
	 *
	 * @return  void
	 *
	 */
	void Reset()      
	{
		Erase();
	}

	/**
	 * @brief   �輳�� key̩ ������ �в���. 
	 *
	 * @param   keyname [in] : key�˶� 
	 * @return  key����, ���̼� key�� �����׼� ʱ˺�� -1.
	 *
	 */
	long FindKey( SString keyname) const;

	/**
	 * @brief   ���̼� key�溷 �輳�� ��̩ ������ �в���.
	 *
	 * @param   keyID     [in] : key���� 
	 * @param   valuename [in] : ��˺�ز� �� 
	 * @return  ��̩ ����, �ܼ� ����˾�� -1
	 *
	 */
	long FindValue( unsigned const keyID, SString valuename) const;

	/**
	 * @brief   ��� key̩ ���㶦 �в���.
	 *
	 * @return  key���� 
	 *
	 */
	unsigned NumKeys() const
	{
		return m_Names.GetSize();
	}
	
	/**
	 * @brief   ��� key̩ ���㶦 �в���.
	 *
	 * @return  key���� 
	 *
	 */
	unsigned GetNumKeys() const                  
	{
		return NumKeys();
	}

	/**
	 * @brief   key�� �����ٳ�.
	 *
	 * @param   keyname [in] : �����׵ز� key�˶�
	 * @return  key���� 
	 *
	 */
	unsigned AddKeyName( SString  keyname);

	/**
	 * @brief   ���̼� ������ �����ײ� key̩ �˶�˾ �в���. 
	 *
	 * @param   keyID [in] : key���� 
	 * @return  key�˶�
	 *
	 */
	SString KeyName( unsigned const keyID) const;
	
	/**
	 * @brief   ���̼� ������ �����ײ� key̩ �˶�˾ �в���. 
	 *
	 * @param   keyID [in] : key���� 
	 * @return  key�˶�
	 *
	 */
	SString GetKeyName( unsigned const keyID) const {return KeyName(keyID);}

	/**
	 * @brief   �輳�� key�� �����ײ� ��̩ ���㶦 �в���.
	 *
	 * @param   keyID [in] : key���� 
	 * @return  ��̩ ���� 
	 *
	 */
	unsigned NumValues( unsigned const keyID) const;
	
	/**
	 * @brief   �輳�� key�� �����ײ� ��̩ ���㶦 �в���.
	 *
	 * @param   keyID [in] : key���� 
	 * @return  ��̩ ���� 
	 *
	 */
	unsigned GetNumValues( unsigned const keyID) const {return NumValues( keyID);}

	/**
	 * @brief   �輳�� key�� �����ײ� ��̩ ���㶦 �в���.
	 *
	 * @param   keyname [in] : key�˶� 
	 * @return  ��̩ ���� 
	 *
	 */
	unsigned NumValues( SString keyname) const;

	/**
	 * @brief   �輳�� key�� �����ײ� ��̩ ���㶦 �в���.
	 *
	 * @param   keyname [in] : key�˶� 
	 * @return  ��̩ ���� 
	 *
	 */
	unsigned GetNumValues( SString keyname) const {return NumValues( keyname);}

	// Returns value name by index for a given keyname or keyID.

	/**
	 * @brief   ���̼� key̩ ���̼� ������ �����ײ� ���˶�˾ �в���. 
	 *
	 * @param   keyID   [in] : key���� 
	 * @param   valueID [in] : ������ 
	 * @return  �����ײ� ���˶� 
	 *
	 */
	SString ValueName( unsigned const keyID, unsigned const valueID) const;
	
	/**
	 * @brief   ���̼� key̩ ���̼� ������ �����ײ� ���˶�˾ �в���. 
	 *
	 * @param   keyID   [in] : key���� 
	 * @param   valueID [in] : ������ 
	 * @return  �����ײ� ���˶� 
	 *
	 */
	SString GetValueName( unsigned const keyID, unsigned const valueID) const
	{
		return ValueName( keyID, valueID);
	}
	
	/**
	 * @brief   ���̼� key̩ ���̼� ������ �����ײ� ���˶�˾ �в���. 
	 *
	 * @param   keyname [in] : key�˶� 
	 * @param   valueID [in] : ������ 
	 * @return  �����ײ� ���˶� 
	 *
	 */
	SString ValueName( SString keyname, unsigned const valueID) const;
	
	/**
	 * @brief   ���̼� key̩ ���̼� ������ �����ײ� ���˶�˾ �в���. 
	 *
	 * @param   keyname [in] : key�˶� 
	 * @param   valueID [in] : ������ 
	 * @return  �����ײ� ���˶� 
	 *
	 */
	SString GetValueName( SString keyname, unsigned const valueID) const
	{
		return ValueName( keyname, valueID);
	}

	// Gets value of [keyname] valuename =.
	// Overloaded to return SString, int, and double.
	// Returns defValue if Key/value not found.
	/**
	 * @brief   ���̼� key̩ ���̼� �������� �����ײ� ��˾ �в���.
	 *
	 * @param   keyID    [in] : key���� 
	 * @param   valueID  [in] : ������ 
	 * @param   defValue [in] : �󱨰� 
	 * @return  �����ײ� ��,  ���İ�˾ �ܼ� ����˾ �� �󱨰�
	 *
	 */
	SString GetValue( unsigned const keyID, unsigned const valueID, SString defValue = "") const;

	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� �����ײ� ��˾ �в���.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   defValue  [in] : �󱨰� 
	 * @return  �����ײ� ��,  ���İ�˾ �ܼ� ����˾ �� �󱨰�
	 *
	 */
	SString GetValue(SString keyname, SString valuename, SString defValue = "") const; 
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� �����ײ� ��˾ �в���.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   defValue  [in] : �󱨰� 
	 * @return  1, ���İ�˾ �ܼ� ����˾ �� 0
	 *
	 */
	int    GetValueI(SString keyname, SString valuename, int const defValue = 0) const;
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� �����ײ� ��˾ �в���.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   defValue  [in] : �󱨰� 
	 * @return  true, ���İ�˾ �ܼ� ����˾ �� false
	 *
	 */
	bool   GetValueB(SString keyname, SString valuename, bool const defValue = false) const
	{
		int dd;
		if (defValue)
			dd = 1;
		else
			dd = 0;
		int res = GetValueI( keyname, valuename, dd);
		if (res == 0)
			return false;
		else 
			return true;
	}
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� �����ײ� ��˾ �в���.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   defValue  [in] : �󱨰� 
	 * @return  ���İ�, ���İ�˾ �ܼ� ����˾ �� �󱨰�
	 *
	 */
	double   GetValueF(SString keyname, SString valuename, double const defValue = 0.0) const;
	// This is a variable length formatted GetValue routine. All these voids
	// are required because there is no vsscanf() like there is a vsprintf().
	// Only a maximum of 8 variable can be read.

	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� �����ײ� ��˾ ���̼� ����˺�� �в���.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   format    [in] : ���� 
	 * @return  ���̼� ����˺�� �� ���İ�, ���İ�˾ �ܼ� ����˾ �� false
	 *
	 */
	unsigned GetValueV( SString keyname, SString valuename, char *format,
			  void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
  			  void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
  			  void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
  			  void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);

	// Sets value of [keyname] valuename =.
	// Specify the optional paramter as false (0) if you do not want it to create
	// the Key if it doesn't exist. Returns true if data entered, false otherwise.
	// Overloaded to accept SString, int, and double.

	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� ��˾ �����ٳ�.
	 *
	 * @param   keyID   [in] : key���� 
	 * @param   valueID [in] : ������ 
	 * @param   value   [in] : �����׵ز� �� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool SetValue( unsigned const keyID, unsigned const valueID, SString const value);

	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� ��˾ �����ٳ�.
	 *          
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   value     [in] : �����׵ز� �� 
	 * @param   create    [in] : key�ۼ�����, ture�˷� key�� �����׼� ʱ˾ �� ���� �ۼ�.
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool SetValue( SString keyname, SString valuename, SString value, bool const create = true);
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� ��˾ �����ٳ�.
	 *          
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   value     [in] : �����׵ز� �� 
	 * @param   create    [in] : key�ۼ�����, ture�˷� key�� �����׼� ʱ˾ �� ���� �ۼ�.
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool SetValueI( SString keyname, SString valuename, int const value, bool const create = true);
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� ��˾ �����ٳ�.
	 *          
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   value     [in] : �����׵ز� �� 
	 * @param   create    [in] : key�ۼ�����, ture�˷� key�� �����׼� ʱ˾ �� ���� �ۼ�.
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool SetValueB( SString keyname, SString valuename, bool const value, bool const create = true)
	{
		return SetValueI( keyname, valuename, int(value), create);
	}
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� ��˾ �����ٳ�.
	 *          
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @param   value     [in] : �����׵ز� �� 
	 * @param   create    [in] : key�ۼ�����, ture�˷� key�� �����׼� ʱ˾ �� ���� �ۼ�.
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool SetValueF( SString keyname, SString valuename, double const value, bool const create = true);
	
	/**
	 * @brief   ���̼� key̩ ���̼� ���˶��� ���̼� ����̩ ��˾ �����ٳ�.
	 *
	 * @param   keyname   [in] : key�˶�  
	 * @param   valuename [in] : ���˶� 
	 * @param   format    [in] : ���� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool SetValueV( SString keyname, SString valuename, char *format, ...);

	/**
	 * @brief   ���̼� key̩ ���̼� ���˶�̩ ��˾ �����ٳ�.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   valuename [in] : ���˶� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool DeleteValue( SString keyname, SString valuename);

	/**
	 * @brief   �輳�� key�� �����ٳ�.
	 *
	 * @param   keyname [in] : key�˶�
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool DeleteKey(SString keyname);

	// Header comment functions.
	// Header m_Comments are those m_Comments before the first Key.
	//
	// Number of header m_Comments.
	/**
	 * @brief   ������������̩ �㶦 �в���.
	 *
	 * @return  ������̩ �� 
	 *
	 */
	unsigned NumHeaderComments()   
	{
		return m_Comments.GetSize();
	}

	/**
	 * @brief   ������������˾ �����ٳ�.
	 *
	 * @param   comment [in] : �����׵ز� ������ 
	 * @return  void
	 *
	 */
	void     HeaderComment( SString comment);

	/**
	 * @brief   �輳�� ����̩ ������������˾ �в���.
	 *
	 * @param   commentID [in] : �����͸��� 
	 * @return  ��˺�ز� ������ 
	 *
	 */
	SString   HeaderComment( unsigned const commentID) const;

	/**
	 * @brief   �輳�� ����̩ ������˾ �����ٳ�.
	 *
	 * @param   commentID [in] : �����͸��� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool     DeleteHeaderComment( unsigned commentID);

	/**
	 * @brief   ���� ������������˾ �����ٳ�.
	 *
	 * @return  void
	 *
	 */
	void     DeleteHeaderComments()               
	{
		m_Comments.RemoveAll();
	}

	// Key comment functions.
	// Key m_Comments are those m_Comments within a Key. Any m_Comments
	// defined within value m_Names will be added to this list. Therefore,
	// these m_Comments will be moved to the top of the Key definition when
	// the IniFile::WriteFile() is called.
	//

	/**
	 * @brief   ���̼� key̩ �����ͺ㶦 �в���.
	 *
	 * @param   keyID [in] : key���� 
	 * @return  �����ͺ� 
	 *
	 */
	unsigned NumKeyComments( unsigned const keyID) const;

	/**
	 * @brief   ���̼� key̩ �����ͺ㶦 �в���.
	 *
	 * @param   keyname [in] : key�˶� 
	 * @return  �����ͺ� 
	 *
	 */
	unsigned NumKeyComments( SString keyname) const;

	/**
	 * @brief   �輳�� key�� ������˾ �����ٳ�.
	 *
	 * @param   keyID   [in] : key���� 
	 * @param   comment [in] : �����׵ز� ������ 
	 * @return  �°��˷� true, ʭ����false
	 *
	 */
	bool     KeyComment( unsigned const keyID, SString comment);

	/**
	 * @brief   �輳�� key�� ������˾ �����ٳ�.
	 *
	 * @param   keyname [in] : key�˶� 
	 * @param   comment [in] : �����׵ز� ������ 
	 * @return  �°��˷� true, ʭ����false
	 *
	 */
	bool     KeyComment( SString keyname, SString comment);

	/**
	 * @brief   ���̼� key̩ �輳�� ����̩ ������˾ �в���.
	 *
	 * @param   keyID     [in] : key���� 
	 * @param   commentID [in] : �����͸��� 
	 * @return  ������ 
	 *
	 */
	SString   KeyComment( unsigned const keyID, unsigned const commentID) const;
	
	/**
	 * @brief   ���̼� key̩ �輳�� ����̩ ������˾ �в���.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   commentID [in] : �����͸��� 
	 * @return  ������ 
	 *
	 */
	SString   KeyComment( SString  keyname, unsigned const commentID) const;

	/**
	 * @brief   ���̼� key̩ ���̼� ������ �ͳ��ײ� ������˾ �����ٳ�.
	 *
	 * @param   keyID     [in] : key���� 
	 * @param   commentID [in] : �����͸��� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool     DeleteKeyComment( unsigned keyID, unsigned const commentID);
	
	/**
	 * @brief   ���̼� key̩ ���̼� ������ �ͳ��ײ� ������˾ �����ٳ�.
	 *
	 * @param   keyname   [in] : key�˶� 
	 * @param   commentID [in] : �����͸��� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool     DeleteKeyComment( SString keyname, unsigned const commentID);

	/**
	 * @brief   �輳�� key̩ ������˾ ���� �����ٳ�.
	 *
	 * @param   keyID [in] : key���� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool     DeleteKeyComments( unsigned const keyID);

	/**
	 * @brief   �輳�� key̩ ������˾ ���� �����ٳ�.
	 *
	 * @param   keyname [in] : key�˶� 
	 * @return  �°��˷� true, ʭ���� false
	 *
	 */
	bool     DeleteKeyComments( SString keyname);
};

/**
	@memo     Simple lo-fi property reader.

	@doc      Simple lo-fi property reader. Used as naive parser/scanner
				for the .asf, acf files et cetera.
*/
class CVLIB_DECLSPEC PropsReader
{
	FILE *fh;
	void SkipRestOfLine();
	void SkipComments();
    void SkipWhiteSpace();
    const char CR;
    const char LF;
    const char COMMENT_CHAR;
	
public:
	
	PropsReader( const SString &filename );
	~PropsReader();
	bool MoreNonWhiteSpaceOnLine();
	
	/// Skips whitespace and any commments preceeding the current file position.
	void Sync() { SkipWhiteSpace(); SkipComments(); }
	
	/// Returns true if the file is valid.
	bool IsValid() { return fh!=NULL; }
	
	/// Current open file.
	FILE *FH() { return fh; }
};

class CVLIB_DECLSPEC TokenFile : public XFileDisk
{
public:
	virtual void WriteString(const char* lpsz);
	virtual char* ReadString(char* lpsz, int nMax);
	virtual bool ReadString(SString& rString);
	virtual bool NextToken(SString& tok);
	TokenFile();
	virtual ~TokenFile();
private:
	SString m_CurString;
	SString m_Delimiter;
};

}

#endif //_INIFILE_H_
