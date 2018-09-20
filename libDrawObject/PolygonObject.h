#pragma once
#include "polylineobject.h"


class CPolygonObject:public CPolyLineObject
{
protected:
	DECLARE_SERIAL(CPolygonObject) 
public:
	CPolygonObject(void);
	CPolygonObject(const CString & name, int x, int y);
	CPolygonObject(const CString & name, const Gdiplus::Point & pt);
	virtual ~ CPolygonObject(void);

	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);

	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual int HitTest(const Gdiplus::Point & pt);
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);
};

