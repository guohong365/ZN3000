#include "stdafx.h"
#include "Settings.h"

#define SECTION_SAMPLE _T("����")
#define ITEM_BUFFER_SIZE _T("������")
#define DEFAULT_BUFFER_SIZE 100000


#define SECTION_BASIC_PARAM _T("��������")
#define ITEM_ELEVATION _T("����")
#define DEFAULT_ELEVATION 0
#define ITEM_Z0 _T("�����迹")
#define DEFAULT_Z0 30
#define ITEM_HOSPITAL _T("ҽԺ")
#define DEFAULT_HOSPITAL _T("X")
#define ITEM_DEPARTMENT _T("����")
#define DEFAULT_DEPARTMENT _T("X")

#define SECTION_DEVICE _T("�豸")
#define ITEM_DEVICE_VERSION _T("Э��汾")
#define DEFAULT_DEVICE_VERSION _T("0001")
#define ITEM_DEVICE_FREQUENCY _T("����Ƶ��")
#define DEFAULT_DEVICE_FREQUENCY (-1)
#define ITEM_DIGITAL_SCOPE _T("��ֵ����Χ")
#define ITEM_PHYSICAL_SCOPE _T("��������Χ")

#define SECTION_COM _T("����")
#define ITEM_PORT _T("�˿ں�")
#define DEFAULT_COM_PORT _T("COM2")
#define ITEM_BAUD_RATE _T("������")
#define DEFAULT_BAUD_RATE 500000
#define ITEM_DATA_BITS _T("����λ")
#define DEFAULT_DATA_BITS 8
#define ITEM_STOP_BITS _T("ֹͣλ")
#define DEFAULT_STOP_BITS 1
#define ITEM_PARITY _T("��żУ��")
#define DEFAULT_PARITY 0

#define SECTION_UI _T("����")
#define ITEM_CTRL_BACKGROUND_COLOR _T("����.��ɫ")
#define DEFAULT_CTRL_BACKGROUND_COLOR 0xFF000000
#define ITEM_GRID_BACKGROUND_COLOR _T("����.����.��ɫ")
#define DEFAULT_GRID_BACKGROUND_COLOR 0xFF004000
#define ITEM_GRID_BORDER_COLOR _T("����.�߿�.��ɫ")
#define DEFAULT_GRID_BORDER_COLOR 0xFF00FF00
#define ITEM_GRID_BORDER_WIDTH _T("����.�߿�.�߿�")
#define DEFAULT_GRID_BORDER_WIDTH 2
#define ITEM_GRID_BORDER_SHOW _T("����.�߿�")
#define DEFAULT_GRID_BORDER_SHOW true

#define ITEM_GRID_THIN_LINE_SPACING _T("����.ϸ.���")
#define DEFAULT_GRID_THIN_LINE_SPACING 20
#define ITEM_GRID_THIN_LINE_COLOR _T("����.ϸ.��ɫ")
#define DEFAULT_GRID_THIN_LINE_COLOR 0xFF00C000
#define ITEM_GRID_THIN_LINE_WIDTH _T("����.ϸ.�߿�")
#define DEFAULT_GRID_THIN_LINE_WIDTH 1
#define ITEM_GRID_THIN_LINE_SHOW _T("����.ϸ.��ʾ")
#define DEFAULT_GRID_THIN_LINE_SHOW true
#define ITEM_GRID_THIN_LINE_STYLE _T("����.��.��ʽ")
#define DEFAULT_GRID_THIN_LINE_STYLE 0

#define ITEM_GRID_THICK_LINE_SPACING _T("����.��.���")
#define DEFAULT_GRID_THICK_LINE_SPACING 250
#define ITEM_GRID_THICK_LINE_COLOR _T("����.��.��ɫ")
#define DEFAULT_GRID_THICK_LINE_COLOR 0xFF00FF00
#define ITEM_GRID_THICK_LINE_WIDTH _T("����.��.�߿�")
#define DEFAULT_GRID_THICK_LINE_WIDTH 2
#define ITEM_GRID_THICK_LINE_SHOW _T("����.��.��ʾ")
#define DEFAULT_GRID_THICK_LINE_SHOW true
#define ITEM_GRID_THICK_LINE_STYLE _T("����.��.��ʽ")
#define DEFAULT_GRID_THICK_LINE_STYLE 0

#define ITEM_WAVE_COLOR _T("����.��ɫ")
#define DEFAULT_WAVE_COLOR 0xFFADD8E6
#define ITEM_WAVE_WIDTH _T("����.�߿�")
#define DEFAULT_WAVE_WIDTH 3
#define ITEM_WAVE_LINE_STYLE _T("����.��ʽ")
#define DEFAULT_WAVE_LINE_STYLE 0
#define ITEM_WAVE_DRAW_MODE _T("����.��ʾ.��ʽ")
#define DEFAULT_WAVE_DRAW_MODE _T("�ƶ�")
#define WAVE_DRAW_MODE_STATIC _T("��ֹ")
#define WAVE_DRAW_MODE_ROLLING _T("�ƶ�")
#define WAVE_DRAW_MODE_ERASURE _T("����")

#define ITEM_WAVE_ERASE_WIDTH _T("����.��ʾ.�������")
#define DEFAULT_WAVE_ERASE_WIDTH 50
#define ITEM_WAVE_VELOCITY _T("����.��ʾ.�ٶ�")
#define DEFAULT_WAVE_VELOCITY 25
#define ITEM_WAVE_BASELINE_SHOW _T("����.����.��ʾ")
#define DEFAULT_WAVE_BASELINE_SHOW false
#define ITEM_WAVE_BASELINE_COLOR _T("����.����.��ɫ")
#define DEFAULT_WAVE_BASELINE_COLOR 0xFF00FFFF
#define ITEM_WAVE_BASELINE_WIDTH _T("����.����.�߿�")
#define DEFAULT_WAVE_BASELINE_WIDTH 3
#define ITEM_WAVE_BASELINE_STYLE _T("����.����.��ʽ")
#define DEFAULT_WAVE_BASELINE_STYLE 0

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

double Settings::getFrequency()
{
	return GetConfigDouble(SECTION_DEVICE, ITEM_DEVICE_FREQUENCY, DEFAULT_DEVICE_FREQUENCY);
}

double Settings::getEvaluate()
{
	return GetConfigDouble(SECTION_BASIC_PARAM, ITEM_ELEVATION, DEFAULT_ELEVATION);
}

double Settings::getZ0()
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
	return BYTE(GetConfigInt(SECTION_COM, ITEM_DATA_BITS, DEFAULT_DATA_BITS));
}

BYTE Settings::getStopBits()
{
	return BYTE(GetConfigInt(SECTION_COM, ITEM_STOP_BITS, DEFAULT_STOP_BITS));
}

BYTE Settings::getParity()
{
	return BYTE(GetConfigInt(SECTION_COM, ITEM_PARITY, DEFAULT_PARITY));
}

CString Settings::getDeviceVersion()
{
	return GetConfigString(SECTION_DEVICE, ITEM_DEVICE_VERSION);
}

static ValueScopeInt DefaultDigitalScope={0, 65535};
ValueScopeInt Settings::getDeviceDigitalScope()
{
	return GetConfigValueScopeInt(SECTION_DEVICE, ITEM_DIGITAL_SCOPE, DefaultDigitalScope);
}
static ValueScopeDouble DefaultPhysicalScope={0, 10};
ValueScopeDouble Settings::getDevicePhysicalScope()
{
	return GetConfigValueScopeDouble(SECTION_DEVICE, ITEM_PHYSICAL_SCOPE, DefaultPhysicalScope);
}

Gdiplus::Color Settings::getGridBackgroundColor()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_BACKGROUND_COLOR, DEFAULT_GRID_BACKGROUND_COLOR);
}

Gdiplus::Color Settings::getGridBorderColor()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_BORDER_COLOR, DEFAULT_GRID_BORDER_COLOR);
}

int Settings::getGridBorderWidth()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_BORDER_WIDTH, DEFAULT_GRID_BORDER_WIDTH);
}

Gdiplus::Color Settings::getGridThinLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_THIN_LINE_COLOR, DEFAULT_GRID_THIN_LINE_COLOR);
}

int Settings::getGridThinLineWidth()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_THIN_LINE_WIDTH, DEFAULT_GRID_THIN_LINE_WIDTH);
}

Gdiplus::Color Settings::getGridThickLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_THICK_LINE_COLOR, DEFAULT_GRID_THICK_LINE_COLOR);
}

int Settings::getGridThickLineWidth()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_THICK_LINE_WIDTH, DEFAULT_GRID_THICK_LINE_WIDTH);
}

Gdiplus::Color Settings::getWaveBaseLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_WAVE_BASELINE_COLOR, DEFAULT_WAVE_BASELINE_COLOR);
}

int Settings::getWaveBaseLineWidth()
{
	return GetConfigInt(SECTION_UI, ITEM_WAVE_BASELINE_WIDTH, DEFAULT_WAVE_BASELINE_WIDTH);
}

Gdiplus::Color Settings::getWaveLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_WAVE_COLOR, DEFAULT_WAVE_COLOR);
}

int Settings::getWaveLineWidth()
{
	return GetConfigInt(SECTION_UI, ITEM_WAVE_WIDTH, DEFAULT_WAVE_WIDTH);
}

bool Settings::getShowWaveBaseLine()
{
	return GetConfigBool(SECTION_UI, ITEM_WAVE_BASELINE_SHOW, DEFAULT_WAVE_BASELINE_SHOW);
}

int Settings::getWaveDrawMode()
{
	const TCHAR* p=GetConfigString(SECTION_UI, ITEM_WAVE_DRAW_MODE, DEFAULT_WAVE_DRAW_MODE);
	if(_tcscmp(WAVE_DRAW_MODE_STATIC,p) == 0) return DRAW_STATIC;
	if(_tcscmp(WAVE_DRAW_MODE_ROLLING, p)==0) return DRAW_ROLLING;
	if(_tcscmp(WAVE_DRAW_MODE_ERASURE, p)==0) return DRAW_ERASURE;
	return DRAW_ROLLING;
}

double Settings::getWaveVelocity()
{
	return GetConfigDouble(SECTION_UI, ITEM_WAVE_VELOCITY, DEFAULT_WAVE_VELOCITY);
}

int Settings::getGridThickLineSpacing()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_THICK_LINE_SPACING, DEFAULT_GRID_THICK_LINE_SPACING);
}

int Settings::getGridThinLineSpacing()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_THIN_LINE_SPACING, DEFAULT_GRID_THIN_LINE_SPACING);
}

bool Settings::getShowGridThickLine()
{
	return GetConfigBool(SECTION_UI, ITEM_GRID_THICK_LINE_SHOW, DEFAULT_GRID_THICK_LINE_SHOW);
}

bool Settings::getShowGridThinLine()
{
	return GetConfigBool(SECTION_UI, ITEM_GRID_THIN_LINE_SHOW, DEFAULT_GRID_THIN_LINE_SHOW);
}

bool Settings::getShowGridBorder()
{
	return GetConfigBool(SECTION_UI, ITEM_GRID_BORDER_SHOW, DEFAULT_GRID_BORDER_SHOW);
}

int Settings::getGridThickLineStyle()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_THICK_LINE_STYLE, DEFAULT_GRID_THICK_LINE_STYLE);
}

int Settings::getGridThinLineStyle()
{
	return GetConfigInt(SECTION_UI, ITEM_GRID_THIN_LINE_STYLE, DEFAULT_GRID_THIN_LINE_STYLE);
}

int Settings::getWaveBaseLineStyle()
{
	return GetConfigInt(SECTION_UI, ITEM_WAVE_BASELINE_STYLE, DEFAULT_WAVE_BASELINE_STYLE);
}

int Settings::getWaveEraseWidth()
{
	return GetConfigInt(SECTION_UI, ITEM_WAVE_ERASE_WIDTH, DEFAULT_WAVE_ERASE_WIDTH);
}

int Settings::getWaveLineStyle()
{
	return GetConfigInt(SECTION_UI, ITEM_WAVE_LINE_STYLE, DEFAULT_WAVE_LINE_STYLE);
}



