
// TestFastLine.h : TestFastLine Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTestFastLineApp:
// �йش����ʵ�֣������ TestFastLine.cpp
//

class CTestFastLineApp : public CWinApp
{
public:
	CTestFastLineApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestFastLineApp theApp;
