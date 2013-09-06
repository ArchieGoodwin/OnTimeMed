/*!
 * \file	Random.cpp
 * \ingroup base
 * \brief	Ë§Êàºã¶¦ »¶ºÂÂ×²÷ ¿Ðµ¹º÷Ì© »¨Âï¹¢¶¦ ¼³Ì©ÂÙ³Þ.
 * \author	±®½èË§
 */

#include "Random.h"

namespace CVLib
{

Random::Random()
{
}

Random::~Random()
{
}

void Random::Init ()
{
	srand( (unsigned)time( NULL ) );
}

int Random::Uniform (bool fInit)
{
	if (fInit)
	{
		static int nInit = 0 ;

		if( !nInit ) 
		{		/* Â­ÊýÂ×³Þ·² seed¶¦ ½ó±¨ÃùÂÙ³Þ. */
			Init() ;
			nInit = 1 ;
		}
	}

	return rand(); 
}

float Random::BoundedUniform (float rMin /*= 0.0001f*/, float rMax /*= 1.0f*/, bool fInit)
{
	return ( (rMax - rMin) * (float)Random::Uniform(fInit) / (float)RAND_MAX ) + rMin ; 
}

float Random::Normal(float rMean /*= 0.0f*/, float rStd /*= 1.0f*/)
{
	static int nFlag = 0;
	float rUnif1, rUnif2 ;
	float rGauss;

	rUnif1 = BoundedUniform(0.00001f, 1.0f);
	rUnif2 = BoundedUniform(0.00001f, 1.0f);

	rGauss = nFlag ?  rMean + rStd * (float)sqrt(-2.0*log(rUnif1))*(float)cos(2*CVLIB_PI*rUnif2) :
		     rMean + rStd* (float)sqrt(-2.0*log(rUnif1))*(float)sin(2*CVLIB_PI*rUnif2);

	nFlag = nFlag ? 0 : 1;
	return rGauss;
}

float Random::Exponential(float rLambda)
{
	return (float)(-1. / rLambda * log(1 - Random::BoundedUniform()));
}

float Random::Cauchy(float rMedian, float rSigma)
{
	return(rMedian + rSigma * (float)tan(CVLIB_PI*(Random::BoundedUniform()-0.5)));
}

float Random::LogNormal(float rMean, float rStdv)
{
	float zm = rMean*rMean;
	float zs = rStdv*rStdv;
	return((float)exp(Random::Normal((float)log(zm/sqrt(zs + zm)), (float)sqrt(log(zs/zm+1)) )));
}

int Random::Geometric(float rP)
{
	return((int)(log(1-Random::BoundedUniform()) / log(rP)) + 1);
}

}
