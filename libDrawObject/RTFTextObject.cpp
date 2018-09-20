#include "StdAfx.h"
#include "rtftextobject.h"
#include "UIHelper.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CRTFTextObject, CRectObject, 0)
CRTFTextObject::CRTFTextObject(void)
{
	Initialize();
}

CRTFTextObject::CRTFTextObject( ITextHost *pHost )
{
	SetTextHost(pHost);
}

CRTFTextObject::~CRTFTextObject(void)
{
	if(_pTextHost)
	{
		_pTextHost->Release();
	}
	delete _pRichTextRender;
}

void CRTFTextObject::OnDrawBorder( Gdiplus::Graphics & graph )
{
	Pen pen(GetLineColor(), GetLineWidth());
	pen.SetDashStyle(DashStyleDash);
	graph.DrawRectangle(&pen, GetObjectRect());
}

void CRTFTextObject::SetRichText( LPCTSTR text )
{
	_rtfText=text;
	_pRichTextRender->SetText(text);
}

const CString & CRTFTextObject::GetRichText()
{
	return _rtfText;
}

void CRTFTextObject::OnDrawFillObject( Gdiplus::Graphics & graph )
{
	SolidBrush brush(GetFillColor());
	graph.FillRectangle(&brush, GetBounds());
}

void CRTFTextObject::OnDraw( Gdiplus::Graphics & graph )
{
	__super::OnDraw(graph);
	REAL m[6];
	Matrix matrix;
	XFORM x;
	XFORM oldForm;
	graph.GetTransform(&matrix);
	matrix.GetElements(m);
	HDC hdc=graph.GetHDC();
	x.eM11=m[0];
	x.eM12=m[1];
	x.eM21=m[2];
	x.eM22=m[3];
	x.eDx=m[4];
	x.eDy=m[5];
	int ret=::SetGraphicsMode(hdc, GM_ADVANCED);
	GetWorldTransform(hdc, &oldForm);
	BOOL ok=::SetWorldTransform(hdc, &x);//, MWT_RIGHTMULTIPLY);
	CRect rc(0,0, GetSize().Width, GetSize().Height);
	_pRichTextRender->DrawText(hdc, &rc);
	SetWorldTransform(hdc, &oldForm);
	graph.ReleaseHDC(hdc);
}

CDrawObject * CRTFTextObject::CopyFrom( CDrawObject * source )
{
	__super::CopyFrom(source);
	if(source->IsKindOf(RUNTIME_CLASS(CRTFTextObject)))
	{
		CRTFTextObject *pro=(CRTFTextObject*)source;
		_rtfText=pro->_rtfText;
		memcpy(&_charFormat, &pro->_charFormat, sizeof(pro->_charFormat));
		memcpy(&_paraFormat, &pro->_paraFormat, sizeof(pro->_paraFormat));
	}
	return this;
}

bool CRTFTextObject::BuildProperties( CXTPPropertyGridItem * pCategoryObjects )
{
    return true;
}

void CRTFTextObject::OnPropertyItemChangedNotify( CXTPPropertyGridItem * pItem )
{

}

void CRTFTextObject::Initialize()
{
	_pRichTextRender=new CRichTextRender();
	_pTextHost=NULL;
}

void CRTFTextObject::Serialize( CArchive & ar )
{
	if(ar.IsLoading())
		{
			ar >> _rtfText;
			ar >> _charFormat.cbSize;
			ar.Read(((char*)&_charFormat) + sizeof(UINT), _charFormat.cbSize);
			ar >> _paraFormat.cbSize;
            ar.Read(((char*)&_paraFormat) + sizeof(UINT), _paraFormat.cbSize);
		}
		else
		{
			ar << _rtfText;
			ar.Write(&_charFormat, _charFormat.cbSize);
			ar.Write(&_paraFormat, _paraFormat.cbSize);
		}
}

ITextHost * CRTFTextObject::GetTextHost()
{
	return _pTextHost;
}

void CRTFTextObject::SetTextHost( ITextHost *pHost )
{
	_pTextHost=pHost;
	pHost->AddRef();
}

