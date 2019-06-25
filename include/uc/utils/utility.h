/** \addtogroup BaseClass
@{
*/
#ifndef __TRANS_UTILITY__
#define __TRANS_UTILITY__
#include <uc/libuc.h>
#include <locale>

/**将字符串转换为大写
* 
* @param str 待转换的字符串，同时返回转换结果
* @return 转换结果指针
* @remark 该函数在原字符串上操作，输入数据将会改变。字符串必须'\0'结束
*/
LIB_UC_API TCHAR *UpperString(TCHAR *str);
/**将字符串全部变成小写
 *
 *@param str 待转换字符串
 *@return 转换结果指针
 *@remark 该函数在原字符串上操作，输入数据将会改变。字符串必须'\0'结束
*/
LIB_UC_API TCHAR *LowerString(TCHAR *str);

/**去掉字符串左右两端的空白符‘\\t’、‘\\n’、‘ ’
*
* @param str 待处理的字符串，同时返回处理结果
* @return 处理结果
* @remark 该函数在原字符串上操作，输入数据将会改变。字符串必须'\0'结束
*/
LIB_UC_API TCHAR *Trim(TCHAR *str);

/**将两个缓冲区中按字符逐一位或，保存到结果缓冲中
 *
 *@param result 结果缓冲区
 *@param buffer1 输入缓冲区之一
 *@param buffer2 输入缓冲区之一
 *@param size 结果缓冲区大小。即按字节位或的长度
 *@remark result、buffer1及buffer2已分配内存的长度必须大于等于size，否则可能会导致缓冲溢出，程序崩溃。
 */
LIB_UC_API void BufferXOR(void * result, const void * buffer1, const void * buffer2, size_t size);
/**将无符号64位整数转化位BCD码
 *
 *BCD码按按自低位到高位从右自左排放，高位不足的填充0
 *例如：Digital2BCD(1234, pBCD, 4) pBCD中结果为 0x00 0x00 0x12 0x34 
 *@param digital 待转整数。
 *@param pBCD 输出缓冲区，返回BCD码转换结果。大小由size指定。
 *@param size 输出缓冲区pBCD可用字节长度
 *@return false 缓冲溢出，size指定的缓冲区不能容纳转换结果。true 转换成功
 *@remark 返回false时，pBCD缓冲区将会改变，结果不可预测
 */
LIB_UC_API bool Digital2BCD(unsigned __int64 digital, void * pBCD, size_t size);
/**将BCD码转换为64位无符号整数
 *@param pBCD 待转BCD码缓冲
 *@param size 待转BCD码字节数
 *@param result 结果整数
 *@return false 输入BCD码格式不正确
 *@remark 该函数未检查结果溢出情况，size过大时可能造成数字上溢
 */
LIB_UC_API bool BCD2Digital(const void* pBCD, size_t size, unsigned __int64 & result);
LIB_UC_API bool BCD2String(const unsigned char * pBCD, size_t size, char *buffer, size_t length);
LIB_UC_API bool String2BCD(const char * buffer, unsigned char * pBCD, size_t size);
LIB_UC_API bool Bin2HEXString(const unsigned char * bin, size_t size, char *str, size_t length);
LIB_UC_API bool HEXString2Bin(const char * str, unsigned char * bin, size_t size);
LIB_UC_API void genRandomHEXString(char * pwd, size_t length);
LIB_UC_API bool StringDigitalToChChar(const TCHAR *text, TCHAR * output, size_t length);
LIB_UC_API bool DigitalToChChar(long digital, TCHAR* output, size_t size);
//CStringW NumberToChChar(long number);
LIB_UC_API void GenerateRandomHEXString(char * pwd, size_t length);
LIB_UC_API void GenerateRandomBuffer(unsigned char* buffer, size_t bufLen);
LIB_UC_API void GenerateRandomOrderMap(unsigned char * mapBuffer, size_t bufLen, size_t maxNum);
LIB_UC_API void DecompressBis(unsigned char* mapBuffer,const unsigned char* buffer, size_t bits, size_t count);
LIB_UC_API void CompressBits(unsigned char * buffer, const unsigned char * mapBuffer, size_t bits, size_t count);
LIB_UC_API void ReorderBytes(unsigned char * outBuffer,const unsigned char * inBuffer, unsigned char * mapBuffer, size_t count);
LIB_UC_API void RestoreBytesOrder(unsigned char * outBuffer,const unsigned char * inBuffer, unsigned char * mapBuffer, size_t count);
LIB_UC_API unsigned short RevertUInt16(unsigned short input);
LIB_UC_API short RevertInt16(short input);
LIB_UC_API unsigned long RevertUInt32(unsigned long input);
LIB_UC_API long RevertInt32(long input);inline
LIB_UC_API std::wstring mbstows(const std::string& str, const char* locale = "chs");
LIB_UC_API std::string wstombs(const std::wstring& str, const char * locale="chs");
#endif
/** @} */