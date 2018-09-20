#pragma once
#include "CurveObject.h"


class CClosedCurveObject:public CCurveObject
{
protected:
	CClosedCurveObject(void);
	DECLARE_SERIAL(CClosedCurveObject) public:
	virtual ~ CClosedCurveObject(void);

public:
	CClosedCurveObject(const CString & name, int x, int y);
	CClosedCurveObject(const CString & name, const Gdiplus::Point & pt);

	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);

	virtual int HitTest(const Gdiplus::Point & pt);
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);
};
