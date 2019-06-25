#include "stdafx.h"
#include <uc/utils/utility.h>
#include <time.h>
#include <clocale>
#include <tchar.h>
#include <string>
#define BUFFER_LEN 1024

TCHAR *UpperString(TCHAR *str)
{
	TCHAR *p=str;
	while(*p)
	{
#ifndef UNICODE
		if(__isascii(*p) && islower(*p))
#else 
		if(iswascii(*p) && iswlower(*p))
#endif
		{
			*p=toupper(*p);
		}
		p++;
	}
	return str;
}
/*
将字符串全部变成小写
*/
TCHAR *LowerString(TCHAR *str)
{
	TCHAR *p=str;
	while(*p)
	{
#ifndef UNICODE
		if(__isascii(*p) && islower(*p))
#else 
		if(iswascii(*p) && iswlower(*p))
#endif
		{
			*p=tolower(*p);
		}
		p++;
	}
	return str;
}

TCHAR *Trim(TCHAR *str)
{
	TCHAR *p;
	p=str;
	while(*p==_T(' ') || *p==_T('\t') || *p==_T('\n') || *p==_T('\r'))
		p++;
	memmove(str, p, (_tcslen(p)+1)*sizeof(TCHAR));
	p=str+_tcslen(str)-1;
	while(*p==_T(' ') || *p==_T('\t') || *p==_T('\n') || *p==_T('\r'))
		p--;
	p[1]=_T('\0');
	p=str;
	return p;
}

void BufferXOR(void * result, const void * buffer1,  const void * buffer2, const size_t size)
{
	unsigned char*pResult=static_cast<unsigned char *>(result);
	const unsigned char*pBuffer1=static_cast<const unsigned char*>(buffer1);
	const unsigned char*pBuffer2=static_cast<const unsigned char*>(buffer2);
    for(size_t i=0; i< size; i++)
    {
        pResult[i] = pBuffer1[i]^pBuffer2[i];
    }
}

bool Digital2BCD(const unsigned __int64 digital, void * pBCD, const size_t size)
{
    unsigned __int64 capacity=100;
	unsigned char *pBytes=reinterpret_cast<unsigned char*>(pBCD);
	if(size == 0)
        return false;
    if(size < 10)
    {
        for (size_t i=0; i< size; i++)
        {
            capacity *=100;            
        }
        if(capacity < digital)
        {
            return false;
        }
    }
    memset(pBCD, 0, size);
    unsigned __int64 quotient = digital;
    for(int i=0; i< 20; i++)
    {
	    const unsigned __int64 remainder = quotient % 10;
        quotient= quotient/10;
        if(quotient==0 && remainder==0)
            break;
        BYTE bcd = BYTE(remainder);
        if(i%2)
        {
            bcd= bcd<<4;
            pBytes[size - i/2 -1] |= bcd;
        }
        else
        {
            pBytes[size - i/2 -1]=bcd;
        }
    }
    return true;
}

bool BCD2Digital(const void* pBCD, const size_t size, unsigned __int64 & result)
{
	const unsigned char * pBytes=static_cast<const unsigned char*>(pBCD);
    bool first=true;
    BYTE bcd;
	const int count = size * 2;
    result=0;
    for(int i=0; i< count; i++)
    {
        if(i%2==0)
        {
            bcd=pBytes[i/2] & 0xF0;
            bcd=bcd >> 4;
        }
        else
        {
            bcd=pBytes[i/2] & 0x0F;
        }
        if(bcd > 9)
            return false;
        if(bcd ==0 && first)
            continue;
        if(first)
        {
            first=false;
            if(count - i > 20)  //overflow, <=20 ignored, maybe overflow.
                return false;
        }
        result *=10;
        result += bcd;
    }
    return true;
}

bool BCD2String(const BYTE * pBCD, size_t size, char *buffer, size_t length)
{
    buffer[0]=0;
    if(size*2 >= length)
        return false;
    size_t count= size * 2;
    BYTE bcd;
    for(size_t i=0; i< count; i++)
    {
        bcd=pBCD[i/2];
        if(i%2==0)
        {
            bcd=(bcd & 0xF0) >> 4;
        }
        else
        {
            bcd=bcd & 0x0F;
        }
        if(bcd > 9)
            return false;
        buffer[i]=(char)(0x30 + bcd);
        buffer[i+1]=0;
    }
    return true;
}

bool String2BCD(const char * buffer, BYTE * pBCD, size_t size)
{
    memset(pBCD, 0, size);
    size_t count =strlen(buffer);
    size_t dcount=size * 2;
    if(dcount  < count)
        return false;
    BYTE bcd;
    size_t byte = 0;
    size_t i=count;
    do
    {
        i--;
        bcd = buffer[i] - '0';
        if(bcd > 9)
        {
            return false;
        }
        byte =size - (count - i -1)/2 - 1;

        if(dcount%2)
        {
            pBCD[byte] |= (bcd <<4);
        }
        else
        {
            pBCD[byte] = bcd;
        }
        dcount --; 
    }while(i!=0);
    return true;
}

bool Bin2HEXString(const BYTE * bin, size_t size, char *str, size_t length)
{
    size_t count = size * 2;
    if(count > length + 1)
        return false;
    char *p=str;
    BYTE ch;
    for(size_t i=0; i< size; i++ )
    {
        ch= (bin[i] & 0xF0) >> 4;
        if(ch < 10)
        {
            ch +='0';
        }
        else if( ch < 16)
        {
            ch =ch + 'A' - 0xA;
        }
        *p=ch;
        p ++;
        ch=bin[i] & 0x0F;
        if(ch < 10)
        {
            ch +='0';
        }
        else if( ch < 16)
        {
            ch =ch + 'A' - 0xA;
        }
        *p=ch;
        p++;
    }
    *p=0;
    return true;
}
bool HEXString2Bin(const char * str, BYTE * bin, size_t size)
{
    size_t count= strlen(str);
    const char * p = str + count -1;

    if(count%2 ? (count + 1)/2 : count/2 > size)
        return false;
    BYTE ch;
    BYTE byte;
    BYTE *b=bin + size - 1;
    memset(bin, 0, size);
    size_t i;
    for( i=0; i< count; i++)
    {
        ch=*p;
        if(ch >= '0' && ch <= '9')
        {
            ch= ch - '0';
        }
        else if(ch >= 'a' && ch <= 'f')
        {
            ch = ch - 'a' + 0x0A;
        }
        else if(ch >= 'A' && ch <= 'F')
        {
            ch= ch - 'A' + 0x0A;
        }
        else
        {
            return false;
        }
        if( i % 2 != 0 )
        {
            byte |= (ch << 4);
            *b=byte;
            b--;
        }
        else
        {
            byte = ch;
        }
        p--;
    }
    if( i % 2 != 0 )
    {
        *b=byte;
    }
    return true;
}
void genRandomHEXString(char * pwd, size_t length)
{
    srand(unsigned int(time(nullptr)));
    for(size_t i=0; i <length -1; i++)
    {
	    const int val= rand()%16;
        if(val <=0)
        {
            pwd[i]= val + '0';
        }
        else
        {
            pwd[i]= val + 'A';
        }
    }
    pwd[length -1]=0;
}
static const char* ChNumberA[]={"零","一","二","三","四","五","六","七","八","九"};

std::string StringDigitalToChCharA(const char *text)
{
	std::string retVal;
    int index = 0;
	const size_t length=strlen(text);
	if(text[0]=='-')
	{
		retVal.append("负");
		index=1;
	}
	if(text[0]=='+')
	{
		index=1;
	}
    for(size_t i=index; i< length; i++)
    {
		if(!isdigit(text[i])) break;
        retVal.append(ChNumberA[text[i]-'0']);        
    }
    return retVal;
}

static const wchar_t ChNumberW[]={L'零',L'一',L'二',L'三',L'四',L'五',L'六',L'七',L'八',L'九'};

std::wstring StringDigitalToChCharW(const wchar_t *text)
{
	setlocale(LOCALE_ALL, "chs");
	std::wstring retVal;
    int index = 0;
	const size_t length=wcslen(text);
	if(text[0]=='-')
	{
		retVal.append(L"负");
		index=1;
	}
	if(text[0]==L'+')
	{
		index=1;
	}
    for(size_t i=index; i< length; i++)
    {
		if(!isdigit(text[i])) break;
        retVal.append(1, ChNumberW[text[i]-L'0']);        
    }
    return retVal;
}
bool StringDigitalToChChar(const TCHAR *text, TCHAR* output, size_t length)
{
#ifdef _UNICODE
    std::wstring retVal=StringDigitalToChCharW(text);
#else
	std::string retVal=StringDigitalToChCharA(text);
#endif
	if(length > retVal.length() + 1)
	{
		_tcscpy_s(output,length, retVal.c_str());
		return true;
	}
    return false;
}
std::wstring DigitalToChChar(long digital)
{
	wchar_t buffer[20]={0};
    wsprintf(buffer, L"%d", digital);
    return StringDigitalToChCharW(buffer);
}
/*
std::wstring NumberToChChar(long number)
{
    WCHAR *unit[]={
        L"",
        L"十",
        L"百",
        L"千",
        L"万",
        L"",
        L"十",
        L"百",
        L"千",
        L"亿",
        L"",
        L"十"
    };
    bool begin=true;
    bool zero=false;
    wchar_t text[20];
    std::wstring retVal;
    std::wstring temp;
    wsprintf(text, L"%d", number);
	int length=wcslen(text);
    int start=length -1;
    int end= text[0]==L'-' ? 1 : 0;
    int count= length - start;
    std::wstring prefix= text[0]==L'-'? L"负":L"";
    if(count==1)
        return prefix.append(StringDigitalToChCharW(text + start));
    if(count==2 && text[start-1]==L'1' && text[start] ==L'0' )
        return prefix.append(L"+");

    //insert unit
    for (int i=start; i>=end; i-- )
    {
        if(begin)
        {
            if( text[i] == L'0')
                continue;
            begin=false;
        }
        else
        {
            if(text[i]==L'0')
            {
                zero=true;
                continue;
            }
        }
        if(zero)
        {
            retVal.Format(L"%c%c零%s", text[i] , unit[start-i] ,  retVal);
            //maybe
            //temp.Format(L"%c%c%c%s", text[i] , unit[start-i] , L'零',  retVal);
            //retVal=temp;
            zero=false;
        }
        else
        {
            retVal.Format(L"%c%c%s", text[i],  unit[start-i] , retVal);
        }
    }
    return StringDigitalToChCharW(retVal);
}
*/
void GenerateRandomHEXString(char * pwd, size_t length)
{
	static unsigned int added=7;
	srand(static_cast<unsigned int>(time(nullptr)) + added++);
	for(size_t i=0; i <length -1; i++)
	{
		const int val= rand()%16;
		if(val <=0)
		{
			pwd[i]= val + '0';
		}
		else
		{
			pwd[i]= val + 'A';
		}
	}
	pwd[length -1]=0;
}
void GenerateRandomBuffer(unsigned char* buffer, const size_t bufLen)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	for(size_t i=0; i< bufLen; i++)
	{
		buffer[i]=char(rand());
		while(buffer[i]==0)
		{
			buffer[i]=char(rand());
		}
	}
}
void GenerateRandomOrderMap(unsigned char * mapBuffer, size_t bufLen, size_t maxNum)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	int pos;
	const size_t realNum= bufLen > maxNum ? maxNum : bufLen;
	memset(mapBuffer, 0xFF, bufLen);
	for(unsigned char i=0; i< realNum; i++)
	{
		pos = rand()%realNum;
		while(mapBuffer[pos]==0xFF)
		{
			pos=rand()%realNum;
		}
		mapBuffer[pos] = i;
	}
}

//************************************
// Method:    CatBits  将输入字节数组按 bits 指定的位拼接到输出数组中
// FullName:  CatBits
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: unsigned char * buffer 输出地址
// Parameter: unsigned char * map_buffer 输入数据
// Parameter: size_t bits   每字节截取的位数
// Parameter: size_t count  拼接字节数
//************************************
void CompressBits( unsigned char * buffer, const unsigned char * mapBuffer, size_t bits, size_t count )
{
	size_t startByte=0;
	size_t startBit=0;
	const unsigned char clrMask= 0xFF >> (8-bits);
	memset(buffer, 0, count*bits /8 + (count * bits)%8 ==0 ? 0:1);
	for(size_t i=0; i< count; i++)
	{
		startByte= i* bits/8;
		startBit= (i*bits)%8;

		const unsigned char val = mapBuffer[i] & clrMask; //高位清0
		if(8 - startBit >= bits)
		{
			buffer[startByte] |= (val << (8-startBit - bits));
		}
		else
		{	
			buffer[startByte] |=(val >> (startBit + bits - 8));
			buffer[startByte + 1] |= val << (16 - startBit - bits);
		}
	}
}

void DecompressBis( unsigned char* mapBuffer,const unsigned char* buffer, size_t bits, size_t count )
{
	unsigned char val;
	unsigned char clrMask;
	memset(mapBuffer, 0, count);
	for(size_t i=0; i< count; i++)
	{
		const size_t startByte = i * bits / 8;
		const size_t startBit = (i * bits) % 8;
		if(8 - startBit >= bits)
		{
			clrMask= (0xFF >> startBit) & (0xFF << (8 - startBit - bits));
			val= buffer[startByte] & clrMask;
			val = val >> (8 - startBit - bits);
			mapBuffer[i]=val;
		}
		else
		{
			clrMask= 0xFF >> startBit;
			unsigned char hi = buffer[startByte] & clrMask;
			hi = hi << (startBit + bits - 8);
			clrMask = 0xFF << (8 - (bits -(8-startBit)));
			unsigned char low = buffer[startByte + 1] & clrMask;

			low = low >> (8 - (bits -(8-startBit)));
			val = hi | low;
			mapBuffer[i]=val;
		}
	}
}

void ReorderBytes(unsigned char * outBuffer,const unsigned char * inBuffer, unsigned char * mapBuffer, size_t count)
{
	for(size_t i=0; i < count; i++)
	{
		outBuffer[i]=inBuffer[mapBuffer[i]];
	}
}

void RestoreBytesOrder(unsigned char * outBuffer,const unsigned char * inBuffer, unsigned char * mapBuffer, size_t count)
{
	for (size_t i=0; i< count; i++)
	{
		outBuffer[mapBuffer[i]]=inBuffer[i];
	}
}

unsigned short RevertUInt16(unsigned short input)
{
	unsigned short output;
	unsigned char * p0=reinterpret_cast<unsigned char*>(&input);
	unsigned char * p1=reinterpret_cast<unsigned char*>(&output);
	p1[0]=p0[1];
	p1[1]=p0[0];
	return output;
}

short RevertInt16(short input)
{
	return RevertUInt16(input);
}

unsigned long RevertUInt32(unsigned long input)
{
	unsigned long output;
	unsigned char * p0=reinterpret_cast<unsigned char*>(&input);
	unsigned char * p1=reinterpret_cast<unsigned char*>(&output);
	p1[0]=p0[3];
	p1[1]=p0[2];
	p1[2]=p0[1];
	p1[3]=p0[0];
	return output;
}

long RevertInt32(long input)
{
	return RevertUInt32(input);
}

std::wstring mbstows(const std::string& str, const char* locale)
{
    const std::locale old;
    std::locale::global(std::locale(locale));
    const size_t size = 1 + mbstowcs(NULL, str.c_str(), 0);
    wchar_t* dest = new wchar_t[size];
    wmemset(dest, 0, size);
    mbstowcs(dest,str.c_str(),size);
    std::wstring result = dest;
    delete []dest;
    std::locale::global(old);
    return result;
}
std::string wstombs(const std::wstring& wstr, const char * locale)
{
    const std::locale old;
    std::locale::global(std::locale(locale));
    const size_t size = wcstombs(NULL, wstr.c_str(), 0) + 1;
    char *dest = new char[size];
    memset(dest,0,size);
    wcstombs(dest,wstr.c_str(),size);
    std::string result = dest;
    delete []dest;
    std::locale::global(old);
    return result;
}