#pragma once
#include "drawobject.h"
#include "EllipseObject.h"

class CTyreSideSelector:public CDrawObject
{
protected:
	CTyreSideSelector(void);
	DECLARE_SERIAL(CTyreSideSelector) public:
	virtual ~ CTyreSideSelector(void);
	CTyreSideSelector(const Gdiplus::Point & pt, const Gdiplus::Size & size, int radioInner);

	int GetInnerRadio() const;
	void SetInnerRadio(int inner);
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual int HitTest(const Gdiplus::Point & pt);
	virtual Gdiplus::Point GetHandle(int nHandle);
	virtual int GetHandleCount();
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);
	virtual HCURSOR GetHandleCursor(int nHandle);

	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);

	virtual void OnSelectedChanged();
	virtual void Serialize(CArchive & ar);
private:
	int m_RadioInner;
};
