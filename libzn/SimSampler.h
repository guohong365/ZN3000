#pragma once

#include "Sampler.h"
#include "SignalBuffer.h"
#include "SignalChannelImpl.h"
#include "AbstractSampler.h"


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

class SimSampler:public AbstractSampler
{
public:
	SimSampler(SimSampleGenerator * pCallBack);
	virtual ~SimSampler();
protected:
	virtual void doSampler();
private:
	SimSampleGenerator * _pCallBack;
};
