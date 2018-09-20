#include "stdafx.h"
#include "WaveDrawer.h"
#include "../libDrawObject/ImageUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WaveDrawer::initialize()
{
	_velocity = 250;
	_waveHeight=GetSize().Height*3/8;
	_baseline=GetSize().Height/200 * 100;
	SetLineWidth(5);
	SetLineColor(Gdiplus::Color::LightBlue);
}

WaveDrawer::WaveDrawer()
: _pWaveBuffer(NULL)
{
	initialize();
}

WaveDrawer::~WaveDrawer(void)
{
	delete _pWaveBuffer;
}

void WaveDrawer::OnDraw( Gdiplus::Graphics & graph )
{
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	int sampleCount = min(_totalSampleCount, _pWaveBuffer->getLength());
	int startSample=_pWaveBuffer->getLength() <= _pWaveBuffer->getSize() ? 0 : _pWaveBuffer->getLength() - _pWaveBuffer->getSize(); 
	int halfHeight=_waveHeight / 2;
	int offset= _baseline;// + halfHeight;
	float startX = GetSize().Width - sampleCount * _sampleDotSpacing;
	float startY =offset - _pWaveBuffer->getBuffer()[startSample%_pWaveBuffer->getSize()] * _waveHeight /65535;
	float endX;
	float endY;
	for(int i= 0; i< sampleCount - 1; i +=3)
	{
		endX = startX + _sampleDotSpacing  + _sampleDotSpacing + _sampleDotSpacing;
		endY= offset - _pWaveBuffer->getBuffer()[(i + startSample + 1)%_pWaveBuffer->getSize()] * _waveHeight /65535;
		graph.DrawLine(&pen, (int)startX, (int)startY, (int)endX, (int)endY);
		startY = endY;
		startX = endX;
	}
}

void WaveDrawer::OnSizeChanged()
{
	__super::OnSizeChanged();
	_baseline=(GetSize().Height/200) * 100;
	_waveHeight=GetSize().Height*3/8;
	_totalSampleCount = (int)(GetSize().Width/ ScreenInfo::GetScreenInfo().GetDpmmX() / _velocity / _pWaveBuffer->getSampleRate());
	_sampleDotSpacing= (float)GetSize().Width / _totalSampleCount;
}
