#include "ImageList.h"

namespace CVLib
{
   
const unsigned int mblock=1024;

ImageList::ImageList(const unsigned int n/*=0*/,const unsigned int width/*=0*/,const unsigned int height/*=1*/,
	const unsigned int depth/*=1*/, const unsigned int dim/*=1*/):m_nSize(n)
{
	if (n)
	{
		m_pData = new CoImage[(mblock+1)*mblock];
		for (unsigned int i = 0; i < m_nSize; i ++)
			m_pData[i]=CoImage(width,height,depth,dim);
	} 
	else 
		m_pData = NULL;
}

ImageList::ImageList(const unsigned int n, CoImage& img):m_nSize(n)
{
	if (n) 
	{
		m_pData = new CoImage[(n/mblock+1)*mblock];
		for (unsigned int i = 0; i < m_nSize; i ++)
			m_pData[i]=img;
	} 
	else 
		m_pData = NULL;
}

ImageList::ImageList(ImageList& list):m_nSize(list.m_nSize)
{
	if (m_nSize)
	{
		m_pData = new CoImage[(m_nSize/mblock+1)*mblock];
		for (unsigned int i = 0; i < m_nSize; i ++)
			m_pData[i]=list[i];
	} 
	else 
		m_pData = NULL;
}

ImageList::ImageList(CoImage& img):m_nSize(1),m_pData(NULL)
{ 
	m_pData = new CoImage[mblock];
	m_pData[0] = img;
}

ImageList::ImageList(CoImage& img1,CoImage& img2):m_nSize(2)
{
	m_pData = new CoImage[mblock];
	m_pData[0] = img1;
	m_pData[1] = img2;
}

ImageList::ImageList(CoImage& img1,CoImage& img2,CoImage& img3):m_nSize(3)
{
	m_pData = new CoImage[mblock];
	m_pData[0] = img1;
	m_pData[1] = img2;
	m_pData[2] = img3;
}

ImageList::ImageList(CoImage& img1, CoImage& img2,CoImage& img3,CoImage& img4):m_nSize(4)
{
	m_pData = new CoImage[mblock];
	m_pData[0] = img1;
	m_pData[1] = img2;
	m_pData[2] = img3;
	m_pData[3] = img4;
}

ImageList& ImageList::operator=(ImageList& list)
{
	if (&list==this)
		return *this;
	return ImageList(list).SwapList(*this);
}

BOOL ImageList::Insert(CoImage& img, const unsigned int nPos)
{
	if (nPos > m_nSize)
		return FALSE;
	CoImage empty;
	CoImage* pNewImg = (!((++m_nSize)%mblock) || !m_pData) ? new CoImage[(m_nSize/mblock+1) * mblock] : NULL;
	if (!m_pData)
	{
		m_pData = pNewImg;
		*m_pData = img;
	}
	else
	{
		if (pNewImg)
		{
			unsigned int i;
			for (i = 0; i < nPos; i ++)
				pNewImg[i] = m_pData[i];
			if (nPos != m_nSize - 1) 
			{
				for (i = 0; i < m_nSize-1-nPos; i ++)
					pNewImg[nPos+1+i] = m_pData[nPos+i];
			}
			for (i = 0; i < m_nSize-1; i ++)
				m_pData[i] = empty;
			delete[] m_pData;
			m_pData = pNewImg;
		}
		else if (nPos != m_nSize - 1) 
		{
			for (unsigned int i = 0; i < m_nSize-1-nPos; i ++)
				m_pData[nPos+1 + i] = m_pData[i+nPos];
		}
		m_pData[nPos] = img;
	}
	empty.Release();
	return TRUE;
}

//! Append a copy of the image \p img at the current image list.
BOOL ImageList::Insert(CoImage& img)
{
	return Insert(img, m_nSize);
}

void ImageList::Remove(const unsigned int nPos)
{
	if (nPos>=m_nSize)
		return;

	m_nSize--;
	if (nPos!=m_nSize)
	{ 
		for (unsigned int i = 0; i < m_nSize - nPos; i ++)
			m_pData[nPos + i] = m_pData[nPos + 1 + i];
	}
	m_pData[m_nSize].Release();
	return;
}

//! Remove the last image from the image list.
void ImageList::Remove()
{
	Remove(m_nSize);
}

CoImage* ImageList::GetAppend(const int nAxe,const int nAlign)
{
	int dx = 0, dy = 0, nPos = 0;
	CoImage* pRes = new CoImage;
	switch (nAxe)
	{
	case AxeX:
		{
			for (unsigned int i = 0; i < m_nSize; i ++)
			{
				const CoImage& img = m_pData[i];
				dx += img.GetWidth();
				dy = MAX(dy ,img.GetHeight());
			}
			pRes->Create(dy, dx, MAT_Tbyte, CoImage::enCOLOR_MODE);
			switch (nAlign)
			{
			case AT_Up:
				{
					for (unsigned int j = 0; j < m_nSize; j ++)
					{
						pRes->DrawImage(m_pData[j], nPos, 0);
						nPos += m_pData[j].GetWidth();
					}
				}
				break;
			case AT_Down:
				{
					for (unsigned int j = 0; j < m_nSize; j ++)
					{
						pRes->DrawImage(m_pData[j], nPos, dy - m_pData[j].GetHeight());
						nPos += m_pData[j].GetWidth();
					}
				}
				break;
			case AT_Center:
				{
					for (unsigned int j = 0; j < m_nSize; j ++)
					{
						pRes->DrawImage(m_pData[j], nPos, (dy - m_pData[j].GetHeight()) / 2);
						nPos += m_pData[j].GetWidth();
					}
				}
				break;
			}
		}
		break;
	case AxeY:
		{
			for (unsigned int i = 0; i < m_nSize; i ++)
			{
				const CoImage& img = m_pData[i];
				dy += img.GetHeight();
				dx = MAX(dx ,img.GetWidth());
			}
			pRes->Create(dy, dx, MAT_Tbyte, CoImage::enCOLOR_MODE);
			switch (nAlign)
			{
			case AT_Left:
				{
					for (unsigned int j = 0; j < m_nSize; j ++)
					{
						pRes->DrawImage(m_pData[j], 0, nPos);
						nPos += m_pData[j].GetHeight();
					}
				}
				break;
			case AT_Right:
				{
					for (unsigned int j = 0; j < m_nSize; j ++)
					{
						pRes->DrawImage(m_pData[j], dx - m_pData[j].GetWidth(), nPos);
						nPos += m_pData[j].GetHeight();
					}
				}
				break;
			case AT_Center:
				{
					for (unsigned int j = 0; j < m_nSize; j ++)
					{
						pRes->DrawImage(m_pData[j], (dx - m_pData[j].GetWidth()) / 2, nPos);
						nPos += m_pData[j].GetHeight();
					}
				}
				break;
			}
		}
		break;
	case AxeZ:
		{
		}
		break;
	}
	return pRes;
}

CoImage* ImageList::GetAppendAlign(const int nAxe, const int nAlign/*=1*/)
{
	int dx = 0, dy = 0;
	CoImage* pRes = new CoImage;
	switch (nAxe)
	{
	case AxeX:
		{
			unsigned int i;
			int ywid=0;
			int xwid=0;
			for (i = 0; i < m_nSize; i ++)
			{
				const CoImage& img = m_pData[i];
				if ( (i+1) % nAlign == 0)
				{
					xwid = MAX(xwid, img.GetWidth());
					dx += xwid;
					xwid=0;
					ywid += img.GetHeight();
					dy = MAX(dy, ywid);
					ywid=0;
				}
				else
				{
					xwid = MAX(xwid, img.GetWidth());
					ywid += img.GetHeight();
				}
			}
			if (m_nSize%nAlign)
			{
				dx += xwid;
				dy = MAX(dy, ywid);
			}
			pRes->Create(dy, dx, MAT_Tbyte);
			pRes->SetBkColor(RGB(192,192,192));
			ywid=0;
			xwid=0;
			dx=dy=0;
			for (i = 0; i < m_nSize; i ++)
			{
				const CoImage& img = m_pData[i];
				if ( (i+1) % nAlign == 0)
				{
					pRes->DrawImage(img, dx, dy);
					xwid = MAX(xwid, img.GetWidth());
					dx += xwid;
					xwid=0;
					dy=0;
				}
				else
				{
					pRes->DrawImage(img, dx, dy);
					xwid = MAX(xwid, img.GetWidth());
					dy += img.GetHeight();
				}
			}
		}
		break;
	case AxeY:
		{
			assert (false);
		}
		break;
	case AxeZ:
		{
		}
		break;
	}
	return pRes;
}

ImagePtrList::ImagePtrList(CoImage* pImg)
{
	Add (pImg);
}

ImagePtrList::ImagePtrList(CoImage* pImg1, CoImage* pImg2)
{
	Add (pImg1);
	Add (pImg2);
}

ImagePtrList::ImagePtrList(CoImage* pImg1, CoImage* pImg2, CoImage* pImg3)
{
	Add (pImg1);
	Add (pImg2);
	Add (pImg3);
}

ImagePtrList::ImagePtrList(CoImage* pImg1, CoImage* pImg2, CoImage* pImg3, CoImage* pImg4)
{
	Add (pImg1);
	Add (pImg2);
	Add (pImg3);
	Add (pImg4);
}

void ImagePtrList::GetAppend(const int nAxe,const int nAlign, CoImage* pImage)
{
	int dx = 0, dy = 0, nPos = 0;
	pImage->Release();
	switch (nAxe)
	{
	case AxeX:
		{
			for (int i = 0; i < m_nSize; i ++)
			{
				CoImage* img = GetAt(i);
				dx += img->GetWidth();
				dy = MAX(dy, img->GetHeight());
			}
			pImage->Create(dy, dx, MAT_Tbyte, CoImage::enCOLOR_MODE);
			switch (nAlign)
			{
			case AT_Up:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						pImage->DrawImage(*GetAt(j), nPos, 0);
						nPos += GetAt(j)->GetWidth();
					}
				}
				break;
			case AT_Down:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						pImage->DrawImage(*GetAt(j), nPos, dy - GetAt(j)->GetHeight());
						nPos += GetAt(j)->GetWidth();
					}
				}
				break;
			case AT_Center:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						pImage->DrawImage(*GetAt(j), nPos, (dy - GetAt(j)->GetHeight()) / 2);
						nPos += GetAt(j)->GetWidth();
					}
				}
				break;
			}
		}
		break;
	case AxeY:
		{
			for (int i = 0; i < m_nSize; i ++)
			{
				CoImage* img = GetAt(i);
				dy += img->GetHeight();
				dx = MAX(dx ,img->GetWidth());
			}
			pImage->Create(dy, dx, MAT_Tbyte, CoImage::enCOLOR_MODE);
			switch (nAlign)
			{
			case AT_Left:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						pImage->DrawImage(*GetAt(j), 0, nPos);
						nPos += GetAt(j)->GetHeight();
					}
				}
				break;
			case AT_Right:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						pImage->DrawImage(*GetAt(j), dx - GetAt(j)->GetWidth(), nPos);
						nPos += GetAt(j)->GetHeight();
					}
				}
				break;
			case AT_Center:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						pImage->DrawImage(*GetAt(j), (dx - GetAt(j)->GetWidth()) / 2, nPos);
						nPos += GetAt(j)->GetHeight();
					}
				}
				break;
			}
		}
		break;
	case AxeZ:
		{
		}
		break;
	}
}


/************************************************************************************/
void MatDescPtrList::GetAppend(const int nAxe,const int nAlign, Mat& image)
{
	if (GetSize() == 0)
	{
		image.Release();
		return;
	}
	int dx = 0, dy = 0, nPos = 0;
	image.Release();
	switch (nAxe)
	{
	case AxeX:
		{
			for (int i = 0; i < m_nSize; i ++)
			{
				Mat* img = GetAt(i);
				dx += img->Cols();
				dy = MAX(dy, img->Rows());
			}
			image.Create(dy, dx, MAT_Tbyte3);
			switch (nAlign)
			{
			case AT_Up:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						image.DrawMat(*GetAt(j), Point2i(nPos, 0));
						nPos += GetAt(j)->Cols();
					}
				}
				break;
			case AT_Down:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						image.DrawMat(*GetAt(j), Point2i(nPos, dy - GetAt(j)->Rows()));
						nPos += GetAt(j)->Cols();
					}
				}
				break;
			case AT_Center:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						image.DrawMat(*GetAt(j), Point2i(nPos, (dy - GetAt(j)->Rows()) / 2));
						nPos += GetAt(j)->Cols();
					}
				}
				break;
			}
		}
		break;
	case AxeY:
		{
			for (int i = 0; i < m_nSize; i ++)
			{
				Mat* img = GetAt(i);
				dy += img->Rows();
				dx = MAX(dx ,img->Cols());
			}
			image.Create(dy, dx, MAT_Tbyte3);
			switch (nAlign)
			{
			case AT_Left:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						image.DrawMat(*GetAt(j), Point2i(0, nPos));
						nPos += GetAt(j)->Rows();
					}
				}
				break;
			case AT_Right:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						image.DrawMat(*GetAt(j), Point2i(dx - GetAt(j)->Cols(), nPos));
						nPos += GetAt(j)->Rows();
					}
				}
				break;
			case AT_Center:
				{
					for (int j = 0; j < m_nSize; j ++)
					{
						image.DrawMat(*GetAt(j), Point2i((dx - GetAt(j)->Cols()) / 2, nPos));
						nPos += GetAt(j)->Rows();
					}
				}
				break;
			}
		}
		break;
	case AxeZ:
		{
		}
		break;
	}
}

void MatDescPtrList::DetachAll()
{
	RemoveAll();
}


}