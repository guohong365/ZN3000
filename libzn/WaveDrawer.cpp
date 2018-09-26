#include "stdafx.h"
#include "WaveDrawer.h"
#include "WaveDrawerAppearance.h"

#define DEFAULT_WAVE_SCALE 0.8f

static WaveDrawerAppearance DefaultDrawerAppearance;
void WaveDrawer::_initialize()
{	
	SetAppearance(DefaultDrawerAppearance);	
	_velocity = 250;
	_scale=DEFAULT_WAVE_SCALE;
	_waveHeight=GetSize().Height*_scale;
	_pBaselinePen=new Gdiplus::Pen(GetBaselineColor(), GetBaselineWidth());
	_pBaselinePen->SetDashStyle(Gdiplus::DashStyle(GetBaselineStyle()));
	_pWaveLinePen=new Gdiplus::Pen(GetLineColor(), GetLineWidth());
	_pWaveLinePen->SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
}

WaveDrawer::WaveDrawer()
	: _layoutPercent(100)
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

BaselineAlignment WaveDrawer::GetBaselineAlignment()
{
	return BaselineAlignment(getThisAppearance().BaselineAlignment);
}

void WaveDrawer::SetBaselineAlignment(BaselineAlignment alignment)
{
	getThisAppearance().BaselineAlignment = alignment;
}

float WaveDrawer::GetBaselineWidth()
{
	return getThisAppearance().BaselineWidth;
}

void WaveDrawer::SetBaselineWidth(float width)
{
	getThisAppearance().BaselineWidth = width;
}

Gdiplus::Color WaveDrawer::GetBaselineColor()
{
	return getThisAppearance().BaselineColor;
}

void WaveDrawer::SetBaselineColor(Gdiplus::Color color)
{
	getThisAppearance().BaselineColor = color.GetValue();
}

int WaveDrawer::GetBaselineStyle()
{
	return getThisAppearance().BaselineStyle;
}

void WaveDrawer::SetBaselineStyle(int style)
{
	getThisAppearance().BaselineStyle = style;
}

bool WaveDrawer::IsShowBaseline()
{
	return getThisAppearance().ShowBaseline;
}

void WaveDrawer::SetShowBaseline(bool isShow)
{
	getThisAppearance().ShowBaseline = isShow;
}

DrawMode WaveDrawer::GetWaveDrawMode()
{
	return DrawMode(getThisAppearance().WaveDrawMode);
}

void WaveDrawer::SetWaveDrawMode(DrawMode drawMode)
{
	getThisAppearance().WaveDrawMode = drawMode;
}

int WaveDrawer::GetEraseWidth()
{
	return getThisAppearance().EraseWidth;
}

void WaveDrawer::SetEraseWidth(int width)
{
	getThisAppearance().EraseWidth = width;
}

WaveDrawerAppearance& WaveDrawer::getThisAppearance()
{
	return static_cast<WaveDrawerAppearance&>(GetAppearance());
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
	const int sampleCount = min(_totalSampleCount, buffer.getLength());
	const int startSample=buffer.getLength() <= buffer.getSize() ? 0 : buffer.getLength() - buffer.getSize(); 
	const int offset= _baseline;
	float startX = GetSize().Width - sampleCount * _sampleDotSpacing;
	float startY =offset - GetSize().Height * _scale * buffer.getBuffer()[startSample%buffer.getSize()] /32767;
	for(int i= 0; i< sampleCount - 2; i +=3)
	{
		const float endX = startX + _sampleDotSpacing*3;
		const float endY = offset - buffer.getBuffer()[(startSample + i + 1) % buffer.getSize()] * GetSize().Height * _scale / 32767;
		graph.DrawLine(&pen, int(startX), int(startY), int(endX), int(endY));
		startY = endY;
		startX = endX;
	}
}

void WaveDrawer::OnSizeChanged()
{
	__super::OnSizeChanged();
	if(GetBaselineAlignment()==ZERO_VALUE)
	{
		_baseline=GetSize().Height/2;
	}
	else
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
	_waveHeight=GetSize().Height*_scale;
	_totalSampleCount = int(GetSize().Width / ScreenInfo::GetScreenInfo().GetDpmmX() / _velocity *	_pSignalChannel->getSampleFrequency());
	_sampleDotSpacing= float(GetSize().Width) / _totalSampleCount;
}

void WaveDrawer::_drawBaseline(Gdiplus::Graphics& graph)
{
	Gdiplus::Pen pen(GetBaselineColor(), GetBaselineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetBaselineStyle()));
	graph.DrawLine(&pen, 0, _baseline, GetSize().Width, _baseline);
}
