#pragma once
#include "polygonobject.h"


class CMeasurePolygonObject :
	public CPolygonObject
{
public:
	CMeasurePolygonObject(const CString & name, int x, int y);
	CMeasurePolygonObject(const CString & name, const Gdiplus::Point & pt);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);
	virtual ~CMeasurePolygonObject(void);
};
