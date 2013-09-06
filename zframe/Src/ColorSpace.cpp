#include "ColorSpace.h"
#include "cvlibbase/Inc/cvlibmacros.h"

namespace CVLib
{

static float D65X = 0.9505f;
static float D65Y = 1.0f;
static float D65Z = 1.0890f;

void ColorSpace::RGBtoGray (const Mat& colorImg, Mat& mgray)
{
	if (colorImg.Channels()==1)
	{
		mgray.Create (colorImg, TRUE);
		return;
	}
	if (mgray.SizeObject() != colorImg.SizeObject() || mgray.Channels()==3)
	{
		mgray.Release();
		mgray.Create (colorImg.SizeObject(), colorImg.Type());
	}
	int nH = colorImg.Rows(), nW = colorImg.Cols();
	for (int i = 0; i < nH; i ++)
	{
		uchar* pcolor=colorImg.data.ptr[i];
		uchar* pgray=mgray.data.ptr[i];
		for (int j = 0, k=0; k<nW; j+=3, k++)
			pgray[k] = (uchar)((299 * pcolor[j] + 587 * pcolor[j+1] + 114 * pcolor[j+2]) / 1000);
	}
}
void ColorSpace::Split (const Mat& color, Mat& x, Mat& y, Mat& z)
{
	x.Create (color.SizeObject(), color.Type());
	y.Create (color.SizeObject(), color.Type());
	z.Create (color.SizeObject(), color.Type());
	int d=0;
    int cn=color.Channels();
	uchar* pcolor=color.data.ptr[0];
	uchar* px=x.data.ptr[0];
	uchar* py=y.data.ptr[0];
	uchar* pz=z.data.ptr[0];
	for (int i=0; i<color.SizeObject().Area(); i++, d+=cn)
	{
		px[i]=pcolor[d];
		py[i]=pcolor[d+1];
		pz[i]=pcolor[d+2];
	}
}
void ColorSpace::BGRAtoRGB(const Mat& color1, Mat& color2)
{
    color2.Create (color1.Rows(), color1.Cols(), MAT_TBYTE3);
	int d=0;
	uchar* psrc=color1.data.ptr[0];
	uchar* pdst=color2.data.ptr[0];
    int datalen=color1.Rows()*color1.Cols()*3;
	for (int i=0; i<datalen; i+=3, d+=4)
	{
		pdst[i]=psrc[d+2];
		pdst[i+1]=psrc[d+1];
		pdst[i+2]=psrc[d];
	}
}

void ColorSpace::GraytoRGB (const Mat& gray, Mat& color)
{
	if (gray.Channels()==3)
	{
		color.Create (gray, TRUE);
		return;
	}
	if (gray.SizeObject() != color.SizeObject() || color.Channels()==3)
	{
		color.Release();
		color.Create (gray.SizeObject(), (TYPE)CVLIB_MAKETYPE(gray.Type(), 3));
	}
	int nH = color.Rows(), nW = color.Cols();
	int elemsize=CVLIB_ELEM_SIZE(gray.Type());
	for (int i = 0; i < nH; i ++)
	{
		uchar* pcolor=color.data.ptr[i];
		uchar* pgray=gray.data.ptr[i];
		for (int k=0; k<nW; k++)
		{
			memcpy (&pcolor[3*k*elemsize], &pgray[k*elemsize], elemsize);
			memcpy (&pcolor[(3*k+1)*elemsize], &pgray[k*elemsize], elemsize);
			memcpy (&pcolor[(3*k+2)*elemsize], &pgray[k*elemsize], elemsize);
		}
	}
}

void ColorSpace::RGBtoHSV (BYTE bR, BYTE bG, BYTE bB, float *prH, float *prS, float *prV)
{
	float rMax, rMin;
	float rR, rG, rB;
	float rcosta,rthita, rH;
	float pi = 3.142592f;
	
	rR = bR / 255.0F;
	rG = bG / 255.0F;
	rB = bB / 255.0F;
	rMax = MAX(rR, MAX(rG, rB));
	rMin = MIN(rR, MIN(rG, rB));
	if (rMax == 0)
	{	/*black*/
		*prH = *prS = *prV= 0;
	}
	else 
	{
		if (rMax==rMin)
		{
			*prH = *prS = 0;
			*prV = rMax;
		}
		else if(rMin == 1)
		{/*white*/
			*prH = *prS = 0;
			*prV = 1;
		}
		else
		{
			rcosta	= (rR - (rG + rB) / 2.0f) / (float)sqrt((rR-rG) * (rR-rG) + (rR-rB)*(rG-rB));
			rthita = (float)acos(rcosta);
			rH = ( rG >= rB ) ? rthita : 2.0f * pi - rthita ;
			*prH =(float)( rH * 180.0 / pi); 
			*prV = (float)((rR + rG + rB)/3.0);						
			*prS = (rMax != 0) ? (float)(1.0 -  rMin / (*prV)) : 0.0f;
		}
	}
}

void ColorSpace::RGBtoHSV (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tbyte);
	assert (pOut->GetType() == MAT_Tfloat);
	BYTE** ppbR = pIn->m_matX.data.ptr;
	BYTE** ppbG = pIn->m_matY.data.ptr;
	BYTE** ppbB = pIn->m_matZ.data.ptr;

	float** pprR = pOut->m_matX.data.fl;
	float** pprG = pOut->m_matY.data.fl;
	float** pprB = pOut->m_matZ.data.fl;

	for (int iH = 0; iH < pIn->GetHeight(); iH ++)
	for (int iW = 0; iW < pIn->GetWidth(); iW ++)
	{
		RGBtoHSV(ppbR[iH][iW], ppbG[iH][iW], ppbB[iH][iW], &pprR[iH][iW], &pprG[iH][iW], &pprB[iH][iW]);
	}
}
void ColorSpace::RGBtoHSV (const Mat& src, Mat& dst)
{
	dst.Create (src.SizeObject(), MAT_Tfloat3);
	BYTE*	pbyte = src.data.ptr[0];
	float*	pfloat = dst.data.fl[0];
	int d=0;
	for (int i=0; i<src.SizeObject().Area(); i++, d+=3)
		RGBtoHSV(pbyte[d], pbyte[d+1], pbyte[d+2], &pfloat[d], &pfloat[d+1], &pfloat[d+2]);
}

void ColorSpace::RGBtoLab (BYTE bR, BYTE bG, BYTE bB, float *prL, float *pra, float *prb)
{
	float rX, rY, rZ;
	RGBtoXYZ(bR, bG, bB, &rX, &rY, &rZ);
	XYZtoLab(rX, rY, rZ, prL, pra, prb);
}

void ColorSpace::RGBtoLab (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tbyte);
	assert (pOut->GetType() == MAT_Tfloat);
	BYTE* pbR = pIn->m_matX.data.ptr[0];
	BYTE* pbG = pIn->m_matY.data.ptr[0];
	BYTE* pbB = pIn->m_matZ.data.ptr[0];
	
	float* prL = pOut->m_matX.data.fl[0];
	float* pra = pOut->m_matY.data.fl[0];
	float* prb = pOut->m_matZ.data.fl[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		RGBtoLab(pbR[i], pbG[i], pbB[i], &prL[i], &pra[i], &prb[i]);
	}
}

void ColorSpace::RGBtoXYZ (BYTE bR, BYTE bG, BYTE bB, float *prX, float *prY, float *prZ)
{		
	// normalize red, green, blue values
	float rLinear = (float)bR/255.0f;
	float gLinear = (float)bG/255.0f;
	float bLinear = (float)bB/255.0f;
	
	// convert to a sRGB form
	float r = (rLinear > 0.04045f)? (float)pow((rLinear + 0.055)/(1.0 + 0.055), 2.2) : (rLinear/12.92f) ;
	float g = (gLinear > 0.04045f)? (float)pow((gLinear + 0.055)/(1.0 + 0.055), 2.2) : (gLinear/12.92f) ;
	float b = (bLinear > 0.04045f)? (float)pow((bLinear + 0.055)/(1.0 + 0.055), 2.2) : (bLinear/12.92f) ;
	
	*prX = r*0.4124f + g*0.3576f + b*0.1805f;
	*prY = r*0.2126f + g*0.7152f + b*0.0722f;
	*prZ = r*0.0193f + g*0.1192f + b*0.9505f;
// 	*prX = rLinear*0.4124f + gLinear*0.3576f + bLinear*0.1805f;
// 	*prY = rLinear*0.2126f + gLinear*0.7152f + bLinear*0.0722f;
// 	*prZ = rLinear*0.0193f + gLinear*0.1192f + bLinear*0.9505f;

	return;
}

void ColorSpace::RGBtoYUV (BYTE bR, BYTE bG, BYTE bB, float *prY, float *prU, float *prV)
{
	
	//	Y = (int)( 0.257 * R + 0.504 * G + 0.098 * B);
	//	U = (int)( 0.439 * R - 0.368 * G - 0.071 * B + 128);
	//	V = (int)(-0.148 * R - 0.291 * G + 0.439 * B + 128);
	*prY = (float)(0.299f * bR + 0.587f * bG + 0.114f * bB);
	*prU = (float)((bB-*prY) * 0.565f + 128);
	*prV = (float)((bR-*prY) * 0.713f + 128);
	
	*prY= MIN(255,MAX(0,*prY));
	*prU= MIN(255,MAX(0,*prU));
	*prV= MIN(255,MAX(0,*prV));
	return;
}

void ColorSpace::RGBtoYUV (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tbyte);
	assert (pOut->GetType() == MAT_Tfloat);
	BYTE* pbR = pIn->m_matX.data.ptr[0];
	BYTE* pbG = pIn->m_matY.data.ptr[0];
	BYTE* pbB = pIn->m_matZ.data.ptr[0];
	
	float* prL = pOut->m_matX.data.fl[0];
	float* pra = pOut->m_matY.data.fl[0];
	float* prb = pOut->m_matZ.data.fl[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		RGBtoYUV(pbR[i], pbG[i], pbB[i], &prL[i], &pra[i], &prb[i]);
	}
}

void ColorSpace::XYZtoRGB(float x, float y, float z, BYTE* pbR, BYTE* pbG, BYTE* pbB)
{
	float* Clinear = new float[3];
	Clinear[0] = x*3.2410f - y*1.5374f - z*0.4986f; // red
	Clinear[1] = -x*0.9692f + y*1.8760f + z*0.0416f; // green
	Clinear[2] = x*0.0556f - y*0.2040f + z*1.0570f; // blue
	
	for(int i=0; i<3; i++)
		Clinear[i] = (Clinear[i]<=0.0031308)? 12.92f*Clinear[i] : (1.0f+0.055f)* (float)pow(Clinear[i], (1.0f/2.4f)) - 0.055f;
	
	*pbR = (BYTE)(Clinear[0]*255.0+0.5); 
	*pbG = (BYTE)(Clinear[1]*255.0+0.5); 
	*pbB = (BYTE)(Clinear[2]*255.0+0.5);
	delete []Clinear;
}

/// <summary>
/// XYZ to L*a*b* transformation function.
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
float ColorSpace::Fxyz(float t)
{
	return ((t > 0.008856)? (float)pow(t, (1.0f/3.0f)) : (7.787f*t + 16.0f/116.0f));
}

/// <summary>
/// Converts CIEXYZ to CIELab structure.
/// </summary>
void ColorSpace::XYZtoLab(float x, float y, float z, float* prL, float* pra, float* prb)
{
// 	*prL = (float)(116.0f * pow(y/D65Y, 1.0/3.0) -16.0f);
// 	if (*prL < 0) *prL = 0.0f;
// 	*pra = 500.0f * (float)(pow(x/D65X, 1.0/3.0) - pow(y/D65Y, 1.0/3.0));
// 	*prb = 200.0f * (float)(pow(y/D65Y, 1.0/3.0) - pow(z/D65Z, 1.0/3.0));

	*prL = 116.0f * Fxyz( y/D65Y ) -16.0f;
	*pra = 500.0f * (Fxyz( x/D65X ) - Fxyz( y/D65Y) );
	*prb = 200.0f * (Fxyz( y/D65Y ) - Fxyz( z/D65Z) );
}

void ColorSpace::LabtoXYZ(float l, float a, float b, float* x, float* y, float* z)
{
	float theta = 6.0f/29.0f;
	
	float fy = (l+16)/116.0f;
	float fx = fy + (a/500.0f);
	float fz = fy - (b/200.0f);
	
	*x = (fx > theta)? D65X * (fx*fx*fx) : (fx - 16.0f/116.0f)*3*(theta*theta)*D65X;
	*y = (fy > theta)? D65Y * (fy*fy*fy) : (fy - 16.0f/116.0f)*3*(theta*theta)*D65Y;
	*z = (fz > theta)? D65Z * (fz*fz*fz) : (fz - 16.0f/116.0f)*3*(theta*theta)*D65Z;
}

/// <summary>
/// Converts CIELab to RGB.
/// </summary>
void ColorSpace::LabtoRGB(float l, float a, float b, BYTE* red, BYTE* green, BYTE* blue)
{
	float x,y,z;
	LabtoXYZ(l, a, b, &x,&y,&z);
	XYZtoRGB(x,y,z,red,green,blue);
}

void ColorSpace::RGBtoXYZ (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tbyte);
	assert (pOut->GetType() == MAT_Tfloat);
	BYTE** ppbR = pIn->m_matX.data.ptr;
	BYTE** ppbG = pIn->m_matY.data.ptr;
	BYTE** ppbB = pIn->m_matZ.data.ptr;
	
	float** pprX = pOut->m_matX.data.fl;
	float** pprY = pOut->m_matY.data.fl;
	float** pprZ = pOut->m_matZ.data.fl;
	
	for (int iH = 0; iH < pIn->GetHeight(); iH ++)
		for (int iW = 0; iW < pIn->GetWidth(); iW ++)
		{
			RGBtoXYZ(ppbR[iH][iW], ppbG[iH][iW], ppbB[iH][iW], &pprX[iH][iW], &pprY[iH][iW], &pprZ[iH][iW]);
		}
}

void ColorSpace::XYZtoRGB (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tfloat);
	assert (pOut->GetType() == MAT_Tbyte);
	float* prX = pIn->m_matX.data.fl[0];
	float* prY = pIn->m_matY.data.fl[0];
	float* prZ = pIn->m_matZ.data.fl[0];
	
	BYTE* pbR = pOut->m_matX.data.ptr[0];
	BYTE* pbG = pOut->m_matY.data.ptr[0];
	BYTE* pbB = pOut->m_matZ.data.ptr[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		XYZtoRGB(prX[i], prY[i], prZ[i], &pbR[i], &pbG[i], &pbB[i]);
	}
}

void ColorSpace::XYZtoLab (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tfloat);
	assert (pOut->GetType() == MAT_Tfloat);
	float* prX = pIn->m_matX.data.fl[0];
	float* prY = pIn->m_matY.data.fl[0];
	float* prZ = pIn->m_matZ.data.fl[0];
	
	float* pbR = pOut->m_matX.data.fl[0];
	float* pbG = pOut->m_matY.data.fl[0];
	float* pbB = pOut->m_matZ.data.fl[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		XYZtoLab(prX[i], prY[i], prZ[i], &pbR[i], &pbG[i], &pbB[i]);
	}
}

void ColorSpace::LabtoXYZ (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tfloat);
	assert (pOut->GetType() == MAT_Tfloat);
	float* prX = pIn->m_matX.data.fl[0];
	float* prY = pIn->m_matY.data.fl[0];
	float* prZ = pIn->m_matZ.data.fl[0];
	
	float* pbR = pOut->m_matX.data.fl[0];
	float* pbG = pOut->m_matY.data.fl[0];
	float* pbB = pOut->m_matZ.data.fl[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		LabtoXYZ(prX[i], prY[i], prZ[i], &pbR[i], &pbG[i], &pbB[i]);
	}
}

void ColorSpace::LabtoRGB (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tfloat);
	assert (pOut->GetType() == MAT_Tbyte);
	float* prL = pIn->m_matX.data.fl[0];
	float* pra = pIn->m_matY.data.fl[0];
	float* prb = pIn->m_matZ.data.fl[0];
	
	BYTE* pbR = pOut->m_matX.data.ptr[0];
	BYTE* pbG = pOut->m_matY.data.ptr[0];
	BYTE* pbB = pOut->m_matZ.data.ptr[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		LabtoRGB(prL[i], pra[i], prb[i], &pbR[i], &pbG[i], &pbB[i]);
	}
}

void ColorSpace::RGBtoHSL (BYTE bR, BYTE bG, BYTE bB, float *prH, float *prS, float *prL)
{
	float h=0, s=0, l=0;
	
	// normalizes red-green-blue values
	float nRed = (float)bR/255.0f;
	float nGreen = (float)bG/255.0f;
	float nBlue = (float)bB/255.0f;
	
	float MAX = MAX(nRed, MAX(nGreen, nBlue));
	float MIN = MIN(nRed, MIN(nGreen, nBlue));
	
	// hue
	if(MAX == MIN)
	{
		h = 0; // undefined
	}
	else if(MAX==nRed && nGreen>=nBlue)
	{
		h = 60.0f*(nGreen-nBlue)/(MAX-MIN);
	}
	else if(MAX==nRed && nGreen<nBlue)
	{
		h = 60.0f*(nGreen-nBlue)/(MAX-MIN) + 360.0f;
	}
	else if(MAX==nGreen)
	{
		h = 60.0f*(nBlue-nRed)/(MAX-MIN) + 120.0f;
	}
	else if(MAX==nBlue)
	{
		h = 60.0f*(nRed-nGreen)/(MAX-MIN) + 240.0f;
	}
	
	// luminance
	l = (MAX+MIN)/2.0f;
	
	// saturation
	if(l == 0 || MAX == MIN)
	{
		s = 0;
	}
	else if(0<l && l<=0.5)
	{
		s = (MAX-MIN)/(MAX+MIN);
	}
	else if(l>0.5)
	{
		s = (MAX-MIN)/(2 - (MAX+MIN)); //(MAX-MIN > 0)?
	}

	*prH = h;
	*prS = s;
	*prL = l;
}

void ColorSpace::RGBtoHSL (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tbyte);
	assert (pOut->GetType() == MAT_Tfloat);
	BYTE** ppbR = pIn->m_matX.data.ptr;
	BYTE** ppbG = pIn->m_matY.data.ptr;
	BYTE** ppbB = pIn->m_matZ.data.ptr;

	float** pprR = pOut->m_matX.data.fl;
	float** pprG = pOut->m_matY.data.fl;
	float** pprB = pOut->m_matZ.data.fl;

	for (int iH = 0; iH < pIn->GetHeight(); iH ++)
	for (int iW = 0; iW < pIn->GetWidth(); iW ++)
	{
		RGBtoHSL(ppbR[iH][iW], ppbG[iH][iW], ppbB[iH][iW], &pprR[iH][iW], &pprG[iH][iW], &pprB[iH][iW]);
	}
}

void ColorSpace::HSLtoRGB (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tfloat);
	assert (pOut->GetType() == MAT_Tbyte);
	float* prH = pIn->m_matX.data.fl[0];
	float* prS = pIn->m_matY.data.fl[0];
	float* prL = pIn->m_matZ.data.fl[0];
	
	BYTE* pbR = pOut->m_matX.data.ptr[0];
	BYTE* pbG = pOut->m_matY.data.ptr[0];
	BYTE* pbB = pOut->m_matZ.data.ptr[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		HSLtoRGB(prH[i], prS[i], prL[i], &pbR[i], &pbG[i], &pbB[i]);
	}
}

void ColorSpace::HSLtoRGB (float h, float s, float l, BYTE* pbR, BYTE* pbG, BYTE* pbB)
{
	if(s == 0)
	{
		*pbR = (BYTE)(l * 255.0f);
		*pbG = *pbR;
		*pbB = *pbR;
	}
	else
	{
		float q = (l<0.5)?(l * (1.0f+s)):(l+s - (l*s));
		float p = (2.0f * l) - q;
		
		float Hk = h/360.0f;
		float* T = new float[3];
		T[0] = Hk + (1.0f/3.0f);	// Tr
		T[1] = Hk;				// Tb
		T[2] = Hk - (1.0f/3.0f);	// Tg
		
		for(int i=0; i<3; i++)
		{
			if(T[i] < 0) T[i] += 1.0f;
			if(T[i] > 1) T[i] -= 1.0f;
			
			if((T[i]*6) < 1)
			{
				T[i] = p + ((q-p)*6.0f*T[i]);
			}
			else if((T[i]*2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
			{
				T[i] = q;
			}
			else if((T[i]*3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
			{
				T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;
			}
			else T[i] = p;
		}
		
		*pbR = (BYTE)(T[0]*255.0f);
		*pbG = (BYTE)(T[1]*255.0f);
		*pbB = (BYTE)(T[2]*255.0f);

		delete []T;
	}
}

void ColorSpace::YUVtoRGB (float y, float u, float v, BYTE* pbR, BYTE* pbG, BYTE* pbB)
{
	int U,V,R,G,B;
	float Y=y;
	U = (int)u - 128,	V = (int)v - 128;
	
	//	R = (int)(1.164 * Y + 2.018 * U);
	//	G = (int)(1.164 * Y - 0.813 * V - 0.391 * U);
	//	B = (int)(1.164 * Y + 1.596 * V);
	R = (int)( Y + 1.403f * V);
	G = (int)( Y - 0.344f * U - 0.714f * V);
	B = (int)( Y + 1.770f * U);
	
	*pbR= MIN(255,MAX(0,R));
	*pbG= MIN(255,MAX(0,G));
	*pbB= MIN(255,MAX(0,B));
	return;
}

void ColorSpace::YUVtoRGB (CoImage* pIn, CoImage* pOut)
{
	assert (pIn->GetType() == MAT_Tfloat);
	assert (pOut->GetType() == MAT_Tbyte);
	float* prY = pIn->m_matX.data.fl[0];
	float* prU = pIn->m_matY.data.fl[0];
	float* prV = pIn->m_matZ.data.fl[0];
	
	BYTE* pbR = pOut->m_matX.data.ptr[0];
	BYTE* pbG = pOut->m_matY.data.ptr[0];
	BYTE* pbB = pOut->m_matZ.data.ptr[0];
	
	for (int i = 0; i < pIn->GetHeight() * pIn->GetWidth(); i ++)
	{
		YUVtoRGB(prY[i], prU[i], prV[i], &pbR[i], &pbG[i], &pbB[i]);
	}
}

float ColorSpace::GetColorDistance(float r1, float g1, float b1, float r2, float g2, float b2)
{
	float a = r2 - r1;
	float b = g2 - g1;
	float c = b2 - b1;

	return (float)sqrt(a*a + b*b + c*c);
}

}
