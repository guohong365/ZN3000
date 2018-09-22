#pragma once

#define READ_INTERVAL_TIMEOUT 0
#define READ_TOTAL_TIMEOUT_MULTIPLIER 0
#define READ_TOTAL_TIMEOUT_CONSTANT 5000
#define WRITE_TOTAL_TIMEOUT_CONSTANT 5000
#define WRITE_TOTAL_TIMEOUT_MULTIPLIER 0
#define COMM_INPUT_BUFFER_SIZE 10240
#define COMM_OUTPUT_BUFFER_SIZE 1024

class SerialPort
{
public:
	explicit SerialPort(
		const CString& portName = _T("\\\\.\\COM1"),
		DWORD baudRate = CBR_14400,
		BYTE dataBits = 8,
		BYTE parity = NOPARITY,
		BYTE stopBits = ONESTOPBIT);

	~SerialPort();
	bool open();
	DWORD read(LPVOID lpBytes, DWORD length) const;
	DWORD write(LPVOID lpBytes, DWORD length) const;
	void close();
	void clear() const;
	bool isOpened() const;
private:
	HANDLE _handle;
	CString _portName;
	DWORD _baudRate;
	BYTE _dataBits;
	BYTE _parity;
	BYTE _stopBits;	
};
