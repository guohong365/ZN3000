#include "stdafx.h"
#include "SampleThread.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI SampleThread::SampleThreadFun( LPVOID pParam )
{
	SampleThread *pSampleThread=(SampleThread*)pParam;
	return pSampleThread->run();
}

SampleThread::SampleThread(SignalBuffer<unsigned short>* pBuffers, int channelCount, ISampler * pSampler )
:_pSampler(pSampler)
,_hThread(INVALID_HANDLE_VALUE)
,_dwThreadId(0xFFFFFFFF)
,_channelCount(channelCount)
{
	_pBuffers=pBuffers;
}

void SampleThread::start()
{
	if(_hThread!=INVALID_HANDLE_VALUE && _hThread!=NULL)
	{
		CloseHandle(_hThread);
	}
	_hThread=CreateThread(NULL, 0, SampleThreadFun, this, 0, &_dwThreadId );
}

void SampleThread::stop( DWORD timeout )
{
	_bQuit = true;
	if(WaitForSingleObject(_hThread, timeout)!=WAIT_OBJECT_0)
	{
		TerminateThread(_hThread, 0);
	}
}

bool SampleThread::isRunning()
{
	DWORD dwErrorCode;
	DWORD dwExitCode;
	if(!GetExitCodeThread(_hThread, &dwExitCode))
	{
		dwErrorCode=GetLastError();
		return dwExitCode==STILL_ACTIVE;
	}
	return false;
}

DWORD SampleThread::run()
{
	ASSERT(_pSampler);
	ASSERT(_pBuffers);

	_pSampler->open();
	while(!_bQuit)
	{
		for(int i=0; i< _channelCount; i++)
		{
			_pBuffers[i].add(_pSampler->readShort(i));
		}
		Sleep(2);
	}
	return 0;
}


