#include "StdAfx.h"
#include <DrawObject\tyresideselector.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CTyreSideSelector, CDrawObject, 0)

CTyreSideSelector::CTyreSideSelector(void)
{
    m_RadioInner = 0;
}

CTyreSideSelector::~CTyreSideSelector(void)
{
}
CTyreSideSelector::CTyreSideSelector(const Gdiplus::Point & pt, const Gdiplus::Size & size, int radioInner):CDrawObject(_T(""), pt, size)
{
    m_RadioInner = radioInner;
    SetLineWidth(2);
    SetLineColor(Gdiplus::Color::Red);
}

void CTyreSideSelector::OnDraw(Gdiplus::Graphics & graph)
{

}
int CTyreSideSelector::HitTest(const Gdiplus::Point & point)
{
    int hit = CDrawObject::HitTest(point);
    if (hit >= 1 && hit <= 8)
    {
        return hit;
    }
    Gdiplus::GraphicsPath pathEllipse;
    pathEllipse.AddEllipse(0, 0, GetSize().Width, GetSize().Height);
    if (pathEllipse.IsVisible(point))
    {
        return GetHandleCount() + 1;
    }
    return 0;
}

Gdiplus::Point CTyreSideSelector::GetHandle(int nHandle)
{
    Gdiplus::Point pt;
    switch (nHandle)
    {
    case 1:
    case 3:
    case 5:
    case 7:
        pt = CDrawObject::GetHandle(nHandle);
        break;
    case 2:
        pt.X = GetSize().Width / 2;
        pt.Y = GetSize().Height / 2 - m_RadioInner;
        break;
    case 4:
        pt.X = GetSize().Width / 2 + m_RadioInner;
        pt.Y = GetSize().Height / 2;
        break;
    case 6:
        pt.X = GetSize().Width / 2;
        pt.Y = GetSize().Height / 2 + m_RadioInner;
        break;
    case 8:
        pt.X = GetSize().Width / 2 - m_RadioInner;
        pt.Y = GetSize().Height / 2;
        break;
    }
    return pt;
}

int CTyreSideSelector::GetHandleCount()
{
    return 8;
}
void CTyreSideSelector::MoveHandleTo(int &nHandle, Gdiplus::Point point)
{
    CRect rect(GetPosition().X, GetPosition().Y, GetPosition().X + GetSize().Width, GetPosition().Y + GetSize().Height);
    Gdiplus::Point pos = GetPosition();
    pos.X = GetPosition().X + GetSize().Width / 2;
    pos.Y = GetPosition().Y + GetSize().Height / 2;
    Gdiplus::Point offset = point - pos;
    int maxOffset = max(abs(offset.X), abs(offset.Y));
    int delta = abs(offset.X - offset.Y);
    switch (nHandle)
    {
    case 1:
        //rect.right - point.X=rect.bottom - point.Y;
        if (abs(rect.right - point.X) < abs(rect.bottom - point.Y))
        {
            point.Y = rect.bottom - (rect.right - point.X);
        }
        else
        {
            point.X = rect.right - (rect.bottom - point.Y);
        }
        CDrawObject::MoveHandleTo(nHandle, point);
        m_RadioInner = GetSize().Width / 3;
        break;
    case 3:
        //point.X-rect.left =rect.bottom - point.Y;
        if (abs(point.X - rect.left) < abs(rect.bottom - point.Y))
        {
            point.Y = rect.bottom - (point.X - rect.left);
        }
        else
        {
            point.X = rect.left + (rect.bottom - point.Y);
        }
        CDrawObject::MoveHandleTo(nHandle, point);
        m_RadioInner = GetSize().Width / 3;
        break;

    case 5:
        //point.X-rect.left =point.Y - rect.top ;
        if (abs(point.X - rect.left) < abs(point.Y - rect.top))
        {
            point.Y = rect.top + (point.X - rect.left);
        }
        else
        {
            point.X = rect.left + (point.Y - rect.top);
        }
        CDrawObject::MoveHandleTo(nHandle, point);
        m_RadioInner = GetSize().Width / 3;
        break;

    case 7:
        //rect.right-point.X =point.Y - rect.top ;
        if (abs(rect.right - point.X) < abs(point.Y - rect.top))
        {
            point.Y = rect.top + (rect.right - point.X);
        }
        else
        {
            point.X = rect.right - (point.Y - rect.top);
        }
        CDrawObject::MoveHandleTo(nHandle, point);
        m_RadioInner = GetSize().Width / 3;
        break;
    case 2:
        m_RadioInner = abs(offset.Y) > GetSize().Width / 2 - 2 ? GetSize().Width / 2 - 2 : abs(offset.Y);
        if (offset.Y < 0)
        {
            nHandle = 6;
        }
        break;
        break;

    case 4:
        m_RadioInner = abs(offset.X) > GetSize().Width / 2 - 2 ? GetSize().Width / 2 - 2 : abs(offset.X);
        if (offset.X < 0)
        {
            nHandle = 8;
        }
        break;
    case 6:
        m_RadioInner = abs(offset.Y) > GetSize().Width / 2 - 2 ? GetSize().Width / 2 - 2 : abs(offset.Y);
        if (offset.Y > 0)
        {
            nHandle = 2;
        }
        break;
    case 8:
        m_RadioInner = abs(offset.X) > GetSize().Width / 2 - 2 ? GetSize().Width / 2 - 2 : abs(offset.X);
        if (offset.X > 0)
        {
            nHandle = 4;
        }
        break;
    }
}
HCURSOR CTyreSideSelector::GetHandleCursor(int nHandle)
{
    return CDrawObject::GetHandleCursor(nHandle);
}

void CTyreSideSelector::OnDrawBorder(Gdiplus::Graphics & graph)
{
    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen1(Gdiplus::Color::Yellow, 1);
    Gdiplus::Pen pen2(color, (float) GetLineWidth());
    graph.DrawEllipse(&pen2, 0, 0, GetSize().Width, GetSize().Height);

    graph.DrawEllipse(&pen2, GetSize().Width / 2 - m_RadioInner, GetSize().Height / 2 - m_RadioInner, m_RadioInner + m_RadioInner,
                      m_RadioInner + m_RadioInner);

    graph.DrawLine(&pen1, GetSize().Width / 2, -10, GetSize().Width / 2, GetSize().Height + 10);
    graph.DrawLine(&pen1, -10, GetSize().Height / 2, GetSize().Width + 10, GetSize().Height / 2);
}

void CTyreSideSelector::OnDrawFillObject(Gdiplus::Graphics & graph)
{

}

void CTyreSideSelector::Serialize(CArchive & ar)
{
    CDrawObject::Serialize(ar);
    if (ar.IsLoading())
    {
        ar >> m_RadioInner;
    }
    else
    {
        ar << m_RadioInner;
    }
}

int CTyreSideSelector::GetInnerRadio() const
{
    return m_RadioInner;
}

void CTyreSideSelector::SetInnerRadio(int inner)
{
    m_RadioInner = inner;
}

void CTyreSideSelector::OnSelectedChanged()
{
    CDrawObject::OnSelectedChanged();
}
