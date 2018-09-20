#include "StdAfx.h"
#include "TextObject.h"

using namespace Gdiplus;

IMPLEMENT_SERIAL(CTextObject, CRectObject, 0)
static const WCHAR *defaultFontName=_T("宋体");
static const float  defaultFontSize=49.4f;  //四号字
static const FontStyle defaultFontStyle=FontStyleRegular;
LPTEXT_EDIT_CALLBACK CTextObject::_pTextEditCallback=NULL;
CTextObject::CTextObject(void)
{
    initialize();
}

CTextObject::~CTextObject(void)
{
}

CTextObject::CTextObject(const CString & name, int x, int y, 
						 const CString & fontName,
						 float fontSize, 
						 int fontStyle)
						 :CRectObject(name, x, y, 1, 1)
{
    initialize();
	CDrawObject::SetFontFace(fontName);
	CDrawObject::SetFontSize(fontSize);
	CDrawObject::SetFontStyle(fontStyle);
	Gdiplus::Font font(fontName, CDrawObject::GetFontSize(), fontStyle, UnitPixel);

	RectF rectF=MeasureString(name, &font, PointF(float(x),float(y)), m_pStringFormat);
	SetSize(Size((int)(rectF.Width+0.5f), (int)(rectF.Height +0.5f)));
}
CTextObject::CTextObject(const CString & name, const Gdiplus::Point & point, 
						 const CString & fontName,
						 float fontSize, 
						 int fontStyle)
:CRectObject(name, point.X, point.Y, 1, 1)
{
    initialize();
	CDrawObject::SetFontFace(fontName);
	CDrawObject::SetFontSize(fontSize);
	CDrawObject::SetFontStyle(fontStyle);
	Gdiplus::Font font(fontName, CDrawObject::GetFontSize(), fontStyle, UnitPixel);
	RectF rectF=MeasureString(name, &font, PointF(float(point.X),float(point.Y)), m_pStringFormat);
	CDrawObject::SetSize(Size(int(rectF.Width + 0.5f), int(rectF.Height + 0.5f)));
}

void CTextObject::OnDrawBorder(Gdiplus::Graphics & graph)
{
    if (GetName().IsEmpty())
        return;
    Gdiplus::RectF rectF(0.f,0.f,(float)GetSize().Width,(float)GetSize().Height);

    Gdiplus::Color color = GetLineColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;

    Gdiplus::Pen pen(color, (float) GetLineWidth());
    Gdiplus::GraphicsPath path;
    Gdiplus::FontFamily family(GetFontFace());
	if(m_pStringFormat)
	{
		m_pStringFormat->SetAlignment((StringAlignment)GetTextAlign());
		m_pStringFormat->SetLineAlignment((StringAlignment)GetTextLineAlign());
		if(GetTextIsVert())
		{
			m_pStringFormat->SetFormatFlags(m_pStringFormat->GetFormatFlags()|StringFormatFlagsDirectionVertical);
		}
		else
		{
			m_pStringFormat->SetFormatFlags(m_pStringFormat->GetFormatFlags()&(~StringFormatFlagsDirectionVertical));
		}
	} 
	path.AddString(GetName(), -1, &family, GetFontStyle(), GetFontSize(), rectF, m_pStringFormat);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.DrawPath(&pen, &path);
}

void CTextObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{
    if (GetName().IsEmpty() )
        return;

    Gdiplus::RectF rectF(0.f,0.f, (float)GetSize().Width,(float)GetSize().Height);

    Gdiplus::Color color = GetFontColor();
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;

    Gdiplus::SolidBrush brush(color);
    Gdiplus::GraphicsPath path;
    Gdiplus::FontFamily family(GetFontFace());
	if(m_pStringFormat)
	{
		m_pStringFormat->SetAlignment((StringAlignment)GetTextAlign());
		m_pStringFormat->SetLineAlignment((StringAlignment)GetTextLineAlign());
		if(GetTextIsVert())
		{
			m_pStringFormat->SetFormatFlags(m_pStringFormat->GetFormatFlags()|StringFormatFlagsDirectionVertical);
		}
		else
		{
			m_pStringFormat->SetFormatFlags(m_pStringFormat->GetFormatFlags()&(~StringFormatFlagsDirectionVertical));
		}	
	}
	path.AddString(GetName(), -1, &family, GetFontStyle(), GetFontSize(), rectF, m_pStringFormat);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.FillPath(&brush, &path);
}

//复制对象
//@param source 复制source对象当前数据到本对象，不复制撤销列表。
CDrawObject *CTextObject::CopyFrom(CDrawObject * source)
{
    CRectObject::CopyFrom(source);
    CTextObject *pTextObject = DYNAMIC_DOWNCAST(CTextObject, source);
    ASSERT(pTextObject);
	ASSERT(m_pStringFormat);
	m_pStringFormat=pTextObject->m_pStringFormat->Clone();
    return this;
}

void CTextObject::Serialize(CArchive & ar)
{
    CRectObject::Serialize(ar);
}

void CTextObject::OnNameChanged()
{
    CRectObject::OnNameChanged();
	
	Gdiplus::Font font(GetFontFace(), GetFontSize(), GetFontStyle(), UnitPixel);

	RectF rectF=MeasureString(GetName(), &font, PointF(0,0), m_pStringFormat);
	SetSize(Size((int)(rectF.Width+0.5f), (int)(rectF.Height +0.5f)));
}

void CTextObject::initialize()
{
	m_pStringFormat=new Gdiplus::StringFormat();
    m_pStringFormat->SetAlignment(StringAlignmentCenter);
    m_pStringFormat->SetLineAlignment(StringAlignmentCenter);
    SetShowBorder(false);
	SetFilled(true);
	SetFillColor(Color::Black);
}

void CTextObject::OnDrawTracker(Gdiplus::Graphics & graph)
{
	Rect rect=GetObjectRect();
	Pen pen(GetActiveColor());
	pen.SetDashStyle(DashStyleDashDot);
	graph.DrawRectangle(&pen, rect);
	__super::OnDrawTracker(graph);
}

RectF CTextObject::GetTextExtent()
{
	Gdiplus::Font font(GetFontFace(), GetFontSize(), GetFontStyle(), UnitPixel);

	return MeasureString(GetName(), &font, PointF(0,0), m_pStringFormat);
}

Gdiplus::StringFormat * CTextObject::GetStringFormat()
{
	return m_pStringFormat;
}

bool CTextObject::OnLButtonDblClk( CWnd *pWnd, UINT nFlags, Point point )
{
	if(_pTextEditCallback)
	{
		CString text=GetName();
		if(_pTextEditCallback(this, pWnd, text))
		{
			SetName(text);
			CView *pView=DYNAMIC_DOWNCAST(CView, pWnd);
			if(pView && pView->GetDocument())
			{
				pView->GetDocument()->SetModifiedFlag();
			}
			pWnd->Invalidate(TRUE);
		}
		return true;
	}
	return false;
}

void CTextObject::SetEditCallBack( LPTEXT_EDIT_CALLBACK pCallback )
{
    _pTextEditCallback=pCallback;
}

LPTEXT_EDIT_CALLBACK CTextObject::GetEditCallback()
{
    return _pTextEditCallback;
}


