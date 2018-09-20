#include "StdAfx.h"
#include "utility.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
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

void BufferXOR(BYTE * result, const BYTE * buffer1,  const BYTE * buffer2, size_t size)
{
    for(size_t i=0; i< size; i++)
    {
        result[i] = buffer1[i]^buffer2[i];
    }
}

bool Decimal2BCD(unsigned __int64 decimal, BYTE * pBCD, size_t size)
{
    unsigned __int64 capcity=100;
    unsigned __int64 quotient, remainder;
    BYTE bcd;
    if(size == 0)
        return false;
    if(size < 10)
    {
        for (size_t i=0; i< size; i++)
        {
            capcity *=100;            
        }
        if(capcity < decimal)
        {
            return false;
        }
    }
    memset(pBCD, 0, size);
    quotient=decimal;
    for(int i=0; i< 20; i++)
    {
        remainder = quotient %10;
        quotient= quotient/10;
        if(quotient==0 && remainder==0)
            break;
        bcd=(BYTE)remainder;
        if(i%2)
        {
            bcd= bcd<<4;
            pBCD[size - i/2 -1] |= bcd;
        }
        else
        {
            pBCD[size - i/2 -1]=bcd;
        }
    }
    return true;
}

bool BCD2Decimal(const BYTE* pBCD, size_t size, unsigned __int64 & result)
{
    bool first=true;
    BYTE bcd;
    int count = size * 2;
    result=0;
    for(int i=0; i< count; i++)
    {
        if(i%2==0)
        {
            bcd=pBCD[i/2] & 0xF0;
            bcd=bcd >> 4;
        }
        else
        {
            bcd=pBCD[i/2] & 0x0F;
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
    srand(time(nullptr));
    for(size_t i=0; i <length -1; i++)
    {
        int val= rand()%16;
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

CStringW StringDigitalToChChar(const CStringW &text)
{
    CStringW retVal;
    for(int i=0; i< text.GetLength(); i++)
    {
        switch(text[i])
        {
        case L'-':
            {			
                if(i==0 && isdigit(text[i]))
                    retVal+=L"负";
                else
                    retVal+=text[i];
            }
            break;
        case L'0':
            retVal+=L"零";
            break;
        case L'1':
            retVal+=L"一";
            break;
        case L'2':
            retVal+=L"二";
            break;
        case L'3':
            retVal+=L"三";
            break;
        case L'4':
            retVal+=L"四";
            break;
        case L'5':
            retVal+=L"五";
            break;
        case L'6':
            retVal+=L"六";
            break;
        case L'7':
            retVal+=L"七";
            break;
        case L'8':
            retVal+=L"八";
            break;
        case L'9':
            retVal+=L"九";
            break;
        default:
            retVal +=text[i];
            break;
        }
    }
    return retVal;
}
CStringW DigitalToChChar(long digital)
{
    CStringW text;
    CStringW retVal;
    text.Format(L"%d", digital);
    return StringDigitalToChChar(text);
}

CStringW NumberToChChar(long number)
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
    CStringW text;
    CStringW retVal;
    CStringW temp;
    text.Format(L"%d", number);
    int start=text.GetLength()-1;
    int end= text[0]==L'-' ? 1 : 0;
    int count= text.GetLength() - start;
    CStringW prefix= text[0]==L'-'? L"负":L"";
    if(count==1)
        return StringDigitalToChChar(prefix + text[start]);
    if(count==2 && text[start-1]==L'1' && text[start] ==L'0' )
        return prefix + L'十';

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
    return StringDigitalToChChar(retVal);
}
void GenerateRandomHEXString(char * pwd, size_t length)
{
	static unsigned int added=7;
	srand((unsigned int)time(NULL) + added++);
	for(size_t i=0; i <length -1; i++)
	{
		int val= rand()%16;
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
void GenerateRandomBuffer(unsigned char* buffer, size_t buf_len)
{
	srand((unsigned int)time(NULL));
	for(size_t i=0; i< buf_len; i++)
	{
		buffer[i]=(char)rand();
		while(buffer[i]==0)
		{
			buffer[i]=(char)rand();
		}
	}
}
void GenerateRandomOrderMap(unsigned char * map_buffer, size_t buf_len, size_t max_num)
{

	ASSERT(max_num < 255);
	srand((unsigned int)time(NULL));
	int pos;
	size_t real_num= buf_len > max_num ? max_num : buf_len;
	memset(map_buffer, 0xFF, buf_len);
	for(unsigned char i=0; i< real_num; i++)
	{
		pos = rand()%real_num;
		while(map_buffer[pos]==0xFF)
		{
			pos=rand()%real_num;
		}
		map_buffer[pos] = i;
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
void CompressBits( unsigned char * buffer, const unsigned char * map_buffer, size_t bits, size_t count )
{
	size_t start_byte=0;
	size_t start_bit=0;
	unsigned char clr_mask= 0xFF >> (8-bits);
	unsigned char val;
	memset(buffer, 0, count*bits /8 + (count * bits)%8 ==0 ? 0:1);
	for(size_t i=0; i< count; i++)
	{
		start_byte= i* bits/8;
		start_bit= (i*bits)%8;
		
		val = map_buffer[i] & clr_mask; //高位清0
		if(8 - start_bit >= bits)
		{
			buffer[start_byte] |= (val << (8-start_bit - bits));
		}
		else
		{	
			buffer[start_byte] |=(val >> (start_bit + bits - 8));
			buffer[start_byte + 1] |= val << (16 - start_bit - bits);
		}
	}
}

void DecompressBis( unsigned char* map_buffer,const unsigned char* buffer, size_t bits, size_t count )
{
	size_t start_byte;
	size_t start_bit;
	unsigned char val;
	unsigned char clr_mask;
	unsigned char hi;
	unsigned char low;
	memset(map_buffer, 0, count);
	for(size_t i=0; i< count; i++)
	{
		start_byte= i*bits/8;
		start_bit= (i*bits)%8;
		if(8 - start_bit >= bits)
		{
			clr_mask= (0xFF >> start_bit) & (0xFF << (8 - start_bit - bits));
			val= buffer[start_byte] & clr_mask;
			val = val >> (8 - start_bit - bits);
			map_buffer[i]=val;
		}
		else
		{
			clr_mask= 0xFF >> start_bit;
			hi = buffer[start_byte] & clr_mask;
			hi = hi << (start_bit + bits - 8);
			clr_mask = 0xFF << (8 - (bits -(8-start_bit)));
			low = buffer[start_byte + 1] & clr_mask;

			low = low >> (8 - (bits -(8-start_bit)));
			val = hi | low;
			map_buffer[i]=val;
		}
	}
}

void ReoderBytes(unsigned char * outbuffer,const unsigned char * inbuffer, unsigned char * map_buffer, size_t count)
{
	for(size_t i=0; i < count; i++)
	{
		outbuffer[i]=inbuffer[map_buffer[i]];
	}
}

void ResotreBytesOrder(unsigned char * outbuffer,const unsigned char * inbuffer, unsigned char * map_buffer, size_t count)
{
	for (size_t i=0; i< count; i++)
	{
		outbuffer[map_buffer[i]]=inbuffer[i];
	}
}
