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
	BaselineAlignment GetBaselineAlignment()
	{
		return BaselineAlignment(getThisAppearance().BaselineAlignment);
	}
	void SetBaselineAlignment(BaselineAlignment alignment)
	{
		getThisAppearance().BaselineAlignment=alignment;
	}
	float GetBaselineWidth()
	{
		return getThisAppearance().BaselineWidth;
	}
	void SetBaselineWidth(float width)
	{
		getThisAppearance().BaselineWidth=width;
	}
	Gdiplus::Color GetBaselineColor()
	{
		return getThisAppearance().BaselineColor;
	}
	void SetBaselineColor(Gdiplus::Color color)
	{
		getThisAppearance().BaselineColor=color.GetValue();
	}
	int GetBaselineStyle()
	{
		return getThisAppearance().BaselineStyle;
	}
	void SetBaselineStyle(int style)
	{
		getThisAppearance().BaselineStyle=style;
	}
	bool IsShowBaseline()
	{
		return getThisAppearance().ShowBaseline;
	}
	void SetShowBaseline(bool isShow)
	{
		getThisAppearance().ShowBaseline=isShow;
	}
	DrawMode GetWaveDrawMode()
	{
		return DrawMode(getThisAppearance().WaveDrawMode);
	}
	void SetWaveDrawMode(DrawMode drawMode)
	{
		getThisAppearance().WaveDrawMode=drawMode;
	}
	int GetEraseWidth()
	{
		return getThisAppearance().EraseWidth;
	}
	void SetEraseWidth(int width)
	{
		getThisAppearance().EraseWidth=width;
	}
protected:	
	virtual void OnSizeChanged();
	void _drawBaseline(Gdiplus::Graphics& graph);
	virtual void OnDraw(Gdiplus::Graphics & graph);
private:
	//basic properties
	float _velocity; // 0.1mm
	float _mmPerSecond;
	//draw componets
	int  _baseline;
	int _waveHeight;
	float _scale;
	double _layoutPercent;
	//data
	SignalChannel * _pSignalChannel;
	int _totalSampleCount;
	float _sampleDotSpacing;
	WaveDrawerAppearance& getThisAppearance()
	{
		return static_cast<WaveDrawerAppearance&>(GetAppearance());
	}
	void _initialize();
};
