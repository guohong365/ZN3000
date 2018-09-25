
// TestSampler.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TestSampler.h"
#include "TestSamplerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestSamplerApp

BEGIN_MESSAGE_MAP(CTestSamplerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestSamplerApp ����

CTestSamplerApp::CTestSamplerApp(): _pSettings(nullptr)
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

Settings& CTestSamplerApp::GetSettings()
{
	return *static_cast<CTestSamplerApp*>(AfxGetApp())->_pSettings;
}


// Ψһ��һ�� CTestSamplerApp ����

CTestSamplerApp theApp;
GdiplusEnvironment GdiPlusEnvironment;

// CTestSamplerApp ��ʼ��

BOOL CTestSamplerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX initCtrls;
	initCtrls.dwSize = sizeof(initCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	_pSettings=new Settings();
	if(0!=_pSettings->load())
	{
		AfxMessageBox(_T("���������ļ�����"));
		return FALSE;
	}
	CTestSamplerDlg dlg;
	m_pMainWnd = &dlg;
	const INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{		
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

