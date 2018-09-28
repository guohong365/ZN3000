#include "stdafx.h"
#include "Settings.h"

#define SECTION_SAMPLE _T("采样")
#define ITEM_BUFFER_SIZE _T("缓冲区")
#define DEFAULT_BUFFER_SIZE 100000


#define SECTION_BASIC_PARAM _T("基础配置")
#define ITEM_ELEVATION _T("海拔")
#define DEFAULT_ELEVATION 0
#define ITEM_Z0 _T("基础阻抗")
#define DEFAULT_Z0 30
#define ITEM_HOSPITAL _T("医院")
#define DEFAULT_HOSPITAL _T("X")
#define ITEM_DEPARTMENT _T("科室")
#define DEFAULT_DEPARTMENT _T("X")

#define SECTION_DEVICE _T("设备")
#define ITEM_DEVICE_VERSION _T("协议版本")
#define DEFAULT_DEVICE_VERSION _T("0001")
#define ITEM_DEVICE_FREQUENCY _T("采样频率")
#define DEFAULT_DEVICE_FREQUENCY (-1)
#define ITEM_DIGITAL_SCOPE _T("数值量范围")
#define ITEM_PHYSICAL_SCOPE _T("物理量范围")

#define SECTION_COM _T("串口")
#define ITEM_PORT _T("端口号")
#define DEFAULT_COM_PORT _T("COM2")
#define ITEM_BAUD_RATE _T("波特率")
#define DEFAULT_BAUD_RATE 500000
#define ITEM_DATA_BITS _T("数据位")
#define DEFAULT_DATA_BITS 8
#define ITEM_STOP_BITS _T("停止位")
#define DEFAULT_STOP_BITS 1
#define ITEM_PARITY _T("奇偶校验")
#define DEFAULT_PARITY 0

#define SECTION_UI _T("界面")
#define ITEM_CTRL_BACKGROUND_COLOR _T("背景.颜色")
#define DEFAULT_CTRL_BACKGROUND_COLOR 0xFF000000
#define ITEM_GRID_BACKGROUND_COLOR _T("网格.背景.颜色")
#define DEFAULT_GRID_BACKGROUND_COLOR 0xFF004000
#define ITEM_GRID_BORDER_COLOR _T("网格.边框.颜色")
#define DEFAULT_GRID_BORDER_COLOR 0xFF00FF00
#define ITEM_GRID_BORDER_WIDTH _T("网格.边框.线宽")
#define DEFAULT_GRID_BORDER_WIDTH 2
#define ITEM_GRID_BORDER_SHOW _T("网格.边框")
#define DEFAULT_GRID_BORDER_SHOW true

#define ITEM_GRID_THIN_LINE_SPACING _T("网格.细.间距")
#define DEFAULT_GRID_THIN_LINE_SPACING 20
#define ITEM_GRID_THIN_LINE_COLOR _T("网格.细.颜色")
#define DEFAULT_GRID_THIN_LINE_COLOR 0xFF00C000
#define ITEM_GRID_THIN_LINE_WIDTH _T("网格.细.线宽")
#define DEFAULT_GRID_THIN_LINE_WIDTH 1
#define ITEM_GRID_THIN_LINE_SHOW _T("网格.细.显示")
#define DEFAULT_GRID_THIN_LINE_SHOW true
#define ITEM_GRID_THIN_LINE_STYLE _T("网格.粗.样式")
#define DEFAULT_GRID_THIN_LINE_STYLE 0

#define ITEM_GRID_THICK_LINE_SPACING _T("网格.粗.间隔")
#define DEFAULT_GRID_THICK_LINE_SPACING 250
#define ITEM_GRID_THICK_LINE_COLOR _T("网格.粗.颜色")
#define DEFAULT_GRID_THICK_LINE_COLOR 0xFF00FF00
#define ITEM_GRID_THICK_LINE_WIDTH _T("网格.粗.线宽")
#define DEFAULT_GRID_THICK_LINE_WIDTH 2
#define ITEM_GRID_THICK_LINE_SHOW _T("网格.粗.显示")
#define DEFAULT_GRID_THICK_LINE_SHOW true
#define ITEM_GRID_THICK_LINE_STYLE _T("网格.粗.样式")
#define DEFAULT_GRID_THICK_LINE_STYLE 0

#define ITEM_WAVE_COLOR _T("波形.颜色")
#define DEFAULT_WAVE_COLOR 0xFFADD8E6
#define ITEM_WAVE_WIDTH _T("波形.线宽")
#define DEFAULT_WAVE_WIDTH 3
#define ITEM_WAVE_LINE_STYLE _T("波形.样式")
#define DEFAULT_WAVE_LINE_STYLE 0
#define ITEM_WAVE_DRAW_MODE _T("波形.显示.方式")
#define DEFAULT_WAVE_DRAW_MODE _T("移动")
#define WAVE_DRAW_MODE_STATIC _T("静止")
#define WAVE_DRAW_MODE_ROLLING _T("移动")
#define WAVE_DRAW_MODE_ERASURE _T("擦除")

#define ITEM_WAVE_ERASE_WIDTH _T("波形.显示.擦除宽度")
#define DEFAULT_WAVE_ERASE_WIDTH 50
#define ITEM_WAVE_VELOCITY _T("波形.显示.速度")
#define DEFAULT_WAVE_VELOCITY 25
#define ITEM_WAVE_BASELINE_SHOW _T("波形.基线.显示")
#define DEFAULT_WAVE_BASELINE_SHOW false
#define ITEM_WAVE_BASELINE_COLOR _T("波形.基线.颜色")
#define DEFAULT_WAVE_BASELINE_COLOR 0xFF00FFFF
#define ITEM_WAVE_BASELINE_WIDTH _T("波形.基线.线宽")
#define DEFAULT_WAVE_BASELINE_WIDTH 3
#define ITEM_WAVE_BASELINE_STYLE _T("波形.基线.样式")
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



