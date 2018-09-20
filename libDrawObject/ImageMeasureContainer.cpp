#include "StdAfx.h"
#include "imagemeasurecontainer.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CImageMeasureContainer, CImageContainer, 0)

CImageMeasureContainer::CImageMeasureContainer(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options)
:CImageContainer(name, pt, size, options)
{
}
CImageMeasureContainer::CImageMeasureContainer(const CString & name, const Gdiplus::Rect & rect, DWORD options )
:CImageContainer(name, rect, options)
{
}

CImageMeasureContainer::~CImageMeasureContainer(void)
{
}

CDrawObject *CImageMeasureContainer::GetObjectsAt(const Gdiplus::Point & point)
{
    CDrawObject *pObject = CObjectsContainer::GetObjectsAt(point);
    return pObject;
}