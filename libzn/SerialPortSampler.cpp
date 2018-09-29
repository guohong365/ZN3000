#include "stdafx.h"
#include "SerialPortSampler.h"
#include "Packet.h"
#include <stdexcept>

#define PACKET_SIZE sizeof(Packet)
#define TEST_BUFFER_SIZE (PACKET_SIZE*3)
SerialPortSampler::SerialPortSampler(
	const CString& portName, 
	DWORD baudRate, 
	BYTE dataBits, 
	BYTE parity, 
	BYTE stopBits)
	: AbstractSampler(4)
	, _serialPort(portName, baudRate, dataBits, parity, stopBits)
{
}

bool SerialPortSampler::onBeforeStart()
{
	return _serialPort.open();
}

bool SerialPortSampler::setMode(const BYTE mode)
{
	CommandPacket command = {0xCB, mode, 0, 0, 0xF1};
	command.CheckSum = 0xCB + mode;
	pause();
	Sleep(200);
	const DWORD ret = _serialPort.write(&command, sizeof(CommandPacket));
	resetAll();
	resume();
	return ret == sizeof(CommandPacket);
}

long SerialPortSampler::scanPacket(const unsigned char* pBuffer, const long size)
{
	long index=0;
	do
	{
		if(pBuffer[index]== 0xCA && 
			size >=index + PACKET_SIZE -1 && 
			pBuffer[index + PACKET_SIZE -1]==0xF1 && 
			checkPacket(reinterpret_cast<const DataBuffer*>(pBuffer + index)))
		{
			return index;
		}
		index ++;
		
	}
	while(index < size);
	return -1;
}


int SerialPortSampler::synchronizePacket() const
{
	BYTE buffer[TEST_BUFFER_SIZE]={0};
	if(_serialPort.read(buffer, TEST_BUFFER_SIZE)!= TEST_BUFFER_SIZE)
	{
		return -1;
	}
	const int index = scanPacket(buffer, TEST_BUFFER_SIZE);
	const int left=PACKET_SIZE - index%PACKET_SIZE;
	if(index >= 0 && _serialPort.read(buffer, left)==left)
	{
		return 0;
	}
	return -2;
}

SerialPortSampler::~SerialPortSampler()
{
}

void SerialPortSampler::onBeforeLoop()
{
	if(synchronizePacket()!=0)
	{
		//TODO need better method
		throw std::runtime_error("ERR_SYNCHRONIZE_PACKET");
	}
}
void SerialPortSampler::onResume()
{
	_serialPort.clear();
	if(synchronizePacket()!=0)
	{
		//TODO need better method
		throw std::runtime_error("ERR_SYNCHRONIZE_PACKET");
	}
}
void SerialPortSampler::doSampler(){
	BYTE buffer[PACKET_SIZE];
	if (_serialPort.read(&buffer, PACKET_SIZE) != PACKET_SIZE)
	{
		//TODO process error.
		//return ERR_READ_FROM_COM;
	}
	DataBuffer* pPacket = reinterpret_cast<DataBuffer*>(buffer);
	if (checkPacket(pPacket))
	{
		revertPacket(pPacket);
		if(get(FEEDBACK_INDEX))
		{
			get(FEEDBACK_INDEX)->append(pPacket->Paket.Feedback);
		}
		if(get(ADMITTANCE_INDEX))
		{
			get(ADMITTANCE_INDEX)->append(pPacket->Paket.Admittance);
		}
		if(get(DIFFERENTIAL_INDEX))
		{
			get(DIFFERENTIAL_INDEX)->append(pPacket->Paket.Differential);
		}
		if(get(ECG_INDEX))
		{
			get(ECG_INDEX)->append(pPacket->Paket.ECG);
			
		}
	}
}
