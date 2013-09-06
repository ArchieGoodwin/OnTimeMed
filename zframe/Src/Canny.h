#if !defined(_CANNY_H_)
#define _CANNY_H_

#include "cvlibbase/inc/Mat.h"

namespace CVLib
{

/** 
 @brief CannyËæ¼è°Á¾¥Ë¾ ¼êÃÔÂÙ³Þ.
 */
class CVLIB_DECLSPEC Canny : public Object
{
public:
	Canny()
	{
		m_nHighThreshold = 60;
		m_nLowThreshold = 30;
	};
	~Canny(){};
	void	Create (int nSizeX, int nSizeY);
	void	Release ();
	void	Process (Mat* pmIn, Mat* pmOut = NULL);
	int m_nHighThreshold;
	int m_nLowThreshold;
private:
	Mat m_mGauss;
	Mat m_mTemp;

	void cannygray(Mat* pmIn, Mat* pmOut = NULL);
	void cannycolor(Mat* pmIn, Mat* pmOut = NULL);
};

namespace ip
{
	// mode : 0 // dx, dy uses
	// mode : 1 // dx
	// mode : 2 // dy
CVLIB_DECLSPEC	void canny (const Mat& src, Mat& dst, float low_thresh, float high_thresh, int mode=0);
}

}

#endif //_CANNY_H_