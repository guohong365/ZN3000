#include "stdafx.h"
#include "AbstractSampler.h"


AbstractSampler::AbstractSampler(SIZE_T channelCount)
	: _hThread(nullptr)
	, _hResume(nullptr)
	, _dwThreadId(0)
	, _channelCount(channelCount)
	, _quit(false)
	, _paused(false)
{
	_pBuffers=new SignalBuffer<float>*[channelCount];
}


AbstractSampler::~AbstractSampler()
{
	AbstractSampler::quit();
	CloseHandle(_hThread);
	CloseHandle(_hResume);
	delete []_pBuffers;

}

bool AbstractSampler::begin()
{
	if(onBeforeStart())
	{
		_hResume = CreateEvent(nullptr, TRUE, false, nullptr);
		_hThread = CreateThread(nullptr, 0, samplerFunc, this, 0, &_dwThreadId);
		return _hThread!=nullptr;
	}
	return false;
}

void AbstractSampler::pause()
{
	_paused = true;
}

void AbstractSampler::resume()
{
	if(_paused)
	{
		SetEvent(_hResume);
	}
}

void AbstractSampler::quit()
{
	_quit=true;
	WaitForSingleObject(_hThread, INFINITE);
}

unsigned long AbstractSampler::join(int timeout)
{
	if(timeout<0)
	{
		return WaitForSingleObject(_hThread, INFINITE);
	}
	return WaitForSingleObject(_hThread, timeout);
}

Sampler::SamplerState AbstractSampler::getState()
{	
	DWORD code=0;
	GetExitCodeThread(_hThread, &code);
	if(code==STILL_ACTIVE){
		if(_paused) return PAUSED;
		return RUNNING;
	}
	return STOPPED;
}

void AbstractSampler::resetAll()
{
	ASSERT(_pBuffers);
	if(getState() == RUNNING)
	{
		pause();
		Sleep(200);
	}
	for(SIZE_T i= 0; i<_channelCount; i++)
	{
		if(_pBuffers[i])
		{
			_pBuffers[i]->clear();
		}		
	}
	resume();
}

void AbstractSampler::attach(int channel, SignalBuffer<float>* pBuffer)
{
	ASSERT(_pBuffers);
	if(channel>=0 && channel< _channelCount)
	{
		_pBuffers[channel]=pBuffer;
	}
}

SignalBuffer<float>* AbstractSampler::get(int channel) 
{
	ASSERT(_pBuffers);
	if(channel< _channelCount || channel > _channelCount) return nullptr;
	return _pBuffers[channel];
}

SIZE_T AbstractSampler::getCount() const
{
	return _channelCount;
}

DWORD AbstractSampler::samplerFunc(LPVOID lpParam)
{
	AbstractSampler * pThis=static_cast<AbstractSampler*>(lpParam);
	pThis->onBeforeLoop();
	while(!pThis->_quit)
	{
		if(pThis->_paused)
		{
			if(WaitForSingleObject(pThis->_hResume, 100)!=WAIT_OBJECT_0)
			{
				continue;
			}
			ResetEvent(pThis->_hResume);
			pThis->_paused=false;
			pThis->onResume();
		}
		pThis->doSampler();		
	}
	return 0;
}
