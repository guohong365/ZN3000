/** \addtogroup BaseClass
@{
*/
#ifndef __INI_FILE__
#define __INI_FILE__
#include <stdio.h>
#include <vector>
#include <algorithm>
#define LEN_CFG_MAX_STRING 1024
#define LEN_CFG_ITEM_NAME 256
#define LEN_CFG_SECTION_NAME 256


/**Ini������*/
enum ConfigItemType
{
	TYPE_COMMENT, //!ע����
	TYPE_SECTION, //!����
	TYPE_ITEM     //!������
};

/**Ini���û�����*/
struct ConfigItem
{
	ConfigItemType Type;//!������
	/** ���������

	�������ļ�ָ��fp��������ļ���
	@param fp �Ѵ򿪵��ļ�ָ��
	*/
	virtual void Dump(FILE *fp)=0;
	virtual ~ConfigItem(){}
};

class DeletePtr
{
public:
	void operator()(ConfigItem* ptr) const {delete ptr;}
};
/**ֵ��Ŀ
*/
struct ConfigItemImpl: ConfigItem
{
	TCHAR ItemName[LEN_CFG_ITEM_NAME+1];//!<������
	TCHAR Value[LEN_CFG_MAX_STRING+1];//!<ֵ
	TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<ע��
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

/**����Ŀ
*/


struct ConfigSection:public ConfigItem
{
	TCHAR SectionName[LEN_CFG_SECTION_NAME+1]; //!<����
	TCHAR Comment[LEN_CFG_MAX_STRING+1]; //!<ע��
	std::vector<ConfigItem *> Items; //!�ý������б���������
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

/**��ע����Ŀ
*/
struct ConfigComment:public ConfigItem
{
	TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<ע��
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



/**�ṩ�������ļ������ֶεĶ�ȡ��д�����
 *
 *@note ʹ��ע��:�ڹ���������Ҫѡ��\n
 *       Project --setting --c/c++ --c++ language --Enable Run_Time information(RTTI)      
 */
class CIniFile
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
	static int isSection(const TCHAR*buf);
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
	static int isComment(const TCHAR*buf);
	/**����һ������Ŀ
	 *
	 *  @param buf �����ַ���
	 * 	@param section ������������ؽ����õĽ���Ŀ����
	 */
	void phaseSection(const TCHAR*buf, ConfigSection &section) const;
	/**����һ��ֵ��Ŀ
	 *
	 *	@param buf �����ַ���
	 *	@param item ���ؽ����õ�ֵ��Ŀ����
	 *	@return 
	 *		- 1 �ɹ�
	 *		- 0 ʧ��
	*/
	long phaseItem(const TCHAR *buf, ConfigItemImpl &item) const;

	/**���ݽ��������Ѽ��صĽڶ���
	 *  @param section ���������������������ִ�Сд
	 *  @return �ڶ���ָ��
	 *      - NULL û���ҵ�
	 *      - ���� ���������Ľڶ���ָ��
	 */
	ConfigSection * findSection(const TCHAR *section);

	/**���ݽ���������������ֵ����
	 *
	 *	@param section ���������������������ִ�Сд
	 *	@param itemName �����������������������ִ�Сд
	 *	@return 
	 *		- NULL û���ҵ�
	 *		- ���� ����������ֵ����ָ��
	 */
	ConfigItemImpl *findItem(const TCHAR *section, const TCHAR *itemName);
public:
	/**
	* ȡ�ø���SECTION��ע��
	*
	* @param section :����SECTION��
	* @return const char * :����SECTION��ע��
	*/
	const TCHAR* GetSectionComment(const TCHAR *section);

	/**
	* ȡ�ø����������ע��
	*
	* @param section :����SECTION��
	* @param itemName:����ITEM��
	* @return const char * :��������ַ
	*/
	const TCHAR* GetItemComment(const TCHAR *section, const TCHAR *itemName);

	/**��ȡ����SECTION��ĳһitem���ַ�ֵ
	 *
	 * ȱʡ(��ITEM������)ȡdefaultV
	 * @param section :����SECTION��
	 * @param itemName :����ITEM��
	 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
	 * @return char : Ҫȡ�õ�item��ֵ
	*/
	TCHAR GetConfigChar(const TCHAR *section, const TCHAR *itemName, TCHAR defaultV);
	/**��ȡ����SECTION��ĳһitem������ֵ
	 *
	 * ȱʡ(��ITEM������)ȡdefaultV
	 * @param section :����SECTION��
	 * @param itemName :����ITEM��
	 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
	 * @return long : Ҫȡ�õ�item��ֵ
	*/
	long GetConfigInt(const TCHAR *section, const TCHAR *itemName, long defaultV);

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
	const TCHAR* GetConfigString(const TCHAR* section, const TCHAR* itemName, const TCHAR* defaultV = nullptr,
	                             TCHAR* buffer = nullptr, long bufferLen = 0);
	/**��ȡ����SECTION��ĳһitem��	DOUBLEֵ
	*
	* ȱʡ(��ITEM������)ȡdefaultV
	* @param section :����SECTION��
	* @param itemName :����ITEM��
	* @param defaultV :ȱʡ(��ITEM������)ȡֵ
	* @return Ҫȡ�õ�item��ֵ
	*/
	double GetConfigDouble(const TCHAR * section, const TCHAR * itemName, double defaultV);

	/**��ȡ��ɫֵ
	 *
	 * ��ɫ��ʽ"#"+ʮ������"ARGB"ֵ�����磺"#FF00FF00"
	 * @param section ����
	 * @param itemName ���ñ�����
	 * @param defaultV Ĭ��ֵ
	 * @return ����������ɫ����ʽ�����δ���ã�����Ĭ��ֵ
	 */
	unsigned long GetConfigColor(const TCHAR*section, const TCHAR* itemName, unsigned long defaultV);


	/**��ȡ����ini�ļ���˳��SECTIONֵ��BUFFER�� 
	 *
	 * ������buflen��
	 * @param buffer :��д˳��SECTION--��ʽSETION1-VALUE\\0SETION2-VALUE\\0SETION3-VALUE\\0SETION4-VALUE\\0\\0
	 * @param bufferLen :����BUFFER�ĳ���
	 * @return ���ش洢���н������軺�����ĳ��ȡ�
	 */
	long GetConfigSection(TCHAR *buffer, long bufferLen);

	/**��ȡĳһSECTION�µ�KEYֵ
	 *
	 * ����ֵ�����BUFFER�У���ʽΪ"ITEM1\\0ITEM2\\0ITEM3\\0ITEM4\\0\\0"
	 * @param section :ָ��SECTION ��
	 * @param buffer ����ȡITEMֵ�Ļ���
	 * @param bufferLen ������BUFFER�ĳ���
	 * @return long : ���ش洢���н������軺�����ĳ��ȡ�
	 */
	long GetConfigItem(const TCHAR *section, TCHAR *buffer, long bufferLen);

	/**��ĳһSECTION������һ��KEY���ֵ��ΪValue
	*
	* @param section : ����
	* @param itemName : ������
	* @param value :������ֵ
	* @return 
	*   - -1 ʧ��
	*   - 0 �ɹ�
	*/
	long AddConfig(const TCHAR*section, const TCHAR*itemName, const TCHAR *value);
	/**�޸�ĳһSECTION�µ�KEY ��������ֵ����
	*
	* @param section : section��
	* @param itemName : key��
	* @param newValue :�滻����ֵ
	* @return long 
	*   - -1 ʧ��
	*   - 0 �ɹ�
	*/
	long ModifyConfig(const TCHAR*section, const TCHAR*itemName, const TCHAR *newValue);

	/** ɾ��ĳһSECTION�µı���
	*
	* @param section ������
	* @param itemName ��������
	* @return 
	* 	- 0 �ɹ�
	* 	- -1 �����ڸ�SECTION
	*/
	long DeleteConfigItem(const TCHAR* section, const TCHAR * itemName);

	/** ɾ��ĳһSECTION�µ�KEY
	*
	* @param section :����
	* @return 
	* 	- 0 �ɹ�
	* 	- -1 ʧ�ܻ򲻴��ڸñ���
	*/
	long DeleteConfigSection(const TCHAR* section);

	/** ��һ�������ļ�����INI�ļ�
	*
	* @param IniFileName ��ȫ·����ʽ�ļ���
	* @return 
	* 	- -1 ʧ��
	* 	- 0 �ɹ�
	*/
	long OpenIniFile(const TCHAR *IniFileName);

	/** �ı�д��ʽ��һ�������ļ�����INI�ļ�
	*
	* @param IniFileName ��ȫ·����ʽ�ļ���
	* @return 
	* 	- -1 ʧ��
	* 	- 0 �ɹ�
	*/
	long WriteIniFile(const TCHAR*IniFileName);

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
	~CIniFile()
	{
		std::for_each(_iniItems.begin(), _iniItems.end(), DeletePtr());
		_iniItems.clear();
	}
};

#endif
/** @} */

