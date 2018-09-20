// Layout.cpp : 实现文件
//
#include "stdafx.h"
#include "RectObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CRectObject, CDrawObject, 0)

CRectObject::CRectObject()
{

}
CRectObject::CRectObject(const CString & name, int x, int y, int width, int height):CDrawObject(name, x, y, width, height)
{

}
CRectObject::CRectObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size):CDrawObject(name, point, size)
{

}
CRectObject::CRectObject(const CString & name, const Gdiplus::Rect & rect):CDrawObject(name, rect)
{

}

void CRectObject::OnDraw(Gdiplus::Graphics & graph)
{
    CDrawObject::OnDraw(graph);
}

void CRectObject::OnDrawBorder(Gdiplus::Graphics & graph)
{
    Gdiplus::Pen pen(GetActive()? GetActiveColor() : GetLineColor(),GetLineWidth());
	pen.SetDashStyle((DashStyle)GetLineStyle());
    graph.DrawRectangle(&pen, 0, 0, GetSize().Width, GetSize().Height);
}

void CRectObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{
    Gdiplus::Color color = GetFillColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::SolidBrush brush(color);
    graph.FillRectangle(&brush, 0, 0, GetSize().Width, GetSize().Height);
}
