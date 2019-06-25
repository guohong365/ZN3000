#include "StdAfx.h"
#include <DrawObject\tyrefrontselector.h>
using namespace Gdiplus;
IMPLEMENT_SERIAL(CTyreFrontSelector, CDrawObject, 0)

CTyreFrontSelector::CTyreFrontSelector(void)
{
    m_AreaHeight = 0;
    SetLineColor(Gdiplus::Color::Red);
    SetLineWidth(2);
    SetShowBorder(true);
}

CTyreFrontSelector::~CTyreFrontSelector(void)
{
}
CTyreFrontSelector::CTyreFrontSelector(const Gdiplus::Point & pt, const Gdiplus::Size & size, int areaHeight):CDrawObject(_T(""), pt, size)
{
    m_AreaHeight = areaHeight;
    SetLineColor(Gdiplus::Color::Red);
    SetLineWidth(2);
    SetShowBorder(true);
}

void CTyreFrontSelector::OnDraw(Gdiplus::Graphics & graph)
{
}
void CTyreFrontSelector::OnDrawBorder(Gdiplus::Graphics & graph)
{
    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen1(Gdiplus::Color::Yellow, 1);
    Gdiplus::Pen pen2(color, (Gdiplus::REAL) GetLineWidth());
    graph.DrawRectangle(&pen2, Gdiplus::Rect(Gdiplus::Point(0, 0), GetSize()));
    graph.DrawRectangle(&pen2, -20, (GetSize().Height - m_AreaHeight) / 2, GetSize().Width + 40, m_AreaHeight);
    graph.DrawLine(&pen1, GetSize().Width / 2, -10, GetSize().Width / 2, GetSize().Height + 10);
    graph.DrawLine(&pen1, -30, GetSize().Height / 2, GetSize().Width + 30, GetSize().Height / 2);
}

void CTyreFrontSelector::OnDrawFillObject(Gdiplus::Graphics & graph)
{

}
int CTyreFrontSelector::HitTest(const Gdiplus::Point & pt)
{
    int hit = CDrawObject::HitTest(pt);
    if (hit >= 1 && hit <= GetHandleCount())
    {
        return hit;
    }
    Gdiplus::Rect rc1(Gdiplus::Point(0, 0), GetSize());
    Gdiplus::Rect rc2(-20, (GetSize().Height - m_AreaHeight) / 2, 40 + GetSize().Width, m_AreaHeight);
    if (rc1.Contains(pt) || rc2.Contains(pt))
    {
        return GetHandleCount() + 1;
    }
    return 0;
}

Gdiplus::Point CTyreFrontSelector::GetHandle(int nHandle)
{
    Gdiplus::Point pt;
    switch (nHandle)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        pt = CDrawObject::GetHandle(nHandle);
        break;
    case 9:
        pt = Gdiplus::Point(GetSize().Width / 2, (GetSize().Height - m_AreaHeight) / 2);
        break;
    case 10:
        pt = Gdiplus::Point(GetSize().Width / 2, (GetSize().Height + m_AreaHeight) / 2);
        break;
    }
    return pt;
}

int CTyreFrontSelector::GetHandleCount()
{
    return 10;
}
void CTyreFrontSelector::MoveHandleTo(int &nHandle, Gdiplus::Point point)
{
    Gdiplus::Point offset = point - GetPosition();
    switch (nHandle)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        CDrawObject::MoveHandleTo(nHandle, point);
        m_AreaHeight = GetSize().Height / 3;
        break;
    case 9:
        if (offset.Y < 1)
        {
            offset.Y = 1;
        }
        m_AreaHeight = abs(GetSize().Height / 2 - offset.Y) * 2;
        if (point.Y > GetSize().Height / 2)
        {
            nHandle = 10;
        }
        break;
    case 10:
        if (offset.Y > GetSize().Height - 1)
        {
            offset.Y = GetSize().Height - 1;
        }
        m_AreaHeight = abs(GetSize().Height / 2 - offset.Y) * 2;
        if (point.Y < GetSize().Height / 2)
        {
            nHandle = 9;
        }
        break;
    }
}

HCURSOR CTyreFrontSelector::GetHandleCursor(int nHandle)
{
    HCURSOR hCursor;
    switch (nHandle)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        hCursor = CDrawObject::GetHandleCursor(nHandle);
        break;
    case 9:
        hCursor = CDrawObject::GetHandleCursor(2);
        break;
    case 10:
        hCursor = CDrawObject::GetHandleCursor(6);
        break;
    default:
        hCursor = CDrawObject::GetHandleCursor(9);
        break;
    }
    return hCursor;
}

void CTyreFrontSelector::Serialize(CArchive & ar)
{
    CDrawObject::Serialize(ar);
    if (ar.IsLoading())
    {
        ar >> m_AreaHeight;
    }
    else
    {
        ar << m_AreaHeight;
    }
}

void CTyreFrontSelector::SetAreaHeight(int height)
{
    m_AreaHeight = height;
}

int CTyreFrontSelector::GetAreaHeight() const 
{
    return m_AreaHeight;
}
