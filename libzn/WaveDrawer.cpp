#include "stdafx.h"
#include "WaveDrawer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WaveDrawer::_initialize()
{
	_velocity = 250;
	_waveHeight=GetSize().Height*3/8;
	_baseline=GetSize().Height/200 * 100;
	SetLineWidth(5);
	SetLineColor(Gdiplus::Color::LightBlue);
}

WaveDrawer::WaveDrawer()
	:_layoutPercent(1)
	,_pSignalChannel(nullptr)
{
	_initialize();
}

WaveDrawer::WaveDrawer(SignalChannel* pChannel, double layoutPercent)
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
	SignalBuffer<unsigned short> & buffer=_pSignalChannel->getSignalBuffer();
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const int sampleCount = min(_totalSampleCount, buffer.getLength());
	const int startSample=buffer.getLength() <= buffer.getSize() ? 0 : buffer.getLength() - buffer.getSize(); 
	int halfHeight=_waveHeight / 2;
	const int offset= _baseline;// + halfHeight;
	float startX = GetSize().Width - sampleCount * _sampleDotSpacing;
	float startY =offset - _waveHeight * buffer.getBuffer()[startSample%buffer.getSize()] /65535.0f;
	for(int i= 0; i< sampleCount - 1; i +=3)
	{
		const float endX = startX + _sampleDotSpacing + _sampleDotSpacing + _sampleDotSpacing;
		const float endY = offset - buffer.getBuffer()[(i + startSample + 1) % buffer.getSize()] * _waveHeight / 65535.0f;
		graph.DrawLine(&pen, int(startX), int(startY), int(endX), int(endY));
		startY = endY;
		startX = endX;
	}
}

void WaveDrawer::OnSizeChanged()
{
	__super::OnSizeChanged();
	_baseline=(GetSize().Height/200) * 100;
	_waveHeight=GetSize().Height*3/8;
	_totalSampleCount = int(GetSize().Width / ScreenInfo::GetScreenInfo().GetDpmmX() / _velocity *
		_pSignalChannel->getSampleFrequency());
	_sampleDotSpacing= float(GetSize().Width) / _totalSampleCount;
}
