#include "StdAfx.h"

#include "LineObject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

CLineObject::CLineObject(void)
{
}

CLineObject::~CLineObject(void)
{
}
CLineObject::CLineObject(const CString & name, const Rect & rect)
:CRectObject(name, rect)
{

}
CLineObject::CLineObject(const CString & name, int x, int y, int width, int height)
:CRectObject(name, x, y, width, height)
{

}
CLineObject::CLineObject(const CString & name, const Point & point, const Size &size )
:CRectObject(name, point, size)
{

}
void CLineObject::OnDraw(Graphics &graph)
{
	Color color=GetLineColor();
	const BYTE alpha=BYTE(GetTransparent() * 255);
	ARGB argb=color.GetValue();
	argb &= ((ARGB(alpha) << Color::AlphaShift) | 0x00FFFFFF) ;
	color=argb;
	Pen pen(color , GetLineWidth());
	pen.SetDashStyle(DashStyle(GetLineStyle()));
	graph.DrawLine(&pen, 0, 0, GetSize().Width, GetSize().Height);
}
void CLineObject::OnDrawBorder(Graphics &graph)
{

}
void CLineObject::OnDrawFillObject(Graphics &graph)
{

}

 int CLineObject::GetHandleCount()
 {
	 return 2;
 }
 
 int CLineObject::HitTest(const Point &pt)
 {
	 if (!GetVisible())
	 {
		 return 0;
	 }

	 GraphicsPath path;
	 const Point pt1(0,0);
	 const Point pt2=pt1 + GetSize();
	 path.AddLine(pt1, pt2);
	 Pen pen(Color::White, 10);
	 if(path.IsOutlineVisible(pt, &pen))
	 {
		 return GetHandleCount() + 1;
	 }
	 return 0;
}
