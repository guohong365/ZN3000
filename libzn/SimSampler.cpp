#include "stdafx.h"
#include "SimSampler.h"
#include <cmath>

SimSampler::SimSampler( ISimSampleGenerator * pCallBack, size_t count)
:_pCallBack(pCallBack)
{
	ASSERT(_pCallBack);
	_pHeartBuffer=new SignalBuffer<unsigned short>(count, 0.002, _T("ÐÄµç"));
	_pCallBack->generator(_pHeartBuffer->getBuffer(), count, 2);
	_pAdmittanceBuffer=new SignalBuffer<unsigned short>(count, 0.002, _T("Î¢·Ö"));
	_pCallBack->generator(_pAdmittanceBuffer->getBuffer(), count, 2);
}



SimSampler::~SimSampler()
{
	delete _pHeartBuffer;
	delete _pAdmittanceBuffer;
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
