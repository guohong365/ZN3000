// Layout.cpp : 实现文件
//
#include "stdafx.h"
#include <uc/drawing/RectObject.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;
namespace uc
{
    namespace drawing{

        RectObject::RectObject()
        {

        }
        RectObject::RectObject(const CString & name, int x, int y, int width, int height):DrawObject(name, x, y, width, height)
        {

        }
        RectObject::RectObject(const CString & name, const Point & point, const Size & size):DrawObject(name, point, size)
        {

        }
        RectObject::RectObject(const CString & name, const Rect & rect):DrawObject(name, rect)
        {

        }

        void RectObject::OnDraw(Graphics & graph)
        {
            DrawObject::OnDraw(graph);
        }

        void RectObject::OnDrawBorder(Graphics & graph)
        {
            Pen pen(GetActive()? GetActiveColor() : GetLineColor(),GetLineWidth());
            pen.SetDashStyle(static_cast<DashStyle>(GetLineStyle()));
            graph.DrawRectangle(&pen, 0, 0, GetSize().Width, GetSize().Height);
        }

        void RectObject::OnDrawFillObject(Graphics & graph)
        {
            Color color = GetFillColor();
            const BYTE alpha = static_cast<BYTE>(GetTransparent() * 255);
            ARGB argb = color.GetValue();
            argb &= ((static_cast<ARGB>(alpha) << Color::AlphaShift) | 0x00FFFFFF);
            color = argb;
            SolidBrush brush(color);
            graph.FillRectangle(&brush, 0, 0, GetSize().Width, GetSize().Height);
        }
    }
}