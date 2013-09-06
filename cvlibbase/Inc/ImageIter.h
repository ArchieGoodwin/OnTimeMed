/*! 
 * \file    ImageIter.h
 * \ingroup base
 * \brief   ��������˾ ̡�� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _IMAGEITERATOR_H_
#define _IMAGEITERATOR_H_

#include "CoImage.h"

namespace CVLib
{
/**
 * @brief  ��������˾ �����ײ� �е���
 */
class CVLIB_DECLSPEC ImageIterator
{
	friend class CoImage;
public:
	
	//! �龢��
	ImageIterator ( CoImage* pImage );
	//! �;˻�
	virtual ~ImageIterator ();

	// Iterators
	/**
	 * @brief   �����輬���� ����̩ �� ������ �����ٳ�.
	 *
	 * @return  void
	 *
	 */
	void  Reset ();
	
	/**
	 * @brief   �����輬���� ����̩ ���������� �����ٳ�.
	 *
	 * @return  void  
	 *
	 */
	void  Upset ();
	
	/**
	 * @brief   ̰�����Ṣ�� �������Ͱᰣ��� �״� ���Ի��궦 Ⱥ��.
	 *
	 * @param   buf [in] : ̰�����Ѻ� 
	 * @param   n   [in] : ȸ�� �� ������� 
	 * @return  void
	 *
	 * @see   GetRow(BYTE *buf, int n)
	 *
	 */
	void  SetRow(BYTE *buf, int n);

	/**
	 * @brief   �������Ͱᰣ˺�Ṣ�� ̰������� �״� ���Ի��궦 ���̴��ͳ�.
	 *
	 * @param   buf [out] : ̰�����Ѻϡ�
	 * @param   n   [in]  : �ϱ� �� ������� ��
	 * @return  void
	 *
	 * @see  SetRow(BYTE *buf, int n)
	 *
	 */
	void  GetRow(BYTE *buf, int n);
	
	/**
	 * @brief   �������Ͱᰣ˺�Ṣ�� �� ������̩ ���궦 �ϲ���.
	 *
	 * @return  �ϱ� �� ���� 
	 *
	 * @see  SetByte()
	 *
	 */
	BYTE  GetByte( ) { return m_pbIterImage[m_nItx]; }

	/**
	 * @brief   �������Ͱᰣ�� �� ������̩ ���궦 Ⱥ��.
	 *
	 * @param   b [in] : ȸ�� ��  ���� 
	 * @return  void
	 *
	 * @see  GetByte()
	 *
	 */
	void  SetByte(BYTE b) { m_pbIterImage[m_nItx] = b; }

	/**
	 * @brief   ���̩ ���Ի��궦 �ϲ���.
	 * @return  �ϱ� �� ���� 
	 *
	 */
	BYTE* GetRow(void);
	
	/**
	 * @brief   �輳�� ����̩ ���Ի��궦 �ϲ���.
	 *
	 * @param   n [in] : �Ը��� 
	 * @return  �ϱ� �� ���� 
	 *
	 */
	BYTE* GetRow(int n);
	
	/**
	 * @brief   �����輬���� ������˺�� �˴����ֳ�.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see   PrevRow()
	 *
	 */
	BOOL  NextRow();
	
	/**
	 * @brief   �����輬���� �˼���˺�� �˴����ֳ�. 
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see  NextRow()
	 *
	 */
	BOOL  PrevRow();
	
	/**
	 * @brief   �����輬���� ������������� �˴����ֳ�.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see  PrevByte()
	 *
	 */
	BOOL  NextByte();
	
	/**
	 * @brief   �����輬���� �˼���������� �˴����ֳ�.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see  NextByte()
	 *
	 */
	BOOL  PrevByte();

	/**
	 * @brief   �Ա� ��̩ �������˾ �����ٳ�.
	 *
	 * @param   x [in] : �԰����� 
	 * @param   y [in] : �۰����� 
	 * @return  void
	 *
	 * @see  GetSteps()
	 *
	 */
	void  SetSteps(int x, int y=0) {  m_nStepx = x; m_nStepy = y; }

	/**
	 * @brief   �Ա� ��̩ �����㶦 �в���.
	 *
	 * @param   x [out] : �԰����� 
	 * @param   y [out] : �۰����� 
	 * @return  void
	 *
	 * @see  SetSteps()
	 *
	 */
	void  GetSteps(int *x, int *y) {  *x = m_nStepx; *y = m_nStepy; }

	/**
	 * @brief   �����輬���� �������� ���� �˴����ֳ�.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see   PrevStep()
	 *
	 */
	BOOL  NextStep();

	/**
	 * @brief   �����輬���� �������� ʼ˺�� �˴����ֳ�.
	 *
	 * @return  �°��˷� true, ʭ���� false
	 *
	 * @see   NextStep()
	 *
	 */
	BOOL  PrevStep();

	/**
	 * @brief   �����輬���� �輳�� ����� �˴����ֳ�. 
	 *
	 * @param   y [in] : �Ը���
	 * @return  void
	 *
	 * @see  GetY()
	 *
	 */
	void  SetY(int y);	/* AD - for interlace */

	/**
	 * @brief   ���̩ �Ը����� �в���. 
	 *
	 * @return  �Ը��� 
	 *
	 * @see  SetY()
	 *
	 */
	int   GetY() {return m_nIty;}

	/**
	 * @brief   BMP�鼿̩ ��������Ṣ�� R, G, B���Ե�˾ �龢�ٳ�.
	 *
	 * @param   pbData [in] : ��������  
	 * @return  void
	 *
	 */
	void  BMP2XYZ(BYTE* pbData = NULL);

	/**
	 * @brief   R, G, B���Ե۵Ṣ�� BMP�鼿̩ �������궦 �в���.
	 *
	 * @return  void 
	 *
	 */
	void  XYZ2BMP();

	//!   ����̩ �������Ͱᰣ
	BYTE* m_pbTemp;

protected:
	//!  ����̩ �۸���, �Ը���
	int  m_nItx, m_nIty;
	//!  �۰�����, �԰����� 
	int  m_nStepx, m_nStepy;
	//!  ����̩ �����輬��   
	BYTE*    m_pbIterImage;
	//!  �����輬��  
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
