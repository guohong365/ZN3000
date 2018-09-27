#pragma once

#include "../libDrawObject/DrawObject.h"
#include "SignalChannelImpl.h"
#include "WaveDrawerAppearance.h"

class WaveDrawer:public DrawObject
{
public:
	WaveDrawer();
	WaveDrawer(SignalChannel* pChannel, double layoutPercent);
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
	virtual DrawObject* CreateInstance();
	double GetLayoutPercent() const;
	void SetLayoutPercent(double layoutPercent);
	BaselineAlignment GetBaselineAlignment();

	void SetBaselineAlignment(BaselineAlignment alignment);

	float GetBaselineWidth();

	void SetBaselineWidth(float width);

	Gdiplus::Color GetBaselineColor();

	void SetBaselineColor(Gdiplus::Color color);

	int GetBaselineStyle();

	void SetBaselineStyle(int style);

	bool IsShowBaseline();

	void SetShowBaseline(bool isShow);

	DrawMode GetWaveDrawMode();

	void SetWaveDrawMode(DrawMode drawMode);

	int GetEraseWidth();

	void SetEraseWidth(int width);
protected:	
	virtual void OnSizeChanged();
	void _drawBaseline(Gdiplus::Graphics& graph);
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void _drawWaveByPixels(Gdiplus::Graphics& graphics,float* pBuffer, SIZE_T bufferSize,int offsetY, int startX, int endX, SIZE_T startIndex, SIZE_T endIndex);
	virtual void _drawWaveBySamples(Gdiplus::Graphics& graphics,float *pBuffer, SIZE_T bufferSize,int offsetY, int startX, int endX, SIZE_T startIndex, SIZE_T endIndex);
private:
	Gdiplus::Pen * _pBaselinePen;
	Gdiplus::Pen * _pWaveLinePen;

	//basic properties
	float _velocity; // 0.1mm
	//draw componets
	int  _baseline;
	int _waveHeight;
	float _scale;
	double _layoutPercent;
	SignalChannel * _pSignalChannel;
	int _totalSampleCount;
	float _sampleDotSpacing;
	WaveDrawerAppearance& getThisAppearance();

	void _initialize();
};
