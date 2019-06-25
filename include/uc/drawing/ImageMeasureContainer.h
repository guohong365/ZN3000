#pragma once
#include "imagecontainer.h"


class CImageMeasureContainer :
	public CImageContainer
{
protected:
	CImageMeasureContainer()
	{
		CImageContainer::Initialize();
	}
	DECLARE_SERIAL(CImageMeasureContainer) 

public:
	CImageMeasureContainer(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options = WITH_HORIZ_RULER | WITH_VERT_RULER);
	CImageMeasureContainer(const CString & name, const Gdiplus::Rect & rect, DWORD options = WITH_HORIZ_RULER | WITH_VERT_RULER);

	virtual CDrawObject *GetObjectsAt(const Gdiplus::Point & point);
	virtual ~CImageMeasureContainer(void);
};
