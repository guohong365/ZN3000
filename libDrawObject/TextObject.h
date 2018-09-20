#pragma once
#include "rectobject.h"
#include "DrawObjectGlobal.h"

typedef BOOL (*LPTEXT_EDIT_CALLBACK)(CDrawObject * pSender, CWnd *pWnd, CString & buffer);



class CTextObject:public CRectObject
{
private:
	static LPTEXT_EDIT_CALLBACK _pTextEditCallback;
	void initialize();
protected:

	CTextObject(void);
	DECLARE_SERIAL(CTextObject) public:
	virtual ~ CTextObject(void);
	CTextObject(const CString & name, int x, int y, const CString & fontName=DEFAULT_FONT_NAME,
		float fontSize=FontSizeArray[DEFAULT_FONT_SIZE_INDEX].SizePoint*FONT_SIZE_FACTOR, 
		int fontStyle=DEFAULT_FONT_STYLE);
	CTextObject(const CString & name, const Gdiplus::Point & point, const CString & fontName=DEFAULT_FONT_NAME,
		float fontSize=FontSizeArray[DEFAULT_FONT_SIZE_INDEX].SizePoint*FONT_SIZE_FACTOR, 
		int fontStyle=DEFAULT_FONT_STYLE);

	static void SetEditCallBack(LPTEXT_EDIT_CALLBACK pCallback);
	static LPTEXT_EDIT_CALLBACK GetEditCallback();
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);
	//复制对象
	//@param source 复制source对象当前数据到本对象，不复制撤销列表。
	virtual CDrawObject *CopyFrom(CDrawObject * source);
	virtual void Serialize(CArchive & ar);
	virtual bool OnLButtonDblClk(CWnd *pWnd, UINT nFlags, Gdiplus::Point point);
	Gdiplus::RectF GetTextExtent();
	Gdiplus::StringFormat * GetStringFormat();
protected:
	virtual void OnNameChanged();
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);
private:
	Gdiplus::StringFormat * m_pStringFormat;
};
