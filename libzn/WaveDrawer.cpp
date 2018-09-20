#include "stdafx.h"
#include "WaveDrawer.h"
#include "../libDrawObject/ImageUtility.h"

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
:_pSignalChannel(nullptr)
{
	_initialize();
}

WaveDrawer::WaveDrawer(SignalChannel* pChannel, const Gdiplus::Point& pt, const Gdiplus::Size& size):
	CDrawObject(pChannel->getLabel(), pt, size)
	, _pSignalChannel(pChannel)
{
	_initialize();
}

WaveDrawer::~WaveDrawer()
{
	delete _pSignalChannel;
}

void WaveDrawer::OnDraw( Gdiplus::Graphics & graph )
{
	SignalBuffer<short> & buffer=_pSignalChannel->getSignalBuffer();
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const int sampleCount = min(_totalSampleCount, buffer.getLength());
	const int startSample=buffer.getLength() <= buffer.getSize() ? 0 : buffer.getLength() - buffer.getSize(); 
	int halfHeight=_waveHeight / 2;
	const int offset= _baseline;// + halfHeight;
	float startX = GetSize().Width - sampleCount * _sampleDotSpacing;
	float startY =offset - _waveHeight * buffer.getBuffer()[startSample%buffer.getSize()] /65535;
	for(int i= 0; i< sampleCount - 1; i +=3)
	{
		float endX = startX + _sampleDotSpacing + _sampleDotSpacing + _sampleDotSpacing;
		float endY = offset - buffer.getBuffer()[(i + startSample + 1) % buffer.getSize()] * _waveHeight / 65535;
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
