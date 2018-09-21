#pragma once

#include "../libDrawObject/DrawObject.h"
#include "SignalBuffer.h"
#include "SignalChannelImpl.h"

class WaveDrawer:public DrawObject
{
	//basic properties
	float _velocity; // 0.1mm
	float _mmPerSecond;
	//draw componets
	int  _baseline;
	int _waveHeight;

	//data
	SignalChannel * _pSignalChannel;
	int _totalSampleCount;
	float _sampleDotSpacing;
	void _initialize();
public:
	WaveDrawer();

	WaveDrawer(SignalChannel* pChannel, const Gdiplus::Point& pt, const Gdiplus::Size& size);

	virtual ~WaveDrawer();

	SignalChannel* getChannelBuffer() const;

	void setChannelBuffer(SignalChannel* pBuffer);

	virtual void SetVelocity(float velocity);

	virtual float GetVelocity();

	virtual void SetBaseline(int pos);

	virtual int GetBaseline();

	virtual void SetWaveHeight(int height);

	virtual int GetWaveHeight();

	virtual int GetTotalSampleCount();

	virtual float GetSampleDotSpacing();
	virtual void OnSizeChanged();
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual DrawObject* CreateInstance();
};
