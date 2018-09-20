#include "StdAfx.h"
#include "GalleryItemSymbol.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CGalleryItemSymbol::CGalleryItemSymbol(CSymbolObject * pSymbol, int Id, LPCTSTR lpszCaption)
:_pSymbolObject(pSymbol)
{
	m_nId=Id;
	m_strCaption=lpszCaption;
}

CGalleryItemSymbol::~CGalleryItemSymbol(void)
{
	delete _pSymbolObject;
}
