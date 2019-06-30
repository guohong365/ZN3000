#pragma once
#include <uc/dsp/AbstractSampler.h>

class ZnRowFileSimSampler : public AbstractSampler
{
    CString _fileDir;
    SignalBuffer<float> * _pData[4];
    SIZE_T _current;
public:
    explicit ZnRowFileSimSampler(const CString& fileDir);
    virtual ~ZnRowFileSimSampler();
protected:
    virtual bool onBeforeStart();
    virtual void doSampler();
public:
    virtual void resetAll();
protected:
    static int load(FILE* fp, SignalBuffer<float>* &pBuffer);
};

