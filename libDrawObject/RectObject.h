#pragma once
#include "DrawObject.h"

class RectObject:public DrawObject
{
public:
	RectObject();
	RectObject(const CString & name, const Gdiplus::Rect & rect);
	RectObject(const CString & name, int x, int y, int width, int height);
	RectObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);

	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);

private:
};
