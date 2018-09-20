#pragma once
#include "RectObject.h"
#define DEFAULT_MAX_SYMBOL_SIZE 50
#define DEFAULT_MIN_SYMBOL_SIZE 10

#define INTERNAL_SYMBOL 0
#define CUSTOM_SYMBOL   1

class CSymbolObject:public CRectObject
{
	static int _maxSize;
	static int _minSize;

	int _index;
    int _type; // 0 - internal symbol, 1 - custom symbol
	CString _category;
	HCURSOR _hCursor;

	DECLARE_SERIAL(CSymbolObject)
public:
	CSymbolObject(void);
	CSymbolObject(const CString &name, const CString & category,const Gdiplus::Size &size, int type=INTERNAL_SYMBOL);
	virtual ~CSymbolObject(void);

	virtual HBITMAP GetSymbolImage(int w, int h, Gdiplus::Color background=0);
    virtual HICON GetSymbolIcon(int w, int h, Gdiplus::Color background=0);
	virtual HCURSOR GetSymbolCursor();

	static void SetMaxSize(int maxSize);
	static int GetMaxSize();
	static void SetMinSize(int minSize);
	static int GetMinSize();

    void SetType(int type)
    {
        _type=type;
    }
    int GetType()
    {
        return _type;
    }
	void SetIndex(int index);
	int GetIndex();

	virtual void SetCategory(const CString & category);
	virtual CString GetCategory();
	virtual void OnCategoryChanging(CString & newCategory);
	virtual void OnCategoryChanged();
	virtual void OnSizeChanging( Gdiplus::Size & newSize );

	virtual CDrawObject* CopyFrom(CDrawObject * source);

	virtual void Serialize(CArchive & ar);

	virtual int GetHandleCount();
	virtual Gdiplus::Point GetHandle(int nHandle);
	virtual void OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam);
	virtual HCURSOR GetHandleCursor(int nHandle);
	void MoveHandleTo(int &nHandle, Gdiplus::Point point);
};

