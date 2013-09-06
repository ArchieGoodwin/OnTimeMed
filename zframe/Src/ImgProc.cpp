/*!
 * \file	ImgProc.cpp
 * \ingroup VOR
 * \brief	
 * \author	
 */

#include "ImgProc.h"
#include "cvlibbase/inc/cvlibMalloc.h"

namespace CVLib { namespace ip{

#define SHIFT             16
#define SCALE(rX)          (rX)
//#define SCALE(rX)          CVUtil::Round((rX)*(1 << SHIFT))
#define NOP(ppnA)      (ppnA)

/* lightweight convolution with 3x3 kernel */
void SepConvSmall( float*  prSrc, int nSrc_step,
						  float*  prDst, int nDst_step,
						  Size src_size,
						  const float* prKx, const float* prKy,
						  float*  prBuffer )
{
    int  nDst_width, nBuffer_step = 0;
    int  nX, nY;
	
    assert( prSrc && prDst && src_size.width > 2 && src_size.height > 2 &&
		(nSrc_step & 3) == 0 && (nDst_step & 3) == 0 &&
		(prKx || prKy) && (prBuffer || !prKx || !prKy));
	
    nSrc_step >>= 2;
    nDst_step >>= 2;
	
    nDst_width = src_size.width - 2;
	
    if( !prKx )
    {
	/* set vars, so that vertical convolution
	will write results into destination ROI and
		horizontal convolution won't run */
        src_size.width = nDst_width;
        nBuffer_step = nDst_step;
        prBuffer = prDst;
        nDst_width = 0;
    }
	
    assert( nSrc_step >= src_size.width && nDst_step >= nDst_width );
	
    src_size.height -= 3;
    if( !prKy )
    {
	/* set vars, so that vertical convolution won't run and
		horizontal convolution will write results into destination ROI and */
        src_size.height += 3;
        nBuffer_step = nSrc_step;
        prBuffer = prSrc;
        src_size.width = 0;
    }
	
    for( nY = 0; nY <= src_size.height; nY++, prSrc += nSrc_step,
		prDst += nDst_step,
		prBuffer += nBuffer_step )
    {
        float* src2 = prSrc + nSrc_step;
        float* src3 = prSrc + nSrc_step*2;
        for( nX = 0; nX < src_size.width; nX++ )
        {
            prBuffer[nX] = (float)(prKy[0]*prSrc[nX] + prKy[1]*src2[nX] + prKy[2]*src3[nX]);
        }
		
        for( nX = 0; nX < nDst_width; nX++ )
        {
            prDst[nX] = (float)(prKx[0]*prBuffer[nX] + prKx[1]*prBuffer[nX+1] + prKx[2]*prBuffer[nX+2]);
        }
    }
}

void* AdjustRect( const void* pSrcptr, int nSrc_step, int nPix_size,
		Size src_size, Size win_size, SPoint ip, Rect* pRect )
{
	Rect rect;
	const char* pchSrc = (const char*)pSrcptr;

	if( ip.x >= 0 )
	{
		pchSrc += ip.x*nPix_size;
		rect.x = 0;
	}
	else
	{
		rect.x = -ip.x;
		if( rect.x > win_size.width )
			rect.x = win_size.width;
	}

	if( ip.x + win_size.width < src_size.width )
		rect.width = win_size.width;
	else
	{
		rect.width = src_size.width - ip.x - 1;
		if( rect.width < 0 )
		{
			pchSrc += rect.width*nPix_size;
			rect.width = 0;
		}
		assert( rect.width <= win_size.width );
	}

	if( ip.y >= 0 )
	{
		pchSrc += ip.y * nSrc_step;
		rect.y = 0;
	}
	else
		rect.y = -ip.y;

	if( ip.y + win_size.height < src_size.height )
		rect.height = win_size.height;
	else
	{
		rect.height = src_size.height - ip.y - 1;
		if( rect.height < 0 )
		{
			pchSrc += rect.height*nSrc_step;
			rect.height = 0;
		}
	}

	*pRect = rect;

	return (void*)(pchSrc - rect.x*nPix_size);
};


BOOL GetRectSubPixel(const BYTE* pbSrc, int nSrc_step, Size src_size, float* prDst,
										 int nDst_step, Size win_size, Point2f center)
{                                                                           
    SPoint ip;                                                             
    float  rA11, rA12, rA21, rA22, rB1, rB2;                                   
    float rA, rB;                                                             
    int i, j;                                                               
                                                                            
    center.x -= (win_size.width - 1) * 0.5f;                                    
    center.y -= (win_size.height - 1) * 0.5f;                                   
                                                                            
    ip.x = Floor( (float)center.x );                                             
    ip.y = Floor( (float)center.y );                                             
                                                                            
    rA = float(center.x - ip.x);                                                    
    rB = float(center.y - ip.y);                                                    
    rA11 = (float)SCALE((1.f-rA)*(1.f-rB));                                     
    rA12 = (float)SCALE(rA*(1.f-rB));                                           
    rA21 = (float)SCALE((1.f-rA)*rB);                                           
    rA22 = (float)SCALE(rA*rB);                                                 
    rB1 = (float)SCALE(1.f - rB);                                               
    rB2 = (float)SCALE(rB);                                                    
                                                                            
    nSrc_step /= sizeof( pbSrc[0] );                                           
                                                                            
    if( 0 <= ip.x && ip.x + win_size.width < src_size.width &&              
        0 <= ip.y && ip.y + win_size.height < src_size.height )             
    {                                                                       
        /* extracted rectangle is totally inside the image */               
        pbSrc += ip.y * nSrc_step + ip.x;                                      
                                                              
        for( i = 0; i < win_size.height; i++, pbSrc += nSrc_step,              
                                              (char*&)prDst += nDst_step )     
        {                                                                   
            for( j = 0; j <= win_size.width - 2; j += 2 )                   
            {                                                               
                float s0 = NOP(pbSrc[j])*rA11 +                      
                              NOP(pbSrc[j+1])*rA12 +                    
                              NOP(pbSrc[j+nSrc_step])*rA21 +             
                              NOP(pbSrc[j+nSrc_step+1])*rA22;            
                float s1 = NOP(pbSrc[j+1])*rA11 +                    
                              NOP(pbSrc[j+2])*rA12 +                    
                              NOP(pbSrc[j+nSrc_step+1])*rA21 +           
                              NOP(pbSrc[j+nSrc_step+2])*rA22;            
                                                                            
                prDst[j] = (float)NOP(s0);                          
                prDst[j+1] = (float)NOP(s1);                        
            }                                                               
                                                                            
            for( ; j < win_size.width; j++ )                                
            {                                                               
                float s0 = NOP(pbSrc[j])*rA11 +                      
                              NOP(pbSrc[j+1])*rA12 +                    
                              NOP(pbSrc[j+nSrc_step])*rA21 +             
                              NOP(pbSrc[j+nSrc_step+1])*rA22;            
                                                                            
                prDst[j] = (float)NOP(s0);                          
            }                                                               
        }                                                                   
    }                                                                       
    else                                                                    
    {                                                                       
        Rect r;                                                           
                                                                            
        pbSrc = (const BYTE*)AdjustRect( pbSrc, nSrc_step*sizeof(*pbSrc),    
                               sizeof(*pbSrc), src_size, win_size,ip, &r);    
                                                                            
        for( i = 0; i < win_size.height; i++, (char*&)prDst += nDst_step )     
        {                                                                   
            const BYTE *pbSrc2 = pbSrc + nSrc_step;                           
                                                                            
            if( i < r.y || i >= r.height )                                  
                pbSrc2 -= nSrc_step;                                           
                                                                            
            for( j = 0; j < r.x; j++ )                                      
            {                                                               
                float rS0 = NOP(pbSrc[r.x])*rB1 +                     
                              NOP(pbSrc2[r.x])*rB2;                     
                                                                            
                prDst[j] = (float)NOP(rS0);                          
            }                                                               
                                                                            
            for( ; j < r.width; j++ )                                       
            {                                                               
                float rS0 = NOP(pbSrc[j])*rA11 +                      
                              NOP(pbSrc[j+1])*rA12 +                    
                              NOP(pbSrc2[j])*rA21 +                     
                              NOP(pbSrc2[j+1])*rA22;                    
                                                                            
                prDst[j] = (float)NOP(rS0);                          
            }                                                               
                                                                            
            for( ; j < win_size.width; j++ )                                
            {                                                               
                float rS0 = NOP(pbSrc[r.width])*rB1 +                 
                              NOP(pbSrc2[r.width])*rB2;                 
                                                                            
                prDst[j] = (float)NOP(rS0);                          
            }                                                               
                                                                            
            if( i < r.height )                                              
                pbSrc = pbSrc2;                                                 
        }                                                                   
    }                                                                       
                                                                            
    return 1;                                                           
}

/*
Various border types, image boundaries are denoted with '|'

* BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
* BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
* BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
* BORDER_WRAP:          cdefgh|abcdefgh|abcdefg        
* BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
*/
int borderInterpolate( int p, int len, int borderType )
{
	if( (unsigned)p < (unsigned)len )
		;
	else if( borderType == BORDER_REPLICATE )
		p = p < 0 ? 0 : len - 1;
	else if( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 )
	{
		int delta = borderType == BORDER_REFLECT_101;
		if( len == 1 )
			return 0;
		do
		{
			if( p < 0 )
				p = -p - 1 + delta;
			else
				p = len - 1 - (p - len) - delta;
		}
		while( (unsigned)p >= (unsigned)len );
	}
	else if( borderType == BORDER_WRAP )
	{
		if( p < 0 )
			p -= ((p-len+1)/len)*len;
		if( p >= len )
			p %= len;
	}
	else if( borderType == BORDER_CONSTANT )
		p = -1;
	else
		assert(false);//, "Unknown/unsupported border type" );
	return p;
}

}}