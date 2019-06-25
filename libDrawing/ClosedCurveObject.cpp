#include "StdAfx.h"
#include "closedcurveobject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CClosedCurveObject, CCurveObject, 0) 

CClosedCurveObject::CClosedCurveObject(void)
{
}

CClosedCurveObject::~CClosedCurveObject(void)
{
}

CClosedCurveObject::CClosedCurveObject(const CString & name, int x, int y):CCurveObject(name, x, y)
{

}
CClosedCurveObject::CClosedCurveObject(const CString & name, const Gdiplus::Point & pt):CCurveObject(name, pt)
{

}

void CClosedCurveObject::OnDrawBorder(Gdiplus::Graphics & graph)
{
    if (GetVetexCount() <= 2)
    {
        return;
    }
    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen(color, (float) GetLineWidth());
    Gdiplus::GraphicsPath path;
    path.AddClosedCurve(GetVetexBuffer(), GetVetexCount());
    graph.DrawPath(&pen, &path);
}

void CClosedCurveObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{
    if (GetVetexCount() <= 2)
    {
        return;
    }
    Gdiplus::Color color = GetFillColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::SolidBrush brush(color);
    Gdiplus::GraphicsPath path;
    path.AddClosedCurve(GetVetexBuffer(), GetVetexCount());
    graph.FillPath(&brush, &path);
}

void CClosedCurveObject::OnDraw(Gdiplus::Graphics & graph)
{
    if (GetVetexCount() <= 2)
    {
        CCurveObject::OnDraw(graph);
    }
    else
    {
        CDrawObject::OnDraw(graph);
    }
}

int CClosedCurveObject::HitTest(const Gdiplus::Point & pt)
{
    if (GetVetexCount() <= 2)
    {
        return CCurveObject::HitTest(pt);
    }
    int hit = CPolyLineObject::HitTest(pt);
    if (hit > 0 && hit <= GetHandleCount())
    {
        return hit;
    }
    Gdiplus::GraphicsPath path;
    path.AddClosedCurve(GetVetexBuffer(), GetVetexCount());
    if (path.IsVisible(pt))
    {
        return GetHandleCount() + 1;
    }
    return 0;
}

void CClosedCurveObject::GetObjectRegion( Gdiplus::GraphicsPath & region )
{
    region.Reset();
    region.AddClosedCurve(GetVetexBuffer(), GetVetexCount());
}
