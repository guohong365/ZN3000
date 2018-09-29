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
			sampler.begin();
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
