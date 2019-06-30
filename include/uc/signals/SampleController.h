#pragma once

class SampleController
{
public:
	virtual ~SampleController(){}
	virtual void start()=0;
	virtual void stop()=0;
};