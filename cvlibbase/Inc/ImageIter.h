/*! 
 * \file    ImageIter.h
 * \ingroup base
 * \brief   Ãùº¬¼¿»õË¾ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _IMAGEITERATOR_H_
#define _IMAGEITERATOR_H_

#include "CoImage.h"

namespace CVLib
{
/**
 * @brief  Ãùº¬¼¿»õË¾ ¼êÃÔÂ×²÷ ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC ImageIterator
{
	friend class CoImage;
public:
	
	//! °é¾¢»ô
	ImageIterator ( CoImage* pImage );
	//! ÃÍ¾Ë»ô
	virtual ~ImageIterator ();

	// Iterators
	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ Ãùº¬Ì© ½ë ÃùºÏËæ º¼¼³ÂÙ³Þ.
	 *
	 * @return  void
	 *
	 */
	void  Reset ();
	
	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ Ãùº¬Ì© ¶ë¼è¶ìÃùºÏËæ º¼¼³ÂÙ³Þ.
	 *
	 * @return  void  
	 *
	 */
	void  Upset ();
	
	/**
	 * @brief   Ì°¾©±¨µá¹¢À¾ ³­¹¢±¨ÊÍ°á°£Ëæµá ¾×´Ý ÂÙÃÔ»ôµê¶¦ Èº³Þ.
	 *
	 * @param   buf [in] : Ì°¾©±¨¼ÑºÏ 
	 * @param   n   [in] : È¸±¨ ÂÚ ¸ÆËËÀâºã 
	 * @return  void
	 *
	 * @see   GetRow(BYTE *buf, int n)
	 *
	 */
	void  SetRow(BYTE *buf, int n);

	/**
	 * @brief   ³­¹¢±¨ÊÍ°á°£Ëºµá¹¢À¾ Ì°¾©±¨Ëæµá ¾×´Ý ÂÙÃÔ»ôµê¶¦ ËÏÊÌ´ÉËÍ³Þ.
	 *
	 * @param   buf [out] : Ì°¾©±¨¼ÑºÏ¡¡
	 * @param   n   [in]  : ËÏ±¨ ÂÚ ¸ÆËËÀâºã ¡¡
	 * @return  void
	 *
	 * @see  SetRow(BYTE *buf, int n)
	 *
	 */
	void  GetRow(BYTE *buf, int n);
	
	/**
	 * @brief   ³­¹¢±¨ÊÍ°á°£Ëºµá¹¢À¾ ÂÙ ¸ÆËËÀâÌ© »ôµê¶¦ ËÏ²÷³Þ.
	 *
	 * @return  ËÏ±¨ ÂÙ »ôµê 
	 *
	 * @see  SetByte()
	 *
	 */
	BYTE  GetByte( ) { return m_pbIterImage[m_nItx]; }

	/**
	 * @brief   ³­¹¢±¨ÊÍ°á°£Ëæ ÂÙ ¸ÆËËÀâÌ© »ôµê¶¦ Èº³Þ.
	 *
	 * @param   b [in] : È¸±¨ ÂÚ  »ôµê 
	 * @return  void
	 *
	 * @see  GetByte()
	 *
	 */
	void  SetByte(BYTE b) { m_pbIterImage[m_nItx] = b; }

	/**
	 * @brief   Âï¼õÌ© ÂÙÃÔ»ôµê¶¦ ËÏ²÷³Þ.
	 * @return  ËÏ±¨ ÂÙ »ôµê 
	 *
	 */
	BYTE* GetRow(void);
	
	/**
	 * @brief   ¼è¼³´ô ¸ÜÂöÌ© ÂÙÃÔ»ôµê¶¦ ËÏ²÷³Þ.
	 *
	 * @param   n [in] : ÃÔ¸ÜÂö 
	 * @return  ËÏ±¨ ÂÙ »ôµê 
	 *
	 */
	BYTE* GetRow(int n);
	
	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ ³ÞËÁÃÔËºµá ËË´´»¤¿Ö³Þ.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see   PrevRow()
	 *
	 */
	BOOL  NextRow();
	
	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ ËË¼­ÃÔËºµá ËË´´»¤¿Ö³Þ. 
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see  NextRow()
	 *
	 */
	BOOL  PrevRow();
	
	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ ³ÞËÁ¸ÆËËÀâËæµá ËË´´»¤¿Ö³Þ.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see  PrevByte()
	 *
	 */
	BOOL  NextByte();
	
	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ ËË¼­¸ÆËËÀâËæµá ËË´´»¤¿Ö³Þ.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see  NextByte()
	 *
	 */
	BOOL  PrevByte();

	/**
	 * @brief   ÃÔ±á µÛÌ© °¾ËÁºã´ÉË¾ º¼¼³ÂÙ³Þ.
	 *
	 * @param   x [in] : ÃÔ°¾ËÁºã 
	 * @param   y [in] : µÛ°¾ËÁºã 
	 * @return  void
	 *
	 * @see  GetSteps()
	 *
	 */
	void  SetSteps(int x, int y=0) {  m_nStepx = x; m_nStepy = y; }

	/**
	 * @brief   ÃÔ±á µÛÌ© °¾ËÁºã¶¦ ÊÐ²÷³Þ.
	 *
	 * @param   x [out] : ÃÔ°¾ËÁºã 
	 * @param   y [out] : µÛ°¾ËÁºã 
	 * @return  void
	 *
	 * @see  SetSteps()
	 *
	 */
	void  GetSteps(int *x, int *y) {  *x = m_nStepx; *y = m_nStepy; }

	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ °¾ËÁºã¶í¿Ñ ´ûµá ËË´´»¤¿Ö³Þ.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see   PrevStep()
	 *
	 */
	BOOL  NextStep();

	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ °¾ËÁºã¶í¿Ñ Ê¼Ëºµá ËË´´»¤¿Ö³Þ.
	 *
	 * @return  ºÂ°áËË·² true, Ê­³£·² false
	 *
	 * @see   NextStep()
	 *
	 */
	BOOL  PrevStep();

	/**
	 * @brief   ³­¹¢¼è¼¬»ô¶¦ ¼è¼³´ô ÃÔËæµá ËË´´»¤¿Ö³Þ. 
	 *
	 * @param   y [in] : ÃÔ¸ÜÂö
	 * @return  void
	 *
	 * @see  GetY()
	 *
	 */
	void  SetY(int y);	/* AD - for interlace */

	/**
	 * @brief   Âï¼õÌ© ÃÔ¸ÜÂö¶¦ ÊÐ²÷³Þ. 
	 *
	 * @return  ÃÔ¸ÜÂö 
	 *
	 * @see  SetY()
	 *
	 */
	int   GetY() {return m_nIty;}

	/**
	 * @brief   BMP°é¼¿Ì© Ãùº¬»ôµêµá¹¢À¾ R, G, B»°ÃÔµÛË¾ °é¾¢ÂÙ³Þ.
	 *
	 * @param   pbData [in] : Ãùº¬»ôµê  
	 * @return  void
	 *
	 */
	void  BMP2XYZ(BYTE* pbData = NULL);

	/**
	 * @brief   R, G, B»°ÃÔµÛµá¹¢À¾ BMP°é¼¿Ì© Ãùº¬»ôµê¶¦ ÊÐ²÷³Þ.
	 *
	 * @return  void 
	 *
	 */
	void  XYZ2BMP();

	//!   Ãùº¬Ì© ³­¹¢±¨ÊÍ°á°£
	BYTE* m_pbTemp;

protected:
	//!  Ãùº¬Ì© µÛ¸ÜÂö, ÃÔ¸ÜÂö
	int  m_nItx, m_nIty;
	//!  µÛ°¾ËÁºã, ÃÔ°¾ËÁºã 
	int  m_nStepx, m_nStepy;
	//!  Ãùº¬Ì© ³­¹¢¼è¼¬»ô   
	BYTE*    m_pbIterImage;
	//!  Ãùº¬¼è¼¬»ô  
	CoImage* m_pImage;
};


inline
ImageIterator::ImageIterator(CoImage *pImage): m_pImage(pImage)
{
	m_pbTemp = new BYTE[m_pImage->GetHeight() * m_pImage->GetEffWidth()];
	if (m_pImage) 
		m_pbIterImage = m_pbTemp;
	m_nItx = m_nIty = 0;
	m_nStepx = m_nStepy = 0;
}

inline
ImageIterator::~ImageIterator ()
{
	delete []m_pbTemp;
}

inline void ImageIterator::Reset()
{
	if (m_pImage) m_pbIterImage = m_pbTemp;
	else	 m_pbIterImage=0;
	m_nItx = m_nIty = 0;
}

inline void ImageIterator::Upset()
{
	m_nItx = 0;
	m_nIty = m_pImage->GetHeight()-1;
	m_pbIterImage = m_pbTemp + m_pImage->GetEffWidth()*(m_pImage->GetHeight()-1);
}

inline BOOL ImageIterator::NextRow()
{
	if (++m_nIty >= (int)m_pImage->GetHeight()) 
		return 0;
	m_pbIterImage += m_pImage->GetEffWidth();
	return 1;
}

inline BOOL ImageIterator::PrevRow()
{
	if (--m_nIty < 0) return 0;
	m_pbIterImage -= m_pImage->GetEffWidth();
	return 1;
}
/* AD - for interlace */
inline void ImageIterator::SetY(int y)
{
	if ((y < 0) || (y > (int)m_pImage->GetHeight())) 
		return;
	m_nIty = y;
	m_pbIterImage = m_pbTemp + m_pImage->GetEffWidth()*y;
}

inline void ImageIterator::SetRow(BYTE *buf, int n)
{
	if (n<0) n = (int)m_pImage->GetEffWidth();
	else n = MIN(n,(int)m_pImage->GetEffWidth());

	if ((m_pbIterImage!=NULL)&&(buf!=NULL)&&(n>0)) memcpy(m_pbIterImage,buf,n);
}

inline void ImageIterator::GetRow(BYTE *buf, int n)
{
	if ((m_pbIterImage!=NULL)&&(buf!=NULL)&&(n>0)) memcpy(buf,m_pbIterImage,n);
}

inline BYTE* ImageIterator::GetRow()
{
	return m_pbIterImage;
}

inline BYTE* ImageIterator::GetRow(int n)
{
	SetY(n);
	return m_pbIterImage;
}

inline BOOL ImageIterator::NextByte()
{
	if (++m_nItx < (int)m_pImage->GetEffWidth()) 
		return 1;
	else
		if (++m_nIty < (int)m_pImage->GetHeight())
		{
			m_pbIterImage += m_pImage->GetEffWidth();
			m_nItx = 0;
			return 1;
		}
		else
			return 0;
}

inline BOOL ImageIterator::PrevByte()
{
  if (--m_nItx >= 0) 
	  return 1;
  else
	  if (--m_nIty >= 0)
	  {
		  m_pbIterImage -= m_pImage->GetEffWidth();
		  m_nItx = 0;
		  return 1;
	  } 
	  else
		  return 0;
}

inline BOOL ImageIterator::NextStep()
{
	m_nItx += m_nStepx;
	if (m_nItx < (int)m_pImage->GetEffWidth()) 
		return 1;
	else 
	{
		m_nIty += m_nStepy;
		if (m_nIty < (int)m_pImage->GetHeight())
		{
			m_pbIterImage += m_pImage->GetEffWidth();
			m_nItx = 0;
			return 1;
		}
		else
			return 0;
	}
}

inline BOOL ImageIterator::PrevStep()
{
	m_nItx -= m_nStepx;
	if (m_nItx >= 0) 
		return 1;
	else 
	{       
		m_nIty -= m_nStepy;
		if (m_nIty >= 0 && m_nIty < (int)m_pImage->GetHeight()) 
		{
			m_pbIterImage -= m_pImage->GetEffWidth();
			m_nItx = 0;
			return 1;
		}
		else
			return 0;
	}
}

inline void ImageIterator::BMP2XYZ(BYTE* pbData)
{
	m_pImage->m_matX.Zero();
	m_pImage->m_matY.Zero();
	m_pImage->m_matZ.Zero();
	BYTE* pbTemp;
	if (pbData)
		pbTemp = pbData;
	else
		pbTemp = m_pbTemp;
	for (int iH = 0; iH < m_pImage->GetHeight(); iH ++)
	{
		BYTE* pbR = m_pImage->m_matX.data.ptr[m_pImage->GetHeight() - iH - 1];
		BYTE* pbG = m_pImage->m_matY.data.ptr[m_pImage->GetHeight() - iH - 1];
		BYTE* pbB = m_pImage->m_matZ.data.ptr[m_pImage->GetHeight() - iH - 1];
		if (m_pImage->m_Head.biClrUsed)
		{
			for (int iW = 0; iW < m_pImage->GetWidth(); iW ++)
				m_pImage->GetPaletteColor(
					m_pImage->GetPixelIndex(pbTemp, iW), 
					&pbR[iW], &pbG[iW], &pbB[iW]);
		}
		else
		{
			for (int iW = 0, iEffW = 0; iW < m_pImage->GetWidth(); iW ++)
			{
				pbB[iW] = pbTemp[iEffW ++];
				pbG[iW] = pbTemp[iEffW ++];
				pbR[iW] = pbTemp[iEffW ++];
			}
		}
		pbTemp += m_pImage->GetEffWidth();
	}
}

inline void ImageIterator::XYZ2BMP()
{
    m_pImage->SetBpp24();

	if (m_pbTemp)
		delete []m_pbTemp;
	m_pbTemp = new BYTE[m_pImage->GetHeight() * m_pImage->GetEffWidth()];

	BYTE* pbTemp  = m_pbTemp;
	if (m_pImage) 
		m_pbIterImage = m_pbTemp;
	
	m_nItx = m_nIty = 0;
	m_nStepx = m_nStepy = 0;
	
	for (int iH = 0; iH < m_pImage->GetHeight(); iH ++)
	{
		BYTE* pbR = m_pImage->m_matX.data.ptr[m_pImage->GetHeight() - iH - 1];
		BYTE* pbG = m_pImage->m_matY.data.ptr[m_pImage->GetHeight() - iH - 1];
		BYTE* pbB = m_pImage->m_matZ.data.ptr[m_pImage->GetHeight() - iH - 1];

		for (int iW = 0, iEffW = 0; iW < m_pImage->GetWidth(); iW ++)
		{
			pbTemp[iEffW ++] = pbB[iW];
			pbTemp[iEffW ++] = pbG[iW];
			pbTemp[iEffW ++] = pbR[iW];
		}
		pbTemp += m_pImage->GetEffWidth();
	}
}

}

#endif //_IMAGEITERATOR_H_
