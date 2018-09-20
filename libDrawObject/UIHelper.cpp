#include "stdafx.h"
#include "UIHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


class CCoordHelperHolder
{
	CUICoordinateHelper *helper;
	friend class CUICoordinateHelper;
public:
	CCoordHelperHolder()
	{
		UpdateHelper();
	}
	void UpdateHelper()
	{
		HDC hDC=::GetDC(NULL);
		helper=&CUICoordinateHelper::GetHelper(hDC);
		::ReleaseDC(NULL, hDC);
	}
	~CCoordHelperHolder()
	{
		delete helper;
	}
};

static CCoordHelperHolder theCoordHelper;

CUICoordinateHelper::CUICoordinateHelper(HDC hdc)
{
	ASSERT(hdc);
	_horzSize=GetDeviceCaps(hdc, HORZSIZE);
	_vertSize=GetDeviceCaps(hdc, VERTSIZE);
	_horzRes=GetDeviceCaps(hdc, HORZRES);
	_vertRes=GetDeviceCaps(hdc, VERTRES);
	_seperate=10; 
	_canRelease=true;
	_horzLMPerDeviceUnit= (double)_horzSize / _horzRes  *  _seperate;    //0.1mm per pixel align x
	_vertLMPerDeviceUnit = (double)_vertSize / _vertRes * _seperate;   //0.1mm per pixel align y

	_horzSize=GetDeviceCaps(hdc, LOGPIXELSX);
	_vertSize=GetDeviceCaps(hdc, LOGPIXELSY);

	_horzLMPerDeviceUnit= (double)25.4 / _horzSize * _seperate;    //0.1mm per pixel align x
	_vertLMPerDeviceUnit = (double)25.4 /_vertSize * _seperate;   //0.1mm per pixel align y
}

CUICoordinateHelper::~CUICoordinateHelper()
{
}

int CUICoordinateHelper::DPtoLP( LPPOINT pPoints, int count, int scale/*=100*/ )
{
	ASSERT(scale > 0);
	int i=0;
	for(i =0; i< count; i++)
	{
		pPoints[i].x=(int) (_horzLMPerDeviceUnit * pPoints[i].x / scale * 100); 
		pPoints[i].y=(int) (_vertLMPerDeviceUnit * pPoints[i].y / scale * 100);
	}
	return i;
}

int CUICoordinateHelper::DPtoLP( LPRECT pRects, int count, int scale/*=100*/ )
{
	ASSERT(scale > 0);
	int i=0;
	for(i =0; i< count; i++)
	{
		pRects[i].left= (int)(_horzLMPerDeviceUnit * pRects[i].left / scale * 100); 
		pRects[i].right= (int)(_horzLMPerDeviceUnit * pRects[i].right / scale * 100); 
		pRects[i].top= (int)(_vertLMPerDeviceUnit * pRects[i].top / scale * 100);
		pRects[i].bottom= (int)(_vertLMPerDeviceUnit * pRects[i].bottom / scale * 100);
	}
	return i;
}

int CUICoordinateHelper::DPtoLP( LPSIZE pSize, int count, int scale/*=100*/ )
{
	ASSERT(scale > 0);
	int i=0;
	for(i =0; i< count; i++)
	{
		pSize[i].cx= (int)(_horzLMPerDeviceUnit * pSize[i].cx / scale * 100); 
		pSize[i].cy= (int)(_vertLMPerDeviceUnit * pSize[i].cy / scale * 100);
	}
	return i;

}
int CUICoordinateHelper::LPtoDP( LPRECT pRects, int count, int scale/*=100*/ )
{
	ASSERT(scale > 0);
	int i=0;
	for(i =0; i< count; i++)
	{
		pRects[i].left= (int)(pRects[i].left / _horzLMPerDeviceUnit * scale / 100); 
		pRects[i].right= (int)(pRects[i].right / _horzLMPerDeviceUnit * scale / 100); 
		pRects[i].top= (int)(pRects[i].top / _vertLMPerDeviceUnit * scale / 100);
		pRects[i].bottom= (int)(pRects[i].bottom / _vertLMPerDeviceUnit * scale / 100);
	}
	return i;
}

int CUICoordinateHelper::LPtoDP( LPPOINT pPoints, int count, int scale/*=100*/ )
{
	int i=0;
	for (i=0; i< count; i++)
	{
		pPoints[i].x= (int)(pPoints[i].x / _horzLMPerDeviceUnit * scale / 100);
		pPoints[i].y= (int)(pPoints[i].y / _vertLMPerDeviceUnit * scale / 100);
	}
	return 0;
}

int CUICoordinateHelper::LPtoDP( LPSIZE pSize, int count , int scale/*=100*/ )
{
	int i=0;
	for (i=0; i< count; i++)
	{
		pSize[i].cx= (int)(pSize[i].cx / _horzLMPerDeviceUnit * scale / 100);
		pSize[i].cy= (int)(pSize[i].cy / _vertLMPerDeviceUnit * scale / 100);
	}
	return 0;

}
int CUICoordinateHelper::DHLtoLHL( int length /*in 0.1 mm unit*/, int scale/*=100*/ )
{
	ASSERT(scale > 0);
	return (int)(length * _horzLMPerDeviceUnit /scale * 100);
}

int CUICoordinateHelper::DVLtoLVL( int length, int scale/*=100*/ )
{
	ASSERT(scale > 0);
	return (int)(length * _vertLMPerDeviceUnit /scale * 100);
}

int CUICoordinateHelper::LHLtoDHL( int length, int scale /*= 100*/ )
{
	return (int)(length / _horzLMPerDeviceUnit * scale /100);
}

int CUICoordinateHelper::LVLtoDVL( int length, int scale /*= 100*/ )
{
	return (int)(length / _vertLMPerDeviceUnit * scale / 100);
}

CUICoordinateHelper& CUICoordinateHelper::GetHelper( HDC hdc )
{
	if(hdc==NULL)
	{
		return *(theCoordHelper.helper);
	}
	CUICoordinateHelper *pHelper=new CUICoordinateHelper(hdc);
	return *pHelper;
}

void CUICoordinateHelper::Release()
{
	if(this!=theCoordHelper.helper)
		delete this;
}