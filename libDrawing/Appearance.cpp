#include "stdafx.h"
#include <uc/drawing/Appearance.h>
#include <uc/drawing/DrawObjectGlobal.h>
namespace uc
{
    namespace drawing
    {

        Appearance::Appearance( const Appearance& another )
        {
            Appearance::CopyFrom(another);
        }

        Appearance& Appearance::operator=(const Appearance& another)
        {
            CopyFrom(another);
            return *this;
        }

        void Appearance::CopyFrom(const Appearance& another)
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

        Appearance* Appearance::Clone()
        {
            Appearance * ret=new Appearance(*this);
            return ret;
        }

        Appearance::Appearance()
        {
            LineStyle=DEFAULT_LINE_STYLE;
            LineWidth=DEFAULT_LINE_WIDTH;
            LineColor=DEFAULT_LINE_COLOR;
            FillColor=DEFAULT_FILL_COLOR;
            FillStyle=DEFAULT_FILL_STYLE;
            FontName=DEFAULT_FONT_NAME;
            FontSize=DEFAULT_FONT_SIZE;
            FontStyle=DEFAULT_FONT_STYLE;
            FontColor=DEFAULT_TEXT_COLOR;
            ActiveColor=DEFAULT_ACTIVE_COLOR;
            TextAlignment=Gdiplus::StringAlignmentCenter;
            LineAlignment=Gdiplus::StringAlignmentCenter;
            ShowBorder=false;
            ShowFill=false;
            ShowTrack=true;
            ShowActive=false;
            VerticalText=false;
            Transparent=DEFAULT_TRANSPARENT;
            TraceColor=DEFAULT_TRACK_COLOR;
        }

    }
}