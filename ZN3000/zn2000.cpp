
// zn2000.cpp : 定义应用程序的类行为。
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

CZN2000App::CZN2000App(): _pSettings(nullptr)
{
}

Settings& CZN2000App::getSettings() const
{
	return *_pSettings;
}


// 唯一的一个 CZN2000App 对象
GdiplusEnvironment GdiPlusEnv;

CZN2000App TheApp;


// CZN2000App 初始化
#define TIME_RES 2

BOOL CZN2000App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("UC"));

	_pSettings=new Settings();
	if(_pSettings->load()!=0)
	{
		AfxMessageBox(_T("错误"));
		return FALSE;
	}

	CZN2000Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	
	return FALSE;
}

int CZN2000App::ExitInstance()
{
	delete _pSettings;
	return __super::ExitInstance();
}
