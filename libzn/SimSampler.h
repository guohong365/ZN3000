#pragma once

#include "AbstractSampler.h"


class SimSampleGenerator
{
public:
    virtual ~SimSampleGenerator(){};
    virtual void generator(void * pBuffer, size_t count)=0;
};

template <typename ValueType>
class SineSampleGenerator:public SimSampleGenerator
{
    double _cycle;
    ValueType _value;
public:
    SineSampleGenerator(double cycle)
        :_cycle(cycle)
    {

    }
    void generator(void *pBuffer, size_t count)
    {
        const double step=_cycle * 2 * 3.14159265 / count;
        for(size_t i=0; i< count; i++)
        {
            if(typeid(_value) == typeid(char))
            {
                const char bVal = static_cast<char>(0x7F * (sin(step * i) + 1));
                *(static_cast<char *>(pBuffer) + i)=bVal;
            }
            else if(typeid(_value)==typeid(short) || typeid(_value)==typeid(float))
            {
                const ValueType sVal = static_cast<ValueType>(32767 * sin(step * i));
                *(static_cast<ValueType*>(pBuffer) + i)= sVal;
            }
            else if(typeid(_value)==typeid(long))
            {
                const long lVal = static_cast<long>(0x7FFFFFFF * (sin(step * i) + 1));
                *(static_cast<long*>(pBuffer) + i)= lVal;
            }
        }
    }
};

class SimSampler:public AbstractSampler
{
public:
    SimSampler(SimSampleGenerator * pCallBack, int step=100, int sleep=100);
    virtual ~SimSampler();
protected:
    virtual void doSampler();
    virtual void onBeforeLoop();
private:
    SimSampleGenerator * _pCallBack;
    int _step;
    int _sleep;
};
