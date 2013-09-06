
/*!
 * \file    XFileDisk.h
 * \ingroup base
 * \brief   ÁÄËÎ¼¿»õË¾ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _XFILEDISK_H_
#define _XFILEDISK_H_

#include "XFile.h"

namespace CVLib
{

/**
 * @brief  ÁÄËÎ¼¿»õË¾ Ì¡ÂÙ ¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC XFileDisk : public XFile
{
public:
	//! °é¾¢»ô 
	XFileDisk(FILE* fp = NULL);
	//! ÃÍ¾Ë»ô 
	~XFileDisk();

	/**
	 * @brief   Ë÷¹¢ÁÄËÎË¾ Êâ±¨ÂÙ³Ş. 
	 *
	 * @param   filename [in] :  ÁÄËÎËË¶§ 
	 * @param   mode     [in] :  Êâ±¨¸Ò»¥ 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Close()
	 *
	 * @par Example:
	 * @code
	 *
	 *  XFileDisk xFile;
	 *  char szFileName[] = "c:/test1.bin";
	 *  assert( xFile.Open(szFileName, "wb") == TRUE );
	 * 
	 *  int nCount = 5;
	 *  int* pnData = new int[nCount];
	 *  for(int i = 0; i < nCount; i ++)
	 *      pnData[i] = i;
	 *
	 *  int nWrite = xFile.Write(pnData, sizeof(int), nCount);
	 * 	assert( nWrite == nCount );
	 * 	assert(xFile.Flush() == TRUE );
	 *  xFile.Close();
	 *
	 *  xFile.Open(szFileName, "rb");
	 *  assert( xFile.Size() == (long)sizeof(int) * nCount );
	 *
	 *  int nData[5];
	 *  xFile.Read(nData, sizeof(int), 3);
	 *  for(i = 0; i < 3; i ++)
	 *	    assert( nData[i] == i );
	 *
	 *  long  position = xFile.Tell();
	 *  assert( position == (sizeof(int) * 3) );
	 *
	 *  xFile.Seek(4L, SEEK_CUR);
	 *  xFile.Read(&(nData[4]), sizeof(int), 1);
	 *  assert( nData[4] == 4 );
	 *  assert( xFile.Eof() == FALSE );
	 * 	assert( xFile.Error() == 0 );
     *
     *  xFile.Close();
	 *  delete []pnData;
	 *  ...
	 *  @endcode
	 * 
	 */
	bool Open(const char *filename, const char *mode);

	/**
	 * @brief   Ë÷¹¢ÁÄËÎË¾ ³á±¨ÂÙ³Ş.
	 *
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false 
	 *
	 * @see  Open()
	 *
	 */
	virtual bool Close();
  	
	/**
	 * @brief   Ë÷¹¢ÁÄËÎµá¹¢À¾ Ì°¾©±¨Ëæ »ôµê¶¦ ËÏÊÌ´ÉËÍ³Ş.
	 *
	 * @param   buffer [in] : Ì°¾©±¨¼ÑºÏ 
	 * @param   size   [in] : ËÏ±¨Â×²÷ ÊıºÏÌ© ¸ÆËËÀâºã 
	 * @param   count  [in] : ËÏ±¨Â×²÷ ÊıºÏÌ© ¾×´İ±¶ºã 
	 * @return  »¨¼è ËÏ±¨ ÂÙ ÊıºÏºã 
	 *
	 * @see  Write(), Open()
	 *
	 */
	virtual int	Read(void *buffer, int size, int count);
	
	/**
	 * @brief   Ë÷¹¢ÁÄËÎËæ Ì°¾©±¨Ì© »ôµê¶¦ È¸±¨ÂÙ³Ş.
	 *
	 * @param   buffer [in] :  Ì°¾©±¨¼ÑºÏ
	 * @param   size   [in] :  È¸±¨Â×²÷ ÊıºÏÌ© ¸ÆËËÀâºã 
	 * @param   count  [in] :  È¸±¨Â×²÷ ÊıºÏÌ© ¾×´İ±¶ºã 
	 * @return  »¨¼è È¸±¨ ÂÙ ÊıºÏºã 
	 *
	 * @see  Read(), Open()
	 * 
	 */
	virtual int	Write(const void *buffer, int size, int count);
	
	/**
	 * @brief   ÁÄËÎ¼è¼¬»ô¶¦ ¼è¼³´ô Ì¡¾·Ëæµá ËË´´»¤¿Ö³Ş.
	 *
	 * @param   offset [in] : ËË´´»¤¿ÔµØ²÷ ¸ÆËËÀâºã 
	 * @param   origin [in] : ½ó±¨Ì¡¾· 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Open()
	 *
	 */
	virtual bool Seek(long offset, int origin);
	
	/**
	 * @brief   ÁÄËÎ¼è¼¬»ôÌ© Âï¼õÌ¡¾·¶¦ ÊĞ²÷ ÂÜºãËË³Ş.
	 *
	 * @return  Âï¼õÌ¡¾· 
	 *
	 * @see  Seek(), Open()
	 *
	 */
	virtual long Tell();
	
	/**
	 * @brief   ÁÄËÎÌ© ¿Í±¨¶¦ ÊĞ²÷ ÂÜºãËË³Ş.
	 *
	 * @return  ÁÄËÎÌ© ¿Í±¨ 
	 *
	 * @see  Seek(), Tell(), Open()
	 *
	 */
	virtual long	Size();
	
	/**
	 * @brief   ÁÄËÎË¾ flushÂÙ³Ş.
	 *
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  Open()
	 *
	 */
	virtual bool	Flush();
	
	/**
	 * @brief   ÁÄËÎÌ© ÄùË¾ °Áº¡Â×²÷ ÂÜºãËË³Ş.
	 *
	 * @return  ÁÄËÎÌ© ÄùËË·² true, ÄùËË Ê­³£·² false
	 *
	 * @see  Open()
	 *
	 */
	virtual bool	Eof();
	
	/**
	 * @brief   ÊïË±¶¦ °Áº¡ÂÙ³Ş.
	 *
	 * @return  ÊïË±°¡ ÊÖËº·² 0, ËØËº·² 1
	 *
	 * @see  Open()
	 *
	 */
	virtual long	Error();
	
	/**
	 * @brief   ¼ÑÊÌ¼ê ·Í»ô¶¦ ÁÄËÎËæ È¸±¨ÂÙ³Ş.
	 *
	 * @param   c [in] : È¸µØ²÷ ·Í»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  GetC()
	 *
	 * @par Example:
	 * @code
	 *
	 *	XFileDisk xFile;
	 *	char szFileName[] = "c:/test2.bin";
	 *	xFile.Open(szFileName, "wb");
	 *	
	 *	char str[] = "test";
	 *	char* szChar = str;
	 *	while( *szChar != 0 )
	 *  	xFile.PutC( *(szChar++) );
	 *  xFile.Close();
	 *  
	 *  xFile.Open(szFileName, "rb");endcode
	 *  assert( (char)xFile.GetC() == 't');
	 *  xFile.Close();
	 *  ...
	 * @endcode
	 *
	 */
	virtual bool PutC(unsigned char c);
	
	/**
	 * @brief   ÁÄËÎµá¹¢À¾ ·Í»ô¶¦ ËÏÊÌ´ÉËÍ³Ş.
	 *
	 * @return  ËÏÊÌ´ÉËÍ ·Í»ô 
	 *
	 * @see  PutC()
	 *
	 */
	virtual long	GetC();
protected:
	//! ÁÄËÎ¼è¼¬»ô 
	FILE *m_fp;
	//! ÁÄËÎËË ÊâµØËØ²÷°¡¶¦ ±ıÀ°³­²÷ ±¨¸Ë 
	bool m_bCloseFile;
};

}

#endif //_XFILEDISK_H_