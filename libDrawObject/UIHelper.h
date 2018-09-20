#pragma once

class CUICoordinateHelper
{
private:
public:
	int _horzSize;
	int _vertSize;
	int _horzRes;
	int _vertRes;
	double _horzLMPerDeviceUnit;
	double _vertLMPerDeviceUnit;
	int _seperate;
	bool _canRelease;
	CUICoordinateHelper(HDC hdc);
	

	~CUICoordinateHelper();
	static CUICoordinateHelper& GetHelper(HDC hdc=NULL);
	 void Release();
	 int DPtoLP(LPPOINT pPoints, int count, int scale=100);
	 int DPtoLP(LPRECT pRects, int count, int scale=100);
	 int LPtoDP(LPPOINT pPoints, int count, int scale=100);
	 int LPtoDP(LPRECT pRects, int count, int scale=100);
	 int DHLtoLHL(int length /*in 0.1 mm unit*/, int scale=100);
	 int DVLtoLVL(int length, int scale=100);
	 int LHLtoDHL(int length, int scale = 100);
	 int LVLtoDVL(int length, int scale = 100);
	 int DPtoLP(LPSIZE pSize, int count, int scale=100);
	 int LPtoDP(LPSIZE pSize, int count , int scale=100);
};