
// zn2000.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <MMSystem.h>
#include "Settings.h"


// CZN2000App:
// �йش����ʵ�֣������ zn2000.cpp
//

class CZN2000App : public CWinAppEx
{
	Settings* _pSettings;
	TIMECAPS _timeCaps;
public:
	CZN2000App();
	Settings& getSettings() const;
// ��д
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CZN2000App TheApp;