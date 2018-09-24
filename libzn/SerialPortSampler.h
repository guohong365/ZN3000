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
	SignalBuffer<unsigned short>* getBuffer(int index);
	void attachBuffer(int index, SignalBuffer<unsigned short> * pBuffer);
private:
	/**ɨ�軺��������ǳ��ֺϷ�Packe�ṹ��λ�á�
	 *
	 *@param pBuffer ��ɨ�軺����
	 *@param size ��������С��Ӧ���ڵ���3��sizeof(Packet)��С
	 *@return ��һ��У����ϵ�Packet֡��ʼλ�ã�zero basec����
	 */
	static long scanPacket(const unsigned char* pBuffer, long size);
	/**ͬ����ȡ���壬ʹ�ò���ѭ����������֡��ȡ������������
	 */
	int synchronizePacket() const;
	/**�����߳�
	 *
	 *@param lpParam �������Ϊthisָ��
	 *@return 0 - ����ֹͣ >1000 ����
	 */
	static DWORD __stdcall samplerFunc(LPVOID lpParam);
public:
	virtual unsigned long join(int timeout =-1);
private:
	SerialPort _serialPort;
	HANDLE _hThread;
	HANDLE _hResume;
	DWORD _dwThreadId;
	SignalBuffer<unsigned short> *_buffers[4];
	bool _quit;
	bool _paused;
	SamplerProgressCallback* _progressCallback;
	SamplerStateChangedCallback * _stateChangedCallback;
};
