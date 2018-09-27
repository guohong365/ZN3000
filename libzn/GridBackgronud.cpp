#include "stdafx.h"
#include "../libDrawObject/LineObject.h"
#include "GridBackgroundAppearance.h"
#include "GridBackgronud.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_FILL_COLOR 0xFF000000
#define DEFAULT_LINE_COLOR 0xFF006400
#define DEFAULT_LINE_WIDTH 5
#define DEFAULT_SHOW_BORDER true
#define DEFAULT_SHOW_TRACKER false

static GridBackgroundAppearance DefaultAppearance;
void GridBackground::_initialize()
{
	SetAppearance(DefaultAppearance);	
}

GridBackgroundAppearance& GridBackground::_getThisAppearance()
{
	return dynamic_cast<GridBackgroundAppearance&>(GetAppearance());
}

GridBackground::GridBackground()
{
	_initialize();
}
GridBackground::GridBackground(const Gdiplus::Point &position, const Gdiplus::Size & size)
:DrawObject(_T(""), position, size)
{
	_initialize();
}

GridBackground::~GridBackground()
{
}

void GridBackground::OnDraw(Gdiplus::Graphics & graph)
{	
	Gdiplus::Pen thickGridPen(GetThickGridLineColor(),GetThickGridLineWidth());
	thickGridPen.SetDashStyle(Gdiplus::DashStyle(GetThickGridLineStyle()));
	Gdiplus::Pen thinGridPen(GetThinGridLineColor(), GetThinGridLineWidth());
	thinGridPen.SetDashStyle(Gdiplus::DashStyle(GetThinGridLineStyle()));
	
	if(GetShowThinGrid())
		_drawGrid(graph, &thinGridPen, Gdiplus::Point(0,0), GetSize(), GetThinGridSpacing());
	if(GetShowThickGrid())
		_drawGrid(graph, &thickGridPen, Gdiplus::Point(0,0), GetSize(), GetThickGridSpacing());
}

void GridBackground::OnDrawFillObject(Gdiplus::Graphics& graph)
{
	Gdiplus::SolidBrush backgroundBrush(GetFillColor());
	graph.FillRectangle(&backgroundBrush, 0, 0, GetSize().Width, GetSize().Height);
}

void GridBackground::SetShowThickGrid(bool isShow)
{
	_getThisAppearance().ShowThickGridLine = isShow;
}

bool GridBackground::GetShowThickGrid()
{
	return _getThisAppearance().ShowThickGridLine;
}

void GridBackground::SetShowThinGrid(bool isShow)
{
	_getThisAppearance().ShowThinGridLine=isShow;
}

bool GridBackground::GetShowThinGrid()
{
	return _getThisAppearance().ShowThinGridLine;
}

void GridBackground::SetThickGridLineWidth(float width)
{
	_getThisAppearance().ThickGridLineWidth = width;
}

float GridBackground::GetThickGridLineWidth()
{
	return _getThisAppearance().ThickGridLineWidth;
}

void GridBackground::SetThickGridLineColor(Gdiplus::Color color)
{
	_getThisAppearance().ThickGridLineColor=color.GetValue();
}

Gdiplus::Color GridBackground::GetThickGridLineColor()
{
	return _getThisAppearance().ThickGridLineColor;
}

void GridBackground::SetThickGridLineStyle(int style)
{
	_getThisAppearance().ThickGridLineStyle=style;
}

int GridBackground::GetThickGridLineStyle()
{
	return _getThisAppearance().ThickGridLineStyle;
}

void GridBackground::SetThickGridSpacing(int spacing)
{
	_getThisAppearance().ThickGridLineSpacing = spacing;
}

int GridBackground::GetThickGridSpacing()
{
	return _getThisAppearance().ThickGridLineSpacing;
}

void GridBackground::SetThinGridLineWidth(float width)
{
	_getThisAppearance().ThinGridLineWidth = width;
}

float GridBackground::GetThinGridLineWidth()
{
	return _getThisAppearance().ThinGridLineWidth;
}

void GridBackground::SetThinGridLineColor(Gdiplus::Color color)
{
	_getThisAppearance().ThinGridLineColor = color.GetValue();
}

Gdiplus::Color GridBackground::GetThinGridLineColor()
{
	return _getThisAppearance().ThinGridLineColor;
}

void GridBackground::SetThinGridLineStyle(int style)
{
	_getThisAppearance().ThinGridLineStyle = style;
}

int GridBackground::GetThinGridLineStyle()
{
	return _getThisAppearance().ThinGridLineStyle;
}

void GridBackground::SetThinGridSpacing(int spacing)
{
	_getThisAppearance().ThinGridLineSpacing = spacing;
}

int GridBackground::GetThinGridSpacing()
{
	return _getThisAppearance().ThinGridLineSpacing;
}

void GridBackground::_drawGrid(Gdiplus::Graphics & graph, Gdiplus::Pen * pPen, 
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

void GridBackground::OnDrawBorder( Gdiplus::Graphics & graph )
{
	const Gdiplus::Size size=GetSize();
	const float x= GetLineWidth()/2;
	const float y= GetLineWidth()/2;
	const float w=size.Width - GetLineWidth();
	const float h=size.Height - GetLineWidth();

	Gdiplus::Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(Gdiplus::DashStyle(GetLineStyle()));
	graph.DrawRectangle(&pen, x, y, w, h);
}

DrawObject* GridBackground::CreateInstance()
{
	return new GridBackground;
}
