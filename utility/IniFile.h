/** \addtogroup BaseClass
@{
*/
#ifndef __INI_FILE__
#define __INI_FILE__
#include <stdio.h>
#include <vector>
#include <algorithm>
#define LEN_CFG_MAX_STRING 256
#define LEN_CFG_ITEM_NAME 64
#define LEN_CFG_SECTION_NAME 64


/**Ini项类型*/
enum ConfigItemType
{
	TYPE_COMMENT, //!注释项
	TYPE_SECTION, //!节项
	TYPE_ITEM     //!变量项
};

/**Ini配置基本项*/
struct ConfigItem
{
	ConfigItemType Type;//!项类型
	/** 项输出函数

	输出该项到文件指针fp相关联的文件中
	@param fp 已打开的文件指针
	*/
	virtual void Dump(FILE *fp)=0;
	virtual ~ConfigItem(){}
};

class DeletePtr
{
public:
	void operator()(ConfigItem* ptr) const {delete ptr;}
};
/**值项目
*/
struct ConfigItemImpl: ConfigItem
{
	TCHAR ItemName[LEN_CFG_ITEM_NAME+1];//!<变量名
	TCHAR Value[LEN_CFG_MAX_STRING+1];//!<值
	TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<注释
	ConfigItemImpl()
	{
		ItemName[0]=Value[0]=Comment[0]=0;
		Type=TYPE_ITEM;
	};
	virtual void Dump(FILE *fp)
	{
		_ftprintf(fp, _T("%s\t=%s\t%s\n"), ItemName, Value, Comment);
	}
};

/**节项目
*/


struct ConfigSection:public ConfigItem
{
	TCHAR SectionName[LEN_CFG_SECTION_NAME+1]; //!<节名
	TCHAR Comment[LEN_CFG_MAX_STRING+1]; //!<注释
	std::vector<ConfigItem *> Items; //!该节下所有变量的链表
	ConfigSection()
	{
		SectionName[0]=Comment[0]=0;
		Type=TYPE_SECTION;
	}
	~ConfigSection()
	{
		std::for_each(Items.begin(), Items.end(), DeletePtr());
		Items.clear();
	}
	virtual void Dump(FILE *fp)
	{
		_ftprintf(fp, _T("\n[%s]\t%s\n"), SectionName, Comment);
		for(size_t i=0; i<Items.size(); i++)
		{
			Items[i]->Dump(fp);
		}
	}
};

/**纯注释项目
*/
struct ConfigComment:public ConfigItem
{
	TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<注释
	ConfigComment()
	{
		Comment[0]=0;
		Type=TYPE_COMMENT;
	}
	virtual void Dump(FILE *fp)
	{
		_ftprintf(fp, _T("%s\n"), Comment);
	}
};



/**
*提供对配置文件各种字段的读取和写入操作
*
@note 使用注意:在工程设置中要选择：\n
*          Project --setting --c/c++ --c++ language 
*          --Enable Run_Time information(RTTI)
*/
class CIniFile
{
private:
	void removeAll()
	{
		std::for_each(_iniItems.begin(),_iniItems.end(), DeletePtr());
		_iniItems.clear();
	}
	/**配置项目链表

	所有的配置项（包括节、变量、注释）存储于该链表中
	*/
	std::vector<ConfigItem *> _iniItems;

	// ReSharper disable CommentTypo
	/**判断buf中的字串是否是一个节项目

	即如同[xxxxxxx]形式，[]中两端的空白符将被忽略
	@param buf 输入字符串
	@return 
	- 0 不是
	- 非0 是
	*/
	// ReSharper restore CommentTypo
	static int isSection(const TCHAR*buf);
	// ReSharper disable CommentTypo
	/**判断buf中的字串是否是一个注释项目

	即忽略开头的空白符后，第一个字符是';'的行
	@param buf 输入字符串
	@return 
	- 0 不是
	- 非0 是
	*/
	// ReSharper restore CommentTypo
	static int isComment(const TCHAR*buf);
	/**解析一个节项目
	@param buf 输入字符串
	@param section 输出参数，返回解析好的节项目对象
	*/
	void phaseSection(const TCHAR*buf, ConfigSection &section) const;
	/**解析一个值项目
	@param buf 输入字符串
	@param item 返回解析好的值项目对象
	@return 
	- 1 成功
	- 0 失败
	*/
	long phaseItem(const TCHAR *buf, ConfigItemImpl &item) const;

	/**根据节名查找已加载的节对象
	@param section 节名，查找条件，不区分大小写
	@return 
	- NULL 没有找到
	- 其它 符合条件的节对象指针
	*/
	ConfigSection * findSection(const TCHAR *section);
	/**根据节名、变量名查找值对象
	@param Section 节名，查找条件，不区分大小写
	@param ItemName 变量名，查找条件，不区分大小写
	@return 
	- NULL 没有找到
	- 其它 符合条件的值对象指针
	*/
	ConfigItemImpl *findItem(const TCHAR *Section, const TCHAR *ItemName);
public:
	/**
	* 取得给定SECTION的注释
	*
	* @param Section :给定SECTION名
	* @return const char * :给定SECTION的注释
	*/
	const TCHAR* GetSectionComment(const TCHAR *Section);

	/**
	* 取得给定配置项的注释
	*
	* @param Section :给定SECTION名
	* @param itemName:给定ITEM名
	* @return const char * :缓冲区地址
	*/
	const TCHAR* GetItemComment(const TCHAR *Section, const TCHAR *itemName);

	/**
	* 获取给定SECTION下某一item的字符值，缺省(该ITEM不存在)取defaultV
	* @param Section :给定SECTION名
	* @param ItemName :给定ITEM名
	* @param defaultV :缺省(该ITEM不存在)取值
	* @return char : 要取得的item的值
	*/
	TCHAR GetConfigChar(const TCHAR *Section, const TCHAR *ItemName, TCHAR defaultV);
	/**
	*获取给定SECTION下某一item的整数值

	缺省(该ITEM不存在)取defaultV
	* @param Section :给定SECTION名
	* @param ItemName :给定ITEM名
	* @param defaultV :缺省(该ITEM不存在)取值
	* @return long : 要取得的item的值
	*/
	long GetConfigInt(const TCHAR *Section, const TCHAR *ItemName, long defaultV);
	/**
	* 取得指定SECTION下某一ITEM的字符串值

	缺省(该ITEM不存在)取defaultV
	* @param section :给定SECTION名
	* @param itemName :给定ITEM名
	* @param buffer :存放ITEM值的缓冲
	* @param bufferLen :存放ITEM值的缓冲的大小
	* @param defaultV :缺省(该ITEM不存在)取值
	* @return char* :要取得的item的值
	*/
	const TCHAR* GetConfigString(const TCHAR* section, const TCHAR* itemName, const TCHAR* defaultV = nullptr,
	                             TCHAR* buffer = nullptr, long bufferLen = 0);
	/**
	* 获取给定SECTION下某一item的	DOUBLE值

	缺省(该ITEM不存在)取defaultV
	* @param section :给定SECTION名
	* @param itemName :给定ITEM名
	* @param defaultV :缺省(该ITEM不存在)取值
	* @return 要取得的item的值
	*/
	double GetConfigDouble(const TCHAR *section, const TCHAR *itemName, double defaultV);

	/**
	* 获取给定ini文件下顺序SECTION值到BUFFER； 最多填充buflen长
	*
	* @param buffer :填写顺序SECTION--格式SETION1-VALUE\\0SETION2-VALUE\\0SETION3-VALUE\\0SETION4-VALUE\\0\\0
	* @param bufferLen :给定BUFFER的长度
	* @return 返回存储所有节名所需缓冲区的长度。
	*/
	long GetConfigSection(TCHAR *buffer, long bufferLen);
	/**
	* 获取某一SECTION下的KEY值

	* 返回值存放在BUFFER中；格式为"ITEM1\\0ITEM2\\0ITEM3\\0ITEM4\\0\\0"
	* @param section :指定SECTION 名
	* @param buffer ：获取ITEM值的缓冲
	* @param bufferLen ：给定BUFFER的长度
	* @return long : 返回存储所有节名所需缓冲区的长度。
	*/
	long GetConfigItem(const TCHAR *section, TCHAR *buffer, long bufferLen);

	/**
	*在某一SECTION下增加一个KEY项；其值赋为Value
	*
	* @param section : 节名
	* @param itemName : 变量名
	* @param value :变量的值
	* @return 
	- -1 失败
	- 0 成功
	*/
	long AddConfig(const TCHAR*section, const TCHAR*itemName, const TCHAR *value);
	/**
	* 修改某一SECTION下的KEY ，并用新值代替
	*
	* @param section : section名
	* @param itemName : key名
	* @param newValue :替换的新值
	* @return long 
	- -1 失败
	- 0 成功
	*/
	long ModifyConfig(const TCHAR*section, const TCHAR*itemName, const TCHAR *newValue);
	/**
	* 删除某一SECTION下的变量
	*
	* @param section ：节名
	* @param itemName ：变量名
	* @return 
	- 0 成功
	- -1 不存在该SECTION
	*/
	long DeleteConfigItem(const TCHAR* section, const TCHAR * itemName);
	/**
	* 删除某一SECTION下的KEY
	*
	* @param section :节名
	* @return 
	- 0 成功
	- -1 失败或不存在该变量
	*/
	long DeleteConfigSection(const TCHAR* section);
	/**
	* 打开一个给定文件名的INI文件
	*
	* @param IniFileName ：全路径格式文件名
	* @return 
	- -1 失败
	- 0 成功
	*/
	long OpenIniFile(const TCHAR *IniFileName); 
	/**
	* 文本写格式打开一个给定文件名的INI文件
	*
	* @param IniFileName ：全路径格式文件名
	* @return 
	- -1 失败
	- 0 成功
	*/
	long WriteIniFile(const TCHAR*IniFileName);
	/** 结束操作后，关闭该INI文件
	*/
	void CloseIniFile();

	/**输出配置文件
	@param fp 文件指针
	*/
	void DumpIni(FILE*fp);
	/*
	suopengfei add it 
	*/
	/**
	* 获取指定INI文件中包括的节的数量
	*
	* @return 节的数量
	*/
	long GetSectionNum() ;
	~CIniFile()
	{
		std::for_each(_iniItems.begin(), _iniItems.end(), DeletePtr());
		_iniItems.clear();
	}
};

#endif
/** @} */

