#pragma once
#include "SymbolObject.h"

class CCharSymbol:public CSymbolObject
{
private:
	TCHAR _symbol;
	CString _fontName;

	DECLARE_SERIAL(CCharSymbol)
	CCharSymbol();
public:
	virtual CDrawObject * CopyFrom(CDrawObject * source);

	CCharSymbol(TCHAR symbol,const CString& fontName, const CString &name, 
		const CString & category,
		Gdiplus::Size size=Gdiplus::Size(DEFAULT_MAX_SYMBOL_SIZE, DEFAULT_MAX_SYMBOL_SIZE), int type=INTERNAL_SYMBOL);
	virtual void SetChar(TCHAR ch);
	virtual TCHAR GetChar();
	virtual void OnCharChanging(TCHAR &ch);
	virtual void OnCharChanged();
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void Serialize(CArchive & ar);
};