#include "stdafx.h"
#include "Settings.h"

#define SECTION_SAMPLE _T("Sample")
#define ITEM_BUFFER_SIZE _T("BufferSize")
#define DEFAULT_BUFFER_SIZE 100000

#define ITEM_SAMPLE_FREQUENCY _T("SampleFrequency")
#define DEFAULT_SAMPLE_FREQUENCY (-1)

#define SECTION_BASIC_PARAM _T("Basic")
#define ITEM_ELEVATION _T("Elevation")
#define DEFAULT_ELEVATION 0
#define ITEM_Z0 _T("Z0")
#define DEFAULT_Z0 30
#define ITEM_HOSPITAL _T("Hospital")
#define DEFAULT_HOSPITAL _T("X")
#define ITEM_DEPARTMENT _T("Department")
#define DEFAULT_DEPARTMENT _T("X")

#define SECTION_COM _T("COM")
#define ITEM_PORT _T("Port")
#define DEFAULT_COM_PORT _T("COM2")
#define ITEM_BAUD_RATE _T("BaudRate")
#define DEFAULT_BAUD_RATE 500000
#define ITEM_DATA_BITS _T("DataBits")
#define DEFAULT_DATA_BITS 8
#define ITEM_STOP_BITS _T("StopBits")
#define DEFAULT_STOP_BITS 1
#define ITEM_PARITY _T("Parity")
#define DEFAULT_PARITY 0
Settings::Settings(const CString& configFile)
	:_iniFile(configFile)
{
}

int Settings::load()
{
	return OpenIniFile(_iniFile);
}

int Settings::save()
{
	FILE * fp=nullptr;
	_tfopen_s(&fp, _iniFile, _T("w"));
	if(fp!=nullptr){
		DumpIni(fp);
		fclose(fp);
		return 0;
	}
	return -1;
}

SIZE_T Settings::getBufferSize()
{
	return GetConfigInt(SECTION_SAMPLE, ITEM_BUFFER_SIZE, DEFAULT_BUFFER_SIZE); 
}

float Settings::getFrequency()
{
	return GetConfigDouble(SECTION_SAMPLE, ITEM_SAMPLE_FREQUENCY, DEFAULT_SAMPLE_FREQUENCY);
}

float Settings::getEvaluate()
{
	return GetConfigDouble(SECTION_BASIC_PARAM, ITEM_ELEVATION, DEFAULT_ELEVATION);
}

float Settings::getZ0()
{
	return GetConfigDouble(SECTION_BASIC_PARAM, ITEM_Z0, DEFAULT_Z0);
}

CString Settings::getHospital()
{
	return GetConfigString(SECTION_BASIC_PARAM, ITEM_HOSPITAL, DEFAULT_HOSPITAL);
}

CString Settings::getDepartment()
{
	return GetConfigString(SECTION_BASIC_PARAM, ITEM_DEPARTMENT, DEFAULT_DEPARTMENT);
}

CString Settings::getSerialPort()
{
	return GetConfigString(SECTION_COM, ITEM_PORT, DEFAULT_COM_PORT);
}

DWORD Settings::getBaudRate()
{
	return GetConfigInt(SECTION_COM, ITEM_BAUD_RATE, DEFAULT_BAUD_RATE);
}

BYTE Settings::getDataBits()
{
	return GetConfigInt(SECTION_COM, ITEM_DATA_BITS, DEFAULT_DATA_BITS);
}

BYTE Settings::getStopBits()
{
	return GetConfigInt(SECTION_COM, ITEM_STOP_BITS, DEFAULT_STOP_BITS);
}

BYTE Settings::getParity()
{
	return GetConfigInt(SECTION_COM, ITEM_PARITY, DEFAULT_PARITY);
}

