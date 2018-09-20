#pragma once

#include "Sampler.h"
#include "SignalBuffer.h"
#include "SignalChannelImpl.h"


class SimSampleGenerator
{
public:
	virtual ~SimSampleGenerator(){};
	virtual void generator(void * pBuffer, size_t count, size_t sample_size, int N)=0;
};

class SineSampleGenerator:public SimSampleGenerator
{
public:
	virtual void generator(void *pBuffer, size_t count, size_t sample_size, int N = 1);
};

class SimSampler:public Sampler
{
public:
	SimSampler(SimSampleGenerator * pCallBack, size_t count);
	virtual ~SimSampler();
	virtual bool begin(){ return true;};
	virtual void stop(){};
private:
	SimSampleGenerator * _pCallBack;
	SignalChannel * _pHeartBuffer;
	SignalChannel * _pAdmittanceBuffer;
};
