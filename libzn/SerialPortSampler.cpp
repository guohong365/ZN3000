#include "stdafx.h"
#include "SerialPortSampler.h"
#include "Packet.h"

#define PACKET_SIZE sizeof(Packet)
#define TEST_BUFFER_SIZE (PACKET_SIZE*3)
SerialPortSampler::
	SerialPortSampler(const CString& portName, DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits)
	: _serialPort(portName, baudRate, dataBits, parity, stopBits)
	  , _hThread(nullptr)
	  , _hResume(nullptr)
	  , _dwThreadId(0)
	  , _quit(false)
	  , _paused(false)
	  , _progressCallback(nullptr)
	  , _stateChangedCallback(nullptr)
{
	_buffers[0] = _buffers[1] = _buffers[2] = _buffers[3] = nullptr;
}

bool SerialPortSampler::begin(SamplerProgressCallback *progressCallback, SamplerStateChangedCallback *stateChangedCallback)
{
	_progressCallback=progressCallback;
	_stateChangedCallback=stateChangedCallback;
	if(_serialPort.open())
	{
		_quit = false;
		_hResume = CreateEvent(nullptr, TRUE, false, nullptr);
		_hThread = CreateThread(nullptr, 0, samplerFunc, this, 0, &_dwThreadId);
	}
	return _hThread != nullptr;
}

void SerialPortSampler::pause()
{
	_paused=true;
}

void SerialPortSampler::resume()
{
	if(_paused)
	{
		SetEvent(_hResume);
	}
}

void SerialPortSampler::quit()
{
	_quit=true;
	WaitForSingleObject(_hThread, INFINITE);
}

Sampler::SamplerState SerialPortSampler::getState()
{
	if(_paused) return PAUSED;
	DWORD code=0;
	GetExitCodeThread(_hThread, &code);
	if(code==STILL_ACTIVE) return RUNNING;
	return STOPPED;
}

bool SerialPortSampler::setMode(const BYTE mode)
{
	CommandPacket command = {0xCB, mode, 0, 0, 0xF1};
	command.CheckSum = 0xCB + mode;
	pause();
	Sleep(200);
	const DWORD ret = _serialPort.write(&command, sizeof(CommandPacket));
	resetBuffer();
	resume();
	return ret == sizeof(CommandPacket);
}

void SerialPortSampler::resetBuffer()
{
	if(getState()==RUNNING)
	{
		pause();
		Sleep(200);
	}
	for (int i = 0; i < 4; ++i)
	{
		_buffers[0]->clear();
	}
	resume();
}

SignalBuffer<float>* SerialPortSampler::getBuffer(int index)
{
	return _buffers[index];
}

void SerialPortSampler::attachBuffer(const int index, SignalBuffer<float>* pBuffer)
{
	if(index >=0 && index < 4)
	{
		_buffers[index]=pBuffer;
	}
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
	CloseHandle(_hResume);
	CloseHandle(_hThread);
	delete _progressCallback;
	delete _stateChangedCallback;
}

DWORD SerialPortSampler::samplerFunc(LPVOID lpParam)
{
	SerialPortSampler* pThis = static_cast<SerialPortSampler*>(lpParam);
	BYTE buffer[PACKET_SIZE];
	if(pThis->synchronizePacket()!=0)
	{
		return ERR_SYNCHRONIZE_PACKET;
	}
	while (!pThis->_quit)
	{
		if(pThis->_paused)
		{
			if(WaitForSingleObject(pThis->_hResume, 100)==WAIT_TIMEOUT)
			{
				continue;
			}
			ResetEvent(pThis->_hResume);
			pThis->_paused=false;
			pThis->_serialPort.clear();
			if(pThis->synchronizePacket()!=0)
			{
				return ERR_SYNCHRONIZE_PACKET;
			}
		}
		if (pThis->_serialPort.read(&buffer, PACKET_SIZE) != PACKET_SIZE)
		{
			return ERR_READ_FROM_COM;
		}
		DataBuffer* pPacket = reinterpret_cast<DataBuffer*>(buffer);
		if (checkPacket(pPacket))
		{
			revertPacket(pPacket);
			if(pThis->_buffers[FEEDBACK_INDEX])
			{
				pThis->_buffers[FEEDBACK_INDEX]->append(pPacket->Paket.Feedback);
			}
			if(pThis->_buffers[ADMITTANCE_INDEX])
			{
				pThis->_buffers[ADMITTANCE_INDEX]->append(pPacket->Paket.Admittance);
			}
			if(pThis->_buffers[DIFFERENTIAL_INDEX])
			{
				pThis->_buffers[DIFFERENTIAL_INDEX]->append(pPacket->Paket.Differential);
			}
			if(pThis->_buffers[ECG_INDEX])
			{
				pThis->_buffers[ECG_INDEX]->append(pPacket->Paket.ECG);
			}
		}
	}
	return 0;
}

unsigned long SerialPortSampler::join(int timeout)
{
	if(timeout <0)
		return WaitForSingleObject(_hThread, INFINITE);
	return WaitForSingleObject(_hThread, timeout);
}
