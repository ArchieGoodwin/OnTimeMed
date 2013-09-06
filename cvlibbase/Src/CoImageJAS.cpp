
/*! 
 * \file	CoImageJAS.cpp
 * \ingroup base
 * \brief	JasperÂô»¥Ì© Ãùº¬¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	¼³Ë¼Êé
 */

#include "CoImageJAS.h"

#ifndef CVLIB_IMG_NOCODEC

#if CVLIB_IMG_SUPPORT_JAS

#include "XFile.h"
#include "XFileDisk.h"
#include "ImageIter.h"

#include "jasper\jasper.h"

namespace CVLib 
{
	class CxFileJas
	{
	public:
		CxFileJas(XFile* pFile,jas_stream_t *stream)
		{
			if (stream->obj_) jas_free(stream->obj_);
			stream->obj_ = pFile;
			
			// <vho> - cannot set the stream->ops_->functions here,
			// because this overwrites a static structure in the Jasper library.
			// This structure is used by Jasper for internal operations too, e.g. tempfile.
			// However the ops_ pointer in the stream can be overwritten.
			
			//stream->ops_->close_ = JasClose;
			//stream->ops_->read_  = JasRead;
			//stream->ops_->seek_  = JasSeek;
			//stream->ops_->write_ = JasWrite;
			
			jas_stream_CxFile.close_ = JasClose;
			jas_stream_CxFile.read_  = JasRead;
			jas_stream_CxFile.seek_  = JasSeek;
			jas_stream_CxFile.write_ = JasWrite;
			
			stream->ops_ = &jas_stream_CxFile;
			
			// <vho> - end
		}
		static int JasRead(jas_stream_obj_t *obj, char *buf, int cnt)
		{		return ((XFile*)obj)->Read(buf,1,cnt); }
		static int JasWrite(jas_stream_obj_t *obj, char *buf, int cnt)
		{		return ((XFile*)obj)->Write(buf,1,cnt); }
		static long JasSeek(jas_stream_obj_t *obj, long offset, int origin)
		{		return ((XFile*)obj)->Seek(offset,origin); }
		static int JasClose(jas_stream_obj_t *obj)
		{		return 1; }
	private:
		jas_stream_ops_t jas_stream_CxFile;
	};


CoImageJAS::CoImageJAS()
{
}

bool CoImageJAS::Decode (FILE* pFile, DWORD nImagetype)
{
	if (pFile == NULL)
		return false;

	XFileDisk hFile(pFile);
	jas_image_t *image=0;
	jas_stream_t *in=0;
	jas_matrix_t **bufs=0;
	long i,error=0;
	//jas_setdbglevel(0);

  try
  {
	if (jas_init())
		throw "cannot initialize jasper";

	if (!(in = jas_stream_fdopen(0, "rb")))
		throw "error: cannot open standard input";

	CxFileJas src(&hFile,in);

	if (!(image = jas_image_decode(in, -1, 0))) 
		throw "error: cannot load image data";

	long x,y,w,h,depth,cmptno;

	w = jas_image_cmptwidth(image,0);
	h = jas_image_cmptheight(image,0);
	depth = jas_image_cmptprec(image,0);

	if (image->numcmpts_ > 64 || image->numcmpts_ < 0)
		throw "error: too much components";

	if (depth!=1 && depth!=4 && depth!=8)
	{
		jas_image_t *newimage;
		jas_cmprof_t *outprof;
		//jas_eprintf("forcing conversion to sRGB\n");
		if (!(outprof = jas_cmprof_createfromclrspc(JAS_CLRSPC_SRGB))) 
		{
			throw "cannot create sRGB profile";
		}
		if (!(newimage = jas_image_chclrspc(image, outprof, JAS_CMXFORM_INTENT_PER))) 
		{
			throw "cannot convert to sRGB";
		}
		jas_image_destroy(image);
		jas_cmprof_destroy(outprof);
		image = newimage;
	}

	bufs = (jas_matrix_t **)calloc(image->numcmpts_, sizeof(jas_matrix_t**));
	for (i = 0; i < image->numcmpts_; ++i) 
	{
		if (!(bufs[i] = jas_matrix_create(1, w))) 
		{
			throw "error: cannot allocate memory";
		}
	}

	if (image->numcmpts_==3 &&
		image->cmpts_[0]->width_ == image->cmpts_[1]->width_ &&
		image->cmpts_[1]->width_ == image->cmpts_[2]->width_ &&
		image->cmpts_[0]->height_ == image->cmpts_[1]->height_ &&
		image->cmpts_[1]->height_ == image->cmpts_[2]->height_ &&
		image->cmpts_[0]->prec_  == image->cmpts_[1]->prec_ &&
		image->cmpts_[1]->prec_ == image->cmpts_[2]->prec_ )
	{

		if(!CreateInfo(h,w, 24,nImagetype))
			throw "Can't allocate memory";

		if (m_Info.pImage == NULL)
			m_Info.pImage = (BYTE*)malloc (m_Head.biSizeImage);
		RGBQUAD1 c;
        for (y=0; y<h; y++) 
		{
			for (cmptno = 0; cmptno < image->numcmpts_; ++cmptno) 
			{
				jas_image_readcmpt(image, cmptno, 0, y, w, 1, bufs[cmptno]);
			}

			for (x=0; x<w; x++)
			{
				c.rgbRed   = (jas_matrix_getv(bufs[0], x));
				c.rgbGreen = (jas_matrix_getv(bufs[1], x));
				c.rgbBlue  = (jas_matrix_getv(bufs[2], x));
				SetPixelColor(x,y,c);
			}
		}

		if (m_Info.pImage)
			free (m_Info.pImage);
		m_Info.pImage = NULL;
	} 
	else 
	{
		m_Info.nNumFrames = image->numcmpts_;
		if ((m_Info.nFrame<0)||(m_Info.nFrame>=m_Info.nNumFrames))
		{
			throw "wrong frame!";
		}
		for (cmptno=0; cmptno<=m_Info.nFrame; cmptno++) 
		{
			w = jas_image_cmptwidth(image,cmptno);
			h = jas_image_cmptheight(image,cmptno);
			depth = jas_image_cmptprec(image,cmptno);
			if (depth>8) depth=8;
			if(!CreateInfo(h,w,depth,nImagetype))
				throw "Can't allocate memory";
			if (m_Info.pImage == NULL)
				m_Info.pImage = (BYTE*)malloc (m_Head.biSizeImage);
			SetGrayPalette();
			for (y=0; y<h; y++) 
			{
				jas_image_readcmpt(image, cmptno, 0, y, w, 1, bufs[0]);
				for (x=0; x<w; x++)
				{
					SetPixelIndex(x,h-1-y,(jas_matrix_getv(bufs[0], x)));
				}
			}

			ImageIterator iter(this);
			iter.BMP2XYZ(m_Info.pImage);
			if (m_Info.pImage)
				free (m_Info.pImage);
			m_Info.pImage = NULL;

			m_Info.dwEffWidth = ((((24 * m_Head.biWidth) + 31) / 32) * 4);
			m_Head.biSizeImage = m_Info.dwEffWidth * m_Head.biHeight;
		}
	}
  }
  catch (char *message) 
  {
	strncpy(m_Info.szLastError,message,255);
	error = 1;
  }

	if (bufs) 
	{
		for (i = 0; i < image->numcmpts_; ++i)
		{	
			if (bufs[i]) jas_matrix_destroy(bufs[i]);
		}
		free(bufs);
	}
	jas_cleanup();
	if (image) jas_image_destroy(image);
	if (in) jas_stream_close(in);
	return (error==0);
}
	
#if CVLIB_IMG_SUPPORT_ENCODE
bool CoImageJAS::Encode(FILE * pFile, DWORD nImagetype)
{
	XFileDisk hFile(pFile);

	if (EncodeSafeCheck(pFile)) return false;

	if (m_Head.biClrUsed!=0 && !CheckGrayMode())
	{
		strcpy(m_Info.szLastError,"JasPer can save only RGB or GrayScale images");
		return false;
	}

	jas_image_t *image=0;
	jas_stream_t *out=0;
	jas_matrix_t *cmpts[3];
	long x,y,yflip,error=0;
	uint_fast16_t cmptno, numcmpts;
	jas_image_cmptparm_t cmptparms[3], *cmptparm;

  try
  {

	if (jas_init())
		throw "cannot initialize jasper";

	if (!(out = jas_stream_fdopen(0, "wb")))
		throw "error: cannot open standard output";

	CxFileJas src(&hFile,out);

	numcmpts = m_Head.biClrUsed==0 ? 3 : 1;

	for (cmptno = 0, cmptparm = cmptparms; cmptno < numcmpts; ++cmptno, ++cmptparm) 
	{
		cmptparm->tlx = 0;
		cmptparm->tly = 0;
		cmptparm->hstep = 1;
		cmptparm->vstep = 1;
		cmptparm->width = m_Head.biWidth;
		cmptparm->height = m_Head.biHeight;
		cmptparm->prec = 8;
		cmptparm->sgnd = false;
	}

	/* Create image object. */
	if (!(image = jas_image_create(numcmpts, cmptparms, JAS_CLRSPC_UNKNOWN)))
		throw "error : jas_image_create";

	if (numcmpts == 3) 
	{
		jas_image_setclrspc(image, JAS_CLRSPC_SRGB);
		jas_image_setcmpttype(image, 0,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_R));
		jas_image_setcmpttype(image, 1,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_G));
		jas_image_setcmpttype(image, 2,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_B));
	}
	else 
	{
		jas_image_setclrspc(image, JAS_CLRSPC_SGRAY);
		jas_image_setcmpttype(image, 0,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_GRAY_Y));
	}

	for (x = 0; x < numcmpts; ++x) { cmpts[x] = 0; }
	/* Create temporary matrices to hold component data. */
	for (x = 0; x < numcmpts; ++x) 
	{
		if (!(cmpts[x] = jas_matrix_create(1, m_Head.biWidth))) 
		{
			throw "error : can't allocate memory";
		}
	}

	RGBQUAD1 c;
	for (y = 0; y < m_Head.biHeight; ++y) 
	{
		for (x = 0; x < m_Head.biWidth; ++x) 
		{
			if (m_Head.biClrUsed==0)
			{
				c = GetPixelColor(x,y);
				jas_matrix_setv(cmpts[0], x, c.rgbRed);
				jas_matrix_setv(cmpts[1], x, c.rgbGreen);
				jas_matrix_setv(cmpts[2], x, c.rgbBlue);
			}
			else
			{
				jas_matrix_setv(cmpts[0], x, GetPixelIndex(x,y));
			}
		}
		yflip = /*m_Head.biHeight - 1 - */y;
		for (cmptno = 0; cmptno < numcmpts; ++cmptno) 
		{
			if (jas_image_writecmpt(image, cmptno, 0, yflip, m_Head.biWidth, 1, cmpts[cmptno])) 
			{
				throw "error : jas_image_writecmpt";
			}
		}
	}

	 char szfmt[4];
	*szfmt = '\0';
#if CVLIB_IMG_SUPPORT_JP2
	if (nImagetype == CVLIB_IMG_FORMAT_JP2) strcpy(szfmt,"jp2");
#endif
#if CVLIB_IMG_SUPPORT_JPC
	if (nImagetype == CVLIB_IMG_FORMAT_JPC) strcpy(szfmt,"jpc");
#endif
#if CVLIB_IMG_SUPPORT_RAS
	if (nImagetype == CVLIB_IMG_FORMAT_RAS) strcpy(szfmt,"ras");
#endif
#if CVLIB_IMG_SUPPORT_PNM
	if (nImagetype == CVLIB_IMG_FORMAT_PNM) strcpy(szfmt,"pnm");
#endif
#if CVLIB_IMG_SUPPORT_PGX
	if (nImagetype == CVLIB_IMG_FORMAT_PGX)
	{
		strcpy(szfmt,"pgx");
		if (m_Head.biClrUsed==0) throw "PGX can save only GrayScale images";
	}
#endif
	int outfmt = jas_image_strtofmt(szfmt);

	char szoutopts[16];
	sprintf(szoutopts,"rate=%.3f", m_Info.nQuality/100.0f);

	if (jas_image_encode(image, out, outfmt, szoutopts)) 
	{
		throw "error: cannot encode image\n";
	}
	jas_stream_flush(out);

  } 
  catch (char *message) 
  {
	strncpy(m_Info.szLastError,message,255);
	error = 1;
  }

	for (x = 0; x < numcmpts; ++x) 
	{ 
		if (cmpts[x]) 
			jas_matrix_destroy(cmpts[x]); 
	} 
	jas_cleanup();
	if (image) jas_image_destroy(image);
	if (out) jas_stream_close(out);

	return (error==0);
}
#endif // COIMAGE_SUPPORT_ENCODE

}

#endif //CVLIB_IMG_SUPPORT_JAS

#endif//CVLIB_IMG_NOCODEC