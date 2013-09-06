/*! 
 * \file	CoImage.cpp
 * \ingroup base
 * \brief	Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */
#include "cvlibmacros.h"

#if CVLIB_OS==CVLIB_OS_WIN32
#include <windows.h>
#include "TCHAR.h"
#undef GetRValue
#undef GetGValue
#undef GetBValue
#endif

#include "CoImage.h"
#include "MatOperation.h"
#include "CoImageBMP.h"
#include "CoImageJPG.h"
#include "CoImagePNG.h"
#include "CoImageTIF.h"
#include "CoImageJAS.h"

namespace CVLib {

/**
 * @brief ´¸ Êú°ýºãÂô¸êºãÌ© °ªË¾ ¸ÆÄä³Þ.
 *
 * @param nA [out]: ¸êºã 1
 * @param nB [out]: ¸êºã 2
 * @return	ÊÖËÁ
 */
static void SWAP_func(int &nA,int &nB)
{
	int nTmpswap;
	nTmpswap = nA; nA = nB; nB = nTmpswap;
}
CoImage::operator Mat() const
{
	assert (IsValid());
	TYPE type= (TYPE)((int)m_matX.Type()+(2<<CVLIB_CN_SHIFT));
	Mat mat(m_nRows,m_nCols,type);
	int i,j;
	switch (m_matX.Type())
	{
	case MAT_Tbyte:
		for (i=0;i<m_nRows; i++) for (j=0;j<m_nCols;j++)
		{
			mat.data.ptr[i][j*3]=m_matX.data.ptr[i][j];
			mat.data.ptr[i][j*3+1]=m_matY.data.ptr[i][j];
			mat.data.ptr[i][j*3+2]=m_matZ.data.ptr[i][j];
		}
		break;
	case MAT_Tshort:
		for (i=0;i<m_nRows; i++) for (j=0;j<m_nCols;j++)
		{
			mat.data.s[i][j*3]=m_matX.data.s[i][j];
			mat.data.s[i][j*3+1]=m_matY.data.s[i][j];
			mat.data.s[i][j*3+2]=m_matZ.data.s[i][j];
		}
		break;
	case MAT_Tint:
		for (i=0;i<m_nRows; i++) for (j=0;j<m_nCols;j++)
		{
			mat.data.i[i][j*3]=m_matX.data.i[i][j];
			mat.data.i[i][j*3+1]=m_matY.data.i[i][j];
			mat.data.i[i][j*3+2]=m_matZ.data.i[i][j];
		}
		break;
	case MAT_Tfloat:
		for (i=0;i<m_nRows; i++) for (j=0;j<m_nCols;j++)
		{
			mat.data.fl[i][j*3]=m_matX.data.fl[i][j];
			mat.data.fl[i][j*3+1]=m_matY.data.fl[i][j];
			mat.data.fl[i][j*3+2]=m_matZ.data.fl[i][j];
		}
		break;
	case MAT_Tdouble:
		for (i=0;i<m_nRows; i++) for (j=0;j<m_nCols;j++)
		{
			mat.data.db[i][j*3]=m_matX.data.db[i][j];
			mat.data.db[i][j*3+1]=m_matY.data.db[i][j];
			mat.data.db[i][j*3+2]=m_matZ.data.db[i][j];
		}
		break;
	}
	return mat;
}

CoImage::CoImage ()
{
	m_nChannel = 0;
	m_nRows = m_nCols = 0;
	m_strName="";
	m_ppBands = NULL;
	Startup (0);
	m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	m_pPalette=0;
	InitBMPHeader (m_nCols, m_nRows);
}

CoImage::~CoImage ()
{
	Release();
}

CoImage::CoImage (const CoImage& img)
{
	if (this == &img) return;
	m_ppBands = NULL;
	m_pPalette = NULL;
	Create (img, TRUE);
}
CoImage::CoImage (const CoImage& img, const Rect& region)
{
	m_ppBands = NULL;
	m_pPalette = NULL;
	if (img.m_matX.SizeObject()!=region.SizeObject() || img.m_matX.Type() != m_matX.Type())
	{
		if (IsValid()) Release();
		Create(region.height, region.width, img.m_matX.Type());
	}
	switch (img.m_matX.Type())
	{
	case MAT_Tbyte:
		{
			for (int y=0; y<region.height; y++) for (int x=0; x<region.width; x++)
			{
				m_matX.data.ptr[y][x]=img.m_matX.data.ptr[y+region.y][x+region.x];
				m_matY.data.ptr[y][x]=img.m_matY.data.ptr[y+region.y][x+region.x];
				m_matZ.data.ptr[y][x]=img.m_matZ.data.ptr[y+region.y][x+region.x];
			}
		}
		break;
	case MAT_Tshort:
		{
			for (int y=0; y<region.height; y++) for (int x=0; x<region.width; x++)
			{
				m_matX.data.s[y][x]=img.m_matX.data.s[y+region.y][x+region.x];
				m_matY.data.s[y][x]=img.m_matY.data.s[y+region.y][x+region.x];
				m_matZ.data.s[y][x]=img.m_matZ.data.s[y+region.y][x+region.x];
			}
		}
		break;
	case MAT_Tint:
		{
			for (int y=0; y<region.height; y++) for (int x=0; x<region.width; x++)
			{
				m_matX.data.i[y][x]=img.m_matX.data.i[y+region.y][x+region.x];
				m_matY.data.i[y][x]=img.m_matY.data.i[y+region.y][x+region.x];
				m_matZ.data.i[y][x]=img.m_matZ.data.i[y+region.y][x+region.x];
			}
		}
		break;
	case MAT_Tfloat:
		{
			for (int y=0; y<region.height; y++) for (int x=0; x<region.width; x++)
			{
				m_matX.data.fl[y][x]=img.m_matX.data.fl[y+region.y][x+region.x];
				m_matY.data.fl[y][x]=img.m_matY.data.fl[y+region.y][x+region.x];
				m_matZ.data.fl[y][x]=img.m_matZ.data.fl[y+region.y][x+region.x];
			}
		}
		break;
	case MAT_Tdouble:
		{
			for (int y=0; y<region.height; y++) for (int x=0; x<region.width; x++)
			{
				m_matX.data.db[y][x]=img.m_matX.data.db[y+region.y][x+region.x];
				m_matY.data.db[y][x]=img.m_matY.data.db[y+region.y][x+region.x];
				m_matZ.data.db[y][x]=img.m_matZ.data.db[y+region.y][x+region.x];
			}
		}
		break;
	}
}

CoImage::CoImage (const Mat& mImg, BOOL fCopy /*= FALSE*/)
{
	m_ppBands = NULL;
	m_pPalette = NULL;
	Create (mImg, fCopy);
}

CoImage::CoImage (int nRows, int nCols, int nMode, int nChannel)
{
	m_ppBands = NULL;
	m_pPalette = NULL;
	Create (nRows, nCols, nMode, nChannel);
}

CoImage::CoImage (BITMAPINFOHEADER1* pBI)
{
	m_ppBands = NULL;
	m_pPalette = NULL;
	Create (pBI);
}

CoImage::CoImage (const Mat& mX, const Mat& mY, const Mat& mZ)
{
	m_ppBands = NULL;
	m_pPalette = NULL;
	Create (mX, mY, mZ);
}

CoImage::CoImage (char* szFilename, DWORD imagetype)
{
	m_ppBands = NULL;
	Startup(imagetype);
	Load (szFilename, imagetype);
}

int CoImage::Create (const Mat& mX, const Mat& mY, const Mat& mZ)
{
	if (mX.Rows() != mY.Rows() || mX.Rows() != mZ.Rows() || mX.Cols() != mY.Cols() || mX.Cols() != mZ.Cols())
		return 0;
	if (mX.Type() != mY.Type() || mX.Type() != mZ.Type())
		return 0;

	Startup (0);
	m_nCols = mX.Cols();
	m_nRows = mX.Rows();
	m_nChannel = CoImage::enCOLOR_MODE;
	
	InitBMPHeader (m_nCols, m_nRows);
	
	m_matX.Create (mX, TRUE);
	m_matY.Create (mY, TRUE);
	m_matZ.Create (mZ, TRUE);
	
	int wBpp = 24;
	m_Info.dwEffWidth = ((((wBpp * m_nCols) + 31) / 32) * 4);
	SetName("");
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	return 1;
}

int	CoImage::Create (const CoImage& img, BOOL fCopy /*= FALSE*/)
{
	if (!img.IsValid())
		return 0;
	Startup (0);
	m_nCols = img.m_nCols;
	m_nRows = img.m_nRows;
	m_nChannel = img.m_nChannel;

	memcpy (&m_Head, &img.m_Head, sizeof(m_Head));
	memcpy (&m_Info, &img.m_Info, sizeof(m_Info));
	m_matX.Create (img.m_matX, fCopy);
	m_matY.Create (img.m_matY, fCopy);
	m_matZ.Create (img.m_matZ, fCopy);
#if CVLIB_IMG_SUPPORT_ALPHA
	if (img.m_matA.IsValid())
		m_matA.Create (img.m_matA, fCopy);
#endif //CVLIB_IMG_SUPPORT_ALPHA
	SetName(img.Name(true));
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	return 1;
}

int	CoImage::Create (const Mat& mImg, BOOL fCopy /*= FALSE*/)
{
	if (!mImg.IsValid())
		return 0;
	Startup (0);
	m_nCols = mImg.Cols();
	m_nRows = mImg.Rows();
	m_nChannel = CoImage::enCOLOR_MODE;

	InitBMPHeader (m_nCols, m_nRows);

	if (mImg.Channels()==1)
	{
		m_matX.Create (mImg, fCopy);
		m_matY.Create (mImg, fCopy);
		m_matZ.Create (mImg, fCopy);
	}
	else if (mImg.Channels()==3)
	{
		Create (m_nRows, m_nCols, MAT_Tbyte, m_nChannel);
		for (int i=0, k=0; i<m_nCols*m_nRows; i++)
		{
			m_matX.data.ptr[0][i]=mImg.data.ptr[0][k++];
			m_matY.data.ptr[0][i]=mImg.data.ptr[0][k++];
			m_matZ.data.ptr[0][i]=mImg.data.ptr[0][k++];
		}
	}
	else
	{
		assert (false);
	}

	int wBpp = 24;
	m_Info.dwEffWidth = ((((wBpp * m_nCols) + 31) / 32) * 4);
	SetName("");
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;

	return 1;
}

int	CoImage::Create (int nRows, int nCols, int nMode, int nChannel)
{
	Startup (0);
	m_nRows = nRows;
	m_nCols = nCols;
	m_nChannel = nChannel;

	m_matX.Create (nRows, nCols, (TYPE)nMode);
	m_matY.Create (nRows, nCols, (TYPE)nMode);
	m_matZ.Create (nRows, nCols, (TYPE)nMode);

	InitBMPHeader (m_nCols, m_nRows);

	int wBpp = 24;
	m_Info.dwEffWidth = ((((wBpp * m_nCols) + 31) / 32) * 4);
	SetName("");
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	return 1;
}

int	CoImage::CreateInfo (int nRows, int nCols, int wBpp, int nImageType)
{
	// prevent further actions if width or height are not vaild <Balabasnia>
	if ((nCols == 0) || (nRows == 0))
	{
		strcpy(m_Info.szLastError,"CoImage::Create : width and height must be greater than zero");
		return 0;
	}

    // Make sure bits per pixel is valid
    if		(wBpp <= 1)	wBpp = 1;
    else if (wBpp <= 4)	wBpp = 4;
    else if (wBpp <= 8)	wBpp = 8;
    else				wBpp = 24;

	if (IsValid())
	{
		if (GetWidth() != nCols || GetHeight() != nRows || m_Head.biBitCount != wBpp)
		{
			Release();
			// limit memory requirements (and also a check for bad parameters)
			if (((nCols*nRows*wBpp)>>8) > CVLIB_IMG_MAX_MEMORY)
			{
				strcpy(m_Info.szLastError,"CVLIB_IMG_MAX_MEMORY exceeded");
				return NULL;
			}
			Create (nRows, nCols, MAT_Tbyte, enCOLOR_MODE);
		}
	}
	else
	{
		Create (nRows, nCols, MAT_Tbyte, enCOLOR_MODE);
	}
	
	// set the correct bpp value
    switch (wBpp)
	{
	case 1:
		m_Head.biClrUsed = 2;	break;
	case 4:
		m_Head.biClrUsed = 16; break;
	case 8:
		m_Head.biClrUsed = 256; break;
	default:
		m_Head.biClrUsed = 0;
    }
	
	//set the common image informations
    m_Info.dwEffWidth = ((((wBpp * nCols) + 31) / 32) * 4);
    m_Info.dwType = nImageType;
	
    // initialize BITMAPINFOHEADER1
	m_Head.biSize = sizeof(BITMAPINFOHEADER1); //<ralphw>
    m_Head.biWidth = nCols;		// fill in width from parameter
    m_Head.biHeight = nRows;	// fill in height from parameter
    m_Head.biPlanes = 1;			// must be 1
    m_Head.biBitCount = (WORD)wBpp;		// from parameter
    m_Head.biCompression = BI_RGB;    
    m_Head.biSizeImage = m_Info.dwEffWidth * nRows;
// 	m_Head.biXPelsPerMeter = 0; See SetXDPI
// 	m_Head.biYPelsPerMeter = 0; See SetYDPI
    m_Head.biClrImportant = 0;

	int nSize = GetPaletteSize();
	if (nSize)
	{
		m_pPalette = malloc(nSize);
		if (!m_pPalette)
		{
			strcpy(m_Info.szLastError,"CoImage::Create can't allocate memory");
			return NULL;
		}
		//clear the palette
		memset(m_pPalette,0,nSize);
	}
#if CVLIB_IMG_SUPPORT_ALPHA
	if (m_matA.IsValid())
		AlphaDelete();
#endif //CVLIB_IMG_SUPPORT_ALPHA

	SetName("");
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	return 1;
}

int CoImage::Create (BITMAPINFOHEADER1* pBI)
{
	Startup (0);
	BYTE* pBMPData = (BYTE*)pBI + sizeof(BITMAPINFOHEADER1);
	int nH = pBI->biHeight;
	int nW = pBI->biWidth;

	memcpy (&m_Head, pBI, sizeof(m_Head));
	CreateInfo (nH, nW, 24, CVLIB_IMG_FORMAT_BMP);
	
	int nRealWidth = ( nW * pBI->biBitCount + 31 ) / 32 * 4;
	
	BYTE** ppbRData = m_matX.data.ptr;
	BYTE** ppbGData = m_matY.data.ptr;
	BYTE** ppbBData = m_matZ.data.ptr;
	if (pBI->biBitCount == 32)
		m_matA.Create (m_matX);
	BYTE** ppbAData = m_matA.data.ptr;

	int nTemp;
	WORD wRGB;
	int i, j;
	switch(pBI->biBitCount)
	{
	case 24:
		for (i = 0; i < nH; i ++)
			for (j = 0; j < nW; j ++)
			{
				nTemp = (nH - i - 1) * nRealWidth + j * 3;
				ppbBData[i][j] = pBMPData[nTemp];
				ppbGData[i][j] = pBMPData[nTemp + 1];
				ppbRData[i][j] = pBMPData[nTemp + 2];
			}
		break;
	case 32:
		for (i = 0; i < nH; i ++)
			for (j = 0; j < nW; j ++)
			{
				nTemp = (nH - i - 1) * nRealWidth + j * 4;
				ppbBData[i][j] = pBMPData[nTemp];
				ppbGData[i][j] = pBMPData[nTemp + 1];
				ppbRData[i][j] = pBMPData[nTemp + 2];
				ppbAData[i][j] = pBMPData[nTemp + 3];
			}
		break;
	case 16:
		for (i = 0; i < nH; i ++)
			for (j = 0; j < nW; j ++)
			{
				nTemp = (nH - i - 1) * nRealWidth + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				ppbBData[i][j] = (wRGB & 0x1F) << 3;
				ppbGData[i][j] = ((wRGB >> 5) & 0x1F) << 3;
				ppbRData[i][j] = ((wRGB >> 10) & 0x1F) << 3;
			}
		break;
	case 12:
		for (i = 0; i < nH; i ++)
			for (j = 0; j < nW; j ++)
			{
				nTemp = (nH - i - 1) * nRealWidth + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				ppbBData[i][j] = (wRGB & 0x1F) << 3;
				ppbGData[i][j] = ((wRGB >> 5) & 0x1F) << 3;
				ppbRData[i][j] = ((wRGB >> 10) & 0x1F) << 3;
			}
		break;
	case 8:
	case 4:
	case 1:
// 		if (pBI->bfOffBits != 0L)
// 			hFile->Seek(off + bf.bfOffBits,SEEK_SET);
// 		switch (dwCompression) {
		break;
	}
	int wBpp = 24;
	m_Info.dwEffWidth = ((((wBpp * m_nCols) + 31) / 32) * 4);
	SetName("");
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	return 1;
}

int CoImage::CreateFromArray (BYTE* pbData, int nRows, int nCols, int nBitsperpixel, int nBytesperline, bool bFlipImage)
{
	Startup (0);
	BYTE* pBMPData = pbData;
	int nH = nRows;
	int nW = nCols;

	memset (&m_Head, 0, sizeof(m_Head));
	CreateInfo (nH, nW, 24, CVLIB_IMG_FORMAT_BMP);
	m_pPalette=NULL;
#if CVLIB_IMG_SUPPORT_ALPHA
	if (nBitsperpixel == enBGRA || nBitsperpixel == enARGB)
		AlphaCreate();
#endif

	BYTE** ppbRData = m_matX.data.ptr;
	BYTE** ppbGData = m_matY.data.ptr;
	BYTE** ppbBData = m_matZ.data.ptr;
	BYTE** ppbAData = m_matA.data.ptr;
	for (int i = 0; i < nH; i ++)
	{
		int nDst = (bFlipImage ? (nH - i - 1) : i) * nBytesperline;
		for (int j = 0; j < nW; j ++)
		{
			int nTemp;
			WORD wRGB;
			switch(nBitsperpixel)
			{
			case enARGB:
				nTemp = nDst + j * 4;
				ppbAData[i][j] = pBMPData[nTemp];
				ppbRData[i][j] = pBMPData[nTemp + 1];
				ppbGData[i][j] = pBMPData[nTemp + 2];
				ppbBData[i][j] = pBMPData[nTemp + 3];
				break;
			case enBGRA:
				nTemp = nDst + j * 4;
				ppbBData[i][j] = pBMPData[nTemp];
				ppbGData[i][j] = pBMPData[nTemp + 1];
				ppbRData[i][j] = pBMPData[nTemp + 2];
				ppbAData[i][j] = pBMPData[nTemp + 3];
				break;
			case enBGR888:
				nTemp = nDst + j * 3;
				ppbBData[i][j] = pBMPData[nTemp];
				ppbGData[i][j] = pBMPData[nTemp + 1];
				ppbRData[i][j] = pBMPData[nTemp + 2];
				break;
			case enBGR555:
				nTemp = nDst + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				ppbBData[i][j] = (wRGB & 0x1F) << 3;
				ppbGData[i][j] = ((wRGB >> 5) & 0x1F) << 3;
				ppbRData[i][j] = ((wRGB >> 10) & 0x1F) << 3;
				break;
			case enBGR565:
				nTemp = nDst + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				ppbBData[i][j] = (wRGB & 0x1F) << 3;
				ppbGData[i][j] = ((wRGB >> 5) & 0x3F) << 2;
				ppbRData[i][j] = ((wRGB >> 11) & 0x1F) << 3;
				break;
			case 12:
				nTemp = nDst + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				ppbBData[i][j] = (wRGB & 0x1F) << 3;
				ppbGData[i][j] = ((wRGB >> 5) & 0x1F) << 3;
				ppbRData[i][j] = ((wRGB >> 10) & 0x1F) << 3;
				break;
			case 8:
				nTemp = nDst + j;
				ppbBData[i][j] = pBMPData[nTemp];
				ppbGData[i][j] = pBMPData[nTemp];
				ppbRData[i][j] = pBMPData[nTemp];
			default:
				break;
			}
		}
	}
	m_Info.dwEffWidth = ((((24 * m_nCols) + 31) / 32) * 4);
	SetName("");
	if (!m_ppBands) m_ppBands = new Mat*[3];
	m_ppBands[0]=&m_matX;
	m_ppBands[1]=&m_matY;
	m_ppBands[2]=&m_matZ;
	return 1;
}

void CoImage::Release ()
{
	m_matX.Release ();
	m_matY.Release ();
	m_matZ.Release ();
	m_matA.Release ();
	m_matG.Release ();
	SetName("");

	if (m_ppBands) 	delete []m_ppBands;
	m_ppBands = NULL;
	if (m_pPalette)
	{
		free (m_pPalette);
		m_pPalette = NULL;
	}
}

CoImage& CoImage::operator=(const CoImage& ImageSrc)
{
	if (this == &ImageSrc)
		return *this;
	Release();
	if (Create (ImageSrc, TRUE))
	{
		m_ppBands[0]=&m_matX;
		m_ppBands[1]=&m_matY;
		m_ppBands[2]=&m_matZ;
	}
	return *this;
}

BYTE*	CoImage::GetBMPInfoHeader() const
{
	assert (m_matX.Type() == MAT_Tbyte);
	int nH = m_nRows;
	int nW = m_nCols;
	
	int nRealWidth;
	if (!m_matA.IsValid())
		nRealWidth = ( nW * 24 + 31 ) / 32 * 4;
	else
		nRealWidth = ( nW * 32 + 31 ) / 32 * 4;
	
	BYTE* pBuf = new BYTE[sizeof(BITMAPINFOHEADER1) + nH * nRealWidth];
	BITMAPINFOHEADER1* pBMPHeader = (BITMAPINFOHEADER1*)pBuf;
	BYTE* pbData = pBuf + sizeof(BITMAPINFOHEADER1);
	
	memset (pBMPHeader, 0, sizeof(BITMAPINFOHEADER1));
	pBMPHeader->biBitCount = m_matA.IsValid() ? 32 : 24;
	pBMPHeader->biHeight = nH;
	pBMPHeader->biWidth = nW;
	pBMPHeader->biSizeImage = nH * nRealWidth;
	pBMPHeader->biSize = sizeof(BITMAPINFOHEADER1);
	pBMPHeader->biPlanes = 1;
	pBMPHeader->biXPelsPerMeter = (long) floor(96 * 10000.0 / 254.0 + 0.5);
	pBMPHeader->biYPelsPerMeter = (long) floor(96 * 10000.0 / 254.0 + 0.5);

	switch (m_nChannel)
	{
	case enGRAY_MODE:
		{
			BYTE** ppbData = m_matX.data.ptr;
			for (int i = 0; i < nH; i ++)
			{
				for (int j = 0; j < nW; j ++)
				{
					int nTemp = (nH - i - 1) * nRealWidth + j * 3;
					pbData[nTemp] = ppbData[i][j];
					pbData[nTemp + 1] = ppbData[i][j];
					pbData[nTemp + 2] = ppbData[i][j];
				}
			}
		}
		break;
	case enCOLOR_MODE:
		{
			BYTE** ppbRData = m_matX.data.ptr;
			BYTE** ppbGData = m_matY.data.ptr;
			BYTE** ppbBData = m_matZ.data.ptr;
			BYTE** ppbAData = m_matA.data.ptr;
			if (!m_matA.IsValid())
			{
				for (int i = 0; i < nH; i ++)
				{
					for (int j = 0; j < nW; j ++)
					{
						int nTemp = (nH - i - 1) * nRealWidth + j * 3;
						pbData[nTemp] = ppbBData[i][j];
						pbData[nTemp + 1] = ppbGData[i][j];
						pbData[nTemp + 2] = ppbRData[i][j];
					}
				}
			}
			else
			{
				for (int i = 0; i < nH; i ++)
				{
					for (int j = 0; j < nW; j ++)
					{
						int nTemp = (nH - i - 1) * nRealWidth + j * 4;
						pbData[nTemp] = ppbBData[i][j];
						pbData[nTemp + 1] = ppbGData[i][j];
						pbData[nTemp + 2] = ppbRData[i][j];
						pbData[nTemp + 3] = ppbAData[i][j];
					}
				}
			}
		}
		break;
	}
	return pBuf;
}

bool CoImage::Crop (Rect* pRect, CoImage* pDest)
{
	if (!IsValid())
		return false;
	long startx = MAX(0L,MIN(pRect->x, m_nCols));
	long endx = MAX(0L,MIN(pRect->x + pRect->width, m_nCols));
	long starty = MAX(0L,MIN(pRect->y, m_nRows));
	long endy = MAX(0L,MIN(pRect->y + pRect->height, m_nRows));

	if (startx==endx || starty==endy) return false;

	CoImage tempImage;
	tempImage.Create (endy-starty, endx-startx, m_matX.Type(), m_nChannel);

	CoImage* pTemp = pDest ? pDest : &tempImage;

	Rect rect;
	rect.x = startx; rect.y = starty;
	rect.width = endx - startx; rect.height = endy - starty;
	if (m_nChannel == enGRAY_MODE)
	{
		MatOp::CropMat (&pTemp->m_matX, &m_matX, rect);
		if (m_matA.IsValid())
			MatOp::CropMat (&pTemp->m_matA, &m_matA, rect);
	}
	else if (m_nChannel == enCOLOR_MODE)
	{
		MatOp::CropMat (&pTemp->m_matX, &m_matX, rect);
		MatOp::CropMat (&pTemp->m_matY, &m_matY, rect);
		MatOp::CropMat (&pTemp->m_matZ, &m_matZ, rect);
		if (m_matA.IsValid())
			MatOp::CropMat (&pTemp->m_matA, &m_matA, rect);
	}

	pTemp->m_nChannel = m_nChannel;
	pTemp->m_nRows = rect.height;
	pTemp->m_nCols = rect.width;

	pTemp->InitBMPHeader(pTemp->m_nCols, pTemp->m_nRows);

	if (!pDest)
	{
		Release();
		Create (tempImage, TRUE);
		tempImage.Release();
	}
	
	return true;
}

CoImage* CoImage::YCbCr (CoImage* pDest)
{
	int i, j;

	BYTE** pprR = m_matX.data.ptr;
	BYTE** pprG = m_matY.data.ptr;
	BYTE** pprB = m_matZ.data.ptr;
	if (pDest != NULL)
	{
		pDest->Create (m_nRows, m_nCols, MAT_Tbyte, m_nChannel);
		BYTE** pprY = pDest->m_matX.data.ptr;
		BYTE** ppCb = pDest->m_matY.data.ptr;
		BYTE** ppCr = pDest->m_matZ.data.ptr;
		for (i = 0; i < m_nRows; i ++)
		for (j = 0; j < m_nCols; j ++)
		{
			pprY[i][j] = (299 * pprR[i][j] + 587 * pprG[i][j] + 114 * pprB[i][j]) / 1000;
			ppCb[i][j] = (-169 * pprR[i][j] - 331 * pprG[i][j] + 500 * pprB[i][j]) / 1000 + 128;
			ppCr[i][j] = (500 * pprR[i][j] - 419 * pprG[i][j] - 81 * pprB[i][j]) / 1000 + 128;
		}
		return pDest;
	}
	else
	{
		for (i = 0; i < m_nRows; i ++)
		for (j = 0; j < m_nCols; j ++)
		{
			BYTE rR = pprR[i][j], rG = pprG[i][j], rB = pprB[i][j];
			pprR[i][j] = (299 * rR + 587 * rG + 114 * rB) / 1000;
			pprG[i][j] = (-169 * rR - 331 * rG + 500 * rB) / 1000 + 128;
			pprB[i][j] = (500 * rR - 419 * rG - 81 * rB) / 1000 + 128;
		}
		return this;
	}
}

Mat* CoImage::GetGrayMat ()
{
	Mat* pMat;
	pMat = new Mat();
	if (m_nChannel == enGRAY_MODE)
	{
		MatOp::CopyMat (pMat, &m_matX, MAT_Tbyte);
	}
	else if (m_nChannel == enCOLOR_MODE)
	{
		int nH = m_matX.Rows(), nW = m_matX.Cols();
		pMat->Create (nH, nW, MAT_Tbyte);
		BYTE* pbData = pMat->data.ptr[0];
		BYTE* pbX = m_matX.data.ptr[0];
		BYTE* pbY = m_matY.data.ptr[0];
		BYTE* pbZ = m_matZ.data.ptr[0];
		for (int i = 0; i < nH * nW; i ++)
		{
			pbData[i] = (BYTE)((299 * pbX[i] + 587 * pbY[i] + 114 * pbZ[i]) / 1000);
		}
	}
	return pMat;
}

void CoImage::Gray (Mat* pmGray)
{
	if (GetType() != MAT_Tbyte)
		return;

	Mat* pmNewGray = NULL;
	int nH = m_matX.Rows(), nW = m_matX.Cols();

	if(pmGray)
	{
		assert (nH == pmGray->Rows());
		assert (nW == pmGray->Cols());
		pmNewGray = pmGray;
	}
	else
	{
		if(m_matG.data.ptr == NULL)
			m_matG.Create(nH, nW, MAT_Tbyte);
		pmNewGray = &m_matG;
	}

	if (m_nChannel == enGRAY_MODE)
	{
		memcpy (pmNewGray->data.ptr[0], m_matX.data.ptr[0], nH * nW);
	}
	else if (m_nChannel == enCOLOR_MODE)
	{
		BYTE* pbData = pmNewGray->data.ptr[0];
		BYTE* pbX = m_matX.data.ptr[0];
		BYTE* pbY = m_matY.data.ptr[0];
		BYTE* pbZ = m_matZ.data.ptr[0];
		for (int i = 0; i < nH * nW; i ++)
		{
			pbData[i] = (BYTE)((299 * pbX[i] + 587 * pbY[i] + 114 * pbZ[i]) / 1000);
		}
	}
}

BOOL CoImage::CheckGrayMode()
{
	int nSize = m_nRows * m_nCols;
	int nR_G = 0, nG_B = 0;
	
	BYTE* pbR = m_matX.data.ptr[0];
	BYTE* pbG = m_matY.data.ptr[0];
	BYTE* pbB = m_matZ.data.ptr[0];
	for (int i = 0; i < nSize; i ++)
	{
		nR_G += abs(pbR[i] - pbG[i]);
		nG_B += abs(pbG[i] - pbB[i]);
	}
	if (nR_G / nSize < 5 && nG_B / nSize < 5)
		return 1;
	
	return 0;
}

void CoImage::DrawRect (Rect* pRect, COLORREF color, const float rOpacity/*=1*/)
{
	unsigned char bR=GetRValue(color);
	unsigned char bG=GetGValue(color);
	unsigned char bB=GetBValue(color);
	unsigned char** ppbR = m_matX.data.ptr;
	unsigned char** ppbG = m_matY.data.ptr;
	unsigned char** ppbB = m_matZ.data.ptr;
	if (!IsInterior(pRect->x, pRect->y) || 
		!IsInterior(pRect->x+pRect->width, pRect->y+pRect->height) )
		return;
	
	const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
	
	if (rNopacity >= 1)
	{
		for (int iX = pRect->x; iX < pRect->x + pRect->width; iX ++)
		{
			ppbR[pRect->y][iX] = bR;
			ppbG[pRect->y][iX] = bG;
			ppbB[pRect->y][iX] = bB;
			ppbR[pRect->y + pRect->height - 1][iX] = bR;
			ppbG[pRect->y + pRect->height - 1][iX] = bG;
			ppbB[pRect->y + pRect->height - 1][iX] = bB;
		}
		for (int iY = pRect->y; iY < pRect->y + pRect->height; iY ++)
		{
			ppbR[iY][pRect->x] = bR;
			ppbG[iY][pRect->x] = bG;
			ppbB[iY][pRect->x] = bB;
			ppbR[iY][pRect->x + pRect->width - 1] = bR;
			ppbG[iY][pRect->x + pRect->width - 1] = bG;
			ppbB[iY][pRect->x + pRect->width - 1] = bB;
		}
	}
	else
	{
		for (int iX = pRect->x; iX < pRect->x + pRect->width; iX ++)
		{
			ppbR[pRect->y][iX] = (BYTE)(ppbR[pRect->y][iX] * rCopacity + bR * rNopacity);
			ppbG[pRect->y][iX] = (BYTE)(ppbG[pRect->y][iX] * rCopacity + bG * rNopacity);
			ppbB[pRect->y][iX] = (BYTE)(ppbB[pRect->y][iX] * rCopacity + bB * rNopacity);
			ppbR[pRect->y + pRect->height - 1][iX] = (BYTE)(ppbR[pRect->y + pRect->height - 1][iX] * rCopacity + bR * rNopacity);
			ppbG[pRect->y + pRect->height - 1][iX] = (BYTE)(ppbG[pRect->y + pRect->height - 1][iX] * rCopacity + bG * rNopacity);
			ppbB[pRect->y + pRect->height - 1][iX] = (BYTE)(ppbB[pRect->y + pRect->height - 1][iX] * rCopacity + bB * rNopacity);
		}
		for (int iY = pRect->y; iY < pRect->y + pRect->height; iY ++)
		{
			ppbR[iY][pRect->x] = (BYTE)(ppbR[iY][pRect->x] * rCopacity + bR * rNopacity);
			ppbG[iY][pRect->x] = (BYTE)(ppbG[iY][pRect->x] * rCopacity + bG * rNopacity);
			ppbB[iY][pRect->x] = (BYTE)(ppbB[iY][pRect->x] * rCopacity + bB * rNopacity);
			ppbR[iY][pRect->x + pRect->width - 1] = (BYTE)(ppbR[iY][pRect->x + pRect->width - 1] * rCopacity + bR * rNopacity);
			ppbG[iY][pRect->x + pRect->width - 1] = (BYTE)(ppbG[iY][pRect->x + pRect->width - 1] * rCopacity + bG * rNopacity);
			ppbB[iY][pRect->x + pRect->width - 1] = (BYTE)(ppbB[iY][pRect->x + pRect->width - 1] * rCopacity + bB * rNopacity);
		}
	}
}

void	CoImage::DrawEllipse (int nLeft, int nTop, int nRight, int nBottom, COLORREF color, const float rOpacity/*=1*/)
{
	Rect	rect = Rect(nLeft, nTop, nRight - nLeft + 1, nBottom - nTop + 1);
	DrawEllipse (&rect, color, rOpacity);
}

void	CoImage::DrawEllipse (Rect* pRect, COLORREF color, const float rOpacity/*=1*/)
{
	unsigned char bR=GetRValue(color);
	unsigned char bG=GetGValue(color);
	unsigned char bB=GetBValue(color);
	int nXrad = pRect->width / 2;
	int nYrad = pRect->height / 2;
	int nXradSq = nXrad * nXrad;
	int nYradSq = nYrad * nYrad;
	int nXcen = pRect->x + nXrad;
	int nYcen = pRect->y + nYrad;
	int nXradBkPt = (int) (nXradSq / sqrt((double) nXradSq + nYradSq));
	int nYradBkPt = (int) (nYradSq / sqrt((double) nXradSq + nYradSq));
	int nX, nY;			// nX & nY are in image coordinates
	if (nXrad<=0 || nYrad<=0)
		return;
	
	// count # of iterations in loops below
	int nPerimLength = 4 * nXradBkPt + 4 * nYradBkPt + 4; 
	int* pnTemp = (int*)malloc(sizeof(int) * nPerimLength * 2);
	int* pnPerim = pnTemp;
	*(pnPerim)++ = nXcen;  // perimeter starts at top and proceeds clockwise
	*(pnPerim)++ = nYcen - nYrad;
	
	// from 0 to 45 degrees, measured cw from top
	for (nX = 1; nX <= nXradBkPt ; nX++)  
	{
		nY = (int) (-nYrad * sqrt(1 - ((double) nX / nXrad)*((double) nX / nXrad)));
		*(pnPerim)++ = nXcen + nX;
		*(pnPerim)++ = nYcen + nY;
	}
	
	// from 45 to 135 degrees (including right axis)
	for (nY = - nYradBkPt ; nY <= nYradBkPt ; nY++)  {
		nX = (int) (nXrad * sqrt(1 - ((double) nY / nYrad)*((double) nY / nYrad)));
		*(pnPerim)++ = nXcen + nX;
		*(pnPerim)++ = nYcen + nY;
	}
	// from 135 to 225 degrees (including down axis)
	for (nX = nXradBkPt  ; nX >= - nXradBkPt ; nX--)  {
		nY = (int) (nYrad * sqrt(1 - ((double) nX / nXrad)*((double) nX / nXrad)));
		*(pnPerim)++ = nXcen + nX;
		*(pnPerim)++ = nYcen + nY;
	}
	// from 225 to 315 degrees (including left axis)
	for (nY = nYradBkPt ; nY >= - nYradBkPt ; nY--)  {
		nX = (int) (-nXrad * sqrt(1 - ((double) nY / nYrad)*((double) nY / nYrad)));
		*(pnPerim)++ = nXcen + nX;
		*(pnPerim)++ = nYcen + nY;
	}
	// from 315 to 360 degrees
	for (nX = - nXradBkPt ; nX < 0 ; nX++)  {
		nY = (int) (-nYrad * sqrt(1 - ((double) nX / nXrad)*((double) nX / nXrad)));
		*(pnPerim)++ = nXcen + nX;
		*(pnPerim)++ = nYcen + nY;
	}
	unsigned char** ppbR = m_matX.data.ptr;
	unsigned char** ppbG = m_matY.data.ptr;
	unsigned char** ppbB = m_matZ.data.ptr;
	
	const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
	
	if (rNopacity >= 1)
	{
		for (int ii = 0; ii < nPerimLength; ii ++)
		{
			nX = pnTemp[ii*2];
			nY = pnTemp[ii*2+1];
			ppbR[nY][nX] = bR;
			ppbG[nY][nX] = bG;
			ppbB[nY][nX] = bB;
		}
	}
	else
	{
		for (int ii = 0; ii < nPerimLength; ii ++)
		{
			nX = pnTemp[ii*2];
			nY = pnTemp[ii*2+1];
			ppbR[nY][nX] = (BYTE)(ppbR[nY][nX] * rCopacity + bR * rNopacity);
			ppbG[nY][nX] = (BYTE)(ppbG[nY][nX] * rCopacity + bG * rNopacity);
			ppbB[nY][nX] = (BYTE)(ppbB[nY][nX] * rCopacity + bB * rNopacity);
		}
	}
	free (pnTemp);
}

void	CoImage::DrawEllipse (const RotatedRect& rect, COLORREF color, const float rOpacity/*=1*/)
{
	assert (false);
}

void	CoImage::DrawPixel (const SPoint& point, COLORREF color, const float rOpacity/*=1*/)
{
	DrawPixel (point.x, point.y, color, rOpacity);
}

void	CoImage::DrawPixel (int nX, int nY, COLORREF color, const float rOpacity/*=1*/)
{
	if (IsInterior(nX, nY))
	{
		const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
		
		if (rNopacity >= 1)
		{
			m_matX.data.ptr[nY][nX] = GetRValue(color);
			m_matY.data.ptr[nY][nX] = GetGValue(color);
			m_matZ.data.ptr[nY][nX] = GetBValue(color);
		}
		else
		{
			m_matX.data.ptr[nY][nX] = (BYTE)(m_matX.data.ptr[nY][nX] * rCopacity + GetRValue(color) * rNopacity);
			m_matY.data.ptr[nY][nX] = (BYTE)(m_matY.data.ptr[nY][nX] * rCopacity + GetGValue(color) * rNopacity);
			m_matZ.data.ptr[nY][nX] = (BYTE)(m_matZ.data.ptr[nY][nX] * rCopacity + GetBValue(color) * rNopacity);
		}
	}
}

void	CoImage::DrawLine (const SPoint& pt1, const SPoint& pt2, COLORREF color, const float rOpacity/*=1*/)
{
	DrawLine(pt1.x, pt1.y, pt2.x, pt2.y, color, rOpacity);
}

void	CoImage::DrawLine (int nX0, int nY0, int nX1, int nY1, COLORREF color, const float rOpacity/*=1*/)
{
	unsigned char bR=GetRValue(color);
	unsigned char bG=GetGValue(color);
	unsigned char bB=GetBValue(color);
	int* pnX;
	int* pnY;
	int nNum;
	Line_pt (nX0, nY0, nX1, nY1, pnX, pnY, nNum);
	unsigned char** ppbR = m_matX.data.ptr;
	unsigned char** ppbG = m_matY.data.ptr;
	unsigned char** ppbB = m_matZ.data.ptr;
	const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
	
	if (rNopacity >= 1)
	{
		for (int ii = 0; ii < nNum; ii ++)
		{
			if (IsInterior(pnX[ii], pnY[ii]))
			{
				ppbR[pnY[ii]][pnX[ii]] = bR;
				ppbG[pnY[ii]][pnX[ii]] = bG;
				ppbB[pnY[ii]][pnX[ii]] = bB;
			}
		}
	}
	else
	{
		for (int ii = 0; ii < nNum; ii ++)
		{
			if (IsInterior(pnX[ii], pnY[ii]))
			{
				ppbR[pnY[ii]][pnX[ii]] = (BYTE)(ppbR[pnY[ii]][pnX[ii]] * rCopacity + bR * rNopacity);
				ppbG[pnY[ii]][pnX[ii]] = (BYTE)(ppbG[pnY[ii]][pnX[ii]] * rCopacity + bG * rNopacity);
				ppbB[pnY[ii]][pnX[ii]] = (BYTE)(ppbB[pnY[ii]][pnX[ii]] * rCopacity + bB * rNopacity);
			}
		}
	}
	delete []pnX;
	delete []pnY;
}

void CoImage::DrawCross (int nX, int nY, int nSide, COLORREF color, const float rOpacity/*=1*/)
{
	if (IsInterior(nX, nY))
	{
		DrawLine (nX-nSide, nY, nX+nSide+1, nY, color, rOpacity);
		DrawLine (nX, nY-nSide, nX, nY+nSide+1, color, rOpacity);
	}
}
void CoImage::DrawCross (const SPoint& pt, int nSide, COLORREF color, const float rOpacity/*=1*/)
{
	DrawCross(pt.x, pt.y, nSide, color, rOpacity);
}

void CoImage::Line_pt(int nX0, int nY0, 
			 int nX1, int nY1, 
			 int* &pnX, int* &pnY, //ouput points
			 int &nNum)//number of the points
{
	int i;
	int steep = 1;
	int sx, sy;  /* step positive or negative (1 or -1) */
	int dx, dy;  /* delta (difference in X and Y between points) */
	int e;
	int alloc_num = (int)(sqrt((double)((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0))) + 3);
	pnX = new int[alloc_num];
	pnY = new int[alloc_num];
	nNum = 0;
	/*
	* optimize for vertical and horizontal lines here
	*/       
	dx = abs(nX1 - nX0);
	sx = ((nX1 - nX0) > 0) ? 1 : -1;
	dy = abs(nY1 - nY0);
	sy = ((nY1 - nY0) > 0) ? 1 : -1;
	if (dy > dx)
	{
		steep = 0;
		SWAP_func(nX0, nY0);
		SWAP_func(dx, dy);
		SWAP_func(sx, sy);
	}
	e = (dy << 1) - dx;
	for (i = 0; i < dx; i++) 
	{
		if (steep) 
		{
			pnX[nNum] = nX0; pnY[nNum] = nY0;
			nNum++;
		}
		else
		{
			pnX[nNum] = nY0; pnY[nNum] = nX0;
			nNum++;
		}
		while (e >= 0)
		{
			nY0 += sy;
			e -= (dx << 1);
		}
		nX0 += sx;
		e += (dy << 1);
	}
	
}

void CoImage::DrawImage (const CoImage& img, int nX /*= 0*/, int nY /*= 0*/, const float rOpacity/*=1*/)
{
	unsigned char** ppbR = m_matX.data.ptr;
	unsigned char** ppbG = m_matY.data.ptr;
	unsigned char** ppbB = m_matZ.data.ptr;
	unsigned char** ppbR1 = img.m_matX.data.ptr;
	unsigned char** ppbG1 = img.m_matY.data.ptr;
	unsigned char** ppbB1 = img.m_matZ.data.ptr;

	const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
	
	if (rNopacity >= 1)
	{
		for (int iX = nX, i = 0; iX < MIN(m_nCols, nX + img.m_nCols); iX ++, i ++)
		for (int iY = nY, j = 0; iY < MIN(m_nRows, nY + img.m_nRows); iY ++, j ++)
		{
			ppbR[iY][iX] = ppbR1[j][i];
			ppbG[iY][iX] = ppbG1[j][i];
			ppbB[iY][iX] = ppbB1[j][i];
		}
	}
	else
	{
		for (int iX = nX, i = 0; iX < MIN(m_nCols, nX + img.m_nCols); iX ++, i ++)
			for (int iY = nY, j = 0; iY < MIN(m_nRows, nY + img.m_nRows); iY ++, j ++)
			{
				ppbR[iY][iX] = (BYTE)(ppbR[iY][iX] * rCopacity + ppbR1[j][i] * rNopacity);
				ppbG[iY][iX] = (BYTE)(ppbG[iY][iX] * rCopacity + ppbG1[j][i] * rNopacity);
				ppbB[iY][iX] = (BYTE)(ppbB[iY][iX] * rCopacity + ppbB1[j][i] * rNopacity);
			}
	}
}

bool CoImage::EncodeSafeCheck(FILE* pFile)
{
	if (pFile == NULL) 
	{
		printf ("%s", CVLIB_IMG_ERR_NOFILE);
		return true;
	}
	
	if (!IsValid())
	{
		printf ("%s", CVLIB_IMG_ERR_NOIMAGE);
		return true;
	}
	return false;
}

bool CoImage::Load (const char* szFilename, DWORD nImageType/* = 0*/)
{
	bool fOK = false;
	if (nImageType > 0 && nImageType < CVLIB_IMG_MAX_FORMATS)
	{
		FILE* pFile = fopen(szFilename, "rb");
		if (pFile == NULL)
			return false;
		fOK = Decode(pFile, nImageType);
		fclose(pFile);
		if (fOK)
		{
			SetName(szFilename);
			return fOK;
		}
	}
	FILE* pFile = fopen(szFilename, "rb");
	if (pFile == NULL)
		return false;
	fOK = Decode(pFile, CVLIB_IMG_FORMAT_UNKNOWN);
	fclose (pFile);
	if (fOK)
		SetName(szFilename);

	return fOK;
}

bool CoImage::Save (const char* szFilename, DWORD nImageType)
{
	FILE* pFile = fopen (szFilename, "wb");
	if (pFile == NULL)
		return false;
	bool fOK = Encode (pFile, nImageType);
	fclose (pFile);
	if (fOK)
		SetName(szFilename);
	return fOK;
}

bool CoImage::Decode (FILE* pFile, DWORD nImageType)
{
	if (nImageType == CVLIB_IMG_FORMAT_UNKNOWN)
	{
		long pos = ftell(pFile);
#if CVLIB_IMG_SUPPORT_BMP
		{ CoImageBMP newImage; if (newImage.Decode (pFile)) { Create (newImage, TRUE); return true; } else fseek (pFile, pos, SEEK_SET); }
#endif

#ifndef CVLIB_IMG_NOCODEC

#if CVLIB_IMG_SUPPORT_JPG
		{ CoImageJPG newImage; if (newImage.Decode (pFile)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_PNG
		{ CoImagePNG newImage; if (newImage.Decode (pFile)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_TIF
		{ CoImageTIF newImage; if (newImage.Decode (pFile)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_JP2
		{ CoImageJAS newImage; if (newImage.Decode (pFile, CVLIB_IMG_FORMAT_JP2)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_JPC
		{ CoImageJAS newImage; if (newImage.Decode (pFile, CVLIB_IMG_FORMAT_JPC)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_PGX
		{ CoImageJAS newImage; if (newImage.Decode (pFile, CVLIB_IMG_FORMAT_PGX)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_PNM
		{ CoImageJAS newImage; if (newImage.Decode (pFile, CVLIB_IMG_FORMAT_PNM)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#if CVLIB_IMG_SUPPORT_RAS
		{ CoImageJAS newImage; if (newImage.Decode (pFile, CVLIB_IMG_FORMAT_RAS)) { Create (newImage, TRUE); return true; } else  fseek (pFile, pos, SEEK_SET); }
#endif
#endif //CVLIB_IMG_NOCODEC
	}

#if CVLIB_IMG_SUPPORT_BMP
	if (nImageType == CVLIB_IMG_FORMAT_BMP)
	{
		CoImageBMP newImage;
		if (newImage.Decode (pFile))
		{ 
			Create (newImage, TRUE); return true;
		}
		else
			return false;
	}
#endif

#ifndef CVLIB_IMG_NOCODEC

#if CVLIB_IMG_SUPPORT_JPG
	if (nImageType == CVLIB_IMG_FORMAT_JPG)
	{
		CoImageJPG newImage;
		if (newImage.Decode (pFile))
		{ 
			Create (newImage, TRUE); return true;
		}
		else
			return false;
	}
#endif
#if CVLIB_IMG_SUPPORT_PNG
	if (nImageType == CVLIB_IMG_FORMAT_PNG)
	{
		CoImagePNG newImage;
		if (newImage.Decode (pFile))
		{ 
			Create (newImage, TRUE); return true;
		}
		else
			return false;
	}
#endif
#if CVLIB_IMG_SUPPORT_TIF
	if (nImageType == CVLIB_IMG_FORMAT_TIF)
	{
		CoImageTIF newImage;
		if (newImage.Decode (pFile))
		{ 
			Create (newImage, TRUE); return true;
		}
		else
			return false;
	}
#endif
#if CVLIB_IMG_SUPPORT_JAS
	if (nImageType == CVLIB_IMG_FORMAT_JP2 ||
		nImageType == CVLIB_IMG_FORMAT_JPC ||
		nImageType == CVLIB_IMG_FORMAT_PGX ||
		nImageType == CVLIB_IMG_FORMAT_PNM ||
		nImageType == CVLIB_IMG_FORMAT_RAS )
	{
		CoImageJAS newImage;
		if (newImage.Decode (pFile, nImageType))
		{ 
			Create (newImage, TRUE); return true;
		}
		else
			return false;
	}
#endif

#endif //CVLIB_IMG_NOCODEC

	return false;
}

bool CoImage::Encode (FILE* pFile, DWORD nImageType) const
{
#if CVLIB_IMG_SUPPORT_BMP
	if (nImageType == CVLIB_IMG_FORMAT_BMP)
	{
		CoImageBMP newImage;
		newImage.Create (*this, TRUE);
		if (newImage.Encode (pFile))
			return true;
		else
			return false;
	}
#endif

#ifndef CVLIB_IMG_NOCODEC

#if CVLIB_IMG_SUPPORT_JPG
	if (nImageType == CVLIB_IMG_FORMAT_JPG)
	{
		CoImageJPG newImage;
		newImage.Create (*this, TRUE);
		if (newImage.Encode (pFile))
			return true;
		else
			return false;
	}
#endif
#if CVLIB_IMG_SUPPORT_PNG
	if (nImageType == CVLIB_IMG_FORMAT_PNG)
	{
		CoImagePNG newImage;
		newImage.Create (*this, TRUE);
		if (newImage.Encode (pFile))
			return true;
		else
			return false;
	}
#endif
#if CVLIB_IMG_SUPPORT_TIF
	if (nImageType == CVLIB_IMG_FORMAT_TIF)
	{
		CoImageTIF newImage;
		newImage.Create (*this, TRUE);
		if (newImage.Encode (pFile))
			return true;
		else
			return false;
	}
#endif	
#if CVLIB_IMG_SUPPORT_JAS
	if (nImageType == CVLIB_IMG_FORMAT_JP2 ||
		nImageType == CVLIB_IMG_FORMAT_JPC ||
		nImageType == CVLIB_IMG_FORMAT_PGX ||
		nImageType == CVLIB_IMG_FORMAT_PNM ||
		nImageType == CVLIB_IMG_FORMAT_RAS )
	{
		CoImageJAS newImage;
		newImage.Create (*this, TRUE);
		if (newImage.Encode (pFile, nImageType))
			return true;
		else
			return false;
	}
#endif	
	
#endif //CVLIB_IMG_NOCODEC

	return false;
}

long CoImage::GetXDPI() const
{
	return m_Info.xDPI;
}

long CoImage::GetYDPI() const
{
	return m_Info.yDPI;
}

void CoImage::SetXDPI(long dpi)
{
	if (dpi<=0) dpi=96;
	m_Info.xDPI = dpi;
	m_Head.biXPelsPerMeter = (long) floor(dpi * 10000.0 / 254.0 + 0.5);
}

void CoImage::SetYDPI(long dpi)
{
	if (dpi<=0) dpi=96;
	m_Info.yDPI = dpi;
	m_Head.biYPelsPerMeter = (long) floor(dpi * 10000.0 / 254.0 + 0.5);
}

BYTE CoImage::GetJpegQuality() const
{
	return m_Info.nQuality;
}

void CoImage::SetJpegQuality(BYTE q)
{
	m_Info.nQuality = q;
}

void CoImage::InitBMPHeader(int nW, int nH)
{
	memset (&m_Head, 0, sizeof(m_Head));

	m_Head.biSize = sizeof(BITMAPINFOHEADER1);
	m_Head.biCompression = BI_RGB;
	m_Head.biWidth = nW;
	m_Head.biHeight = nH;
	m_Head.biBitCount = 24;
	m_Head.biPlanes = 1;
	int wBpp = 24;
	m_Info.dwEffWidth = ((((wBpp * m_nCols) + 31) / 32) * 4);
	m_Head.biSizeImage = m_Info.dwEffWidth * nH;
	SetXDPI (96);
	SetYDPI (96);

	int nSize = GetPaletteSize();
	if (nSize)
		m_pPalette = malloc(nSize);
}

void CoImage::Startup(DWORD imagetype)
{
	memset(&m_Head,0,sizeof(BITMAPINFOHEADER1));
	memset(&m_Info,0,sizeof(COIMAGEINFO));
	//init default attributes
    m_Info.dwType = imagetype;
	m_Info.nQuality = 90;
	m_Info.nAlphaMax = 255;
	m_Info.nBkgndIndex = -1;
	m_Info.bEnabled = true;
	SetXDPI(96);
	SetYDPI(96);
	m_pPalette = NULL;
	if (m_ppBands)
		delete []m_ppBands;
	m_ppBands = NULL;
}

/**
* \sa SetCodecOption
*/
DWORD CoImage::GetCodecOption(DWORD imagetype) const
{
	if (imagetype<CVLIB_IMG_MAX_FORMATS)
	{
		if (imagetype==0)
			imagetype = m_Info.dwType;
		return m_Info.dwCodecOpt[imagetype];
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
* Encode option for GIF, TIF and JPG.
* - GIF : 0 = LZW (default), 1 = none, 2 = RLE.
* - TIF : 0 = automatic (default), or a valid compression code as defined in "tiff.h" (COMPRESSION_NONE = 1, COMPRESSION_CCITTRLE = 2, ...)
* - JPG : valid values stored in enum CODEC_OPTION ( ENCODE_BASELINE = 0x01, ENCODE_PROGRESSIVE = 0x10, ...)
*
* \return true if everything is ok
*/
bool CoImage::SetCodecOption(DWORD opt, DWORD imagetype)
{
	if (imagetype<CVLIB_IMG_MAX_FORMATS)
	{
		if (imagetype==0)
			imagetype = m_Info.dwType;
		m_Info.dwCodecOpt[imagetype] = opt;
		return true;
	}
	return false;
}

/**
* Returns the color of the specified index.
* \param i = palette index
* \param r, g, b = output color channels
*/
bool CoImage::GetPaletteColor(BYTE i, BYTE* r, BYTE* g, BYTE* b) const
{
	RGBQUAD1* ppal=GetPalette();
	if (ppal) 
	{
		*r = ppal[i].rgbRed;
		*g = ppal[i].rgbGreen;
		*b = ppal[i].rgbBlue; 
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void CoImage::SetPalette(DWORD n, BYTE *r, BYTE *g, BYTE *b)
{
	if ((!r)||(m_pPalette==NULL)||(m_Head.biClrUsed==0)) return;
	if (!g) g = r;
	if (!b) b = g;
	RGBQUAD1* ppal=GetPalette();
	DWORD m=MIN(n,m_Head.biClrUsed);
	for (DWORD i=0; i<m;i++){
		ppal[i].rgbRed=r[i];
		ppal[i].rgbGreen=g[i];
		ppal[i].rgbBlue=b[i];
	}
	m_Info.last_c_isvalid = false;
}
////////////////////////////////////////////////////////////////////////////////
void CoImage::SetPalette(rgb_color *rgb,DWORD nColors)
{
	if ((!rgb)||(GetPalette()==NULL)||(m_Head.biClrUsed==0)) return;
	RGBQUAD1* ppal=GetPalette();
	DWORD m=MIN(nColors,m_Head.biClrUsed);
	for (DWORD i=0; i<m;i++){
		ppal[i].rgbRed=rgb[i].r;
		ppal[i].rgbGreen=rgb[i].g;
		ppal[i].rgbBlue=rgb[i].b;
	}
	m_Info.last_c_isvalid = false;
}
////////////////////////////////////////////////////////////////////////////////
void CoImage::SetPalette(RGBQUAD1* pPal,DWORD nColors)
{
	if ((pPal==NULL)||(GetPalette()==NULL)||(m_Head.biClrUsed==0)) return;
	memcpy(GetPalette(),pPal,MIN(GetPaletteSize(),nColors*sizeof(RGBQUAD1)));
	m_Info.last_c_isvalid = false;
}
////////////////////////////////////////////////////////////////////////////////
/**
* Sets (or replaces) the palette to gray scale palette.
* The function doesn't change the pixels; for standard
* gray scale conversion use GrayScale().
*/
void CoImage::SetGrayPalette()
{
	if ((GetPalette()==NULL)||(m_Head.biClrUsed==0)) return;
	RGBQUAD1* pal=GetPalette();
	for (DWORD ni=0;ni<m_Head.biClrUsed;ni++)
		pal[ni].rgbBlue=pal[ni].rgbGreen = pal[ni].rgbRed = (BYTE)(ni*(255/(m_Head.biClrUsed-1)));
}
////////////////////////////////////////////////////////////////////////////////
/**
* Colorize the palette.
* \sa Colorize
*/
void CoImage::BlendPalette(COLORREF cr,long perc) const
{
	if ((GetPalette()==NULL)||(m_Head.biClrUsed==0)) return;
	DWORD i,r,g,b;
	RGBQUAD1* pPal=GetPalette();
	r = GetRValue(cr);
	g = GetGValue(cr);
	b = GetBValue(cr);
	if (perc>100) perc=100;
	for(i=0;i<m_Head.biClrUsed;i++){
		pPal[i].rgbBlue=(BYTE)((pPal[i].rgbBlue*(100-perc)+b*perc)/100);
		pPal[i].rgbGreen =(BYTE)((pPal[i].rgbGreen*(100-perc)+g*perc)/100);
		pPal[i].rgbRed =(BYTE)((pPal[i].rgbRed*(100-perc)+r*perc)/100);
	}
}

/**
* Returns the palette index of the specified pixel.
*/

BYTE CoImage::GetPixelIndex(BYTE* pData,long x) const
{
	if ((pData==NULL)||(m_Head.biClrUsed==0))
		return 0;
	
	if (m_Head.biBitCount==8)
	{
		return pData[x];
	}
	else
	{
		BYTE pos;
		BYTE iDst= pData[x*m_Head.biBitCount >> 3];
		if (m_Head.biBitCount==4)
		{
			pos = (BYTE)(4*(1-x%2));
			iDst &= (0x0F<<pos);
			return (BYTE)(iDst >> pos);
		}
		else if (m_Head.biBitCount==1)
		{
			pos = (BYTE)(7-x%8);
			iDst &= (0x01<<pos);
			return (BYTE)(iDst >> pos);
		}
	}
	return 0;
}

BYTE CoImage::GetPixelIndex(long x,long y) const
{
	if ((GetPalette()==NULL)||(m_Head.biClrUsed==0)) return 0;
	
	if ((x<0)||(y<0)||(x>=m_Head.biWidth)||(y>=m_Head.biHeight)) 
	{
		if (m_Info.nBkgndIndex != -1)
			return (BYTE)m_Info.nBkgndIndex;
		else
			return *m_Info.pImage;
	}
	if (m_Head.biBitCount==8)
	{
		return m_Info.pImage[y*m_Info.dwEffWidth + x];
	} 
	else
	{
		BYTE pos;
		BYTE iDst= m_Info.pImage[y*m_Info.dwEffWidth + (x*m_Head.biBitCount >> 3)];
		if (m_Head.biBitCount==4)
		{
			pos = (BYTE)(4*(1-x%2));
			iDst &= (0x0F<<pos);
			return (BYTE)(iDst >> pos);
		}
		else if (m_Head.biBitCount==1)
		{
			pos = (BYTE)(7-x%8);
			iDst &= (0x01<<pos);
			return (BYTE)(iDst >> pos);
		}
	}
	return 0;
}

void CoImage::SetPixelIndex(long x,long y,BYTE i)
{
	if ((GetPalette() == NULL) || (m_Head.biClrUsed == 0) ||
		(x<0) || (y<0)||(x >= m_Head.biWidth) || (y >= m_Head.biHeight)) return ;
	
	if (m_Head.biBitCount == 8)
	{
		m_Info.pImage[y*m_Info.dwEffWidth + x]=i;
		return;
	} 
	else
	{
		BYTE pos;
		BYTE* iDst= m_Info.pImage + y*m_Info.dwEffWidth + (x*m_Head.biBitCount >> 3);
		if (m_Head.biBitCount == 4)
		{
			pos = (BYTE)(4*(1-x%2));
			*iDst &= ~(0x0F<<pos);
			*iDst |= ((i & 0x0F)<<pos);
			return;
		}
		else if (m_Head.biBitCount == 1)
		{
			pos = (BYTE)(7-x%8);
			*iDst &= ~(0x01<<pos);
			*iDst |= ((i & 0x01)<<pos);
			return;
		}
	}
}


RGBQUAD1 CoImage::GetPixelColor(long x,long y, bool fGetAlpha) const
{
	//	RGBQUAD1 rgb={0,0,0,0};
	RGBQUAD1 rgb = m_Info.nBkgndColor; //<mpwolski>
	if ((!m_matX.IsValid())||(x<0)||(y<0)||
		(x>=m_Head.biWidth)||(y>=m_Head.biHeight))
	{
		if (m_Info.nBkgndIndex != -1)
		{
			if (m_Head.biBitCount<24) 
				return GetPaletteColor((BYTE)m_Info.nBkgndIndex);
			else return m_Info.nBkgndColor;
		} 
		else if (GetPalette()) 
			return GetPixelColor(0,0);
		return rgb;
	}
	
	if (m_Head.biClrUsed)
	{
		rgb = GetPaletteColor(GetPixelIndex(x,y));
	}
	else
	{
		rgb.rgbRed = m_matX.data.ptr[y][x];
		rgb.rgbGreen= m_matY.data.ptr[y][x];
		rgb.rgbBlue  = m_matZ.data.ptr[y][x];
	}
#if CVLIB_IMG_SUPPORT_ALPHA
	if (m_matA.IsValid() && fGetAlpha) rgb.rgbReserved = m_matA.data.ptr[y][x];
#else
	rgb.rgbReserved = 0;
#endif //CVLIB_IMG_SUPPORT_ALPHA
	return rgb;
}

void CoImage::SetPixelColor(long x,long y,COLORREF cr)
{
	SetPixelColor(x,y,RGBtoRGBQUAD(cr));
}

void CoImage::SetPixelColor(long x,long y,float* color)
{
	for (int i = 0; i<3; i ++)
		GetBand()[i]->data.ptr[y][x] = (BYTE)color[i];
}

void CoImage::SetPixelColor(long x,long y,RGBQUAD1 c, bool bSetAlpha)
{
	if ((!m_matX.IsValid()) || (x<0) || (y<0) ||
		(x>=m_Head.biWidth) || (y>=m_Head.biHeight))
		return;
	if (m_Head.biClrUsed)
		SetPixelIndex(x,y,GetNearestIndex(c));
	else 
	{
		m_matX.data.ptr[y][x] = c.rgbRed;
		m_matY.data.ptr[y][x] = c.rgbGreen;
		m_matZ.data.ptr[y][x] = c.rgbBlue;
#if CVLIB_IMG_SUPPORT_ALPHA
		if (bSetAlpha) m_matA.data.ptr[y][x] = c.rgbReserved;
#endif //CVLIB_IMG_SUPPORT_ALPHA
	}
}

RGBQUAD1 CoImage::RGBtoRGBQUAD(COLORREF cr) const
{
	RGBQUAD1 c;
	c.rgbRed = GetRValue(cr);	/* get R, G, and B out of DWORD */
	c.rgbGreen = GetGValue(cr);
	c.rgbBlue = GetBValue(cr);
	c.rgbReserved=0;
	return c;
}

/**
* returns the palette dimension in byte
*/
DWORD CoImage::GetPaletteSize() const
{
	return (m_Head.biClrUsed * sizeof(RGBQUAD1));
}

RGBQUAD1* CoImage::GetPalette() const
{
	return (RGBQUAD1*)m_pPalette;
}

void CoImage::SetPaletteColor(BYTE idx, BYTE r, BYTE g, BYTE b, BYTE alpha)
{
	if (GetPalette() && (m_Head.biClrUsed))
	{
		BYTE* iDst = (BYTE*)(m_pPalette);
		if (idx < m_Head.biClrUsed)
		{
			long ldx= idx * sizeof(RGBQUAD1);
			iDst[ldx++] = (BYTE) b;
			iDst[ldx++] = (BYTE) g;
			iDst[ldx++] = (BYTE) r;
			iDst[ldx] = (BYTE) alpha;
			m_Info.last_c_isvalid = false;
		}
	}
}

void CoImage::SetPaletteColor(BYTE idx, RGBQUAD1 c)
{
	if (GetPalette() && (m_Head.biClrUsed))
	{
		BYTE* iDst = (BYTE*)(m_pPalette);
		if (idx < m_Head.biClrUsed)
		{
			long ldx=idx * sizeof(RGBQUAD1);
			iDst[ldx++] = (BYTE) c.rgbBlue;
			iDst[ldx++] = (BYTE) c.rgbGreen;
			iDst[ldx++] = (BYTE) c.rgbRed;
			iDst[ldx] = (BYTE) c.rgbReserved;
			m_Info.last_c_isvalid = false;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void CoImage::SetPaletteColor(BYTE idx, COLORREF cr)
{
	if (GetPalette() && (m_Head.biClrUsed))
	{
		BYTE* iDst = (BYTE*)(m_pPalette);
		if (idx < m_Head.biClrUsed)
		{
			long ldx=idx*sizeof(RGBQUAD1);
			iDst[ldx++] = (BYTE) GetBValue(cr);
			iDst[ldx++] = (BYTE) GetGValue(cr);
			iDst[ldx++] = (BYTE) GetRValue(cr);
			iDst[ldx] = (BYTE) 0;
			m_Info.last_c_isvalid = false;
		}
	}
}

RGBQUAD1 CoImage::GetPaletteColor(BYTE idx) const
{
	RGBQUAD1 rgb = {0,0,0,0};
	if ( GetPalette() && (m_Head.biClrUsed) )
	{
		BYTE* iDst = (BYTE*)m_pPalette;
		if ( idx < m_Head.biClrUsed )
		{
			long ldx = idx * sizeof(RGBQUAD1);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen = iDst[ldx++];
			rgb.rgbRed = iDst[ldx++];
			rgb.rgbReserved = iDst[ldx];
		}
	}
	return rgb;
}

BYTE CoImage::GetNearestIndex(RGBQUAD1 c)
{
	if ((GetPalette() == NULL)||(m_Head.biClrUsed==0)) return 0;
	
	// <RJ> check matching with the previous result
	if (m_Info.last_c_isvalid && (*(long*)&m_Info.last_c == *(long*)&c)) return m_Info.last_c_index;
	m_Info.last_c = c;
	m_Info.last_c_isvalid = true;
	
	BYTE* iDst = (BYTE*)(GetPalette()) + sizeof(BITMAPINFOHEADER1);
	long distance=200000;
	int i,j = 0;
	long k,l;
	int m = (int)(m_Head.biClrImportant==0 ? m_Head.biClrUsed : m_Head.biClrImportant);
	for(i=0,l=0;i<m;i++,l+=sizeof(RGBQUAD1))
	{
		k = (iDst[l]-c.rgbBlue)*(iDst[l]-c.rgbBlue)+
			(iDst[l+1]-c.rgbGreen)*(iDst[l+1]-c.rgbGreen)+
			(iDst[l+2]-c.rgbRed)*(iDst[l+2]-c.rgbRed);
		//		k = abs(iDst[l]-c.rgbBlue)+abs(iDst[l+1]-c.rgbGreen)+abs(iDst[l+2]-c.rgbRed);
		if (k==0)
		{
			j=i;
			break;
		}
		if (k<distance)
		{
			distance=k;
			j=i;
		}
	}
	m_Info.last_c_index = j;
	return (BYTE)j;
}
/**
* swaps the blue and red components (for RGB images)
* \param buffer : pointer to the pixels
* \param length : number of bytes to swap. lenght may not exceed the scan line.
*/
void CoImage::RGBtoBGR(BYTE *buffer, int length)
{
	if (buffer && (m_Head.biClrUsed==0))
	{
		BYTE temp;
		length = MIN(length,(int)m_Info.dwEffWidth);
		for (int i=0;i<length;i+=3)
		{
			temp = buffer[i]; buffer[i] = buffer[i+2]; buffer[i+2] = temp;
		}
	}
}

void CoImage::SetBpp24()
{
	int wBPP = 24;
    m_Info.dwEffWidth = ((((wBPP * m_nCols) + 31) / 32) * 4);
    m_Info.dwType = CVLIB_IMG_FORMAT_UNKNOWN;

	m_Head.biClrUsed = 0;
	int dwEffWidth = ((((wBPP * m_nCols) + 31) / 32) * 4);
    // initialize BITMAPINFOHEADER1
	m_Head.biSize = sizeof(BITMAPINFOHEADER1); //<ralphw>
    m_Head.biWidth = m_nCols;		// fill in width from parameter
    m_Head.biHeight = m_nRows;	// fill in height from parameter
    m_Head.biPlanes = 1;			// must be 1
    m_Head.biBitCount = (WORD)wBPP;		// from parameter
    m_Head.biCompression = BI_RGB;    
    m_Head.biSizeImage = dwEffWidth * m_nRows;
	// 	m_Head.biXPelsPerMeter = 0; See SetXDPI
	// 	m_Head.biYPelsPerMeter = 0; See SetYDPI
    m_Head.biClrImportant = 0;
}

void CoImage::SetBkColor(COLORREF color)
{
	memset (m_matX.data.ptr[0], GetRValue(color), m_nRows * m_nCols);
	memset (m_matY.data.ptr[0], GetGValue(color), m_nRows * m_nCols);
	memset (m_matZ.data.ptr[0], GetBValue(color), m_nRows * m_nCols);
}

bool CoImage::Flip(BOOL fVertical /*= TRUE*/)
{
	CoImage* pImageTemp = new CoImage(*this);
	if (fVertical)
	{
		for (int iH = 0; iH < m_nRows; iH ++)
		{
			memcpy(m_matX.data.ptr[iH], pImageTemp->m_matX.data.ptr[m_nRows-iH-1], m_matX.Step() * m_nCols);
			memcpy(m_matY.data.ptr[iH], pImageTemp->m_matY.data.ptr[m_nRows-iH-1], m_matY.Step() * m_nCols);
			memcpy(m_matZ.data.ptr[iH], pImageTemp->m_matZ.data.ptr[m_nRows-iH-1], m_matZ.Step() * m_nCols);
		}
	}
	else
	{
		for (int iH = 0; iH < m_nRows; iH ++)
		for (int iW = 0; iW < m_nCols; iW ++)
		{
			m_matX.data.ptr[iH][iW] = pImageTemp->m_matX.data.ptr[iH][m_nCols - iW-1];
			m_matY.data.ptr[iH][iW] = pImageTemp->m_matY.data.ptr[iH][m_nCols - iW-1];
			m_matZ.data.ptr[iH][iW] = pImageTemp->m_matZ.data.ptr[iH][m_nCols - iW-1];
		}
	}
	delete pImageTemp;

	return true;
}

bool CoImage::Mirror()
{
	CoImage* pImageTemp = new CoImage(*this);
	BYTE** ppbDst1 = m_matX.data.ptr;
	BYTE** ppbDst2 = m_matY.data.ptr;
	BYTE** ppbDst3 = m_matZ.data.ptr;
	BYTE** ppbSrc1 = pImageTemp->m_matX.data.ptr;
	BYTE** ppbSrc2 = pImageTemp->m_matY.data.ptr;
	BYTE** ppbSrc3 = pImageTemp->m_matZ.data.ptr;
	for (int iH = 0; iH < m_nRows; iH ++)
	for (int iW = 0; iW < m_nCols; iW ++)
	{
		ppbDst1[iH][iW] = ppbSrc1[iH][m_nCols-1-iW];
		ppbDst2[iH][iW] = ppbSrc2[iH][m_nCols-1-iW];
		ppbDst3[iH][iW] = ppbSrc3[iH][m_nCols-1-iW];
	}
	delete pImageTemp;
	
	return true;
}

bool CoImage::Negative()
{
	BYTE* pb1 = m_matX.data.ptr[0];
	BYTE* pb2 = m_matY.data.ptr[0];
	BYTE* pb3 = m_matZ.data.ptr[0];
	for (int i = 0; i < m_nRows * m_nCols; i ++)
	{
		pb1[i] = 255-pb1[i];
		pb2[i] = 255-pb2[i];
		pb3[i] = 255-pb3[i];
	}
	
	return true;
}

double CoImage::Mean() const
{
	return (m_matX.Mean() + m_matY.Mean() + m_matZ.Mean()) / 3.0;
}

RGBQUAD1  CoImage::GetTransColor() const
{
	return m_Info.nBkgndColor;
}

void CoImage::SetTransColor(RGBQUAD1 rgb)
{
	rgb.rgbReserved=0;
	m_Info.nBkgndColor = rgb;
}

#if CVLIB_OS==CVLIB_OS_WIN32

void CoImage::Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY,
			 int nSrcX, int nSrcY, int nSrcCX, int nSrcCY) const
{
	if (!IsValid())
		return;
	if (!m_matA.IsValid())
	{
		BYTE* pbData = GetBMPInfoHeader();
		int nPrevMode = SetStretchBltMode (*(HDC*)&hDC, HALFTONE);
		RealizePalette(*(HDC*)&hDC);
		BITMAPINFO1 bmpInfo;
		memset(&bmpInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER1));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER1);
		bmpInfo.bmiHeader.biHeight = m_nRows;
		bmpInfo.bmiHeader.biWidth = m_nCols;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 24;
		::StretchDIBits (*(HDC*)&hDC,
			nDstX, nDstY, nDstCX, nDstCY,
			nSrcX, nSrcY, nSrcCX, nSrcCY,
			pbData + sizeof(BITMAPINFOHEADER1), (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		delete []pbData;
		SetStretchBltMode (*(HDC*)&hDC, nPrevMode);
	}
	else
	{
 		//Alpha blend - Thanks to Florian Egel
		bool bAlpha = m_matA.IsValid();
 		BYTE* pAlpha = m_matA.data.ptr[0];
		bool bTransparent = m_Info.nBkgndIndex != -1;
 
 		RECT clipbox,paintbox;
		clipbox.left = nSrcX;
		clipbox.top = nSrcY;
		clipbox.right = nSrcX + nSrcCX;
		clipbox.bottom = nSrcY + nSrcCY;
		paintbox.left = nDstX;
		paintbox.top = nDstY;
		paintbox.right = nDstX + nDstCX;
		paintbox.bottom = nDstY + nDstCY;

		//pixel informations
		RGBQUAD1 c={0,0,0,0};
		RGBQUAD1 ct = GetTransColor();
		long* pc = (long*)&c;
		long* pct= (long*)&ct;
		long ci;

		long destw = nDstCX;
		long desth = nDstCY;
		//Preparing Bitmap Info
		BITMAPINFO bmInfo;
		memset(&bmInfo.bmiHeader,0,sizeof(BITMAPINFOHEADER));
		bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bmInfo.bmiHeader.biWidth=destw;
		bmInfo.bmiHeader.biHeight=desth;
		bmInfo.bmiHeader.biPlanes=1;
		bmInfo.bmiHeader.biBitCount=24;

		BYTE *pbase;	//points to the final dib
		BYTE *pdst;		//current pixel from pbase

		//get the background
		HDC TmpDC=CreateCompatibleDC((HDC)hDC);
		HBITMAP TmpBmp=CreateDIBSection((HDC)hDC,&bmInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);
		HGDIOBJ TmpObj=SelectObject(TmpDC,TmpBmp);
		BitBlt(TmpDC,0,0,destw,desth,(HDC)hDC,paintbox.left,paintbox.top,SRCCOPY);

		if (pbase)
		{
			long xx,yy,alphaoffset,ix,iy;
			BYTE a,a1/*,*psrc*/;
			long ew = ((((24 * destw) + 31) / 32) * 4);
			long ymax = paintbox.bottom;
			long xmin = paintbox.left;
			int cx = nDstCX, cy = nDstCY;
			int x = nDstX, y = nDstY;

			if (cx!=m_Head.biWidth || cy!=m_Head.biHeight)
			{
				//STRETCH
				float fx=(float)m_Head.biWidth/(float)cx;
				float fy=(float)m_Head.biHeight/(float)cy;
				float dx,dy;
				long sx,sy,sy1;
				
				for(yy=0;yy<desth;yy++)
				{
					dy = m_Head.biHeight-(ymax-yy-y)*fy;
					sy = MAX(0L,(long)floor(dy));
					sy = MIN(GetHeight()-1,sy);
					sy1=GetHeight()-1-sy;

					alphaoffset = sy*m_Head.biWidth;
					pdst = pbase + yy*ew;

					for(xx=0;xx<destw;xx++)
					{
						dx = (xx+xmin-x)*fx;
						sx = MAX(0L,(long)floor(dx));
						sx = MIN(m_nCols-1,sx);

						if (bAlpha) 
							a=pAlpha[alphaoffset+sx]; 
						else 
							a=255;
						a =(BYTE)((a*(1+m_Info.nAlphaMax))>>8);

						{
#if CVLIB_IMG_SUPPORT_INTERPOLATION
							if (bSmooth)
							{
								c = GetPixelColorInterpolated(dx - 0.5f, dy - 0.5f, CxImage::IM_BILINEAR, CxImage::OM_REPEAT);
							}
							else
#endif //CVLIB_IMG_SUPPORT_INTERPOLATION
							{
								c.rgbBlue = m_matZ.data.ptr[sy1][sx];
								c.rgbGreen= m_matY.data.ptr[sy1][sx];
								c.rgbRed  = m_matX.data.ptr[sy1][sx];
							}
						}
						if (/*(m_Head.biClrUsed && ci!=cit) ||*/ (!m_Head.biClrUsed && *pc!=*pct) || !bTransparent)
						{
							// DJT, assume many pixels are fully transparent or opaque and thus avoid multiplication
							if (a == 0) 
							{			// Transparent, retain dest 
								pdst+=3; 
							} 
							else if (a == 255) 
							{	// opaque, ignore dest 
								*pdst++= c.rgbBlue; 
								*pdst++= c.rgbGreen; 
								*pdst++= c.rgbRed; 
							}
							else 
							{				// semi transparent 
								a1=(BYTE)~a;
								*pdst++=(BYTE)((*pdst * a1 + a * c.rgbBlue)>>8); 
								*pdst++=(BYTE)((*pdst * a1 + a * c.rgbGreen)>>8); 
								*pdst++=(BYTE)((*pdst * a1 + a * c.rgbRed)>>8); 
							} 
						} 
						else 
						{
							pdst+=3;
						}
					}
				}
			} 
			else 
			{
				//NORMAL
				iy=m_Head.biHeight-ymax+y;
				for(yy=0;yy<desth;yy++,iy++)
				{
					int iy1=m_nRows-iy-1;
					alphaoffset=iy*m_Head.biWidth;
					ix=xmin-x;
					pdst=pbase+yy*ew;
					for(xx=0;xx<destw;xx++,ix++)
					{

						if (bAlpha) 
							a=pAlpha[alphaoffset+ix]; 
						else 
							a=255;
						a = (BYTE)((a*(1+m_Info.nAlphaMax))>>8);

						if (m_Head.biClrUsed)
						{
							ci = GetPixelIndex(ix,iy1);
							c = GetPaletteColor((BYTE)ci);
							if (m_Info.bAlphaPaletteEnabled)
							{
								a = (BYTE)((a*(1+c.rgbReserved))>>8);
							}
						} 
						else 
						{
							c.rgbBlue = m_matZ.data.ptr[iy1][ix];
							c.rgbGreen= m_matY.data.ptr[iy1][ix];
							c.rgbRed  = m_matX.data.ptr[iy1][ix];
						}

						//if (*pc!=*pct || !bTransparent){
						if (/*(m_Head.biClrUsed && ci!=cit) ||*/ (!m_Head.biClrUsed && *pc!=*pct) || !bTransparent)
						{
							// DJT, assume many pixels are fully transparent or opaque and thus avoid multiplication
							if (a == 0) 
							{			// Transparent, retain dest 
								pdst+=3; 
							}
							else if (a == 255) 
							{	// opaque, ignore dest 
								*pdst++= c.rgbBlue; 
								*pdst++= c.rgbGreen; 
								*pdst++= c.rgbRed; 
							} 
							else 
							{				// semi transparent 
								a1=(BYTE)~a;
								*pdst++=(BYTE)((*pdst * a1 + a * c.rgbBlue)>>8); 
								*pdst++=(BYTE)((*pdst * a1 + a * c.rgbGreen)>>8); 
								*pdst++=(BYTE)((*pdst * a1 + a * c.rgbRed)>>8); 
							} 
						} 
						else 
						{
							pdst+=3;
						}
					}
				}
			}
		}
		//paint the image & cleanup
		SetDIBitsToDevice((HDC)hDC,paintbox.left,paintbox.top,destw,desth,0,0,0,desth,pbase,&bmInfo,0);
		DeleteObject(SelectObject(TmpDC,TmpObj));
		DeleteDC(TmpDC);
	}
}
void CoImage::Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY) const
{
	if (nDstCX < 0) nDstCX = m_nCols;
	if (nDstCY < 0) nDstCY = m_nRows;
	Draw(hDC, nDstX, nDstY, nDstCX, nDstCY, 0, 0, m_nCols, m_nRows);
}
void CoImage::Draw(DWORD hDC) const
{
	Draw(hDC, 0, 0, m_nCols, m_nRows, 0, 0, m_nCols, m_nRows);
}

int	CoImage::DrawString(DWORD hdc, long x, long y, const char* szText1, COLORREF color, const char* szFont1, 
			long lSize, long lWeight, BYTE bItalic, BYTE bUnderline, const float rOpacity /*= 1*/)
{
	if (IsValid())
	{
		const TCHAR* szText = (const TCHAR*)szText1;
		const TCHAR* szFont = (const TCHAR*)szFont1;
		//get the background
		HDC TmpDC=CreateCompatibleDC(*(HDC*)&hdc);
		//choose the font
		HFONT m_Font;
		LOGFONT* m_pLF;
		m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
		_tcsncpy(m_pLF->lfFaceName,szFont,31);	// For UNICODE support
		//strncpy(m_pLF->lfFaceName,szFont,31);
		m_pLF->lfHeight=lSize;
		m_pLF->lfWeight=lWeight;
		m_pLF->lfItalic=bItalic;
		m_pLF->lfUnderline=bUnderline;
		m_Font=CreateFontIndirect(m_pLF);
		//select the font in the dc
		HFONT pOldFont=NULL;
		if (m_Font)
			pOldFont = (HFONT)SelectObject(TmpDC,m_Font);
		else
			pOldFont = (HFONT)SelectObject(TmpDC,GetStockObject(DEFAULT_GUI_FONT));
		
		//Set text color
		SetTextColor(TmpDC,RGB(255,255,255));
		::SetBkColor(TmpDC,RGB(0,0,0));
		//draw the text
		SetBkMode(TmpDC,OPAQUE);
		//Set text position;
		RECT pos = {0,0,0,0};
		//long len = (long)strlen(text);
		long len = (long)_tcslen(szText);	// For UNICODE support
		::DrawText(TmpDC,szText,len,&pos,DT_CALCRECT);
		pos.right+=pos.bottom; //for italics
		
		//Preparing Bitmap Info
		long width=pos.right;
		long height=pos.bottom;
		BITMAPINFO bmInfo;
		memset(&bmInfo.bmiHeader,0,sizeof(BITMAPINFOHEADER1));
		bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER1);
		bmInfo.bmiHeader.biWidth=width;
		bmInfo.bmiHeader.biHeight=height;
		bmInfo.bmiHeader.biPlanes=1;
		bmInfo.bmiHeader.biBitCount=24;
		BYTE *pbase; //points to the final dib
		
		HBITMAP TmpBmp=CreateDIBSection(TmpDC,&bmInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);
		HGDIOBJ TmpObj=SelectObject(TmpDC,TmpBmp);
		memset(pbase,0,height*((((24 * width) + 31) / 32) * 4));
		
		::DrawText(TmpDC,szText,len,&pos,0);
		
		CoImage itext;
		itext.CreateFromHBITMAP(*(DWORD*)&TmpBmp);
		
		for (long ix=0;ix<width;ix++)
		{
			for (long iy=0;iy<height;iy++)
			{
				if (itext.GetPixelColor(ix,iy).rgbBlue)
					DrawPixel(x+ix,y+iy,color);
//					SetPixelColor(x+ix,y+iy,color);
//					SetPixelColor(x+ix,y+iy,color,bSetAlpha);
			}
		}
		
		//cleanup
		if (pOldFont) SelectObject(TmpDC,pOldFont);
		DeleteObject(m_Font);
		free(m_pLF);
		DeleteObject(SelectObject(TmpDC,TmpObj));
		DeleteDC(TmpDC);
	}
	
	return 1;
}

int	CoImage::CreateFromHBITMAP(DWORD hBitmap, DWORD hPalette/* = 0*/)
{
	m_pPalette=NULL;
	
	Release();
	
	if (hBitmap) { 
		CoImage tempImage;
        BITMAP bm;
		// get informations about the bitmap
        GetObject(*(HBITMAP*)&hBitmap, sizeof(BITMAP), (LPSTR) &bm);
		// create the image
        if (!tempImage.Create(bm.bmHeight, bm.bmWidth, MAT_Tbyte, CoImage::enCOLOR_MODE))
			return false;
		// create a device context for the bitmap
        HDC dc = ::GetDC(NULL);
		if (!dc)
			return false;
		
		if (hPalette){
			SelectObject(dc,*(HPALETTE*)&hPalette); //the palette you should get from the user or have a stock one
			RealizePalette(dc);
		}

		BYTE* pBMPHeader = tempImage.GetBMPInfoHeader();
		// copy the pixels
        if (GetDIBits(dc, *(HBITMAP*)&hBitmap, 0, tempImage.m_Head.biHeight, pBMPHeader+sizeof(BITMAPINFOHEADER1), (LPBITMAPINFO)pBMPHeader, DIB_RGB_COLORS) == 0)
		{
			//replace &m_Head with pDib <Wil Stark>
            strcpy(m_Info.szLastError,"GetDIBits failed");
			::ReleaseDC(NULL, dc);
			return false;
        }
        ::ReleaseDC(NULL, dc);

		this->Create((BITMAPINFOHEADER1*)pBMPHeader);
		delete []pBMPHeader;
		return true;
    }
	return false;
}

bool CoImage::LoadResource(DWORD hRes, DWORD imagetype, DWORD hModule/*=NULL*/)
{
	DWORD rsize=SizeofResource((HMODULE)hModule,(HRSRC)hRes);
	HGLOBAL hMem=::LoadResource((HMODULE)hModule,(HRSRC)hRes);
	if (hMem)
	{
		char* lpVoid=(char*)LockResource(hMem);
		if (lpVoid)
		{
			char tmpPath[CVLIB_PATH_MAX] = {0};
			char tmpFile[CVLIB_PATH_MAX] = {0};
			GetTempPath(CVLIB_PATH_MAX,tmpPath);
			GetTempFileName(tmpPath,"IMG",0,tmpFile);
			FILE* fTmp=fopen(tmpFile,"w+b");
			if (fTmp)
			{
				fwrite(lpVoid,rsize,1,fTmp);
				fseek(fTmp,0,SEEK_SET);
				bool bOK = Decode(fTmp,imagetype);
				fclose(fTmp);
				DeleteFile(tmpFile);
				return bOK;
			}
// 			XFileMem fTmp((BYTE*)lpVoid,rsize);
// 			return Decode(&fTmp,imagetype);
		}
	}
	return false;
}

bool CoImage::Tile(DWORD hdc, Rect *rc)
{
	if(IsValid()&&(hdc)&&(rc))
	{
		int w = rc->width;
		int h = rc->height;
		int x,y,z;
		int bx=GetWidth();
		int by=GetHeight();
		for (y = 0 ; y < h ; y += by)
		{
			if ((y+by)>h)
			{
				by=h-y;
			}
			z=bx;
			for (x = 0 ; x < w ; x += z)
			{
				if ((x+z)>w)
					z=w-x;
				if (by == GetHeight())
					Draw(hdc,rc->x + x, rc->y + y, z, by, 0, 0, z, by);
				else
					Draw(hdc,rc->x + x, rc->y + y, z, by, 0, GetHeight()-by, z, by);
			}
		}
		return 1;
	}
	return 0;
}

DWORD CoImage::CopyToHandle()
{
	HANDLE hMem=NULL;
	if (IsValid())
	{
		hMem= GlobalAlloc(GHND, m_Head.biSize + m_Head.biSizeImage);
		if (hMem)
		{
			BYTE* pDst=(BYTE*)GlobalLock(hMem);
			if (pDst)
			{
				BYTE* pbData = GetBMPInfoHeader();
				memcpy(pDst,pbData,m_Head.biSize + m_Head.biSizeImage);
				delete []pbData;
			}
			GlobalUnlock(hMem);
		}
	}
	return (DWORD)hMem;
}

/**
 * Global object (clipboard paste) constructor
 * \param hMem: source bitmap object, the clipboard format must be CF_DIB
 * \return true if everything is ok
 */
bool CoImage::CreateFromHANDLE(DWORD hMem)
{
	m_pPalette=NULL;
	Release();

	DWORD dwSize = GlobalSize((HANDLE)hMem);
	if (!dwSize) return false;

	BYTE *lpVoid;						//pointer to the bitmap
	lpVoid = (BYTE *)GlobalLock((HANDLE)hMem);
	BITMAPINFOHEADER1 *pHead;			//pointer to the bitmap header
	pHead = (BITMAPINFOHEADER1 *)lpVoid;
	if (lpVoid)
	{
		BOOL res = Create(pHead);
		bool fRes;
		if (res)
			fRes = true;
		else
			fRes = false;
		GlobalUnlock(lpVoid);
		return fRes;
	}
	return false;
}
DWORD CoImage::MakeBitmap (DWORD hdc)
{
	if (!IsValid())
		return 0;

	if (!hdc){
		// this call to CreateBitmap doesn't create a DIB <jaslet>
		// // Create a device-independent bitmap <CSC>
		//  return CreateBitmap(head.biWidth,head.biHeight,	1, head.biBitCount, GetBits());
		// use instead this code
		BYTE* pBMPHeader=GetBMPInfoHeader();
		HDC hMemDC = CreateCompatibleDC(NULL);
		LPVOID pBit32;
		HBITMAP bmp = CreateDIBSection(hMemDC,(LPBITMAPINFO)pBMPHeader,DIB_RGB_COLORS, &pBit32, NULL, 0);
		if (pBit32) memcpy(pBit32, pBMPHeader, m_Head.biSizeImage);
		DeleteDC(hMemDC);
		delete []pBMPHeader;
		return (DWORD)bmp;
	}

	// this single line seems to work very well
	BYTE* pBMPHeader=GetBMPInfoHeader();
	HBITMAP bmp = CreateDIBitmap((HDC)hdc, (LPBITMAPINFOHEADER)pBMPHeader, CBM_INIT,
		pBMPHeader, (LPBITMAPINFO)pBMPHeader, DIB_RGB_COLORS);
	delete []pBMPHeader;

	return (DWORD)bmp;
}


#endif

#if CVLIB_IMG_SUPPORT_ALPHA
void CoImage::AlphaCreate()
{
	if (!m_matA.IsValid())
	{
		m_matA.Create (m_nRows, m_nCols, MAT_Tbyte);
		m_matA.Zero();
	}
}
void CoImage::AlphaDelete()
{
	if (m_matA.IsValid())
	{
		m_matA.Release();
	}
}
bool CoImage::AlphaIsValid()
{
	return (bool)m_matA.IsValid();
}
void CoImage::AlphaClear()
{
	m_matA.Zero();
}

void CoImage::AlphaInvert()
{
	if (AlphaIsValid()) 
	{
		BYTE *iSrc=m_matA.data.ptr[0];
		long n=GetHeight()*GetWidth();
		for(long i=0; i < n; i++){
			*iSrc=(BYTE)~(*(iSrc));
			iSrc++;
		}
	}
}

bool CoImage::AlphaMirror()
{
	if (!AlphaIsValid()) return false;
	Mat mAlpha2 (m_matA);
	BYTE *iSrc,*iDst;
	long wdt=GetWidth()-1;
	iSrc=mAlpha2.data.ptr[0] + wdt;
	iDst=m_matA.data.ptr[0];
	for(long y=0; y < GetHeight(); y++)
	{
		for(long x=0; x <= wdt; x++)
			*(iDst+x)=*(iSrc-x);
		iSrc+=GetWidth();
		iDst+=GetWidth();
	}
	return true;
}

bool CoImage::AlphaFlip()
{
	if (!AlphaIsValid()) return false;
	Mat mAlpha2 (m_matA);
	BYTE *iSrc,*iDst;
	iSrc=mAlpha2.data.ptr[0]-(GetHeight()-1)*GetWidth();
	iDst=m_matA.data.ptr[0];
    for(long y=0; y < GetHeight(); y++)
	{
		memcpy(iDst,iSrc,GetWidth());
		iSrc-=GetWidth();
		iDst+=GetWidth();
	}
	return true;
}

bool CoImage::AlphaCopy(CoImage &from)
{
	if (!from.AlphaIsValid() || GetWidth() != from.GetWidth() || GetHeight() != from.GetHeight()) 
		return false;
	if (!AlphaIsValid()) 
		AlphaCreate();
	memcpy(m_matA.data.ptr[0],from.AlphaGetPointer(), GetWidth()*GetHeight());
 	m_Info.nAlphaMax=from.m_Info.nAlphaMax;
	return true;
}

BYTE CoImage::AlphaGet(const long x,const long y)
{
	if (IsInterior(x, y)) return m_matA.data.ptr[y][x];
	return 0;
}

BYTE* CoImage::AlphaGetPointer(const long x /*= 0*/,const long y /*= 0*/)
{
	return &m_matA.data.ptr[y][x];
}

// 	bool AlphaSplit(CxImage *dest);
// 	void AlphaStrip();
void CoImage::AlphaSet(BYTE level)
{
	if (m_matA.data.ptr)
		memset(m_matA.data.ptr[0],level,m_nRows * m_nCols);
}
void CoImage::AlphaSet(const long x,const long y,const BYTE level)
{
	if (m_matA.data.ptr && IsInterior(x,y)) m_matA.data.ptr[y][x]=level;
}

#endif //CVLIB_IMG_SUPPORT_ALPHA

void CoImage::SetName(const char *name)
{
	assert((m_strName.IsEmpty()) || 
		( (*m_strName.GetLPCTSTR()) == 0) || 
		(name != m_strName.GetLPCTSTR()));
	m_strName = name;
}

const char * CoImage::Name(bool fIncludePath/*=false*/) const
{
	const char *szName = m_strName.GetLPCTSTR();
	
	if ((szName != 0) && (szName[0] != 0) && (!fIncludePath))
	{
		int cch = strlen(szName);
		assert(cch > 0);
		
		for (int ich = cch - 1; ich > 0; --ich)
		{
			if ((szName[ich] == '/') || (szName[ich] == '\\'))
			{
				szName += (ich + 1);
				break;
			}
		}
	}
	
	return szName;
}

void CoImage::GetPixelColor (float x, float y, float * result, int nChannel) const
{
	register int Y = (int)y;
	register int X = (int)x;
	
	register float c=y-Y;
	register float d=x-X;
	
	register float t1[3], t2[3];
	int i;
	
	if (nChannel == 3)
	{
		for (i=0; i<3; i++)
		{
			t1[i] = c*(GetBand()[i]->data.ptr[Y+1][X]-GetBand()[i]->data.ptr[Y][X]) + GetBand()[i]->data.ptr[Y][X];
			t2[i] = c*(GetBand()[i]->data.ptr[Y+1][X+1]-GetBand()[i]->data.ptr[Y][X+1]) + GetBand()[i]->data.ptr[Y][X+1];
			result[i] = d*(t2[i]-t1[i])+t1[i];
		}
	}
	else
	{
		if (m_matG.IsValid())
		{
			t1[0] = c*(m_matG.data.ptr[Y+1][X]-m_matG.data.ptr[Y][X]) + m_matG.data.ptr[Y][X];
			t2[0] = c*(m_matG.data.ptr[Y+1][X+1]-m_matG.data.ptr[Y][X+1]) + m_matG.data.ptr[Y][X+1];
			result[0] = d*(t2[0]-t1[0])+t1[0];
		}
	}
}

}
