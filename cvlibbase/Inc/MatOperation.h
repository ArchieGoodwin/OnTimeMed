/*!
 * \file	MatOperation.h
 * \ingroup base
 * \brief	�Ե��຤˾ ̡�� �ܺ��˾ ��̩��.
 * \author	����˧
 */
#ifndef _MATOPERATION_H_
#define _MATOPERATION_H_

#include "CoImage.h"

namespace CVLib 
{

#define CVLIB_C            1
#define CVLIB_L1           2
#define CVLIB_L2           4
#define CVLIB_NORM_MASK    7
#define CVLIB_RELATIVE     8
#define CVLIB_DIFF         16
#define CVLIB_MINMAX       32

class CVLIB_DECLSPEC MatOp
{
public:
	
	// �Ե��溷 �輳�� 4��������˾ ���� �ױ�̩ �Ե۵� �ٳ�.
	static bool CropMat (Mat* pDest, const Mat* pMat, const Rect& rect);
	//�Ե�˾ ��ʭ 0 - 255���˵� �������ٳ�.
	static int Normalize(Mat* pMat, float rMin, float rMax);

	// �Ե�˾ �����ٳ�.
	static void CopyMat (Mat* pDst, const Mat* pSrc, TYPE dsttype);
	static void CopyMat(Mat* pDst, const Mat* pSrc, const Rect& srcRect, const Point2i& dstpt=Point2i());
	static void CopyMat(Mat* pDst, const Mat* pSrc, int nLeft1, int nTop1, int nRight1, int nBottom1, int nLeft2, int nTop2);

	// ���˶��� �����ٳ�.
	static bool CopyVec (Vec* pDst, const Vec* pSrc, TYPE type);

	//  �Ե�̩ ���ױ��� �����ٳ�. A = B + C 
	static void Add (Mat* pA, const Mat* pB, const Mat* pC);

	//	�Ե�̩ ������ �����ٳ�. A = B - C  
	static void Sub (Mat* pA, const Mat* pB, const Mat* pC);

	// �Ե۱� ���˶�̩ ���ױ��� �����ٳ�. A = B * C  
	static void Mul (Vec* pA, const Mat* pB, const Vec* pC);

	// ���˶�̮ �Ե�̩ ���ױ��� �����ٳ�. A = B * C 
	static void Mul (Vec* pA, const Vec* pB, const Mat* pC);

	// x' * A * x �� �˺��ײ� �ܺ��˳�.
	static float TrXAX (const Vec* pvX, const Mat* pmA);

	// ���̼� �Ե�̩ ��̡�Ե�˾ ���ٳ�. A = B'  
	static void Transpose(Mat* pA, const Mat* pB);

	// ���̼� �Ե�̩ ��̡�Ե�˾ ���ٳ�. A = A'
	static void Transpose(Mat* pA);

	// A = B  
	static void Equal(Vec* pA, const Vec* pB);

	// ���˶� ���ױ��� �����ٳ�. A = B + C  
	static void Add (Vec* pA, const Vec* pB, const Vec* pC);

	// ���˶�̩ ������ �����ٳ�. A = B - C  
	static void Sub (Vec* pA, const Vec* pB, const Vec* pC);

	// ���˶�̩ ���ױ��� �����ٳ�. C = A * B'  
	static void Mul (Mat* pC, const Vec* pA, const Vec* pB);

	// ��̡�ֵᶣ̮ ���˶�̮̩ ���ױ��� �����ٳ�. A' * B  
	static double DotProduct (const Vec* pA, const Vec* pB);

	// C = A * B * A'  
	static void ABTrA (Mat* pmC, const Mat* pmA, const Mat* pmB);

	// C = A' * B * A  
	static void TrABA (Mat* pmC, const Mat* pmA, const Mat* pmB);

	// �Ե�̩ ������ױ��� �����ٳ�. B = r * A  
	static void Mul (Mat* pB, double rReal, const Mat* pA);

	// ����̮ ���˶�̩ ���ױ��� �����ٳ�.
	static void Mul (Vec* pB, double rReal, const Vec* pA);

	// �Ե�̩ ���ױ��຤˾ �����ٳ�. C = A * B  
	static void Mul (Mat* pC, const Mat* pA, const Mat* pB);

	// ���̼� �Ե�˾ �ݰ��� �Ե۵� ��ǳ�.
	static void Diag(Mat* pA, double rVal);

	// B = A' * A. 
	static void TrAA(Mat* pmB, const Mat* pmA);

	// B = A * A'. 
	static void ATrA(Mat* pmB, const Mat* pmA);

	// Trace (A) 
	static double Trace(const Mat* pmA);

	static double Sum(const Vec* pvA);
	static double Sum(const Mat* pmA);
	static void SumRows(const Mat* pmA, Vec* pvB);
	static void SumCols(const Mat* pmA, Vec* pvB);
	
	static void inRange(const Mat& src, const Mat& lb, const Mat& rb, Mat& dst);
	static void inRange(const Mat& src, const double lb, const double rb, Mat& dst);
	static void inRange(const CoImage& src, const double lb[3], const double rb[3], Mat& dst, int dims);

	static void Normalize( const Mat& src, Mat& dst, double alpha=1, double beta=0,
					int norm_type=NORM_L2, TYPE dtype=MAT_Tbyte, const Mat& mask=Mat());

	static void AddWeighted(const Mat& src1, double alpha, const Mat& src2, double beta, double gamma, Mat& dst);
	static void GEMM(const Mat* matA, const Mat* matB, double alpha,
		const Mat* matC, double beta, Mat* matD, int flags=0);
	/* Matrix transform: dst = A*B + C, C is optional */
	static void MatMulAdd(const Mat* src1, const Mat* src2, const Mat* src3, Mat* dst );
	static double ScalarProduct(Mat* pmA, Mat *pmB);
};

void CVLIB_DECLSPEC CartToPolar( const Mat& X, const Mat& Y, Mat& Mag, Mat& Angle, bool angleInDegrees );

}

#endif //_MATOPERATION_H_