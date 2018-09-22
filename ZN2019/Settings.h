#pragma once
#include "../utility/IniFile.h"
#define SETTING_FILE_NAME _T("ZN3000.ini")

struct ValueScopeInt
{
	long LowValue;
	long HighValue;
};

struct ValueScopeDouble
{
	double LowValue;
	double HighValue;
};

class Settings : public CIniFile
{
	CString _iniFile;
public:
	explicit Settings(const CString& configFile=SETTING_FILE_NAME);
	int load();
	int save();

	ValueScopeInt GetConfigValueScopeInt(const TCHAR* section, const TCHAR* item,const ValueScopeInt& defaultV);
	ValueScopeDouble GetConfigValueScopeDouble(const TCHAR* section, const TCHAR* item,const ValueScopeDouble& defaultV);
	SIZE_T getBufferSize();
	double getFrequency();

	double getEvaluate();  //º£°Î
	double getZ0();        //»ù´¡µ¼ÄÉ(Ä·Å·)

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
	bool isShowBaseLine();
	int getWaveShowType(); // 1 ÒÆ¶¯ 2 ²Á³ý
	double getWaveVelocity();
};