// libView.h : libView DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ClibViewApp
// �йش���ʵ�ֵ���Ϣ������� libView.cpp
//

class CLibViewApp : public CWinApp
{
public:
	CLibViewApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
