#pragma once

#include "Sampler.h"
#include "SerialPort.h"
#include "SignalBuffer.h"

class SerialPortSampler : public Sampler
{
public:
	explicit SerialPortSampler(
		const CString& portName = _T("\\\\.\\COM1"),
		DWORD baudRate = CBR_14400,
		BYTE dataBits = 8,
		BYTE parity = NOPARITY,
		BYTE stopBits = ONESTOPBIT);
	~SerialPortSampler();
	virtual bool begin();
	virtual void pause();
	virtual void resume();
	virtual void quit();
private:
	static DWORD __stdcall samplerFunc(LPVOID lpParam);	
	SerialPort _serialPort;
	HANDLE _hThread;
	HANDLE _hResume;
	DWORD _dwThreadId;
	SignalBuffer<short> *_buffers;
	bool _quit;
	bool _paused;
};
