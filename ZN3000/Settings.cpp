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
#define ITEM_GRID_THIN_SPACE _T("网格.细.间距")
#define DEFAULT_GRID_THIN_SPACE 20
#define ITEM_GRID_THIN_COLOR _T("网格.细.颜色")
#define DEFAULT_GRID_THIN_COLOR 0xFF00C000
#define ITEM_GRID_THIN_WIDTH _T("网格.细.线宽")
#define DEFAULT_GRID_THIN_WIDTH 1
#define ITEM_GRID_THICK_INTERVAL _T("网格.粗.间隔")
#define DEFAULT_GRID_THICK_INTERVAL 5
#define ITEM_GRID_THICK_COLOR _T("网格.粗.颜色")
#define DEFAULT_GRID_THICK_COLOR 0xFF00FF00
#define ITEM_GRID_THICK_WIDTH _T("网格.粗.线宽")
#define DEFAULT_GRID_THICK_WIDTH 2
#define ITEM_WAVE_COLOR _T("波形.颜色")
#define DEFAULT_WAVE_COLOR 0xFFADD8E6
#define ITEM_WAVE_WIDTH _T("波形.线宽")
#define DEFAULT_WAVE_WIDTH 3
#define ITEM_WAVE_SHOW_TYPE _T("波形.显示.方式")
#define DEFAULT_WAVE_SHOW_TYPE _T("移动")
#define ITEM_WAVE_VELOCITY _T("波形.显示.速度")
#define DEFAULT_WAVE_VELOCITY 25
#define ITEM_WAVE_BASELINE_SHOW _T("波形.基线.显示")
#define DEFAULT_WAVE_BASELINE_SHOW _T('否')
#define ITEM_WAVE_BASELINE_COLOR _T("波形.基线.颜色")
#define DEFAULT_WAVE_BASELINE_COLOR 0xFF00FFFF
#define ITEM_WAVE_BASELINE_WIDTH _T("波形.基线.线宽")
#define DEFAULT_WAVE_BASELINE_WIDTH 3

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
bool parseScopeInt(const TCHAR* buffer, size_t buf_len, ValueScopeInt& scope)
{
	if(buffer[0]!=_T('(') || buffer[buf_len-1]!=_T(')')) return false;
	const TCHAR* p=buffer;
	size_t index = 1;
	const size_t lowStart=index;
	size_t lowEnd=0;
	size_t highStart=0;
	size_t highEnd=0;
	if(p[index]==_T('-') || p[index]==_T('+'))
	{
		index++; //skip sign
	}
	//scan buffer
	int stage= 0;  //0 低值，1 低值结束，2 高值起始，3 高值结束
	while(index < buf_len)
	{
		switch (stage)
		{
		case 0:
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index]==_T(' ')) //标记低值结束, 转到state 1 跳过空格并期望第一部分结束符','
			{
				lowEnd = index;
				stage =1;
				index ++;
				continue;
			}
			if(p[index]==_T(',')) //结束第一部分，直接转入扫描第二部分 stage 2
			{
				lowEnd=index;
				stage = 2;
				index ++;
				continue;
			}
			return false;
		case 1:  
			if(p[index]==_T(' ')) //跳过第一部分后部空格
			{
				index++;
				continue;
			}
			if(p[index]==_T(',')) //转入第二部分扫描判断
			{
				stage = 2;
				index ++;
				continue;
			}
			return false;
		case 2:
			if(p[index]==_T(' ')) //跳过第二部分前导空格
			{
				index ++;
				continue;
			}
			if(p[index]==_T('+') || p[index]==_T('-')) //标记第二部分起始位置，进入stage 3 并期望数字
			{
				highStart = index;
				stage = 3;
				index ++;
				continue;
			}
			if(p[index] >= _T('0') && p[index] <= _T('9')) //无符号，直接进入stage 3
			{
				highStart=index;
				stage=3;
				index ++;
				continue;
			}
			return false;
		case 3:
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			} 
			highEnd = index; //标记第二部分结束
			break;
		}
		if(highEnd!=0) break; //两部分扫描结束，跳出循环
	}
	//判断两部分起止位置逻辑关系，确定是否成功
	if(lowEnd < lowStart || highEnd < highStart) return false;
	
	//转换
	scope.LowValue = _ttol(p + lowStart);
	scope.HighValue= _ttol(p + highStart);
	return true;
}
ValueScopeInt Settings::GetConfigValueScopeInt(const TCHAR* section, const TCHAR* item, const ValueScopeInt& defaultV)
{
	ValueScopeInt scope;
	ConfigItemImpl * pConfig=findItem(section, item);
	
	if(pConfig)
	{
		const size_t len=_tcslen(pConfig->Value);
		if(len>4 && parseScopeInt(pConfig->Value, len, scope))
		{
			return scope;
		}
	}
	return defaultV;
}
bool parseScopeDouble(const TCHAR* buffer, size_t buf_len, ValueScopeDouble& scope)
{
	if(buffer[0]!=_T('(') || buffer[buf_len-1]!=_T(')')) return false;
	const TCHAR* p=buffer;
	size_t index = 1;
	const size_t lowStart=index;
	size_t lowEnd=0;
	size_t highStart=0;
	size_t highEnd=0;
	if(p[index]==_T('-') || p[index]==_T('+'))
	{
		index++; //skip sign
	}
	//scan buffer
	int stage= 0;  //0 低值整数部分，1 低值小数 2 低值结束， 3 高值起始， 4 高值整数 5高值小数 6 高值结束
	while(index < buf_len)
	{
		switch (stage)
		{
		case 0://扫描低值整数部分
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index]==_T('.'))
			{
				index ++;
				stage = 1;   //扫描小数部分
				continue;
			}
			if(p[index]==_T(' ')) //标记低值扫描结束, 转到扫描低值束符','
			{
				if(lowEnd==0) lowEnd = index;
				stage =2;
				index ++;
				continue;
			}
			if(p[index]==_T(',')) //标记低值扫描结束，直接转入高值扫描
			{
				if(lowEnd==0) lowEnd=index;
				stage = 2;
				index ++;
				continue;
			}
			return false;
		case 1: //扫描低值小数部分
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index]==_T(' ')) //无小数部分，标记低值扫描结束
			{
				lowEnd=index;
				stage = 2;
				index++;
				continue;
			}
			if(p[index]==_T(',')) //转入高值扫描判断
			{
				lowEnd = index;
				stage = 3;
				index ++;
				continue;
			}
			return false;
		case 2: //扫描低值结束符','
			if(p[index] == _T(' ')) //跳过低值后部空格
			{
				index ++;
				continue;
			}
			if(p[index]==_T(',')) //低值结束，扫描高值
			{
				index++;
				stage = 3;
				continue;
			}
			return false;
		case 3: //扫描高值整数起始位置
			if(p[index]==_T(' ')) //跳过高值前导空格
			{
				index ++;
				continue;
			}
			if(p[index]==_T('+') || p[index]==_T('-')) //标记高值扫描起始，进入高值整数部分扫描stage 4
			{
				highStart = index;
				stage = 4;
				index ++;
				continue;
			}
			if(p[index] >= _T('0') && p[index] <= _T('9')) //无符号，标记高值扫描起始，直接进入高值整数部分扫描stage 4
			{
				highStart=index;
				stage=4;
				index ++;
				continue;
			}
			return false;
		case 4: //扫描高值整数
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index] == _T('.')) //扫描高值小数部分
			{
				index++;
				stage = 5;
				continue;
			}
			highEnd = index; //标记高值扫描结束
			break;
		case 5:
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			highEnd = index; //标记高值扫描结束
			break;
		}
		if(highEnd!=0) break; //两部分扫描结束，跳出循环
	}
	//判断两部分起止位置逻辑关系，确定是否成功
	if(lowEnd < lowStart || highEnd < highStart) return false;
	
	//转换
	scope.LowValue = _ttof(p + lowStart);
	scope.HighValue= _ttof(p + highStart);
	return true;
}
ValueScopeDouble Settings::GetConfigValueScopeDouble(const TCHAR* section, const TCHAR* item,
	const ValueScopeDouble& defaultV)
{
	ValueScopeDouble scope;
	ConfigItemImpl * pConfig=findItem(section, item);
	
	if(pConfig)
	{
		const size_t len=_tcslen(pConfig->Value);
		if(len>4 && parseScopeDouble(pConfig->Value, len, scope))
		{
			return scope;
		}
	}
	return defaultV;
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
	return GetConfigColor(SECTION_UI, ITEM_GRID_THIN_COLOR, DEFAULT_GRID_THIN_COLOR);
}

int Settings::getGridThinLineWidth()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_THIN_WIDTH, DEFAULT_GRID_THIN_WIDTH);
}

Gdiplus::Color Settings::getGridThickLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_THICK_COLOR, DEFAULT_GRID_THICK_COLOR);
}

int Settings::getGridThickLineWidth()
{
	return GetConfigColor(SECTION_UI, ITEM_GRID_THICK_WIDTH, DEFAULT_GRID_THICK_WIDTH);
}

Gdiplus::Color Settings::getWaveBaseLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_WAVE_BASELINE_COLOR, DEFAULT_WAVE_BASELINE_COLOR);
}

int Settings::getWaveBaseLineWidth()
{
	return GetConfigColor(SECTION_UI, ITEM_WAVE_BASELINE_WIDTH, DEFAULT_WAVE_BASELINE_WIDTH);
}

Gdiplus::Color Settings::getWaveLineColor()
{
	return GetConfigColor(SECTION_UI, ITEM_WAVE_COLOR, DEFAULT_WAVE_COLOR);
}

int Settings::getWaveLineWidth()
{
	return GetConfigColor(SECTION_UI, ITEM_WAVE_WIDTH, DEFAULT_WAVE_WIDTH);
}

bool Settings::isShowBaseLine()
{
	const TCHAR c=GetConfigChar(SECTION_UI, ITEM_WAVE_BASELINE_SHOW, DEFAULT_WAVE_BASELINE_SHOW);
	return c==_T('是')||c==_T('y')||c==_T('Y')||c==_T('T') || c==_T('t')|| c==_T('1');
}

int Settings::getWaveShowType()
{
	return _tcscmp(_T("擦除"),GetConfigString(SECTION_UI, ITEM_WAVE_SHOW_TYPE, DEFAULT_WAVE_SHOW_TYPE)) == 0 ?
		2 : 1;
}

double Settings::getWaveVelocity()
{
	return GetConfigDouble(SECTION_UI, ITEM_WAVE_VELOCITY, DEFAULT_WAVE_VELOCITY);
}


