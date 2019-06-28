/** \addtogroup BaseClass
@{
*/
#ifndef __INI_FILE__
#define __INI_FILE__
#include <uc/libuc.h>
#include <stdio.h>
#include <tchar.h>

namespace uc
{
	namespace utils
	{
		struct ValueScopeInt
		{
			long LowValue;
			long HighValue;
		};

		struct ValueScopeDouble
		{
			double LowValue;
			double HighValue;
		};

		/**提供对配置文件各种字段的读取和写入操作
		 *
		 *@note 使用注意:在工程设置中要选择：\n
		 *       Project --setting --c/c++ --c++ language --Enable Run_Time information(RTTI)      
		 */
		class LIB_UC_API IniFile
		{
		protected:
			IniFile()
			{
			};
			virtual ~IniFile();

			IniFile(const IniFile& iniFile)
			{
			};
		public:
			/**
			* 取得给定SECTION的注释
			*
			* @param section :给定SECTION名
			* @return const char * :给定SECTION的注释
			*/
			virtual const _TCHAR* GetSectionComment(const _TCHAR* section) =0;

			/**
			* 取得给定配置项的注释
			*
			* @param section :给定SECTION名
			* @param itemName:给定ITEM名
			* @return const char * :缓冲区地址
			*/
			virtual const _TCHAR* GetItemComment(const _TCHAR* section, const _TCHAR* itemName) =0;

			/**获取给定SECTION下某一item的字符值
			 *
			 * 缺省(该ITEM不存在)取defaultV
			 * @param section :给定SECTION名
			 * @param itemName :给定ITEM名
			 * @param defaultV :缺省(该ITEM不存在)取值
			 * @return char : 要取得的item的值
			*/
			virtual _TCHAR GetConfigChar(const _TCHAR* section, const _TCHAR* itemName, _TCHAR defaultV) =0;
			/**获取给定SECTION下某一item的整数值
			 *
			 * 缺省(该ITEM不存在)取defaultV
			 * @param section :给定SECTION名
			 * @param itemName :给定ITEM名
			 * @param defaultV :缺省(该ITEM不存在)取值
			 * @return long : 要取得的item的值
			*/
			virtual long GetConfigInt(const _TCHAR* section, const _TCHAR* itemName, long defaultV) =0;

			/**取得指定SECTION下某一ITEM的字符串值
			 *
			 * 缺省(该ITEM不存在)取defaultV
			 * @param section :给定SECTION名
			 * @param itemName :给定ITEM名
			 * @param buffer :存放ITEM值的缓冲
			 * @param bufferLen :存放ITEM值的缓冲的大小
			 * @param defaultV :缺省(该ITEM不存在)取值
			 * @return char* :要取得的item的值
			 */
			virtual const _TCHAR* GetConfigString(const _TCHAR* section, const _TCHAR* itemName,
			                                      const _TCHAR* defaultV = nullptr,
			                                      _TCHAR* buffer = nullptr, long bufferLen = 0) =0;
			/**获取给定SECTION下某一item的	DOUBLE值
			*
			* 缺省(该ITEM不存在)取defaultV
			* @param section :给定SECTION名
			* @param itemName :给定ITEM名
			* @param defaultV :缺省(该ITEM不存在)取值
			* @return 要取得的item的值
			*/
			virtual double GetConfigDouble(const _TCHAR* section, const _TCHAR* itemName, double defaultV) =0;

			/**获取颜色值
			 *
			 * 颜色格式"#"+十六进制"ARGB"值，列如："#FF00FF00"
			 * @param section 节名
			 * @param itemName 配置变量名
			 * @param defaultV 默认值
			 * @return 返回配置颜色。格式错误或未配置，返回默认值
			 */
			virtual unsigned long GetConfigColor(const _TCHAR* section, const _TCHAR* itemName, unsigned long defaultV)
			=0;

			/** 获取配置项布尔值
			 *
			 * 配置项为"是","yes","true","1"时，返回 true；配置项为"否","no","false","0"时，返回 false
			 * @param section 节名
			 * @param itemName 配置变量名
			 * @param defaultV 默认值
			 * @return 返回配置项代表的布尔值。格式错误或未配置，返回默认值
			 */
			virtual bool GetConfigBool(const _TCHAR* section, const _TCHAR* itemName, bool defaultV) =0;

			/**获取整数范围
			 *
			 *范围值用","号分隔
			 * @param section 节名
			 * @param item 配置变量名
			 * @param defaultV 默认值
			 * @return 返回配置项代表的布尔值。格式错误或未配置，返回默认值
			 */
			virtual ValueScopeInt GetConfigValueScopeInt(const _TCHAR* section, const _TCHAR* item,
			                                             const ValueScopeInt& defaultV) =0;
			/**获取小数范围
			 *
			 *范围值用","号分隔
			 * @param section 节名
			 * @param item 配置变量名
			 * @param defaultV 默认值
			 * @return 返回配置项代表的布尔值。格式错误或未配置，返回默认值
			 */
			virtual ValueScopeDouble GetConfigValueScopeDouble(const _TCHAR* section, const _TCHAR* item,
			                                                   const ValueScopeDouble& defaultV) =0;

			/**获取给定ini文件下顺序SECTION值到BUFFER； 
			 *
			 * 最多填充buflen长
			 * @param buffer :填写顺序SECTION--格式SETION1-VALUE\\0SETION2-VALUE\\0SETION3-VALUE\\0SETION4-VALUE\\0\\0
			 * @param bufferLen :给定BUFFER的长度
			 * @return 返回存储所有节名所需缓冲区的长度。
			 */
			virtual long GetConfigSection(_TCHAR* buffer, long bufferLen) =0;

			/**获取某一SECTION下的KEY值
			 *
			 * 返回值存放在BUFFER中；格式为"ITEM1\\0ITEM2\\0ITEM3\\0ITEM4\\0\\0"
			 * @param section :指定SECTION 名
			 * @param buffer ：获取ITEM值的缓冲
			 * @param bufferLen ：给定BUFFER的长度
			 * @return long : 返回存储所有节名所需缓冲区的长度。
			 */
			virtual long GetConfigItem(const _TCHAR* section, _TCHAR* buffer, long bufferLen) =0;

			/**在某一SECTION下增加一个KEY项；其值赋为Value
			*
			* @param section : 节名
			* @param itemName : 变量名
			* @param value :变量的值
			* @return 
			*   - -1 失败
			*   - 0 成功
			*/
			virtual long AddConfig(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* value) =0;
			/**修改某一SECTION下的KEY ，并用新值代替
			*
			* @param section : section名
			* @param itemName : key名
			* @param newValue :替换的新值
			* @return long 
			*   - -1 失败
			*   - 0 成功
			*/
			virtual long ModifyConfig(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* newValue) =0;

			/** 删除某一SECTION下的变量
			*
			* @param section ：节名
			* @param itemName ：变量名
			* @return 
			* 	- 0 成功
			* 	- -1 不存在该SECTION
			*/
			virtual long DeleteConfigItem(const _TCHAR* section, const _TCHAR* itemName) =0;

			/** 删除某一SECTION下的KEY
			*
			* @param section :节名
			* @return 
			* 	- 0 成功
			* 	- -1 失败或不存在该变量
			*/
			virtual long DeleteConfigSection(const _TCHAR* section) =0;

			/** 打开一个给定文件名的INI文件
			*
			* @param IniFileName ：全路径格式文件名
			* @return 
			* 	- -1 失败
			* 	- 0 成功
			*/
			static IniFile* OpenIniFile(const _TCHAR* IniFileName);

			/** 文本写格式打开一个给定文件名的INI文件
			*
			* @param IniFileName ：全路径格式文件名
			* @return 
			* 	- -1 失败
			* 	- 0 成功
			*/
			virtual long WriteIniFile(const _TCHAR* IniFileName) =0;

			/** 结束操作后，关闭该INI文件
			*/
			virtual void Release() =0;

			/**输出配置文件
			 *	@param fp 文件指针
			*/
			virtual void DumpIni(FILE* fp) =0;
			/*
			suopengfei add it 
			*/
			/** 获取指定INI文件中包括的节的数量
			*
			* @return 节的数量
			*/
			virtual long GetSectionNum() =0;
		};
	}
}
#endif
/** @} */
