
/*!
 * \file	Object.h
 * \ingroup base
 * \brief	·º´Ç ¿Ğµ¹º÷Ì© ±¨½ó¿Ğµ¹º÷µá ¶®Ë¦´ó²÷ Object¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "cvlibbaseDef.h"

namespace CVLib 
{

struct _CVLIB_POSITION { };
typedef _CVLIB_POSITION* CVLIB_POSITION;
#define CVLIB_BEFORE_START_POSITION ((CVLIB_POSITION)-1L)
	
/**
 * @brief	CVLib LibraryÌ© ±¨½ó¿Ğµ¹º÷.
 <pre> ËË ¿Ğµ¹º÷²÷ ±¨ÊÍ±¨ÂÚ³ë±á ÃÍ¸ÒÌ© Áİ¶®ºÂË¾ ´ª·ºÂ×±¨ Ì¡Â×ÊŞ º¼±Ë´óÊî³Ş. </pre>
 */
class CVLIB_DECLSPEC Object
{
public:
	// Construction and Destruction
	//! °é¾¢»ô
	Object ();
	//! ÃÍ¾Ë»ô
    virtual ~Object();

	/**
	 * @brief   Ë÷¹¢ÁÄËÎµá¹¢À¾ ¼¬¼õÂÙ³Ş. 
	 *
	 * @param   szFilename [in] : ÁÄËÎ°Òµá  
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0
	 *
	 * @see  ToFile(char* szFilename), FromFile(FILE* pFile)
	 *
	 */
	virtual int	FromFile (const char* szFilename);
	/**
	 * @brief   Ë÷¹¢ÁÄËÎËæ ¸ó±ãÂÙ³Ş. 
	 *
	 * @param   szFilename [in] : ÁÄËÎ°Òµá 
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0
	 *
	 * @see  FromFile(char* szFilename), ToFile(FILE* pFile)
	 *
	 */
	virtual int	ToFile (const char* szFilename) const;
	/**
	 * @brief   ¼è¼³´ô ÁÄËÎµá¹¢À¾ ¼¬¼õÂÙ³Ş 
	 *
	 * @param   pFile [in] : ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0   
	 *
	 * @see  ToFile(FILE* pFile)
	 *
	 */
	virtual int	FromFile (FILE* pFile);
// 	virtual int FromFile (XFile* pFile);
	/**
	 * @brief   ¼è¼³´ô ÁÄËÎËæ ¸ó±ãÂÙ³Ş. 
	 *
	 * @param   pFile [in]  ÁÄËÎ¼è¼¬»ô 
	 * @return  ºÂ°áÂ×·² 1, »¨Â²Â×·² 0
	 *
	 * @see  FromFile (FILE* pFile)
	 *
	 */
	virtual int	ToFile (FILE* pFile) const;
// 	virtual int ToFile (XFile* pFile) const;
	// new and delete operations
	/**
	 * @brief   ±¨ÊÍ±¨¶¦ Ãú¸óÂÙ³Ş.
	 *
	 * @param   nSize [in] : Ãú¸óÂÚ ±¨ÊÍ±¨Ì© ¿Í±¨ 
	 * @return  ÂÚ³ë´ô ±¨ÊÍ±¨¼ÑºÏ
	 *
	 */
	void* operator new(size_t nSize);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ Ãú¸óÂÙ³Ş.
	 *
	 * @param   nSize      [in] : Ãú¸óÂÚ ±¨ÊÍ±¨Ì© ¿Í±¨ 
	 * @param   szFilename [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´õ Ìº½çÁÄËÎÌ© ËË¶§ 
	 * @param   nLine      [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´õ Ìº½çÁÄËÎÌ© ÃÔ¸ÜÂö 
	 * @return  ÂÚ³ë´ô ±¨ÊÍ±¨¼ÑºÏ 
	 *
	 */
	void* operator new(size_t nSize, char* szFilename, int nLine);
// 	void* operator new(size_t nSize, void* ptr);
// 	void* operator new(size_t nSize, void* ptr, char* szFilename, int nLine);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ Ãú¸óÂÙ³Ş.
	 *
	 * @param   nSize [in] : Ãú¸óÂÚ ±¨ÊÍ±¨Ì© ¿Í±¨ 
	 * @return  ÂÚ³ë´ô ±¨ÊÍ±¨¼ÑºÏ
	 *
	 */
	void* operator new[](size_t nSize);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ Ãú¸óÂÙ³Ş.
	 *
	 * @param   nSize      [in] : Ãú¸óÂÚ ±¨ÊÍ±¨Ì© ¿Í±¨ 
	 * @param   szFilename [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´õ Ìº½çÁÄËÎÌ© ËË¶§ 
	 * @param   nLine      [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´õ Ìº½çÁÄËÎÌ© ÃÔ¸ÜÂö 
	 * @return  ÂÚ³ë´ô ±¨ÊÍ±¨¼ÑºÏ 
	 *
	 */
	void* operator new[](size_t nSize, char* szFilename, int nLine);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ ÃÍ¸ÒÂÙ³Ş.
	 *
	 * @param   ptr [in] : ÃÍ¸ÒÂÚ ±¨ÊÍ±¨¼ÑºÏ
	 * @return  void
	 *
	 */
	void  operator delete(void *ptr);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ ÃÍ¸ÒÂÙ³Ş.
	 *
	 * @param   ptr        [in] : ÃÍ¸ÒÂÚ ±¨ÊÍ±¨¼ÑºÏ 
	 * @param   szFilename [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´ô Ìº½çÁÄËÎÌ© ËË¶§ 
	 * @param   nLine      [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´ô Ìº½çÁÄËÎÌ© ÃÔ¸ÜÂö 
	 * @return  void
	 *
	 */
	void  operator delete(void *ptr, char* szFilename, int nLine);
// 	void  operator delete(void *ptr, void *ptr1);
// 	void  operator delete(void *ptr, void *ptr1, char* szFilename, int nLine);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ ÃÍ¸ÒÂÙ³Ş.
	 *
	 * @param   ptr [in] : ÃÍ¸ÒÂÚ ±¨ÊÍ±¨¼ÑºÏ
	 * @return  void
	 */
	void  operator delete[](void *ptr);

	/**
	 * @brief   ±¨ÊÍ±¨¶¦ ÃÍ¸ÒÂÙ³Ş.
	 *
	 * @param   ptr        [in] : ÃÍ¸ÒÂÚ ±¨ÊÍ±¨¼ÑºÏ 
	 * @param   szFilename [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´ô Ìº½çÁÄËÎÌ© ËË¶§ 
	 * @param   nLine      [in] : ±¨ÊÍ±¨°¡ Ãú¸ó´ô Ìº½çÁÄËÎÌ© ÃÔ¸ÜÂö 
	 * @return  void
	 *
	 */
	void  operator delete[](void *ptr, char* szFilename, int nLine);

};

}

#endif //_OBJECT_H_
