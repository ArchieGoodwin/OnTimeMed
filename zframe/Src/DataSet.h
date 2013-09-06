
/*!

 * \file    DataSet.h
 * \ingroup	base
 * \brief	����е����� ��̩��.
 * \author	����˧
 */

#if !defined (_DATASET_H_)
#define _DATASET_H_

#include "cvlibbase/inc/Mat.h"
#include "cvlibbase/inc/XFile.h"
#include <stdio.h>

namespace CVLib
{

/**
 * @brief  ���귺�ҿе���
 */
class CVLIB_DECLSPEC DataSet : public Object
{
public:
	enum 
	{
		DS_ALL, 
		DS_ADDRESS
	};
public:
	//! ê�ڻ���̩ �� 
	int		m_nCount;			
	//! ê�ڻ���̩ ��̺�� 
	int		m_nDim;	
	//! ��������˾ �����ײ� �ɵ�, �����溷 ������, ����溷 ����
	double*	m_prCls;			
	//! ��������̩ �ɵ� 
	double**	m_pprData;			
	//! ê�ڻ����̩ �˱� 
	double*	m_prWeights;

	//! �龢�� 
	DataSet();
	DataSet (int nDim, int nCount, int nMode = DS_ALL);
	DataSet (const DataSet& other);
	DataSet (const char* szFilename);
	//! �;˻� 
	~DataSet();

	/**
	 * @brief   �е����� �����ٳ�.
	 *
	 * @param   nDim   [in] : ê�ڻ���̩ ��̺��
	 * @param   nCount [in] : ê�ڻ���̩ ��
	 * @param   nMode  [in] : ���궮˦�һ� 
	 * @return  �°��׷� 1, ��²�׷� 0
	 *
	 * @see  Release ()
	 *
	 */
	virtual int		Create (int nDim, int nCount, int nMode = DS_ALL);
	int			FlagCreate () const {return m_nFlagCreate;}

	/**
	 * @brief   ����� ���ͱ���˾ �͸��ٳ�. 
	 *
	 * @return  void  
	 *
	 * @see  Create(), Init()
	 *
	 */
	virtual void	Release ();

	/**
	 * @brief   �������궦 �輳�� ���εṢ�� �����ٳ�.
	 *
	 * @param   pFile [in] : ���μ輬�� 
	 * @return  �°��׷� 1, ��²�׷� 0
	 *
	 * @see  ToFile()
	 *
	 */
	virtual	int		FromFile (const char* szFilename);
	virtual int		FromFile (FILE* pFile);
	virtual int		FromFile (XFile* pFile);
	
	/**
	 * @brief   �������궦 �輳�� ������ ����ٳ�.
	 *
	 * @param   pFile [in] : ���μ輬�� 
	 * @return  �°��׷� 1, ��²�׷� 0 
	 *
	 * @see  FromFile()
	 *
	 */
	virtual	int		ToFile (const char* szFilename) const;
	virtual int		ToFile (FILE* pFile) const;
	virtual int		ToFile (XFile* pFile) const;

	/**
	 * @brief   �輳�� �е����� ���ܴ� ���걶�㶦 �в���.
	 *
	 * @param   nClass [in] : �е������� 
	 * @return  ���걶��  
	 *
	 */
	Matd*			ToMat();
	void			FromMat(Matd* pM);

	virtual int		GetSampleNum (int nClass);
	virtual int		GetClassNum ();
	virtual int		Statistics(double* prMin, double* prMax, double* prMean);

	inline int		GetDims() const {return m_nDim;}
	inline int		GetCount() const {return m_nCount;}
	inline int		GetMaxCount() const {return m_nMaxCount;}
	int				CopyFrom (const DataSet& other);
	int				Add (double* prData, double rCls, double rWei=0.0);

	void			SetData (int nIdx, double* prData, double rCls, double rWei=0.0);
	void			SortClsIdx();
protected:
	//! ���궮˦�һ� 
	int		m_nMode;
	int		m_nFlagCreate;
	//! ê�ڻ����̩ �״ݱ��� 
	int		m_nMaxCount;
	int		m_nGrowBy;

	/**
	 * @brief   �е����� �����ٳ�.
	 *
	 * @return  void  
	 *
	 * @see  Create()
	 *
	 */
	void	Init ();
};

}

#endif //_DATASET_H_