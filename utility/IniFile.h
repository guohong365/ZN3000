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


/**Ini������*/
enum ITEM_TYPE
{
	TYPE_COMMENT, //!ע����
	TYPE_SECTION, //!����
	TYPE_ITEM     //!������
};

/**Ini���û�����*/
struct CONFIG_ITEM
{
	ITEM_TYPE Type;//!������
	/** ���������

	�������ļ�ָ��fp��������ļ���
	@param fp �Ѵ򿪵��ļ�ָ��
	*/
	virtual void Dump(FILE *fp)=0;
	virtual ~CONFIG_ITEM(){}
};

class delete_ptr
{
public:
	void operator()(CONFIG_ITEM* ptr){delete ptr;}
};
/**ֵ��Ŀ
*/
struct ITEM: public CONFIG_ITEM
{
	TCHAR ItemName[LEN_CFG_ITEM_NAME+1];//!<������
	TCHAR Value[LEN_CFG_MAX_STRING+1];//!<ֵ
	TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<ע��
	ITEM()
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


struct SECTION:public CONFIG_ITEM
{
	TCHAR SectionName[LEN_CFG_SECTION_NAME+1]; //!<����
	TCHAR Comment[LEN_CFG_MAX_STRING+1]; //!<ע��
	std::vector<CONFIG_ITEM *> Items; //!�ý������б���������
	SECTION()
	{
		SectionName[0]=Comment[0]=0;
		Type=TYPE_SECTION;
	}
	~SECTION()
	{
		std::for_each(Items.begin(), Items.end(), delete_ptr());
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
struct COMMENT:public CONFIG_ITEM
{
	TCHAR Comment[LEN_CFG_MAX_STRING+1];//!<ע��
	COMMENT()
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
*�ṩ�������ļ������ֶεĶ�ȡ��д�����
*
@note ʹ��ע��:�ڹ���������Ҫѡ��\n
*          Project --setting --c/c++ --c++ language 
*          --Enable Run_Time information(RTTI)
*/
class CIniFile
{
private:
	void RemoveAll()
	{
		std::for_each(m_IniItems.begin(),m_IniItems.end(), delete_ptr());
		m_IniItems.clear();
	}
	/**������Ŀ����

	���е�����������ڡ�������ע�ͣ��洢�ڸ�������
	*/
	std::vector<CONFIG_ITEM *> m_IniItems;

	/**�ж�buf�е��ִ��Ƿ���һ������Ŀ

	����ͬ[xxxxxxx]��ʽ��[]�����˵Ŀհ׷���������
	@param buf �����ַ���
	@return 
	- 0 ����
	- ��0 ��
	*/
	int IsSection(const TCHAR*buf);
	/**�ж�buf�е��ִ��Ƿ���һ��ע����Ŀ

	�����Կ�ͷ�Ŀհ׷��󣬵�һ���ַ���';'����
	@param buf �����ַ���
	@return 
	- 0 ����
	- ��0 ��
	*/
	int IsComment(const TCHAR*buf);
	/**����һ������Ŀ
	@param buf �����ַ���
	@param section ������������ؽ����õĽ���Ŀ����
	*/
	void PhaseSection(const TCHAR*buf, SECTION &section);
	/**����һ��ֵ��Ŀ
	@param buf �����ַ���
	@param item ���ؽ����õ�ֵ��Ŀ����
	@return 
	- 1 �ɹ�
	- 0 ʧ��
	*/
	long PhaseItem(const TCHAR *buf, ITEM &item);

	/**���ݽ��������Ѽ��صĽڶ���
	@param section ���������������������ִ�Сд
	@return 
	- NULL û���ҵ�
	- ���� ���������Ľڶ���ָ��
	*/
	SECTION * FindSection(const TCHAR *section);
	/**���ݽ���������������ֵ����
	@param Section ���������������������ִ�Сд
	@param ItemName �����������������������ִ�Сд
	@return 
	- NULL û���ҵ�
	- ���� ����������ֵ����ָ��
	*/
	ITEM *FindItem(const TCHAR *Section, const TCHAR *ItemName);
public:
	/**
	* ȡ�ø���SECTION��ע��
	*
	* @param Section :����SECTION��
	* @return const char * :����SECTION��ע��
	*/
	const TCHAR* GetSectionComment(const TCHAR *Section);

	/**
	* ȡ�ø����������ע��
	*
	* @param Section :����SECTION��
	* @param itemName:����ITEM��
	* @return const char * :��������ַ
	*/
	const TCHAR* GetItemComment(const TCHAR *Section, const TCHAR *itemName);

	/**
	* ��ȡ����SECTION��ĳһitem���ַ�ֵ��ȱʡ(��ITEM������)ȡdefaultV
	* @param Section :����SECTION��
	* @param ItemName :����ITEM��
	* @param defaultV :ȱʡ(��ITEM������)ȡֵ
	* @return char : Ҫȡ�õ�item��ֵ
	*/
	TCHAR GetConfigChar(const TCHAR *Section, const TCHAR *ItemName, TCHAR defaultV);
	/**
	*��ȡ����SECTION��ĳһitem������ֵ

	ȱʡ(��ITEM������)ȡdefaultV
	* @param Section :����SECTION��
	* @param ItemName :����ITEM��
	* @param defaultV :ȱʡ(��ITEM������)ȡֵ
	* @return long : Ҫȡ�õ�item��ֵ
	*/
	long GetConfigInt(const TCHAR *Section, const TCHAR *ItemName, long defaultV);
	/**
	* ȡ��ָ��SECTION��ĳһITEM���ַ���ֵ

	ȱʡ(��ITEM������)ȡdefaultV
	* @param Section :����SECTION��
	* @param ItemName :����ITEM��
	* @param buffer :���ITEMֵ�Ļ���
	* @param buflen :���ITEMֵ�Ļ���Ĵ�С
	* @param defaultV :ȱʡ(��ITEM������)ȡֵ
	* @return char* :Ҫȡ�õ�item��ֵ
	*/
	const TCHAR *GetConfigString(const TCHAR *Section, const TCHAR *ItemName, const TCHAR *defaultV=NULL, TCHAR *buffer=NULL, long buflen=0);
	/**
	* ��ȡ����SECTION��ĳһitem��	DOUBLEֵ

	ȱʡ(��ITEM������)ȡdefaultV
	* @param Section :����SECTION��
	* @param ItemName :����ITEM��
	* @param defaultV :ȱʡ(��ITEM������)ȡֵ
	* @return Ҫȡ�õ�item��ֵ
	*/
	double GetConfigDouble(const TCHAR *Section, const TCHAR *ItemName, double defaultV);

	/**
	* ��ȡ����ini�ļ���˳��SECTIONֵ��BUFFER�� ������buflen��
	*
	* @param buffer :��д˳��SECTION--��ʽSETION1-VALUE\\0SETION2-VALUE\\0SETION3-VALUE\\0SETION4-VALUE\\0\\0
	* @param buflen :����BUFFER�ĳ���
	* @return ���ش洢���н������軺�����ĳ��ȡ�
	*/
	long GetConfigSection(TCHAR *buffer, long buflen);
	/**
	* ��ȡĳһSECTION�µ�KEYֵ

	* ����ֵ�����BUFFER�У���ʽΪ"ITEM1\\0ITEM2\\0ITEM3\\0ITEM4\\0\\0"
	* @param section :ָ��SECTION ��
	* @param buffer ����ȡITEMֵ�Ļ���
	* @param buflen ������BUFFER�ĳ���
	* @return long : ���ش洢���н������軺�����ĳ��ȡ�
	*/
	long GetConfigItem(const TCHAR *section, TCHAR *buffer, long buflen);

	/**
	*��ĳһSECTION������һ��KEY���ֵ��ΪValue
	*
	* @param Section : ����
	* @param ItemName : ������
	* @param Value :������ֵ
	* @return 
	- -1 ʧ��
	- 0 �ɹ�
	*/
	long AddConfig(const TCHAR*Section, const TCHAR*ItemName, const TCHAR *Value);
	/**
	* �޸�ĳһSECTION�µ�KEY ��������ֵ����
	*
	* @param Section : section��
	* @param ItemName : key��
	* @param NewValue :�滻����ֵ
	* @return long 
	- -1 ʧ��
	- 0 �ɹ�
	*/
	long ModifyConfig(const TCHAR*Section, const TCHAR*ItemName, const TCHAR *NewValue);
	/**
	* ɾ��ĳһSECTION�µı���
	*
	* @param Section ������
	* @param ItemName ��������
	* @return 
	- 0 �ɹ�
	- -1 �����ڸ�SECTION
	*/
	long DeleteConfigItem(const TCHAR* Section, const TCHAR *ItemName);
	/**
	* ɾ��ĳһSECTION�µ�KEY
	*
	* @param Section :����
	* @return 
	- 0 �ɹ�
	- -1 ʧ�ܻ򲻴��ڸñ���
	*/
	long DeleteConfigSection(const TCHAR* Section);
	/**
	* ��һ�������ļ�����INI�ļ�
	*
	* @param IniFileName ��ȫ·����ʽ�ļ���
	* @return 
	- -1 ʧ��
	- 0 �ɹ�
	*/
	long OpenIniFile(const TCHAR *IniFileName); 
	/**
	* �ı�д��ʽ��һ�������ļ�����INI�ļ�
	*
	* @param IniFileName ��ȫ·����ʽ�ļ���
	* @return 
	- -1 ʧ��
	- 0 �ɹ�
	*/
	long WriteIniFile(const TCHAR*IniFileName);
	/** ���������󣬹رո�INI�ļ�
	*/
	void CloseIniFile();

	/**��������ļ�
	@param fp �ļ�ָ��
	*/
	void DumpIni(FILE*fp);
	/*
	suopengfei add it 
	*/
	/**
	* ��ȡָ��INI�ļ��а����Ľڵ�����
	*
	* @return �ڵ�����
	*/
	long GetSectionNum() ;
	~CIniFile()
	{
		std::for_each(m_IniItems.begin(), m_IniItems.end(), delete_ptr());
		m_IniItems.clear();
	}
};

#endif
/** @} */

