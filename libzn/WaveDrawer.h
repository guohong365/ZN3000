#pragma once

#include "../libDrawObject/DrawObject.h"
#include "SignalBuffer.h"

class WaveDrawer:public CDrawObject
{
	//basic properties
	float _velocity; // 0.1mm
	float _mmPerSecond;
	//draw componets
	int  _baseline;
	int _waveHeight;

	//data
	SignalBuffer<long> * _pWaveBuffer;
	int _totalSampleCount;
	float _sampleDotSpacing;
	void initialize();
public:
	WaveDrawer();

	WaveDrawer(SignalBuffer<long> * pWaveBuffer, const Gdiplus::Point &pt, const Gdiplus::Size & size )
	:CDrawObject(pWaveBuffer->getName(), pt, size)
		,_pWaveBuffer(pWaveBuffer)
	{
		initialize();
	}
	
	virtual ~WaveDrawer(void);
	
	SignalBuffer<long> * GetWaveBuffer() const
	{
		return _pWaveBuffer;
	}
	void SetWaveBuffer(SignalBuffer<long> * pBuffer)
	{
		_pWaveBuffer=pBuffer;
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
