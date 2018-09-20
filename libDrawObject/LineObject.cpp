#include "StdAfx.h"

#include "LineObject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CLineObject, CRectObject, 0)

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
	BYTE alpha=(BYTE)(GetTransparent() * 255);
	ARGB argb=color.GetValue();
	argb &= (((ARGB)(alpha) << Color::AlphaShift) | 0x00FFFFFF) ;
	color=argb;
	Pen pen(color , GetLineWidth());
	pen.SetDashStyle((DashStyle)GetLineStyle());
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
 Point CLineObject::GetHandle(int nHandle)
 {
	 if(nHandle==2)
	 {
		 nHandle=5;
	 }
	 return CDrawObject::GetHandle(nHandle);
 }

 HCURSOR CLineObject::GetHandleCursor(int nHandle)
 {
	 LPCTSTR id;
	 if(!GetSizable())
	 {
		 id= IDC_ARROW;
	 }
	 else
	 {
		 switch (nHandle)
		 {
		 default:
			 id=IDC_ARROW;
			 break;
		 case 1:
		 case 2:
			 id = IDC_SIZENWSE;
			 break;

		 case 3:
			 id = IDC_SIZEALL;
			 break;
		 }
	 }

	 return AfxGetApp()->LoadStandardCursor(id);
 }
 //void CLineObject::MoveHandleTo(int nHandle, Gdiplus::Point point)
 //{
	// if(nHandle==2)
	// {
	//	 nHandle=5;
	// }
	// 
	// return CDrawObject::MoveHandleTo(nHandle, point);
 //}
 void CLineObject::MoveHandleTo(int nHandle, Point point)
 {
	 if(nHandle==2)
	 {
		 nHandle=5;
	 }
	 if(GetSizable())
	 {
		 Size newSize=GetSize();
		 Point newPos=GetPosition();
		 CRect rect(
			 newPos.X, 
			 newPos.Y,
			 newPos.X + newSize.Width, 
			 newPos.Y + newSize.Height);
		 switch(nHandle)
		 {
		 case 1:
			 rect.left=point.X;
			 rect.top=point.Y;
			 break;
		 case 5:
			 rect.right=point.X;
			 rect.bottom=point.Y;
			 break;
		 case 2:
			 rect.top=point.Y;
			 break;
		 case 6:
			 rect.bottom=point.Y;
			 break;
		 case 3:
			 rect.right=point.X;
			 rect.top=point.Y;
			 break;
		 case 7:
			 rect.left=point.X;
			 rect.bottom=point.Y;
			 break;
		 case 4:
			 rect.right=point.X;
			 break;
		 case 8:
			 rect.left=point.X;
			 break;
		 }
		 rect.NormalizeRect();
		 newSize.Width=rect.Width();
		 newSize.Height=rect.Height();
		 newPos.X=rect.left;
		 newPos.Y=rect.top;
		 SetPosition(newPos);
		 SetSize(newSize);
	 }
 }

 int CLineObject::HitTest(const Point &pt)
 {
	 if (GetVisible() == false && GetSelectable() == false)
	 {
		 return 0;
	 }
	 int nHandleCount = GetHandleCount();
	 if (GetSelected())
	 {
		 for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		 {
			 // GetHandleRect returns in logical coords
			 Rect rc = GetHandleRect(nHandle);
			 if (rc.Contains(pt))
			 {
				 return nHandle;
			 }
		 }
	 }	

	 GraphicsPath path;
	 Point pt1(0,0);
	 Point pt2=pt1 + GetSize();
	 path.AddLine(pt1, pt2);
	 Pen pen(Color::White, 10);
	 if(path.IsOutlineVisible(pt, &pen))
	 {
		 return GetHandleCount() + 1;
	 }
	 return 0;
}
