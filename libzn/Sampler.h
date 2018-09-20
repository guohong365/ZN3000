#pragma once

#include "ZNDData.h"
#include "SignalBuffer.h"

class ISampler
{
public:
	virtual bool begin()=0;
	virtual void stop()=0;
	virtual ~ISampler(){}
};


