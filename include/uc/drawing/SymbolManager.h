#pragma once
#include "DrawObjectList.h"

enum ErrorCodeSymbolManager
{
    
};

class CSymbolManager
{
	CMapStringToPtr _Symbols;
public:
    enum ErrorCodeSymbolManager
    {
        noError=0,
        outOfMemory,
        saveFialed,
        loadFialed,
        openFialed,
        writeFialed,
        readFialed,
        encryptFialed,
        decryptFialed,
        noKeyFound,
        unknownError
    };
	CSymbolManager();
	~CSymbolManager();
	void Add(const CString& category, CDrawObject *pItem);
	void Add( CDrawObject *pSymbol );
    void Remove(CDrawObject * pSymbol);
	bool LoadSymbols(const CString &filename);
	bool SaveSymbols(const CString &filename);
	int GetImageList(const CString & category, CImageList &imageList, int w, int h, Gdiplus::Color background=0);

	int GetSymbolCategoryCount();
	CString GetCategoryName(int index);

	CDrawObjectList * GetSymbolList(LPCTSTR lpszCategory);

	CMapStringToPtr & GetSymbolMap();
};
