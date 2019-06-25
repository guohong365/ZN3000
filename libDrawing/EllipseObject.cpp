#include "StdAfx.h"
#include "ellipseobject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CEllipseObject, CRectObject, 0) 

CEllipseObject::CEllipseObject(void)
{
}

CEllipseObject::~CEllipseObject(void)
{
}

CEllipseObject::CEllipseObject(const CString & name, const Gdiplus::Rect & rect):CRectObject(name, rect)
{

}
CEllipseObject::CEllipseObject(const CString & name, int x, int y, int width, int height):CRectObject(name, x, y, width, height)
{

}
CEllipseObject::CEllipseObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size):CRectObject(name, point, size)
{

}

void CEllipseObject::OnDrawBorder(Gdiplus::Graphics & graph)
{
    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen(color, (float) GetLineWidth());
	pen.SetDashStyle((Gdiplus::DashStyle)GetLineStyle());
    graph.DrawEllipse(&pen, 0, 0, GetSize().Width, GetSize().Height);
}

void CEllipseObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{
    Gdiplus::Color color = GetFillColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::SolidBrush brush(color);
    graph.FillEllipse(&brush, 0, 0, GetSize().Width, GetSize().Height);
}

int CEllipseObject::HitTest(const Gdiplus::Point & pt)
{
    int hit = CDrawObject::HitTest(pt);
    if (hit <= GetHandleCount())
    {
        return hit;
    }
    Gdiplus::GraphicsPath pathEllipse;
    pathEllipse.AddEllipse(0, 0, GetSize().Width, GetSize().Height);
    if (pathEllipse.IsVisible(pt))
    {
        return GetHandleCount() + 1;
    }
    return 0;
}

void CEllipseObject::GetObjectRegion( Gdiplus::GraphicsPath & region )
{
	region.Reset();
	region.AddEllipse(GetObjectRect());
}
