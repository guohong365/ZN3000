#include "StdAfx.h"
#include "GridBackgroundAppearance.h"

#define DEFAULT_SHOW_THICK_GRID true
#define DEFAULT_SHOW_THIN_GRID true
#define DEFAULT_GRID_THICK_LINE_WIDTH 3
#define DEFAULT_GRID_THIN_LINE_WIDTH 1
#define DEFAULT_GRID_THICK_LINE_STYLE 0
#define DEFAULT_GRID_THIN_LINE_STYLE 1
#define DEFAULT_GRID_THICK_SPACING 100
#define DEFAULT_GRID_THIN_SPACING 20
#define DEFAULT_GRID_THICK_LINE_COLOR 0x80008000
#define DEFAULT_GRID_THIN_LINE_COLOR  0x20C0C0C0
#define DEFAULT_GRID_FILL_COLOR 0xFF004000
#define DEFAULT_GRID_BORDER_COLOR 0xFF00FF00

GridBackgroundAppearance::GridBackgroundAppearance(const GridBackgroundAppearance& appearance)
{
	GridBackgroundAppearance::CopyFrom(appearance);
}

GridBackgroundAppearance::GridBackgroundAppearance()
:Appearance()
, IsDrawThickGrid(DEFAULT_SHOW_THICK_GRID)
, IsDrawThinGrid(DEFAULT_SHOW_THIN_GRID)
, ThickGridLineWidth(DEFAULT_GRID_THICK_LINE_WIDTH)
, ThinGridLineWidth(DEFAULT_GRID_THIN_LINE_WIDTH)
, ThickGridLineStyle(DEFAULT_GRID_THICK_LINE_STYLE)
, ThinGridLineStyle(DEFAULT_GRID_THIN_LINE_STYLE)
, ThickGridSpacing(DEFAULT_GRID_THICK_SPACING)
, ThinGridSpacing(DEFAULT_GRID_THIN_SPACING)
, ThickGridColor(DEFAULT_GRID_THICK_LINE_COLOR)
, ThinGridLineColor(DEFAULT_GRID_THIN_LINE_COLOR)
{
	FillColor=DEFAULT_GRID_FILL_COLOR;
	LineColor = DEFAULT_GRID_BORDER_COLOR;
	ShowFill=true;
	ShowBorder=true;
	ShowTrack=false;
}


GridBackgroundAppearance::~GridBackgroundAppearance()
{
}

Appearance& GridBackgroundAppearance::operator=(const Appearance& another)
{
	CopyFrom(another);
	return *this;
}

void GridBackgroundAppearance::CopyFrom(const Appearance& another)
{
	Appearance::CopyFrom(another);
	const GridBackgroundAppearance * pAnother=dynamic_cast<const GridBackgroundAppearance*>(&another);
	if(pAnother==nullptr) return;
	IsDrawThickGrid=pAnother->IsDrawThickGrid;
	IsDrawThinGrid=pAnother->IsDrawThinGrid;
	ThickGridLineWidth=pAnother->ThickGridLineWidth;
	ThinGridLineWidth=pAnother->ThinGridLineWidth;
	ThickGridLineStyle=pAnother->ThickGridLineStyle;
	ThinGridLineStyle=pAnother->ThinGridLineStyle;
	ThickGridSpacing=pAnother->ThickGridSpacing;
	ThinGridSpacing=pAnother->ThinGridSpacing;
	ThickGridColor=pAnother->ThickGridColor;
	ThinGridLineColor=pAnother->ThinGridLineColor;
}

Appearance* GridBackgroundAppearance::Clone()
{
	return new GridBackgroundAppearance(*this);
}
