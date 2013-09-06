#if !defined(_COLORCONVERT_H_)
#define _COLORCONVERT_H_

#include "cvlibbase/inc/CoImage.h"

namespace CVLib
{

/** 
 @brief 
 */
class CVLIB_DECLSPEC ColorSpace
{
public:
	static float GetColorDistance(float r1, float g1, float b1, float r2, float g2, float b2);

	static void RGBtoGray (const Mat& colorImg, Mat& mgray);
	static void GraytoRGB (const Mat& gray, Mat& color);
	static void Split (const Mat& color, Mat& x, Mat& y, Mat& z);
    static void BGRAtoRGB(const Mat& color1, Mat& color2);

	static void RGBtoHSV (BYTE bR, BYTE bG, BYTE bB, float *prH, float *prS, float *prV);
	static void RGBtoHSB (BYTE bR, BYTE bG, BYTE bB, float *prH, float *prS, float *prB);
	static void RGBtoHSL (BYTE bR, BYTE bG, BYTE bB, float *prH, float *prS, float *prL);
	static void RGBtoLab (BYTE bR, BYTE bG, BYTE bB, float *prL, float *pra, float *prb);
	static void RGBtoXYZ (BYTE bR, BYTE bG, BYTE bB, float *prX, float *prY, float *prZ);
	static void RGBtoYUV (BYTE bR, BYTE bG, BYTE bB, float *prY, float *prU, float *prV);
	static void RGBtoHSV (CoImage* pIn, CoImage* pOut);
	static void RGBtoHSB (CoImage* pIn, CoImage* pOut);
	static void RGBtoHSL (CoImage* pIn, CoImage* pOut);
	static void RGBtoLab (CoImage* pIn, CoImage* pOut);
	static void RGBtoXYZ (CoImage* pIn, CoImage* pOut);
	static void RGBtoYUV (CoImage* pIn, CoImage* pOut);
	static void RGBtoHSV (const Mat& src, Mat& dst);

	static void XYZtoRGB (float x, float y, float z, BYTE* pbR, BYTE* pbG, BYTE* pbB);
	static void XYZtoHSV (float x, float y, float z, float* prH, float* prS, float* prV);
	static void XYZtoHSB (float x, float y, float z, float* prH, float* prS, float* prB);
	static void XYZtoHSL (float x, float y, float z, float* prH, float* prS, float* prL);
	static void XYZtoYUV (float x, float y, float z, float* prY, float* prU, float* prV);
	static void XYZtoLab (float x, float y, float z, float* prL, float* pra, float* prb);
	static void XYZtoRGB (CoImage* pIn, CoImage* pOut);
	static void XYZtoHSV (CoImage* pIn, CoImage* pOut);
	static void XYZtoHSB (CoImage* pIn, CoImage* pOut);
	static void XYZtoHSL (CoImage* pIn, CoImage* pOut);
	static void XYZtoYUV (CoImage* pIn, CoImage* pOut);
	static void XYZtoLab (CoImage* pIn, CoImage* pOut);

	static void LabtoXYZ (float l, float a, float b, float* x, float* y, float* z);
	static void LabtoRGB (float l, float a, float b, BYTE* red, BYTE* green, BYTE* blue);
	static void LabtoHSV (float l, float a, float b, float* prH, float* prS, float* prV);
	static void LabtoHSB (float l, float a, float b, float* prH, float* prS, float* prB);
	static void LabtoHSL (float l, float a, float b, float* prH, float* prS, float* prL);
	static void LabtoYUV (float l, float a, float b, float* prY, float* prU, float* prV);
	static void LabtoXYZ (CoImage* pIn, CoImage* pOut);
	static void LabtoRGB (CoImage* pIn, CoImage* pOut);
	static void LabtoHSV (CoImage* pIn, CoImage* pOut);
	static void LabtoHSB (CoImage* pIn, CoImage* pOut);
	static void LabtoHSL (CoImage* pIn, CoImage* pOut);
	static void LabtoYUV (CoImage* pIn, CoImage* pOut);

	static void HSVtoRGB (float h, float s, float v, BYTE* pbR, BYTE* pbG, BYTE* pbB);
	static void HSVtoXYZ (float h, float s, float v, float* prX, float* prY, float* prZ);
	static void HSVtoHSB (float h, float s, float v, float* prH, float* prS, float* prB);
	static void HSVtoHSL (float h, float s, float v, float* prH, float* prS, float* prL);
	static void HSVtoYUV (float h, float s, float v, float* prY, float* prU, float* prV);
	static void HSVtoLab (float h, float s, float v, float* prL, float* pra, float* prb);
	static void HSVtoRGB (CoImage* pIn, CoImage* pOut);
	static void HSVtoXYZ (CoImage* pIn, CoImage* pOut);
	static void HSVtoHSB (CoImage* pIn, CoImage* pOut);
	static void HSVtoHSL (CoImage* pIn, CoImage* pOut);
	static void HSVtoYUV (CoImage* pIn, CoImage* pOut);
	static void HSVtoLab (CoImage* pIn, CoImage* pOut);

	static void HSBtoRGB (float h, float s, float b, BYTE* pbR, BYTE* pbG, BYTE* pbB);
	static void HSBtoXYZ (float h, float s, float b, float* prX, float* prY, float* prZ);
	static void HSBtoHSV (float h, float s, float b, float* prH, float* prS, float* prV);
	static void HSBtoHSL (float h, float s, float b, float* prH, float* prS, float* prL);
	static void HSBtoYUV (float h, float s, float b, float* prY, float* prU, float* prV);
	static void HSBtoLab (float h, float s, float b, float* prL, float* pra, float* prb);
	static void HSBtoRGB (CoImage* pIn, CoImage* pOut);
	static void HSBtoXYZ (CoImage* pIn, CoImage* pOut);
	static void HSBtoHSV (CoImage* pIn, CoImage* pOut);
	static void HSBtoHSL (CoImage* pIn, CoImage* pOut);
	static void HSBtoYUV (CoImage* pIn, CoImage* pOut);
	static void HSBtoLab (CoImage* pIn, CoImage* pOut);

	static void HSLtoRGB (float h, float s, float l, BYTE* pbR, BYTE* pbG, BYTE* pbB);
	static void HSLtoXYZ (float h, float s, float l, float* prX, float* prY, float* prZ);
	static void HSLtoHSV (float h, float s, float l, float* prH, float* prS, float* prV);
	static void HSLtoHSB (float h, float s, float l, float* prH, float* prS, float* prB);
	static void HSLtoYUV (float h, float s, float l, float* prY, float* prU, float* prV);
	static void HSLtoLab (float h, float s, float l, float* prL, float* pra, float* prb);
	static void HSLtoRGB (CoImage* pIn, CoImage* pOut);
	static void HSLtoXYZ (CoImage* pIn, CoImage* pOut);
	static void HSLtoHSV (CoImage* pIn, CoImage* pOut);
	static void HSLtoHSB (CoImage* pIn, CoImage* pOut);
	static void HSLtoYUV (CoImage* pIn, CoImage* pOut);
	static void HSLtoLab (CoImage* pIn, CoImage* pOut);

	static void YUVtoRGB (float y, float u, float v, BYTE* pbR, BYTE* pbG, BYTE* pbB);
	static void YUVtoXYZ (float y, float u, float v, float* prX, float* prY, float* prZ);
	static void YUVtoHSV (float y, float u, float v, float* prH, float* prS, float* prV);
	static void YUVtoHSB (float y, float u, float v, float* prH, float* prS, float* prB);
	static void YUVtoHSL (float y, float u, float v, float* prH, float* prS, float* prL);
	static void YUVtoLab (float y, float u, float v, float* prL, float* pra, float* prb);
	static void YUVtoRGB (CoImage* pIn, CoImage* pOut);
	static void YUVtoXYZ (CoImage* pIn, CoImage* pOut);
	static void YUVtoHSV (CoImage* pIn, CoImage* pOut);
	static void YUVtoHSB (CoImage* pIn, CoImage* pOut);
	static void YUVtoHSL (CoImage* pIn, CoImage* pOut);
	static void YUVtoLab (CoImage* pIn, CoImage* pOut);
private:
	static float Fxyz(float t);
};

}

#endif //_COLORCONVERT_H_