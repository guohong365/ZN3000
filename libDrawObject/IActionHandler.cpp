#include "stdafx.h"
#include "IActionHandler.h"
#include "DrawObject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
IMPLEMENT_SERIAL(IActionHandler, CObject, 0)
void IActionHandler::SetSite(CDrawObject *pSite)
{
	_pSite=pSite;
}
CDrawObject * IActionHandler::GetSite()
{
	return _pSite;
}

IActionHandler::~IActionHandler(void)
{
}

IActionHandler::IActionHandler()
{

}

void IActionHandler::OnMouseEnter()
{

}

void IActionHandler::OnMouseOver( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnMouseLeave()
{

}

void IActionHandler::OnMouseLButtonDown( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnMouseLButtonUp( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnMouseLButtonDlbClick( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnMouseRButtonDown( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnMouseRButtonUp( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnMouseRButtonDlbClick( Gdiplus::Point pt, UINT flags )
{

}

void IActionHandler::OnKeyDown( int key, UINT flags )
{

}

void IActionHandler::OnKeyUp( int key, UINT flags )
{

}
