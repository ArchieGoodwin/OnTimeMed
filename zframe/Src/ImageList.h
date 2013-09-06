/*!
 * \file	ipCoreABC.h
 * \ingroup cvlibip
 * \brief   Ãùº¬¹ÉµÛË¾ ±ã¶®Â×±¨ Ì¡ÂÙ ÂÜºã´ÉË¾ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§
 */

#if !defined(_ImageList_H_)
#define _ImageList_H_

#include "cvlibbase/inc/CoImage.h"
#include "cvlibbase/inc/Template.h"
#include "cvlibbase/inc/StringArray.h"

namespace CVLib
{

enum enAlignType
{
	AT_Center,
	AT_Up,
	AT_Down,
	AT_Left,
	AT_Right
};
enum enAxeType
{
	AxeX,
		AxeY,
		AxeZ
};

class CVLIB_DECLSPEC ImageList
{
public:
protected:
	//! This variable represents the number of images in the image list.
	/**
	   \note if \c size==0, the image list is empty.
	**/
	unsigned int m_nSize;

	//! This variable represents a pointer to the first \c CoImage image of the list.
	/**
	   \note the images are stored continuously in memory.
	   \note If the list is empty, \c data=NULL.
	**/
	CoImage *m_pData;                      //!< Pointer to the first image of the image list.
public:
	//! Create a list of \p n new images, each having size (\p width,\p height,\p depth,\p dim).
	ImageList(const unsigned int n=0,const unsigned int width=0,const unsigned int height=1,
	  const unsigned int depth=1, const unsigned int dim=1);

	// ! Create a list of \p n copy of the input image.
	ImageList(const unsigned int n, CoImage& img);

	//! Copy constructor.
	ImageList(ImageList& list);

	//! Create a list from a single image \p img.
	ImageList(CoImage& img);

	//! Create a list from two images \p img1 and \p img2 (images are copied).
	ImageList(CoImage& img1,CoImage& img2);

	//! Create a list from three images \p img1,\p img2 and \p img3 (images are copied).
	ImageList(CoImage& img1,CoImage& img2,CoImage& img3);

	//! Create a list from four images \p img1,\p img2,\p img3 and \p img4 (images are copied).
	ImageList(CoImage& img1,CoImage& img2,CoImage& img3,CoImage& img4);

	//! Copy a list into another one.
	ImageList& operator=(ImageList& list);

	//! Destructor
	~ImageList() 
	{
		if (m_pData)
		{
			for (unsigned int i = 0; i < m_nSize; i ++)
				m_pData[i].Release();
			delete[] m_pData;
			m_pData = NULL;
		}
	}

	//! Empty list
	ImageList& Empty() 
	{
		return ImageList().SwapList(*this);
	}

	//@}
	//------------------------------------------
	//------------------------------------------
	//
	//! \name List operations
	//@{
	//------------------------------------------
	//------------------------------------------

	int GetCount() const {return m_nSize;}
	//! Return a reference to the i-th element of the image list.
	CoImage& operator[](const unsigned int pos) const
	{
		return m_pData[pos];
	}

	//! Equivalent to ImageList::operator[]
	CoImage& operator()(const unsigned int pos) const 
	{
		return (*this)[pos];
	}

	//! Insert a copy of the image \p img into the current image list, at position \p pos.
	BOOL Insert(CoImage& img, const unsigned int nPos);
	BOOL Insert(CoImage& img);

	void Remove(const unsigned int nPos);
	void Remove();

 	//! Append images of a list into a single image (which is returned), by concatenating them along the specified axe \p axe,
 	// centering them using the alignment \p align.
 	CoImage* GetAppend(const int nAxe,const int nAlign);
	CoImage* GetAppendAlign (const int nAxe, const int nAlign=1);

	//! Same as \ref cimg::wait()
	// 	const ImageList& Wait(const unsigned int milliseconds) const { cimg::wait(milliseconds); return *this;  }
	ImageList& SwapList(ImageList& list)
	{
		int tmp = list.m_nSize; list.m_nSize = m_nSize; m_nSize = tmp;
		CoImage* ptr = list.m_pData; list.m_pData = m_pData; m_pData = ptr;
		return list;
	}
};

template class CVLIB_DECLSPEC Array<CoImage*>;
class CVLIB_DECLSPEC ImagePtrList : public Array<CoImage*>
{
public:
	//! Create a list from a single image \p img.
	ImagePtrList(){}
	ImagePtrList(CoImage* pImg);
	ImagePtrList(CoImage* pImg1, CoImage* pImg2);
	ImagePtrList(CoImage* pImg1, CoImage* pImg2, CoImage* pImg3);
	ImagePtrList(CoImage* pImg1, CoImage* pImg2, CoImage* pImg3, CoImage* pImg4);

	//! Destructor
	~ImagePtrList() 
	{
		Release();
	}

	void Release()
	{
		for (int i = 0; i < GetSize(); i ++)
			delete GetAt(i);
		RemoveAll();
	}

	void DetachAll();
 	//! Append images of a list into a single image (which is returned), by concatenating them along the specified axe \p axe,
 	// centering them using the alignment \p align.
 	void GetAppend(const int nAxe,const int nAlign, CoImage* pImage);
};

class CVLIB_DECLSPEC ImageDescPtrList
{
public:
	//! Create a list from a single image \p img.
	ImageDescPtrList(){}
	
	//! Destructor
	~ImageDescPtrList() 
	{
		Release();
	}
	
	int GetCount () {return m_images.GetSize();}

	void Release()
	{
		for (int i = 0; i < m_images.GetSize(); i ++)
			delete m_images.GetAt(i);
		m_images.RemoveAll();
		m_szDescArray.RemoveAll();
	}

	void DetachAll ()
	{
		m_images.RemoveAll();
		m_szDescArray.RemoveAll();
	}

	int Add (CoImage* pImage, const SString& szDesc=SString("noname"))
	{
		m_szDescArray.Add(szDesc);
		return m_images.Add (pImage);
	}

	CoImage* GetImage(int nIdx) {return m_images[nIdx];}
	SString GetDesc(int nIdx) {return m_szDescArray[nIdx];}
public:
	Vector<CoImage*> m_images;
	StringArray		m_szDescArray;
};

class CVLIB_DECLSPEC MatDescPtrList : public Vector<Mat*>
{
public:
	MatDescPtrList(){}

	//! Destructor
	~MatDescPtrList() 
	{
		Release();
	}

	void Release()
	{
		for (int i = 0; i < GetSize(); i ++)
			delete GetAt(i);
		RemoveAll();
	}

	void DetachAll();
 	//! Append images of a list into a single image (which is returned), by concatenating them along the specified axe \p axe,
 	// centering them using the alignment \p align.
 	void GetAppend(const int nAxe,const int nAlign, Mat& image);
};

}

#endif //_ImageList_H_