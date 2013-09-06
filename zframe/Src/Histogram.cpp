
/*!
 * \file
 * \brief ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®Âî½è
 */
#include "Histogram.h"
#include "cvlibbase/inc/template.h"
#include "colorspace.h"

namespace CVLib {

static const int OUT_OF_RANGE = (int)1 << (sizeof(int)*8 - 2);

/**
 @brief ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×²÷ ÂÜºãËË³Þ.
 
 @param pIn			[in]:	ËÓµÙÃÔµÛ, Ãùº¬
 @param pOutHisto	[out]:	ÃÅº÷ÀË°ûµ½
 */
void
Histogram::Process (Mat* pIn, Vec* pOutHisto)
{
	assert (pIn->Type() == MAT_Tbyte);
	assert (pOutHisto->IsValid());

	int nH = pIn->Rows();
	int nW = pIn->Cols();
	int i;
	int	nPixel, nInSize, nOutSize;
	BYTE*	pbData = pIn->data.ptr[0];

	nInSize = nH * nW;
	nOutSize = GRAY_LEVEL_NUMBER;

	pOutHisto->Zero();

	switch (pOutHisto->Type())
	{
	case MAT_Tfloat:
		{
			for (i = 0; i < nInSize; i ++)
			{
				nPixel = pbData[i];
				if (nPixel > MAX_8BIT_VALUE)	
					nPixel = MAX_8BIT_VALUE;
				else if (nPixel < 0)
					nPixel = 0;

				pOutHisto->data.fl[nPixel] += 1.0f;
			}
				
			if (fNormalize)
			{
				for (i = 0; i < nOutSize; i ++)
				{
					int	nValue = (int)pOutHisto->data.fl[i];
					if (nValue != 0)
						pOutHisto->data.fl[i] /= nInSize;
				}
			}
		}
		break;
	case MAT_Tint:
		{
			for (i = 0; i < nInSize; i ++)
			{
				nPixel = pbData[i];
				if (nPixel > MAX_8BIT_VALUE)	
					nPixel = MAX_8BIT_VALUE;
				else if (nPixel < 0)
					nPixel = 0;

				pOutHisto->data.i[nPixel] ++;
			}
		}
		break;
        default:
            assert(false);
	}
}

void	Histogram::RGBHist (CoImage* pImage, ColorBin* pBin, Vec* pvHist)
{
#define GRAYLEVEL 256
	assert (pImage->GetChannel() == CoImage::enCOLOR_MODE);
	assert (pvHist->IsValid());
	if (fNormalize)
		assert (pvHist->Type() == MAT_Tfloat);
	else
		assert (pvHist->Type() == MAT_Tint);

	BYTE* pbX = pImage->m_matX.data.ptr[0];
	BYTE* pbY = pImage->m_matY.data.ptr[0];
	BYTE* pbZ = pImage->m_matZ.data.ptr[0];
	int		nPixelNum = pImage->GetHeight() * pImage->GetWidth();
	int	nUX = (GRAYLEVEL + pBin->nBinX - 1) / pBin->nBinX;
	int	nUY = (GRAYLEVEL + pBin->nBinY - 1) / pBin->nBinY;
	int	nUZ = (GRAYLEVEL + pBin->nBinZ - 1) / pBin->nBinZ;

	int nBinXY = pBin->nBinX * pBin->nBinY;
	int	nBins = pBin->Bins();
	assert (nBins <= pvHist->Length());

	int i;
	pvHist->Zero();
	if (fNormalize)
	{
		float* prHist = pvHist->data.fl;
		for (i = 0; i < nPixelNum; i ++)
		{
			int iBin = pbX[i] / nUX + pbY[i] / nUY * pBin->nBinX + pbZ[i] / nUZ * nBinXY;
			prHist[iBin] += 1.0f;
		}
		for (i = 0; i < pvHist->Length(); i ++)
		{
			prHist[i] /= (float)nPixelNum;
		}
	}
	else
	{
		int* pnHist = pvHist->data.i;
		for (i = 0; i < nPixelNum; i ++)
		{
			int iBin = pbX[i] / nUX + pbY[i] / nUY * pBin->nBinX + pbZ[i] / nUZ * nBinXY;
			pnHist[iBin] ++;
		}
	}
}

float	Histogram::DistanceL1 (Vec* pvHist1, Vec* pvHist2)
{
	assert (pvHist1->Length() == pvHist2->Length());
	assert (pvHist1->Type() == MAT_Tfloat);
	int nLen = pvHist1->Length();
	float rL1 = 0.0f;
	float* prHist1 = pvHist1->data.fl;
	float* prHist2 = pvHist2->data.fl;
	for (int i = 0 ; i < nLen; i ++)
		rL1 += (float)fabs (prHist1[i] - prHist2[i]);

	return rL1;
}

float	Histogram::Bhattacharyya (Vec* pvHist1, Vec* pvHist2)
{
	assert (pvHist1->Length() == pvHist2->Length());
	assert (pvHist1->Type() == MAT_Tfloat);
	int nLen = pvHist1->Length();
	float rDist = 0.0f;
	float* prHist1 = pvHist1->data.fl;
	float* prHist2 = pvHist2->data.fl;
	for (int i = 0 ; i < nLen; i ++)
		rDist += (float)sqrt (prHist1[i] * prHist2[i]);
	
	return (float)sqrt (1.0f - rDist);
}

/**
 @brief ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×?EÂÜ?E
 
 @param pImg		[in]:	ËÓµÙÃùº¬
 @param pOutHisto	[out]:	ÃÅº÷ÀË°ûµ½
 @param nQuantStep	[in]:	¹ÀÌ© ¿Í±¨
 */
void
Histogram::	Process(CoImage* pImg, Vec* pOutHisto, int nQuantStep) // ajh
{
	Mat* pMatX = &pImg->m_matX;
	Mat* pMatY = &pImg->m_matY;
	Mat* pMatZ = &pImg->m_matZ;
	
	assert(pMatX->Type() == MAT_Tbyte && pMatY->Type() == MAT_Tbyte && pMatZ->Type() == MAT_Tbyte);
	assert (pOutHisto->IsValid());
	assert (pOutHisto->Type() == MAT_Tfloat);
	
	int nH = pImg->GetHeight();
	int nW = pImg->GetWidth();
	
	BYTE* pbDataX = pMatX->data.ptr[0];
	BYTE* pbDataY = pMatY->data.ptr[0];
	BYTE* pbDataZ = pMatZ->data.ptr[0];
	
    int nImgSize = nH * nW;
	int nPixelX, nPixelY, nPixelZ;
	int nBinNum;
	int nBin = GRAY_LEVEL_NUMBER / nQuantStep;   // ·ô±¶ ºÂ¹¤Ì© bin ±¶?E
	
	pOutHisto->Zero();
	
	for ( int i = 0 ; i < nImgSize ;  i ++)
	{
		nPixelX = pbDataX[i] / nQuantStep;
		nPixelY = pbDataY[i] / nQuantStep;
		nPixelZ = pbDataZ[i] / nQuantStep;
		
		nBinNum = nPixelX * nBin * nBin + nPixelY * nBin + nPixelZ;
		pOutHisto->data.fl[nBinNum] ++;		
	}
	
	for ( int Bin = 0 ; Bin < 512 ; Bin ++)
	{
		pOutHisto->data.fl[Bin] /= (float)nImgSize;
	}
}

int Histogram::Process(CoImage* pSrc, int* red, int* green, int* blue, int* gray)
{
	if (red) memset(red,0,256*sizeof(int));
	if (green) memset(green,0,256*sizeof(int));
	if (blue) memset(blue,0,256*sizeof(int));
	if (gray) memset(gray,0,256*sizeof(int));

	if (gray)
		pSrc->Gray();
	
	int xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pSrc->GetWidth(); ymax=pSrc->GetHeight();
	
	for(int y=ymin; y<ymax; y++)
	{
		for(int x=xmin; x<xmax; x++)
		{
			if (red) red[pSrc->m_matX.data.ptr[y][x]]++;
			if (green) green[pSrc->m_matY.data.ptr[y][x]]++;
			if (blue) blue[pSrc->m_matZ.data.ptr[y][x]]++;
			if (gray) gray[pSrc->m_matG.data.ptr[y][x]]++;
		}
	}
	
	int n=0;
	for (int i=0; i<256; i++){
		if (red && red[i]>n) n=red[i];
		if (green && green[i]>n) n=green[i];
		if (blue && blue[i]>n) n=blue[i];
		if (gray && gray[i]>n) n=gray[i];
	}
	
	return n;
}
int Histogram::Process(Mat* pSrc, int* red, int* green, int* blue, int* gray)
{
	if (red) memset(red,0,256*sizeof(int));
	if (green) memset(green,0,256*sizeof(int));
	if (blue) memset(blue,0,256*sizeof(int));
	if (gray) memset(gray,0,256*sizeof(int));

	Mat mGray;
	if (gray)
		ColorSpace::RGBtoGray(*pSrc, mGray);
	
	int xmin,xmax,ymin,ymax;
	xmin = ymin = 0;
	xmax = pSrc->Cols(); ymax=pSrc->Rows();
	int cn=pSrc->Channels();
	for(int y=ymin; y<ymax; y++)
	{
		uchar* pbsrc=pSrc->data.ptr[y];
		for(int x=xmin, k=xmin*cn; x<xmax; x++,k+=cn)
		{
			if (red) red[pbsrc[k]]++;
			if (green) green[pbsrc[k+1]]++;
			if (blue) blue[pbsrc[k+2]]++;
			if (gray) gray[pbsrc[x]]++;
		}
	}
	
	int n=0;
	for (int i=0; i<256; i++){
		if (red && red[i]>n) n=red[i];
		if (green && green[i]>n) n=green[i];
		if (blue && blue[i]>n) n=blue[i];
		if (gray && gray[i]>n) n=gray[i];
	}
	
	return n;
}
/**
 @brief ¼è¼³´ô µßÊßËæº· ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×²÷ ÂÜºã
 
 @param pImg		[in]:	ËÓµÙÃùº¬
 @param pOutHisto	[out]:	ÃÅº÷ÀË°ûµ½
 @param nQuantStep	[in]:	¹ÀÌ© ¿Í±¨
 */
void
Histogram::	Process(CoImage* pImg, Rect rect,Vec* pOutHisto, int nQuantStep)
{
	if ( m_fRegionStyle )
		Process_Rect(pImg, rect, pOutHisto, nQuantStep);
	else
		Process_Ellipse(pImg, rect, pOutHisto, nQuantStep);
}

																				   
/**
 @brief 4°¢ÂôµßÊßËæº· ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×²÷ ÂÜºã
 
 @param pImg		[in]:	ËÓµÙÃùº¬
 @param pOutHisto	[out]:	ÃÅº÷ÀË°ûµ½
 @param nQuantStep	[in]:	¹ÀÌ© ¿Í±¨
 */
void
Histogram::	Process_Rect(CoImage* pImg, Rect rect,Vec* pOutHisto, int nQuantStep)
{
	Mat* pMatX = &pImg->m_matX;
	Mat* pMatY = &pImg->m_matY;
	Mat* pMatZ = &pImg->m_matZ;
	
	assert(pMatX->Type() == MAT_Tbyte && pMatY->Type() == MAT_Tbyte && pMatZ->Type() == MAT_Tbyte);
	assert (pOutHisto->IsValid());
	assert (pOutHisto->Type() == MAT_Tfloat);
	
	int nH = rect.height;
	int nW = rect.width;
	
	BYTE* pbDataX = pMatX->data.ptr[0];
	BYTE* pbDataY = pMatY->data.ptr[0];
	BYTE* pbDataZ = pMatZ->data.ptr[0];
	
    int nImgSize = nH * nW;
	int nPixelX, nPixelY, nPixelZ;
	int nBinNum;
	int nBin = GRAY_LEVEL_NUMBER / nQuantStep;   // ·ô±¶ ºÂ¹¤Ì© bin ±¶ºã 
	
	pOutHisto->Zero();
	
	for ( int i = rect.y ; i < rect.y + rect.height ;  i ++)
	{
		for( int j = rect.x; j < rect.x + rect.width ; j ++)
		{
			nPixelX = pbDataX[i * pImg->GetWidth() + j] / nQuantStep;
			nPixelY = pbDataY[i * pImg->GetWidth() + j] / nQuantStep;
			nPixelZ = pbDataZ[i * pImg->GetWidth() + j] / nQuantStep;
			
			nBinNum = nPixelX * nBin * nBin + nPixelY * nBin + nPixelZ;
			pOutHisto->data.fl[nBinNum] ++;	
		}
	}
	
	for ( int Bin = 0 ; Bin < 512 ; Bin ++)
	{
		pOutHisto->data.fl[Bin] /= (float)nImgSize;
	}
}

/**
 @brief À°ÌºµßÊßËæº· ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×²÷ ÂÜºã
 
 @param pImg		[in]:	ËÓµÙÃùº¬
 @param pOutHisto	[out]:	ÃÅº÷ÀË°ûµ½
 @param nQuantStep	[in]:	¹ÀÌ© ¿Í±¨
 */
void
Histogram::	Process_Ellipse(CoImage* pImg, Rect rect,Vec* pOutHisto, int nQuantStep)
{
	Mat* pMatX = &pImg->m_matX;
	Mat* pMatY = &pImg->m_matY;
	Mat* pMatZ = &pImg->m_matZ;
	
	assert(pMatX->Type() == MAT_Tbyte && pMatY->Type() == MAT_Tbyte && pMatZ->Type() == MAT_Tbyte);
	assert (pOutHisto->IsValid());
	assert (pOutHisto->Type() == MAT_Tfloat);
	
	BYTE* pbDataX = pMatX->data.ptr[0];
	BYTE* pbDataY = pMatY->data.ptr[0];
	BYTE* pbDataZ = pMatZ->data.ptr[0];
	
    int nImgSize = 0;//
	int nPixelX, nPixelY, nPixelZ;
	int nBinNum;
	int nBin = GRAY_LEVEL_NUMBER / nQuantStep;   // ·ô±¶ ºÂ¹¤Ì© bin ±¶ºã 

	int i, j, k ;
	// ¼è¼³´ô 4°¢ÂôËæ ³­¼±Â×²÷ À°ÌºµßÊßÌ© x»ô¶®ÁìÂÙ±Ë´ÉË¾ ´ÐµâÂÙ³Þ.
	int *px = (int*) calloc( 2 * rect.height, sizeof(int) ) ;
	int cx = rect.x + rect.width / 2 ;
	int cy = rect.y + rect.height / 2 ;
	float A = rect.width / 2.f ;
	float B = rect.height / 2.f ;
	float A_B = A / B ;
	B *= B ;
	for ( i = rect.y, k = 0 ; k < rect.height ; i ++, k ++ )
	{
		float dy = float (i - cy) ;
		float dx = A_B * sqrtf(B - dy * dy) ;

		px[2 * k + 0] = cx - (int) dx ;
		px[2 * k + 1] = cx + (int) dx ;
	}	

	
	// ÃÅº÷ÀË°ûµ½ »õºÂ
	pOutHisto->Zero();
	
	for ( i = rect.y, k = 0 ; k < rect.height ; i ++, k ++ )
	{
		for( j = px[2 * k + 0]; j < px[2 * k + 1] ; j ++ )
		{
			nPixelX = pbDataX[i * pImg->GetWidth() + j] / nQuantStep;
			nPixelY = pbDataY[i * pImg->GetWidth() + j] / nQuantStep;
			nPixelZ = pbDataZ[i * pImg->GetWidth() + j] / nQuantStep;
			
			nBinNum = nPixelX * nBin * nBin + nPixelY * nBin + nPixelZ;
			pOutHisto->data.fl[nBinNum] ++;	
			 nImgSize ++;
		}
	}

	for ( int Bin = 0 ; Bin < 512 ; Bin ++)
	{
		pOutHisto->data.fl[Bin] /= (float)nImgSize;
	}

	free( px ) ;
}


static void
calcHistLookupTables_8u( const MatND& hist, int dims, const float** ranges, const double* uniranges,
						bool uniform, Vector<int>& _tab )
{
	const int low = 0, high = 256;
	int i, j;
	_tab.SetSize((high-low)*dims);
	int* tab = &_tab[0];

	if( uniform )
	{
		for( i = 0; i < dims; i++ )
		{
			double a = uniranges[i*2];
			double b = uniranges[i*2+1];
			int sz = hist.msize[i];
			int step = hist.mstep[i];

			for( j = low; j < high; j++ )
			{
				int idx = (int)floor(j*a + b);
				int written_idx;
				if( (unsigned)idx < (unsigned)sz )
					written_idx = idx*step;
				else
					written_idx = OUT_OF_RANGE;

				tab[i*(high - low) + j - low] = written_idx;
			}
		}
	}
	else
	{
		for( i = 0; i < dims; i++ )
		{
			int limit = MIN((int)ceil(ranges[i][0]), high);
			int idx = -1, sz = hist.msize[i];
			int written_idx = OUT_OF_RANGE;
			int step = hist.mstep[i];

			for(j = low;;)
			{
				for( ; j < limit; j++ )
					tab[i*(high - low) + j - low] = written_idx;

				if( (unsigned)(++idx) < (unsigned)sz )
				{
					limit = MIN((int)ceil(ranges[i][idx+1]), high);
					written_idx = idx*step;
				}
				else
				{
					for( ; j < high; j++ )
						tab[i*(high - low) + j - low] = OUT_OF_RANGE;
					break;
				}
			}
		}
	}
}

static void 
CalcBackProj_8u( Vector<uchar*>& _ptrs, const Vector<int>& _deltas,
				Size imsize, const MatND& hist, int dims, const float** _ranges,
				const double* _uniranges, float scale, bool uniform )
{
	uchar** ptrs = &_ptrs[0];
//	const int* deltas = &_deltas[0];
	const int* deltas = _deltas.GetData();
	uchar* H = hist.data.ptr;
	int i, x;
	uchar* bproj = _ptrs[dims];
	int bpstep = _deltas[dims*2 + 1];
	Vector<int> _tab;

	calcHistLookupTables_8u( hist, dims, _ranges, _uniranges, uniform, _tab );
	const int* tab = &_tab[0];

	if( dims == 1 )
	{
		int d0 = deltas[0], step0 = deltas[1];
		uchar matH[256] = {0};
		const uchar* p0 = (const uchar*)ptrs[0];

		for( i = 0; i < 256; i++ )
		{
			int hidx = tab[i];
			if( hidx < OUT_OF_RANGE )
				matH[i] = (uchar)(*(float*)(H + hidx)*scale);
		}

		for( ; imsize.height--; p0 += step0, bproj += bpstep )
		{
			if( d0 == 1 )
			{
				for( x = 0; x <= imsize.width - 4; x += 4 )
				{
					uchar t0 = matH[p0[x]], t1 = matH[p0[x+1]];
					bproj[x] = t0; bproj[x+1] = t1;
					t0 = matH[p0[x+2]]; t1 = matH[p0[x+3]];
					bproj[x+2] = t0; bproj[x+3] = t1;
				}
				p0 += x;
			}
			else   
				for( x = 0; x <= imsize.width - 4; x += 4 )
				{
					uchar t0 = matH[p0[0]], t1 = matH[p0[d0]];
					bproj[x] = t0; bproj[x+1] = t1;
					p0 += d0*2;
					t0 = matH[p0[0]]; t1 = matH[p0[d0]];
					bproj[x+2] = t0; bproj[x+3] = t1;
					p0 += d0*2;
				}

				for( ; x < imsize.width; x++, p0 += d0 )
					bproj[x] = matH[*p0];
		}
	}
	else if( dims == 2 )
	{
		int d0 = deltas[0], step0 = deltas[1],
			d1 = deltas[2], step1 = deltas[3];
		const uchar* p0 = (const uchar*)ptrs[0];
		const uchar* p1 = (const uchar*)ptrs[1];

		for( ; imsize.height--; p0 += step0, p1 += step1, bproj += bpstep )
		{
			for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1 )
			{
				int idx = tab[*p0] + tab[*p1 + 256];
				bproj[x] = idx < OUT_OF_RANGE ? (uchar)(*(float*)(H + idx)*scale) : 0;
			}
		}
	}
	else if( dims == 3 )
	{
		int d0 = deltas[0], step0 = deltas[1],
			d1 = deltas[2], step1 = deltas[3],
			d2 = deltas[4], step2 = deltas[5];
		const uchar* p0 = (const uchar*)ptrs[0];
		const uchar* p1 = (const uchar*)ptrs[1];
		const uchar* p2 = (const uchar*)ptrs[2];

		for( ; imsize.height--; p0 += step0, p1 += step1, p2 += step2, bproj += bpstep )
		{
			for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1, p2 += d2 )
			{
				int idx = tab[*p0] + tab[*p1 + 256] + tab[*p2 + 512];
				bproj[x] = idx < OUT_OF_RANGE ? (uchar)(*(float*)(H + idx)*scale) : 0;
			}
		}
	}
	else
	{
		for( ; imsize.height--; bproj += bpstep )
		{
			for( x = 0; x < imsize.width; x++ )
			{
				uchar* Hptr = H;
				for( i = 0; i < dims; i++ )
				{
					int idx = tab[*ptrs[i] + i*256];
					if( idx >= OUT_OF_RANGE )
						break;
					ptrs[i] += deltas[i*2];
					Hptr += idx;
				}

				if( i == dims )
					bproj[x] = (uchar)(*(float*)Hptr*scale);
				else
				{
					bproj[x] = 0;
					for( ; i < dims; i++ )
						ptrs[i] += deltas[i*2];
				}
			}
			for( i = 0; i < dims; i++ )
				ptrs[i] += deltas[i*2 + 1];
		}
	}
}

static void histPrepareImages( Mat** const images, int nimages, const int* channels,
							  const Mat& mask, int dims, const int* histSize,
							  const float** ranges, bool uniform,
							  Vector<uchar*>& ptrs, Vector<int>& deltas,
							  Size& imsize, Vector<double>& uniranges )
{
	int i, j, c;
	assert( channels != 0 || nimages == dims );

	imsize = images[0]->SizeObject();
	TYPE depth = images[0]->Type();
// 	int esz1 = (int)images[0].elemSize1();

	ptrs.SetSize(dims + 1);
	deltas.SetSize((dims + 1)*2);

	for( i = 0; i < dims; i++ )
	{
		if(!channels)
		{
			j = i;
			c = 0;
			assert( images[j]->Channels() == 1 );
		}
		else
		{
			c = channels[i];
			assert( c >= 0 );
			for( j = 0; j < nimages; c -= images[j]->Channels(), j++ )
				if( c < images[j]->Channels() )
					break;
			assert( j < nimages );
		}

		assert( images[j]->SizeObject() == imsize && images[j]->Type() == depth );
//		ptrs[i] = images[j]->data.ptr + c*esz1;
		ptrs[i] = images[j]->data.ptr[0];
		deltas[i*2] = images[j]->Channels();
//		deltas[i*2+1] = (int)(images[j]->step/esz1 - imsize.width*deltas[i*2]);
		deltas[i*2+1] = 0;
	}

	if( mask.IsValid() )
	{
		assert( mask.SizeObject() == imsize && mask.Channels() == 1 );
		ptrs[dims] = mask.data.ptr[0];
		deltas[dims*2] = 1;
//		deltas[dims*2 + 1] = (int)(mask.step/mask.elemSize1());
//		deltas[dims*2 + 1] = (int)(mask.Step());
		deltas[dims*2 + 1] = (int)(mask.Cols());
	}

	if( !ranges )
	{
		assert( depth == MAT_Tbyte );

		uniranges.SetSize( dims*2 );
		for( i = 0; i < dims; i++ )
		{
			uniranges[i*2] = histSize[i]/256.;
			uniranges[i*2+1] = 0;
		}
	}
	else if( uniform )
	{
		uniranges.SetSize( dims*2 );
		for( i = 0; i < dims; i++ )
		{
			assert( ranges[i] && ranges[i][0] < ranges[i][1] );
			double low = ranges[i][0], high = ranges[i][1];
			double t = histSize[i]/(high - low);
			uniranges[i*2] = t;
			uniranges[i*2+1] = -t*low;
		}
	}
	else
	{
		for( i = 0; i < dims; i++ )
		{
			int j, n = histSize[i];
			for( j = 0; j < n; j++ )
				assert( ranges[i][j] < ranges[i][j+1] );
		}
	}
}

template<typename T, typename BT> static void
CalcBackProj_( Vector<uchar*>& _ptrs, const Vector<int>& _deltas,
			  Size imsize, const MatND& hist, int dims, const float** _ranges,
			  const double* _uniranges, float scale, bool uniform )
{
	T** ptrs = (T**)&_ptrs[0];
	const int* deltas = _deltas.GetData();
	uchar* H = hist.data.ptr;
	int i, x;
	BT* bproj = (BT*)_ptrs[dims];
	int bpstep = _deltas[dims*2 + 1];
	int size[CVLIB_MAX_DIM];
	int hstep[CVLIB_MAX_DIM];

	for( i = 0; i < dims; i++ )
	{
		size[i] = hist.msize[i];
		hstep[i] = hist.mstep[i];
	}

	if( uniform )
	{
		const double* uniranges = &_uniranges[0];

		if( dims == 1 )
		{
			double a = uniranges[0], b = uniranges[1];
			int sz = size[0], d0 = deltas[0], step0 = deltas[1];
			const T* p0 = (const T*)ptrs[0];

			for( ; imsize.height--; p0 += step0, bproj += bpstep )
			{
				for( x = 0; x < imsize.width; x++, p0 += d0 )
				{
					int idx = (int)floor(*p0*a + b);
					bproj[x] = ((unsigned)idx < (unsigned)sz) ? (BT)(((float*)H)[idx]*scale) : 0;
				}
			}
		}
		else if( dims == 2 )
		{
			double a0 = uniranges[0], b0 = uniranges[1],
				a1 = uniranges[2], b1 = uniranges[3];
			int sz0 = size[0], sz1 = size[1];
			int d0 = deltas[0], step0 = deltas[1],
				d1 = deltas[2], step1 = deltas[3];
			int hstep0 = hstep[0];
			const T* p0 = (const T*)ptrs[0];
			const T* p1 = (const T*)ptrs[1];

			for( ; imsize.height--; p0 += step0, p1 += step1, bproj += bpstep )
			{
				for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1 )
				{
					int idx0 = (int)floor(*p0*a0 + b0);
					int idx1 = (int)floor(*p1*a1 + b1);
					bproj[x] = ((unsigned)idx0 < (unsigned)sz0 && (unsigned)idx1 < (unsigned)sz1) ?
						(BT)(((float*)(H + hstep0*idx0))[idx1]*scale) : 0;
				}
			}
		}
		else if( dims == 3 )
		{
			double a0 = uniranges[0], b0 = uniranges[1],
				a1 = uniranges[2], b1 = uniranges[3],
				a2 = uniranges[4], b2 = uniranges[5];
			int sz0 = size[0], sz1 = size[1], sz2 = size[2];
			int d0 = deltas[0], step0 = deltas[1],
				d1 = deltas[2], step1 = deltas[3],
				d2 = deltas[4], step2 = deltas[5];
			int hstep0 = hstep[0], hstep1 = hstep[1];
			const T* p0 = (const T*)ptrs[0];
			const T* p1 = (const T*)ptrs[1];
			const T* p2 = (const T*)ptrs[2];            

			for( ; imsize.height--; p0 += step0, p1 += step1, p2 += step2, bproj += bpstep )
			{
				for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1, p2 += d2 )
				{
					int idx0 = (int)floor(*p0*a0 + b0);
					int idx1 = (int)floor(*p1*a1 + b1);
					int idx2 = (int)floor(*p2*a2 + b2);
					bproj[x] = ((unsigned)idx0 < (unsigned)sz0 && (unsigned)idx1 < (unsigned)sz1 && (unsigned)idx2 < (unsigned)sz2) ?
						(BT)(((float*)(H + hstep0*idx0 + hstep1*idx1))[idx2]*scale) : 0;
				}
			}
		}
		else
		{
			for( ; imsize.height--; bproj += bpstep )
			{
				for( x = 0; x < imsize.width; x++ )
				{
					uchar* Hptr = H;
					for( i = 0; i < dims; i++ )
					{
						int idx = (int)floor(*ptrs[i]*uniranges[i*2] + uniranges[i*2+1]);
						if( (unsigned)idx >= (unsigned)size[i] || (_ranges && *ptrs[i] >= _ranges[i][1]))
							break;
						ptrs[i] += deltas[i*2];
						Hptr += idx*hstep[i];
					}

					if( i == dims )
						bproj[x] = (BT)(*(float*)Hptr*scale);
					else
					{
						bproj[x] = 0;
						for( ; i < dims; i++ )
							ptrs[i] += deltas[i*2];
					}
				}
				for( i = 0; i < dims; i++ )
					ptrs[i] += deltas[i*2 + 1];
			}
		}
	}
	else
	{
		// non-uniform histogram
		const float* ranges[CVLIB_MAX_DIM];
		for( i = 0; i < dims; i++ )
			ranges[i] = &_ranges[i][0];

		for( ; imsize.height--; bproj += bpstep )
		{
			for( x = 0; x < imsize.width; x++ )
			{
				uchar* Hptr = H;
				for( i = 0; i < dims; i++ )
				{
					float v = (float)*ptrs[i];
					const float* R = ranges[i];
					int idx = -1, sz = size[i];

					while( v >= R[idx+1] && ++idx < sz )
						; // nop

					if( (unsigned)idx >= (unsigned)sz )
						break;

					ptrs[i] += deltas[i*2];
					Hptr += idx*hstep[i];
				}

				if( i == dims )
					bproj[x] = (BT)(*(float*)Hptr*scale);
				else
				{
					bproj[x] = 0;
					for( ; i < dims; i++ )
						ptrs[i] += deltas[i*2];
				}
			}

			for( i = 0; i < dims; i++ )
				ptrs[i] += deltas[i*2 + 1];
		}
	}    
}

void CalcBackProject( Mat** const images, int nimages, const int* Channels, 
					MatND& hist, Mat& backProject,
					const float** ranges, double scale, bool uniform )
{
	Vector<uchar*> ptrs;
	Vector<int> deltas;
	Vector<double> uniranges;
	Size imsize;
	int dims = hist.Dims() == 2 && hist.msize[1] == 1 ? 1 : hist.Dims();

	assert( hist.IsValid() );
	if ( !backProject.IsEqualMat(*images[0]) )
	{
		backProject.Release();
		backProject.Create( images[0]->SizeObject(), images[0]->Type() );
	}
	histPrepareImages( images, nimages, Channels, backProject, dims, hist.msize, ranges,
		uniform, ptrs, deltas, imsize, uniranges );
	const double* _uniranges = uniform ? &uniranges[0] : 0;

	TYPE depth = images[0]->Type();
	if( depth == MAT_Tbyte )
		CalcBackProj_8u(ptrs, deltas, imsize, hist, dims, ranges, _uniranges, (float)scale, uniform);
	else if( depth == MAT_Tshort )
		CalcBackProj_<short,short>(ptrs, deltas, imsize, hist, dims, ranges, _uniranges, (float)scale, uniform );
	else if( depth == MAT_Tfloat )
		CalcBackProj_<float,float>(ptrs, deltas, imsize, hist, dims, ranges, _uniranges, (float)scale, uniform );
	else
		assert (false);
}

void CalcBackProject( const CoImage& image, const int* channels, MatND& hist, Mat& backProject, 
					 const float** ranges, double scale/*=1*/, bool uniform/*=true */)
{
	CalcBackProject(image.GetBand(), image.GetChannel(), channels, hist, backProject, 
		ranges, scale, uniform);
}

template<typename T> static void
calcHist_( Vector<uchar*>& _ptrs, const Vector<int>& _deltas,
		  Size imsize, MatND& hist, int dims, const float** _ranges,
		  const double* _uniranges, bool uniform )
{
	T** ptrs = (T**)&_ptrs[0];
	const int* deltas = _deltas.GetData();
	uchar* H = hist.data.ptr;
	int i, x;
	const uchar* mask = _ptrs[dims];
	int mstep = _deltas[dims*2 + 1];
	int size[CVLIB_MAX_DIM];
	int hstep[CVLIB_MAX_DIM];

	for( i = 0; i < dims; i++ )
	{
		size[i] = hist.msize[i];
		hstep[i] = hist.mstep[i];
	}

	if( uniform )
	{
		const double* uniranges = &_uniranges[0];

		if( dims == 1 )
		{
			double a = uniranges[0], b = uniranges[1];
			int sz = size[0], d0 = deltas[0], step0 = deltas[1];
			const T* p0 = (const T*)ptrs[0];

			for( ; imsize.height--; p0 += step0, mask += mstep )
			{
				if( !mask )
					for( x = 0; x < imsize.width; x++, p0 += d0 )
					{
						int idx = (int)floor(*p0*a + b);
						if( (unsigned)idx < (unsigned)sz )
							((int*)H)[idx]++;
					}
				else
					for( x = 0; x < imsize.width; x++, p0 += d0 )
						if( mask[x] )
						{
							int idx = (int)floor(*p0*a + b);
							if( (unsigned)idx < (unsigned)sz )
								((int*)H)[idx]++;
						}
			}
		}
		else if( dims == 2 )
		{
			double a0 = uniranges[0], b0 = uniranges[1], a1 = uniranges[2], b1 = uniranges[3];
			int sz0 = size[0], sz1 = size[1];
			int d0 = deltas[0], step0 = deltas[1],
				d1 = deltas[2], step1 = deltas[3];
			int hstep0 = hstep[0];
			const T* p0 = (const T*)ptrs[0];
			const T* p1 = (const T*)ptrs[1];

			for( ; imsize.height--; p0 += step0, p1 += step1, mask += mstep )
			{
				if( !mask )
					for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1 )
					{
						int idx0 = (int)floor(*p0*a0 + b0);
						int idx1 = (int)floor(*p1*a1 + b1);
						if( (unsigned)idx0 < (unsigned)sz0 && (unsigned)idx1 < (unsigned)sz1 )
							((int*)(H + hstep0*idx0))[idx1]++;
					}
				else
					for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1 )
						if( mask[x] )
						{
							int idx0 = (int)floor(*p0*a0 + b0);
							int idx1 = (int)floor(*p1*a1 + b1);
							if( (unsigned)idx0 < (unsigned)sz0 && (unsigned)idx1 < (unsigned)sz1 )
								((int*)(H + hstep0*idx0))[idx1]++;
						}
			}
		}
		else if( dims == 3 )
		{
			double a0 = uniranges[0], b0 = uniranges[1],
				a1 = uniranges[2], b1 = uniranges[3],
				a2 = uniranges[4], b2 = uniranges[5];
			int sz0 = size[0], sz1 = size[1], sz2 = size[2];
			int d0 = deltas[0], step0 = deltas[1],
				d1 = deltas[2], step1 = deltas[3],
				d2 = deltas[4], step2 = deltas[5];
			int hstep0 = hstep[0], hstep1 = hstep[1];
			const T* p0 = (const T*)ptrs[0];
			const T* p1 = (const T*)ptrs[1];
			const T* p2 = (const T*)ptrs[2];            

			for( ; imsize.height--; p0 += step0, p1 += step1, p2 += step2, mask += mstep )
			{
				if( !mask )
					for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1, p2 += d2 )
					{
						int idx0 = (int)floor(*p0*a0 + b0);
						int idx1 = (int)floor(*p1*a1 + b1);
						int idx2 = (int)floor(*p2*a2 + b2);
						if( (unsigned)idx0 < (unsigned)sz0 &&
							(unsigned)idx1 < (unsigned)sz1 &&
							(unsigned)idx2 < (unsigned)sz2 )
							((int*)(H + hstep0*idx0 + hstep1*idx1))[idx2]++;
					}
				else
					for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1, p2 += d2 )
						if( mask[x] )
						{
							int idx0 = (int)floor(*p0*a0 + b0);
							int idx1 = (int)floor(*p1*a1 + b1);
							int idx2 = (int)floor(*p2*a2 + b2);
							if( (unsigned)idx0 < (unsigned)sz0 &&
								(unsigned)idx1 < (unsigned)sz1 &&
								(unsigned)idx2 < (unsigned)sz2 )
								((int*)(H + hstep0*idx0 + hstep1*idx1))[idx2]++;
						}
			}
		}
		else
		{
			for( ; imsize.height--; mask += mstep )
			{
				if( !mask )
					for( x = 0; x < imsize.width; x++ )
					{
						uchar* Hptr = H;
						for( i = 0; i < dims; i++ )
						{
							int idx = (int)floor(*ptrs[i]*uniranges[i*2] + uniranges[i*2+1]);
							if( (unsigned)idx >= (unsigned)size[i] )
								break;
							ptrs[i] += deltas[i*2];
							Hptr += idx*hstep[i];
						}

						if( i == dims )
							++*((int*)Hptr);
						else
							for( ; i < dims; i++ )
								ptrs[i] += deltas[i*2];
					}
				else
					for( x = 0; x < imsize.width; x++ )
					{
						uchar* Hptr = H;
						i = 0;
						if( mask[x] )
							for( ; i < dims; i++ )
							{
								int idx = (int)floor(*ptrs[i]*uniranges[i*2] + uniranges[i*2+1]);
								if( (unsigned)idx >= (unsigned)size[i] )
									break;
								ptrs[i] += deltas[i*2];
								Hptr += idx*hstep[i];
							}

							if( i == dims )
								++*((int*)Hptr);
							else
								for( ; i < dims; i++ )
									ptrs[i] += deltas[i*2];
					}
					for( i = 0; i < dims; i++ )
						ptrs[i] += deltas[i*2 + 1];
			}
		}
	}
	else
	{
		// non-uniform histogram
		const float* ranges[CVLIB_MAX_DIM];
		for( i = 0; i < dims; i++ )
			ranges[i] = &_ranges[i][0];

		for( ; imsize.height--; mask += mstep )
		{
			for( x = 0; x < imsize.width; x++ )
			{
				uchar* Hptr = H;
				i = 0;

				if( !mask || mask[x] )
					for( ; i < dims; i++ )
					{
						float v = (float)*ptrs[i];
						const float* R = ranges[i];
						int idx = -1, sz = size[i];

						while( v >= R[idx+1] && ++idx < sz )
							; // nop

						if( (unsigned)idx >= (unsigned)sz )
							break;

						ptrs[i] += deltas[i*2];
						Hptr += idx*hstep[i];
					}

					if( i == dims )
						++*((int*)Hptr);
					else
						for( ; i < dims; i++ )
							ptrs[i] += deltas[i*2];
			}

			for( i = 0; i < dims; i++ )
				ptrs[i] += deltas[i*2 + 1];
		}
	}    
}

static void
calcHist_8u( Vector<uchar*>& _ptrs, const Vector<int>& _deltas,
			Size imsize, MatND& hist, int dims, const float** _ranges,
			const double* _uniranges, bool uniform )
{
	uchar** ptrs = &_ptrs[0];
	const int* deltas = _deltas.GetData();
	uchar* H = hist.data.ptr;
	int i, x;
	const uchar* mask = _ptrs[dims];
	int mstep = _deltas[dims*2 + 1];
	Vector<int> _tab;

	calcHistLookupTables_8u( hist, dims, _ranges, _uniranges, uniform, _tab );
	const int* tab = &_tab[0];

	if( dims == 1 )
	{
		int d0 = deltas[0], step0 = deltas[1];
		int matH[256] = {0};
		const uchar* p0 = (const uchar*)ptrs[0];

		for( ; imsize.height--; p0 += step0, mask += mstep )
		{
			if( !mask )
			{
				if( d0 == 1 )
				{
					for( x = 0; x <= imsize.width - 4; x += 4 )
					{
						int t0 = p0[x], t1 = p0[x+1];
						matH[t0]++; matH[t1]++;
						t0 = p0[x+2]; t1 = p0[x+3];
						matH[t0]++; matH[t1]++;
					}
					p0 += x;
				}
				else   
					for( x = 0; x <= imsize.width - 4; x += 4 )
					{
						int t0 = p0[0], t1 = p0[d0];
						matH[t0]++; matH[t1]++;
						p0 += d0*2;
						t0 = p0[0]; t1 = p0[d0];
						matH[t0]++; matH[t1]++;
						p0 += d0*2;
					}

					for( ; x < imsize.width; x++, p0 += d0 )
						matH[*p0]++;
			}
			else
				for( x = 0; x < imsize.width; x++, p0 += d0 )
					if( mask[x] )
						matH[*p0]++;
		}

		for( i = 0; i < 256; i++ )
		{
			int hidx = tab[i];
			if( hidx < OUT_OF_RANGE )
				*(int*)(H + hidx) += matH[i];
		}
	}
	else if( dims == 2 )
	{
		int d0 = deltas[0], step0 = deltas[1],
			d1 = deltas[2], step1 = deltas[3];
		const uchar* p0 = (const uchar*)ptrs[0];
		const uchar* p1 = (const uchar*)ptrs[1];

		for( ; imsize.height--; p0 += step0, p1 += step1, mask += mstep )
		{
			if( !mask )
				for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1 )
				{
					int idx = tab[*p0] + tab[*p1 + 256];
					if( idx < OUT_OF_RANGE )
						++*(int*)(H + idx);
				}
			else
				for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1 )
				{
					int idx;
					if( mask[x] && (idx = tab[*p0] + tab[*p1 + 256]) < OUT_OF_RANGE )
						++*(int*)(H + idx);
				}
		}
	}
	else if( dims == 3 )
	{
		int d0 = deltas[0], step0 = deltas[1],
			d1 = deltas[2], step1 = deltas[3],
			d2 = deltas[4], step2 = deltas[5];

		const uchar* p0 = (const uchar*)ptrs[0];
		const uchar* p1 = (const uchar*)ptrs[1];
		const uchar* p2 = (const uchar*)ptrs[2];

		for( ; imsize.height--; p0 += step0, p1 += step1, p2 += step2, mask += mstep )
		{
			if( !mask )
				for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1, p2 += d2 )
				{
					int idx = tab[*p0] + tab[*p1 + 256] + tab[*p2 + 512];
					if( idx < OUT_OF_RANGE )
						++*(int*)(H + idx);
				}
			else
				for( x = 0; x < imsize.width; x++, p0 += d0, p1 += d1, p2 += d2 )
				{
					int idx;
					if( mask[x] && (idx = tab[*p0] + tab[*p1 + 256] + tab[*p2 + 512]) < OUT_OF_RANGE )
						++*(int*)(H + idx);
				}
		}
	}
	else
	{
		for( ; imsize.height--; mask += mstep )
		{
			if( !mask )
				for( x = 0; x < imsize.width; x++ )
				{
					uchar* Hptr = H;
					for( i = 0; i < dims; i++ )
					{
						int idx = tab[*ptrs[i] + i*256];
						if( idx >= OUT_OF_RANGE )
							break;
						Hptr += idx;
						ptrs[i] += deltas[i*2];
					}

					if( i == dims )
						++*((int*)Hptr);
					else
						for( ; i < dims; i++ )
							ptrs[i] += deltas[i*2];
				}
			else
				for( x = 0; x < imsize.width; x++ )
				{
					uchar* Hptr = H;
					int i = 0;
					if( mask[x] )
						for( ; i < dims; i++ )
						{
							int idx = tab[*ptrs[i] + i*256];
							if( idx >= OUT_OF_RANGE )
								break;
							Hptr += idx;
							ptrs[i] += deltas[i*2];
						}

						if( i == dims )
							++*((int*)Hptr);
						else
							for( ; i < dims; i++ )
								ptrs[i] += deltas[i*2];
				}
				for( i = 0; i < dims; i++ )
					ptrs[i] += deltas[i*2 + 1];
		}
	}
}
void CalcHist(const CoImage& image, const Mat& mask, MatND& hist, 
			  int dims, const int* histSize, const float** ranges, 
			  bool uniform/*=true*/, bool accumulate/*=false*/)
{
	assert(histSize);

	Vector<uchar*> ptrs;
	Vector<int> deltas;
	Vector<double> uniranges;
	Size imsize;

	assert( !mask.IsValid() || mask.Type() == MAT_Tbyte );
	hist.Release();
	hist.Create(dims, histSize, MAT_Tint, NULL, true);
	hist.Zero();

	const int channels[]={0,1,2};
	histPrepareImages( image.GetBand(), image.GetChannel(), channels, mask, dims, histSize, 
		ranges, uniform, ptrs, deltas, imsize, uniranges );
	const double* _uniranges = uniform ? &uniranges[0] : 0;

	TYPE depth = image.m_matX.Type();

	if( depth == MAT_Tbyte )
		calcHist_8u(ptrs, deltas, imsize, hist, dims, ranges, _uniranges, uniform );
	else if( depth == MAT_Tshort )
		calcHist_<short>(ptrs, deltas, imsize, hist, dims, ranges, _uniranges, uniform );
	else if( depth == MAT_Tfloat )
		calcHist_<float>(ptrs, deltas, imsize, hist, dims, ranges, _uniranges, uniform );
	else
		assert(false);

	hist.Convert(MAT_Tfloat);
}

}
