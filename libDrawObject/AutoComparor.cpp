#include "StdAfx.h"
#include <GdiPlus.h>
#include "autocomparor.h"
#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAutoComparor, CPolygonObject, 0)

CAutoComparor::CAutoComparor(void)
{
}

CAutoComparor::CAutoComparor(const Gdiplus::Point pt):CPolygonObject(_T("AutoComparor"), pt.X, pt.Y)
{
    SetLineColor(Gdiplus::Color::Yellow);
    SetLineWidth(2);
	SetFontFace(_T("Wingdings"));
	SetFontSize(16);
	SetFontStyle( Gdiplus::FontStyleRegular);
    SetFontColor(Gdiplus::Color::Yellow);
}

CAutoComparor::~CAutoComparor(void)
{
}


void CAutoComparor::DrawMark(Gdiplus::Graphics & graph)
{
    //WCHAR ch[]={ 140, 141,142 };
    WCHAR ch[] = { 129, 130, 131 };

    Gdiplus::SolidBrush brush(GetFontColor());
    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    Gdiplus::Point * pt = GetVetexBuffer();

	Gdiplus::GraphicsPath path;
	Gdiplus::FontFamily family(GetFontFace());
    for (int i = 0; i < 3; i++)
    {
		path.AddString(&ch[i], -1, &family, GetFontStyle(), GetFontSize(), Gdiplus::PointF((float) pt[i].X, (float) pt[i].Y), &stringFormat);
	}
	graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graph.FillPath(&brush, &path);
}

void CAutoComparor::OnDraw(Gdiplus::Graphics & graph)
{
    CPolygonObject::OnDraw(graph);
    DrawMark(graph);
}
