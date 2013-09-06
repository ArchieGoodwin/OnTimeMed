
/*!
 * \file
 * \brief ÃÅº÷ÀË°ûµ½Ë¾ ±Ëº¤Â×²÷ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®Âî½è
 */
#ifndef _Histogram_H_
#define _Histogram_H_

#include "cvlibbase/inc/Mat.h"
#include "cvlibbase/inc/CoImage.h"
#include "SparseMat.h"

namespace CVLib 
{
	
class CVLIB_DECLSPEC Histogram : public Object
{
public:
	struct ColorBin
	{
		int		nBinX;
		int		nBinY;
		int		nBinZ;
		int	Bins ()
		{
			int nBins;
			nBins = 
				(nBinX < 0 ? 1 : nBinX) * 
				(nBinY < 0 ? 1 : nBinY) * 
				(nBinZ < 0 ? 1 : nBinZ);
			return nBins;
		}
	};

	bool	fNormalize;			/**< ¼³°öÃù±¨¸Ë */
	bool	m_fRegionStyle;		/**< ÂôÀô±¨¸Ë(4°¢Âô, À°Ìº) */

	//! Construction and Destruction
	Histogram(bool	bRectOrEllipse=true) : m_fRegionStyle(bRectOrEllipse) { fNormalize = false; }
	~Histogram() {};

	//! Operations
	void	Process (Mat* pIn, Vec* pOutHisto);
	void	Process(CoImage* pImg, Vec* pOutHisto, int nQuantStep);
	int		Process(CoImage* pSrc, int* red=0, int* green=0, int* blue=0, int* gray=0);
	int		Process(Mat* pSrc, int* red=0, int* green=0, int* blue=0, int* gray=0);

	void	RGBHist (CoImage* pImage, ColorBin* pBin, Vec* pvHist);

	float	DistanceL1 (Vec* pvHist1, Vec* pvHist2);
	float	Bhattacharyya (Vec* pvHist1, Vec* pvHist2);
	void	Process(CoImage* pImg, Rect rect,Vec* pOutHisto, int nQuantStep);
protected: 
	void	Process_Rect(CoImage* pImg, Rect rect,Vec* pOutHisto, int nQuantStep); 
	void	Process_Ellipse(CoImage* pImg, Rect rect,Vec* pOutHisto, int nQuantStep); 
};

void CVLIB_DECLSPEC 
CalcBackProject( const CoImage& image, const int* channels, MatND& hist, Mat& backProject, 
				const float** ranges, double scale=1, bool uniform=true );

void CVLIB_DECLSPEC 
CalcBackProject( Mat** const images, int nimages, const int* channels, MatND& hist, Mat& backProject,
				const float** ranges, double scale=1, bool uniform=true );

void CVLIB_DECLSPEC 
CalcHist(const CoImage& image, const Mat& mask, MatND& hist, 
		 int dims, const int* histSize, const float** ranges, 
		 bool uniform=true, bool accumulate=false);

}

#endif //_Histogram_H_