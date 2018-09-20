#pragma once

#include "Sampler.h"
#include "SerialPort.h"

class SerialPortSampler : public ISampler
{
public:
	explicit SerialPortSampler(
		const CString& portName = _T("\\\\.\\COM1"),
		DWORD baudRate = CBR_14400,
		BYTE dataBits = 8,
		BYTE parity = NOPARITY,
		BYTE stopBits = ONESTOPBIT);

	virtual bool begin();

	virtual void stop();

	~SerialPortSampler();

private:
	static DWORD __stdcall samplerFunc(LPVOID lpParam);
	SerialPort _serialPort;
	HANDLE _hThread;
	DWORD _dwThreadId;
	SignalBuffer<short> *_buffers;
	bool _quit;
};
