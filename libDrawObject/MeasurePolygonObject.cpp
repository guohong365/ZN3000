#include "StdAfx.h"
#include "measurepolygonobject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

CMeasurePolygonObject::~CMeasurePolygonObject(void)
{
}

CMeasurePolygonObject::CMeasurePolygonObject( const CString & name, int x, int y ) :CPolygonObject(name, x, y)
{

}

CMeasurePolygonObject::CMeasurePolygonObject( const CString & name, const Gdiplus::Point & pt ) :CPolygonObject(name, pt)
{

}

void CMeasurePolygonObject::OnDrawBorder( Gdiplus::Graphics & graph )
{
    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen penBlue(Gdiplus::Color::Blue, (float) GetLineWidth());
    Gdiplus::Pen penGreen(Gdiplus::Color::Green, (float)GetLineWidth());

    Gdiplus::GraphicsPath path;
    graph.DrawLine(&penGreen, GetVetexBuffer()[0], GetVetexBuffer()[1]);
    graph.DrawLine(&penBlue, GetVetexBuffer()[1], GetVetexBuffer()[2]);
    graph.DrawLine(&penGreen, GetVetexBuffer()[2], GetVetexBuffer()[3]);
    graph.DrawLine(&penBlue, GetVetexBuffer()[3], GetVetexBuffer()[0]);
}

void CMeasurePolygonObject::OnDrawFillObject( Gdiplus::Graphics & graph )
{

}