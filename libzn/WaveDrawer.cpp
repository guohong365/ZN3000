#include "stdafx.h"
#include "WaveDrawer.h"
#include "WaveDrawerAppearance.h"
#include "../libDrawObject/RectObject.h"

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

void WaveDrawer::_drawWaveByPixels(Gdiplus::Graphics& graphics,float* pBuffer, SIZE_T bufferSize,int offsetY, int startX, int endX, SIZE_T startIndex, SIZE_T endIndex)
{
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const float step=float(endIndex - startIndex)/(endX - startX);
	float lastIndex=startIndex;
	const float height=GetSize().Height * _scale/32767;
	int x=startX;
	int y=offsetY - pBuffer[startIndex%bufferSize]* height;
	for(int i=startX + 1; i < endX; ++i)
	{
		const float currentIndex= lastIndex + step;
		const int x1= x+1;
		const int y1=offsetY - pBuffer[int(currentIndex)%bufferSize] * height;
		graphics.DrawLine(&pen, x, y, x1, y1);
		x=x1;
		y=y1;
		lastIndex =currentIndex;
	}
}

void WaveDrawer::_drawWaveBySamples(Gdiplus::Graphics& graphics, float* pBuffer, SIZE_T bufferSize, int offsetY, int startX, int endX,
	SIZE_T startIndex, SIZE_T endIndex)
{
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const float step=float(endX - startX)/(endIndex - startIndex);
	const float height=GetSize().Height * _scale/32767;
	float x=startX;
	int y=offsetY -  pBuffer[startIndex%bufferSize]* height;
	for(int i=startIndex + 1; i < endIndex; ++i)
	{
		const float x1= x+step;
		if(int(x1)==int(x))
		{
			x=x1;
			continue;
		}
		const int y1=offsetY - pBuffer[i%bufferSize] * height;
		graphics.DrawLine(&pen, int(x), y, int(x1), y1);
		x=x1;
		y=y1;
	}
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
	const SIZE_T current = buffer.getLength();
	const SIZE_T bufferSize=buffer.getSize();
	const float width=GetSize().Width;
	const float height=GetSize().Height * _scale / 32767;
	const SIZE_T sampleCount = min(_totalSampleCount, current);
	const long startSample=  current <= sampleCount ? 0 : current - sampleCount;

	const int offset= _baseline;
	float startX = width - sampleCount * _sampleDotSpacing;
	float startY =offset - height * _scale * buffer.getBuffer()[startSample%bufferSize] /32767;
	/*
	if(width > sampleCount)
	{
		_drawWaveBySamples(graph, buffer.getBuffer() + startSample, sampleCount, offset, startX, startX + width, startSample, startSample + sampleCount);
	}
	else
	{
		_drawWaveByPixels(graph, buffer.getBuffer() + startSample, sampleCount, offset, startX, startX + width, startSample, startSample + sampleCount);
	}
	*/	
	for(int i= 0; i< sampleCount - 2; i +=3)
	{
		const float endX = startX + _sampleDotSpacing + _sampleDotSpacing + _sampleDotSpacing;// + _sampleDotSpacing + _sampleDotSpacing;
		const float endY = offset - buffer.getBuffer()[(startSample + i + 1) % bufferSize] * height;
		graph.DrawLine(&pen, int(startX), int(startY), int(endX), int(endY));
		startY = endY;
		startX = endX;
	}
	CString info;
	Gdiplus::Font font(_T("ËÎÌו"), 50, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel, nullptr);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	format.SetLineAlignment(Gdiplus::StringAlignmentNear);
	Gdiplus::SolidBrush brush(Gdiplus::Color::White); 
	info.Format(_T("total:%.2fs, start: %ld, length: %d. "),current/getChannelBuffer()->getSampleFrequency(), startSample, _totalSampleCount);
	graph.DrawString(info, -1, &font,Gdiplus::PointF(0,0), &format, &brush );
	
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
	SetClipRect(Gdiplus::Rect(Gdiplus::Point(0,0), GetSize()));
	if(!GetClipRect().IsEmptyArea()){
		SetUsingClip(true);
	}
}

void WaveDrawer::_drawBaseline(Gdiplus::Graphics& graph)
{
	Gdiplus::Pen pen(GetBaselineColor(), GetBaselineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetBaselineStyle()));
	graph.DrawLine(&pen, 0, _baseline, GetSize().Width, _baseline);
}
