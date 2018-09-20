#pragma once

#include "../libDrawObject/DrawObject.h"
#include "SignalBuffer.h"
#include "SignalChannelImpl.h"

class WaveDrawer:public CDrawObject
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
	
	SignalChannel * getChannelBuffer() const
	{
		return _pSignalChannel;
	}
	void setChannelBuffer(SignalChannel * pBuffer)
	{
		_pSignalChannel=pBuffer;
	}

	virtual void SetVelocity(float velocity)
	{
		_velocity=velocity;
	}
	virtual float GetVelocity()
	{
		return _velocity;
	}
	virtual void SetBaseline(int pos)
	{
		_baseline=pos;
	}
	virtual int GetBaseline()
	{
		return _baseline;
	}
	virtual void SetWaveHeight(int height)
	{
		_waveHeight=height;
	}
	virtual int GetWaveHeight()
	{
		return _waveHeight;
	}
	virtual int GetTotalSampleCount()
	{
		return _totalSampleCount;
	}
	virtual float GetSampleDotSpacing()
	{
		return _sampleDotSpacing;
	}
	virtual void OnSizeChanged();
	virtual void OnDraw(Gdiplus::Graphics & graph);
};
