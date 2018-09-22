#pragma once

#include "Sampler.h"
#include "SerialPort.h"
#include "SignalBuffer.h"

#define FEEDBACK_INDEX 0
#define ADMITTANCE_INDEX 1
#define DIFFERENTIAL_INDEX 2
#define ECG_INDEX 3

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
	virtual SamplerState getState();
	bool setMode(BYTE mode);
	void resetBuffer();
	SignalBuffer<unsigned short>* getBuffer(int index);
	void attachBuffer(int index, SignalBuffer<unsigned short> * pBuffer);
private:
	static DWORD __stdcall samplerFunc(LPVOID lpParam);	
	SerialPort _serialPort;
	HANDLE _hThread;
	HANDLE _hResume;
	DWORD _dwThreadId;
	SignalBuffer<unsigned short> *_buffers[4];
	bool _quit;
	bool _paused;
};
