/*! 
 * \file    CoImage.h
 * \ingroup base
 * \brief   Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _COIMAGE_H_
#define _COIMAGE_H_

#include "cvlibbasedef.h"	//<vho> adjust some #define
#include "Mat.h"
#include "CoImageDef.h"	    //<vho> adjust some #define
#include "SString.h"
#include "cvlibstructs.h"

namespace CVLib 
{

//! Ãùº¬Âô»¥ 
enum ENUM_CVLIB_IMG_FORMATS
{
			CVLIB_IMG_FORMAT_UNKNOWN,
#if CVLIB_IMG_SUPPORT_BMP
			CVLIB_IMG_FORMAT_BMP,
#endif
#if CVLIB_IMG_SUPPORT_GIF
			CVLIB_IMG_FORMAT_GIF,
#endif
// #if CVLIB_IMG_SUPPORT_JPG
			CVLIB_IMG_FORMAT_JPG,
// #endif
#if CVLIB_IMG_SUPPORT_PNG
			CVLIB_IMG_FORMAT_PNG,
#endif
#if CVLIB_IMG_SUPPORT_MNG
			CVLIB_IMG_FORMAT_MNG,
#endif
#if CVLIB_IMG_SUPPORT_ICO
			CVLIB_IMG_FORMAT_ICO,
#endif
#if CVLIB_IMG_SUPPORT_TIF
			CVLIB_IMG_FORMAT_TIF,
#endif
#if CVLIB_IMG_SUPPORT_TGA
			CVLIB_IMG_FORMAT_TGA,
#endif
#if CVLIB_IMG_SUPPORT_PCX
			CVLIB_IMG_FORMAT_PCX,
#endif
#if CVLIB_IMG_SUPPORT_WBMP
			CVLIB_IMG_FORMAT_WBMP,
#endif
#if CVLIB_IMG_SUPPORT_WMF
			CVLIB_IMG_FORMAT_WMF,
#endif
#if CVLIB_IMG_SUPPORT_J2K
			CVLIB_IMG_FORMAT_J2K,
#endif
#if CVLIB_IMG_SUPPORT_JBG
			CVLIB_IMG_FORMAT_JBG,
#endif
#if CVLIB_IMG_SUPPORT_JP2
			CVLIB_IMG_FORMAT_JP2,
#endif
#if CVLIB_IMG_SUPPORT_JPC
			CVLIB_IMG_FORMAT_JPC,
#endif
#if CVLIB_IMG_SUPPORT_PGX
			CVLIB_IMG_FORMAT_PGX,
#endif
#if CVLIB_IMG_SUPPORT_PNM
			CVLIB_IMG_FORMAT_PNM,
#endif
#if CVLIB_IMG_SUPPORT_RAS
			CVLIB_IMG_FORMAT_RAS,
#endif
			CVLIB_IMG_MAX_FORMATS
};

/**
 * @brief  °£³àÂÙ Ãùº¬½å¶®¶¦ ¼êÃÔÂ×²÷ ¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC CoImage : public Object
{
public:

	/**
	 * @brief  Ãùº¬¼³¸ó¶¦ ¸ÉÊéÂ×²÷ °é¼¿¾Ë
	 */
	typedef struct tagCoImageInfo 
	{
		DWORD	dwEffWidth;			
		BYTE*	pImage;				
		CoImage* pGhost;			
		CoImage* pParent;			
		DWORD	dwType;				
		char	szLastError[256];	
		long	nProgress;			
		long	nEscape;			
		long	nBkgndIndex;		
		RGBQUAD1 nBkgndColor;		
		BYTE	nQuality;			
		BYTE	nJpegScale;			
		long	nFrame;				
		long	nNumFrames;			
		DWORD	dwFrameDelay;		
		long	xDPI;				
		long	yDPI;				
//		RECT	rSelectionBox;		
		BYTE	nAlphaMax;			
		bool	bAlphaPaletteEnabled; 
		bool	bEnabled;			
		long	xOffset;
		long	yOffset;
		DWORD	dwCodecOpt[CVLIB_IMG_MAX_FORMATS];	
		RGBQUAD1 last_c;				
		BYTE	last_c_index;
		bool	last_c_isvalid;
		long	nNumLayers;
		DWORD	dwFlags;			
	} COIMAGEINFO;
	
	/**
	 * @brief  »°°é¼¿¾Ë 
	 */
	struct rgb_color 
	{
		BYTE r;  /**< »°Ì© rºÂ¹¤°ª */
		BYTE g;  /**< »°Ì© gºÂ¹¤°ª */
		BYTE b;  /**< »°Ì© bºÂ¹¤°ª */
	};

	//! »°¸Ò»¥
	enum 
	{
		enGRAY_MODE = 1,	/**< °û¶ÁËË¸Ò»¥ */
		enCOLOR_MODE = 3	/**< »°Ãùº¬¸Ò»¥ */
	};
	
	Mat		m_matX, m_matY, m_matZ, m_matA, m_matG;
	BITMAPINFOHEADER1	m_Head;
protected:
	int		m_nRows, m_nCols, m_nChannel;
	SString	m_strName;
	Mat**   m_ppBands;
public:
	//! Construction and Destruction
	CoImage ();
	CoImage (const CoImage& img);
	CoImage (const Mat& mImg, BOOL fCopy = FALSE);
	CoImage (const Mat& mX, const Mat& mY, const Mat& mZ);
	CoImage (int nRows, int nCols, int nMode, int nChannel=enCOLOR_MODE);
	CoImage (BITMAPINFOHEADER1* pBI);
	CoImage (char* szFilename, DWORD imagetype=0);
	CoImage (const CoImage& img, const Rect& region);

	virtual ~CoImage ();
	operator Mat() const;

	//! Initialization
	int		Create (const CoImage& img, BOOL fCopy = FALSE);
	int		Create (const Mat& mImg, BOOL fCopy = FALSE);
	int		Create (const Mat& mX, const Mat& mY, const Mat& mZ);
	int		Create (int nRows, int nCols, int nMode, int nChannel=enCOLOR_MODE);
	int		Create (BITMAPINFOHEADER1* pBI);
	int		CreateFromArray (BYTE* pbData, int nRows, int nCols, int nBitsperpixel, int nBytesperline, bool bFlipImage=false);

	void	Release ();
	CoImage& operator=(const CoImage& ImageSrc);
	BYTE*	GetBMPInfoHeader() const;

	//! Property
	inline int		GetWidth() const {return m_nCols;}
	inline int		GetHeight() const {return m_nRows;}
	inline int		GetChannel() const {return m_nChannel;}
	inline Size		SizeObject() const {return Size(m_nCols, m_nRows);}
	inline bool		IsValid () const { return m_matX.IsValid();}
	inline bool		IsInterior (int x, int y) const {return (x<m_nCols && x>=0 && y<m_nRows && y>=0);}
	inline bool		IsEqual(const CoImage& t) const {return m_matX.IsEqualMat(t.m_matX) && m_nChannel==t.m_nChannel;}

	bool	Load (const char* szFilename, DWORD nImageType = 0);
	bool	Save (const char* szFilename, DWORD nImageType=CVLIB_IMG_FORMAT_BMP);
	bool	Decode (FILE* pFile, DWORD nImageType);
	bool	Encode (FILE* pFile, DWORD nImageType) const;
	BOOL	CheckGrayMode ();
	CoImage*	YCbCr (CoImage* pDest = NULL);

	Mat*	GetGrayMat ();
	void	Gray (Mat* pmGray = NULL);
	bool	Crop (Rect* pRect, CoImage* pDest = NULL);

	// Simple Drawing Functions
	void	DrawImage (const CoImage& img, int nX = 0, int nY = 0, const float rOpacity=1);
	void	DrawRect (Rect* pRect, COLORREF color, const float rOpacity=1);
	void	DrawEllipse (int nLeft, int nTop, int nRight, int nBottom, COLORREF color, const float rOpacity=1);
	void	DrawEllipse (Rect* pRect, COLORREF color, const float rOpacity=1);
	void	DrawEllipse (const RotatedRect& rect, COLORREF color, const float rOpacity=1);
	void	DrawPixel (int nX, int nY, COLORREF color, const float rOpacity=1);
	void	DrawPixel (const SPoint& point, COLORREF color, const float rOpacity=1);
	void	DrawLine (int nX0, int nY0, int nX1, int nY1, COLORREF color, const float rOpacity=1);
	void	DrawLine (const SPoint& pt1, const SPoint& pt2, COLORREF color, const float rOpacity=1);
	void	DrawCross (int nX, int nY, int nSide, COLORREF color, const float rOpacity=1);
	void	DrawCross (const SPoint& pt, int nSide, COLORREF color, const float rOpacity=1);
	void	DrawString(long x, long y, const char* szText, COLORREF color, const char* szFont,
				long lSize, long lWeight, BYTE bItalic, BYTE bUnderline, const float rOpacity = 1);
	static void	Line_pt(int nX0, int nY0, int nX1, int nY1, int* &pnX, int* &pnY, int &nNum);

	BYTE		GetJpegQuality() const;
	void		SetJpegQuality(BYTE q);
	DWORD		GetCodecOption(DWORD imagetype = 0) const;
	bool		SetCodecOption(DWORD opt, DWORD imagetype = 0);
	void		Startup(DWORD imagetype = 0);

	BYTE		GetPixelIndex(BYTE* pData,long x) const;
	BYTE		GetPixelIndex(long x,long y) const;
	RGBQUAD1	GetPixelColor(long x,long y, bool fGetAlpha = true) const;
	void		SetPixelColor(long x,long y,RGBQUAD1 c, bool fSetAlpha = false);
	void		SetPixelColor(long x,long y,COLORREF cr);
	void		SetPixelIndex(long x,long y,BYTE i);
	RGBQUAD1	RGBtoRGBQUAD(COLORREF cr) const;
	DWORD		GetPaletteSize() const;
	RGBQUAD1*	GetPalette() const;
	int			GetNumColors() const {return m_Head.biClrUsed;}
	RGBQUAD1	GetPaletteColor(BYTE idx) const;
	bool		GetPaletteColor(BYTE i, BYTE* r, BYTE* g, BYTE* b) const;
	BYTE		GetNearestIndex(RGBQUAD1 c);
	void		BlendPalette(COLORREF cr,long perc) const;
	void		SetGrayPalette();
	void		SetPalette(DWORD n, BYTE *r, BYTE *g, BYTE *b);
	void		SetPalette(RGBQUAD1* pPal,DWORD nColors=256);
	void		SetPalette(rgb_color *rgb,DWORD nColors=256);
	void		SetPaletteColor(BYTE idx, BYTE r, BYTE g, BYTE b, BYTE alpha=0);
	void		SetPaletteColor(BYTE idx, RGBQUAD1 c);
	void		SetPaletteColor(BYTE idx, COLORREF cr);
	int			GetEffWidth() const {return m_Info.dwEffWidth;}
	
	void	  SetBpp24();

	void	  SetBkColor(COLORREF color);

	bool	  Flip(BOOL fVertical = TRUE);
	bool	  Mirror();
	bool	  Negative();
	double	  Mean() const;

	TYPE	  GetType () const {return m_matX.Type();};
	RGBQUAD1  GetTransColor() const;
	void	  SetTransColor(RGBQUAD1 rgb);

	void	SetName(const char *name);
	const char * Name(bool fIncludePath=false) const;
	Mat**	GetBand() const { return m_ppBands;}
	void GetPixelColor (float x, float y, float * result, int nChannel) const;
	void SetPixelColor(long x,long y,float* color);



#if CVLIB_IMG_SUPPORT_ALPHA
	/** \addtogroup Alpha */ //@{
	void AlphaCreate();
 	void AlphaDelete();
 	void AlphaClear();
 	void AlphaInvert();
 	bool AlphaMirror();
 	bool AlphaFlip();
 	bool AlphaCopy(CoImage &from);
// 	bool AlphaSplit(CoImage *dest);
// 	void AlphaStrip();
 	void AlphaSet(BYTE level);
// 	bool AlphaSet(CxImage &from);
 	void AlphaSet(const long x,const long y,const BYTE level);
 	BYTE AlphaGet(const long x,const long y);
// 	BYTE AlphaGetMax() const;
// 	void AlphaSetMax(BYTE nAlphaMax);
 	bool AlphaIsValid();
 	BYTE* AlphaGetPointer(const long x = 0,const long y = 0);
// 	
// 	void AlphaPaletteClear();
// 	void AlphaPaletteEnable(bool enable=true);
// 	bool AlphaPaletteIsEnabled();
// 	bool AlphaPaletteIsValid();
// 	bool AlphaPaletteSplit(CxImage *dest);
	//@}
#endif //CVLIB_IMG_SUPPORT_ALPHA

#if CVLIB_OS==CVLIB_OS_WIN32
	void	  Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX, int nDstCY,
					int nSrcX, int nSrcY, int nSrcCX, int nSrcCY) const;
	void	  Draw(DWORD hDC, int nDstX, int nDstY, int nDstCX=-1, int nDstCY=-1) const;
	void	  Draw(DWORD hDC) const;
	int		  DrawString(DWORD hdc, long x, long y, const char* szText, COLORREF color, const char* szFont,
					long lSize, long lWeight, BYTE bItalic, BYTE bUnderline, const float rOpacity = 1);
	int		  CreateFromHBITMAP(DWORD hBitmap, DWORD hPalette = 0);
	bool	  LoadResource(DWORD hRes, DWORD imagetype, DWORD hModule=NULL);
	bool	  Tile(DWORD hdc, Rect* rc);
	DWORD	  MakeBitmap (DWORD hDC=0);
	bool	  CreateFromHANDLE(DWORD hMem);
	DWORD	  CopyToHandle();
#endif

protected:
	bool EncodeSafeCheck(FILE* pFile);
	void InitBMPHeader(int nW, int nH);
	
	int	 CreateInfo (int nRows, int nCols, int wBpp, int nImageType);
	long GetXDPI() const;
	long GetYDPI() const;
	void SetXDPI(long dpi);
	void SetYDPI(long dpi);
	void RGBtoBGR(BYTE *buffer, int length);

	COIMAGEINFO	m_Info; //extended information
	void* m_pPalette;

};

}

#if CVLIB_OS == CVLIB_OS_WIN32
#ifndef CVLIB_IMG_NOCODEC

#ifdef NDEBUG
#if CVLIB_IMG_SUPPORT_J2K
#pragma comment(lib,"j2k.lib")
#endif //CVLIB_IMG_SUPPORT_J2K
#if CVLIB_IMG_SUPPORT_JAS
#pragma comment(lib,"jasper.lib")
#endif //CVLIB_IMG_SUPPORT_JAS
#if CVLIB_IMG_SUPPORT_JBG
#pragma comment(lib,"jbig.lib")
#endif //CVLIB_IMG_SUPPORT_JBG
#if CVLIB_IMG_SUPPORT_JPG
#pragma comment(lib,"jpeg.lib")
#endif //CVLIB_IMG_SUPPORT_JPG
#if CVLIB_IMG_SUPPORT_PNG
#pragma comment(lib,"png.lib")
#endif //CVLIB_IMG_SUPPORT_PNG
#if CVLIB_IMG_SUPPORT_TIF
#pragma comment(lib,"tiff.lib")
#endif //CVLIB_IMG_SUPPORT_TIF
#if CVLIB_IMG_SUPPORT_PNG || CVLIB_IMG_SUPPORT_TIF
#pragma comment(lib,"zlib.lib")
#endif //CVLIB_IMG_SUPPORT_PNG || CVLIB_IMG_SUPPORT_TIF
#else
#if CVLIB_IMG_SUPPORT_J2K
#pragma comment(lib,"j2kd.lib")
#endif //CVLIB_IMG_SUPPORT_J2K
#if CVLIB_IMG_SUPPORT_JAS
#pragma comment(lib,"jasperd.lib")
#endif //CVLIB_IMG_SUPPORT_JAS
#if CVLIB_IMG_SUPPORT_JBG
#pragma comment(lib,"jbigd.lib")
#endif //CVLIB_IMG_SUPPORT_JBG
#if CVLIB_IMG_SUPPORT_JPG
#pragma comment(lib,"jpegd.lib")
#endif //CVLIB_IMG_SUPPORT_JPG
#if CVLIB_IMG_SUPPORT_PNG
#pragma comment(lib,"pngd.lib")
#endif //CVLIB_IMG_SUPPORT_PNG
#if CVLIB_IMG_SUPPORT_TIF
#pragma comment(lib,"tiffd.lib")
#endif //CVLIB_IMG_SUPPORT_TIF
#if CVLIB_IMG_SUPPORT_PNG || CVLIB_IMG_SUPPORT_TIF
#pragma comment(lib,"zlibd.lib")
#endif //CVLIB_IMG_SUPPORT_PNG || CVLIB_IMG_SUPPORT_TIF
#endif //NDEBUG

#endif //CVLIB_IMG_NOCODEC
#endif

#endif //_COIMAGE_H_