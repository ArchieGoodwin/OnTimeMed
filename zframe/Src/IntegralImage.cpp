/*!
 * \file
 * \brief ¼¬¹¤Ãùº¬Ë¾ °éÂ×±¨ Ì¡ÂÙ ·º´ÂË¾ ¼³Ì©ÂÙ³Þ.
 * \author CholU Kim
 */
#include "IntegralImage.h"
#include "cvlibbase/inc/cvlibMalloc.h"

namespace CVLib {

/**
 @brief		´¸½£°ßÁì
 */
const int SqrTable[] =
{
 16384,  16129,  15876,  15625,  15376,  15129,  14884,  14641,
 14400,  14161,  13924,  13689,  13456,  13225,  12996,  12769,
 12544,  12321,  12100,  11881,  11664,  11449,  11236,  11025,
 10816,  10609,  10404,  10201,  10000,   9801,   9604,   9409,
  9216,   9025,   8836,   8649,   8464,   8281,   8100,   7921,
  7744,   7569,   7396,   7225,   7056,   6889,   6724,   6561,
  6400,   6241,   6084,   5929,   5776,   5625,   5476,   5329,
  5184,   5041,   4900,   4761,   4624,   4489,   4356,   4225,
  4096,   3969,   3844,   3721,   3600,   3481,   3364,   3249,
  3136,   3025,   2916,   2809,   2704,   2601,   2500,   2401,
  2304,   2209,   2116,   2025,   1936,   1849,   1764,   1681,
  1600,   1521,   1444,   1369,   1296,   1225,   1156,   1089,
  1024,    961,    900,    841,    784,    729,    676,    625,
   576,    529,    484,    441,    400,    361,    324,    289,
   256,    225,    196,    169,    144,    121,    100,     81,
    64,     49,     36,     25,     16,      9,      4,      1,
     0,      1,      4,      9,     16,     25,     36,     49,
    64,     81,    100,    121,    144,    169,    196,    225,
   256,    289,    324,    361,    400,    441,    484,    529,
   576,    625,    676,    729,    784,    841,    900,    961,
  1024,   1089,   1156,   1225,   1296,   1369,   1444,   1521,
  1600,   1681,   1764,   1849,   1936,   2025,   2116,   2209,
  2304,   2401,   2500,   2601,   2704,   2809,   2916,   3025,
  3136,   3249,   3364,   3481,   3600,   3721,   3844,   3969,
  4096,   4225,   4356,   4489,   4624,   4761,   4900,   5041,
  5184,   5329,   5476,   5625,   5776,   5929,   6084,   6241,
  6400,   6561,   6724,   6889,   7056,   7225,   7396,   7569,
  7744,   7921,   8100,   8281,   8464,   8649,   8836,   9025,
  9216,   9409,   9604,   9801,  10000,  10201,  10404,  10609,
 10816,  11025,  11236,  11449,  11664,  11881,  12100,  12321,
 12544,  12769,  12996,  13225,  13456,  13689,  13924,  14161,
 14400,  14641,  14884,  15129,  15376,  15625,  15876,  16129,
 16384,  16641,  16900,  17161,  17424,  17689,  17956,  18225,
 18496,  18769,  19044,  19321,  19600,  19881,  20164,  20449,
 20736,  21025,  21316,  21609,  21904,  22201,  22500,  22801,
 23104,  23409,  23716,  24025,  24336,  24649,  24964,  25281,
 25600,  25921,  26244,  26569,  26896,  27225,  27556,  27889,
 28224,  28561,  28900,  29241,  29584,  29929,  30276,  30625,
 30976,  31329,  31684,  32041,  32400,  32761,  33124,  33489,
 33856,  34225,  34596,  34969,  35344,  35721,  36100,  36481,
 36864,  37249,  37636,  38025,  38416,  38809,  39204,  39601,
 40000,  40401,  40804,  41209,  41616,  42025,  42436,  42849,
 43264,  43681,  44100,  44521,  44944,  45369,  45796,  46225,
 46656,  47089,  47524,  47961,  48400,  48841,  49284,  49729,
 50176,  50625,  51076,  51529,  51984,  52441,  52900,  53361,
 53824,  54289,  54756,  55225,  55696,  56169,  56644,  57121,
 57600,  58081,  58564,  59049,  59536,  60025,  60516,  61009,
 61504,  62001,  62500,  63001,  63504,  64009,  64516,  65025
};	// SqrTable[]

#define cast_sqr_macro(x)  SqrTable[(x)+128]
#define cast_macro(x)  (x)

void	IntegralImage::Process (Mat* pmImg, Mat* pmSum, Mat* pmSquare/* = NULL */)
{
	switch (pmImg->Type())
	{
	case MAT_Tbyte:
		iProcess_Byte(pmImg, pmSum, pmSquare);
		break;
	case MAT_Tfloat:
		iProcess_float(pmImg, pmSum, pmSquare);
		break;
	case MAT_Tint:
		iProcess_int(pmImg, pmSum, pmSquare);
		break;
        default:
            break;
	}
}

/**
 @brief  ¼¬¹¤Ãùº¬Ë¾ °éÂÙ³Þ.
 @param		pmImg		[in]	ËÓµÙÃùº¬
 @param		pmSum		[out]	±Ëº¤´ô ¼¬¹¤Ãùº¬.
 @param		pmSquare	[out]	±Ëº¤´ô ´¸½£°ß¼¬¹¤Ãùº¬.
		ËË ÁÄµ¹¸¡À¾°¡ ËÓµÙ»¤Ëæ NULLËË·² ´¸½£°ß¼¬¹¤Ãùº¬Ë¾ °éÂ×¼è Ê±²÷³Þ.
 */
void	IntegralImage::iProcess_Byte (Mat* pmImg, Mat* pmSum, Mat* pmSquare/* = NULL */)
{
#define worktype int

	assert (pmImg->Type() == MAT_Tbyte);
	if (pmSum)
		assert (pmSum->Type() == MAT_Tint);
	if (pmSquare)
		assert (pmSquare->Type() == MAT_Tdouble);

	int x, y;                                                   
	int s;                                                  
	double sq;                                               
	BYTE* src = pmImg->data.ptr[0];
	int* mSum = pmSum ? pmSum->data.i[0]: NULL;
	double* sqsum = pmSquare ? pmSquare->data.db[0]: NULL;
//	int* sqsum = pmSquare ? pmSquare->data.i[0]: NULL;
	Size size = pmImg->SizeObject();
	int srcstep, sumstep, sqsumstep;
	srcstep = size.width;

	if (mSum)
	{
		memset( mSum, 0, (size.width+1) * sizeof(mSum[0]));
		sumstep = size.width+1;
		mSum += sumstep + 1;                                         
	}

	if (mSum==NULL && sqsum)
	{
		memset( sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     
		sqsumstep = size.width + 1;
		sqsum += sqsumstep + 1;

		for( y = 0; y < size.height; y++, src += srcstep, sqsum += sqsumstep)
		{ 
			sqsum[-1] = 0;

			for( x = 0, s = 0, sq = 0; x < size.width; x++ )
			{
				double tq = cast_sqr_macro(src[x]);
				sq += tq;
				tq = sqsum[x - sqsumstep] + sq;
				sqsum[x] = (double)tq;
			}
		}
	} 
	else
	{
		if( sqsum )                                                 
		{
			memset( sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     
			sqsumstep = size.width + 1;
			sqsum += sqsumstep + 1;
		}                                                           

		if( sqsum == 0 )
		{
			for( y = 0; y < size.height; y++, 
				src += srcstep, mSum += sumstep )
			{
				mSum[-1] = 0;
				for( x = 0, s = 0; x < size.width; x++ )
				{
					int t = cast_macro(src[x]);
					s += t;
					mSum[x] = mSum[x - sumstep] + s;
				}
			}
		}
		else
		{
			for( y = 0; y < size.height; y++, 
				src += srcstep, mSum += sumstep, sqsum += sqsumstep)
			{ 
				mSum[-1] = 0;
				sqsum[-1] = 0;

				for( x = 0, s = 0, sq = 0; x < size.width; x++ )
				{
					worktype it = src[x];
					int t = cast_macro(it);
					double tq = cast_sqr_macro(it);
					s += t;
					sq += tq;
					t = mSum[x - sumstep] + s;
					tq = sqsum[x - sqsumstep] + sq;
					mSum[x] = t;
					sqsum[x] = (double)tq;
				}
			}
		}
	}

	return;
}

void	IntegralImage::iProcess_int (Mat* pmImg, Mat* pmSum, Mat* pmSquare/* = NULL */)
{
#define worktype int

	assert (pmImg->Type() == MAT_Tint);
	if (pmSum)
		assert (pmSum->Type() == MAT_Tint);
	if (pmSquare)
		assert (pmSquare->Type() == MAT_Tdouble);

	int x, y;                                                   
	int s;                                                  
	double sq;                                               
	int* src = pmImg->data.i[0];
	int* mSum = pmSum?pmSum->data.i[0]:NULL;
	double* sqsum = pmSquare ? pmSquare->data.db[0]: NULL;
//	int* sqsum = pmSquare ? pmSquare->data.i[0]: NULL;
	Size size = pmImg->SizeObject();
	int srcstep, sumstep, sqsumstep;
	srcstep = size.width;

	if (mSum)
	{
		memset( mSum, 0, (size.width+1) * sizeof(mSum[0]));
		sumstep = size.width+1;
		mSum += sumstep + 1;                                         
	}

	if( sqsum )                                                 
	{
		memset( sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     
		sqsumstep = size.width + 1;
		sqsum += sqsumstep + 1;
    }
	if (mSum==NULL && sqsum)
	{
		memset( sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     
		sqsumstep = size.width + 1;
		sqsum += sqsumstep + 1;

		for( y = 0; y < size.height; y++, src += srcstep, sqsum += sqsumstep)
		{ 
			sqsum[-1] = 0;

			for( x = 0, s = 0, sq = 0; x < size.width; x++ )
			{
				double tq = cast_sqr_macro(src[x]);
				sq += tq;
				tq = sqsum[x - sqsumstep] + sq;
				sqsum[x] = (double)tq;
			}
		}
	} 
	else
	{
		if( sqsum == 0 )
		{
			for( y = 0; y < size.height; y++, 
				src += srcstep, mSum += sumstep )
			{
				mSum[-1] = 0;
				for( x = 0, s = 0; x < size.width; x++ )
				{
					int t = cast_macro(src[x]);
					s += t;
					mSum[x] = mSum[x - sumstep] + s;
				}
			}
		}
		else
		{
			for( y = 0; y < size.height; y++, 
				src += srcstep, mSum += sumstep, sqsum += sqsumstep)
			{ 
				mSum[-1] = 0;
				sqsum[-1] = 0;

				for( x = 0, s = 0, sq = 0; x < size.width; x++ )
				{
					worktype it = src[x];
					int t = cast_macro(it);
					double tq = it * it;
					s += t;
					sq += tq;
					t = mSum[x - sumstep] + s;
					tq = sqsum[x - sqsumstep] + sq;
					mSum[x] = t;
					sqsum[x] = (double)tq;
				}
			}
		}
	}


	return;
}
void	IntegralImage::iProcess_float(Mat* pmImg, Mat* pmSum, Mat* pmSquare/* = NULL */)
{
#undef worktype
#define worktype float

	assert (pmImg->Type() == MAT_Tfloat);
	if (pmSum)
		assert (pmSum->Type() == MAT_Tdouble);
	if (pmSquare)
		assert (pmSquare->Type() == MAT_Tdouble);

	int x, y;                                                   
	float s;                                                  
	double sq;                                               
	float* src = pmImg->data.fl[0];
	double* mSum = pmSum?pmSum->data.db[0]:NULL;
	double* sqsum = pmSquare ? pmSquare->data.db[0]: NULL;
	//	int* sqsum = pmSquare ? pmSquare->data.i[0]: NULL;
	Size size = pmImg->SizeObject();
	int srcstep, sumstep, sqsumstep;
	srcstep = size.width;

	if (mSum)
	{
		memset( mSum, 0, (size.width+1) * sizeof(mSum[0]));
		sumstep = size.width+1;
		mSum += sumstep + 1;                                         
	}
	if( sqsum )                                                 
	{
		memset( sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     
		sqsumstep = size.width + 1;
		sqsum += sqsumstep + 1;
	}                                                           

	if (mSum==NULL && sqsum)
	{
		for( y = 0; y < size.height; y++, src += srcstep, sqsum += sqsumstep)
		{ 
			sqsum[-1] = 0;

			for( x = 0, sq = 0; x < size.width; x++ )
			{
				double tq = src[x]*src[x];
				sq += tq;
				tq = sqsum[x - sqsumstep] + sq;
				sqsum[x] = (double)tq;
			}
		}
	}
	else
	{
		if( sqsum == 0 )
		{
			for( y = 0; y < size.height; y++, 
				src += srcstep, mSum += sumstep )
			{
				mSum[-1] = 0;
				for( x = 0, s = 0; x < size.width; x++ )
				{
					float t = cast_macro(src[x]);
					s += t;
					mSum[x] = mSum[x - sumstep] + s;
				}
			}
		}
		else
		{
			for( y = 0; y < size.height; y++, 
				src += srcstep, mSum += sumstep, sqsum += sqsumstep)
			{ 
				mSum[-1] = 0;
				sqsum[-1] = 0;

				for( x = 0, s = 0, sq = 0; x < size.width; x++ )
				{
					worktype it = src[x];
					float t = cast_macro(it);
					double tq = it * it;
					s += t;
					sq += tq;
					t = (float)(mSum[x - sumstep] + s);
					tq = sqsum[x - sqsumstep] + sq;
					mSum[x] = t;
					sqsum[x] = (double)tq;
				}
			}
		}
	}

	return;
}
void	IntegralImage::ProcessEx (Mat* pmImg, Mat* pmSum, Mat* pmTilted, Mat* pmSquare/* = NULL*/)
{
#undef worktype
#define worktype int

	assert (pmImg->Type() == MAT_Tbyte);
	assert (pmSum->Type() == MAT_Tint);
	if (pmTilted)
		assert (pmTilted->Type() == MAT_Tint);
	if (pmSquare)
		assert (pmSquare->Type() == MAT_Tdouble);

	int x, y;                                                   
	int s;                                                  
	double sq;                                               
	int* buf = 0;
	BYTE* src = pmImg->data.ptr[0];
	int* mSum = pmSum->data.i[0];
	int* mTilted = pmTilted ? pmTilted->data.i[0]: NULL;
	double* sqsum = pmSquare ? pmSquare->data.db[0]: NULL;
	int srcstep, sumstep, tiltedstep, sqsumstep;
	Size size = pmImg->SizeObject ();
	srcstep = size.width;

	memset( mSum, 0, (size.width+1) * sizeof(mSum[0]));
	sumstep = size.width+1;
    mSum += sumstep + 1;                                         

	if( sqsum )                                                 
	{
		memset( sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     
		sqsumstep = size.width + 1;
		sqsum += sqsumstep + 1;
    }                                                           

	if( mTilted )
	{
		memset( mTilted, 0, (size.width+1)*sizeof(mTilted[0]));
		tiltedstep = size.width + 1;
		mTilted += tiltedstep + 1;
	}

	if( sqsum == 0 && mTilted == 0 )
	{
		for( y = 0; y < size.height; y++, 
			src += srcstep, mSum += sumstep )
		{
			mSum[-1] = 0;
			for( x = 0, s = 0; x < size.width; x++ )
			{
				int t = cast_macro(src[x]);
				s += t;
				mSum[x] = mSum[x - sumstep] + s;
			}
		}
	}
	else if( mTilted == 0 )
    {
		for( y = 0; y < size.height; y++, 
			src += srcstep, mSum += sumstep, sqsum += sqsumstep)
		{ 
			mSum[-1] = 0;
			sqsum[-1] = 0;

			for( x = 0, s = 0, sq = 0; x < size.width; x++ )
			{
				worktype it = src[x];
				int t = cast_macro(it);
				double tq = cast_sqr_macro(it);
				s += t;
				sq += tq;
				t = mSum[x - sumstep] + s;
				tq = (int)sqsum[x - sqsumstep] + sq;
				mSum[x] = t;
				sqsum[x] = (double)tq;
			}
		}
	}
	else
	{                                                           
		if( sqsum == 0 )
		{
			assert(0);
			return;
		}

		MALLOC (buf, (size.width + 1 )* sizeof(buf[0]), int);
		mSum[-1] = mTilted[-1] = 0;
		sqsum[-1] = 0;

		for( x = 0, s = 0, sq = 0; x < size.width; x++ )
		{
			worktype it = src[x];
            int t = cast_macro(it);
            int tq = cast_sqr_macro(it);
            buf[x] = mTilted[x] = t;
			s += t;
			sq += tq;
			mSum[x] = s;
			sqsum[x] = (double)sq;
        }                                                       
                                                                
        if( size.width == 1 )                                   
            buf[1] = 0;                                         
                                                                
        for( y = 1; y < size.height; y++ )                      
        {                                                       
            worktype it;                                        
            int t0;                                         
            int tq0;                                      
                                                                
            src += srcstep;                                     
            mSum += sumstep;                                     
            sqsum += sqsumstep;                                 
            mTilted += tiltedstep;                               
                                                                
            it = src[0/*x*/];                                   
            s = t0 = cast_macro(it);                            
            sq = tq0 = cast_sqr_macro(it);                      
                                                                
            mSum[-1] = 0;                                        
            sqsum[-1] = 0;                                      
            /*mTilted[-1] = buf[0];*/                            
            mTilted[-1] = mTilted[-tiltedstep];                   
                                                                
            mSum[0] = mSum[-sumstep] + t0;                        
            sqsum[0] = sqsum[-sqsumstep] + tq0;                 
            mTilted[0] = mTilted[-tiltedstep] + t0 + buf[1];      
                                                                
            for( x = 1; x < size.width - 1; x++ )               
            {                                                   
                int t1 = buf[x];                            
                buf[x-1] = t1 + t0;                             
                it = src[x];                                    
                t0 = cast_macro(it);                            
                tq0 = cast_sqr_macro(it);                       
                s += t0;                                        
                sq += tq0;                                      
                mSum[x] = mSum[x - sumstep] + s;                  
                sqsum[x] = sqsum[x - sqsumstep] + sq;           
                t1 += buf[x+1] + t0 + mTilted[x - tiltedstep - 1];
                mTilted[x] = t1;                                 
            }                                                   
                                                                
            if( size.width > 1 )                                
            {                                                   
                int t1 = buf[x];                            
                buf[x-1] = t1 + t0;                             
                it = src[x];    /*+*/                           
                t0 = cast_macro(it);                            
                tq0 = cast_sqr_macro(it);                       
                s += t0;                                        
                sq += tq0;                                      
                mSum[x] = mSum[x - sumstep] + s;                  
                sqsum[x] = sqsum[x - sqsumstep] + sq;           
                mTilted[x] = t0 + t1 + mTilted[x - tiltedstep - 1];
                buf[x] = t0;                                    
			}
		}
		FREE (buf);
	}

	return;
}

}
