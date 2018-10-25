#include "stdafx.h"
#include "WaveDrawer.h"
#include "WaveDrawerAppearance.h"
#include "../libDrawObject/RectObject.h"

#define DEFAULT_WAVE_SCALE 0.8f

static WaveDrawerAppearance DefaultDrawerAppearance;
void WaveDrawer::_initialize()
{	
	SetAppearance(DefaultDrawerAppearance);	
	_scale=DEFAULT_WAVE_SCALE;
	_waveHeight=GetSize().Height*_scale;
	_pSignalChannel=nullptr;
	_pCacheBitmap=nullptr;
	_layoutPercent=100;
	_lastEndSample=0;
	_lastEndX=0;
}

WaveDrawer::WaveDrawer()
{
	_initialize();
}

WaveDrawer::WaveDrawer(SignalChannel* pChannel, const double layoutPercent)
	:DrawObject(pChannel->getLabel(), Gdiplus::Point(0,0), Gdiplus::Size(0,0))
{

	_initialize();
	_pSignalChannel=pChannel;
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

void WaveDrawer::SetVelocity(double velocity)
{
	getThisAppearance().Velocity = velocity;
}

double WaveDrawer::GetVelocity()
{
	return getThisAppearance().Velocity;
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

void WaveDrawer::_drawWaveByPixels(Gdiplus::Graphics& graphics,double* pBuffer, size_t bufferSize,int offsetY, int startX, int endX, size_t startIndex, size_t endIndex)
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

void WaveDrawer::_drawWaveBySamples(Gdiplus::Graphics& graphics, double* pBuffer, size_t bufferSize, int offsetY, int startX, int endX,
	size_t startIndex, size_t endIndex)
{
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const double step=double(endX - startX)/(endIndex - startIndex);
	const double height=GetSize().Height * _scale/32767;
	double x=startX;
	int y=offsetY -  pBuffer[startIndex%bufferSize]* height;
	for(int i=startIndex + 1; i < endIndex; ++i)
	{
		const double x1= x+step;
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


void _drawScopeByPixel(
	Gdiplus::Graphics & graphics,Gdiplus::Pen* pen, 
	SignalBuffer<double> & buffer, size_t start, size_t end,
	int startX,
	int endX,
	float scale, float zero)
{
    const size_t bufferSize=buffer.getSize();
    const float step=float(end-start)/(endX - startX);
	float current=start;
	int x0 = startX;
	int y0 =zero - int(buffer.getBuffer()[start%bufferSize] * scale);
	for(int i=startX + 1; i<=endX; ++i)
	{
		current +=step;
	    const int x1=i;
	    const int y1=zero - buffer.getBuffer()[int(current)%bufferSize] * scale;
		graphics.DrawLine(pen, x0, y0, x1, y1);
		x0=x1;
		y0=y1;
	}
}
void _drawScopeBySample(
	Gdiplus::Graphics & graphics,Gdiplus::Pen* pen, 
	SignalBuffer<double> & buffer, size_t start, size_t end,
	int startX,
	int endX,
	float scale, float zero)
{
    const size_t bufferSize=buffer.getSize();
    const float step=float(endX-startX)/(end - start);
	float current=startX;
	int x0 = startX;
	int y0 =zero - int(buffer.getBuffer()[start%bufferSize] * scale);
	for(int i=start + 1; i<=end; ++i)
	{
		current +=step;
	    const int x1=current;
	    const int y1=zero - buffer.getBuffer()[i%bufferSize] * scale;
		graphics.DrawLine(pen, x0, y0, x1, y1);
		x0=x1;
		y0=y1;
	}
}
void _drawScope(
	Gdiplus::Graphics & graphics,Gdiplus::Pen* pen, 
	SignalBuffer<double> & buffer, size_t start, size_t end,
	int startX,
	int endX,
	float scale, float zero)
{
	if(end-start < endX - startX)
	{
		_drawScopeBySample(graphics, pen, buffer, start, end, startX, endX, scale, zero);
		return;
	}
	_drawScopeByPixel(graphics, pen, buffer, start, end, startX, endX, scale, zero);
}
void WaveDrawer::_drawFull(Gdiplus::Graphics& graphics, SignalBuffer<double> & buffer, size_t totalSampleCount, double sampleDotSpacing)
{
    const Gdiplus::CompositingMode compositingMode=graphics.GetCompositingMode();
	graphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
	graphics.Clear(Gdiplus::Color::Transparent);

	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	size_t current= buffer.getLength();
	const int width=GetSize().Width;
	const float height=GetSize().Height * _scale / 32767;

	const size_t sampleCount = min(totalSampleCount, current);
	const size_t startSample=  current <= sampleCount ? 0 : current - sampleCount;
	_lastEndSample = buffer.getLength();
	const int offset= _baseline;
	const int startX = int(width - sampleCount * sampleDotSpacing);
	_lastEndX = width;
	if(IsShowBaseline())
	{
		_drawBaseline(graphics, 0, width);
	}
	_drawScope(graphics, &pen, buffer, startSample, _lastEndSample, startX, _lastEndX, height, offset);
	graphics.SetCompositingMode(compositingMode);
}
void WaveDrawer::OnDraw( Gdiplus::Graphics & graph )
{
	Gdiplus::Size size=GetSize();
	UICoordinateHelper::GetHelper().LPtoDP(&size, 1);
	if(_pCacheBitmap==nullptr ||
		_pCacheBitmap->GetWidth() != size.Width ||
		_pCacheBitmap->GetHeight() !=size.Height)
	{
		delete _pCacheBitmap;
		_pCacheBitmap=BitmapCreate(size.Width, size.Height, PixelFormat32bppARGB);
	}
	Gdiplus::Graphics memGraphics(_pCacheBitmap);
	memGraphics.ScaleTransform(1.0f/UICoordinateHelper::GetHelper().HorizontalLmPerDeviceUnit, 1.0f/UICoordinateHelper::GetHelper().VerticalLmPerDeviceUnit);	
	memGraphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	memGraphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	memGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	memGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	SignalBuffer<double> & buffer=_pSignalChannel->getSignalBuffer();
	const size_t current=buffer.getLength();
    const size_t totalSampleCount=int(
		GetSize().Width / ScreenInfo::GetScreenInfo().GetDpmmX() / GetVelocity() * _pSignalChannel->
		getSampleFrequency());
    const double sampleDotSpacing= double(GetSize().Width) / totalSampleCount;
	if(GetWaveDrawMode()==DRAW_ROLLING)
	{
		_drawFull(memGraphics, buffer, totalSampleCount, sampleDotSpacing);		
	}
	else 
	{
		_drawErase(memGraphics, buffer, totalSampleCount, sampleDotSpacing);		
	}
	graph.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
	graph.DrawImage(_pCacheBitmap, Gdiplus::Rect(Gdiplus::Point(0, 0), GetSize()), 0, 0, _pCacheBitmap->GetWidth(),
	                _pCacheBitmap->GetHeight(), Gdiplus::UnitPixel);
	CString info;
	Gdiplus::Font font(_T("宋体"), 50, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel, nullptr);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	format.SetLineAlignment(Gdiplus::StringAlignmentNear);
	Gdiplus::SolidBrush brush(Gdiplus::Color::White); 
	info.Format(_T("total:%.2fs, start: %ld, length: %d. "), current / getChannelBuffer()->getSampleFrequency(),
	            _lastEndSample, totalSampleCount);
	graph.DrawString(info, -1, &font,Gdiplus::PointF(0,0), &format, &brush );
	
}

void WaveDrawer::OnSizeChanged()
{
	__super::OnSizeChanged();
	if(!_pSignalChannel) return;
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
	SetClipRect(Gdiplus::Rect(Gdiplus::Point(0,0), GetSize()));
	if(!GetClipRect().IsEmptyArea()){
		SetUsingClip(true);
	}
}

void WaveDrawer::_drawBaseline(Gdiplus::Graphics& graph, int startX, int width)
{
	Gdiplus::Pen pen(GetBaselineColor(), GetBaselineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetBaselineStyle()));
	graph.DrawLine(&pen, startX, _baseline, startX + width, _baseline);
}

void WaveDrawer::_drawErase(Gdiplus::Graphics& graphics, SignalBuffer<double>& buffer, size_t totalSampleCount, double sampleDotSpacing)
{
	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	Gdiplus::SolidBrush spot(Gdiplus::Color::Red);
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	const int height=GetSize().Height;
	const size_t current=buffer.getLength();
	//需要重绘的宽度  logic unit
	const int drawWidth=(current - _lastEndSample) * sampleDotSpacing;
    const int spotSize=20;
	//计算需要擦除的宽度
	const int eraseWidth= drawWidth + GetEraseWidth() +  spotSize;
	Gdiplus::SolidBrush brush(Gdiplus::Color::Transparent);
	const Gdiplus::CompositingMode compositingMode= graphics.GetCompositingMode();
	graphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);	
	graphics.FillRectangle(&brush, _lastEndX, 0, eraseWidth, height);
	if(IsShowBaseline())
	{
		_drawBaseline(graphics, _lastEndX, eraseWidth );
	}
	_drawScope(graphics, &pen, buffer, _lastEndSample, current, _lastEndX, _lastEndX + drawWidth, height *_scale /32767.0f, _baseline);
	graphics.FillEllipse(&spot, _lastEndX + drawWidth, _baseline - int(buffer.getBuffer()[current % buffer.getSize()] * _scale * height / 32767), spotSize, spotSize);
	_lastEndSample=current;
	_lastEndX +=drawWidth;
	if(_lastEndX > GetSize().Width) _lastEndX =0;
	graphics.SetCompositingMode(compositingMode);

}
