#ifndef __INI_FILE_IMPL_
#define __INI_FILE_IMPL_
#include <stdio.h>
#include <vector>
#include <tchar.h>
#include <uc/utils/IniFile.h>

#define LEN_CFG_MAX_STRING 1024
#define LEN_CFG_ITEM_NAME 256
#define LEN_CFG_SECTION_NAME 256

namespace uc
{
    namespace utils
    {
        enum ConfigItemType
        {
            TYPE_COMMENT,
            TYPE_SECTION,
            TYPE_ITEM
        };

        struct ConfigItem
        {
            ConfigItemType Type;
            virtual void Dump(FILE* fp) =0;
            virtual ~ConfigItem();
        };


        struct ConfigItemImpl : ConfigItem
        {
            _TCHAR ItemName[LEN_CFG_ITEM_NAME + 1];
            _TCHAR Value[LEN_CFG_MAX_STRING + 1];
            _TCHAR Comment[LEN_CFG_MAX_STRING + 1];
            ConfigItemImpl();
            virtual void Dump(FILE* fp);
        };

        struct ConfigSection : ConfigItem
        {
            _TCHAR SectionName[LEN_CFG_SECTION_NAME + 1];
            _TCHAR Comment[LEN_CFG_MAX_STRING + 1];
            std::vector<ConfigItem *> Items;
            ConfigSection();
            ~ConfigSection();
            virtual void Dump(FILE* fp);
        };

        struct ConfigComment : ConfigItem
        {
            _TCHAR Comment[LEN_CFG_MAX_STRING + 1];
            ConfigComment();
            virtual void Dump(FILE* fp);
        };

        class IniFileImpl : public IniFile
        {
        protected:
            void removeAll();
            std::vector<ConfigItem *> _iniItems;
            static int isSection(const _TCHAR* buf);
            static int isComment(const _TCHAR* buf);
            void phaseSection(const _TCHAR* buf, ConfigSection& section) const;
            long phaseItem(const _TCHAR* buf, ConfigItemImpl& item) const;
            ConfigSection* findSection(const _TCHAR* section);
            ConfigItemImpl* findItem(const _TCHAR* section, const _TCHAR* itemName);
        public:
            virtual const _TCHAR* GetSectionComment(const _TCHAR* section);
            virtual const _TCHAR* GetItemComment(const _TCHAR* section, const _TCHAR* itemName);
            virtual _TCHAR GetConfigChar(const _TCHAR* section, const _TCHAR* itemName, _TCHAR defaultV);
            virtual long GetConfigInt(const _TCHAR* section, const _TCHAR* itemName, long defaultV);
            virtual const _TCHAR* GetConfigString(const _TCHAR* section, const _TCHAR* itemName,
                                          const _TCHAR* defaultV = nullptr,
                                          _TCHAR* buffer = nullptr, long bufferLen = 0);
            virtual double GetConfigDouble(const _TCHAR* section, const _TCHAR* itemName, double defaultV);
            virtual unsigned long GetConfigColor(const _TCHAR* section, const _TCHAR* itemName, unsigned long defaultV);
            virtual bool GetConfigBool(const _TCHAR* section, const _TCHAR* itemName, bool defaultV);
            virtual ValueScopeInt GetConfigValueScopeInt(const _TCHAR* section, const _TCHAR* item,
                                                 const ValueScopeInt& defaultV);
            virtual ValueScopeDouble GetConfigValueScopeDouble(const _TCHAR* section, const _TCHAR* item,
                                                       const ValueScopeDouble& defaultV);
            virtual long GetConfigSection(_TCHAR* buffer, long bufferLen);
            virtual long GetConfigItem(const _TCHAR* section, _TCHAR* buffer, long bufferLen);
            virtual long AddConfig(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* value);
            virtual long ModifyConfig(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* newValue);
            virtual long DeleteConfigItem(const _TCHAR* section, const _TCHAR* itemName);
            virtual long DeleteConfigSection(const _TCHAR* section);
            virtual long OpenIniFile(const _TCHAR* IniFileName);
            virtual long WriteIniFile(const _TCHAR* IniFileName);
            virtual void DumpIni(FILE* fp);
            virtual long GetSectionNum();
            virtual void Release();
            virtual ~IniFileImpl();
        };
    }
}

#endif
