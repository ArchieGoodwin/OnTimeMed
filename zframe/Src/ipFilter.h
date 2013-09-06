
/*!
 * \file	ipFilter.h
 * \ingroup ipCore
 * \brief   Ãùº¬µØÁÄ¶¦ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.  
 * \author  ±®½èË§ 
 */
#ifndef _ipFilter_H_
#define _ipFilter_H_

#include "ipCoreABC.h"

namespace CVLib
{

class CVLIB_DECLSPEC ipFilter : public ipCorePump
{
	DECLARE_PUMP(ipFilter);
public:
	enum filterintype{MAT, VEC, VECRow, VECCol};
	ipFilter();
	ipFilter(const ipFilter& from);
	ipFilter(int* pnKernel, int nKsize, int nKFactor, int nKOffset, filterintype type=MAT);
	ipFilter(float* prKernel, int nKsize, float rKFactor, int nKOffset, filterintype type=MAT);
	ipFilter(double* prKernel, int nKsize, double rKFactor, int nKOffset, filterintype type=MAT);
	virtual ~ipFilter() { Release(); }

	static int		KernelFactor(int* pnKernel, int nLen);
	static float	KernelFactor(float* prKernel, int nLen);
	static double	KernelFactor(double* prKernel, int nLen);

	static bool Process2(long* kernel, long Ksize, long Kfactor, long Koffset, Mat* pmatOrg, Mat* pmatDest = NULL);
	static bool Process2(long* kernel, long Ksize, long Kfactor, long Koffset, Rect* pFilteringRect, Mat* pmatOrg, Mat* pmatDest = NULL);

	int Create(int* pnKernel, int nKsize, int nKFactor, int nKOffset, filterintype type=MAT);
	int Create(float* prKernel, int nKsize, float rKFactor, int nKOffset, filterintype type=MAT);
	int Create(double* prKernel, int nKsize, double rKFactor, int nKOffset, filterintype type=MAT);
	inline bool FlagCreate() const {return m_fcreate;}
	virtual bool Process (CoImage* pSrc, CoImage* pDst = NULL);
	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
	virtual bool Process (Vec* pvSrc, Vec* pvDst = NULL);
	virtual void Release ();
public:
	// parameters
	filterintype	m_filterType;
	int* m_pnKernel;
	float* m_prKernel;
	int m_nKSize;
	int m_nKFactor;
	float m_rKFactor;
	int m_nKOffset;

protected:
	bool m_fcreate;
	bool iProcessMat (Mat* pmSrc, Mat* pmDst = NULL);
	bool iProcessVecRow (Mat* pmSrc, Mat* pmDst = NULL);
	bool iProcessVecCol (Mat* pmSrc, Mat* pmDst = NULL);
};

class CVLIB_DECLSPEC ipFilterSep : public ipFilter
{
	DECLARE_PUMP(ipFilterSep)
public:
	ipFilterSep();
	ipFilterSep(const ipFilter& from);
	ipFilterSep(int* pnKernel, int nKsize);
	ipFilterSep(float* pnKernel, int nKsize);
	ipFilterSep(double* pnKernel, int nKsize);
	virtual ~ipFilterSep();

	virtual bool Process (Mat* pmSrc, Mat* pmDst = NULL);
private:
	Mat* m_pmTemp;
};

class CVLIB_DECLSPEC Filter
{
public:
	static bool Process2D (int* pnKernel, int nKsize, int nKFactor, int nKOffset, Mat* pmatOrg, Mat* pmatDest);

	//functions in linearfilter.h

	static int	 KernelFactor(int* pnKernel, int nLen);
	static float	KernelFactor(float* prKernel, int nLen);

	static bool Conv2Sep(int* pnKernel, int nKSize, int nKFactor, Mat* pmIn, Mat* pmDest);
	static bool Conv2Sep(float* prKernel, int nKSize, float rKFactor, Mat* pmIn, Mat* pmDest);
};

namespace ip
{
void sepFilter2D(const Mat& src, Mat& dst, int ddepth, Mat& kernelX, Mat& _kernelY, Point2i anchor,
                    double delta, int borderType );
void filter2D(const Mat& src, Mat& dst, int ddepth, Mat& kernel, Point2i anchor, 
					double delta, int borderType );
}

}

#endif //_ipCOREABC_H_