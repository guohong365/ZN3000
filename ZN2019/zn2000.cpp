
// zn2000.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"

#include "zn2000.h"
#include "ZN2000Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZN2000App

BEGIN_MESSAGE_MAP(CZN2000App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CZN2000App::CZN2000App()
{
	
}

Settings& CZN2000App::getSettings() const
{
	return *_pSettings;
}


// Ψһ��һ�� CZN2000App ����
GdiplusEnvironment GdiPlusEnv;

CZN2000App TheApp;


// CZN2000App ��ʼ��
#define TIME_RES 2

BOOL CZN2000App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();
	timeGetDevCaps(&_timeCaps, sizeof(_timeCaps));
	timeBeginPeriod(_timeCaps.wPeriodMin);
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("UC"));

	_pSettings=new Settings();
	if(_pSettings->load()!=0)
	{
		AfxMessageBox(_T("����"));
		return FALSE;
	}

	CZN2000Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	
	return FALSE;
}

int CZN2000App::ExitInstance()
{
	timeEndPeriod(_timeCaps.wPeriodMin);
	delete _pSettings;
	return __super::ExitInstance();
}
