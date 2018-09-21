#include "stdafx.h"
#include "WaveBackgronud.h"

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

	for(int i=0; i< _baselines.size(); i++)
	{
		Gdiplus::Pen baseLinePen(GetBaselineColor(i), GetBaselineWidth(i));
		graph.DrawLine(&baseLinePen, 0, GetBaseline(i), GetSize().Width, GetBaseline(i));
	}
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
