/*! 
 * \file	CoImageBMP.cpp
 * \ingroup base
 * \brief	BMPÂô»¥Ì© Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#include "CoImageBMP.h"

#include "ImageIter.h"

namespace CVLib 
{

#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

#if CVLIB_IMG_SUPPORT_BMP

#ifndef WIDTHBYTES
#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */
#endif

#endif
	
#define DibWidthBytesN(lpbi, n) (UINT)WIDTHBYTES((UINT)(lpbi)->biWidth * (UINT)(n))
#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)
	
#define DibSizeImage(lpbi)      ((lpbi)->biSizeImage == 0 \
	? ((DWORD)(UINT)DibWidthBytes(lpbi) * (DWORD)(UINT)(lpbi)->biHeight) \
	: (lpbi)->biSizeImage)
	
#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
	? (int)(1 << (int)(lpbi)->biBitCount)          \
	: (int)(lpbi)->biClrUsed)

#define FixBitmapInfo(lpbi)     if ((lpbi)->biSizeImage == 0)                 \
	(lpbi)->biSizeImage = DibSizeImage(lpbi); \
	if ((lpbi)->biClrUsed == 0)                   \
	(lpbi)->biClrUsed = DibNumColors(lpbi);		\

const int RLE_COMMAND     = 0;
const int RLE_ENDOFLINE   = 0;
const int RLE_ENDOFBITMAP = 1;
const int RLE_DELTA       = 2;
	
#if !defined(BI_RLE8)
#define BI_RLE8  1L
#endif
#if !defined(BI_RLE4)
#define BI_RLE4  2L
#endif

CoImageBMP::CoImageBMP()
{
}

bool CoImageBMP::Decode (FILE* pFile)
{
	if (pFile == NULL) 
		return false;

	BITMAPFILEHEADER1   bf;
	DWORD off = ftell(pFile); //<CSC>
	try 
	{
		if (fread (&bf.bfType,sizeof(bf.bfType),1, pFile) == 0)
			throw "Not a BMP";
		if (fread (&bf.bfSize,sizeof(bf.bfSize),1, pFile) == 0)
			throw "Not a BMP";
		if (fread (&bf.bfReserved1,sizeof(bf.bfReserved1),1, pFile) == 0)
			throw "Not a BMP";
		if (fread (&bf.bfReserved2,sizeof(bf.bfReserved2),1, pFile) == 0)
			throw "Not a BMP";
		if (fread (&bf.bfOffBits,sizeof(bf.bfOffBits),1, pFile) == 0)
			throw "Not a BMP";
// 		if (fread(&bf,MIN(14,sizeof(bf)),1, pFile)==0) 
// 			throw "Not a BMP";
		if (bf.bfType != BFT_BITMAP)
		{ //do we have a RC HEADER?
			bf.bfOffBits = 0L;
			fseek(pFile, off, SEEK_SET);
			return false;
		}

		BITMAPINFOHEADER1 tempHeader;
		if (!DibReadBitmapInfo(pFile,&tempHeader)) 
			throw "Error reading BMP info";

		tempHeader.biSizeImage = DibSizeImage(&tempHeader);
		long llen = tempHeader.biSizeImage + tempHeader.biClrUsed * sizeof(RGBQUAD1);

		BYTE* pbData = new BYTE[llen + sizeof(tempHeader)];
		memcpy(pbData, &tempHeader, sizeof(tempHeader));
		if (!fread (pbData + sizeof(tempHeader), 1, llen, pFile) == 1)
		{
			delete []pbData;
			return false;
		}

		BITMAPINFOHEADER1* pbmpHeader = (BITMAPINFOHEADER1*)pbData;

		bool bIsOldBmp = pbmpHeader->biSize == sizeof(BITMAPCOREHEADER1);
		
		bool bTopDownDib = pbmpHeader->biHeight<0; //<Flanders> check if it's a top-down bitmap
		if (bTopDownDib) pbmpHeader->biHeight=-pbmpHeader->biHeight;
		
		if (m_Info.nEscape == -1) {
			// Return output dimensions only
			m_Head.biWidth = pbmpHeader->biWidth;
			m_Head.biHeight = pbmpHeader->biHeight;
			throw "output dimensions returned";
		}

		if (pbmpHeader->biBitCount > 8)
		{
			if (Create ((BITMAPINFOHEADER1*)pbData))
			{
				if (bTopDownDib)
					Flip();
				delete []pbData;
				return true;
			}
			delete []pbData;
			return false;
		}
		Startup (0);
		BYTE* pBMPData = pbData + sizeof(BITMAPINFOHEADER1);
		int nH = pbmpHeader->biHeight;
		int nW = pbmpHeader->biWidth;
		
		memcpy (&m_Head, pbmpHeader, sizeof(m_Head));
		CreateInfo (nH, nW, pbmpHeader->biBitCount, CVLIB_IMG_FORMAT_BMP);

		m_Head.biXPelsPerMeter = pbmpHeader->biXPelsPerMeter;
		m_Head.biYPelsPerMeter = pbmpHeader->biYPelsPerMeter;
		m_Info.xDPI = (long) floor(pbmpHeader->biXPelsPerMeter * 254.0 / 10000.0 + 0.5);
		m_Info.yDPI = (long) floor(pbmpHeader->biYPelsPerMeter * 254.0 / 10000.0 + 0.5);
		
		if (m_Info.nEscape) 
			throw "Cancelled"; // <vho> - cancel decoding
		
		RGBQUAD1 *pRgb = GetPalette();
		if (pRgb){
			if (bIsOldBmp){
				// convert a old color table (3 byte entries) to a new
				// color table (4 byte entries)
				memcpy (pRgb, pBMPData, DibNumColors(pbmpHeader) * sizeof(RGBTRIPLE1));
				for (int i=DibNumColors(&m_Head)-1; i>=0; i--){
					pRgb[i].rgbRed      = ((RGBTRIPLE1 *)pRgb)[i].rgbtRed;
					pRgb[i].rgbBlue     = ((RGBTRIPLE1 *)pRgb)[i].rgbtBlue;
					pRgb[i].rgbGreen    = ((RGBTRIPLE1 *)pRgb)[i].rgbtGreen;
					pRgb[i].rgbReserved = (BYTE)0;
				}
			} else {
				memcpy (pRgb, pBMPData, DibNumColors(pbmpHeader) * sizeof(RGBQUAD1));
				//force rgbReserved=0, to avoid problems with some WinXp bitmaps
				for (unsigned int i=0; i<m_Head.biClrUsed; i++)
					pRgb[i].rgbReserved=0;
			}
		}
		
		if (m_Info.nEscape) throw "Cancelled"; // <vho> - cancel decoding
		BYTE** ppbRData = m_matX.data.ptr;
		BYTE** ppbGData = m_matY.data.ptr;
		BYTE** ppbBData = m_matZ.data.ptr;
		{
			if (bf.bfOffBits != 0L) 
 				fseek(pFile, off + bf.bfOffBits,SEEK_SET);
			switch (pbmpHeader->biCompression) 
			{
			case BI_RGB :
				int i;
				{
					BYTE* pbTemp=new BYTE[m_Info.dwEffWidth];
					for (i = nH - 1; i >= 0; i --)
					{
						fread (pbTemp, m_Info.dwEffWidth, 1, pFile);
						int bt=pbTemp[0];
						switch (m_Head.biBitCount)
						{
						case 1:
							{
								for (int j=0; j<nW; j++)
								{
									if (j != 0 && (j%8) == 0)
										bt=pbTemp[j/8];
									bt = bt << 1;
									int nIdx=bt/256;
									ppbRData[i][j] = GetPalette()[nIdx].rgbRed;
									ppbGData[i][j] = GetPalette()[nIdx].rgbGreen;
									ppbBData[i][j] = GetPalette()[nIdx].rgbBlue;
									bt = bt%256;
								}
							}
							break;
						case 4:
							{
								for (int j=0; j<nW; j++)
								{
									if (j != 0 && (j%2) == 0)
										bt=pbTemp[j/2];
									bt = bt << 4;
									int nIdx=bt/256;
									ppbRData[i][j] = GetPalette()[nIdx].rgbRed;
									ppbGData[i][j] = GetPalette()[nIdx].rgbGreen;
									ppbBData[i][j] = GetPalette()[nIdx].rgbBlue;
									bt = bt%256;
								}
							}
							break;
						case 8:
							{
								for (int j=0; j<nW; j++)
								{
									int nIdx=pbTemp[j];
									ppbRData[i][j] = GetPalette()[nIdx].rgbRed;
									ppbGData[i][j] = GetPalette()[nIdx].rgbGreen;
									ppbBData[i][j] = GetPalette()[nIdx].rgbBlue;
								}
							}
							break;
						}
					}
					delete []pbTemp;
				}
				break;
			case BI_RLE4 :
				{
					BYTE status_byte = 0;
					BYTE second_byte = 0;
					int scanline = 0;
					int bits = 0;
					BOOL low_nibble = FALSE;
					ImageIterator iter(this);

					for (BOOL bContinue = TRUE; bContinue;)
					{
						fread(&status_byte, sizeof(BYTE), 1, pFile);
						switch (status_byte)
						{
						case RLE_COMMAND :
							fread(&status_byte, sizeof(BYTE), 1, pFile);
							switch (status_byte) 
							{
							case RLE_ENDOFLINE :
								bits = 0;
								scanline++;
								low_nibble = FALSE;
								break;
							case RLE_ENDOFBITMAP :
								bContinue=FALSE;
								break;
							case RLE_DELTA :
								{
									// read the delta values
									BYTE delta_x;
									BYTE delta_y;
									fread(&delta_x, sizeof(BYTE), 1, pFile);
									fread(&delta_y, sizeof(BYTE), 1, pFile);
									// apply them
									bits       += delta_x / 2;
									scanline   += delta_y;
									break;
								}
							default :
								fread(&second_byte, sizeof(BYTE), 1, pFile);
								BYTE *sline = iter.GetRow(scanline);
								for (int i = 0; i < status_byte; i++)
								{
									if (low_nibble)
									{
										if ((DWORD)(sline+bits) < (DWORD)(m_Info.pImage+m_Head.biSizeImage))
										{
											*(sline + bits) |=  (second_byte & 0x0F);
										}
										if (i != status_byte - 1)
											fread(&second_byte, sizeof(BYTE), 1, pFile);
										bits++;
									}
									else
									{
										if ((DWORD)(sline+bits) < (DWORD)(m_Info.pImage+m_Head.biSizeImage)){
											*(sline + bits) = (BYTE)(second_byte & 0xF0);
										}
									}
									low_nibble = !low_nibble;
								}
								if ((((status_byte+1) >> 1) & 1 )== 1)
									fread(&second_byte, sizeof(BYTE), 1, pFile);
								break;
							};
							break;
						default :
							{
								BYTE *sline = iter.GetRow(scanline);
								fread(&second_byte, sizeof(BYTE), 1, pFile);
								for (unsigned i = 0; i < status_byte; i++) {
									if (low_nibble) {
										if ((DWORD)(sline+bits) < (DWORD)(m_Info.pImage+m_Head.biSizeImage)){
											*(sline + bits) |= (second_byte & 0x0F);
										}
										bits++;
									} else {
										if ((DWORD)(sline+bits) < (DWORD)(m_Info.pImage+m_Head.biSizeImage)){
											*(sline + bits) = (BYTE)(second_byte & 0xF0);
										}
									}				
									low_nibble = !low_nibble;
								}
							}
							break;
						};
					}
					break;
				}
			case BI_RLE8 :
				{
					BYTE status_byte = 0;
					BYTE second_byte = 0;
					int scanline = 0;
					int bits = 0;
					ImageIterator iter(this);

					for (BOOL bContinue = TRUE; bContinue; )
					{
						fread(&status_byte, sizeof(BYTE), 1, pFile);
						switch (status_byte)
						{
						case RLE_COMMAND :
							fread(&status_byte, sizeof(BYTE), 1, pFile);
							switch (status_byte) 
							{
							case RLE_ENDOFLINE :
								bits = 0;
								scanline++;
								break;
							case RLE_ENDOFBITMAP :
								bContinue=FALSE;
								break;
							case RLE_DELTA :
								{
									// read the delta values
									BYTE delta_x;
									BYTE delta_y;
									fread(&delta_x, sizeof(BYTE), 1, pFile);
									fread(&delta_y, sizeof(BYTE), 1, pFile);
									// apply them
									bits     += delta_x;
									scanline += delta_y;
									break;
								}
							default :
								fread((void *)(iter.GetRow(scanline) + bits), sizeof(BYTE) * status_byte, 1, pFile);
								// align run length to even number of bytes 
								if ((status_byte & 1) == 1)
									fread(&second_byte, sizeof(BYTE), 1, pFile);
								bits += status_byte;													
								break;								
							};
							break;
						default :
							BYTE *sline = iter.GetRow(scanline);
							fread(&second_byte, sizeof(BYTE), 1, pFile);
							for (unsigned i = 0; i < status_byte; i++)
							{
								if ((DWORD)bits<m_Info.dwEffWidth)
								{
									*(sline + bits) = second_byte;
									bits++;					
								}
								else
								{
									bContinue = FALSE;
									break;
								}
							}
							break;
						};
					}
					break;
				}
			default :								
				throw "compression type not supported";
			}
		}
		m_Info.dwEffWidth = ((((24 * nW) + 31) / 32) * 4);
		m_Head.biSizeImage = m_Info.dwEffWidth * nH;
		
		delete []pbData;
 		if (bTopDownDib)
 			Flip(); //<Flanders>
		if (!m_ppBands) m_ppBands = new Mat*[3];
		m_ppBands[0]=&m_matX;
		m_ppBands[1]=&m_matY;
		m_ppBands[2]=&m_matZ;
	}
	catch (char* szMessage)
	{
		printf ("%s", szMessage, 255);
		return false;
	}
    return true;
}

bool CoImageBMP::DibReadBitmapInfo(FILE* pFile, BITMAPINFOHEADER1 *pdib)
{
	if ((pFile==NULL)||(pdib==NULL))
		return false;
	
    if (fread (pdib,sizeof(BITMAPINFOHEADER1),1, pFile)==0)
		return false;

    BITMAPCOREHEADER1   bc;

    switch (pdib->biSize) // what type of bitmap info is this?
    {
	case sizeof(BITMAPINFOHEADER1):
		break;

	case 64: //sizeof(OS2_BMP_HEADER):
		fseek(pFile, (long)(64 - sizeof(BITMAPINFOHEADER1)),SEEK_CUR);
		break;

	case sizeof(BITMAPCOREHEADER1):
		bc = *(BITMAPCOREHEADER1*)pdib;
		pdib->biSize               = bc.bcSize;
		pdib->biWidth              = (DWORD)bc.bcWidth;
		pdib->biHeight             = (DWORD)bc.bcHeight;
		pdib->biPlanes             =  bc.bcPlanes;
		pdib->biBitCount           =  bc.bcBitCount;
		pdib->biCompression        = BI_RGB;
		pdib->biSizeImage          = 0;
		pdib->biXPelsPerMeter      = 0;
		pdib->biYPelsPerMeter      = 0;
		pdib->biClrUsed            = 0;
		pdib->biClrImportant       = 0;
		
		fseek(pFile, (long)(sizeof(BITMAPCOREHEADER1)-sizeof(BITMAPINFOHEADER1)), SEEK_CUR);

		break;
	default:
		//give a last chance
		if (pdib->biSize>(sizeof(BITMAPINFOHEADER1))&&
			(pdib->biSizeImage==(unsigned long)(pdib->biHeight*((((pdib->biBitCount*pdib->biWidth)+31)/32)*4)))&&
			(pdib->biPlanes==1)&&(pdib->biCompression==BI_RGB)&&(pdib->biClrUsed==0))
		{
			fseek(pFile, (long)(pdib->biSize - sizeof(BITMAPINFOHEADER1)),SEEK_CUR);
			break;
		}
		return false;
    }
	
    FixBitmapInfo(pdib);
	
    return true;
}
		
#if CVLIB_IMG_SUPPORT_ENCODE
bool CoImageBMP::Encode(FILE* pFile)
{
	if (EncodeSafeCheck(pFile))
		return false;

	BITMAPINFOHEADER1* pBIF = (BITMAPINFOHEADER1*)GetBMPInfoHeader ();
	int nSize = pBIF->biSize + pBIF->biSizeImage + pBIF->biClrUsed * sizeof(RGBQUAD1);

	BITMAPFILEHEADER1	hdr;

	hdr.bfType = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
	hdr.bfSize = nSize + 14/*sizeof(BITMAPFILEHEADER1)*/;
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = 14/*sizeof(BITMAPFILEHEADER1)*/ + pBIF->biSize + pBIF->biClrUsed * sizeof(RGBQUAD1);
	
    // Write the file header
//	fwrite (&hdr,MIN(14,sizeof(BITMAPFILEHEADER1)),1, pFile);
	fwrite (&hdr.bfType,sizeof(hdr.bfType),1, pFile);
	fwrite (&hdr.bfSize,sizeof(hdr.bfSize),1, pFile);
	fwrite (&hdr.bfReserved1,sizeof(hdr.bfReserved1),1, pFile);
	fwrite (&hdr.bfReserved2,sizeof(hdr.bfReserved2),1, pFile);
	fwrite (&hdr.bfOffBits,sizeof(hdr.bfOffBits),1, pFile);

    // Write the DIB header and the pixels
 	fwrite ((BYTE*)pBIF, nSize, 1, pFile);

	delete []pBIF;

	return true;
}
#endif // CVLIB_IMG_SUPPORT_ENCODE

}