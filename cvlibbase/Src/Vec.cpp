/*!
 * \file Vec.cpp
 * \ingroup base
 * \brief ¹ÖÀË¶£¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */

#include "Vec.h"
#include "cvlibutil.h"
#include "string.h"

namespace CVLib 
{

#define ExternalStorage 0
#define NonExternalStorage 1

Vec::Vec(int len, TYPE type)
{
	Create (len, type);
}

Vec::Vec(void* ptr, int len, TYPE type)
{
	Create (ptr, len, type);
}

Vec::Vec (const Vec& vec)
{
	if (this == &vec)
		return;
	Create (vec, TRUE);
}

Vec::~Vec()
{
	Release();
}

int Vec::Create(int len, TYPE type)
{
	this->len = len;
	this->type = type;
	this->m_nMaxLen = len;
	switch (type)
	{
	case MAT_Tbyte:
		step = sizeof(BYTE);
		MALLOC (data.ptr, len * step, BYTE);
		break;
	case MAT_Tshort:
		step = sizeof(short);
		MALLOC (data.s, len * step, short);
		break;
	case MAT_Tint:
		step = sizeof(int);
		MALLOC (data.i, len * step, int);
		break;
	case MAT_Tfloat:
		step = sizeof(float);
		MALLOC (data.fl, len * step, float);
		break;
	case MAT_Tdouble:
		step = sizeof(double);
		MALLOC (data.db, len * step, double);
		break;
	}
	depend = ExternalStorage;
	return 1;
}

int Vec::Create(void* ptr, int len, TYPE type)
{
	this->len = len;
	this->type = type;
	this->m_nMaxLen = len;
	switch (type)
	{
	case MAT_Tbyte:
		step = sizeof(BYTE);
		data.ptr = (BYTE*)ptr;
		break;
	case MAT_Tshort:
		step = sizeof(short);
		data.s = (short*)ptr;
		break;
	case MAT_Tint:
		step = sizeof(int);
		data.i = (int*)ptr;
		break;
	case MAT_Tfloat:
		step = sizeof(float);
		data.fl = (float*)ptr;
		break;
	case MAT_Tdouble:
		step = sizeof(double);
		data.db = (double*)ptr;
		break;
	}
	depend = NonExternalStorage;
	return 1;
}

int	Vec::Create(const Vec& vec, BOOL fCopy/* = FALSE*/)
{
	int nRet = Create (vec.len, vec.type);
	if (nRet)
	{
		if (fCopy)
			memcpy (data.ptr, vec.data.ptr, Step() * Length());
		return 1;
	}
	return 0;
}

void Vec::Release ()
{
	if (data.ptr)
	{
		if (depend == ExternalStorage)
			FREE(data.ptr);
		data.ptr = 0;
		len=0;
	}
}

void Vec::Resize(int num)
{
	if (m_nMaxLen >= num) len = num; 
	else if (depend == ExternalStorage)
	{
		if (IsValid())
		{
			Vec v(num, Type());
			memcpy (v.data.ptr, data.ptr, len * step);
			Release();
			Create (v, TRUE);
		}
		else
			assert(false);
	}
}

Vec& Vec::operator=(const Vec& vecSrc)
{
	if (this==&vecSrc)
		return *this;
	if (Length() == vecSrc.Length() && Type() == vecSrc.Type())
	{
		memcpy (data.ptr, vecSrc.data.ptr, Length() * Step());
	}
	else
	{
		Release();
		Create (vecSrc, TRUE);
	}
	return *this;
}

Vec& Vec::operator=(double value)
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Length();j++)
			data.ptr[j] = (BYTE)value;
		break;
	case MAT_Tshort:
		for(j=0;j<Length();j++)
			data.s[j] = (short)value;
		break;
	case MAT_Tint:
		for(j=0;j<Length();j++)
			data.i[j] = (int)value;
		break;
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)value;
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)value;
		break;
	}
	return *this;
}

void Vec::Zero ()
{
	memset (data.ptr, 0, step * Length());
}

double	Vec::Value (int i) const
{
	switch (Type())
	{
	case MAT_Tbyte:
		return data.ptr[i];
	case MAT_Tshort:
		return data.s[i];
	case MAT_Tint:
		return data.i[i];
	case MAT_Tfloat:
		return data.fl[i];
	case MAT_Tdouble:
		return data.db[i];
	}
	return 0.0f;
}

double Vec::Norm1() const
{
	double rSum = 0;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE *ptr_x = data.ptr;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += abs(*ptr_x);
		}
		break;
	case MAT_Tshort:
		{
			short *ptr_x = data.s;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += abs(*ptr_x);
		}
		break;
	case MAT_Tint:
		{
			int *ptr_x = data.i;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += abs(*ptr_x);
		}
		break;
	case MAT_Tfloat:
		{
			float *ptr_x = data.fl;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += fabs(*ptr_x);
		}
		break;
	case MAT_Tdouble:
		{
			double *ptr_x = data.db;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += fabs(*ptr_x);
		}
		break;
	}
	
	return rSum;
}

double Vec::Norm2() const
{
	return sqrt(Energy());
}

double Vec::Energy() const
{
	double rSum = 0;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE *ptr_x = data.ptr;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += (*ptr_x) * (*ptr_x);
		}
		break;
	case MAT_Tshort:
		{
			short *ptr_x = data.s;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += (*ptr_x) * (*ptr_x);
		}
		break;
	case MAT_Tint:
		{
			int *ptr_x = data.i;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += (*ptr_x) * (*ptr_x);
		}
		break;
	case MAT_Tfloat:
		{
			float *ptr_x = data.fl;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += (*ptr_x) * (*ptr_x);
		}
		break;
	case MAT_Tdouble:
		{
			double *ptr_x = data.db;
			for (int i = 0; i < Length(); i++, ptr_x++)
				rSum += (*ptr_x) * (*ptr_x);
		}
		break;
	}
	
	return rSum;
}

double Vec::NormInf() const
{
	double rMax_val=0;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE *ptr_x = data.ptr;
			rMax_val = (BYTE)abs(*ptr_x++);
			for (int i = 1; i < Length(); i++, ptr_x++)
			{
				int rZ = (BYTE)abs(*ptr_x);
				if (rMax_val < rZ)
					rMax_val = rZ;
			}
		}
		break;
	case MAT_Tshort:
		{
			short *ptr_x = data.s;
			rMax_val = (short)abs(*ptr_x++);
			for (int i = 1; i < Length(); i++, ptr_x++)
			{
				int rZ = (short)abs(*ptr_x);
				if (rMax_val < rZ)
					rMax_val = rZ;
			}
		}
		break;
	case MAT_Tint:
		{
			int *ptr_x = data.i;
			rMax_val = (int)abs(*ptr_x++);
			for (int i = 1; i < Length(); i++, ptr_x++)
			{
				int rZ = (int)abs(*ptr_x);
				if (rMax_val < rZ)
					rMax_val = rZ;
			}
		}
		break;
	case MAT_Tfloat:
		{
			float *ptr_x = data.fl;
			rMax_val = (float)fabs(*ptr_x++);
			for (int i = 1; i < Length(); i++, ptr_x++)
			{
				float rZ = (float)fabs(*ptr_x);
				if (rMax_val < rZ)
					rMax_val = rZ;
			}
		}
		break;
	case MAT_Tdouble:
		{
			double *ptr_x = data.db;
			rMax_val = (double)fabs(*ptr_x++);
			for (int i = 1; i < Length(); i++, ptr_x++)
			{
				double rZ = (double)fabs(*ptr_x);
				if (rMax_val < rZ)
					rMax_val = rZ;
			}
		}
		break;
	}

	return rMax_val;
}

Vec* Vec::SubVec(int nFirst, int nLast, BOOL fShare /*= FALSE*/) const
{
	Vec* pvRet = NULL;
	if (fShare)
	{
		pvRet = new Vec(data.ptr + Step() * nFirst, nLast - nFirst + 1, Type());
	}
	else
	{
		pvRet = new Vec(nLast - nFirst + 1, Type());
		memcpy (pvRet->data.ptr, data.ptr + Step() * nFirst, Step() * (nLast - nFirst + 1));
	}
	return pvRet;
}

int Vec::FromFile (const char* szFilename)
{
	return Object::FromFile(szFilename);
}

int Vec::FromFile (FILE* pFile)
{
	if (IsValid())
		Release ();
	int nLen, nType;
	fread (&nLen, sizeof(int), 1, pFile);
	fread (&nType, sizeof(int), 1, pFile);
	
	Create(nLen, (TYPE)nType);
	fread (data.ptr, nLen * Step(), 1, pFile);
	
	return 1;
}
int Vec::FromFile (XFile* pFile)
{
	if (IsValid())
		Release ();
	int nLen, nType;
	pFile->Read(&nLen, sizeof(int), 1);
	pFile->Read(&nType, sizeof(int), 1);
	
	Create(nLen, (TYPE)nType);
	pFile->Read(data.ptr, nLen * Step(), 1);
	
	return 1;
}

int Vec::ToFile (const char* szFilename) const
{
	return Object::ToFile(szFilename);
}

int Vec::ToFile (FILE* pFile) const
{
	if (!IsValid())
		return 0;
	int nLen, nType;
	nLen = Length();
	fwrite (&nLen, sizeof(int), 1, pFile);
	nType = Type();
	fwrite (&nType, sizeof(int), 1, pFile);
	fwrite (data.ptr, nLen * Step(), 1, pFile);
	return 1;
}
int Vec::ToFile (XFile* pFile) const
{
	if (!IsValid())
		return 0;
	int nLen, nType;
	nLen = Length();
	pFile->Write(&nLen, sizeof(int), 1);
	nType = Type();
	pFile->Write(&nType, sizeof(int), 1);
	pFile->Write(data.ptr, nLen * Step(), 1);
	return 1;
}

double Vec::Unit()
{
	double rNorm2 = Norm2();
	switch (Type())
	{
	case MAT_Tfloat:
		{
			float* ptr_x = data.fl;
			for (int i = 0; i < Length(); i++)
				ptr_x[i] /= (float)rNorm2;
		}
		break;
	case MAT_Tdouble:
		{
			double* ptr_x = data.db;
			for (int i = 0; i < Length(); i++)
				ptr_x[i] /= rNorm2;
		}
		break;
	}
	return rNorm2;
}

/**
	@memo     Takes the absolute valueof each element.
	@doc      Takes the absolute valueof each element.
	@return   Nothing.
*/
void Vec::Abs()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Length();j++)
			data.ptr[j] = (BYTE)abs(data.ptr[j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Length();j++)
			data.s[j] = (short)abs(data.s[j]);
		break;
	case MAT_Tint:
		for(j=0;j<Length();j++)
			data.i[j] = (int)abs(data.i[j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)fabs(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)fabs(data.db[j]);
		break;
	}
}

/**
	@memo     Calculates the autocorrelation of the vector.
	@doc      Calculates the autocorrelation of the vector with
			a given lag (default lag is 1).
	@param    lag     The lag.  
	@return   The autocorrelation.
*/
double Vec::AutoCorrelation( const int lag /*= 1*/ ) const
{
    int n = this->Length();
    double mean = .0;
    double d = n*this->Var( &mean );    
    double s = .0;
	int i;
	switch (Type())
	{
	case MAT_Tbyte:
		for(i=0;i<n-lag;i++) 
			s += ( data.ptr[i]-mean )*( data.ptr[i+lag]-mean ); 
		break;
	case MAT_Tshort:
		for(i=0;i<n-lag;i++) 
			s += ( data.s[i]-mean )*( data.s[i+lag]-mean ); 
		break;
	case MAT_Tint:
		for(i=0;i<n-lag;i++) 
			s += ( data.i[i]-mean )*( data.i[i+lag]-mean ); 
		break;
	case MAT_Tfloat:
		for(i=0;i<n-lag;i++) 
			s += ( data.fl[i]-mean )*( data.fl[i+lag]-mean ); 
		break;
	case MAT_Tdouble:
		for(i=0;i<n-lag;i++) 
			s += ( data.db[i]-mean )*( data.db[i+lag]-mean ); 
		break;
	}
    return s/d;
}
/**
	@memo     Finds the maximum element in the vector.
	@doc      Finds the maximum element in the vector.  
	@return   The maximum value of the elements.
*/
double Vec::Max() const
{
	int i;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE dMax = data.ptr[0];
			for (i=1;i < Length(); i++)
				dMax = MAX(dMax,data.ptr[i]);
			return dMax;
		}
		break;
	case MAT_Tshort:
		{
			short dMax = data.s[0];
			for (i=1;i < Length(); i++)
				dMax = MAX(dMax,data.s[i]);
			return dMax;
		}
		break;
	case MAT_Tint:
		{
			int dMax = data.i[0];
			for (i=1;i < Length(); i++)
				dMax = MAX(dMax,data.i[i]);
			return dMax;
		}
		break;
	case MAT_Tfloat:
		{
			float dMax = data.fl[0];
			for (i=1;i < Length(); i++)
				dMax = MAX(dMax,data.fl[i]);
			return dMax;
		}
		break;
	case MAT_Tdouble:
		{
			double dMax = data.db[0];
			for (i=1;i < Length(); i++)
				dMax = MAX(dMax,data.db[i]);
			return dMax;
		}
		break;
	}
	return 0;
}
double Vec::Max(int& iPos) const
{
	int i;
	iPos = 0;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE dMax;
			dMax = data.ptr[0];
			for (i=1;i < Length(); i++)
			{
				if (dMax < data.ptr[i])
				{
					dMax = data.ptr[i];
					iPos=i;
				}
			}
			return dMax;
		}
		break;
	case MAT_Tshort:
		{
			short dMax;
			dMax = data.s[0];
			for (i=1;i < Length(); i++)
			{
				if (dMax < data.s[i])
				{
					dMax = data.s[i];
					iPos=i;
				}
			}
			return dMax;
		}
		break;
	case MAT_Tint:
		{
			int dMax;
			dMax = data.i[0];
			for (i=1;i < Length(); i++)
			{
				if (dMax < data.i[i])
				{
					dMax = data.i[i];
					iPos=i;
				}
			}
			return dMax;
		}
		break;
	case MAT_Tfloat:
		{
			float dMax;
			dMax = data.fl[0];
			for (i=1;i < Length(); i++)
			{
				if (dMax < data.fl[i])
				{
					dMax = data.fl[i];
					iPos=i;
				}
			}
			return dMax;
		}
		break;
	case MAT_Tdouble:
		{
			double dMax;
			dMax = data.db[0];
			for (i=1;i < Length(); i++)
			{
				if (dMax < data.db[i])
				{
					dMax = data.db[i];
					iPos=i;
				}
			}
			return dMax;
		}
		break;
	}
	return 0;
}
double Vec::Mean() const
{
	int i;
	double sum=0;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			for (i=0;i < Length(); i++)
				sum += data.ptr[i];
		}
		break;
	case MAT_Tshort:
		{
			for (i=0;i < Length(); i++)
				sum += data.s[i];
		}
		break;
	case MAT_Tint:
		{
			for (i=0;i < Length(); i++)
				sum += data.i[i];
		}
		break;
	case MAT_Tfloat:
		{
			for (i=0;i < Length(); i++)
				sum += data.fl[i];
		}
		break;
	case MAT_Tdouble:
		{
			for (i=0;i < Length(); i++)
				sum += data.db[i];
		}
		break;
	}
	return sum / Length();
}
//////////////////////////////////////////////////////////////////////////
#define ELEM_SWAP_BYTE(a,b) { register BYTE t=(a);(a)=(b);(b)=t; }
#define ELEM_SWAP_short(a,b) { register short t=(a);(a)=(b);(b)=t; }
#define ELEM_SWAP_int(a,b) { register int t=(a);(a)=(b);(b)=t; }
#define ELEM_SWAP_float(a,b) { register float t=(a);(a)=(b);(b)=t; }
#define ELEM_SWAP_double(a,b) { register double t=(a);(a)=(b);(b)=t; }

#define I_DEF_quick_select( flavor, SrcType)  \
static SrcType quick_select_##flavor(SrcType arr[], int n) \
{\
    int low, high ;\
    int median;\
    int middle, ll, hh;\
	\
    low = 0 ; high = n-1 ; median = (low + high) / 2;\
    for (;;) {\
        if (high <= low) /* One element only */\
            return arr[median] ;\
		\
        if (high == low + 1) {  /* Two elements only */\
            if (arr[low] > arr[high])\
                ELEM_SWAP_##flavor(arr[low], arr[high]) ;\
            return arr[median] ;\
        }\
		\
		/* Find median of low, middle and high items; swap into position low */\
		middle = (low + high) / 2;\
		if (arr[middle] > arr[high])    ELEM_SWAP_##flavor(arr[middle], arr[high]) ;\
		if (arr[low] > arr[high])       ELEM_SWAP_##flavor(arr[low], arr[high]) ;\
		if (arr[middle] > arr[low])     ELEM_SWAP_##flavor(arr[middle], arr[low]) ;\
		\
		/* Swap low item (now in position middle) into position (low+1) */\
		ELEM_SWAP_##flavor(arr[middle], arr[low+1]) ;\
		\
		/* Nibble from each end towards middle, swapping items when stuck */\
		ll = low + 1;\
		hh = high;\
		for (;;) {\
			do ll++; while (arr[low] > arr[ll]) ;\
			do hh--; while (arr[hh]  > arr[low]) ;\
			\
			if (hh < ll)\
				break;\
			\
			ELEM_SWAP_##flavor(arr[ll], arr[hh]) ;\
		}\
		\
		/* Swap middle item (in position low) back into correct position */\
		ELEM_SWAP_##flavor(arr[low], arr[hh]) ;\
		\
		/* Re-set active partition */\
		if (hh <= median)\
			low = ll;\
        if (hh >= median)\
			high = hh - 1;\
    }\
}

I_DEF_quick_select(BYTE, BYTE)
I_DEF_quick_select(short, short)
I_DEF_quick_select(int, int)
I_DEF_quick_select(float, float)
I_DEF_quick_select(double, double)

#undef ELEM_SWAP
//////////////////////////////////////////////////////////////////////////
double Vec::Median() const
{
	Vec copy(*this);
	double median;
	bool doSorting = false;
	if (doSorting)
	{
		copy.Sort();
		const int len = copy.Length();
		const bool isOdd = len%2!=0;
		switch(Type())
		{
		case MAT_Tbyte:
			median = isOdd ? data.ptr[len/2] : .5*(data.ptr[len/2-1]+data.ptr[len/2]);
			break;
		case MAT_Tshort:
			median = isOdd ? data.s[len/2] : .5*(data.s[len/2-1]+data.s[len/2]);
			break;
		case MAT_Tint:
			median = isOdd ? data.i[len/2] : .5*(data.i[len/2-1]+data.i[len/2]);
			break;
		case MAT_Tfloat:
			median = isOdd ? data.fl[len/2] : .5*(data.fl[len/2-1]+data.fl[len/2]);
			break;
		case MAT_Tdouble:
			median = isOdd ? data.db[len/2] : .5*(data.db[len/2-1]+data.db[len/2]);
			break;
		}
	} 
	else 
	{
		// *much* faster alternative kindly provided by Nicolas Devillard
		// see code snip above
		switch(Type())
		{
		case MAT_Tbyte:
			median = quick_select_BYTE( data.ptr, copy.Length() );
			break;
		case MAT_Tshort:
			median = quick_select_short( data.s, copy.Length() );
			break;
		case MAT_Tint:
			median = quick_select_int( data.i, copy.Length() );
			break;
		case MAT_Tfloat:
			median = quick_select_float( data.fl, copy.Length() );
			break;
		case MAT_Tdouble:
			median = quick_select_double( data.db, copy.Length() );
			break;
		}
	}
	
	return median;
}
Vec	Vec::Trim( const double percentage /*= .10*/ ) const
{
	assert( percentage>=.0 && percentage<=1. );
	
	if ( percentage>=1. ) {
		
		// by convention
		Vec v(1, this->Type());
		switch(Type())
		{
		case MAT_Tbyte:
			v.data.ptr[0]= (BYTE)this->Median();
			break;
		case MAT_Tshort:
			v.data.s[0]= (short)this->Median();
			break;
		case MAT_Tint:
			v.data.i[0]= (int)this->Median();
			break;
		case MAT_Tfloat:
			v.data.fl[0]= (float)this->Median();
			break;
		case MAT_Tdouble:
			v.data.db[0]= (double)this->Median();
			break;
		}
		return v;
	} 
	else 
	{
		const int len = this->Length();
		const int head_tail_len = (int)(.5+len*percentage/2.);
		
		Vec trimmedVec(*this);
		trimmedVec.Sort();
		return trimmedVec.Range( head_tail_len, len-head_tail_len-1 );
	}
}
double Vec::TrimmedMean( const double percentage /*= .10*/ ) const
{
	Vec trimmedVec = this->Trim( percentage );
	return trimmedVec.Mean();
}
double Vec::TrimmedVar( const double percentage /*= .10*/ ) const
{
	Vec trimmedVec = this->Trim( percentage );
	return trimmedVec.Var();
}
double Vec::TrimmedStd( const double percentage /*= .10*/ ) const
{
	Vec trimmedVec = this->Trim( percentage );
	return trimmedVec.Std();
}
double Vec::Min() const
{
	int i;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE dMin = data.ptr[0];
			for (i=1;i < Length(); i++)
				dMin = MIN(dMin,data.ptr[i]);
			return dMin;
		}
		break;
	case MAT_Tshort:
		{
			short dMin = data.s[0];
			for (i=1;i < Length(); i++)
				dMin = MIN(dMin,data.s[i]);
			return dMin;
		}
		break;
	case MAT_Tint:
		{
			int dMin = data.i[0];
			for (i=1;i < Length(); i++)
				dMin = MIN(dMin,data.i[i]);
			return dMin;
		}
		break;
	case MAT_Tfloat:
		{
			float dMin = data.fl[0];
			for (i=1;i < Length(); i++)
				dMin = MIN(dMin,data.fl[i]);
			return dMin;
		}
		break;
	case MAT_Tdouble:
		{
			double dMin = data.db[0];
			for (i=1;i < Length(); i++)
				dMin = MIN(dMin,data.db[i]);
			return dMin;
		}
		break;
	}
	return 0;
}
double Vec::Min(int& iPos) const
{
	int i;
	iPos = 0;
	switch (Type())
	{
	case MAT_Tbyte:
		{
			BYTE dMin;
			dMin = data.ptr[0];
			for (i=1;i < Length(); i++)
			{
				if (dMin > data.ptr[i])
				{
					dMin = data.ptr[i];
					iPos=i;
				}
			}
			return dMin;
		}
		break;
	case MAT_Tshort:
		{
			short dMin;
			dMin = data.s[0];
			for (i=1;i < Length(); i++)
			{
				if (dMin > data.s[i])
				{
					dMin = data.s[i];
					iPos=i;
				}
			}
			return dMin;
		}
		break;
	case MAT_Tint:
		{
			int dMin;
			dMin = data.i[0];
			for (i=1;i < Length(); i++)
			{
				if (dMin > data.i[i])
				{
					dMin = data.i[i];
					iPos=i;
				}
			}
			return dMin;
		}
		break;
	case MAT_Tfloat:
		{
			float dMin;
			dMin = data.fl[0];
			for (i=1;i < Length(); i++)
			{
				if (dMin > data.fl[i])
				{
					dMin = data.fl[i];
					iPos=i;
				}
			}
			return dMin;
		}
		break;
	case MAT_Tdouble:
		{
			double dMin;
			dMin = data.db[0];
			for (i=1;i < Length(); i++)
			{
				if (dMin > data.db[i])
				{
					dMin = data.db[i];
					iPos=i;
				}
			}
			return dMin;
		}
		break;
	}
	return 0;
}
double Vec::Skewness() const
{
	int i;
	double dSkewness = 0, dMean = Mean(), dTmp;
	switch (Type())
	{
	case MAT_Tbyte:
		for (i=0;i < Length(); i++)
		{
			dTmp = (double)data.ptr[i] - dMean;
			dSkewness += dTmp*dTmp*dTmp;
		}
		break;
	case MAT_Tshort:
		for (i=0;i < Length(); i++)
		{
			dTmp = (double)data.s[i] - dMean;
			dSkewness += dTmp*dTmp*dTmp;
		}
		break;
	case MAT_Tint:
		for (i=0;i < Length(); i++)
		{
			dTmp = (double)data.i[i] - dMean;
			dSkewness += dTmp*dTmp*dTmp;
		}
		break;
	case MAT_Tfloat:
		for (i=0;i < Length(); i++)
		{
			dTmp = (double)data.fl[i] - dMean;
			dSkewness += dTmp*dTmp*dTmp;
		}
		break;
	case MAT_Tdouble:
		for (i=0;i < Length(); i++)
		{
			dTmp = (double)data.db[i] - dMean;
			dSkewness += dTmp*dTmp*dTmp;
		}
		break;
	}
	double dStd = Std();
	return dSkewness/(Length()*dStd*dStd*dStd);
}
double Vec::Std() const
{
	return sqrt(Var());
}
double Vec::Sum() const
{
	int i;
	double dSum=0;
	switch (Type())
	{
	case MAT_Tbyte:
		for (i=0;i < Length(); i++)
			dSum+=(double)data.ptr[i];
		break;
	case MAT_Tshort:
		for (i=0;i < Length(); i++)
			dSum+=(double)data.s[i];
		break;
	case MAT_Tint:
		for (i=0;i < Length(); i++)
			dSum+=(double)data.i[i];
		break;
	case MAT_Tfloat:
		for (i=0;i < Length(); i++)
			dSum+=(double)data.fl[i];
		break;
	case MAT_Tdouble:
		for (i=0;i < Length(); i++)
			dSum+=(double)data.db[i];
		break;
	}
	return dSum;
}
double Vec::Var() const
{
	int i;
	double dVar = 0, dMean = Mean();
	switch (Type())
	{
	case MAT_Tbyte:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.ptr[i]-dMean);
		break;
	case MAT_Tshort:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.s[i]-dMean);
		break;
	case MAT_Tint:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.i[i]-dMean);
		break;
	case MAT_Tfloat:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.fl[i]-dMean);
		break;
	case MAT_Tdouble:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.db[i]-dMean);
		break;
	}
	return dVar;
}
double Vec::Var( double *pMean ) const
{
	int i;
	double dVar = 0, dMean = Mean();
	switch (Type())
	{
	case MAT_Tbyte:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.ptr[i]-dMean);
		break;
	case MAT_Tshort:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.s[i]-dMean);
		break;
	case MAT_Tint:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.i[i]-dMean);
		break;
	case MAT_Tfloat:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.fl[i]-dMean);
		break;
	case MAT_Tdouble:
		for (i=0;i < Length(); i++)
			dVar+=SQR((double)data.db[i]-dMean);
		break;
	}
	if (pMean)
		*pMean = dMean;
	return dVar;
}
void Vec::Rand()
{
	int i;
    int len = Length();
    switch (Type())
	{
	case MAT_Tfloat:
		for(i=0;i<len;i++)
			data.fl[i] = (float)(rand()/(double)RAND_MAX);
		break;
	case MAT_Tdouble:
		for(i=0;i<len;i++)
			data.db[i] = (double)(rand()/(double)RAND_MAX);
		break;
	}
}
/**
	@memo     Uniformly distributed integer random numbers.
	@doc      Inserts uniformly distributed integer random numbers in
			the range [st;end].
	@return   Nothing.
*/
void Vec::Rand( const int st, const int end )
{
	int i;
    int len = Length();
    double width = end-st;
    switch (Type())
	{
	case MAT_Tbyte:
		for(i=0;i<len;i++)
			data.ptr[i] = (BYTE)(int)(.5 + width*(rand()/(double)RAND_MAX) + st);
		break;
	case MAT_Tshort:
		for(i=0;i<len;i++)
			data.s[i] = (short)(int)(.5 + width*(rand()/(double)RAND_MAX) + st);
		break;
	case MAT_Tint:
		for(i=0;i<len;i++)
			data.i[i] = (int)(int)(.5 + width*(rand()/(double)RAND_MAX) + st);
		break;
	case MAT_Tfloat:
		for(i=0;i<len;i++)
			data.fl[i] = (float)(int)(.5 + width*(rand()/(double)RAND_MAX) + st);
		break;
	case MAT_Tdouble:
		for(i=0;i<len;i++)
			data.db[i] = (double)(int)(.5 + width*(rand()/(double)RAND_MAX) + st);
		break;
	}
}
void Vec::Ceil()
{
	int j;
	switch(Type())
	{
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)ceil(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)ceil(data.db[j]);
		break;
	}
}
void Vec::Floor()
{
	int j;
	switch(Type())
	{
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)floor(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)floor(data.db[j]);
		break;
	}
}
void Vec::Round()
{
	int j;
	switch(Type())
	{
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)CVUtil::Round(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)CVUtil::Round(data.db[j]);
		break;
	}
}
/**
	@memo     Generates a vector of linearly equally spaced points 
				between x1 and x2 (inclusive).
	@doc      Generates a vector of linearly equally spaced points 
				between x1 and x2 (inclusive).  
	@param    x1  Starting point.
	@param    x2  Ending point.
	@param    n   Number of points.
	@return   Nothing.
*/
void Vec::Linspace( const double x1, const double x2, const int n )
{
    assert( n>0 );
    assert( x1<x2 );
	Resize(n);
    double mul = (x2-x1)/(n-1.);
	int i;
	switch (Type())
	{
	case MAT_Tbyte:
		for(i=0;i<n;i++)
			data.ptr[i] = (BYTE)(x1 + i*mul);
		break;
	case MAT_Tshort:
		for(i=0;i<n;i++)
			data.s[i] = (short)(x1 + i*mul);
		break;
	case MAT_Tint:
		for(i=0;i<n;i++)
			data.i[i] = (int)(x1 + i*mul);
		break;
	case MAT_Tfloat:
		for(i=0;i<n;i++)
			data.fl[i] = (float)(x1 + i*mul);
		break;
	case MAT_Tdouble:
		for(i=0;i<n;i++)
			data.db[i] = (double)(x1 + i*mul);
		break;
	}
}

#define I_DEF_cmp(flavor, SrcType)  \
static int __cmp_asc_##flavor( const void *arg1, const void *arg2 ) {   \
	\
    SrcType v1 = *(SrcType*)arg1;\
    SrcType v2 = *(SrcType*)arg2;\
	\
    if (v1<v2)\
        return -1;\
    if (v1>v2)\
        return  1;\
	\
    return 0;\
}\
static int __cmp_des_##flavor( const void *arg1, const void *arg2 ) {   \
	\
    SrcType v1 = *(SrcType*)arg1;\
    SrcType v2 = *(SrcType*)arg2;\
	\
    if (v1>v2)\
        return -1;\
    if (v1<v2)\
        return  1;\
	\
    return 0;\
}

I_DEF_cmp(BYTE, BYTE)
I_DEF_cmp(short, short)
I_DEF_cmp(int, int)
I_DEF_cmp(float, float)
I_DEF_cmp(double, double)
/**
	@memo     Calculates the median.
	@doc      Calculates the median.
	@see      Trim
	@return   The median.
*/
void Vec::Sort( bool ascending /*= true */)
{
	switch (Type())
	{
	case MAT_Tbyte:
		qsort(  data.ptr, (size_t)this->Length(), sizeof( BYTE ), 
			ascending ? __cmp_asc_BYTE : __cmp_des_BYTE   );
		break;
	case MAT_Tshort:
		qsort(  data.ptr, (size_t)this->Length(), sizeof( short ), 
			ascending ? __cmp_asc_short: __cmp_des_short );
		break;
	case MAT_Tint:
		qsort(  data.ptr, (size_t)this->Length(), sizeof( int ), 
			ascending ? __cmp_asc_int: __cmp_des_int );
		break;
	case MAT_Tfloat:
		qsort(  data.ptr, (size_t)this->Length(), sizeof( float ), 
			ascending ? __cmp_asc_float: __cmp_des_float);
		break;
	case MAT_Tdouble:
		qsort(  data.ptr, (size_t)this->Length(), sizeof( double ), 
			ascending ? __cmp_asc_double: __cmp_des_double);
		break;
	}
}
/**
	@memo     Shuffles (randomizes) the vector.
	@doc      Shuffles (randomizes) the vector.
	@see      Rand 
	@return   Nothing.
*/
void Vec::Shuffle()
{
    int n = Length();
	
    int shuffle_amount = 2;
	int s, i;
	switch(Type())
	{
	case MAT_Tbyte:
		for(s=0;s<shuffle_amount;s++)
		{
			for(i=0;i<n;i++)
			{
				int index = (int)(.5+(n-1)*rand()/(double)RAND_MAX); 
				ELEM_SWAP_BYTE (data.ptr[i], data.ptr[index]);
			}
		}
		break;
	case MAT_Tshort:
		for(s=0;s<shuffle_amount;s++)
		{
			for(i=0;i<n;i++)
			{
				int index = (int)(.5+(n-1)*rand()/(double)RAND_MAX); 
				ELEM_SWAP_short(data.s[i], data.s[index]);
			}
		}
		break;
	case MAT_Tint:
		for(s=0;s<shuffle_amount;s++)
		{
			for(i=0;i<n;i++)
			{
				int index = (int)(.5+(n-1)*rand()/(double)RAND_MAX); 
				ELEM_SWAP_int (data.i[i], data.i[index]);
			}
		}
		break;
	case MAT_Tfloat:
		for(s=0;s<shuffle_amount;s++)
		{
			for(i=0;i<n;i++)
			{
				int index = (int)(.5+(n-1)*rand()/(double)RAND_MAX); 
				ELEM_SWAP_float(data.fl[i], data.fl[index]);
			}
		}
		break;
	case MAT_Tdouble:
		for(s=0;s<shuffle_amount;s++)
		{
			for(i=0;i<n;i++)
			{
				int index = (int)(.5+(n-1)*rand()/(double)RAND_MAX); 
				ELEM_SWAP_double(data.db[i], data.db[index]);
			}
		}
		break;
	}
}
SString Vec::ToString(const bool fNewline /*= true*/) const
{
	SString strOut = "";
	
	SString strTmp;
	int i;
	switch (Type())
	{
	case MAT_Tbyte:
		for(i=0;i<Length();i++)
		{
			strTmp.Format("%d ",data.ptr[i]);
			strOut += strTmp;
		}
		break;
	case MAT_Tshort:
		for(i=0;i<Length();i++)
		{
			strTmp.Format("%d ",data.s[i]);
			strOut += strTmp;
		}
		break;
	case MAT_Tint:
		for(i=0;i<Length();i++)
		{
			strTmp.Format("%d ",data.i[i]);
			strOut += strTmp;
		}
		break;
	case MAT_Tfloat:
		for(i=0;i<Length();i++)
		{
			strTmp.Format("%8.3f ",data.fl[i]);
			strOut += strTmp;
		}
		break;
	case MAT_Tdouble:
		for(i=0;i<Length();i++)
		{
			strTmp.Format("%8.3f ",data.db[i]);
			strOut += strTmp;
		}
		break;
	}
	
	if (fNewline)
		strOut += "\r\n";	
	
    return strOut;
}
void Vec::ElementMultiply(const Vec& vector)
{
	int i;
	assert(Type()==vector.Type() && Length() == vector.Length());
	switch(Type())
	{
	case MAT_Tint:
		for (i=0;i < Length(); i++)
			data.i[i] *= vector.data.i[i];
		break;
	case MAT_Tfloat:
		for (i=0;i < Length(); i++)
			data.fl[i] *= vector.data.fl[i];
		break;
	case MAT_Tdouble:
		for (i=0;i < Length(); i++)
			data.db[i] *= vector.data.db[i];
		break;
	}
}
void Vec::ElementDivide(const Vec& vector)
{
	int i;
	assert(Type()==vector.Type() && Length() == vector.Length());
	switch(Type())
	{
	case MAT_Tfloat:
		for (i=0;i < Length(); i++)
			data.fl[i] /= vector.data.fl[i];
		break;
	case MAT_Tdouble:
		for (i=0;i < Length(); i++)
			data.db[i] /= vector.data.db[i];
		break;
	}
}
/**
	@memo     Clamps the vector to [min,max].
	@doc      Clamps the vector to [min,max]. 
	@param    min     Minumim value.
	@param    max     Maximum value.
	@return   Nothing.
*/
void Vec::Clamp( const double min, const double max )
{
	int len = this->Length();

	int i;
	switch(Type())
	{
	case MAT_Tbyte:
		for(i=0;i<len;i++)
		{
			BYTE *val = data.ptr+i;
			*val = *val<(BYTE)min ? (BYTE)min : (*val>(BYTE)max ? (BYTE)max : *val);
		}
		break;
	case MAT_Tshort:
		for(i=0;i<len;i++)
		{
			short *val = data.s+i;
			*val = *val<(short)min ? (short)min : (*val>(short)max ? (short)max : *val);
		}
		break;
	case MAT_Tint:
		for(i=0;i<len;i++)
		{
			int *val = data.i+i;
			*val = *val<(int)min ? (int)min : (*val>(int)max ? (int)max : *val);
		}
		break;
	case MAT_Tfloat:
		for(i=0;i<len;i++)
		{
			float *val = data.fl+i;
			*val = *val<(float)min ? (float)min : (*val>(float)max ? (float)max : *val);
		}
		break;
	case MAT_Tdouble:
		for(i=0;i<len;i++)
		{
			double*val = data.db+i;
			*val = *val<(double)min ? (double)min : (*val>(double)max ? (double)max : *val);
		}
		break;
	}
}

void Vec::Pow(double dP)
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Length();j++)
			data.ptr[j] = (BYTE)pow(data.ptr[j], dP);
		break;
	case MAT_Tshort:
		for(j=0;j<Length();j++)
			data.s[j] = (short)pow(data.s[j], dP);
		break;
	case MAT_Tint:
		for(j=0;j<Length();j++)
			data.i[j] = (int)pow(data.i[j], dP);
		break;
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)powf(data.fl[j], (float)dP);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)pow(data.db[j], dP);
		break;
	}
}
void Vec::Sqrt()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Length();j++)
			data.ptr[j] = (BYTE)sqrtf(data.ptr[j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Length();j++)
			data.s[j] = (short)sqrtf(data.s[j]);
		break;
	case MAT_Tint:
		for(j=0;j<Length();j++)
			data.i[j] = (int)sqrtf((float)data.i[j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)sqrtf(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)sqrt(data.db[j]);
		break;
	}
}
void Vec::Sqr()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Length();j++)
			data.ptr[j] = (BYTE)SQR(data.ptr[j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Length();j++)
			data.s[j] = (short)SQR(data.s[j]);
		break;
	case MAT_Tint:
		for(j=0;j<Length();j++)
			data.i[j] = (int)SQR(data.i[j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)SQR(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)SQR(data.db[j]);
		break;
	}
}
void Vec::Log()
{
	int j;
	switch(Type())
	{
	case MAT_Tbyte:
		for(j=0;j<Length();j++)
			data.ptr[j] = (BYTE)logf(data.ptr[j]);
		break;
	case MAT_Tshort:
		for(j=0;j<Length();j++)
			data.s[j] = (short)logf(data.s[j]);
		break;
	case MAT_Tint:
		for(j=0;j<Length();j++)
			data.i[j] = (int)logf((float)data.i[j]);
		break;
	case MAT_Tfloat:
		for(j=0;j<Length();j++)
			data.fl[j] = (float)logf(data.fl[j]);
		break;
	case MAT_Tdouble:
		for(j=0;j<Length();j++)
			data.db[j] = (double)log(data.db[j]);
		break;
	}
}
void Vec::Reverse()
{
	int i;
	int len = Length();
	switch(Type())
	{
	case MAT_Tbyte:
		for (i=0;i<len/2;i++)
			ELEM_SWAP_BYTE(data.ptr[i], data.ptr[len-i-1])
		break;
	case MAT_Tshort:
		for (i=0;i<len/2;i++)
			ELEM_SWAP_short(data.s[i], data.s[len-i-1])
		break;
	case MAT_Tint:
		for (i=0;i<len/2;i++)
			ELEM_SWAP_int(data.i[i], data.i[len-i-1])
		break;
	case MAT_Tfloat:
		for (i=0;i<len/2;i++)
			ELEM_SWAP_float(data.fl[i], data.fl[len-i-1])
		break;
	case MAT_Tdouble:
		for (i=0;i<len/2;i++)
			ELEM_SWAP_double(data.db[i], data.db[len-i-1])
		break;
	}
}
Vec Vec::Range( const int st, const int end) const
{
    assert( st<end );
    assert( end<Length() );
	
    int len=end-st+1;
    Vec range( len, this->Type() );
	memcpy(range.data.ptr, this->data.ptr+Step()*st, Step()*len);
    return range;
}
void Vec::ToMatrix( const int nRows, const int nCols, Mat &m, bool rowWise /*=true*/ )
{
    assert( nRows>0 && nCols>0 );
    assert( nRows*nCols==this->Length() );
	
    int i=0;
    m.Create( nRows, nCols, Type() );
	switch (Type())
	{
	case MAT_Tbyte:
		if (rowWise)
		{
			for(int r=0;r<nRows;r++)
			{
				for(int c=0;c<nCols;c++)
					m.data.ptr[r][c] = data.ptr[i++];
			}
		} 
		else 
		{
			for(int c=0;c<nCols;c++) 
			{
				for(int r=0;r<nRows;r++) 
					m.data.ptr[r][c] = data.ptr[i++];
			}
		}
		break;
	case MAT_Tshort:
		if (rowWise)
		{
			for(int r=0;r<nRows;r++)
			{
				for(int c=0;c<nCols;c++)
					m.data.s[r][c] = data.s[i++];
			}
		} 
		else 
		{
			for(int c=0;c<nCols;c++) 
			{
				for(int r=0;r<nRows;r++) 
					m.data.s[r][c] = data.s[i++];
			}
		}
		break;
	case MAT_Tint:
		if (rowWise)
		{
			for(int r=0;r<nRows;r++)
			{
				for(int c=0;c<nCols;c++)
					m.data.i[r][c] = data.i[i++];
			}
		} 
		else 
		{
			for(int c=0;c<nCols;c++) 
			{
				for(int r=0;r<nRows;r++) 
					m.data.i[r][c] = data.i[i++];
			}
		}
		break;
	case MAT_Tfloat:
		if (rowWise)
		{
			for(int r=0;r<nRows;r++)
			{
				for(int c=0;c<nCols;c++)
					m.data.fl[r][c] = data.fl[i++];
			}
		} 
		else 
		{
			for(int c=0;c<nCols;c++) 
			{
				for(int r=0;r<nRows;r++) 
					m.data.fl[r][c] = data.fl[i++];
			}
		}
		break;
	case MAT_Tdouble:
		if (rowWise)
		{
			for(int r=0;r<nRows;r++)
			{
				for(int c=0;c<nCols;c++)
					m.data.db[r][c] = data.db[i++];
			}
		} 
		else 
		{
			for(int c=0;c<nCols;c++) 
			{
				for(int r=0;r<nRows;r++) 
					m.data.db[r][c] = data.db[i++];
			}
		}
		break;
	}
}

void Vec::ToMatlab(const SString& sFilename,const SString& sName,const SString& sComment,bool fAppend) const
{
	assert(Length() > 0);
	
	Mat mOut(data.ptr, Length(),1, Type());
	
	memcpy(mOut.data.ptr[0], data.ptr, Step()*Length());
	mOut.ToMatlab(sFilename,sName,sComment,fAppend);
}

void Vec::FromMatlab(const SString& sFilename,const SString& sName)
{
	Mat mOut;
	mOut.FromMatlab(sFilename,sName);
	
	Resize(mOut.Rows());
	memcpy(data.ptr, mOut.data.ptr[0], mOut.Step()*Length());
}

void Vec::ToC (const SString& sFilename, const SString& sVarName, const int nElement, bool fAppend) const
{
	if (Length() == 0)
		return;
	FILE* pFile = NULL;
	int* pan = new int[Length()];
	if(fAppend)
		pFile = fopen(sFilename, "a++");
	else 
		pFile = fopen(sFilename, "w");
	
	fprintf(pFile, "int %s[%d]={", (const char*)sVarName, Length());

	int i;
	for(i=0;i<Length();i++)
	{
		switch(Type())
		{
		case MAT_Tbyte:
			pan[i] = data.ptr[i];
			break;
		case MAT_Tshort:
			pan[i] = data.s[i];
			break;
		case MAT_Tint:
			pan[i] = data.i[i];
			break;
		case MAT_Tfloat:
			pan[i] = CVUtil::Round(data.fl[i] * 1048576);
			break;
		case MAT_Tdouble:
			pan[i] = CVUtil::Round(data.db[i] * 1048576);
			break;
		}
	}
	for(i=0;i<Length()-1;i++)
	{
		if (i % nElement == 0)
			fprintf(pFile, "\n\t");
		fprintf (pFile, "%d,", pan[i]);
	}
	fprintf (pFile, "%d};\n", pan[Length()-1]);
	fclose (pFile);
	delete []pan;
}

void Vec::Swap(Vec& other)
{
	int ntemp;
	BYTE* pbtemp;
	TYPE nType;
	SWAP(step, other.step, ntemp);
	SWAP(depend, other.depend, ntemp);
	SWAP(len, other.len, ntemp);
	SWAP(m_nMaxLen, other.m_nMaxLen, ntemp);
	SWAP(type, other.type, nType);
	SWAP(data.ptr, other.data.ptr, pbtemp);
}

}
