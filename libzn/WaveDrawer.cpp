#include "stdafx.h"
#include "WaveDrawer.h"
#include "WaveDrawerAppearance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DEFAULT_WAVE_SCALE 0.6f

static WaveDrawerAppearance DefaultDrawerAppearance;
void WaveDrawer::_initialize()
{	
	SetAppearance(DefaultDrawerAppearance);
	if(GetBaselineAlignment()==MIDDLE_VALUE)
	{
		_baseline=GetSize().Height/2;
	}
	else 
	{ 
		_baseline = GetSize().Height*9/10;   //波形高度占80%，上下各留10%
	}
	_velocity = 250;
	_scale=DEFAULT_WAVE_SCALE;
	_waveHeight=GetSize().Height*_scale;
}

WaveDrawer::WaveDrawer()
	:DrawObject()
	,_layoutPercent(1)
	,_pSignalChannel(nullptr)
{
	_initialize();
}

WaveDrawer::WaveDrawer(SignalChannel* pChannel, const double layoutPercent)
	:DrawObject(pChannel->getLabel(), Gdiplus::Point(0,0), Gdiplus::Size(0,0))
	, _layoutPercent(layoutPercent)
	, _pSignalChannel(pChannel)
{
	_initialize();
}

WaveDrawer::~WaveDrawer()
{	
}

SignalChannel* WaveDrawer::getChannelBuffer() const
{
	return _pSignalChannel;
}

void WaveDrawer::setChannelBuffer(SignalChannel* pBuffer)
{
	_pSignalChannel = pBuffer;
}

void WaveDrawer::SetVelocity(float velocity)
{
	_velocity = velocity;
}

float WaveDrawer::GetVelocity()
{
	return _velocity;
}

void WaveDrawer::SetBaseline(int pos)
{
	_baseline = pos;
}

int WaveDrawer::GetBaseline()
{
	return _baseline;
}

void WaveDrawer::SetWaveHeight(int height)
{
	_waveHeight = height;
}

int WaveDrawer::GetWaveHeight()
{
	return _waveHeight;
}

int WaveDrawer::GetTotalSampleCount()
{
	return _totalSampleCount;
}

float WaveDrawer::GetSampleDotSpacing()
{
	return _sampleDotSpacing;
}

DrawObject* WaveDrawer::CreateInstance()
{
	return new WaveDrawer;
}

double WaveDrawer::GetLayoutPercent() const
{
	return _layoutPercent;
}

void WaveDrawer::SetLayoutPercent(double layoutPercent)
{
	_layoutPercent = layoutPercent;
}

void WaveDrawer::OnDraw( Gdiplus::Graphics & graph )
{
	if(IsShowBaseline())
	{
		_drawBaseline(graph);
	}
	SignalBuffer<float> & buffer=_pSignalChannel->getSignalBuffer();
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const int sampleCount = min(_totalSampleCount, buffer.getSize());
	const int startSample=buffer.getLength() <= buffer.getSize() ? 0 : buffer.getLength() - buffer.getSize(); 
	const int offset= GetSize().Height * 9/10;
	float startX = GetSize().Width - sampleCount * _sampleDotSpacing;
	float startY =offset - GetSize().Height * _scale * buffer.getBuffer()[startSample%buffer.getSize()] /65535.0f;
	for(int i= 0; i< sampleCount - 2; i ++)
	{
		const float endX = startX + _sampleDotSpacing + _sampleDotSpacing + _sampleDotSpacing;
		const float endY = offset - buffer.getBuffer()[(startSample + i + 1) % buffer.getSize()] * GetSize().Height * _scale / 65535.0f;
		graph.DrawLine(&pen, int(startX), int(startY), int(endX), int(endY));
		startY = endY;
		startX = endX;
	}
}

void WaveDrawer::OnSizeChanged()
{
	__super::OnSizeChanged();
	if(GetBaselineAlignment()==MIDDLE_VALUE)
	{
		_baseline=GetSize().Height/2;
	}
	else if(GetBaselineAlignment()==ZERO_VALUE)
	{
		if(_pSignalChannel->getDigitalMinimum()>0)
		{
			_baseline=GetSize().Height* 9/10;
		}
		else
		{
			_baseline = GetSize().Height*(1 - 8*(_pSignalChannel->getDigitalMinimum()/(_pSignalChannel->getDigitalMaximum() - _pSignalChannel->getDigitalMinimum())))/10;
		}
	}
	else  //MIDDLE_VALUE
	{
		_baseline=GetSize().Height* 9/10;
	}
	_waveHeight=GetSize().Height*4/5;
	_totalSampleCount = int(GetSize().Width / ScreenInfo::GetScreenInfo().GetDpmmX() / _velocity *	_pSignalChannel->getSampleFrequency());
	_sampleDotSpacing= float(GetSize().Width) / _totalSampleCount;
}

void WaveDrawer::_drawBaseline(Gdiplus::Graphics& graph)
{
	Gdiplus::Pen pen(GetBaselineColor(), GetBaselineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetBaselineStyle()));
	graph.DrawLine(&pen, 0, _baseline, 0, _baseline);
}
