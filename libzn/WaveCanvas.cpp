#include "stdafx.h"
#include "WaveCanvas.h"
#include "GridBackgronud.h"
#define DEFAULT_PADDING_LEFT 400
#define DEFAULT_PADDING_RIGHT 400
#define DEFAULT_PADDING_TOP 200
#define DEFAULT_PADDING_BOTTOM 200
#define DEFAULT_CANVAS_BACKGROUND_COLOR 0xFF000000
void WaveCanvas::initialize()
{
	_pBackground= nullptr;
	_pWaveBackground=new GridBackground();
	_pWaveBackground->SetParent(this);
	SetFillColor(DEFAULT_CANVAS_BACKGROUND_COLOR);
	SetFontFace(_T("Arial"));
	SetFontSize(40.0f);
	SetFontStyle(Gdiplus::FontStyleRegular);
	_pFont=new Gdiplus::Font(GetFontFace(), GetFontSize(), GetFontStyle(), Gdiplus::UnitPixel, nullptr);	
	_labelInterval = 100;
	_paddingLeft=DEFAULT_PADDING_LEFT;
	_paddingRight=DEFAULT_PADDING_RIGHT;
	_paddingTop=DEFAULT_PADDING_TOP;
	_paddingBottom=DEFAULT_PADDING_BOTTOM;
}

WaveCanvas::WaveCanvas()
{
	initialize();
}

WaveCanvas::WaveCanvas( const Gdiplus::Point &pt, const Gdiplus::Size & size )
	:DrawObject(_T(""), pt, size)
{
	initialize();
}

WaveCanvas::~WaveCanvas()
{
	delete _pWaveBackground;
	delete _pBackground;
	Clear();
}

void WaveCanvas::AddWave(SignalChannel* pChannel, const double percent)
{
	WaveDrawer* pDrawer = new WaveDrawer(pChannel, percent);
	pDrawer->SetParent(this);
	_waveDrawers.push_back(pDrawer);
	_pWaveBackground->AddBaseline(pChannel->getLabel(), 0, 5, DEFAULT_WAVE_BASELINE_COLOR);
	_calcLayout();
}

int WaveCanvas::GetWaveCount() const
{
	return _waveDrawers.size();
}

void WaveCanvas::SetPadding(int left, int top, int right, int bottom)
{
	_paddingLeft=left;
	_paddingTop=top;
	_paddingRight=right;
	_paddingBottom=bottom;
}

WaveDrawer* WaveCanvas::GetWave(int i)
{
	ASSERT(i>=0 && i< _waveDrawers.size());
	return _waveDrawers[i];
}

void WaveCanvas::Clear()
{
	for (int i = 0; i < _waveDrawers.size(); i ++)
	{
		delete _waveDrawers[i];
	}
	_waveDrawers.clear();
}

void WaveCanvas::OnSizeChanged()
{
	__super::OnSizeChanged();
	Gdiplus::Size size=GetSize();	
	if(size.Width > _paddingLeft+_paddingRight)
	{
		size.Width -= _paddingLeft+_paddingRight;
	}
	if(size.Height > _paddingTop + _paddingBottom)
	{
		size.Height -=_paddingTop + _paddingBottom;
	}
	_pWaveBackground->SetSize(size);
	_pWaveBackground->SetPosition(Gdiplus::Point(_paddingLeft,_paddingTop));
	_calcLayout();
	_drawBackground();
}

void WaveCanvas::_calcLayout()
{
	double sum = 0;
	for(int i=0; i< _waveDrawers.size(); i++)
	{
		sum +=_waveDrawers[i]->GetLayoutPercent();
	}
	Gdiplus::Point pos=_pWaveBackground->GetPosition();
	Gdiplus::Size size=GetSize();
	Gdiplus::Point baselinePos;
	size.Width = _pWaveBackground->GetSize().Width;
	for (int i=0; i< _waveDrawers.size(); i++)
	{
		size.Height = _pWaveBackground->GetSize().Height * _waveDrawers[i]->GetLayoutPercent() / sum;
		_waveDrawers[i]->SetPosition(pos);
		_waveDrawers[i]->SetSize(size);
		baselinePos.Y = _waveDrawers[i]->GetBaseline();
		_waveDrawers[i]->Local2Global(&baselinePos);
		_pWaveBackground->Global2Local(&baselinePos);
		_pWaveBackground->SetBaseline(i, baselinePos.Y);
		pos.Y = (pos.Y + size.Height) / 100 * 100 ;
	}
}

void WaveCanvas::OnDraw( Gdiplus::Graphics & graph )
{
	if(!_pBackground)
	{
		_drawBackground();
	}
	graph.DrawImage(_pBackground, Gdiplus::Rect(Gdiplus::Point(0,0), GetSize()), 0,0, _pBackground->GetWidth(), _pBackground->GetHeight(), Gdiplus::UnitPixel);

	for(int i=0; i< _waveDrawers.size(); i ++)
	{
		_waveDrawers[i]->Draw(graph);
	}
}

//dx, dy, 设备坐标单位——像素
void WaveCanvas::_drawBackground()
{
	Gdiplus::Size size=GetSize();
	UICoordinateHelper::GetHelper().LPtoDP(&size, 1);
	if(!_pBackground ||
		_pBackground->GetWidth()!=size.Width ||
		_pBackground->GetHeight()!=size.Height)
	{
		delete _pBackground;
		_pBackground=BitmapCreate(size.Width, size.Height, PixelFormat32bppARGB);
		Gdiplus::Graphics graphics(_pBackground);
		graphics.ScaleTransform(1.0f/UICoordinateHelper::GetHelper().HorizontalLmPerDeviceUnit, 1.0f/UICoordinateHelper::GetHelper().VerticalLmPerDeviceUnit);	
		graphics.Clear(GetFillColor());
		_pWaveBackground->Draw(graphics);
	}
}

void WaveCanvas::_drawLabel( Gdiplus::Graphics &graph, int x, int y, int width, int height, int baseline ) const
{
	CString strLabel;
	const int upperStep=(baseline - y ) / _labelInterval -1;
	const int downStep= (height + y - baseline) / _labelInterval -1;

	Gdiplus::SolidBrush brush(Gdiplus::Color::LightGreen);
	Gdiplus::RectF layout(x, baseline - _labelInterval, width, _labelInterval * 2);
	Gdiplus::StringFormat stringFormat;
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);

	graph.DrawString(_T("0"), 1, _pFont, layout, &stringFormat, &brush);
	for(int i = 1; i <= upperStep; i++ )
	{
		layout.Y =baseline  - i * _labelInterval  - _labelInterval;
		strLabel.Format(_T("%d"), i * 10);
		graph.DrawString(strLabel, strLabel.GetLength(), _pFont, layout, &stringFormat, &brush);
	}
	layout.Y = baseline;
	for(int i = 1; i< downStep; i++)
	{
		layout.Y = baseline + i * _labelInterval - _labelInterval;
		strLabel.Format(_T("-%d"), i * 10);
		graph.DrawString(strLabel, strLabel.GetLength(), _pFont,layout, &stringFormat, &brush);
	}
}

void WaveCanvas::_drawHorizontalLabel(Gdiplus::Graphics & graph)
{
	if(_waveDrawers.empty())return;
	Gdiplus::SolidBrush brush(Gdiplus::Color::LightGreen);
	Gdiplus::RectF layout(0, _pWaveBackground->GetPosition().Y + _pWaveBackground->GetSize().Height + 10, 
		1000,(GetSize().Height - _pWaveBackground->GetSize().Height/2.0f));

	Gdiplus::StringFormat stringFormat;
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	const float hInc= static_cast<float>(500.0f)/_waveDrawers[0]->GetVelocity();
	const int hStep= _pWaveBackground->GetSize().Width / 500;
	float t;
	CString strLabel;
	for(int i=0; i< hStep; i++)
	{
		t=hInc * i;
		layout.X = _pWaveBackground->GetPosition().X + i * 500 - 500;
		strLabel.Format(_T("%2.2f"), t);
		graph.DrawString(strLabel, strLabel.GetLength(), _pFont,layout, &stringFormat, &brush);
	}
	layout.X = _pWaveBackground->GetPosition().X + _pWaveBackground->GetSize().Width;
	stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	graph.DrawString(_T("(s)"), -1, _pFont,layout, &stringFormat, &brush);
}

DrawObject* WaveCanvas::CreateInstance()
{
	return new WaveCanvas;
}
