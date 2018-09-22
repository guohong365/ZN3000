#include "stdafx.h"
#include "IniFile.h"
#include "utility.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LEN_ARGB_COLOR_VALUE 9
#define LEN_RGB_COLOR_VALUE 7

int CIniFile::isSection(const TCHAR *buf)
{
	if(buf[0]==_T('['))
	{
		for(size_t i = 1; i< _tcslen(buf); i++)
		{
			if(buf[i]==_T(']'))
				return 1;
		}
	}
	return 0;
}
int CIniFile::isComment(const TCHAR *buf)
{
	if(buf[0]==_T(';'))
		return 1;
	return 0;
}
void CIniFile::phaseSection(const TCHAR *buf, ConfigSection &section) const
{
	const TCHAR *end=buf;
	int found=0;
	while(*end!=_T(']') && *end!=_T(';') && *end!=0)
	{
		end++;
		if(*end==_T(']'))
		{
			found=1;
			break;
		}
	}

	if(found && end-buf-1>0)
	{

		_tcsncpy_s(section.SectionName, buf+1, end-buf-1);
		section.SectionName[end-buf-1]=0;
		Trim(section.SectionName);
		_tcscpy_s(section.Comment, end+1);
		Trim(section.Comment);
	}
}

long CIniFile::phaseItem(const TCHAR* buf, ConfigItemImpl &item) const
{
	const TCHAR* end=buf;
	int found=0;
	while(*end!=_T(';') && *end!=0)
	{
		if(*end==_T('='))
		{
			found=1;
			break;
		}
		end++;
	}
	if(found && end-buf>0)
	{
		_tcsncpy_s(item.ItemName, buf, end-buf);
		item.ItemName[end-buf]=0;
		Trim(item.ItemName);
		const TCHAR* start = end + 1;
		while(*end!=_T(';')&& *end!=0)
		{
			end++;
		}
		if(end-start>0)
		{
			_tcsncpy_s(item.Value, start, end-start);
			item.Value[end-start]=0;
			Trim(item.Value);
		}
		else
		{
			item.Value[0]=0;
		}
		if(*end==_T(';'))
		{
			_tcscpy_s(item.Comment, end);
		}
		return 1;
	}
	return 0;
}

ConfigSection *CIniFile::findSection(const TCHAR *sectionName)
{
	for(size_t i = 0; i<_iniItems.size(); i++)
	{
		ConfigSection* pSection = dynamic_cast<ConfigSection *>(_iniItems[i]);
		if(pSection)
		{
			if(!_tcsicmp(sectionName, pSection->SectionName))
				return pSection;
		}
	}
	return nullptr;
}

const TCHAR* CIniFile::GetSectionComment(const TCHAR *section)
{
	ConfigSection *pSection=findSection(section);
	if(pSection)
	{
		return pSection->Comment;
	}
	return _T("");
}

const TCHAR* CIniFile::GetItemComment(const TCHAR *section, const TCHAR *itemName)
{
	ConfigItemImpl *item=findItem(section, itemName);
	if(item)
	{
		return item->Comment;
	}
	return _T("");
}
long CIniFile::OpenIniFile(const TCHAR *IniFileName)
{
	
	_tsetlocale(0, _T("chs"));
	ConfigSection*curSec = nullptr;

	TCHAR buf[LEN_CFG_MAX_STRING+1];
	FILE *fp;
	int newSection=1;    
	if(_tfopen_s(&fp, IniFileName, _T("r"))!=0)
	{
		return -1;
	}
	try
	{
		while(!feof(fp))
		{
			if(_fgetts(buf, LEN_CFG_MAX_STRING, fp)== nullptr)
				break; 
			Trim(buf);
			if(_tcslen(buf)==0)
			{
				continue;
			}
			if(isComment(buf))
			{
				ConfigComment* comment = new ConfigComment;
				_tcscpy_s(comment->Comment, buf);
				_iniItems.push_back(comment);
			}
			else if(isSection(buf))
			{
				ConfigSection* section = new ConfigSection();
				phaseSection(buf, *section);
				UpperString(section->SectionName);
				curSec=findSection(section->SectionName);
				if(!curSec)
				{
					_iniItems.push_back(section);
					curSec=section;
				}
				else
				{
					delete section;
				}
			}
			else
			{
				ConfigItemImpl* item = new ConfigItemImpl();
				if(!item)
				{
					throw -1;
				}
				if(phaseItem(buf, *item))
				{
					curSec->Items.push_back(item);
				}
				else
				{
					delete item;
				}
			}
		}
	}
	catch( int e)
	{
		removeAll();
		fclose(fp);
		return e;
	}
	fclose(fp);
	return 0;
}

ConfigItemImpl *CIniFile::findItem(const TCHAR *Section, const TCHAR *ItemName)
{
	ConfigSection* section = findSection(Section);
	if(section)
	{
		const size_t count = section->Items.size();
		for(size_t i = 0; i<count; i++)
		{
			ConfigItemImpl* item = dynamic_cast<ConfigItemImpl*>(section->Items[i]);
			if(item &&
				_tcsicmp(item->ItemName, ItemName)==0)
		{
				return item;
			}
		}
	}
	return nullptr;
}
TCHAR CIniFile::GetConfigChar(const TCHAR *section, const TCHAR *itemName, TCHAR defaultV)
{
	ConfigItemImpl *item=findItem(section, itemName);
	if(item && item->Value[0]!=0)
	{
		return item->Value[0];
	}
	return defaultV;
}

long CIniFile::GetConfigInt(const TCHAR *section, const TCHAR *itemName, long defaultV)
{
	ConfigItemImpl *item=findItem(section, itemName);
	if(item && item->Value[0]!=0 )
	{
		return _tstol(item->Value);
	}
	return defaultV;
}

const TCHAR *CIniFile::GetConfigString(const TCHAR *section, const TCHAR *itemName, const TCHAR *defaultV, TCHAR *buffer, long bufferLen)
{
	ConfigItemImpl* pConfig = findItem(section, itemName);
	if(pConfig && pConfig->Value[0]!=0)
	{
		if(buffer && bufferLen >1)
		{
			_tcsncpy_s(buffer,bufferLen, pConfig->Value, bufferLen-1);
			buffer[bufferLen-1]=0;
		}
		return pConfig->Value;
	}
	if(buffer && bufferLen >1)
	{
		_tcsncpy_s(buffer,bufferLen, defaultV?defaultV:_T(""), bufferLen-1);
		buffer[bufferLen-1]=0;
	}
	return defaultV;
}

double CIniFile::GetConfigDouble(const TCHAR *section, const TCHAR *itemName, double defaultV)
{
	ConfigItemImpl* pConfig = findItem(section, itemName);
	if(pConfig && pConfig->Value[0]!=0)
	{
		return _tstol(pConfig->Value);
	}
	return defaultV;
}
bool parseHexToLow4Bits(const TCHAR c, unsigned char &outChar)
{
	if(c >= _T('0') && c <= _T('9'))
	{
		outChar |= c-_T('0');
		return true;
	}
	if(c >= _T('a') && c <= _T('f'))
	{
		outChar |= 0x0A + c - _T('a');
		return true;
	}
	if(c >= _T('A') && c <= _T('F'))
	{
		outChar |= 0x0A + c - _T('A');
		return true;
	}
	return false;
}
bool parseHexChar(const TCHAR*buffer, unsigned char &outChar)
{
	if(parseHexToLow4Bits(buffer[0], outChar))
	{
		outChar <<= 4;
		return parseHexToLow4Bits(buffer[1], outChar);
	}
	return false;	
}
unsigned int parseColor(const TCHAR* buffer, unsigned int defaultV)
{
	if(buffer[0]!=_T('#')) return defaultV;
	const size_t len=_tcslen(buffer);
	if(len!= LEN_ARGB_COLOR_VALUE && len!=LEN_RGB_COLOR_VALUE) return defaultV;
	unsigned char a, r, g, b;
	int index=0;
	if(len==LEN_RGB_COLOR_VALUE)
	{
		a=0xFF;
	}
	else
	{
		if(!parseHexChar(buffer, a)) return defaultV;
		index += 2;
	}
	if(!parseHexChar(buffer + index, r)) return defaultV;
	if(!parseHexChar(buffer + index + 2, g)) return defaultV;
	if(!parseHexChar(buffer + index + 4, b)) return defaultV;
	return (a << 24)|(r << 16)|(g << 8)|b;
}
unsigned long CIniFile::GetConfigColor(const TCHAR* section, const TCHAR* itemName, unsigned long defaultV)
{
	ConfigItemImpl*pConfig=findItem(section, itemName);
	if(pConfig && pConfig->Value[0]!=0)
	{
		return parseColor(pConfig->Value, defaultV);
	}
	return defaultV;
}

long CIniFile::GetConfigSection(TCHAR *buffer, long bufferLen)
{
	ConfigSection *section;
	TCHAR *p;
	int i;
	int len=0;

	int lineCount = _iniItems.size();
	for(i=0; i<lineCount; i++)
	{
		section=dynamic_cast<ConfigSection*>(_iniItems[i]);
		if(section)
		{
			len+=_tcslen(section->SectionName)+1;
		}
	}
	len++;
	if(buffer== nullptr || bufferLen==0)
		return len;

	buffer[0]=0;
	TCHAR* old = p = buffer;

	lineCount=_iniItems.size();
	for(i=0; i<lineCount; i++)
	{
		section=dynamic_cast<ConfigSection*>(_iniItems[i]);
		if(section)
		{
			if(p+_tcslen(section->SectionName)+1-buffer<bufferLen)
			{
				old=p;
				_tcscpy(p, section->SectionName);
				p+=(_tcslen(p)+1);

			}
			else
			{
				break;
			}
		}
	}
	*p=0;
	return len+1;
}
long CIniFile::GetConfigItem(const TCHAR *section, TCHAR *buffer, long bufferLen)
{
	ConfigItemImpl* item;
	TCHAR *p;
	size_t i;
	int len=0;

	//get needed buffer len
	ConfigSection* sec = findSection(section);
	if(!sec)
	{
		return 0;
	}

	for(i=0; i<sec->Items.size(); i++)
	{
		item=dynamic_cast<ConfigItemImpl*>(sec->Items[i]);
		if(item)
		{
			len+=_tcslen(item->ItemName)+1;
		}
	}
	len++;
	if(buffer== nullptr || bufferLen==0)
		return len;
	buffer[0]=0;
	TCHAR* old = p = buffer;

	sec=findSection(section);
	if(!sec)
	{
		return 0;
	}

	for(i=0; i<sec->Items.size(); i++)
	{
		item=dynamic_cast<ConfigItemImpl*>(sec->Items[i]);
		if(item)
		{
			if(p+_tcslen(item->ItemName)+1-buffer<bufferLen)
			{
				_tcscpy(p, item->ItemName);
				p+=(_tcslen(p)+1);
			}
			else
			{
				break;
			}
		}
	}
	*p=0;
	return len+1;
}

long CIniFile::AddConfig(const TCHAR*section, const TCHAR*itemName, const TCHAR *value)
{
	if(section== nullptr ||section[0]==0)
		return -2;
	ConfigItemImpl* item = findItem(section, itemName);
	if(item)
		return -1;
	ConfigSection* sec = findSection(section);
	if(!sec)
	{
		sec=new ConfigSection;
		_tcscpy(sec->SectionName, section);
		_iniItems.push_back(sec);
	}
	if(itemName== nullptr ||itemName[0]==0)
		return 0;
	item=new ConfigItemImpl;
	_tcscpy(item->ItemName, itemName);
	_tcscpy(item->Value, value);
	sec->Items.push_back(item);
	return 0;
}

long CIniFile::ModifyConfig(const TCHAR*section, const TCHAR*itemName, const TCHAR *newValue)
{
	ConfigItemImpl* pConfig = findItem(section, itemName);
	if(pConfig)
	{
		_tcscpy(pConfig->Value, newValue);
		return 0;
	}
	return AddConfig(section, itemName, newValue);
}

long CIniFile::DeleteConfigItem(const TCHAR* section, const TCHAR *itemName)
{
	DumpIni(stdout);
	ConfigSection* sec = findSection(section);
	if(sec)
	{
		for(size_t i = 0; i<sec->Items.size(); i++)
		{
			ConfigItemImpl* item = dynamic_cast<ConfigItemImpl*>(sec->Items[i]);

			if(item &&
				_tcsicmp(item->ItemName, itemName)==0)
			{
				sec->Items.erase(sec->Items.begin()+i);
				DumpIni(stdout);
				return 0;
			}
		}
	}
	return -1;    
}

long CIniFile::DeleteConfigSection(const TCHAR *section)
{
	const size_t count = _iniItems.size();
	for(size_t i = count - 1; i>=0; i--)
	{
		ConfigSection* pSec = dynamic_cast<ConfigSection*>(_iniItems[i]);
		if(pSec &&
			_tcsicmp(section, pSec->SectionName)==0)

		{
			_iniItems.erase(_iniItems.begin()+i);
		}
	}
	return 0;
}

long CIniFile::WriteIniFile(const TCHAR*IniFileName)
{
	FILE *fp;
	if(_tfopen_s(&fp, IniFileName, _T("w"))==0)
	{
		DumpIni(fp);
		fclose(fp);
		return 0;
	}
	return -1;
}

void CIniFile::CloseIniFile()
{
	removeAll();
}

void CIniFile::DumpIni(FILE *fp)
{
	const size_t count = _iniItems.size();
	for(size_t i = 0; i<count; i++)
	{
		_iniItems[i]->Dump(fp);
	}
}
long CIniFile::GetSectionNum()
{
	size_t count=0;
	const size_t lineCount = _iniItems.size();

	for(size_t i = 0; i<lineCount; i++)
	{
		ConfigSection* section = dynamic_cast<ConfigSection*>(_iniItems[i]);
		if(section)
		{
			count ++;
		}
	}
	return count;
}

