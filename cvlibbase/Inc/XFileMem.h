
/*!
 * \file    XFileMem.h
 * \ingroup base
 * \brief   ÁÄËÎ¼¿»õË¾ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _XFILEMEM_H_
#define _XFILEMEM_H_

#include "XFile.h"
#include "cvlibbasedef.h"

namespace CVLib
{

/**
 * @brief  ÁÄËÎ¼¿»õ Ì¡ÂÙ ¿Ğµ¹º÷
 */
class CVLIB_DECLSPEC XFileMem : public XFile
{
public:
	/**
	 * @brief   ¿Ğµ¹º÷¶¦ ½ó±¨ÃùÂÙ³Ş.
	 *
	 * @param   pBuffer [in] : ÁÄËÎË¾ Ì¡ÂÙ ±¨ÊÍ±¨¼ÑºÏ 
	 * @param   size    [in] : ÁÄËÎ¿Í±¨ 
	 *
	 */
	 XFileMem(BYTE* pBuffer = NULL, DWORD size = 0);
	 
	//! ÃÍ¾Ë»ô 
	~XFileMem();

	/**
	 * @brief   ÁÄËÎË¾ Ì¡ÂÙ ±¨ÊÍ±¨¶¦ ½Û¼¿ÂÙ³Ş.
	 *
	 * @return  ¼³ÃúÃÅ ½Û¼¿´óÊîËº·² true, Ê­³£·² false
	 *
	 * @see  Close()
	 *
	 * @par Example:
	 * @code
	 *
	 *  BYTE* pBuffer = new BYTE[1000];
	 *  XFileMem xFile(pBuffer);	
	 *  xFile.Open();
	 * 
	 *  int nCount = 5;
	 *  int* pnData = new int[nCount];
	 *  for(int i = 0; i < nCount; i ++)
	 *  	pnData[i] = i;
	 *
	 *  int nWrite = xFile.Write(pnData, sizeof(int), nCount);
	 * 	assert( nWrite == nCount );
	 *  assert(xFile.Flush() == TRUE );
	 *  assert( xFile.Size() == (long)(sizeof(int) * nCount) );
	 *  
	 *  xFile.Seek(0, SEEK_SET);
	 *  int nData[5];
	 *  xFile.Read(nData, sizeof(int), 3);
	 *  for(i = 0; i < 3; i ++)
	 *  	assert( nData[i] == i );
	 *	
	 *  long  position = xFile.Tell();
	 *  assert( position == (sizeof(int) * 3) );
	 *  
	 *  xFile.Seek(4L, SEEK_CUR);
	 *  xFile.Read(&(nData[4]), sizeof(int), 1);
	 *  assert( nData[4] == 4 );
	 *  assert( xFile.Eof() == TRUE );
	 *  assert( xFile.Error() == 0 );	
	 * 
	 *  assert ( xFile.GetBuffer() == pBuffer );
     *  xFile.Close();
     *  delete []pnData;
	 *  ...
	 *  @endcode
	 * 
	 */
	bool Open();

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨Ì© ¼ÑºÏ¶¦ ÊĞ²÷³Ş. 
	 *
	 * @param   bDetachBuffer [in] : 
	 * @return  ÁÄËÎ¼ÑºÏ 
	 *
	 * @see  open()
	 *
	 */
	BYTE* GetBuffer(bool bDetachBuffer = true);

	/**
	 * @brief   ÁÄËÎË¾ Ì¡ÃÍ Ãú¸ó´ô ±¨ÊÍ±¨¶¦ ÃÍ¸ÒÂÙ³Ş.
	 *
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  open()
	 * 
	 */
	virtual bool	Close();

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨µá¹¢À¾ Ì°¾©±¨Ëæµá »ôµê¶¦ ËÏÊÌ´ÉËÍ³Ş.
	 *
	 * @param   buffer [in] : Ì°¾©±¨¼ÑºÏ 
	 * @param   size   [in] : ËÏ±¨Â×²÷ ÊıºÏÌ© ¸ÆËËÀâºã 
	 * @param   count  [in] : ËÏ±¨Â×²÷ ÊıºÏÌ© ¾×´İºã 
	 * @return  »¨¼è ËÏ±¨ ÂÙ  ÊıºÏºã 
	 *
	 * @see  Write(), open(), XFileDisk::Read()
	 *
	 */
	virtual int	Read(void *buffer, int size, int count);

	/**
	 * @brief   Ì°¾©±¨Ì© »ôµê¶¦ ÁÄËÎËæ È¸±¨ÂÙ³Ş.
	 *
	 * @param   buffer [in] : Ì°¾©±¨¼ÑºÏ 
	 * @param   size   [in] : È¸±¨Â×²÷ ÊıºÏÌ© ¸ÆËËÀâºã  
	 * @param   count  [in] : È¸±¨Â×²÷ ÊıºÏÌ© ¾×´İºã 
	 * @return  »¨¼è È¸±¨ ÂÙ ÊıºÏºã 
	 *
	 * @see  Read(), open(), XFileDisk::Write()
	 *
	 */
	virtual int	Write(const void *buffer, int size, int count);

	/**
	 * @brief   ÁÄËÎ³­¹¢¼è¼¬»ô¶¦ ¼è¼³´ô Ì¡¾·Ëæµá ËË´´»¤¿Ö³Ş.
	 *
	 * @param   offset [in] : ËË´´»¤¿ÔµØ²÷ ¸ÆËËÀâºã 
	 * @param   origin [in] : ½ó±¨Ì¡¾· 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false 
	 *
	 * @see  open()
	 *
	 */
	virtual bool	Seek(long offset, int origin);

	/**
	 * @brief   ÁÄËÎ³­¹¢¼è¼¬»ôÌ© Âï¼õÌ¡¾·¶¦ ÊĞ²÷³Ş.
	 * 
	 * @return  Âï¼õÌ¡¾· 
	 *
	 * @see  Seek(), open()
	 *
	 */
	virtual long	Tell();

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨Ì© ¿Í±¨¶¦ ÊĞ²÷³Ş.
	 *
	 * @return  ÁÄËÎ¿Í±¨  
	 *
	 * @see  Seek(), Tell(), open()
	 *
	 */
	virtual long	Size();

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨¶¦ flushÂÙ³Ş.
	 *
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  open()
	 *
	 */
	virtual bool	Flush();

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨Ì© ÄùË¾ °Áº¡Â×²÷ ÂÜºãËË³Ş.
	 *
	 * @return  ÄùËË·² true, Ê­³£·² false
	 *
	 * @see  open()
	 *
	 */
	virtual bool	Eof();

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨³­Ì© ÊïË±¶¦ °Áº¡ÂÙ³Ş.
	 *
	 * @return  ÊïË±°¡ ÊÖËº·² 0, ËØËº·² 1
	 *
	 * @see  open()
	 *
	 */
	virtual long	Error();

	/**
	 * @brief   ¼ÑÊÌ¼ê ·Í»ô¶¦ ÁÄËÎ±¨ÊÍ±¨Ëæ È¸±¨ÂÙ³Ş.
	 *
	 * @param   c [in] : È¸µØ²÷ ·Í»ô 
	 * @return  ºÂ°áËË·² true, »¨Â²ËË·² false
	 *
	 * @see  GetC()
	 *
	 * @par Example:
	 * @code
	 *
	 *	BYTE* pBuffer = new BYTE[1000];
	 *	XFileMem xFile(pBuffer);
	 *	xFile.Open();
	 *	
	 *	char str[] = "test";
	 *	char* szChar = str;
	 *	while( *szChar != 0 )
	 *  	xFile.PutC( *(szChar++) );
	 *  
	 *  xFile.Seek(0, SEEK_SET);
	 *  assert( (char)xFile.GetC() == 't');
	 *  assert( (char)xFile.GetC() == 'e');
	 *  assert( (char)xFile.GetC() == 's');
     *
	 *  xFile.Close();
	 * ...
	 * @endcode
	 * 
	 */
	virtual bool	PutC(unsigned char c);

	/**
	 * @brief   ÁÄËÎ±¨ÊÍ±¨µá¹¢À¾ ·Í»ô¶¦ ËÏÊÌ´ÉËÍ³Ş.
	 *
	 * @return  ËÏÊÌ´ÉËÍ ·Í»ô 
	 *
	 * @see  PutC()
	 *
	 */
	virtual long	GetC();

protected:
	void	Alloc(DWORD nBytes);
	void	Free();

	//!  ÁÄËÎ±¨ÊÍ±¨Ì© ¼ÑºÏ 
	BYTE*	m_pBuffer;
	//!  ÁÄËÎ±¨ÊÍ±¨Ì© ¿Í±¨ 
	DWORD	m_Size;
	//!  ÁÄËÎËË ÊâµØËØ²÷°¡¶¦ ±ıÀ°³­²÷ ±¨¸Ë 
	bool	m_bFreeOnClose;
	//!  ÁÄËÎ³­¹¢¼è¼¬»ôÌ© Âï¼õÌ¡¾· 
	long	m_Position;
	//!  ÁÄËÎ±¨ÊÍ±¨Ì© ¾×´İ¿Í±¨    
	long	m_Edge;	
};

}

#endif //_XFILEMEM_H_
