#pragma once
#include <IniFile.h>
#include "GridBackgroundAppearance.h"
#include "WaveDrawerAppearance.h"
#define SETTING_FILE_NAME _T("ZN3000.ini")

class Settings : public IniFile
{
	CString _iniFile;
public:
	explicit Settings(const CString& configFile=SETTING_FILE_NAME);
	int load();
	int save();


	SIZE_T getBufferSize();
	double getFrequency();

	double getEvaluate();  //����
	double getZ0();        //��������(ķŷ)

	CString getHospital();
	CString getDepartment();
	CString getSerialPort();
	DWORD getBaudRate();
	BYTE getDataBits();
	BYTE getStopBits();
	BYTE getParity();
	CString getDeviceVersion();
	ValueScopeInt getDeviceDigitalScope();
	ValueScopeDouble getDevicePhysicalScope();
	Gdiplus::Color getGridBackgroundColor();
	Gdiplus::Color getGridBorderColor();
	int getGridBorderWidth();
	Gdiplus::Color getGridThinLineColor();
	int getGridThinLineWidth();
	Gdiplus::Color getGridThickLineColor();
	int getGridThickLineWidth();
	Gdiplus::Color getWaveBaseLineColor();
	int getWaveBaseLineWidth();
	Gdiplus::Color getWaveLineColor();
	int getWaveLineWidth();
	bool getShowWaveBaseLine();
	int getWaveDrawMode(); // 0��̬ 1 �ƶ� 2 ����
	double getWaveVelocity();
	int getGridThickLineSpacing();
	int getGridThinLineSpacing();
	bool getShowGridThickLine();
	bool getShowGridThinLine();
	bool getShowGridBorder();
	int getGridThickLineStyle();
	int getGridThinLineStyle();
	int getWaveBaseLineStyle();
	int getWaveEraseWidth();
	int getWaveLineStyle();
};