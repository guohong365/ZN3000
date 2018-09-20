#include "stdafx.h"
#include "SerialPortSampler.h"
#include "Packet.h"

SerialPortSampler::
SerialPortSampler(const CString& portName, DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits):
	_serialPort(portName, baudRate, dataBits, parity, stopBits),
	_hThread(nullptr), _dwThreadId(0), _buffers(nullptr),
	_quit(false)
{
}

bool SerialPortSampler::begin()
{
	_quit = false;
	_hThread = CreateThread(nullptr, 0, samplerFunc, this, 0, &_dwThreadId);
	return _hThread != nullptr;
}

void SerialPortSampler::stop()
{
	_quit=true;
	WaitForSingleObject(_hThread, INFINITE);
}

SerialPortSampler::~SerialPortSampler()
{
	delete[] _buffers;
}

DWORD SerialPortSampler::samplerFunc(LPVOID lpParam)
{
	SerialPortSampler* pThis = static_cast<SerialPortSampler*>(lpParam);
	BYTE buffer[sizeof(Packet) * 2];
	buffer[0] = 0xCA;
	SIZE_T index = 0;
	bool matchHeader = false;
	bool matchTail = false;
	BYTE byte;
	while (!pThis->_quit)
	{
		if (pThis->_serialPort.read(&byte, 1) != 1)
			return DWORD(-1);
		if (byte == 0xF1)
		{
			if (index == 0)
			{
				matchTail = true;
			}
			else if (index < sizeof(Packet) - 1)
			{
				buffer[index] = byte;
				index ++;
			}
			else if (index == sizeof(Packet) - 1)
			{
				DataBuffer* pPacket = reinterpret_cast<DataBuffer*>(buffer);
				if (checkPacket(pPacket))
				{
					//TODO
					//addData(buffer);
					//reset();						
				}
				else
				{
					//TODO
					int pos = 1; //tryScanHeader(buffer);
					if (pos != -1)
					{
						memmove(buffer, buffer + pos, index - pos + 1);
						index = index - pos + 1;
					}
					else
					{
						buffer[index] = byte;
						index ++;
					}
				}
			}
			else
			{
				buffer[index] = byte;
				index++;
				continue;
			}
		}
		else if (byte == 0xCA) //疑似header
		{
			if (matchTail)
			{
				index ++;
				matchTail = false;
			}
		}
		else
		{
			if (index == 0) //buffer中第一个字节
			{
				buffer[0] = byte;
				index ++;
				matchHeader = true;
			}
			else if (index < sizeof(Packet)) //帧内字节
			{
				buffer[index] = byte;
				index ++;
			}
		}
	}
	return 0;
}
