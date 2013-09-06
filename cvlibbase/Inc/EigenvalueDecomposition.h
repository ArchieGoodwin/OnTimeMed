/*! 
 * \file    EigenvalueDecomposition.h
 * \ingroup math
 * \brief   °ÖË±°ª¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 */

#ifndef _EIGENVALUEDECOMPOSITION_H_
#define _EIGENVALUEDECOMPOSITION_H_

#include "Mat.h"

namespace CVLib
{
	
/** 
 * @brief	»¨ºãÃÔµÛÌ© °ÖË±°ª¹¤ÃÍ¶¦ ¼êÃÔÂ×²÷ ¿Ğµ¹º÷  
  <pre>
	  A°¡ ´İ¾ÀÃÔµÛËÎ ÆÁ °ûÌ© °ÖË±°ª¹¤ÃÍ²÷ A = V * D * V'ËË³Ş.
	  ÊŞ±¨º· °ÖË±°ªÃÔµÛ D²÷ ´İ°¢ººÃÔµÛ, °ÖË±¹ÖÀË¶£ÃÔµÛ V²÷ ¼é°äÃÔµÛËË³Ş.
	  ³Ş»¤ ¶ğÂ×ÊŞ A = V.Multiply(D.Multiply(V.Transpose())) ËË°Ö 
	  V.Multiply(V.Transpose())²÷ ³àÌ¡ÃÔµÛËË³Ş.
	  A°¡ ¹¾´İ¾ÀÃÔµÛËÎ ÆÁ °ÖË±°ªÃÔµÛ D²÷ 1 x 1 Ì© »¨°ÖË±°ª¹¹µá¿ÍÌ® 
	  2 x 2Ì© ¸ôºÏ°ÖË±°ª(lambda + i*mu)¹¹µá¿Íµá ´ô ¹¹µá¿Í´İ°¢ÃÔµÛËË³Ş. 
	  A*V = V*Dµ¹²÷ Ì©·çËæº· VÌ© µÛ´ÉË¼ °ÖË±¹ÖÀË¶£´ÉË¾ ±ıÀ°³¯³Ş.
	  ÃÔµÛ VÌ© ¼¿°¼ËË ±ıÇ¿±Â, ¼èÊÌ²÷ ¹¦Á¬ÃùÃÔµÛµáÄ´¼è ¼ÑÊÌ¼ìºã ËØËº·àµá 
	  ¸Ò¼³»¥ A = V * D *Inverse(V)Ì© Ë±Ã£ºÂË¼ VÌ© ¼¿°¼Ëæ Ì©¼ÁÂÙ³Ş.
  </pre>
 */
class CVLIB_DECLSPEC EigenvalueDecomposition
{
public:
	/** 
	 * @brief	°é¾¢»ô 
	  <pre>   ´İ¾ÀºÂË¾ °Áº¡Â×°Ö °ÖË±°ª¹¤ÃÍ¶¦ ¼êÃÔÂÙ³Ş. </pre>
	 *
	 * @param	pArg [in] : ¸Æ¶¥ ÃÔµÛ 
	 *
	 * @return	D, V
	 *
	 */
	EigenvalueDecomposition(Mat* pArg, Matd* pmD=NULL, Matd* pmV=NULL);

	//!	ÃÍ¾Ë»ô  
	virtual ~EigenvalueDecomposition();

	/** 
	 * @brief	°ÖË±°ªÌ© »¨ºã¹¢¶¦ ÊĞ²÷³Ş.
	 *
	 * @return	°ÖË±°ªÌ© »¨ºã¹¢ 
	 *
	 * @see  GetV()
     *
	 */
	virtual double* RealEigenvalues() const;

	/** 
	 * @brief	°ÖË±°ªÌ© Ââºã¹¢¶¦ ÊĞ²÷³Ş.
	 *
	 * @return	°ÖË±°ªÌ© Ââºã¹¢ 
	 *
	 * @see  GetV()
     *
	 */
	virtual double* ImagEigenvalues() const;

	/** 
	 * @brief	¹¹µá¿Í´İ°¢°ÖË±°ªÃÔµÛË¾ °éÂÙ³Ş.
	 *
	 * @return	D
	 *
	 * @see  GetV()
     *
	 */
	virtual Mat* D() const;

	/** 
	 * @brief	°ÖË±¹ÖÀË¶£ÃÔµÛË¾ °éÂÙ³Ş.
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
	 *  fprintf(pF, "°ÖË±°ª¹¤ÃÍÌ© »¨¶Ê: A = V * D * V' \n" );
	 *  
	 *  // A ¾¥µÙ 
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
	 *  // D ¾¥µÙ 
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
	 *  // V ¾¥µÙ 
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
	 *  // »¨ºã°ÖË±°ª¹ÉµÛ¾¥µÙ 
	 *  fprintf(pF, "d = (" );
	 *  for(i = 0; i < nDim; i ++)
	 *  	fprintf(pF, "\t%.2f", rValue[i] );
	 *  fprintf(pF, "\t)\n" );
	 *  
	 *  // ¸ôºÏºã°ÖË±°ª¹ÉµÛ¾¥µÙ 
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
	 *  °ÖË±°ª¹¤ÃÍÌ© »¨¶Ê: A = V * D * V'
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
	//! ÃÔ, µÛ ½ÓÌº
	int n;
	//! ´İ¾À±¨¸Ë
	bool m_fIsSymmetric;

	//! °ÖË±°ª´ÉÌ© ³­¹¢±¨ÊÍ°á°£
	double* d;
	//! °ÖË±°ª´ÉÌ© ³­¹¢±¨ÊÍ°á°£
	double* e;
	//! °ÖË±¹ÖÀË¶£´ÉÌ© ·ºËÒ
	double** V;
	
	//! ¹¾´İ¾À HessenbergÂô»¥
	double** H;
	//! ¹¾´İ¾ÀÊ²°Ö¶®´ÍÌ© »õÊÕ°éÊß
	double* ort;
	
	//! ¸ôºÏºãº÷¾õµ¹ »¨ºã¹¢
	double cdivr;
	//! ¸ôºÏºãº÷¾õµ¹ Ââºã¹¢
	double cdivi;

	/** 
	 * @brief   »½¼Ô´İ°¢ººÂôËºµáÌ© ´İ¾ÀÂ×Ë§º÷Âú³ó¸êÃûË¾ ¼êÃÔÂÙ³Ş. 
	 *
	 * @return	void
	 */
	void  tred2();

	/** 
	 * @brief	´İ¾À»½¼Ô´İ°¢ººÃù QLÊ²°Ö¶®´ÍË¾ »¨ÂïÂÙ ÂÜºãËË³Ş.
	 *
	 * @return	void
	 */
	void  tql2();

	/** 
	 * @brief	Ã×È×¹Õ¶£°ûÂôËºµáÌ© ¹¾´İ¾À¸êÃûË¾ ¼êÃÔÂÙ³Ş. 
	 *
	 * @return	void
	 */
	void  orthes();
			
	/** 
	 * @brief	¸ôºÏºãº÷¾õµ¹Ì© ±ı²ä±¨¶¦ ¼êÃÔÂÙ³Ş.
	 * @param	xr [in] : ±ı²äËÎºã »¨ºã¹¢
	 * @param	xi [in] : ±ı²äËÎºã Ââºã¹¢
	 * @param	yr [in] : ±ı²äÊÌ¼è²÷ ºã »¨ºã¹¢
	 * @param	yi [in] : ±ı²äÊÌ¼è²÷ ºã Ââºã¹¢
	 *
	 * @return	void
	 */
	void  cdiv(double xr, double xi, double yr, double yi);

	/** 
	 * @brief	Ã×È×¹Õ¶£°ûÂôËºµá¹¢À¾ »¨ SchurÂôËºµáÌ© ¹¾´İ¾À¸êÃûË¾ ¼êÃÔÂÙ³Ş.
	 *
	 * @return	void
	 */
	void  hqr2();
};

}

#endif //_EIGENVALUEDECOMPOSITION_H_