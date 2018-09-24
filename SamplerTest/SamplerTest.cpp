// SamplerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SamplerTest.h"
#include "../libzn/SerialPortSampler.h"
#include "../libzn/Packet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

class ProgressCallback : public SamplerProgressCallback
{
public:
	virtual void onProgress(Sampler* sender, unsigned long current, void* pPacketBuffer, size_t size);
};

void ProgressCallback::onProgress(Sampler* sender, unsigned long current, void* pPacketBuffer, size_t size)
{
	if (!pPacketBuffer)
	{
		_tprintf(_T("EMPTY\n"));
		return;
	}
	Packet* pPacket = reinterpret_cast<Packet*>(pPacketBuffer);
	_tprintf(_T("packet: %x %x %x %x %x %x %x %x %x"),
	         pPacket->Header,
	         pPacket->FrameCount,
	         pPacket->Mode,
	         pPacket->Feedback,
	         pPacket->Admittance,
	         pPacket->Differential,
	         pPacket->ECG,
	         pPacket->CheckSum,
	         pPacket->Tail);
}

class StateChangedCallback : public SamplerStateChangedCallback
{
public:
	virtual void onStateChanged(Sampler* sender, Sampler::SamplerState state)
	{
		_tprintf(_T("state changed to [%d]\n"), state);
	}
};

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	const HMODULE hModule = ::GetModuleHandle(nullptr);
	SerialPortSampler sampler(_T("COM2"), 500000, 8, 0, 0);

	if (hModule != nullptr)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			sampler.begin(new ProgressCallback, new StateChangedCallback);
			sampler.join(-1);
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
