/** \addtogroup BaseClass
@{
*/
#ifndef __TRANS_UTILITY__
#define __TRANS_UTILITY__
#include <uc/libuc.h>
#include <locale>

namespace uc
{
	namespace utils
	{
		/**���ַ���ת��Ϊ��д
		* 
		* @param str ��ת�����ַ�����ͬʱ����ת�����
		* @return ת�����ָ��
		* @remark �ú�����ԭ�ַ����ϲ������������ݽ���ı䡣�ַ�������'\0'����
		*/
		LIB_UC_API TCHAR* UpperString(TCHAR* str);
		/**���ַ���ȫ�����Сд
		 *
		 *@param str ��ת���ַ���
		 *@return ת�����ָ��
		 *@remark �ú�����ԭ�ַ����ϲ������������ݽ���ı䡣�ַ�������'\0'����
		*/
		LIB_UC_API TCHAR* LowerString(TCHAR* str);

		/**ȥ���ַ����������˵Ŀհ׷���\\t������\\n������ ��
		*
		* @param str ��������ַ�����ͬʱ���ش�����
		* @return ������
		* @remark �ú�����ԭ�ַ����ϲ������������ݽ���ı䡣�ַ�������'\0'����
		*/
		LIB_UC_API TCHAR* Trim(TCHAR* str);

		/**�������������а��ַ���һλ�򣬱��浽���������
		 *
		 *@param result ���������
		 *@param buffer1 ���뻺����֮һ
		 *@param buffer2 ���뻺����֮һ
		 *@param size �����������С�������ֽ�λ��ĳ���
		 *@remark result��buffer1��buffer2�ѷ����ڴ�ĳ��ȱ�����ڵ���size��������ܻᵼ�»�����������������
		 */
		LIB_UC_API void BufferXOR(void* result, const void* buffer1, const void* buffer2, size_t size);
		/**���޷���64λ����ת��λBCD��
		 *
		 *BCD�밴���Ե�λ����λ���������ŷţ���λ��������0
		 *���磺Digital2BCD(1234, pBCD, 4) pBCD�н��Ϊ 0x00 0x00 0x12 0x34 
		 *@param digital ��ת������
		 *@param pBCD ���������������BCD��ת���������С��sizeָ����
		 *@param size ���������pBCD�����ֽڳ���
		 *@return false ���������sizeָ���Ļ�������������ת�������true ת���ɹ�
		 *@remark ����falseʱ��pBCD����������ı䣬�������Ԥ��
		 */
		LIB_UC_API bool Digital2BCD(unsigned __int64 digital, void* pBCD, size_t size);
		/**��BCD��ת��Ϊ64λ�޷�������
		 *@param pBCD ��תBCD�뻺��
		 *@param size ��תBCD���ֽ���
		 *@param result �������
		 *@return false ����BCD���ʽ����ȷ
		 *@remark �ú���δ�������������size����ʱ���������������
		 */
		LIB_UC_API bool BCD2Digital(const void* pBCD, size_t size, unsigned __int64& result);
		LIB_UC_API bool BCD2String(const unsigned char* pBCD, size_t size, char* buffer, size_t length);
		LIB_UC_API bool String2BCD(const char* buffer, unsigned char* pBCD, size_t size);
		LIB_UC_API bool Bin2HEXString(const unsigned char* bin, size_t size, char* str, size_t length);
		LIB_UC_API bool HEXString2Bin(const char* str, unsigned char* bin, size_t size);
		LIB_UC_API void genRandomHEXString(char* pwd, size_t length);
		LIB_UC_API bool StringDigitalToChChar(const TCHAR* text, TCHAR* output, size_t length);
		LIB_UC_API bool DigitalToChChar(long digital, TCHAR* output, size_t size);
		//CStringW NumberToChChar(long number);
		LIB_UC_API void GenerateRandomHEXString(char* pwd, size_t length);
		LIB_UC_API void GenerateRandomBuffer(unsigned char* buffer, size_t bufLen);
		LIB_UC_API void GenerateRandomOrderMap(unsigned char* mapBuffer, size_t bufLen, size_t maxNum);
		LIB_UC_API void DecompressBis(unsigned char* mapBuffer, const unsigned char* buffer, size_t bits, size_t count);
		LIB_UC_API void CompressBits(unsigned char* buffer, const unsigned char* mapBuffer, size_t bits, size_t count);
		LIB_UC_API void ReorderBytes(unsigned char* outBuffer, const unsigned char* inBuffer, unsigned char* mapBuffer,
		                             size_t count);
		LIB_UC_API void RestoreBytesOrder(unsigned char* outBuffer, const unsigned char* inBuffer,
		                                  unsigned char* mapBuffer, size_t count);
		LIB_UC_API unsigned short RevertUInt16(unsigned short input);
		LIB_UC_API short RevertInt16(short input);
		LIB_UC_API unsigned long RevertUInt32(unsigned long input);
		LIB_UC_API long RevertInt32(long input);
		inline
		LIB_UC_API std::wstring mbstows(const std::string& str, const char* locale = "chs");
		LIB_UC_API std::string wstombs(const std::wstring& str, const char* locale = "chs");
	}
}
#endif
/** @} */
