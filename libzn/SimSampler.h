#pragma once

#include "Sampler.h"
#include "SignalBuffer.h"


class ISimSampleGenerator
{
public:
	virtual ~ISimSampleGenerator(){};

	virtual void generator(void * pBuffer, size_t count, size_t sample_size, int N=1)=0;
};

class SineSampleGenerator:public ISimSampleGenerator
{
public:
	virtual void generator(void *pBuffer, size_t count, size_t sample_size, int N = 1);
};

class SimSampler:public ISampler
{
public:
	SimSampler(ISimSampleGenerator * pCallBack, size_t count);
	virtual ~SimSampler();
	virtual bool begin(){ return true;};
	virtual void stop(){};
private:
	ISimSampleGenerator * _pCallBack;
	SignalBuffer<unsigned short> * _pHeartBuffer;
	SignalBuffer<unsigned short> * _pAdmittanceBuffer;
};
