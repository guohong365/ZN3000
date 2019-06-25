/** \addtogroup BaseClass
@{
*/
#ifndef __INI_FILE__
#define __INI_FILE__
#include <uc/libuc.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <tchar.h>
#define LEN_CFG_MAX_STRING 1024
#define LEN_CFG_ITEM_NAME 256
#define LEN_CFG_SECTION_NAME 256



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


/**Ini������*/
enum ConfigItemType
{
	TYPE_COMMENT, //!ע����
	TYPE_SECTION, //!����
	TYPE_ITEM     //!������
};

/**Ini���û�����*/
struct LIB_UC_API ConfigItem
{
	ConfigItemType Type;//!������
	/** ���������

	�������ļ�ָ��fp��������ļ���
	@param fp �Ѵ򿪵��ļ�ָ��
	*/
	virtual void Dump(FILE *fp)=0;
	virtual ~ConfigItem();
};

class LIB_UC_API DeletePtr
{
public:
	void operator()(ConfigItem* ptr) const;
};
/**ֵ��Ŀ
*/
struct LIB_UC_API ConfigItemImpl: ConfigItem
{
	_TCHAR ItemName[LEN_CFG_ITEM_NAME+1];//!<������
	_TCHAR Value[LEN_CFG_MAX_STRING+1];//!<ֵ
	_TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<ע��
	ConfigItemImpl();;
	virtual void Dump(FILE* fp);
};

/**����Ŀ
*/
template class LIB_UC_API std::allocator<ConfigItem*>;
template class LIB_UC_API std::vector<ConfigItem *, std::allocator<ConfigItem*>>;

struct LIB_UC_API ConfigSection: ConfigItem
{
	_TCHAR SectionName[LEN_CFG_SECTION_NAME+1]; //!<����
	_TCHAR Comment[LEN_CFG_MAX_STRING+1]; //!<ע��
	std::vector<ConfigItem *> Items; //!�ý������б���������
	ConfigSection();

	~ConfigSection();

	virtual void Dump(FILE* fp);
};

/**��ע����Ŀ
*/
struct LIB_UC_API ConfigComment: ConfigItem
{
	_TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<ע��
	ConfigComment();

	virtual void Dump(FILE* fp);
};



/**�ṩ�������ļ������ֶεĶ�ȡ��д�����
 *
 *@note ʹ��ע��:�ڹ���������Ҫѡ��\n
 *       Project --setting --c/c++ --c++ language --Enable Run_Time information(RTTI)      
 */
class LIB_UC_API IniFile
{
protected:
	void removeAll()
	{
		std::for_each(_iniItems.begin(),_iniItems.end(), DeletePtr());
		_iniItems.clear();
	}
	/**������Ŀ����
	 *
	 *	���е�����������ڡ�������ע�ͣ��洢�ڸ�������
	 */
	std::vector<ConfigItem *> _iniItems;

	// ReSharper disable CommentTypo
	/**�ж�buf�е��ִ��Ƿ���һ������Ŀ
	 *	����ͬ[xxxxxxx]��ʽ��[]�����˵Ŀհ׷���������
	 *	@param buf �����ַ���
	 *	@return 
	 *		- 0 ����
	 *		- ��0 ��
	 */
	// ReSharper restore CommentTypo
	static int isSection(const _TCHAR*buf);
	// ReSharper disable CommentTypo
	/**�ж�buf�е��ִ��Ƿ���һ��ע����Ŀ
	 *
	 *	�����Կ�ͷ�Ŀհ׷��󣬵�һ���ַ���';'����
	 *	@param buf �����ַ���
	 *	@return 
	 *		- 0 ����
	 *		- ��0 ��
	*/
	// ReSharper restore CommentTypo
	static int isComment(const _TCHAR*buf);
	/**����һ������Ŀ
	 *
	 *  @param buf �����ַ���
	 * 	@param section ������������ؽ����õĽ���Ŀ����
	 */
	void phaseSection(const _TCHAR*buf, ConfigSection &section) const;
	/**����һ��ֵ��Ŀ
	 *
	 *	@param buf �����ַ���
	 *	@param item ���ؽ����õ�ֵ��Ŀ����
	 *	@return 
	 *		- 1 �ɹ�
	 *		- 0 ʧ��
	*/
	long phaseItem(const _TCHAR *buf, ConfigItemImpl &item) const;

	/**���ݽ��������Ѽ��صĽڶ���
	 *  @param section ���������������������ִ�Сд
	 *  @return �ڶ���ָ��
	 *      - NULL û���ҵ�
	 *      - ���� ���������Ľڶ���ָ��
	 */
	ConfigSection * findSection(const _TCHAR *section);

	/**���ݽ���������������ֵ����
	 *
	 *	@param section ���������������������ִ�Сд
	 *	@param itemName �����������������������ִ�Сд
	 *	@return 
	 *		- NULL û���ҵ�
	 *		- ���� ����������ֵ����ָ��
	 */
	ConfigItemImpl *findItem(const _TCHAR *section, const _TCHAR *itemName);
public:
	/**
	* ȡ�ø���SECTION��ע��
	*
	* @param section :����SECTION��
	* @return const char * :����SECTION��ע��
	*/
	const _TCHAR* GetSectionComment(const _TCHAR *section);

	/**
	* ȡ�ø����������ע��
	*
	* @param section :����SECTION��
	* @param itemName:����ITEM��
	* @return const char * :��������ַ
	*/
	const _TCHAR* GetItemComment(const _TCHAR *section, const _TCHAR *itemName);

	/**��ȡ����SECTION��ĳһitem���ַ�ֵ
	 *
	 * ȱʡ(��ITEM������)ȡdefaultV
	 * @param section :����SECTION��
	 * @param itemName :����ITEM��
	 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
	 * @return char : Ҫȡ�õ�item��ֵ
	*/
	_TCHAR GetConfigChar(const _TCHAR *section, const _TCHAR *itemName, _TCHAR defaultV);
	/**��ȡ����SECTION��ĳһitem������ֵ
	 *
	 * ȱʡ(��ITEM������)ȡdefaultV
	 * @param section :����SECTION��
	 * @param itemName :����ITEM��
	 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
	 * @return long : Ҫȡ�õ�item��ֵ
	*/
	long GetConfigInt(const _TCHAR *section, const _TCHAR *itemName, long defaultV);

	/**ȡ��ָ��SECTION��ĳһITEM���ַ���ֵ
	 *
	 * ȱʡ(��ITEM������)ȡdefaultV
	 * @param section :����SECTION��
	 * @param itemName :����ITEM��
	 * @param buffer :���ITEMֵ�Ļ���
	 * @param bufferLen :���ITEMֵ�Ļ���Ĵ�С
	 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
	 * @return char* :Ҫȡ�õ�item��ֵ
	 */
	const _TCHAR* GetConfigString(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* defaultV = nullptr,
	                             _TCHAR* buffer = nullptr, long bufferLen = 0);
	/**��ȡ����SECTION��ĳһitem��	DOUBLEֵ
	*
	* ȱʡ(��ITEM������)ȡdefaultV
	* @param section :����SECTION��
	* @param itemName :����ITEM��
	* @param defaultV :ȱʡ(��ITEM������)ȡֵ
	* @return Ҫȡ�õ�item��ֵ
	*/
	double GetConfigDouble(const _TCHAR * section, const _TCHAR * itemName, double defaultV);

	/**��ȡ��ɫֵ
	 *
	 * ��ɫ��ʽ"#"+ʮ������"ARGB"ֵ�����磺"#FF00FF00"
	 * @param section ����
	 * @param itemName ���ñ�����
	 * @param defaultV Ĭ��ֵ
	 * @return ����������ɫ����ʽ�����δ���ã�����Ĭ��ֵ
	 */
	unsigned long GetConfigColor(const _TCHAR*section, const _TCHAR* itemName, unsigned long defaultV);

	/** ��ȡ�������ֵ
	 *
	 * ������Ϊ"��","yes","true","1"ʱ������ true��������Ϊ"��","no","false","0"ʱ������ false
	 * @param section ����
	 * @param itemName ���ñ�����
	 * @param defaultV Ĭ��ֵ
	 * @return �������������Ĳ���ֵ����ʽ�����δ���ã�����Ĭ��ֵ
	 */
	bool GetConfigBool(const _TCHAR*section, const _TCHAR*itemName, bool defaultV);

	/**��ȡ������Χ
	 *
	 *��Χֵ��","�ŷָ�
	 * @param section ����
	 * @param itemName ���ñ�����
	 * @param defaultV Ĭ��ֵ
	 * @return �������������Ĳ���ֵ����ʽ�����δ���ã�����Ĭ��ֵ
	 */
	ValueScopeInt GetConfigValueScopeInt(const _TCHAR* section, const _TCHAR* item,const ValueScopeInt& defaultV);
	/**��ȡС����Χ
	 *
	 *��Χֵ��","�ŷָ�
	 * @param section ����
	 * @param itemName ���ñ�����
	 * @param defaultV Ĭ��ֵ
	 * @return �������������Ĳ���ֵ����ʽ�����δ���ã�����Ĭ��ֵ
	 */
	ValueScopeDouble GetConfigValueScopeDouble(const _TCHAR* section, const _TCHAR* item,const ValueScopeDouble& defaultV);

	/**��ȡ����ini�ļ���˳��SECTIONֵ��BUFFER�� 
	 *
	 * ������buflen��
	 * @param buffer :��д˳��SECTION--��ʽSETION1-VALUE\\0SETION2-VALUE\\0SETION3-VALUE\\0SETION4-VALUE\\0\\0
	 * @param bufferLen :����BUFFER�ĳ���
	 * @return ���ش洢���н������軺�����ĳ��ȡ�
	 */
	long GetConfigSection(_TCHAR *buffer, long bufferLen);

	/**��ȡĳһSECTION�µ�KEYֵ
	 *
	 * ����ֵ�����BUFFER�У���ʽΪ"ITEM1\\0ITEM2\\0ITEM3\\0ITEM4\\0\\0"
	 * @param section :ָ��SECTION ��
	 * @param buffer ����ȡITEMֵ�Ļ���
	 * @param bufferLen ������BUFFER�ĳ���
	 * @return long : ���ش洢���н������軺�����ĳ��ȡ�
	 */
	long GetConfigItem(const _TCHAR *section, _TCHAR *buffer, long bufferLen);

	/**��ĳһSECTION������һ��KEY���ֵ��ΪValue
	*
	* @param section : ����
	* @param itemName : ������
	* @param value :������ֵ
	* @return 
	*   - -1 ʧ��
	*   - 0 �ɹ�
	*/
	long AddConfig(const _TCHAR*section, const _TCHAR*itemName, const _TCHAR *value);
	/**�޸�ĳһSECTION�µ�KEY ��������ֵ����
	*
	* @param section : section��
	* @param itemName : key��
	* @param newValue :�滻����ֵ
	* @return long 
	*   - -1 ʧ��
	*   - 0 �ɹ�
	*/
	long ModifyConfig(const _TCHAR*section, const _TCHAR*itemName, const _TCHAR *newValue);

	/** ɾ��ĳһSECTION�µı���
	*
	* @param section ������
	* @param itemName ��������
	* @return 
	* 	- 0 �ɹ�
	* 	- -1 �����ڸ�SECTION
	*/
	long DeleteConfigItem(const _TCHAR* section, const _TCHAR * itemName);

	/** ɾ��ĳһSECTION�µ�KEY
	*
	* @param section :����
	* @return 
	* 	- 0 �ɹ�
	* 	- -1 ʧ�ܻ򲻴��ڸñ���
	*/
	long DeleteConfigSection(const _TCHAR* section);

	/** ��һ�������ļ�����INI�ļ�
	*
	* @param IniFileName ��ȫ·����ʽ�ļ���
	* @return 
	* 	- -1 ʧ��
	* 	- 0 �ɹ�
	*/
	long OpenIniFile(const _TCHAR *IniFileName);

	/** �ı�д��ʽ��һ�������ļ�����INI�ļ�
	*
	* @param IniFileName ��ȫ·����ʽ�ļ���
	* @return 
	* 	- -1 ʧ��
	* 	- 0 �ɹ�
	*/
	long WriteIniFile(const _TCHAR*IniFileName);

	/** ���������󣬹رո�INI�ļ�
	*/
	void CloseIniFile();

	/**��������ļ�
	 *	@param fp �ļ�ָ��
	*/
	void DumpIni(FILE*fp);
	/*
	suopengfei add it 
	*/
	/** ��ȡָ��INI�ļ��а����Ľڵ�����
	*
	* @return �ڵ�����
	*/
	long GetSectionNum() ;
	~IniFile()
	{
		std::for_each(_iniItems.begin(), _iniItems.end(), DeletePtr());
		_iniItems.clear();
	}
};

#endif
/** @} */

