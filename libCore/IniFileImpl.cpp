#include "stdafx.h"
#include <uc/utils/utility.h>
#include "IniFileImpl.h"
#include <algorithm>

#define LEN_ARGB_COLOR_VALUE 9

#define LEN_RGB_COLOR_VALUE 7

namespace uc
{
    namespace utils
    {
        ConfigItem::~ConfigItem()
        {
        }

        class DeletePtr
        {
        public:
            void operator()(ConfigItem* ptr) const
            {
                delete ptr;
            }
        };

        ConfigItemImpl::ConfigItemImpl()
        {
            ItemName[0] = Value[0] = Comment[0] = 0;
            Type = TYPE_ITEM;
        }

        void ConfigItemImpl::Dump(FILE* fp)
        {
            _ftprintf(fp, _T("%s\t=%s\t%s\n"), ItemName, Value, Comment);
        }

        ConfigSection::ConfigSection()
        {
            SectionName[0] = Comment[0] = 0;
            Type = TYPE_SECTION;
        }

        ConfigSection::~ConfigSection()
        {
            std::for_each(Items.begin(), Items.end(), DeletePtr());
            Items.clear();
        }

        void ConfigSection::Dump(FILE* fp)
        {
            _ftprintf(fp, _T("\n[%s]\t%s\n"), SectionName, Comment);
            for (size_t i = 0; i < Items.size(); i++)
            {
                Items[i]->Dump(fp);
            }
        }

        ConfigComment::ConfigComment()
        {
            Comment[0] = 0;
            Type = TYPE_COMMENT;
        }

        void ConfigComment::Dump(FILE* fp)
        {
            _ftprintf(fp, _T("%s\n"), Comment);
        }

        void IniFileImpl::removeAll()
        {
            std::for_each(_iniItems.begin(), _iniItems.end(), DeletePtr());
            _iniItems.clear();
        }

        IniFileImpl::~IniFileImpl()
        {
            std::for_each(_iniItems.begin(), _iniItems.end(), DeletePtr());
            _iniItems.clear();
        }

        int IniFileImpl::isSection(const TCHAR* buf)
        {
            if (buf[0] == _T('['))
            {
                for (size_t i = 1; i < _tcslen(buf); i++)
                {
                    if (buf[i] == _T(']'))
                        return 1;
                }
            }
            return 0;
        }

        int IniFileImpl::isComment(const TCHAR* buf)
        {
            if (buf[0] == _T(';'))
                return 1;
            return 0;
        }

        void IniFileImpl::phaseSection(const TCHAR* buf, ConfigSection& section) const
        {
            const TCHAR* end = buf;
            int found = 0;
            while (*end != _T(']') && *end != _T(';') && *end != 0)
            {
                end++;
                if (*end == _T(']'))
                {
                    found = 1;
                    break;
                }
            }

            if (found && end - buf - 1 > 0)
            {
                _tcsncpy_s(section.SectionName, buf + 1, end - buf - 1);
                section.SectionName[end - buf - 1] = 0;
                Trim(section.SectionName);
                _tcscpy_s(section.Comment, end + 1);
                Trim(section.Comment);
            }
        }

        long IniFileImpl::phaseItem(const TCHAR* buf, ConfigItemImpl& item) const
        {
            const TCHAR* end = buf;
            int found = 0;
            while (*end != _T(';') && *end != 0)
            {
                if (*end == _T('='))
                {
                    found = 1;
                    break;
                }
                end++;
            }

            if (found && end - buf > 0)
            {
                _tcsncpy_s(item.ItemName, buf, end - buf);
                item.ItemName[end - buf] = 0;
                Trim(item.ItemName);
                const TCHAR* start = end + 1;

                while (*end != _T(';') && *end != 0)
                {
                    end++;
                }

                if (end - start > 0)
                {
                    _tcsncpy_s(item.Value, start, end - start);
                    item.Value[end - start] = 0;
                    Trim(item.Value);
                }
                else
                {
                    item.Value[0] = 0;
                }

                if (*end == _T(';'))
                {
                    _tcscpy_s(item.Comment, end);
                }
                return 1;
            }
            return 0;
        }

        ConfigSection* IniFileImpl::findSection(const TCHAR* sectionName)
        {
            for (size_t i = 0; i < _iniItems.size(); i++)
            {
                ConfigSection* pSection = dynamic_cast<ConfigSection *>(_iniItems[i]);

                if (pSection)
                {
                    if (!_tcsicmp(sectionName, pSection->SectionName))
                        return pSection;
                }
            }
            return nullptr;
        }

        const TCHAR* IniFileImpl::GetSectionComment(const TCHAR* section)
        {
            ConfigSection* pSection = findSection(section);

            if (pSection)
            {
                return pSection->Comment;
            }

            return _T("");
        }

        const TCHAR* IniFileImpl::GetItemComment(const TCHAR* section, const TCHAR* itemName)
        {
            ConfigItemImpl* item = findItem(section, itemName);

            if (item)
            {
                return item->Comment;
            }

            return _T("");
        }

        long IniFileImpl::OpenIniFile(const TCHAR* IniFileName)
        {
            _tsetlocale(0, _T("chs"));
            ConfigSection* curSec = nullptr;
            TCHAR buf[LEN_CFG_MAX_STRING + 1];
            FILE* fp;

            int newSection = 1;

            if (_tfopen_s(&fp, IniFileName, _T("r")) != 0)
            {
                return -1;
            }

            try
            {
                while (!feof(fp))
                {
                    if (_fgetts(buf, LEN_CFG_MAX_STRING, fp) == nullptr)
                        break;

                    Trim(buf);

                    if (_tcslen(buf) == 0)
                    {
                        continue;
                    }

                    if (isComment(buf))
                    {
                        ConfigComment* comment = new ConfigComment;
                        _tcscpy_s(comment->Comment, buf);
                        _iniItems.push_back(comment);
                    }
                    else if (isSection(buf))
                    {
                        ConfigSection* section = new ConfigSection();
                        phaseSection(buf, *section);
                        UpperString(section->SectionName);
                        curSec = findSection(section->SectionName);
                        if (!curSec)
                        {
                            _iniItems.push_back(section);
                            curSec = section;
                        }
                        else
                        {
                            delete section;
                        }
                    }
                    else
                    {
                        ConfigItemImpl* item = new ConfigItemImpl();

                        if (!item)
                        {
                            throw -1;
                        }
                        if (phaseItem(buf, *item))
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
            catch (int e)
            {
                removeAll();
                fclose(fp);
                return e;
            }
            fclose(fp);
            return 0;
        }

        ConfigItemImpl* IniFileImpl::findItem(const TCHAR* Section, const TCHAR* ItemName)
        {
            ConfigSection* section = findSection(Section);

            if (section)
            {
                const size_t count = section->Items.size();

                for (size_t i = 0; i < count; i++)
                {
                    ConfigItemImpl* item = dynamic_cast<ConfigItemImpl*>(section->Items[i]);

                    if (item && _tcsicmp(item->ItemName, ItemName) == 0)
                    {
                        return item;
                    }
                }
            }
            return nullptr;
        }

        TCHAR IniFileImpl::GetConfigChar(const TCHAR* section, const TCHAR* itemName, TCHAR defaultV)
        {
            ConfigItemImpl* item = findItem(section, itemName);

            if (item && item->Value[0] != 0)
            {
                return item->Value[0];
            }
            return defaultV;
        }

        long IniFileImpl::GetConfigInt(const TCHAR* section, const TCHAR* itemName, long defaultV)
        {
            ConfigItemImpl* item = findItem(section, itemName);

            if (item && item->Value[0] != 0)
            {
                return _tstol(item->Value);
            }
            return defaultV;
        }

        const TCHAR* IniFileImpl::GetConfigString(const TCHAR* section, const TCHAR* itemName, const TCHAR* defaultV,
                                                  TCHAR* buffer, long bufferLen)

        {
            ConfigItemImpl* pConfig = findItem(section, itemName);

            if (pConfig && pConfig->Value[0] != 0)
            {
                if (buffer && bufferLen > 1)
                {
                    _tcsncpy_s(buffer, bufferLen, pConfig->Value, bufferLen - 1);
                    buffer[bufferLen - 1] = 0;
                }
                return pConfig->Value;
            }

            if (buffer && bufferLen > 1)
            {
                _tcsncpy_s(buffer, bufferLen, defaultV ? defaultV : _T(""), bufferLen - 1);
                buffer[bufferLen - 1] = 0;
            }
            return defaultV;
        }

        double IniFileImpl::GetConfigDouble(const TCHAR* section, const TCHAR* itemName, double defaultV)
        {
            ConfigItemImpl* pConfig = findItem(section, itemName);

            if (pConfig && pConfig->Value[0] != 0)
            {
                return _tstol(pConfig->Value);
            }
            return defaultV;
        }

        bool parseHexToLow4Bits(const TCHAR c, unsigned char& outChar)
        {
            if (c >= _T('0') && c <= _T('9'))
            {
                outChar |= c - _T('0');
                return true;
            }

            if (c >= _T('a') && c <= _T('f'))
            {
                outChar |= 0x0A + c - _T('a');

                return true;
            }

            if (c >= _T('A') && c <= _T('F'))
            {
                outChar |= 0x0A + c - _T('A');

                return true;
            }

            return false;
        }

        bool parseHexChar(const TCHAR* buffer, unsigned char& outChar)
        {
            outChar = 0;

            if (parseHexToLow4Bits(buffer[0], outChar))
            {
                outChar <<= 4;

                return parseHexToLow4Bits(buffer[1], outChar);
            }

            return false;
        }

        unsigned int parseColor(const TCHAR* buffer, unsigned int defaultV)
        {
            if (buffer[0] != _T('#')) return defaultV;

            const size_t len = _tcslen(buffer);

            if (len != LEN_ARGB_COLOR_VALUE && len != LEN_RGB_COLOR_VALUE) return defaultV;

            unsigned char a, r, g, b;

            int index = 0;

            const TCHAR* p = buffer + 1;

            if (len == LEN_RGB_COLOR_VALUE)
            {
                a = 0xFF;
            }
            else
            {
                if (!parseHexChar(p, a)) return defaultV;

                index += 2;
            }

            if (!parseHexChar(p + index, r)) return defaultV;

            if (!parseHexChar(p + index + 2, g)) return defaultV;

            if (!parseHexChar(p + index + 4, b)) return defaultV;

            return (a << 24) | (r << 16) | (g << 8) | b;
        }

        unsigned long IniFileImpl::GetConfigColor(const TCHAR* section, const TCHAR* itemName, unsigned long defaultV)
        {
            ConfigItemImpl* pConfig = findItem(section, itemName);

            if (pConfig && pConfig->Value[0] != 0)
            {
                return parseColor(pConfig->Value, defaultV);
            }

            return defaultV;
        }

        bool IniFileImpl::GetConfigBool(const TCHAR* section, const TCHAR* itemName, bool defaultV)
        {
            TCHAR buffer[20];

            GetConfigString(section, itemName, _T(""), buffer, 20);

            if (defaultV)
            {
                return !(_tcsicmp(buffer, _T("��")) == 0 || _tcsicmp(buffer, _T("no")) == 0 ||
                    _tcsicmp(buffer, _T("false")) == 0 || _tcsicmp(buffer, _T("0")) == 0);
            }

            return (_tcsicmp(buffer, _T("��")) == 0 || _tcsicmp(buffer, _T("yes")) == 0 || _tcsicmp(buffer, _T("true"))
                == 0 || _tcsicmp(buffer, _T("1")) == 0);
        }

        long IniFileImpl::GetConfigSection(TCHAR* buffer, long bufferLen)
        {
            ConfigSection* section;

            TCHAR* p;

            int i;

            int len = 0;

            int lineCount = _iniItems.size();

            for (i = 0; i < lineCount; i++)
            {
                section = dynamic_cast<ConfigSection*>(_iniItems[i]);

                if (section)
                {
                    len += _tcslen(section->SectionName) + 1;
                }
            }

            len++;

            if (buffer == nullptr || bufferLen == 0)
                return len;

            buffer[0] = 0;

            TCHAR* old = p = buffer;

            lineCount = _iniItems.size();

            for (i = 0; i < lineCount; i++)
            {
                section = dynamic_cast<ConfigSection*>(_iniItems[i]);

                if (section)
                {
                    if (p + _tcslen(section->SectionName) + 1 - buffer < bufferLen)
                    {
                        old = p;

                        _tcscpy(p, section->SectionName);

                        p += (_tcslen(p) + 1);
                    }
                    else
                    {
                        break;
                    }
                }
            }

            *p = 0;

            return len + 1;
        }

        long IniFileImpl::GetConfigItem(const TCHAR* section, TCHAR* buffer, long bufferLen)
        {
            ConfigItemImpl* item;

            TCHAR* p;

            size_t i;

            int len = 0;

            //get needed buffer len

            ConfigSection* sec = findSection(section);

            if (!sec)
            {
                return 0;
            }

            for (i = 0; i < sec->Items.size(); i++)
            {
                item = dynamic_cast<ConfigItemImpl*>(sec->Items[i]);

                if (item)
                {
                    len += _tcslen(item->ItemName) + 1;
                }
            }

            len++;

            if (buffer == nullptr || bufferLen == 0)
                return len;

            buffer[0] = 0;

            TCHAR* old = p = buffer;

            sec = findSection(section);

            if (!sec)
            {
                return 0;
            }

            for (i = 0; i < sec->Items.size(); i++)
            {
                item = dynamic_cast<ConfigItemImpl*>(sec->Items[i]);

                if (item)
                {
                    if (p + _tcslen(item->ItemName) + 1 - buffer < bufferLen)
                    {
                        _tcscpy(p, item->ItemName);

                        p += (_tcslen(p) + 1);
                    }
                    else
                    {
                        break;
                    }
                }
            }

            *p = 0;

            return len + 1;
        }

        long IniFileImpl::AddConfig(const TCHAR* section, const TCHAR* itemName, const TCHAR* value)
        {
            if (section == nullptr || section[0] == 0)
                return -2;

            ConfigItemImpl* item = findItem(section, itemName);

            if (item)
                return -1;

            ConfigSection* sec = findSection(section);

            if (!sec)
            {
                sec = new ConfigSection;

                _tcscpy(sec->SectionName, section);

                _iniItems.push_back(sec);
            }

            if (itemName == nullptr || itemName[0] == 0)
                return 0;

            item = new ConfigItemImpl;

            _tcscpy(item->ItemName, itemName);

            _tcscpy(item->Value, value);

            sec->Items.push_back(item);

            return 0;
        }

        long IniFileImpl::ModifyConfig(const TCHAR* section, const TCHAR* itemName, const TCHAR* newValue)
        {
            ConfigItemImpl* pConfig = findItem(section, itemName);

            if (pConfig)
            {
                _tcscpy(pConfig->Value, newValue);
                return 0;
            }

            return AddConfig(section, itemName, newValue);
        }

        long IniFileImpl::DeleteConfigItem(const TCHAR* section, const TCHAR* itemName)
        {
            DumpIni(stdout);

            ConfigSection* sec = findSection(section);

            if (sec)
            {
                for (size_t i = 0; i < sec->Items.size(); i++)
                {
                    ConfigItemImpl* item = dynamic_cast<ConfigItemImpl*>(sec->Items[i]);

                    if (item &&

                        _tcsicmp(item->ItemName, itemName) == 0)
                    {
                        sec->Items.erase(sec->Items.begin() + i);

                        DumpIni(stdout);

                        return 0;
                    }
                }
            }

            return -1;
        }

        long IniFileImpl::DeleteConfigSection(const TCHAR* section)
        {
            const size_t count = _iniItems.size();

            for (size_t i = count - 1; i >= 0; i--)
            {
                ConfigSection* pSec = dynamic_cast<ConfigSection*>(_iniItems[i]);
                
                if (pSec &&
                    _tcsicmp(section, pSec->SectionName) == 0)
                {
                    _iniItems.erase(_iniItems.begin() + i);
                }
            }

            return 0;
        }

        long IniFileImpl::WriteIniFile(const TCHAR* IniFileName)
        {
            FILE* fp;

            if (_tfopen_s(&fp, IniFileName, _T("w")) == 0)
            {
                DumpIni(fp);
                fclose(fp);
                return 0;
            }

            return -1;
        }

        void IniFileImpl::Release()
        {
            removeAll();
            delete this;
        }

        void IniFileImpl::DumpIni(FILE* fp)
        {
            const size_t count = _iniItems.size();

            for (size_t i = 0; i < count; i++)
            {
                _iniItems[i]->Dump(fp);
            }
        }

        long IniFileImpl::GetSectionNum()
        {
            size_t count = 0;

            const size_t lineCount = _iniItems.size();

            for (size_t i = 0; i < lineCount; i++)
            {
                ConfigSection* section = dynamic_cast<ConfigSection*>(_iniItems[i]);

                if (section)
                {
                    count ++;
                }
            }

            return count;
        }

        static bool parseScopeInt(const TCHAR* buffer, size_t buf_len, ValueScopeInt& scope)
        {
            if (buffer[0] != _T('(') || buffer[buf_len - 1] != _T(')')) return false;

            const TCHAR* p = buffer;
            size_t index = 1;
            const size_t lowStart = index;
            size_t lowEnd = 0;
            size_t highStart = 0;
            size_t highEnd = 0;

            if (p[index] == _T('-') || p[index] == _T('+'))
            {
                index++; //skip sign
            }

            //scan buffer

            int stage = 0;

            while (index < buf_len)
            {
                switch (stage)
                {
                case 0:
                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        index++;
                        continue;
                    }
                    if (p[index] == _T(' '))
                    {
                        lowEnd = index;
                        stage = 1;
                        index ++;
                        continue;
                    }
                    if (p[index] == _T(','))
                    {
                        lowEnd = index;
                        stage = 2;
                        index ++;
                        continue;
                    }
                    return false;
                case 1:
                    if (p[index] == _T(' '))
                    {
                        index++;
                        continue;
                    }
                    if (p[index] == _T(','))
                    {
                        stage = 2;
                        index ++;
                        continue;
                    }
                    return false;
                case 2:
                    if (p[index] == _T(' ')) 
                    {
                        index ++;
                        continue;
                    }
                    if (p[index] == _T('+') || p[index] == _T('-')) 
                    {
                        highStart = index;
                        stage = 3;
                        index ++;
                        continue;
                    }
                    if (p[index] >= _T('0') && p[index] <= _T('9')) 
                    {
                        highStart = index;
                        stage = 3;
                        index ++;
                        continue;
                    }
                    return false;
                case 3:
                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        index++;
                        continue;
                    }
                    highEnd = index; 
                    break;
                }

                if (highEnd != 0) break;
            }
            
            if (lowEnd < lowStart || highEnd < highStart) return false;

            scope.LowValue = _ttol(p + lowStart);

            scope.HighValue = _ttol(p + highStart);

            return true;
        }

        ValueScopeInt IniFileImpl::GetConfigValueScopeInt(const TCHAR* section, const TCHAR* item,
                                                          const ValueScopeInt& defaultV)
        {
            ValueScopeInt scope;
            ConfigItemImpl* pConfig = findItem(section, item);

            if (pConfig)
            {
                const size_t len = _tcslen(pConfig->Value);

                if (len > 4 && parseScopeInt(pConfig->Value, len, scope))
                {
                    return scope;
                }
            }

            return defaultV;
        }

        static bool parseScopeDouble(const TCHAR* buffer, size_t buf_len, ValueScopeDouble& scope)
        {
            if (buffer[0] != _T('(') || buffer[buf_len - 1] != _T(')')) return false;

            const TCHAR* p = buffer;
            size_t index = 1;
            const size_t lowStart = index;
            size_t lowEnd = 0;
            size_t highStart = 0;
            size_t highEnd = 0;

            if (p[index] == _T('-') || p[index] == _T('+'))
            {
                index++; //skip sign
            }

            //scan buffer
            int stage = 0; 

            while (index < buf_len)
            {
                switch (stage)
                {
                case 0: 
                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        index++;
                        continue;
                    }

                    if (p[index] == _T('.'))
                    {
                        index ++;
                        stage = 1; 
                        continue;
                    }

                    if (p[index] == _T(' ')) 
                    {
                        if (lowEnd == 0) lowEnd = index;
                        stage = 2;
                        index ++;
                        continue;
                    }

                    if (p[index] == _T(',')) 
                    {
                        if (lowEnd == 0) lowEnd = index;

                        stage = 2;
                        index ++;
                        continue;
                    }
                    return false;

                case 1: 
                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        index++;
                        continue;
                    }

                    if (p[index] == _T(' '))
                    {
                        lowEnd = index;
                        stage = 2;
                        index++;
                        continue;
                    }

                    if (p[index] == _T(',')) 
                    {
                        lowEnd = index;
                        stage = 3;
                        index ++;
                        continue;
                    }

                    return false;

                case 2:
                    if (p[index] == _T(' ')) 
                    {
                        index ++;
                        continue;
                    }

                    if (p[index] == _T(',')) 
                    {
                        index++;
                        stage = 3;
                        continue;
                    }
                    return false;

                case 3: 
                    if (p[index] == _T(' ')) 
                    {
                        index ++;
                        continue;
                    }

                    if (p[index] == _T('+') || p[index] == _T('-')) 
                    {
                        highStart = index;
                        stage = 4;
                        index ++;
                        continue;
                    }

                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        highStart = index;
                        stage = 4;
                        index ++;
                        continue;
                    }

                    return false;

                case 4: 
                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        index++;
                        continue;
                    }

                    if (p[index] == _T('.')) 
                    {
                        index++;
                        stage = 5;
                        continue;
                    }

                    highEnd = index; 
                    break;

                case 5:

                    if (p[index] >= _T('0') && p[index] <= _T('9'))
                    {
                        index++;
                        continue;
                    }

                    highEnd = index; 
                    break;
                }

                if (highEnd != 0) break; 
            }

            if (lowEnd < lowStart || highEnd < highStart) return false;

            scope.LowValue = _ttof(p + lowStart);
            scope.HighValue = _ttof(p + highStart);

            return true;
        }

        ValueScopeDouble IniFileImpl::GetConfigValueScopeDouble(const TCHAR* section, const TCHAR* item,
                                                                const ValueScopeDouble& defaultV)
        {
            ValueScopeDouble scope;
            ConfigItemImpl* pConfig = findItem(section, item);

            if (pConfig)
            {
                const size_t len = _tcslen(pConfig->Value);

                if (len > 4 && parseScopeDouble(pConfig->Value, len, scope))
                {
                    return scope;
                }
            }
            return defaultV;
        }
    }
}
