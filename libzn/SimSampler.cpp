#include "stdafx.h"
#include "SimSampler.h"
#include "ZnRecord.h"
#include <cmath>

SimSampler::SimSampler( SimSampleGenerator * pCallBack)
	:AbstractSampler(4)
	, _pCallBack(pCallBack)
{
}

SimSampler::~SimSampler()
{
}

void SimSampler::doSampler()
{

}

void SineSampleGenerator::generator( void* pBuffer, size_t count, size_t sample_size, int N )
{
	BYTE bVal;
	unsigned short sVal;
	unsigned long lVal;
	const double step=N * 2 * 3.14159265 / count; 
	for(size_t i=0; i< count; i++)
	{
		switch(sample_size)
		{
		case 1:
			bVal = static_cast<BYTE>(0x7F * (sin(step * i) + 1));
			*(static_cast<BYTE *>(pBuffer) + i)=bVal;
			break;
		case 2:
			sVal = static_cast<unsigned short>(0x7FFF * (sin(step * i) + 1));
			*(static_cast<unsigned short*>(pBuffer) + i)= sVal;
			break;
		case 4:
			lVal = static_cast<unsigned long>(0x7FFFFFFF * (sin(step * i) + 1));
			*(static_cast<unsigned long*>(pBuffer) + i)= lVal;
			break;
		default: 
			break;
		}
	}
}
