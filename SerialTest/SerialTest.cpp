// SerialTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windef.h>
#include <winbase.h>

#include <cerrno>
#include "../libzn/Packet.h"
#include "../libzn/SerialPort.h"

#define LEN_BUFFER 10240

void parseBuffer(BYTE * buffer,BYTE * data, size_t packetCount)
{
	ULONG pos=0;
	ULONG index=0;
	do
	{
		if(buffer[pos]==0xCA && 
			buffer[pos + sizeof(Packet) -1]==0xF1) // maybe a packet
		{
			Packet * pPacket=reinterpret_cast<Packet*>(buffer + pos);
			BYTE sum=0;
			BYTE* pByte=reinterpret_cast<BYTE*>(data + index);
			for(ULONG j=0; j < sizeof(Packet) - 2; j ++)
			{
				sum += buffer[pos + j];
			}
			if(pPacket->CheckSum== sum) //is packet
			{
				memcpy(pByte, buffer + pos + 4, 8);
				index += 8;
				pos += sizeof(Packet);
				continue;
			}
		}
		pos ++;
	}while(pos < packetCount * sizeof(Packet));		
}

void revert(BYTE* data, long count)
{
	long index=0;
	for(int i=0; i< count; i++)
	{
		BYTE byte = data[index];
		data[index] = data [index + 1];
		data[index + 1] = byte;
		index +=2;
		byte = data[index];
		data[index] = data [index + 1];
		data[index + 1] = byte;
		index +=2;
		byte = data[index];
		data[index] = data [index + 1];
		data[index + 1] = byte;
		index +=2;
		byte = data[index];
		data[index] = data [index + 1];
		data[index + 1] = byte;
		index +=2;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	CommandPacket cmd=CHECK_COMMAND;

	SerialPort serialPort(_T("COM2"), 500000);
	Packet buffer[LEN_BUFFER];
	memset(buffer, 0, LEN_BUFFER * sizeof(Packet));
	if(!serialPort.open())
	{
		_tprintf(_T("open serial error %d"), GetLastError());
	}
	DWORD send= serialPort.write(&cmd ,sizeof(CommandPacket));
	LARGE_INTEGER frequency;
	LARGE_INTEGER begin, end;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&begin);
	for(long i=0; i< LEN_BUFFER; i+=1)
	{
		if(serialPort.read(buffer + i, sizeof(Packet))!=sizeof(Packet))
		{
			_tprintf(_T("read serial error %d"), GetLastError());
			serialPort.close();
			return -1;
		}
	}
	QueryPerformanceCounter(&end);
	__int64 ns=(end.QuadPart-begin.QuadPart)*1000000 /frequency.QuadPart;
	double dns=ns/1000000.0;
	_tprintf(_T("Elapsed %lld, %f"), ns, dns);
	serialPort.close();
	FILE * fp;
	_tfopen_s(&fp, _T("data1.dat"), _T("wb"));
	if(fp==nullptr)
	{
		_tprintf(_T("open file 1 error %d"), errno);
		return -2;
	}
	fwrite(buffer, sizeof(Packet),LEN_BUFFER, fp);
	fclose(fp);
	_tprintf(_T("file 1 write ok!"));
	/*
	BYTE data[LEN_BUFFER * sizeof(short) * 4];
	parseBuffer(reinterpret_cast<BYTE*>(buffer), data, LEN_BUFFER);
	
	_tfopen_s(&fp, _T("data2.dat"), _T("wb"));
	if(fp==nullptr)
	{
		_tprintf(_T("open file 2 error %d"), errno);
		return -2;
	}
	fwrite(data, sizeof(short)*4, LEN_BUFFER, fp);
	fclose(fp);
	revert(data, LEN_BUFFER);
	_tfopen_s(&fp, _T("data3.dat"), _T("wb"));
	if(fp==nullptr)
	{
		_tprintf(_T("open file 3 error %d"), errno);
		return -2;
	}
	fwrite(data, sizeof(short)*4, LEN_BUFFER, fp);
	fclose(fp);

	short *pShort=reinterpret_cast<short*>(data);
	for(long int i=0; i<LEN_BUFFER * 4; i+=4)
	{
		pShort[i] = short(static_cast<long>(*reinterpret_cast<unsigned short*>(data + i)) - 32767);
	}
		_tfopen_s(&fp, _T("data4.dat"), _T("wb"));
	if(fp==nullptr)
	{
		_tprintf(_T("open file 4 error %d"), errno);
		return -2;
	}
	fwrite(data, sizeof(short)*4, LEN_BUFFER, fp);
	fclose(fp);
	*/
	return 0;
}

