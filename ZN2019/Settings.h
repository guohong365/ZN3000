#pragma once
#include "../utility/IniFile.h"
#define SETTING_FILE_NAME _T("zn2000.ini")

class Settings : public CIniFile
{
	CString _iniFile;
public:
	explicit Settings(const CString& configFile);
	int load();
	int save();

	SIZE_T getBufferSize();
	float getFrequency();

	float getEvaluate();  //����
	float getZ0();        //��������(ķŷ)

	CString getHospital();
	CString getDepartment();
	CString getSerialPort();
	DWORD getBaudRate();
	BYTE getDataBits();
	BYTE getStopBits();
	BYTE getParity();
};