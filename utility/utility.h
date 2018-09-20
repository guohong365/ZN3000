/** \addtogroup BaseClass
@{
*/
#ifndef __TRANS_UTILITY__
#define __TRANS_UTILITY__

#define BUFFER_LEN 1024

/**
* ���ַ���ת��Ϊ��д
*
* @param str ��ת�����ַ�����ͬʱ����ת�����
* @return ת�����
*/
TCHAR *UpperString(TCHAR *str);
/*
���ַ���ȫ�����Сд
*/
TCHAR *LowerString(TCHAR *str);

/**
* ȥ���ַ����������˵Ŀհ׷���\\t������\\n������ ��
*
* @param str ��������ַ�����ͬʱ���ش�����
* @return ������
*/
TCHAR *Trim(TCHAR *str);
void BufferXOR(BYTE * result, const BYTE * buffer1, const BYTE * buffer2, size_t size);
bool Decimal2BCD(unsigned __int64 decimal, BYTE * pBCD, size_t size);
bool BCD2Decimal(const BYTE* pBCD, size_t size, unsigned __int64 & result);
bool BCD2String(const BYTE * pBCD, size_t size, char *buffer, size_t length);
bool String2BCD(const char * buffer, BYTE * pBCD, size_t size);
bool Bin2HEXString(const BYTE * bin, size_t size, char *str, size_t length);
bool HEXString2Bin(const char * str, BYTE * bin, size_t size);
void genRandomHEXString(char * pwd, size_t length);
CStringW StringDigitalToChChar(const CStringW &text);
CStringW DigitalToChChar(long digital);
CStringW NumberToChChar(long number);
void GenerateRandomHEXString(char * pwd, size_t length);
void GenerateRandomBuffer(unsigned char* buffer, size_t buf_len);
void GenerateRandomOrderMap(unsigned char * map_buffer, size_t buf_len, size_t max_num);
void DecompressBis(unsigned char* map_buffer,const unsigned char* buffer, size_t bits, size_t count);
void CompressBits(unsigned char * buffer, const unsigned char * map_buffer, size_t bits, size_t count);
void ReoderBytes(unsigned char * outbuffer,const unsigned char * inbuffer, unsigned char * map_buffer, size_t count);
void ResotreBytesOrder(unsigned char * outbuffer,const unsigned char * inbuffer, unsigned char * map_buffer, size_t count);
#endif
/** @} */