#pragma once
#include "drawobject.h"
#include "PolyLineObject.h"
#include "RectObject.h"

class CTyreFrontSelector:public CDrawObject
{
protected:
	DECLARE_SERIAL(CTyreFrontSelector) CTyreFrontSelector();

public:
	CTyreFrontSelector(const Gdiplus::Point & pt, const Gdiplus::Size & size, int areaHeight);
	virtual ~ CTyreFrontSelector(void);

	int GetAreaHeight() const;
	void SetAreaHeight(int height);
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual int HitTest(const Gdiplus::Point & pt);
	virtual Gdiplus::Point GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);
	virtual HCURSOR GetHandleCursor(int nHandle);

	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);
	virtual void Serialize(CArchive & ar);


private:
	int m_AreaHeight;
};
