#include "stdafx.h"
#include "PrespectiveRulerObject.h"
#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CPrespectiveRulerObject, CPolyLineObject, 0)
CPrespectiveRulerObject::CPrespectiveRulerObject()
{
}

CPrespectiveRulerObject::CPrespectiveRulerObject( const CString & name, int x, int y ) :CPolyLineObject(name, x, y)
{

}

CPrespectiveRulerObject::CPrespectiveRulerObject( const CString & name, const Gdiplus::Point & pt ) :CPolyLineObject(name, pt)
{

}

CPrespectiveRulerObject::~CPrespectiveRulerObject()
{

}

void CPrespectiveRulerObject::SetCoordMapParameter( const double * mapParam, const double * reversMapParam )
{
    m_pMapParameter=mapParam;
    m_pReversMapParameter=reversMapParam;
}

void CPrespectiveRulerObject::OnDraw( Gdiplus::Graphics &graph )
{
    CPolyLineObject::OnDraw(graph);

    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen(color, (float) GetLineWidth());

    double angle = CMathUtility::CalculateLineAngle(GetVetexBuffer()[0].X, GetVetexBuffer()[0].Y, GetVetexBuffer()[1].X, GetVetexBuffer()[1].Y);

    Gdiplus::Matrix matrix;
    graph.GetTransform(&matrix);
    Gdiplus::Point pt[2];
    GetVertexes(pt, 2);
    graph.TranslateTransform((float) pt[0].X, (float) pt[0].Y);
    graph.RotateTransform((float) angle);
    graph.DrawLine(&pen, 0, -10, 0, 10);
    graph.SetTransform(&matrix);

    graph.TranslateTransform((float) pt[1].X, (float) pt[1].Y);
    graph.RotateTransform((float) angle);
    graph.DrawLine(&pen, 0, -10, 0, 10);
    graph.SetTransform(&matrix);

    GetVertexes(pt, 2);
    Local2Global(pt,2);
    double x1, y1;
    double x2, y2;
    CMathUtility::CalculatePrespectiveCoordinate(m_pMapParameter, pt[0].X, pt[0].Y, x1, y1);
    CMathUtility::CalculatePrespectiveCoordinate(m_pMapParameter, pt[1].X, pt[1].Y, x2, y2);

    double len= sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

    GetVertexes(pt, 2);

    CString text;
    text.Format(_T("%.1f mm"), len);

    Gdiplus::Font myFont(L"Arial", 8, Gdiplus::FontStyleBold);
    Gdiplus::PointF origin((float)pt[1].X, (float)pt[1].Y);
    Gdiplus::SolidBrush blackBrush(GetLineColor());

    // Draw string.
    graph.DrawString(text, -1, &myFont,  origin,   &blackBrush);

}