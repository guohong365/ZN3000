#pragma once
#include "SymbolObject.h"

class CGalleryItemSymbol :
	public CXTPControlGalleryItem
{
	CSymbolObject *_pSymbolObject;
public:
	CGalleryItemSymbol(CSymbolObject * pSymbol, int Id, LPCTSTR lpszCaption);
	virtual ~CGalleryItemSymbol(void);

	virtual void Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
	{

	}
};
