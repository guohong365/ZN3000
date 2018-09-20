#include "stdafx.h"
#include "CharSymbol.h"
#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CCharSymbol, CSymbolObject, 0)

CCharSymbol::CCharSymbol()
{

}
CCharSymbol::CCharSymbol(TCHAR symbol,const CString& fontName, const CString &name, const CString & category, Gdiplus::Size size, int type) 
:CSymbolObject(name, category, size, type)
,_fontName(fontName)
,_symbol(symbol)
{
	SetFillColor(Gdiplus::Color::Black);
	SetFontFace(fontName);
	SetFontSize((float)size.Height);
	SetFontStyle(Gdiplus::FontStyleRegular);
}

void CCharSymbol::SetChar( TCHAR ch )
{
	if(_symbol!=ch)
	{
		TCHAR s=ch;
		OnCharChanging(s);
		_symbol=s;
		OnCharChanged();
	}
}

TCHAR CCharSymbol::GetChar()
{
	return _symbol;
}

void CCharSymbol::OnCharChanging( TCHAR &ch )
{
	Notify(DNM_SYMBOL_DATA_CHANGING, 0, &ch);
}

void CCharSymbol::OnCharChanged()
{
	Notify(DNM_SYMBOL_DATA_CHANGED);
}

void CCharSymbol::OnDraw( Gdiplus::Graphics & graph )
{
	Gdiplus::GraphicsPath path;
	Gdiplus::SolidBrush brush(GetFillColor());
	Gdiplus::StringFormat strFormat;
	strFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	strFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	strFormat.SetFormatFlags(Gdiplus::StringFormatFlagsNoClip |Gdiplus::StringFormatFlagsDisplayFormatControl | Gdiplus::StringFormatFlagsNoWrap);
	Gdiplus::FontFamily family(GetFontFace());
	path.AddString(&_symbol, 1, &family, GetFontStyle(), (Gdiplus::REAL)GetSize().Height, Gdiplus::PointF(0,0), &strFormat);
	Gdiplus::RectF bounds;
	path.GetBounds(&bounds);
	Gdiplus::SizeF layout((Gdiplus::REAL)GetSize().Width, (Gdiplus::REAL)GetSize().Height);
	float ratio=GetRatio(Gdiplus::SizeF(	layout.Width*0.9f,layout.Height*0.9f),
		Gdiplus::SizeF((Gdiplus::REAL)bounds.Width, (Gdiplus::REAL)bounds.Height));
	
	Gdiplus::PointF pt;
	Gdiplus::Matrix m;
	m.Scale(ratio, ratio);
	path.Transform(&m);
	path.GetBounds(&bounds);
	m.Reset();
	m.Translate(-bounds.X, -bounds.Y);
	pt.X =(layout.Width - bounds.Width)/2;
	pt.Y =(layout.Height - bounds.Height)/2;
	m.Translate(pt.X,pt.Y);
	path.Transform(&m);
	path.GetBounds(&bounds);
    graph.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graph.FillPath(&brush, &path);
}

void CCharSymbol::Serialize( CArchive & ar )
{
	__super::Serialize(ar);
	if(ar.IsLoading())
	{
		ar >> _symbol;
		ar >> _fontName;
	}
	else
	{
		ar << _symbol;
		ar << _fontName;
	}
}

CDrawObject * CCharSymbol::CopyFrom( CDrawObject * source )
{
	__super::CopyFrom(source);
	CCharSymbol *pCharSymbol=DYNAMIC_DOWNCAST(CCharSymbol, source);
	if(pCharSymbol)
	{
		_symbol=pCharSymbol->_symbol;
		_fontName=pCharSymbol->_fontName;
	}
	else
	{
		_symbol=0;
		_fontName=_T("");
	}
	return this;
}

