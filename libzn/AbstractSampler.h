#pragma once
#include "Sampler.h"

class AbstractSampler : public Sampler
{
public:
	explicit AbstractSampler(SIZE_T channelCount);
	~AbstractSampler();
	virtual bool begin();
	virtual void pause();
	virtual void resume();
	virtual void quit();
	virtual unsigned long join(int timeout);
	virtual SamplerState getState();
	virtual void resetAll();
	virtual void attach(int channel, SignalBuffer<float>* pBuffer);
	virtual SignalBuffer<float>* get(int channel) ;
	virtual SIZE_T getCount() const;
    virtual void setMode(int mode);
protected:
	virtual bool onBeforeStart(){ return true; }
	virtual void onBeforeLoop(){}
	virtual void onResume(){}
	virtual void doSampler()=0;
	/**采样线程
	 *
	 *@param lpParam 传入参数为this指针
	 *@return 0 - 正常停止 >1000 错误
	 */
	static DWORD __stdcall samplerFunc(LPVOID lpParam);
private:
	HANDLE _hThread;
	HANDLE _hResume;
	DWORD _dwThreadId;
	SignalBuffer<float> **_pBuffers;
	SIZE_T _channelCount;
	bool _quit;
	bool _paused;
};

