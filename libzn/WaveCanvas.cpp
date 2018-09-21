#include "stdafx.h"
#include "WaveBackgronud.h"
#include "WaveCanvas.h"
#include "../libDrawObject/ImageUtility.h"
#include "../libDrawObject/UIHelper.h"

void WaveCanvas::initialize()
{
	_pBackground= nullptr;
	_pWaveBackground=new WaveBackground();
	_pWaveBackground->SetFillColor(Gdiplus::Color(DEFAULT_WAVE_BACKGROUND_COLOR));

	_pWaveBackground->SetParent(this);
	SetFontFace(_T("Arial"));
	SetFontSize(40.0f);
	SetFontStyle(Gdiplus::FontStyleRegular);
	_pFont=new Gdiplus::Font(GetFontFace(), GetFontSize(), GetFontStyle(), Gdiplus::UnitPixel, NULL);	
	_labelInterval = 100;
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
	for(int i=0; i< _waveDrawers.size(); i ++)
	{
		delete _waveDrawers[i];
	}
}

void WaveCanvas::AddWave(SignalChannel* pChannel, int percent)
{
	WaveDrawer* pDrawer = new WaveDrawer(pChannel, Gdiplus::Point(0, 0), Gdiplus::Size(0, 0));
	pDrawer->SetParent(this);
	_waveDrawers.push_back(pDrawer);
	_layoutRatio.push_back(percent);
	_pWaveBackground->AddBaseline(pChannel->getLabel(), 0, 5, DEFAULT_WAVE_BASELINE_COLOR);
}

int WaveCanvas::GetWaveCount() const
{
	return _waveDrawers.size();
}

WaveDrawer* WaveCanvas::GetWave(int i)
{
	ASSERT(i>=0 && i< _waveDrawers.size());
	return _waveDrawers[i];
}

//void WaveCanvas::AddWave( WaveBuffer<long> * pBuffer, int percent )
//{
//	WaveDrawer * pDrawer=new WaveDrawer(pBuffer, Gdiplus::Point(0,0), Gdiplus::Size(0,0));
//	pDrawer->SetParent(this);
//	_waveDrawers.push_back(pDrawer);
//	_layoutRatio.push_back(percent);
//	_pWaveBackground->AddBaseline(0, 5, DEFAULT_WAVE_BASELINE_COLOR);
//}


void WaveCanvas::OnSizeChanged()
{
	__super::OnSizeChanged();
	_pWaveBackground->SetPosition(Gdiplus::Point(100,100));
	_pWaveBackground->SetSize(Gdiplus::Size(GetSize().Width - 200, GetSize().Height - 200));
	_calcLayout();
}

void WaveCanvas::_calcLayout()
{
	int sum = 0;
	for(int i=0; i< _layoutRatio.size(); i++)
	{
		sum +=_layoutRatio[i];
	}
	Gdiplus::Point pos=_pWaveBackground->GetPosition();
	Gdiplus::Size size=GetSize();
	Gdiplus::Point baselinePos;
	size.Width = _pWaveBackground->GetSize().Width;
	for (int i=0; i< _waveDrawers.size(); i++)
	{
		size.Height = _pWaveBackground->GetSize().Height * _layoutRatio[i] / sum;
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
		if(_pBackground)
		{
			graph.DrawImage(_pBackground, Gdiplus::Rect(Gdiplus::Point(0,0), GetSize()), 0,0, _pBackground->GetWidth(), _pBackground->GetHeight(), Gdiplus::UnitPixel);
		}
		else
		{
			_pWaveBackground->Draw(graph);
		}
		for(int i=0; i< _waveDrawers.size(); i ++)
		{
			_waveDrawers[i]->Draw(graph);
		}
}

//dx, dy, 设备坐标单位——像素
void WaveCanvas::_drawBackground()
{

}

void WaveCanvas::PrepareCanvas( int dx, int dy )
{
	delete _pBackground;
	_pBackground=BitmapCreate(dx, dy, PixelFormat32bppRGB);
	Gdiplus::Graphics graph(_pBackground);
	graph.ScaleTransform(1.0f/CUICoordinateHelper::GetHelper()._horzLMPerDeviceUnit , 1.0f/CUICoordinateHelper::GetHelper()._vertLMPerDeviceUnit);	
	_pWaveBackground->Draw(graph);
	for (int i=0; i< _waveDrawers.size(); i ++)
	{
		_drawLabel(graph, 0,_waveDrawers[i]->GetPosition().Y,
			_waveDrawers[i]->GetPosition().X - 10, _waveDrawers[i]->GetSize().Height,
			_pWaveBackground->GetPosition().X + _pWaveBackground->GetBaseline(i)->GetPosition().Y);
	}
	_drawHorizontalLabel(graph);
}

void WaveCanvas::_drawLabel( Gdiplus::Graphics &graph, int x, int y, int width, int height, int baseline )
{
	CString strLebel;
	int upperStep=(baseline - y ) / _labelInterval -1;
	int downStep= (height + y - baseline) / _labelInterval -1;
	
	

	Gdiplus::SolidBrush brush(Gdiplus::Color::LightGreen);
	Gdiplus::RectF layout(x, baseline - _labelInterval, width, _labelInterval * 2);
	Gdiplus::StringFormat stringFormat;
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);

	graph.DrawString(_T("0"), 1, _pFont, layout, &stringFormat, &brush);
	for(int i = 1; i <= upperStep; i++ )
	{
		layout.Y =baseline  - i * _labelInterval  - _labelInterval;
		strLebel.Format(_T("%d"), i * 10);
		graph.DrawString(strLebel, strLebel.GetLength(), _pFont, layout, &stringFormat, &brush);
	}
	layout.Y = baseline;
	for(int i = 1; i< downStep; i++)
	{
		layout.Y = baseline + i * _labelInterval - _labelInterval;
		strLebel.Format(_T("-%d"), i * 10);
		graph.DrawString(strLebel, strLebel.GetLength(), _pFont,layout, &stringFormat, &brush);
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
