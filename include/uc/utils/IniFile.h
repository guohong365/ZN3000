/** \addtogroup BaseClass
@{
*/
#ifndef __INI_FILE__
#define __INI_FILE__
#include <stdio.h>
#include <tchar.h>
#include <uc/libcore.h>
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

		/**�ṩ�������ļ������ֶεĶ�ȡ��д�����
		 *
		 *@note ʹ��ע��:�ڹ���������Ҫѡ��\n
		 *       Project --setting --c/c++ --c++ language --Enable Run_Time information(RTTI)      
		 */
		class LIB_CORE_API IniFile
		{
		protected:
			virtual ~IniFile();
		public:
			/**
			* ȡ�ø���SECTION��ע��
			*
			* @param section :����SECTION��
			* @return const char * :����SECTION��ע��
			*/
			virtual const _TCHAR* GetSectionComment(const _TCHAR* section) =0;

			/**
			* ȡ�ø����������ע��
			*
			* @param section :����SECTION��
			* @param itemName:����ITEM��
			* @return const char * :��������ַ
			*/
			virtual const _TCHAR* GetItemComment(const _TCHAR* section, const _TCHAR* itemName) =0;

			/**��ȡ����SECTION��ĳһitem���ַ�ֵ
			 *
			 * ȱʡ(��ITEM������)ȡdefaultV
			 * @param section :����SECTION��
			 * @param itemName :����ITEM��
			 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
			 * @return char : Ҫȡ�õ�item��ֵ
			*/
			virtual _TCHAR GetConfigChar(const _TCHAR* section, const _TCHAR* itemName, _TCHAR defaultV) =0;
			/**��ȡ����SECTION��ĳһitem������ֵ
			 *
			 * ȱʡ(��ITEM������)ȡdefaultV
			 * @param section :����SECTION��
			 * @param itemName :����ITEM��
			 * @param defaultV :ȱʡ(��ITEM������)ȡֵ
			 * @return long : Ҫȡ�õ�item��ֵ
			*/
			virtual long GetConfigInt(const _TCHAR* section, const _TCHAR* itemName, long defaultV) =0;

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
			virtual const _TCHAR* GetConfigString(const _TCHAR* section, const _TCHAR* itemName,
			                                      const _TCHAR* defaultV = nullptr,
			                                      _TCHAR* buffer = nullptr, long bufferLen = 0) =0;
			/**��ȡ����SECTION��ĳһitem��	DOUBLEֵ
			*
			* ȱʡ(��ITEM������)ȡdefaultV
			* @param section :����SECTION��
			* @param itemName :����ITEM��
			* @param defaultV :ȱʡ(��ITEM������)ȡֵ
			* @return Ҫȡ�õ�item��ֵ
			*/
			virtual double GetConfigDouble(const _TCHAR* section, const _TCHAR* itemName, double defaultV) =0;

			/**��ȡ��ɫֵ
			 *
			 * ��ɫ��ʽ"#"+ʮ������"ARGB"ֵ�����磺"#FF00FF00"
			 * @param section ����
			 * @param itemName ���ñ�����
			 * @param defaultV Ĭ��ֵ
			 * @return ����������ɫ����ʽ�����δ���ã�����Ĭ��ֵ
			 */
			virtual unsigned long GetConfigColor(const _TCHAR* section, const _TCHAR* itemName, unsigned long defaultV)
			=0;

			/** ��ȡ�������ֵ
			 *
			 * ������Ϊ"��","yes","true","1"ʱ������ true��������Ϊ"��","no","false","0"ʱ������ false
			 * @param section ����
			 * @param itemName ���ñ�����
			 * @param defaultV Ĭ��ֵ
			 * @return �������������Ĳ���ֵ����ʽ�����δ���ã�����Ĭ��ֵ
			 */
			virtual bool GetConfigBool(const _TCHAR* section, const _TCHAR* itemName, bool defaultV) =0;

			/**��ȡ������Χ
			 *
			 *��Χֵ��","�ŷָ�
			 * @param section ����
			 * @param item ���ñ�����
			 * @param defaultV Ĭ��ֵ
			 * @return �������������Ĳ���ֵ����ʽ�����δ���ã�����Ĭ��ֵ
			 */
			virtual ValueScopeInt GetConfigValueScopeInt(const _TCHAR* section, const _TCHAR* item,
			                                             const ValueScopeInt& defaultV) =0;
			/**��ȡС����Χ
			 *
			 *��Χֵ��","�ŷָ�
			 * @param section ����
			 * @param item ���ñ�����
			 * @param defaultV Ĭ��ֵ
			 * @return �������������Ĳ���ֵ����ʽ�����δ���ã�����Ĭ��ֵ
			 */
			virtual ValueScopeDouble GetConfigValueScopeDouble(const _TCHAR* section, const _TCHAR* item,
			                                                   const ValueScopeDouble& defaultV) =0;

			/**��ȡ����ini�ļ���˳��SECTIONֵ��BUFFER�� 
			 *
			 * ������buflen��
			 * @param buffer :��д˳��SECTION--��ʽSETION1-VALUE\\0SETION2-VALUE\\0SETION3-VALUE\\0SETION4-VALUE\\0\\0
			 * @param bufferLen :����BUFFER�ĳ���
			 * @return ���ش洢���н������軺�����ĳ��ȡ�
			 */
			virtual long GetConfigSection(_TCHAR* buffer, long bufferLen) =0;

			/**��ȡĳһSECTION�µ�KEYֵ
			 *
			 * ����ֵ�����BUFFER�У���ʽΪ"ITEM1\\0ITEM2\\0ITEM3\\0ITEM4\\0\\0"
			 * @param section :ָ��SECTION ��
			 * @param buffer ����ȡITEMֵ�Ļ���
			 * @param bufferLen ������BUFFER�ĳ���
			 * @return long : ���ش洢���н������軺�����ĳ��ȡ�
			 */
			virtual long GetConfigItem(const _TCHAR* section, _TCHAR* buffer, long bufferLen) =0;

			/**��ĳһSECTION������һ��KEY���ֵ��ΪValue
			*
			* @param section : ����
			* @param itemName : ������
			* @param value :������ֵ
			* @return 
			*   - -1 ʧ��
			*   - 0 �ɹ�
			*/
			virtual long AddConfig(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* value) =0;
			/**�޸�ĳһSECTION�µ�KEY ��������ֵ����
			*
			* @param section : section��
			* @param itemName : key��
			* @param newValue :�滻����ֵ
			* @return long 
			*   - -1 ʧ��
			*   - 0 �ɹ�
			*/
			virtual long ModifyConfig(const _TCHAR* section, const _TCHAR* itemName, const _TCHAR* newValue) =0;

			/** ɾ��ĳһSECTION�µı���
			*
			* @param section ������
			* @param itemName ��������
			* @return 
			* 	- 0 �ɹ�
			* 	- -1 �����ڸ�SECTION
			*/
			virtual long DeleteConfigItem(const _TCHAR* section, const _TCHAR* itemName) =0;

			/** ɾ��ĳһSECTION�µ�KEY
			*
			* @param section :����
			* @return 
			* 	- 0 �ɹ�
			* 	- -1 ʧ�ܻ򲻴��ڸñ���
			*/
			virtual long DeleteConfigSection(const _TCHAR* section) =0;

			/** ��һ�������ļ�����INI�ļ�
			*
			* @param IniFileName ��ȫ·����ʽ�ļ���
			* @return 
			* 	- -1 ʧ��
			* 	- 0 �ɹ�
			*/
			static IniFile* OpenIniFile(const _TCHAR* IniFileName);

			/** �ı�д��ʽ��һ�������ļ�����INI�ļ�
			*
			* @param IniFileName ��ȫ·����ʽ�ļ���
			* @return 
			* 	- -1 ʧ��
			* 	- 0 �ɹ�
			*/
			virtual long WriteIniFile(const _TCHAR* IniFileName) =0;

			/** ���������󣬹رո�INI�ļ�
			*/
			virtual void Release() =0;

			/**��������ļ�
			 *	@param fp �ļ�ָ��
			*/
			virtual void DumpIni(FILE* fp) =0;
			/*
			suopengfei add it 
			*/
			/** ��ȡָ��INI�ļ��а����Ľڵ�����
			*
			* @return �ڵ�����
			*/
			virtual long GetSectionNum() =0;
		};
	}
}
#endif
/** @} */
