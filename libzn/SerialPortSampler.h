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
	/**ɨ�軺��������ǳ��ֺϷ�Packe�ṹ��λ�á�
	 *
	 *@param pBuffer ��ɨ�軺����
	 *@param size ��������С��Ӧ���ڵ���3��sizeof(Packet)��С
	 *@return ��һ��У����ϵ�Packet֡��ʼλ�ã�zero basec����
	 */
	static long scanPacket(const unsigned char* pBuffer, long size);
	/**ͬ����ȡ���壬ʹ�ò���ѭ����������֡��ȡ������������
	 *
	 *@return 0 - �ɹ��� < 0 �����ڴ���
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
