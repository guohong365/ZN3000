
// TestSampler.cpp : 定义应用程序的类行为。
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


// CTestSamplerApp 构造

CTestSamplerApp::CTestSamplerApp(): _pSettings(nullptr)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

Settings& CTestSamplerApp::GetSettings()
{
	return *static_cast<CTestSamplerApp*>(AfxGetApp())->_pSettings;
}


// 唯一的一个 CTestSamplerApp 对象

CTestSamplerApp theApp;
GdiplusEnvironment GdiPlusEnvironment;

// CTestSamplerApp 初始化

BOOL CTestSamplerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX initCtrls;
	initCtrls.dwSize = sizeof(initCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	initCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&initCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	_pSettings=new Settings();
	if(0!=_pSettings->load())
	{
		AfxMessageBox(_T("加载配置文件错误！"));
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

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}	
	return FALSE;
}

int CTestSamplerApp::ExitInstance()
{
	CWinApp::ExitInstance();
	delete _pSettings;
	return 0;
}

