#ifndef _SOBEL_H_
#define _SOBEL_H_

#include "cvlibbase/inc/Mat.h"

namespace CVLib 
{

class CVLIB_DECLSPEC Sobel
{
public:
	Sobel();
	~Sobel();
	virtual void	Process (Mat* pIn, Mat* pOut = NULL);

	void			GetIntensity_X (const Mat* pmatIn, Mat* pmatOut = NULL);
	void			GetIntensity_Y (const Mat* pmatIn, Mat* pmatOut = NULL);

	static void		GradientMagnitude(const Mat* pmIn, Mat* pmOut);
	static void		GradientMagnitudeX(const Mat* pmIn, Mat* pmOut, bool fABS=true);
	static void		GradientMagnitudeY(const Mat* pmIn, Mat* pmOut, bool fABS=true);

	static void		ProcessEOF (Mat* pmatIn, Mat* pmatOut/* = NULL*/, int nThreshold = 0);
	static void		EOField (Mat* pmatIn, Mat* pmatOut, Mati* pmEdge=NULL);
};

namespace ip
{

CVLIB_DECLSPEC void EOField (Mat* pmatIn, Mat* pmatOut, Mati* pmEdge=NULL);
CVLIB_DECLSPEC void Sobel(const Mat& src, Mat& dst, int dx, int dy, int aperture_size );
CVLIB_DECLSPEC void Laplace(const Mat& src, Mat& dst, int aperture_size );

}

}

#endif //_SOBEL_H_
