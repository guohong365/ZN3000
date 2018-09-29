#pragma once

#include "SerialPort.h"
#include "AbstractSampler.h"

#define FEEDBACK_INDEX 0
#define ADMITTANCE_INDEX 1
#define DIFFERENTIAL_INDEX 2
#define ECG_INDEX 3

#define ERR_SYNCHRONIZE_PACKET 1000
#define ERR_READ_FROM_COM 1002

#define WORK_MODE_CALIBRATION 0x01
#define WORK_MODE_EXAMINE 0x00

class SerialPortSampler : public AbstractSampler
{
public:
	explicit SerialPortSampler(
		const CString& portName = _T("\\\\.\\COM1"),
		DWORD baudRate = CBR_14400,
		BYTE dataBits = 8,
		BYTE parity = NOPARITY,
		BYTE stopBits = ONESTOPBIT);
	~SerialPortSampler();	
	bool setMode(BYTE mode);
private:
	/**扫描缓冲区，标记出现合法Packe结构的位置。
	 *
	 *@param pBuffer 待扫描缓冲区
	 *@param size 缓冲区大小，应大于等于3倍sizeof(Packet)大小
	 *@return 第一个校验符合的Packet帧起始位置（zero basec）。
	 */
	static long scanPacket(const unsigned char* pBuffer, long size);
	/**同步读取缓冲，使得采样循环可以完整帧读取，并处理数据
	 *
	 *@return 0 - 成功， < 0 读串口错误。
	 */
	int synchronizePacket() const;

protected:
	virtual void doSampler();
	virtual bool onBeforeStart();
	virtual void onBeforeLoop();
	virtual void onResume();
private:
	SerialPort _serialPort;	
};
