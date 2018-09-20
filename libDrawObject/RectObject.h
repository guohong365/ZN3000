#pragma once
#include "DrawObject.h"

class CRectObject:public CDrawObject
{
protected:
	DECLARE_SERIAL(CRectObject) 
public:
	CRectObject();
	CRectObject(const CString & name, const Gdiplus::Rect & rect);
	CRectObject(const CString & name, int x, int y, int width, int height);
	CRectObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);

	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);

private:
};
