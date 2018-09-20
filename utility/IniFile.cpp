#include "stdafx.h"
#include "IniFile.h"
#include "utility.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int CIniFile::IsSection(const TCHAR *buf)
{
	size_t i;
	if(buf[0]==_T('['))
	{
		for(i=1; i< _tcslen(buf); i++)
		{
			if(buf[i]==_T(']'))
				return 1;
		}
	}
	return 0;
}
int CIniFile::IsComment(const TCHAR *buf)
{
	if(buf[0]==_T(';'))
		return 1;
	return 0;
}
void CIniFile::PhaseSection(const TCHAR *buf, SECTION &section)
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

long CIniFile::PhaseItem(const TCHAR* buf, ITEM &item)
{
	const TCHAR* start;
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
		start=end+1;
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

SECTION *CIniFile::FindSection(const TCHAR *SectionName)
{
	SECTION *psec;
	size_t i;
	for(i=0; i<m_IniItems.size(); i++)
	{
		psec=dynamic_cast<SECTION *>(m_IniItems[i]);
		if(psec)
		{
			if(!_tcsicmp(SectionName, psec->SectionName))
				return psec;
		}
	}
	return NULL;
}

const TCHAR* CIniFile::GetSectionComment(const TCHAR *Section)
{
	SECTION *psec=FindSection(Section);
	if(psec)
	{
		psec->Comment;
	}
	return _T("");
}

const TCHAR* CIniFile::GetItemComment(const TCHAR *Section, const TCHAR *itemName)
{
	ITEM *item=FindItem(Section, itemName);
	if(item)
	{
		return item->Comment;
	}
	return _T("");
}
long CIniFile::OpenIniFile(const TCHAR *IniFileName)
{
	
	_tsetlocale(0, _T("chs"));
	SECTION *section, *CurSec;
	ITEM *item;
	COMMENT *comment;

	TCHAR buf[LEN_CFG_MAX_STRING+1];
	FILE *fp;
	int NewSection=1;    
	if(_tfopen_s(&fp, IniFileName, _T("r"))!=0)
	{
		return -1;
	}
	try
	{
		while(!feof(fp))
		{
			if(_fgetts(buf, LEN_CFG_MAX_STRING, fp)==NULL)
				break; 
			Trim(buf);
			if(_tcslen(buf)==0)
			{
				continue;
			}
			if(IsComment(buf))
			{
				comment=new COMMENT;
				_tcscpy_s(comment->Comment, buf);
				m_IniItems.push_back(comment);
			}
			else if(IsSection(buf))
			{
				section=new SECTION();
				PhaseSection(buf, *section);
				UpperString(section->SectionName);
				CurSec=FindSection(section->SectionName);
				if(!CurSec)
				{
					m_IniItems.push_back(section);
					CurSec=section;
				}
				else
				{
					delete section;
				}
			}
			else
			{
				item=new ITEM();
				if(!item)
				{
					throw -1;
				}
				if(PhaseItem(buf, *item))
				{
					CurSec->Items.push_back(item);
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
		RemoveAll();
		fclose(fp);
		return e;
	}
	fclose(fp);
	return 0;
}

ITEM *CIniFile::FindItem(const TCHAR *Section, const TCHAR *ItemName)
{
	size_t i;
	size_t count;
	SECTION *section;
	ITEM *item;
	section=FindSection(Section);
	if(section)
	{
		count=section->Items.size();
		for(i=0; i<count; i++)
		{
			item=dynamic_cast<ITEM*>(section->Items[i]);
			if(item &&
				_tcsicmp(item->ItemName, ItemName)==0)
		{
				return item;
			}
		}
	}
	return NULL;
}
TCHAR CIniFile::GetConfigChar(const TCHAR *Section, const TCHAR *ItemName, TCHAR defaultV)
{
	ITEM *item=FindItem(Section, ItemName);
	if(item && item->Value[0]!=0)
	{
		return item->Value[0];
	}
	return defaultV;
}

long CIniFile::GetConfigInt(const TCHAR *Section, const TCHAR *ItemName, long defaultV)
{
	ITEM *item=FindItem(Section, ItemName);
	if(item && item->Value[0]!=0 )
	{
		return _tstol(item->Value);
	}
	return defaultV;
}

const TCHAR *CIniFile::GetConfigString(const TCHAR *Section, const TCHAR *ItemName, const TCHAR *defaultV, TCHAR *buffer, long buflen)
{
	ITEM *pConfig;
	pConfig=FindItem(Section, ItemName);
	if(pConfig && pConfig->Value[0]!=0)
	{
		if(buffer && buflen >1)
		{
			_tcsncpy_s(buffer,buflen, pConfig->Value, buflen-1);
			buffer[buflen-1]=0;
		}
		return pConfig->Value;
	}
	if(buffer && buflen >1)
	{
		_tcsncpy_s(buffer,buflen, defaultV?defaultV:_T(""), buflen-1);
		buffer[buflen-1]=0;
	}
	return defaultV;
}

double CIniFile::GetConfigDouble(const TCHAR *Section, const TCHAR *ItemName, double defaultV)
{
	ITEM *pConfig;
	pConfig=FindItem(Section, ItemName);
	if(pConfig && pConfig->Value[0]!=0)
	{
		return _tstol(pConfig->Value);
	}
	return defaultV;
}

long CIniFile::GetConfigSection(TCHAR *buffer, long buflen)
{
	SECTION *section;
	TCHAR *p, *old;
	int i;
	int len=0;
	int lineCount;

	lineCount=m_IniItems.size();
	for(i=0; i<lineCount; i++)
	{
		section=dynamic_cast<SECTION*>(m_IniItems[i]);
		if(section)
		{
			len+=_tcslen(section->SectionName)+1;
		}
	}
	len++;
	if(buffer==NULL || buflen==0)
		return len;

	buffer[0]=0;
	old=p=buffer;

	lineCount=m_IniItems.size();
	for(i=0; i<lineCount; i++)
	{
		section=dynamic_cast<SECTION*>(m_IniItems[i]);
		if(section)
		{
			if(p+_tcslen(section->SectionName)+1-buffer<buflen)
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
long CIniFile::GetConfigItem(const TCHAR *section, TCHAR *buffer, long buflen)
{
	SECTION *sec;
	ITEM* item;
	TCHAR *p, *old;
	size_t i;
	int len=0;

	//get needed buffer len
	sec=FindSection(section);
	if(!sec)
	{
		return 0;
	}

	for(i=0; i<sec->Items.size(); i++)
	{
		item=dynamic_cast<ITEM*>(sec->Items[i]);
		if(item)
		{
			len+=_tcslen(item->ItemName)+1;
		}
	}
	len++;
	if(buffer==NULL || buflen==0)
		return len;
	buffer[0]=0;
	old=p=buffer;

	sec=FindSection(section);
	if(!sec)
	{
		return 0;
	}

	for(i=0; i<sec->Items.size(); i++)
	{
		item=dynamic_cast<ITEM*>(sec->Items[i]);
		if(item)
		{
			if(p+_tcslen(item->ItemName)+1-buffer<buflen)
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

long CIniFile::AddConfig(const TCHAR*Section, const TCHAR*ItemName, const TCHAR *Value)
{
	SECTION *sec;
	ITEM *item;
	if(Section==NULL ||Section[0]==0)
		return -2;
	item=FindItem(Section, ItemName);
	if(item)
		return -1;
	sec=FindSection(Section);
	if(!sec)
	{
		sec=new SECTION;
		_tcscpy(sec->SectionName, Section);
		m_IniItems.push_back(sec);
	}
	if(ItemName==NULL ||ItemName[0]==0)
		return 0;
	item=new ITEM;
	_tcscpy(item->ItemName, ItemName);
	_tcscpy(item->Value, Value);
	sec->Items.push_back(item);
	return 0;
}

long CIniFile::ModifyConfig(const TCHAR*Section, const TCHAR*ItemName, const TCHAR *NewValue)
{
	ITEM *pConfig;
	pConfig=FindItem(Section, ItemName);
	if(pConfig)
	{
		_tcscpy(pConfig->Value, NewValue);
		return 0;
	}
	return AddConfig(Section, ItemName, NewValue);
}

long CIniFile::DeleteConfigItem(const TCHAR* Section, const TCHAR *ItemName)
{
	SECTION *sec;
	ITEM *item;
	size_t i;
	DumpIni(stdout);
	sec=FindSection(Section);
	if(sec)
	{
		for(i=0; i<sec->Items.size(); i++)
		{
			item=dynamic_cast<ITEM*>(sec->Items[i]);

			if(item &&
				_tcsicmp(item->ItemName, ItemName)==0)
			{
				sec->Items.erase(sec->Items.begin()+i);
				DumpIni(stdout);
				return 0;
			}
		}
	}
	return -1;    
}

long CIniFile::DeleteConfigSection(const TCHAR *Section)
{
	SECTION *pSec;

	size_t i, count;
	count=m_IniItems.size();
	for(i=count-1; i>=0; i--)
	{
		pSec=dynamic_cast<SECTION*>(m_IniItems[i]);
		if(pSec &&
			_tcsicmp(Section, pSec->SectionName)==0)

		{
			m_IniItems.erase(m_IniItems.begin()+i);
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
	RemoveAll();
}

void CIniFile::DumpIni(FILE *fp)
{
	size_t i, count;
	count=m_IniItems.size();
	for(i=0; i<count; i++)
	{
		m_IniItems[i]->Dump(fp);
	}
}
long CIniFile::GetSectionNum()
{
	SECTION *section;
	size_t i;
	size_t count=0;
	size_t lineCount;
	lineCount=m_IniItems.size();

	for(i=0; i<lineCount; i++)
	{
		section=dynamic_cast<SECTION*>(m_IniItems[i]);
		if(section)
		{
			count ++;
		}
	}
	return count;
}

