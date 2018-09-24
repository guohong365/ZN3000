#pragma once

class SamplerStateChangedCallback;
class SamplerProgressCallback;

class Sampler
{
public:
	enum SamplerState
	{
		STOPPED,
		RUNNING,
		PAUSED
	};
	virtual bool begin(SamplerProgressCallback *progressCallback, SamplerStateChangedCallback *stateChangedCallback)=0;
	virtual void pause()=0;
	virtual void resume()=0;
	virtual void quit()=0;
	virtual unsigned long join(int timeout)=0;
	virtual SamplerState getState()=0;
	virtual ~Sampler(){}
};

class SamplerProgressCallback
{
public:
	virtual ~SamplerProgressCallback(){}
	virtual void onProgress(Sampler* sender, unsigned long current, void *pPacketBuffer, size_t size)=0;
};
class SamplerStateChangedCallback
{
public:
	virtual ~SamplerStateChangedCallback(){}
	virtual void onStateChanged(Sampler*sender, Sampler::SamplerState state)=0;
};



