/*!
 * \file    XFile.h
 * \ingroup base
 * \brief   ÁÄËÎ¼¿»õË¾ Ì¡ÂÙ ±¨½ó¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#if !defined(_XFILE_H_)
#define _XFILE_H_

#include "cvlibbasedef.h"

namespace CVLib
{
/**
 * @brief  ÁÄËÎ¼¿»õË¾ Ì¡ÂÙ ±¨½ó¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC XFile
{
public:
	//! °é¾¢»ô 
	XFile(void) { };
	//! ÃÍ¾Ë»ô 
	virtual ~XFile() { };

	/**
	 * @brief   ÁÄËÎË¾ ³á±¨ÂÙ³Ş.
	 *
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false 
	 */
	virtual bool	Close() = 0;
	
	/**
	 * @brief   ÁÄËÎµá¹¢À¾ Ì°¾©±¨Ëæ »ôµê¶¦ ËÏÊÌ´ÉËÍ³Ş.
	 *
	 * @param   buffer [in] : Ì°¾©±¨¼ÑºÏ 
	 * @param   size   [in] : ËÏ±¨Â×²÷ ÊıºÏÌ© ¸ÆËËÀâºã 
	 * @param   count  [in] : ËÏ±¨Â×²÷ ÊıºÏÌ© ¾×´İ±¶ºã 
	 * @return  »¨¼è ËÏ±¨ ÂÙ ÊıºÏºã 
	 *
	 */
	virtual int		Read(void *buffer, int size, int count) = 0;

	/**
	 * @brief   ÁÄËÎËæ Ì°¾©±¨Ì© »ôµê¶¦ È¸±¨ÂÙ³Ş.
	 *
	 * @param   buffer [in] :  Ì°¾©±¨¼ÑºÏ
	 * @param   size   [in] :  È¸±¨Â×²÷ ÊıºÏÌ© ¸ÆËËÀâºã 
	 * @param   count  [in] :  È¸±¨Â×²÷ ÊıºÏÌ© ¾×´İ±¶ºã 
	 * @return  »¨¼è È¸±¨ ÂÙ ÊıºÏºã 
	 *
	 */
	virtual int		Write(const void *buffer, int size, int count) = 0;
	
	/**
	 * @brief   ÁÄËÎ¼è¼¬»ô¶¦ ¼è¼³´ô Ì¡¾·Ëæµá ËË´´»¤¿Ö³Ş.
	 *
	 * @param   offset [in] : ËË´´»¤¿ÔµØ²÷ ¸ÆËËÀâºã 
	 * @param   origin [in] : ½ó±¨Ì¡¾· 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 */
	virtual bool	Seek(long offset, int origin) = 0;
		
	/**
	 * @brief   ÁÄËÎ¼è¼¬»ôÌ© Âï¼õÌ¡¾·¶¦ ÊĞ²÷ ÂÜºãËË³Ş.
	 *
	 * @return  Âï¼õÌ¡¾· 
	 *
	 */
	virtual long	Tell() = 0;
	
	/**
	 * @brief   ÁÄËÎÌ© ¿Í±¨¶¦ ÊĞ²÷ ÂÜºãËË³Ş.
	 *
	 * @return  ÁÄËÎÌ© ¿Í±¨ 
	 *
	 */
	virtual long	Size() = 0;
	
	/**
	 * @brief   ÁÄËÎË¾ flushÂÙ³Ş.
	 *
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 */
	virtual bool	Flush() = 0;
	
	/**
	 * @brief   ÁÄËÎÌ© ÄùË¾ °Áº¡Â×²÷ ÂÜºãËË³Ş.
	 *
	 * @return  ÁÄËÎÌ© ÄùËË·² true, ÄùËË Ê­³£·² false
	 *
	 */
	virtual bool	Eof() = 0;

	/**
	 * @brief   ÊïË±¶¦ °Áº¡ÂÙ³Ş.
	 *
	 * @return  ÊïË±°¡ ÊÖËº·² 0, ËØËº·² 1
	 *
	 */
	virtual long	Error() = 0;
	
	/**
	 * @brief   ¼ÑÊÌ¼ê ·Í»ô¶¦ ÁÄËÎËæ È¸±¨ÂÙ³Ş.
	 *
	 * @param   c [in] : È¸µØ²÷ ·Í»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 */
	virtual bool	PutC(unsigned char c)
		{
		// Default implementation
		int nWrote = Write(&c, 1, 1);
		return (bool)(nWrote == 1);
		}

	/**
	 * @brief   ÁÄËÎµá¹¢À¾ ·Í»ô¶¦ ËÏÊÌ´ÉËÍ³Ş.
	 *
	 * @return  ËÏÊÌ´ÉËÍ ·Í»ô 
	 *
	 */
	virtual long	GetC() = 0;
};

}

#endif //_XFILE_H_
