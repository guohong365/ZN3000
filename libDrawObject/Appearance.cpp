#include "stdafx.h"

#include "Appearance.h"
#include "DrawObjectGlobal.h"

Appearance::Appearance( const Appearance& another )
{
	LineStyle=another.LineStyle;
	LineWidth=another.LineWidth;
	FillColor=another.FillColor;
	FillStyle=another.FillStyle;
	LineColor=another.LineColor;
	FontName=another.FontName;
	FontSize=another.FontSize;
	FontStyle=another.FontStyle;
	FontColor=another.FontColor;
	TextAlignment=another.TextAlignment;
	LineAlignment=another.LineAlignment;
	ShowActive=another.ShowActive;
	ShowBorder=another.ShowBorder;
	ShowFill=another.ShowFill;
	ShowTrack=another.ShowTrack;
	VerticalText=another.VerticalText;
	ActiveColor=another.ActiveColor;
	Transparent=another.Transparent;
}

Appearance::Appearance()
{
	LineStyle=DEFAULT_LINE_STYLE;
	LineWidth=DEFAULT_LINE_WIDTH;
	LineColor.SetFromCOLORREF(DEFAULT_LINE_COLOR);
	FillColor.SetFromCOLORREF(DEFAULT_FILL_COLOR);
	FontName=DEFAULT_FONT_NAME;
	FontSize=DEFAULT_FONT_SIZE;
	FontStyle=DEFAULT_FONT_STYLE;
	FontColor.SetFromCOLORREF(DEFAULT_TEXT_COLOR);
	ActiveColor.SetFromCOLORREF(DEFAULT_ACTIVE_COLOR);
	TextAlignment=Gdiplus::StringAlignmentCenter;
	LineAlignment=Gdiplus::StringAlignmentCenter;
	ShowBorder=false;
	ShowFill=false;
	ShowTrack=true;
	ShowActive=false;
	VerticalText=false;
	Transparent=DEFAULT_TRANSPARENT;
	TraceColor.SetFromCOLORREF(DEFAULT_TRACK_COLOR);
}

