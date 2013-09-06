
/*!
 * \file
 * \brief	Ãä±Ù¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author	±®½èË§
 */
#if !defined (_REGRESSOR_H_)
#define _REGRESSOR_H_

#include "ClassifierABC.h"

namespace CVLib
{

/**
 @brief		ººÂôÃä±Ù¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC LinearRegressor : public ClassifierABC
{
	friend class LinearRegressorTrainer;
public:
	//! Construction and Destruction
	LinearRegressor();
	virtual ~LinearRegressor();
	virtual ClassifierABC* New() const {return new LinearRegressor;}

	//! Initialization
	virtual void	Release ();

	//! Operations for loading and saving
	virtual int		FromFile (FILE* pFile);
	virtual int		ToFile (FILE* pFile) const;

	//! Operations
	virtual float	Eval(Vec* pvFea);

	float GetConstCoeff();
	void SetConstCoeff(float rConf);
	Vec& GetCoeffVec() {return m_vCoeff;}
protected:
	Vec m_vCoeff;
};

/**
 @brief		2½ÓÃä±Ù¿Ðµ¹º÷
 */
class CVLIB_DECLSPEC QuadraticRegressor : public LinearRegressor
{
	friend class QuadraticRegressorTrainer;
public:
	virtual ClassifierABC* New() const {return new QuadraticRegressor;}
	virtual void	Release ();
	virtual float	Eval(Vec* pvFea);
	static Vec* iConvert (Vec* pvFea);
};

class CVLIB_DECLSPEC LeastSquareMachine : public LinearRegressor
{
	friend class LeastSquareTrainer;
public:
	virtual ClassifierABC* New() const {return new LeastSquareMachine;}
	virtual float	Eval(Vec* pvFea);
};

}

#endif //_REGRESSOR_H_