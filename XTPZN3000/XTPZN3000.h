
// XTPZN3000.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "../libzn/Settings.h"


// CXTPZN3000App:
// 有关此类的实现，请参阅 XTPZN3000.cpp
//

class CXTPZN3000App : public CWinApp
{
	Settings _settings;
public:
	CXTPZN3000App();
	static Settings& GetSettings();
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CXTPZN3000App theApp;