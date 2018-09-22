#include "stdafx.h"
#include "SerialPortSampler.h"
#include "Packet.h"

SerialPortSampler::
SerialPortSampler(const CString& portName, DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits)
	: _serialPort(portName, baudRate, dataBits, parity, stopBits)
	  , _hThread(nullptr)
	  , _hResume(nullptr)
	  , _dwThreadId(0)
	  , _quit(false)
	  , _paused(false)
{	
	_buffers[0]=_buffers[1]=_buffers[2]=_buffers[3]=nullptr;
}

bool SerialPortSampler::begin()
{
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

SignalBuffer<unsigned short>* SerialPortSampler::getBuffer(int index)
{
	return _buffers[index];
}

void SerialPortSampler::attachBuffer(const int index, SignalBuffer<unsigned short>* pBuffer)
{
	if(index >=0 && index < 4)
	{
		_buffers[index]=pBuffer;
	}
}

SerialPortSampler::~SerialPortSampler()
{
	CloseHandle(_hResume);
	CloseHandle(_hThread);
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
		if(pThis->_paused)
		{
			if(WaitForSingleObject(pThis->_hResume, 100)==WAIT_TIMEOUT)
			{
				continue;
			}
			ResetEvent(pThis->_hResume);
			pThis->_paused=false;
			pThis->_serialPort.clear();
		}
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
