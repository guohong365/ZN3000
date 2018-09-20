#include "StdAfx.h"
#include "polygonobject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CPolygonObject, CPolyLineObject, 0)

CPolygonObject::CPolygonObject(void)
{
}

CPolygonObject::~CPolygonObject(void)
{
}
CPolygonObject::CPolygonObject(const CString & name, int x, int y):CPolyLineObject(name, x, y)
{

}
CPolygonObject::CPolygonObject(const CString & name, const Gdiplus::Point & pt):CPolyLineObject(name, pt)
{

}

void CPolygonObject::OnDrawBorder(Gdiplus::Graphics & graph)
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
	pen.SetDashStyle((DashStyle)GetLineStyle());
    Gdiplus::GraphicsPath path;
    path.AddPolygon(GetVetexBuffer(), GetVetexCount());
    graph.DrawPath(&pen, &path);
}

void CPolygonObject::OnDrawFillObject(Gdiplus::Graphics & graph)
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
    path.AddPolygon(GetVetexBuffer(), GetVetexCount());
    graph.FillPath(&brush, &path);
}

void CPolygonObject::OnDraw(Gdiplus::Graphics & graph)
{
    if (GetVetexCount() <= 2)
    {
        CPolyLineObject::OnDraw(graph);
    }
    else
    {
        CDrawObject::OnDraw(graph);
    }
}
int CPolygonObject::HitTest(const Gdiplus::Point & pt)
{
    if (GetVetexCount() <= 2)
    {
        return CPolyLineObject::HitTest(pt);
    }
    int hit = CPolyLineObject::HitTest(pt);
    if (hit > 0 && hit <= GetHandleCount())
    {
        return hit;
    }
    Gdiplus::GraphicsPath path;
    path.AddPolygon(GetVetexBuffer(), GetVetexCount());
    if (path.IsVisible(pt))
    {
        return GetHandleCount() + 1;
    }
    return 0;
}

void CPolygonObject::GetObjectRegion( Gdiplus::GraphicsPath & region )
{
    region.Reset();
    region.AddPolygon(GetVetexBuffer(),GetVetexCount());
}
