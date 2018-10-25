#pragma once
#include "Sampler.h"

class AbstractSampler : public Sampler
{
public:
	explicit AbstractSampler(size_t channelCount);
	~AbstractSampler();
	virtual bool begin();
	virtual void pause();
	virtual void resume();
	virtual void quit();
	virtual unsigned long join(int timeout);
	virtual SamplerState getState();
	virtual void resetAll();
	virtual void attach(int channel, SignalBuffer<double>* pBuffer);
	virtual SignalBuffer<double>* get(int channel) ;
	virtual size_t getCount() const;
    virtual void setMode(int mode);
protected:
	virtual bool onBeforeStart(){ return true; }
	virtual void onBeforeLoop(){}
	virtual void onResume(){}
	virtual void doSampler()=0;
	/**�����߳�
	 *
	 *@param lpParam �������Ϊthisָ��
	 *@return 0 - ����ֹͣ >1000 ����
	 */
	static DWORD __stdcall samplerFunc(LPVOID lpParam);
private:
	HANDLE _hThread;
	HANDLE _hResume;
	DWORD _dwThreadId;
	SignalBuffer<double> **_pBuffers;
	SIZE_T _channelCount;
	bool _quit;
	bool _paused;
};

