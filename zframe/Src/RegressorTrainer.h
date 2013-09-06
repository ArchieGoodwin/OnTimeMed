
/*!
 * \file
 * \brief	Ãä±Ù·ºÂôË¾ ÃªµÚ»¤¿Ô±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */

#ifndef _RegressorTrainer_H_
#define _RegressorTrainer_H_

#include "Engine.h"
#include "ClassifierABC.h"

namespace CVLib
{

class CVLIB_DECLSPEC RegressorTrainerABC : public Engine
{
public:
	RegressorTrainerABC() {};
	virtual ~RegressorTrainerABC() {};

	virtual void SetData (Mat* pmExamples/*Vec* pvExamples, int nNum, Vec* pvWeights = NULL*/, Vec* pvObj) = 0;
	virtual int	Train (ClassifierABC* pMachine) = 0;
	virtual void SetData (DataSet* pData);
	virtual void Release () = 0;
};

class CVLIB_DECLSPEC LinearRegressorTrainer : public RegressorTrainerABC
{
public:
	LinearRegressorTrainer() {m_pmData=NULL;m_pvObj=NULL;};
	virtual ~LinearRegressorTrainer();
	
	virtual void SetData (Mat* pmExamples, Vec* pvObj);
	virtual int	Train (ClassifierABC* pMachine);
	virtual MachineABC* Train(DataSet* pData);
	virtual void Release ();
protected:
	Mat* m_pmData;
	Vec* m_pvObj;
};

class CVLIB_DECLSPEC QuadraticRegressorTrainer : public LinearRegressorTrainer
{
public:
	virtual void SetData (Mat* pmExamples/*Vec* pvExamples, int nNum, Vec* pvWeights = NULL*/, Vec* pvObj);
	virtual MachineABC* Train(DataSet* pData);
};

class CVLIB_DECLSPEC LeastSquareTrainer : public Engine
{
public:
	LeastSquareTrainer() {}
	virtual ~LeastSquareTrainer() {}
	virtual MachineABC* Train(DataSet* pData);
};

}

#endif //_RegressorTrainer_H_