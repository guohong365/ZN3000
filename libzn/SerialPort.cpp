#include "stdafx.h"
#include "SerialPort.h"


SerialPort::SerialPort(const CString& portName, DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits):
	_handle(INVALID_HANDLE_VALUE),
	_portName(portName),
	_baudRate(baudRate),
	_dataBits(dataBits),
	_parity(parity),
	_stopBits(stopBits)
{
}

SerialPort::~SerialPort()
{
	close();
}

bool SerialPort::open()
{
	_handle = CreateFile(_portName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0,
	                     nullptr);
	if (_handle == INVALID_HANDLE_VALUE) return false;
	COMMTIMEOUTS commTimeouts;
	if (!GetCommTimeouts(_handle, &commTimeouts)) return false;
	commTimeouts.ReadIntervalTimeout = READ_INTERVAL_TIMEOUT;
	commTimeouts.ReadTotalTimeoutMultiplier = READ_TOTAL_TIMEOUT_MULTIPLIER;
	commTimeouts.ReadTotalTimeoutConstant = READ_TOTAL_TIMEOUT_CONSTANT;
	commTimeouts.WriteTotalTimeoutConstant = WRITE_TOTAL_TIMEOUT_CONSTANT;
	commTimeouts.WriteTotalTimeoutMultiplier = WRITE_TOTAL_TIMEOUT_MULTIPLIER;
	if (!SetCommTimeouts(_handle, &commTimeouts)) return false;
	if (!SetupComm(_handle, COMM_INPUT_BUFFER_SIZE, COMM_OUTPUT_BUFFER_SIZE)) return false;
	DCB dcb;
	if (!GetCommState(_handle, &dcb)) return false;
	dcb.BaudRate = _baudRate;
	dcb.Parity = _parity;
	dcb.ByteSize = _dataBits;
	dcb.StopBits = _stopBits;
	dcb.fParity = _parity != NOPARITY ? 1 : 0;
	if (!SetCommState(_handle, &dcb)) return false;
	if (!PurgeComm(_handle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT)) return false;
	DWORD dwError;
	COMSTAT comStat;
	return ClearCommError(_handle, &dwError, &comStat)==TRUE;
}

DWORD SerialPort::read(LPVOID lpBytes, DWORD length) const
{
	DWORD readBytes;
	if (ReadFile(_handle, lpBytes, length, &readBytes, nullptr))
	{
		return readBytes;
	}
	return DWORD(-1);
}

DWORD SerialPort::write(const LPVOID pBytes, const DWORD length) const
{
	DWORD writeBytes;
	if (WriteFile(_handle, pBytes, length, &writeBytes, nullptr))
	{
		return writeBytes;
	}
	return DWORD(-1);
}

void SerialPort::close()
{
	if(_handle !=nullptr && _handle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(_handle);
		_handle = nullptr;
	}
}

void SerialPort::clear() const
{
	if(_handle !=INVALID_HANDLE_VALUE){
		PurgeComm(_handle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}
}
