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
#define ITEM_GRID_THIN_SPACE _T("����.ϸ.���")
#define DEFAULT_GRID_THIN_SPACE 20
#define ITEM_GRID_THIN_COLOR _T("����.ϸ.��ɫ")
#define DEFAULT_GRID_THIN_COLOR 0xFF00C000
#define ITEM_GRID_THIN_WIDTH _T("����.ϸ.�߿�")
#define DEFAULT_GRID_THIN_WIDTH 1
#define ITEM_GRID_THICK_INTERVAL _T("����.��.���")
#define DEFAULT_GRID_THICK_INTERVAL 5
#define ITEM_GRID_THICK_COLOR _T("����.��.��ɫ")
#define DEFAULT_GRID_THICK_COLOR 0xFF00FF00
#define ITEM_GRID_THICK_WIDTH _T("����.��.�߿�")
#define DEFAULT_GRID_THICK_WIDTH 2
#define ITEM_WAVE_COLOR _T("����.��ɫ")
#define DEFAULT_WAVE_COLOR 0xFFADD8E6
#define ITEM_WAVE_WIDTH _T("����.�߿�")
#define DEFAULT_WAVE_WIDTH 3
#define ITEM_WAVE_SHOW_TYPE _T("����.��ʾ.��ʽ")
#define DEFAULT_WAVE_SHOW_TYPE _T("�ƶ�")
#define ITEM_WAVE_VELOCITY _T("����.��ʾ.�ٶ�")
#define DEFAULT_WAVE_VELOCITY 25
#define ITEM_WAVE_BASELINE_SHOW _T("����.����.��ʾ")
#define DEFAULT_WAVE_BASELINE_SHOW _T('��')
#define ITEM_WAVE_BASELINE_COLOR _T("����.����.��ɫ")
#define DEFAULT_WAVE_BASELINE_COLOR 0xFF00FFFF
#define ITEM_WAVE_BASELINE_WIDTH _T("����.����.�߿�")
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
	int stage= 0;  //0 ��ֵ��1 ��ֵ������2 ��ֵ��ʼ��3 ��ֵ����
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
			if(p[index]==_T(' ')) //��ǵ�ֵ����, ת��state 1 �����ո�������һ���ֽ�����','
			{
				lowEnd = index;
				stage =1;
				index ++;
				continue;
			}
			if(p[index]==_T(',')) //������һ���֣�ֱ��ת��ɨ��ڶ����� stage 2
			{
				lowEnd=index;
				stage = 2;
				index ++;
				continue;
			}
			return false;
		case 1:  
			if(p[index]==_T(' ')) //������һ���ֺ󲿿ո�
			{
				index++;
				continue;
			}
			if(p[index]==_T(',')) //ת��ڶ�����ɨ���ж�
			{
				stage = 2;
				index ++;
				continue;
			}
			return false;
		case 2:
			if(p[index]==_T(' ')) //�����ڶ�����ǰ���ո�
			{
				index ++;
				continue;
			}
			if(p[index]==_T('+') || p[index]==_T('-')) //��ǵڶ�������ʼλ�ã�����stage 3 ����������
			{
				highStart = index;
				stage = 3;
				index ++;
				continue;
			}
			if(p[index] >= _T('0') && p[index] <= _T('9')) //�޷��ţ�ֱ�ӽ���stage 3
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
			highEnd = index; //��ǵڶ����ֽ���
			break;
		}
		if(highEnd!=0) break; //������ɨ�����������ѭ��
	}
	//�ж���������ֹλ���߼���ϵ��ȷ���Ƿ�ɹ�
	if(lowEnd < lowStart || highEnd < highStart) return false;
	
	//ת��
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
	int stage= 0;  //0 ��ֵ�������֣�1 ��ֵС�� 2 ��ֵ������ 3 ��ֵ��ʼ�� 4 ��ֵ���� 5��ֵС�� 6 ��ֵ����
	while(index < buf_len)
	{
		switch (stage)
		{
		case 0://ɨ���ֵ��������
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index]==_T('.'))
			{
				index ++;
				stage = 1;   //ɨ��С������
				continue;
			}
			if(p[index]==_T(' ')) //��ǵ�ֵɨ�����, ת��ɨ���ֵ����','
			{
				if(lowEnd==0) lowEnd = index;
				stage =2;
				index ++;
				continue;
			}
			if(p[index]==_T(',')) //��ǵ�ֵɨ�������ֱ��ת���ֵɨ��
			{
				if(lowEnd==0) lowEnd=index;
				stage = 2;
				index ++;
				continue;
			}
			return false;
		case 1: //ɨ���ֵС������
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index]==_T(' ')) //��С�����֣���ǵ�ֵɨ�����
			{
				lowEnd=index;
				stage = 2;
				index++;
				continue;
			}
			if(p[index]==_T(',')) //ת���ֵɨ���ж�
			{
				lowEnd = index;
				stage = 3;
				index ++;
				continue;
			}
			return false;
		case 2: //ɨ���ֵ������','
			if(p[index] == _T(' ')) //������ֵ�󲿿ո�
			{
				index ++;
				continue;
			}
			if(p[index]==_T(',')) //��ֵ������ɨ���ֵ
			{
				index++;
				stage = 3;
				continue;
			}
			return false;
		case 3: //ɨ���ֵ������ʼλ��
			if(p[index]==_T(' ')) //������ֵǰ���ո�
			{
				index ++;
				continue;
			}
			if(p[index]==_T('+') || p[index]==_T('-')) //��Ǹ�ֵɨ����ʼ�������ֵ��������ɨ��stage 4
			{
				highStart = index;
				stage = 4;
				index ++;
				continue;
			}
			if(p[index] >= _T('0') && p[index] <= _T('9')) //�޷��ţ���Ǹ�ֵɨ����ʼ��ֱ�ӽ����ֵ��������ɨ��stage 4
			{
				highStart=index;
				stage=4;
				index ++;
				continue;
			}
			return false;
		case 4: //ɨ���ֵ����
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			if(p[index] == _T('.')) //ɨ���ֵС������
			{
				index++;
				stage = 5;
				continue;
			}
			highEnd = index; //��Ǹ�ֵɨ�����
			break;
		case 5:
			if(p[index] >= _T('0') && p[index] <= _T('9'))
			{
				index++;
				continue;
			}
			highEnd = index; //��Ǹ�ֵɨ�����
			break;
		}
		if(highEnd!=0) break; //������ɨ�����������ѭ��
	}
	//�ж���������ֹλ���߼���ϵ��ȷ���Ƿ�ɹ�
	if(lowEnd < lowStart || highEnd < highStart) return false;
	
	//ת��
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
	return c==_T('��')||c==_T('y')||c==_T('Y')||c==_T('T') || c==_T('t')|| c==_T('1');
}

int Settings::getWaveShowType()
{
	return _tcscmp(_T("����"),GetConfigString(SECTION_UI, ITEM_WAVE_SHOW_TYPE, DEFAULT_WAVE_SHOW_TYPE)) == 0 ?
		2 : 1;
}

double Settings::getWaveVelocity()
{
	return GetConfigDouble(SECTION_UI, ITEM_WAVE_VELOCITY, DEFAULT_WAVE_VELOCITY);
}


