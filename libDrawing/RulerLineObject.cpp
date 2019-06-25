#include "StdAfx.h"
#include <math.h>
#include "rulerlineobject.h"
#include "PolyLineObject.h"
#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CRulerLineObject, CPolyLineObject, 0) 

CRulerLineObject::CRulerLineObject(void)
{
    SetLineColor(Gdiplus::Color::Yellow);
}

CRulerLineObject::~CRulerLineObject(void)
{
}

CRulerLineObject::CRulerLineObject(const CString & name, int x, int y):CPolyLineObject(name, x, y)
{
    SetLineColor(Gdiplus::Color::Yellow);

}
CRulerLineObject::CRulerLineObject(const CString & name, const Gdiplus::Point & point):CPolyLineObject(name, point)
{
    SetLineColor(Gdiplus::Color::Yellow);
}

void CRulerLineObject::OnDraw(Gdiplus::Graphics & graph)
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

}

void CRulerLineObject::OnDrawTracker(Gdiplus::Graphics & graph)
{
}

double C3DCorrectorObject::m_Ratio=1.0;

IMPLEMENT_SERIAL(C3DCorrectorObject, CPolygonObject, 0)
C3DCorrectorObject::C3DCorrectorObject()
{
    SetLineWidth(2);
}

C3DCorrectorObject::C3DCorrectorObject(int x, int y):CPolygonObject(_T(""), x, y)
{
    SetLineWidth(2);
}

C3DCorrectorObject::C3DCorrectorObject(const Gdiplus::Point & pt):CPolygonObject(_T(""), pt)
{
    SetLineWidth(2);
}

C3DCorrectorObject::~C3DCorrectorObject()
{

}

void C3DCorrectorObject::DoCalculate()
{
    int i;
    double angle[4];
    Gdiplus::Point pt[4];
    Gdiplus::Point vet[4];
    GetVertexes(pt, 4);
    for (i = 0; i < 4; i++)
    {
        angle[i] = CMathUtility::CalculateLineAngle(pt[i].X, pt[i].Y, pt[(i + 1) % 4].X, pt[(i + 1) % 4].Y);
        while (angle[i] < 0)
        {
            angle[i] += 360;
        }
    }
    int baseIndex = 0;
    double base = angle[0];
    double l;
    for (i = 1; i < 4; i++)
    {
        if (angle[i] < base)
        {
            base = angle[i];
            baseIndex = i;
        }
    }
    Local2World(pt, 4);

    double X0[4] = { 0, 500, 500, 0 };
    double Y0[4] = { 0, 0, 500, 500 };
    double X1[4] = { 0, 298, 439, 173 };
    double Y1[4] = { 0, 116, -134, -194 };

    double K[8];
    for(i=0;i<4; i ++)
    {
          X1[i]=pt[i].X;
          Y1[i]=pt[i].Y;
    }

    bool ok = CMathUtility::CalculateCorrectParameter(X0, Y0, X1, Y1, K);

    double X = 43.78;
    double Y = -215;

    double ax = (K[0] * X + K[1] * Y + K[2]) / (K[6] * X + K[7] * Y + 1);
    double ay = (K[3] * X + K[4] * Y + K[5]) / (K[6] * X + K[7] * Y + 1);

    l = (pt[baseIndex].X - pt[(baseIndex + 1) % 4].X) * (pt[baseIndex].X - pt[(baseIndex + 1) % 4].X) +
        (pt[baseIndex].Y - pt[(baseIndex + 1) % 4].Y) * (pt[baseIndex].Y - pt[(baseIndex + 1) % 4].Y);
}

double *C3DCorrectorObject::GetParam()
{
    return m_CorrectParem;
}

void C3DCorrectorObject::SetOrigion(double x, double y)
{
    m_OrigionX = x;
    m_OrigionY = y;
}

void C3DCorrectorObject::SetBaselineAngle(double angle)
{
    m_BaselineAngle = angle;
}

bool C3DCorrectorObject::OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point)
{
	return false;
}

void C3DCorrectorObject::OnDrawBorder(Gdiplus::Graphics & graph)
{
    if (GetVetexCount() <= 2)
    {
        return;
    }
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::Pen pen1(Gdiplus::Color::Green, (float) GetLineWidth());
    Gdiplus::Pen pen2(Gdiplus::Color::Yellow, (float) GetLineWidth());
    double angle;
    double len;
    Gdiplus::Matrix oldMatrix;
    Gdiplus::Matrix matrix;
    graph.GetTransform(&oldMatrix);

    Gdiplus::Point pt[4];
    int count=GetVetexCount();
    for(int i=0; i< count ; i++)
    {
        GetVertexes(pt, count);
        angle = CMathUtility::CalculateLineAngle(GetVetexBuffer()[i].X, GetVetexBuffer()[i].Y, 
            GetVetexBuffer()[(i+1) % count].X, GetVetexBuffer()[(i + 1) % count].Y);
        len=sqrt((double)(pt[i].X -pt[(i+1)%count].X)*(pt[i].X -pt[(i+1)%count].X)+(pt[i].Y -pt[(i+1)%count].Y)*(pt[i].Y -pt[(i+1)%count].Y));

        graph.TranslateTransform((float) pt[i].X, (float) pt[i].Y);
        graph.RotateTransform((float) angle);
		graph.DrawLine(i%2==0? & pen1:&pen2, -50.f, 0.f, (float)(len + 50), 0.f);
        graph.SetTransform(&oldMatrix);
    }
}

//void C3DCorrectorObject::OnDrawTracker(Gdiplus::Graphics & graph)
//{
//
//}
