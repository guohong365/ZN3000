#pragma once
#include "RectObject.h"


class CLineObject :
	public CRectObject
{
protected:
	CLineObject();
public:
	virtual ~CLineObject();
	CLineObject(const CString & name, const Gdiplus::Rect & rect);
	CLineObject(const CString & name, int x, int y, int width, int height);
	CLineObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size &size );

	virtual void OnDraw(Gdiplus::Graphics &graph);
	virtual void OnDrawBorder(Gdiplus::Graphics &graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics &graph);

	virtual int GetHandleCount();
	virtual int HitTest(const Gdiplus::Point &pt);
	virtual DrawObject* CreateInstance(){return new CLineObject();}
};
