
// XTPZN3000.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../libzn/Settings.h"


// CXTPZN3000App:
// �йش����ʵ�֣������ XTPZN3000.cpp
//

class CXTPZN3000App : public CWinApp
{
	Settings _settings;
public:
	CXTPZN3000App();
	static Settings& GetSettings();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXTPZN3000App theApp;