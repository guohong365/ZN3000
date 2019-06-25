#include "StdAfx.h"
#include "curveobject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CCurveObject, CPolyLineObject, 0)

CCurveObject::CCurveObject(void)
{
}

CCurveObject::~CCurveObject(void)
{
}
CCurveObject::CCurveObject(const CString & name, int x, int y):CPolyLineObject(name, x, y)
{

}
CCurveObject::CCurveObject(const CString & name, const Gdiplus::Point & pt):CPolyLineObject(name, pt)
{

}

void CCurveObject::OnDraw(Gdiplus::Graphics & graph)
{
    ASSERT(GetVetexCount() >= 2);
    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen(color, (float) GetLineWidth());
    Gdiplus::GraphicsPath path;
    path.AddCurve(GetVetexBuffer(), GetVetexCount());
    graph.DrawPath(&pen, &path);
}

int CCurveObject::HitTest(const Gdiplus::Point & pt)
{
    int handle = CDrawObject::HitTest(pt);
    if (handle >= 1 && handle <= GetHandleCount())
        return handle;

    Gdiplus::GraphicsPath path;
    path.AddCurve(GetVetexBuffer(), GetVetexCount());
    Gdiplus::Pen pen(Gdiplus::Color::White, 10);
    if (path.IsOutlineVisible(pt, &pen))
    {
        return GetHandleCount() + 1;
    }
    return 0;

}

void CCurveObject::GetObjectRegion( Gdiplus::GraphicsPath & region )
{
    region.Reset();
    region.AddCurve(GetVetexBuffer(), GetVetexCount());
}
