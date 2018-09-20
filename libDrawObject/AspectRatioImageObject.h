#pragma once
#include "ImageObject.h"

class CAspectRatioImageObject:public CImageObject
{
protected:
	CAspectRatioImageObject();
	DECLARE_SERIAL(CAspectRatioImageObject) 
public:
	CAspectRatioImageObject(const CString & name, const Gdiplus::Rect & rect);
	CAspectRatioImageObject(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size);
	virtual void OnSizeChanging(Gdiplus::Size & newSize);
	virtual void OnSizeChanged();
	virtual int GetHandleCount();
	virtual Gdiplus::Point GetHandle(int nHandle);

	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);
	virtual void Serialize(CArchive & ar);
};
