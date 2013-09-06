
/*! 
 * \file    IniFile.h
 * \ingroup base
 * \brief   iniÁÄËÎË¾ ËÏ°Ö È¸°Ö ¼¿»õÂ×±¨ Ì¡ÂÙ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
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
 * @brief iniÁÄËÎ¼¿»õË¾ Ì¡ÂÙ ¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC IniFile  
{
private:
	//!  Match case º¼¼³±¨¸Ë 
	bool   m_fCaseInsensitive;
	//! ÁÄËÎ°Òµá 
	SString m_szPath;
	/**
	 * @brief ¿Ô¼³Ì© ¿Ğµ¹º÷
	 */
	class CVLIB_DECLSPEC Key
	{
	public:
		//!  °é¾¢»ô 
		Key ();

		/**
		 * @brief   °é¾¢»ô(¼ÑÊÌ¼ê keyµá¹¢À¾ »¯ key¶¦ °é¾¢ÂÙ³Ş.)
		 *
		 * @param   key [in] : ËÓµÙkey 
		 *
		 */
		Key (const Key& key);

		//!  ÃÍ¾Ë»ô 
		virtual ~Key();

		//!  keyËË¶§ 
		StringArray	names;
		//!  key°ª 
		StringArray values; 
		//!  keyº¼···Í 
		StringArray comments;
	};
	//! key¹ÉµÛ 
//	Array<Key>	m_Keys;
	PtrArray	m_Keys;
	//! keyËË¶§¹ÉµÛ 
	StringArray m_Names; 
	//! keyº¼···Í¹ÉµÛ 
	StringArray m_Comments;

	/**
	 * @brief   Match case¶¦ °Áº¡Â×°Ö °ûËæ ¶÷±Â ·Í»ôµÛË¾ ¸êÃûÂÙ³Ş. 
	 *
	 * @param   s [in] : ËÓµÙ·Í»ôµÛ 
	 * @return  °Î±á·Í»ôµÛ 
	 *
	 */
	SString CheckCase( SString s) const;

public:
	/**
	 * @brief ÊïË±¶¦ ¼è¼³Â×²÷ µÛ°ºÂô
	 */
	enum errors
	{
		noID = -1   /**< ÊïË±±¨¸Ë */
	};

	//! °é¾¢»ô 
	IniFile(const SString iniPath = "");
	//! ÃÍ¾Ë»ô 
	virtual ~IniFile() 
	{
		Reset();
	}

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.

	/**
	 * @brief   Match case¶¦ Sensitiveµá º¼¼³ÂÙ³Ş.
	 *
	 * @return  void 
	 *
	 */
	void CaseSensitive() 
	{
		m_fCaseInsensitive = false;
	}
	/**
	 * @brief   Match case¶¦ Insensitiveµá º¼¼³ÂÙ³Ş.
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
	 * @brief   iniÁÄËÎÌ© °Òµá¶¦ º¼¼³ÂÙ³Ş.
	 *
	 * @param   newPath [in] : °Òµá 
	 * @return  void
	 *
	 */
	void Path(const SString newPath)                
	{
		m_szPath = newPath;
	}
	
	/**
	 * @brief   iniÁÄËÎÌ© °Òµá¶¦ ÊĞ²÷³Ş. 
	 *
	 * @return  ÁÄËÎÌ© °Òµá 
	 *
	 */
	
	SString Path()
	{
		return m_szPath;
	}

	/**
	 * @brief   iniÁÄËÎÌ© °Òµá¶¦ º¼¼³ÂÙ³Ş.
	 *
	 * @param   newPath [in] : ÁÄËÎ°Òµá 
	 * @return  void
	 *
	 */
	void SetPath(const SString newPath)             
	{
		Path( newPath);
	}

	/**
	 * @brief   °Òµá¶¦ ¶®Ë¦Â×ÊŞ iniÁÄËÎË¾ ËÏ²÷³Ş.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see   WriteFile()
	 *
	 */
	bool ReadFile();

	/**
	 * @brief   iniÁÄËÎËæ »ôµê¶¦ Èº³Ş.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see   ReadFile()
	 *
	 */
	bool WriteFile() const;

	// Deletes all stored ini data.

	/**
	 * @brief   ·º´Ç »ôµê¶¦ ÃÍ¸ÒÂÙ³Ş. 
	 *
	 * @return  void
	 *
	 */
	void Erase();
	
	/**
	 * @brief   ·º´Ç »ôµê¶¦ ÃÍ¸ÒÂÙ³Ş. 
	 *
	 * @return  void
	 *
	 */
	void Clear()   
	{
		Erase();
	}

	/**
	 * @brief   ·º´Ç »ôµê¶¦ ÃÍ¸ÒÂÙ³Ş. 
	 *
	 * @return  void
	 *
	 */
	void Reset()      
	{
		Erase();
	}

	/**
	 * @brief   ¼è¼³´ô keyÌ© ¸ÜÂö¶¦ ÊĞ²÷³Ş. 
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @return  key¸ÜÂö, ¼ÑÊÌ¼ê key°¡ ¼Á¼õÂ×¼è Ê±Ëº·² -1.
	 *
	 */
	long FindKey( SString keyname) const;

	/**
	 * @brief   ¼ÑÊÌ¼ê keyËæº· ¼è¼³´ô °ªÌ© ¸ÜÂö¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyID     [in] : key¸ÜÂö 
	 * @param   valuename [in] : ½ÜËºµØ²÷ °ª 
	 * @return  °ªÌ© ¸ÜÂö, ½Ü¼è ·ÃÃÕË¾ÆÁ -1
	 *
	 */
	long FindValue( unsigned const keyID, SString valuename) const;

	/**
	 * @brief   Âï¼õ keyÌ© ±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @return  key±¶ºã 
	 *
	 */
	unsigned NumKeys() const
	{
		return m_Names.GetSize();
	}
	
	/**
	 * @brief   Âï¼õ keyÌ© ±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @return  key±¶ºã 
	 *
	 */
	unsigned GetNumKeys() const                  
	{
		return NumKeys();
	}

	/**
	 * @brief   key¶¦ ¾¡°¡ÂÙ³Ş.
	 *
	 * @param   keyname [in] : ¾¡°¡Â×µØ²÷ keyËË¶§
	 * @return  key¸ÜÂö 
	 *
	 */
	unsigned AddKeyName( SString  keyname);

	/**
	 * @brief   ¼ÑÊÌ¼ê ¸ÜÂöËæ ´İËÄÂ×²÷ keyÌ© ËË¶§Ë¾ ÊĞ²÷³Ş. 
	 *
	 * @param   keyID [in] : key¸ÜÂö 
	 * @return  keyËË¶§
	 *
	 */
	SString KeyName( unsigned const keyID) const;
	
	/**
	 * @brief   ¼ÑÊÌ¼ê ¸ÜÂöËæ ´İËÄÂ×²÷ keyÌ© ËË¶§Ë¾ ÊĞ²÷³Ş. 
	 *
	 * @param   keyID [in] : key¸ÜÂö 
	 * @return  keyËË¶§
	 *
	 */
	SString GetKeyName( unsigned const keyID) const {return KeyName(keyID);}

	/**
	 * @brief   ¼è¼³´ô keyËæ ¼Á¼õÂ×²÷ °ªÌ© ±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyID [in] : key¸ÜÂö 
	 * @return  °ªÌ© ±¶ºã 
	 *
	 */
	unsigned NumValues( unsigned const keyID) const;
	
	/**
	 * @brief   ¼è¼³´ô keyËæ ¼Á¼õÂ×²÷ °ªÌ© ±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyID [in] : key¸ÜÂö 
	 * @return  °ªÌ© ±¶ºã 
	 *
	 */
	unsigned GetNumValues( unsigned const keyID) const {return NumValues( keyID);}

	/**
	 * @brief   ¼è¼³´ô keyËæ ¼Á¼õÂ×²÷ °ªÌ© ±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @return  °ªÌ© ±¶ºã 
	 *
	 */
	unsigned NumValues( SString keyname) const;

	/**
	 * @brief   ¼è¼³´ô keyËæ ¼Á¼õÂ×²÷ °ªÌ© ±¶ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @return  °ªÌ© ±¶ºã 
	 *
	 */
	unsigned GetNumValues( SString keyname) const {return NumValues( keyname);}

	// Returns value name by index for a given keyname or keyID.

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê ¸ÜÂöËæ ´İËÄÂ×²÷ °ªËË¶§Ë¾ ÊĞ²÷³Ş. 
	 *
	 * @param   keyID   [in] : key¸ÜÂö 
	 * @param   valueID [in] : °ª¸ÜÂö 
	 * @return  ´İËÄÂ×²÷ °ªËË¶§ 
	 *
	 */
	SString ValueName( unsigned const keyID, unsigned const valueID) const;
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê ¸ÜÂöËæ ´İËÄÂ×²÷ °ªËË¶§Ë¾ ÊĞ²÷³Ş. 
	 *
	 * @param   keyID   [in] : key¸ÜÂö 
	 * @param   valueID [in] : °ª¸ÜÂö 
	 * @return  ´İËÄÂ×²÷ °ªËË¶§ 
	 *
	 */
	SString GetValueName( unsigned const keyID, unsigned const valueID) const
	{
		return ValueName( keyID, valueID);
	}
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê ¸ÜÂöËæ ´İËÄÂ×²÷ °ªËË¶§Ë¾ ÊĞ²÷³Ş. 
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @param   valueID [in] : °ª¸ÜÂö 
	 * @return  ´İËÄÂ×²÷ °ªËË¶§ 
	 *
	 */
	SString ValueName( SString keyname, unsigned const valueID) const;
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê ¸ÜÂöËæ ´İËÄÂ×²÷ °ªËË¶§Ë¾ ÊĞ²÷³Ş. 
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @param   valueID [in] : °ª¸ÜÂö 
	 * @return  ´İËÄÂ×²÷ °ªËË¶§ 
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
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ª¸ÜÂöËæ ´İËÄÂ×²÷ °ªË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyID    [in] : key¸ÜÂö 
	 * @param   valueID  [in] : °ª¸ÜÂö 
	 * @param   defValue [in] : ½ó±¨°ª 
	 * @return  ´İËÄÂ×²÷ °ª,  ´İËÄ°ªË¾ ½Ü¼è ·ÃÃÕË¾ ÆÁ ½ó±¨°ª
	 *
	 */
	SString GetValue( unsigned const keyID, unsigned const valueID, SString defValue = "") const;

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ ´İËÄÂ×²÷ °ªË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   defValue  [in] : ½ó±¨°ª 
	 * @return  ´İËÄÂ×²÷ °ª,  ´İËÄ°ªË¾ ½Ü¼è ·ÃÃÕË¾ ÆÁ ½ó±¨°ª
	 *
	 */
	SString GetValue(SString keyname, SString valuename, SString defValue = "") const; 
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ ´İËÄÂ×²÷ °ªË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   defValue  [in] : ½ó±¨°ª 
	 * @return  1, ´İËÄ°ªË¾ ½Ü¼è ·ÃÃÕË¾ ÆÁ 0
	 *
	 */
	int    GetValueI(SString keyname, SString valuename, int const defValue = 0) const;
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ ´İËÄÂ×²÷ °ªË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   defValue  [in] : ½ó±¨°ª 
	 * @return  true, ´İËÄ°ªË¾ ½Ü¼è ·ÃÃÕË¾ ÆÁ false
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
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ ´İËÄÂ×²÷ °ªË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   defValue  [in] : ½ó±¨°ª 
	 * @return  ´İËÄ°ª, ´İËÄ°ªË¾ ½Ü¼è ·ÃÃÕË¾ ÆÁ ½ó±¨°ª
	 *
	 */
	double   GetValueF(SString keyname, SString valuename, double const defValue = 0.0) const;
	// This is a variable length formatted GetValue routine. All these voids
	// are required because there is no vsscanf() like there is a vsprintf().
	// Only a maximum of 8 variable can be read.

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ ´İËÄÂ×²÷ °ªË¾ ¼ÑÊÌ¼ê Âô»¥Ëºµá ÊĞ²÷³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   format    [in] : Âô»¥ 
	 * @return  ¼ÑÊÌ¼ê Âô»¥Ëºµá ´ô ´İËÄ°ª, ´İËÄ°ªË¾ ½Ü¼è ·ÃÃÕË¾ ÆÁ false
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
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ °ªË¾ º¼¼³ÂÙ³Ş.
	 *
	 * @param   keyID   [in] : key¸ÜÂö 
	 * @param   valueID [in] : °ª¸ÜÂö 
	 * @param   value   [in] : º¼¼³Â×µØ²÷ °ª 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool SetValue( unsigned const keyID, unsigned const valueID, SString const value);

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ °ªË¾ º¼¼³ÂÙ³Ş.
	 *          
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   value     [in] : º¼¼³Â×µØ²÷ °ª 
	 * @param   create    [in] : key½Û¼¿±¨¸Ë, tureËË·² key°¡ ¼Á¼õÂ×¼è Ê±Ë¾ ÆÁ »¯µá ½Û¼¿.
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool SetValue( SString keyname, SString valuename, SString value, bool const create = true);
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ °ªË¾ º¼¼³ÂÙ³Ş.
	 *          
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   value     [in] : º¼¼³Â×µØ²÷ °ª 
	 * @param   create    [in] : key½Û¼¿±¨¸Ë, tureËË·² key°¡ ¼Á¼õÂ×¼è Ê±Ë¾ ÆÁ »¯µá ½Û¼¿.
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool SetValueI( SString keyname, SString valuename, int const value, bool const create = true);
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ °ªË¾ º¼¼³ÂÙ³Ş.
	 *          
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   value     [in] : º¼¼³Â×µØ²÷ °ª 
	 * @param   create    [in] : key½Û¼¿±¨¸Ë, tureËË·² key°¡ ¼Á¼õÂ×¼è Ê±Ë¾ ÆÁ »¯µá ½Û¼¿.
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool SetValueB( SString keyname, SString valuename, bool const value, bool const create = true)
	{
		return SetValueI( keyname, valuename, int(value), create);
	}
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ °ªË¾ º¼¼³ÂÙ³Ş.
	 *          
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   value     [in] : º¼¼³Â×µØ²÷ °ª 
	 * @param   create    [in] : key½Û¼¿±¨¸Ë, tureËË·² key°¡ ¼Á¼õÂ×¼è Ê±Ë¾ ÆÁ »¯µá ½Û¼¿.
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool SetValueF( SString keyname, SString valuename, double const value, bool const create = true);
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ëæ ¼ÑÊÌ¼ê Âô»¥Ì© °ªË¾ º¼¼³ÂÙ³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§  
	 * @param   valuename [in] : °ªËË¶§ 
	 * @param   format    [in] : Âô»¥ 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool SetValueV( SString keyname, SString valuename, char *format, ...);

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê °ªËË¶§Ì© °ªË¾ º¢½£ÂÙ³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   valuename [in] : °ªËË¶§ 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool DeleteValue( SString keyname, SString valuename);

	/**
	 * @brief   ¼è¼³´ô key¶¦ º¢½£ÂÙ³Ş.
	 *
	 * @param   keyname [in] : keyËË¶§
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool DeleteKey(SString keyname);

	// Header comment functions.
	// Header m_Comments are those m_Comments before the first Key.
	//
	// Number of header m_Comments.
	/**
	 * @brief   ·£¶®¹¢º¼···ÍÌ© ºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @return  º¼···ÍÌ© ºã 
	 *
	 */
	unsigned NumHeaderComments()   
	{
		return m_Comments.GetSize();
	}

	/**
	 * @brief   ·£¶®¹¢º¼···ÍË¾ ¾¡°¡ÂÙ³Ş.
	 *
	 * @param   comment [in] : ¾¡°¡Â×µØ²÷ º¼···Í 
	 * @return  void
	 *
	 */
	void     HeaderComment( SString comment);

	/**
	 * @brief   ¼è¼³´ô ¸ÜÂöÌ© ·£¶®¹¢º¼···ÍË¾ ÊĞ²÷³Ş.
	 *
	 * @param   commentID [in] : º¼···Í¸ÜÂö 
	 * @return  ÊĞËºµØ²÷ º¼···Í 
	 *
	 */
	SString   HeaderComment( unsigned const commentID) const;

	/**
	 * @brief   ¼è¼³´ô ¸ÜÂöÌ© º¼···ÍË¾ º¢½£ÂÙ³Ş.
	 *
	 * @param   commentID [in] : º¼···Í¸ÜÂö 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool     DeleteHeaderComment( unsigned commentID);

	/**
	 * @brief   ·º´Ç ·£¶®¹¢º¼···ÍË¾ º¢½£ÂÙ³Ş.
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
	 * @brief   ¼ÑÊÌ¼ê keyÌ© º¼···Íºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyID [in] : key¸ÜÂö 
	 * @return  º¼···Íºã 
	 *
	 */
	unsigned NumKeyComments( unsigned const keyID) const;

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© º¼···Íºã¶¦ ÊĞ²÷³Ş.
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @return  º¼···Íºã 
	 *
	 */
	unsigned NumKeyComments( SString keyname) const;

	/**
	 * @brief   ¼è¼³´ô keyËæ º¼···ÍË¾ ¾¡°¡ÂÙ³Ş.
	 *
	 * @param   keyID   [in] : key¸ÜÂö 
	 * @param   comment [in] : ¾¡°¡Â×µØ²÷ º¼···Í 
	 * @return  ºÂ°áËË·² true, Ê­³£·²false
	 *
	 */
	bool     KeyComment( unsigned const keyID, SString comment);

	/**
	 * @brief   ¼è¼³´ô keyËæ º¼···ÍË¾ ¾¡°¡ÂÙ³Ş.
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @param   comment [in] : ¾¡°¡Â×µØ²÷ º¼···Í 
	 * @return  ºÂ°áËË·² true, Ê­³£·²false
	 *
	 */
	bool     KeyComment( SString keyname, SString comment);

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼è¼³´ô ¸ÜÂöÌ© º¼···ÍË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyID     [in] : key¸ÜÂö 
	 * @param   commentID [in] : º¼···Í¸ÜÂö 
	 * @return  º¼···Í 
	 *
	 */
	SString   KeyComment( unsigned const keyID, unsigned const commentID) const;
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼è¼³´ô ¸ÜÂöÌ© º¼···ÍË¾ ÊĞ²÷³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   commentID [in] : º¼···Í¸ÜÂö 
	 * @return  º¼···Í 
	 *
	 */
	SString   KeyComment( SString  keyname, unsigned const commentID) const;

	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê ¸ÜÂöËæ ÃÍ³ëÂ×²÷ º¼···ÍË¾ º¢½£ÂÙ³Ş.
	 *
	 * @param   keyID     [in] : key¸ÜÂö 
	 * @param   commentID [in] : º¼···Í¸ÜÂö 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool     DeleteKeyComment( unsigned keyID, unsigned const commentID);
	
	/**
	 * @brief   ¼ÑÊÌ¼ê keyÌ© ¼ÑÊÌ¼ê ¸ÜÂöËæ ÃÍ³ëÂ×²÷ º¼···ÍË¾ º¢½£ÂÙ³Ş.
	 *
	 * @param   keyname   [in] : keyËË¶§ 
	 * @param   commentID [in] : º¼···Í¸ÜÂö 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool     DeleteKeyComment( SString keyname, unsigned const commentID);

	/**
	 * @brief   ¼è¼³´ô keyÌ© º¼···ÍË¾ ·º´¸ º¢½£ÂÙ³Ş.
	 *
	 * @param   keyID [in] : key¸ÜÂö 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 */
	bool     DeleteKeyComments( unsigned const keyID);

	/**
	 * @brief   ¼è¼³´ô keyÌ© º¼···ÍË¾ ·º´¸ º¢½£ÂÙ³Ş.
	 *
	 * @param   keyname [in] : keyËË¶§ 
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
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
