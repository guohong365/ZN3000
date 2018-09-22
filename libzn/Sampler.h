#pragma once

class Sampler
{
public:
	enum SamplerState
	{
		STOPPED,
		RUNNING,
		PAUSED
	};
	virtual bool begin()=0;
	virtual void pause()=0;
	virtual void resume()=0;
	virtual void quit()=0;
	virtual SamplerState getState()=0;
	virtual ~Sampler(){}
};


