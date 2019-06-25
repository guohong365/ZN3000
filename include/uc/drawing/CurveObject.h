#pragma once
#include "polylineobject.h"

class CCurveObject:public CPolyLineObject
{
protected:
	CCurveObject(void);
	DECLARE_SERIAL(CCurveObject) public:
	CCurveObject(const CString & name, int x, int y);
	CCurveObject(const CString & name, const Gdiplus::Point & pt);
	virtual ~ CCurveObject(void);
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual int HitTest(const Gdiplus::Point & pt);
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);
};
