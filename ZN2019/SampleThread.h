#pragma once
#include "../libzn/SignalBuffer.h"
#include "../libzn/Sampler.h"

class SampleThread
{
	static DWORD WINAPI SampleThreadFun(LPVOID pParam);
public:
	SampleThread(SignalBuffer<unsigned short>* pBuffers, int channelCount,  Sampler * pSampler);
	virtual void start();
	virtual void stop(DWORD timeout);
	virtual DWORD run();
	virtual bool isRunning();
protected:
	HANDLE _hThread;
	DWORD  _dwThreadId;
	bool _bQuit;
	SignalBuffer<unsigned short> * _pBuffers;
	int _channelCount;
	Sampler * _pSampler;
};
