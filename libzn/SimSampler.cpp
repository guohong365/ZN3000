#include "stdafx.h"
#include "SimSampler.h"
#include "ZnRecord.h"
#include <cmath>

SimSampler::SimSampler( SimSampleGenerator * pCallBack, int step, int sleep)
	:AbstractSampler(4)
	, _pCallBack(pCallBack)
    , _step(step)
    , _sleep(sleep)
{
}

SimSampler::~SimSampler()
{
    delete _pCallBack;
}

void SimSampler::doSampler()
{
    SIZE_T current=0;
    for(SIZE_T i=0; i< getCount(); i++)
    {
        SignalBuffer<double>* pBuffer=get(i);
        if(pBuffer)
        {
            current=pBuffer->getLength() + _step;
            pBuffer->setLength(current);
        }
    }
    _tprintf(_T("do sampler[%u]\n"), current);
    Sleep(_sleep);
}

void SimSampler::onBeforeLoop()
{
    for(SIZE_T i=0; i< getCount(); i++)
    {
        SignalBuffer<double>* pBuffer=get(i);
        if(pBuffer)
        {
            _pCallBack->generator(pBuffer->getBuffer(), pBuffer->getSize());
        }
    }
}

