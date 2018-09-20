
// zn2000.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <MMSystem.h>


// CZN2000App:
// 有关此类的实现，请参阅 zn2000.cpp
//

class CZN2000App : public CWinAppEx
{
	TIMECAPS _timeCaps;
public:
	CZN2000App();

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CZN2000App TheApp;