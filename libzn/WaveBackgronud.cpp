#include "stdafx.h"
#include "WaveBackgronud.h"
#include "../libDrawObject/LineObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void WaveBackground::initialize()
{
	SetFillColor(Gdiplus::Color::Black);
	SetLineColor(Gdiplus::Color::DarkGreen);
	SetLineWidth(5.0f);
	SetShowTracker(false);
	SetShowBorder(true);
}

WaveBackground::WaveBackground()
:_thickGridLineWidth(3.0f)
,_thickGridLineStyle(Gdiplus::DashStyleSolid)
,_thickGridColor(Gdiplus::Color(128, 0, 128,0))
,_thickGridSpacing(100)
,_thinGridLineWidth(1.0f)
,_thinGridLineStyle(Gdiplus::DashStyleDash)
,_thinGridLineColor(Gdiplus::Color(32, 192, 192, 192))
,_thinGridSpacing(20)
,_isDrawThickGrid(true)
,_isDrawThinGrid(true)
{
	initialize();
}
WaveBackground::WaveBackground(const Gdiplus::Point &position, const Gdiplus::Size & size)
:DrawObject(_T(""), position, size)
,_thickGridLineWidth(3.0f)
,_thickGridLineStyle(Gdiplus::DashStyleSolid)
,_thickGridColor(Gdiplus::Color(128, 0, 128,0))
,_thickGridSpacing(100)
,_thinGridLineWidth(1.0f)
,_thinGridLineStyle(Gdiplus::DashStyleDash)
,_thinGridLineColor(Gdiplus::Color(32, 192, 192, 192))
,_thinGridSpacing(20)
,_isDrawThickGrid(true)
,_isDrawThinGrid(true)
{
	initialize();
}
void WaveBackground::OnDraw(Gdiplus::Graphics & graph)
{
	Gdiplus::SolidBrush backgroundBrush(GetFillColor());
	Gdiplus::Pen thickGridPen(_thickGridColor, _thickGridLineWidth);
	thickGridPen.SetDashStyle(_thickGridLineStyle);
	Gdiplus::Pen thinGridPen(_thinGridLineColor, _thinGridLineWidth);
	thinGridPen.SetDashStyle(_thinGridLineStyle);

	graph.FillRectangle(&backgroundBrush, 0, 0, GetSize().Width, GetSize().Height);
	if(_isDrawThinGrid)
		DrawGrid(graph, &thinGridPen, Gdiplus::Point(0,0), GetSize(), _thinGridSpacing);
	if(_isDrawThickGrid)
		DrawGrid(graph, &thickGridPen, Gdiplus::Point(0,0), GetSize(), _thickGridSpacing);
	
	for(size_t i=0; i< _baselines.size(); i++)
	{
		_baselines[i]->Draw(graph);
	}
}

void WaveBackground::SetShowThickGrid(bool isShow)
{
	_isDrawThickGrid = isShow;
}

bool WaveBackground::GetShowThickGrid()
{
	return _isDrawThickGrid;
}

void WaveBackground::SetShowThinGrid(bool isShow)
{
	_isDrawThinGrid = isShow;
}

bool WaveBackground::GetShowThinGrid()
{
	return _isDrawThinGrid;
}

void WaveBackground::SetThickGridLineWidth(float width)
{
	_thickGridLineWidth = width;
}

float WaveBackground::GetThickGridLineWidth()
{
	return _thickGridLineWidth;
}

void WaveBackground::SetThickGridLineColor(Gdiplus::Color color)
{
	_thickGridColor = color;
}

Gdiplus::Color WaveBackground::GetThickGridLineColor()
{
	return _thickGridColor;
}

void WaveBackground::SetThickGridLineStyle(int style)
{
	_thickGridLineStyle = Gdiplus::DashStyle(style);
}

int WaveBackground::GetThickGridLineStyle()
{
	return _thickGridLineStyle;
}

void WaveBackground::SetThickGridSpacing(int spacing)
{
	_thickGridSpacing = spacing;
}

int WaveBackground::GetThickGridSpacing()
{
	return _thickGridSpacing;
}

void WaveBackground::SetThinGridLineWidth(float width)
{
	_thinGridLineWidth = width;
}

float WaveBackground::GetThinGridLineWidth()
{
	return _thinGridLineWidth;
}

void WaveBackground::SetThinGridLineColor(Gdiplus::Color color)
{
	_thinGridLineColor = color;
}

Gdiplus::Color WaveBackground::GetThinGridLineColor()
{
	return _thinGridLineColor;
}

void WaveBackground::SetThinGridLineStyle(int style)
{
	_thinGridLineStyle = Gdiplus::DashStyle(style);
}

int WaveBackground::GetThinGridLineStyle()
{
	return _thinGridLineStyle;
}

void WaveBackground::SetThinGridSpacing(int spacing)
{
	_thinGridSpacing = spacing;
}

int WaveBackground::GetThinGridSpacing()
{
	return _thinGridSpacing;
}

int WaveBackground::GetBaselineCount()
{
	return _baselines.size();
}

void WaveBackground::AddBaseline(const CString& name, int y, float width, Gdiplus::Color color)
{
	CLineObject *pLine = new CLineObject(name, 0, y, GetSize().Width, 0);
	pLine->SetParent(this);
	pLine->SetLineColor(color);
	pLine->SetLineWidth(width);
	_baselines.push_back(pLine);
}

CLineObject* WaveBackground::GetBaseline(int i)
{
	return _baselines[i];
}

void WaveBackground::SetBaseline(int i, int y)
{
	_baselines[i]->SetPosition(Gdiplus::Point(0, y));
}

void WaveBackground::SetBaselineWidth(int i, float width)
{
	_baselines[i]->SetLineWidth(width);
}

float WaveBackground::GetBaselineWidth(int i)
{
	return _baselines[i]->GetLineWidth();
}

void WaveBackground::SetBaselineColor(int i, Gdiplus::Color color)
{
	_baselines[i]->SetLineColor(color);
}

Gdiplus::Color WaveBackground::GetBaselineColor(int i)
{
	return _baselines[i]->GetLineColor();
}

void WaveBackground::DrawGrid(Gdiplus::Graphics & graph, Gdiplus::Pen * pPen, 
			  const Gdiplus::Point &pos, const Gdiplus::Size& size, int interval)
{
	for(int i=0; i < size.Width; i +=interval)
	{
		graph.DrawLine(pPen, pos.X + i,  pos.Y, pos.X + i, pos.Y + size.Height);
	}
	for(int i=0; i < size.Height; i +=interval)
	{
		graph.DrawLine(pPen, pos.X, pos.Y + i, pos.X + size.Width, pos.Y + i);
	}
}

void WaveBackground::OnDrawBorder( Gdiplus::Graphics & graph )
{
	Gdiplus::Size size=GetSize();
	float x= GetLineWidth()/2;
	float y= GetLineWidth()/2;
	float w=size.Width - GetLineWidth();
	float h=size.Height - GetLineWidth();

	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle((Gdiplus::DashStyle)GetLineStyle());
	graph.DrawRectangle(&pen, x, y, w, h);
}

DrawObject* WaveBackground::CreateInstance()
{
	return new WaveBackground;
}
