#pragma once

#include "Sampler.h"
#include "SerialPort.h"
#include "SignalBuffer.h"

#define FEEDBACK_INDEX 0
#define ADMITTANCE_INDEX 1
#define DIFFERENTIAL_INDEX 2
#define ECG_INDEX 3

#define ERR_SYNCHRONIZE_PACKET 1000
#define ERR_READ_FROM_COM 1002

#define WORK_MODE_CALIBRATION 0x01
#define WORK_MODE_EXAMINE 0x00

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
	virtual bool begin(SamplerProgressCallback *progressCallback=nullptr, SamplerStateChangedCallback *stateChangedCallback=nullptr);
	virtual void pause();
	virtual void resume();
	virtual void quit();
	virtual SamplerState getState();
	bool setMode(BYTE mode);
	void resetBuffer();
	SignalBuffer<float>* getBuffer(int index);
	void attachBuffer(int index, SignalBuffer<float> * pBuffer);
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
	/**采样线程
	 *
	 *@param lpParam 传入参数为this指针
	 *@return 0 - 正常停止 >1000 错误
	 */
	static DWORD __stdcall samplerFunc(LPVOID lpParam);
public:
	virtual unsigned long join(int timeout =-1);
private:
	SerialPort _serialPort;
	HANDLE _hThread;
	HANDLE _hResume;
	DWORD _dwThreadId;
	SignalBuffer<float> *_buffers[4];
	bool _quit;
	bool _paused;
	SamplerProgressCallback* _progressCallback;
	SamplerStateChangedCallback * _stateChangedCallback;
};
