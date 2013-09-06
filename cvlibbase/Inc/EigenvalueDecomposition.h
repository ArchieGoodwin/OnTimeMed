/*! 
 * \file    EigenvalueDecomposition.h
 * \ingroup math
 * \brief   ��˱�����Ͷ� �����ײ� �е����� ��̩��.
 * \author  ����˧
 */

#ifndef _EIGENVALUEDECOMPOSITION_H_
#define _EIGENVALUEDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{
	
/** 
 * @brief	�����Ե�̩ ��˱�����Ͷ� �����ײ� �е���  
  <pre>
	  A�� �ݾ��Ե��� �� ��̩ ��˱�����Ͳ� A = V * D * V'�˳�.
	  �ޱ��� ��˱���Ե� D�� �ݰ����Ե�, ��˱���˶��Ե� V�� ����Ե��˳�.
	  �޻� ������ A = V.Multiply(D.Multiply(V.Transpose())) �˰� 
	  V.Multiply(V.Transpose())�� ��̡�Ե��˳�.
	  A�� ���ݾ��Ե��� �� ��˱���Ե� D�� 1 x 1 ̩ ����˱�������̮ 
	  2 x 2̩ ���ϰ�˱��(lambda + i*mu)����͵� �� ����ʹݰ��Ե��˳�. 
	  A*V = V*D���� ̩���溷 V̩ �۴�˼ ��˱���˶���˾ ��������.
	  �Ե� V̩ ������ ��ǿ��, ���̲� �������Ե۵�Ĵ�� ���̼�� ��˺��� 
	  �Ҽ��� A = V * D *Inverse(V)̩ ˱ã��˼ V̩ ������ ̩���ٳ�.
  </pre>
 */
class CVLIB_DECLSPEC EigenvalueDecomposition
{
public:
	/** 
	 * @brief	�龢�� 
	  <pre>   �ݾ���˾ �����װ� ��˱�����Ͷ� �����ٳ�. </pre>
	 *
	 * @param	pArg [in] : �ƶ� �Ե� 
	 *
	 * @return	D, V
	 *
	 */
	EigenvalueDecomposition(Mat* pArg, Matd* pmD=NULL, Matd* pmV=NULL);

	//!	�;˻�  
	virtual ~EigenvalueDecomposition();

	/** 
	 * @brief	��˱��̩ ���㹢�� �в���.
	 *
	 * @return	��˱��̩ ���㹢 
	 *
	 * @see  GetV()
     *
	 */
	virtual double* RealEigenvalues() const;

	/** 
	 * @brief	��˱��̩ ��㹢�� �в���.
	 *
	 * @return	��˱��̩ ��㹢 
	 *
	 * @see  GetV()
     *
	 */
	virtual double* ImagEigenvalues() const;

	/** 
	 * @brief	����ʹݰ���˱���Ե�˾ ���ٳ�.
	 *
	 * @return	D
	 *
	 * @see  GetV()
     *
	 */
	virtual Mat* D() const;

	/** 
	 * @brief	��˱���˶��Ե�˾ ���ٳ�.
	 *
	 * @return	V
	 *
	 * @par Example:
	 * @code
	 *  
	 *  int i, j;
	 *  int nDim = 3;
	 *  char szFileName[] = "c:/testEVD.bin";
	 *  
	 *  Mat mTest(nDim, nDim, MAT_Tdouble);
	 *  mTest.Zero();
     * 	for(i = 0; i < nDim; i ++)
	 *  	for(j = 0; j <= i; j ++)
	 *  	{
	 *  		if( i == j)  
	 *  			mTest.data.db[i][j] = Random::Normal();
	 *  		else
	 *  			mTest.data.db[i][j] = mTest.data.db[j][i] = Random::Normal();
	 *  	}
	 *  
	 *  EigenvalueDecomposition EVD(&mTest);
	 *  
	 *  Mat* pmD = EVD.D();
	 *  Mat* pmV = EVD.GetV();
	 *  double* rValue = EVD.RealEigenvalues();
	 *  double* iValue = EVD.ImagEigenvalues();
	 *  
	 *  FILE* pF = fopen(szFileName, "w");
	 *  fprintf(pF, "��˱������̩ ����: A = V * D * V' \n" );
	 *  
	 *  // A ���� 
	 *  fprintf(pF, "A = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
	 *  	for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		fprintf(pF, "\t%.2f", mTest.data.db[i][j] );
	 *  	}
	 *  	fprintf(pF, "\n" );
	 *  }
	 *  
	 *  // D ���� 
	 *  fprintf(pF, "D = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
	 *  	for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		fprintf(pF, "\t%.2f", pmD->data.db[i][j] );
	 *  		
	 *  	}
	 *  	fprintf(pF, "\n" );
	 *  }
	 *  
	 *  // V ���� 
	 *  fprintf(pF, "V = \n" );
	 *  for(i = 0; i < nDim; i ++)
	 *  {
	 *  	for(j = 0; j < nDim; j ++)
	 *  	{
	 *  		fprintf(pF, "\t%.2f", pmV->data.db[i][j] );
	 *  		
	 *  	}
	 *		fprintf(pF, "\n" );
	 *  }
	 *  
	 *  // �����˱���ɵ۾��� 
	 *  fprintf(pF, "d = (" );
	 *  for(i = 0; i < nDim; i ++)
	 *  	fprintf(pF, "\t%.2f", rValue[i] );
	 *  fprintf(pF, "\t)\n" );
	 *  
	 *  // ���Ϻ��˱���ɵ۾��� 
	 *  fprintf(pF, "e = (" );
	 *  for(i = 0; i < nDim; i ++)
	 *  	fprintf(pF, "\t%.2f", iValue[i] );
	 *  fprintf(pF, "\t)\n" );
	 *  
	 *  fclose(pF);
	 *  mTest.Release();
	 *  ReleaseMat(pmD);
	 *  ReleaseMat(pmV);
	 *  
	 *
	 *  Output
	 *  
	 *  ��˱������̩ ����: A = V * D * V'
	 *  A = 
	 *  	1.95	0.15	-0.92
	 *  	0.15	-0.45	-0.40
	 *  	-0.92	-0.40	-0.98
	 *  D = 
	 *  	2.24	0.00	0.00
	 * 		0.00	-0.35	0.00
	 *		0.00	0.00	-1.37
	 *  V = 
	 *		-0.95	-0.18	-0.24
	 *		-0.09	0.93	-0.35
	 *		0.28	-0.31	-0.91
	 *  d = (	2.24	-0.35	-1.37	)
	 *  e = (	0.00	0.00	0.00	)
	 *
	 * @endcode
	 * 
	 */
	virtual Mat* GetV() const;
	
private:
	//! ��, �� ��̺
	int n;
	//! �ݾ�����
	bool m_fIsSymmetric;

	//! ��˱����̩ �������Ͱᰣ
	double* d;
	//! ��˱����̩ �������Ͱᰣ
	double* e;
	//! ��˱���˶���̩ ����
	double** V;
	
	//! ���ݾ� Hessenberg����
	double** H;
	//! ���ݾ�ʲ�ֶ���̩ ���հ���
	double* ort;
	
	//! ���Ϻ������ ���㹢
	double cdivr;
	//! ���Ϻ������ ��㹢
	double cdivi;

	/** 
	 * @brief   ���Դݰ�����˺��̩ �ݾ���˧���������˾ �����ٳ�. 
	 *
	 * @return	void
	 */
	void  tred2();

	/** 
	 * @brief	�ݾ����Դݰ����� QLʲ�ֶ���˾ ������ �ܺ��˳�.
	 *
	 * @return	void
	 */
	void  tql2();

	/** 
	 * @brief	���׹ն�����˺��̩ ���ݾ�����˾ �����ٳ�. 
	 *
	 * @return	void
	 */
	void  orthes();
			
	/** 
	 * @brief	���Ϻ������̩ ���䱨�� �����ٳ�.
	 * @param	xr [in] : �����κ� ���㹢
	 * @param	xi [in] : �����κ� ��㹢
	 * @param	yr [in] : �����̼�� �� ���㹢
	 * @param	yi [in] : �����̼�� �� ��㹢
	 *
	 * @return	void
	 */
	void  cdiv(double xr, double xi, double yr, double yi);

	/** 
	 * @brief	���׹ն�����˺�Ṣ�� �� Schur��˺��̩ ���ݾ�����˾ �����ٳ�.
	 *
	 * @return	void
	 */
	void  hqr2();
};

}

#endif //_EIGENVALUEDECOMPOSITION_H_