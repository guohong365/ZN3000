#pragma once
#include "rectobject.h"

class CEllipseObject:public CRectObject
{
protected:
	DECLARE_SERIAL(CEllipseObject)
public:
	CEllipseObject(void);
	virtual ~ CEllipseObject(void);
	CEllipseObject(const CString & name, const Gdiplus::Rect & rect);
	CEllipseObject(const CString & name, int x, int y, int width, int height);
	CEllipseObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);

	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);

	virtual int HitTest(const Gdiplus::Point & pt);
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);
};
