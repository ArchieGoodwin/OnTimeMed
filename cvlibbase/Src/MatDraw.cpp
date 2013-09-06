/*!
 * \file matdraw.cpp
 * \ingroup base
 * \brief ÃÔµÛ¿Ðµ¹º÷Ëæ drawingÂÜºã¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */

#include "cvlibmacros.h"

#if CVLIB_OS==CVLIB_OS_WIN32
#include <windows.h>
#include "TCHAR.h"
#undef GetRValue
#undef GetGValue
#undef GetBValue
#endif

#include "Mat.h"
#include "CoImage.h"

namespace CVLib 
{
	int Mat::Create (BITMAPINFOHEADER1* pBI)
	{
		BYTE* pBMPData = (BYTE*)pBI + sizeof(BITMAPINFOHEADER1);
		int nH = pBI->biHeight;
		int nW = pBI->biWidth;

		if (pBI->biBitCount==32)
			Create (nH, nW, MAT_Tbyte4);
		else
			Create (nH, nW, MAT_Tbyte3);

		int cn=Channels();
		int nRealWidth = ( nW * pBI->biBitCount + 31 ) / 32 * 4;

		int nTemp;
		WORD wRGB;
		int i, j;
		switch(pBI->biBitCount)
		{
		case 24:
			for (i = 0; i < nH; i ++) 
			{
				nTemp=(nH - i - 1) * nRealWidth;
				for (j = 0; j < nW; j ++)
				{
					memcpy(&data.ptr[i][j*cn], &pBMPData[nTemp], cn);
					nTemp += 3;
				}
			}
			break;
		case 32:
			for (i = 0; i < nH; i ++)
			{
				nTemp = (nH - i - 1) * nRealWidth;
				for (j = 0; j < nW; j ++)
				{
					memcpy(&data.ptr[i][j*cn], &pBMPData[nTemp], cn);
					nTemp += 4;
				}
			}
			break;
		case 16:
			for (i = 0; i < nH; i ++) for (j = 0; j < nW; j ++)
			{
				nTemp = (nH - i - 1) * nRealWidth + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				data.ptr[i][j*cn+2] = (wRGB & 0x1F) << 3;
				data.ptr[i][j*cn+1] = ((wRGB >> 5) & 0x1F) << 3;
				data.ptr[i][j*cn] = ((wRGB >> 10) & 0x1F) << 3;
			}
			break;
		case 12:
			for (i = 0; i < nH; i ++) for (j = 0; j < nW; j ++)
			{
				nTemp = (nH - i - 1) * nRealWidth + j * 2;
				wRGB = (WORD)(pBMPData[nTemp] + (pBMPData[nTemp + 1] << 8));
				data.ptr[i][j*cn+2] = (wRGB & 0x1F) << 3;
				data.ptr[i][j*cn+1] = ((wRGB >> 5) & 0x1F) << 3;
				data.ptr[i][j*cn] = ((wRGB >> 10) & 0x1F) << 3;
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
		return 1;
	}

	void Mat::DrawRect (const Rect& rect, COLORREF color, int thick/*=1*/, const float rOpacity/*=1*/)
	{
		int cn=Channels();
		uchar ucolor[3]={GetRValue(color),GetGValue(color),GetBValue(color)};
		if (!IsInterior(rect.x, rect.y) || rect.LimX()>Cols() || rect.LimY()>Rows() )
			return;
		if (rect.width==0||rect.height==0)
			return;

		const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
		int nfrom=-thick/2;
		int nto=nfrom+thick;

		for (int ii=nfrom; ii<nto; ii++)
		{
			Rect drawRegion=Rect(rect.x+ii,rect.y+ii,rect.width-ii*2,rect.height-ii*2);
			
			if (rNopacity >= 1)
			{
				for (int iX = drawRegion.x; iX < drawRegion.LimX(); iX ++)
				{
					for (int k=0;k<cn;k++)
					{
						data.ptr[drawRegion.y][iX*cn+k]=ucolor[k];
						data.ptr[drawRegion.y+drawRegion.height-1][iX*cn+k]=ucolor[k];
					}
				}
				for (int iY = drawRegion.y; iY < drawRegion.LimY(); iY ++)
				{
					for (int k=0;k<cn;k++)
					{
						data.ptr[iY][drawRegion.x*cn+k]=ucolor[k];
						data.ptr[iY][(drawRegion.x+drawRegion.width-1)*cn+k]=ucolor[k];
					}
				}
			}
			else
			{
				for (int iX = drawRegion.x; iX < drawRegion.LimX(); iX ++)
				{
					for (int k=0;k<cn;k++)
					{
						data.ptr[drawRegion.y][iX*cn+k]=(BYTE)(data.ptr[drawRegion.y][iX*cn+k]*rCopacity+ucolor[k]*rNopacity);
						data.ptr[drawRegion.y+drawRegion.height-1][iX*cn+k]=(BYTE)(data.ptr[drawRegion.y+drawRegion.height-1][iX*cn+k]*rCopacity+ucolor[k]*rNopacity);
					}
				}
				for (int iY = drawRegion.y; iY < drawRegion.LimY(); iY ++)
				{
					for (int k=0;k<cn;k++)
					{
						data.ptr[iY][drawRegion.x*cn+k]=(BYTE)(data.ptr[iY][drawRegion.x*cn+k]*rCopacity+ucolor[k]*rNopacity);
						data.ptr[iY][(drawRegion.x+drawRegion.width-1)*cn+k]=(BYTE)(data.ptr[iY][(drawRegion.x+drawRegion.width-1)*cn+k]*rCopacity+ucolor[k]*rNopacity);
					}
				}
			}
		}
	}
	void	Mat::DrawEllipse (const Rect& rect, COLORREF color, const float rOpacity/*=1*/)
	{
		int cn=Channels();
		uchar ucolor[3]={GetRValue(color),GetGValue(color),GetBValue(color)};

		int nXrad = rect.width / 2;
		int nYrad = rect.height / 2;
		int nXradSq = nXrad * nXrad;
		int nYradSq = nYrad * nYrad;
		int nXcen = rect.x + nXrad;
		int nYcen = rect.y + nYrad;
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

		const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);

		if (rNopacity >= 1)
		{
			for (int ii = 0; ii < nPerimLength; ii ++) for (int k=0;k<cn;k++)
			{
				nX = pnTemp[ii*2];
				nY = pnTemp[ii*2+1];
				data.ptr[nY][nX*cn+k]=ucolor[k];
			}
		}
		else
		{
			for (int ii = 0; ii < nPerimLength; ii ++) for (int k=0;k<cn;k++)
			{
				nX = pnTemp[ii*2];
				nY = pnTemp[ii*2+1];
				data.ptr[nY][nX*cn+k]=(BYTE)(data.ptr[nY][nX*cn+k] * rCopacity + ucolor[k] * rNopacity);
			}
		}
		free (pnTemp);
	}
	void	Mat::DrawEllipse (const RotatedRect& rect, COLORREF color, const float rOpacity/*=1*/)
	{
		assert (false);
	}
	void	Mat::DrawPolygon (Point2i* pts, int num, COLORREF color, bool open, const float rOpacity)
	{
		for (int i=0; i<num-1; i++)
			DrawLine (pts[i], pts[i+1], color, rOpacity);
		if (!open)
			DrawLine (pts[0], pts[num-1], color, rOpacity);
	}
	void	Mat::DrawPixel (int x, int y, COLORREF color, const float rOpacity)
	{
		DrawPixel(Point2i(x,y), color, rOpacity);
	}
	void	Mat::DrawPixel (const Point2i& pt, COLORREF color, const float rOpacity/*=1*/)
	{
		int cn=Channels();
		uchar ucolor[3]={GetRValue(color),GetGValue(color),GetBValue(color)};
		if (IsInterior(pt.x, pt.y))
		{
			const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);

			if (rNopacity >= 1)
			{
				for (int k=0;k<cn;k++)
					data.ptr[pt.y][pt.x*cn+k] = ucolor[k];
			}
			else
			{
				for (int k=0;k<cn;k++)
					data.ptr[pt.y][pt.x*cn+k] = (BYTE)(data.ptr[pt.y][pt.x*cn+k]*rCopacity+ucolor[k]*rNopacity);
			}
		}
	}
	void	Mat::DrawLine (const Point2i& pt1, const Point2i& pt2, COLORREF color, const float rOpacity/*=1*/)
	{
		int cn=Channels();
		uchar ucolor[3]={GetRValue(color),GetGValue(color),GetBValue(color)};
		int* pnX;
		int* pnY;
		int nNum;
		CoImage::Line_pt (pt1.x, pt1.y, pt2.x, pt2.y, pnX, pnY, nNum);
		const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);

		if (rNopacity >= 1)
		{
			for (int ii = 0; ii < nNum; ii ++)
			{
				if (IsInterior(pnX[ii], pnY[ii]))
				{
					for (int k=0; k<cn; k++)
						data.ptr[pnY[ii]][pnX[ii]*cn+k]=ucolor[k];
				}
			}
		}
		else
		{
			for (int ii = 0; ii < nNum; ii ++)
			{
				if (IsInterior(pnX[ii], pnY[ii]))
				{
					for (int k=0; k<cn; k++)
						data.ptr[pnY[ii]][pnX[ii]*cn+k]=(BYTE)(data.ptr[pnY[ii]][pnX[ii]*cn+k]*rCopacity+ucolor[k]*rNopacity);
				}
			}
		}
		delete []pnX;
		delete []pnY;
	}
	void Mat::DrawCross (const Point2i& pt, int nSide, COLORREF color, const float rOpacity/*=1*/)
	{
		if (IsInterior(pt.x, pt.y))
		{
			DrawLine ( Point2i(pt.x-nSide, pt.y), Point2i(pt.x+nSide+1, pt.y), color, rOpacity);
			DrawLine ( Point2i(pt.x, pt.y-nSide), Point2i(pt.x, pt.y+nSide+1), color, rOpacity);
		}
	}

	void Mat::DrawMat(const Mat& img, const Point2i& pt, const float rOpacity/*=1*/)
	{
		int cn=Channels();
		if (cn<img.Channels()) 
			return;

		const float rNopacity = ABS(rOpacity), rCopacity = 1-MAX(rOpacity, 0.0f);
		if (cn==img.Channels())
		{
			if (rNopacity >= 1)
			{
				for (int iX = pt.x, i = 0; iX < MIN(cols, pt.x + img.cols); iX ++, i ++)
					for (int iY = pt.y, j = 0; iY < MIN(rows, pt.y + img.rows); iY ++, j ++)
					{
						for (int k=0;k<cn;k++)
							data.ptr[iY][iX*cn+k]=img.data.ptr[j][i*cn+k];
					}
			}
			else
			{
				for (int iX = pt.x, i = 0; iX < MIN(cols, pt.x+img.cols); iX ++, i ++)
					for (int iY = pt.y, j = 0; iY < MIN(rows, pt.y + img.rows); iY ++, j ++)
					{
						for (int k=0;k<cn;k++)
							data.ptr[iY][iX*cn+k]=(BYTE)(data.ptr[iY][iX*cn+k]*rCopacity+img.data.ptr[j][i*cn+k]*rNopacity);
					}
			}
		}
		else
		{
			assert(cn==3);
			assert(img.Channels()==1);
			if (rNopacity >= 1)
			{
				for (int iX = pt.x, i = 0; iX < MIN(cols, pt.x + img.cols); iX ++, i ++)
					for (int iY = pt.y, j = 0; iY < MIN(rows, pt.y + img.rows); iY ++, j ++)
					{
						for (int k=0;k<cn;k++)
							data.ptr[iY][iX*cn+k]=img.data.ptr[j][i];
					}
			}
			else
			{
				for (int iX = pt.x, i = 0; iX < MIN(cols, pt.x+img.cols); iX ++, i ++)
					for (int iY = pt.y, j = 0; iY < MIN(rows, pt.y + img.rows); iY ++, j ++)
					{
						for (int k=0;k<cn;k++)
							data.ptr[iY][iX*cn+k]=(BYTE)(data.ptr[iY][iX*cn+k]*rCopacity+img.data.ptr[j][i]*rNopacity);
					}
			}
		}
	}

#if CVLIB_OS==CVLIB_OS_WIN32
	BYTE*	Mat::GetBMPInfoHeader() const
	{
		assert (Type() == MAT_Tbyte);
		int nH = rows;
		int nW = cols;

		int nRealWidth = ( nW * 24 + 31 ) / 32 * 4;

		BYTE* pBuf = new BYTE[sizeof(BITMAPINFOHEADER1) + nH * nRealWidth];
		BITMAPINFOHEADER1* pBMPHeader = (BITMAPINFOHEADER1*)pBuf;
		BYTE* pbData = pBuf + sizeof(BITMAPINFOHEADER1);

		memset (pBMPHeader, 0, sizeof(BITMAPINFOHEADER1));
		pBMPHeader->biBitCount = 24;
		pBMPHeader->biHeight = nH;
		pBMPHeader->biWidth = nW;
		pBMPHeader->biSizeImage = nH * nRealWidth;
		pBMPHeader->biSize = sizeof(BITMAPINFOHEADER1);
		pBMPHeader->biPlanes = 1;
		pBMPHeader->biXPelsPerMeter = (long) floor(96 * 10000.0 / 254.0 + 0.5);
		pBMPHeader->biYPelsPerMeter = (long) floor(96 * 10000.0 / 254.0 + 0.5);
		int cn=Channels();
		if (cn==1)
		{
			for (int i = 0; i < nH; i ++)
			{
				BYTE* pbSrc = data.ptr[i];
				for (int j = 0; j < nW; j ++)
				{
					int nTemp = (nH - i - 1) * nRealWidth + j * 3;
					pbData[nTemp] = pbSrc[j];
					pbData[nTemp + 1] = pbSrc[j];
					pbData[nTemp + 2] = pbSrc[j];
				}
			}
		}
		else
		{
			for (int i = 0; i < nH; i ++)
			{
				int nTemp=(nH - i - 1) * nRealWidth;
				for (int j = 0; j < nW*3; j+=3)
				{
					pbData[nTemp+2]=data.ptr[i][j];
					pbData[nTemp+1]=data.ptr[i][j+1];
					pbData[nTemp+0]=data.ptr[i][j+2];
					nTemp+=3;
				}
			}
		}
		return pBuf;
	}
	void Mat::Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY) const
	{
		Draw (hDC, nDstX,nDstY, nDstCX, nDstCY,0,0,Cols(),Rows());
	}
	void Mat::Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY,
		int nSrcX, int nSrcY, int nSrcCX, int nSrcCY) const
	{
		if (!IsValid())
			return;
		BYTE* pbData = GetBMPInfoHeader();
		int nPrevMode = SetStretchBltMode (*(HDC*)&hDC, HALFTONE);
		RealizePalette(*(HDC*)&hDC);
		BITMAPINFO1 bmpInfo;
		memset(&bmpInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER1));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER1);
		bmpInfo.bmiHeader.biHeight = rows;
		bmpInfo.bmiHeader.biWidth = cols;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 24;
		::StretchDIBits (*(HDC*)&hDC,
			nDstX, nDstY, nDstCX, nDstCY, nSrcX, nSrcY, nSrcCX, nSrcCY,
			pbData + sizeof(BITMAPINFOHEADER1), (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		delete []pbData;
		SetStretchBltMode (*(HDC*)&hDC, nPrevMode);
	}

	void Mat::Draw(DWORD hDC) const
	{
		Draw(hDC, 0, 0, cols, rows, 0, 0, cols, rows);
	}

	int	Mat::DrawString(DWORD hdc, long x, long y, const char* szText1, COLORREF color, const char* szFont1, 
		long lSize, long lWeight, BYTE bItalic, BYTE bUnderline, const float rOpacity /*= 1*/)
	{
		if (!IsValid())
			return 1;
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

		Mat itext;
		itext.CreateFromHBITMAP(*(DWORD*)&TmpBmp);

		for (long ix=0;ix<width;ix++)
		{
			for (long iy=0;iy<height;iy++)
			{
				if (itext.Value(iy,ix,0))
					DrawPixel(x+ix,y+iy,color);
			}
		}

		//cleanup
		if (pOldFont) SelectObject(TmpDC,pOldFont);
		DeleteObject(m_Font);
		free(m_pLF);
		DeleteObject(SelectObject(TmpDC,TmpObj));
		DeleteDC(TmpDC);
		return 1;
	}

	int	Mat::CreateFromHBITMAP(DWORD hBitmap, DWORD hPalette/* = 0*/)
	{
		Release();

		if (hBitmap) { 
			Mat tempImage;
			BITMAP bm;
			// get informations about the bitmap
			GetObject(*(HBITMAP*)&hBitmap, sizeof(BITMAP), (LPSTR) &bm);
			// create the image
			if (!tempImage.Create(bm.bmHeight, bm.bmWidth, MAT_Tbyte3))
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
			if (GetDIBits(dc, *(HBITMAP*)&hBitmap, 0, tempImage.Rows(), pBMPHeader+sizeof(BITMAPINFOHEADER1), (LPBITMAPINFO)pBMPHeader, DIB_RGB_COLORS) == 0)
			{
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

	bool Mat::LoadResource(DWORD hRes, DWORD imagetype, DWORD hModule/*=NULL*/)
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
					CoImage tempImage;
					fwrite(lpVoid,rsize,1,fTmp);
					fseek(fTmp,0,SEEK_SET);
					bool bOK = tempImage.Decode(fTmp,imagetype);
					*this=(Mat)tempImage;
					fclose(fTmp);
					DeleteFile(tmpFile);
					return bOK;
				}
			}
		}
		return false;
	}

	bool Mat::Tile(DWORD hdc, Rect *rc)
	{
		if(IsValid()&&(hdc)&&(rc))
		{
			int w = rc->width;
			int h = rc->height;
			int x,y,z;
			int bx=Cols();
			int by=Rows();
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
					if (by == Rows())
						Draw(hdc,rc->x + x, rc->y + y, z, by, 0, 0, z, by);
					else
						Draw(hdc,rc->x + x, rc->y + y, z, by, 0, Rows()-by, z, by);
				}
			}
			return 1;
		}
		return 0;
	}

	DWORD Mat::CopyToHandle()
	{
		HANDLE hMem=NULL;
		if (IsValid())
		{
			int bisize=sizeof(BITMAPINFOHEADER1);
			int dwEffWidth = ((((24 * cols) + 31) / 32) * 4);
			int biSizeImage = dwEffWidth * rows;
			hMem= GlobalAlloc(GHND, bisize + biSizeImage);
			if (hMem)
			{
				BYTE* pDst=(BYTE*)GlobalLock(hMem);
				if (pDst)
				{
					BYTE* pbData = GetBMPInfoHeader();
					memcpy(pDst,pbData,bisize+biSizeImage);
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
	bool Mat::CreateFromHANDLE(DWORD hMem)
	{
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

#endif

}