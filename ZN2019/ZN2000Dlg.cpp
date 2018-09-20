
// ZN2000Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zn2000.h"
#include "ZN2000Dlg.h"

#include "PersonInfoInpputDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZN2000Dlg 对话框

#define SHOW_TIMER_ELAPSE  100


CZN2000Dlg::CZN2000Dlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CZN2000Dlg::IDD, pParent)
	, _dwState(OS_IDLE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZN2000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_BASE, _mainBaseFrame);
	DDX_Control(pDX, IDC_INFO_PANE, _InfoPaneFrame);
	DDX_Control(pDX, IDC_BUTTON_PART, _btnPartSelect);
	DDX_Control(pDX, IDC_BEGIN, _btnStart);
	DDX_Control(pDX, IDC_PAUSE, _btnPause);
	DDX_Control(pDX, IDC_CALC, _btnCalc);
	DDX_Control(pDX, IDC_INPUT, _btnInputParam);
	DDX_Control(pDX, IDC_SEARCH, _btnSearch);
	DDX_Control(pDX, IDCANCEL, _btnQuit);
}

BEGIN_MESSAGE_MAP(CZN2000Dlg, CXTResizeDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BEGIN, &CZN2000Dlg::OnBnClickedBegin)
	ON_UPDATE_COMMAND_UI(IDC_BEGIN, OnUpdateBtnBegin)
	ON_BN_CLICKED(IDC_PAUSE, &CZN2000Dlg::OnBnClickedPause)
	ON_UPDATE_COMMAND_UI(IDC_PAUSE, OnUpdateBtnPause)
	ON_BN_CLICKED(IDC_SEARCH, &CZN2000Dlg::OnBnClickedSearch)
	ON_UPDATE_COMMAND_UI(IDC_SEARCH, OnUpdateBtnSearch)
	ON_CBN_DROPDOWN(IDC_BUTTON_PART, OnPartSelectDropDown)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PART, OnUpdateBtnPartSelect)
	ON_COMMAND_RANGE(IDM_PART_HEART, IDM_PART_RIGHT_LEG, OnPartSelected)
	ON_BN_CLICKED(IDC_INPUT, &CZN2000Dlg::OnBnClickedInput)
	ON_UPDATE_COMMAND_UI(IDC_INPUT, OnUpdateBtnInputParam)
	ON_BN_CLICKED(IDC_CALC, &CZN2000Dlg::OnBnClickedCalc)
	ON_UPDATE_COMMAND_UI(IDC_CALC, OnUpdateBtnCalc)
	ON_UPDATE_COMMAND_UI(IDCANCEL, OnUpdateBtnQuit)
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)

END_MESSAGE_MAP()


// CZN2000Dlg 消息处理程序

BOOL CZN2000Dlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetResize(IDC_STATIC_PART, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_PART, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BEGIN, SZ_BOTTOM_CENTER,SZ_BOTTOM_CENTER);
	SetResize(IDC_PAUSE, SZ_BOTTOM_CENTER,SZ_BOTTOM_CENTER);
	SetResize(IDC_CALC, SZ_BOTTOM_CENTER,SZ_BOTTOM_CENTER);
	SetResize(IDC_INPUT, SZ_BOTTOM_CENTER,SZ_BOTTOM_CENTER);
	SetResize(IDC_SEARCH, SZ_BOTTOM_CENTER,SZ_BOTTOM_CENTER);
	SetResize(IDCANCEL, SZ_BOTTOM_CENTER,SZ_BOTTOM_CENTER);
	SetResize(IDC_MAIN_BASE, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_INFO_PANE, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
	ShowWindow(SW_MAXIMIZE);

	_btnPartSelect.SetPushButtonStyle(xtpButtonDropDown);

	CRect rect;
	_mainBaseCtrl.Create(CMainBaseCtrl::IDD, this);
	_mainBaseFrame.GetWindowRect(&rect);
	_mainBaseCtrl.MoveWindow(&rect, TRUE);
	_mainBaseFrame.ShowWindow(SW_HIDE);
	_mainBaseCtrl.ShowWindow(SW_SHOW);

	_infoPane.Create(CInfoPane::IDD, this);
	_InfoPaneFrame.GetWindowRect(&rect);
	_infoPane.MoveWindow(&rect, TRUE);
	_InfoPaneFrame.ShowWindow(SW_HIDE);
	_infoPane.ShowWindow(SW_SHOW);
	_infoPane.SetRecord(&_patientInfo);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CZN2000Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CZN2000Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CZN2000Dlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//cs.style|=WS_MAXIMIZE;
	return CXTResizeDialog::PreCreateWindow(cs);
}

HBRUSH CZN2000Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		hbr=(HBRUSH)GetStockObject(BLACK_BRUSH);
		break;
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(0, 255, 0));
		pDC->SetBkMode(TRANSPARENT);
		break;
	}
	hbr=(HBRUSH)GetStockObject(BLACK_BRUSH);
	return hbr;
}

void CZN2000Dlg::OnBnClickedBegin()
{
	_mainBaseCtrl.start();
	_dwState |=OS_BEGIN;
}

void CZN2000Dlg::OnBnClickedPause()
{
	_mainBaseCtrl.stop();
	_dwState &=~OS_BEGIN;
	_dwState |=OS_PAUSE;

}

void CZN2000Dlg::OnBnClickedSearch()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CZN2000Dlg::OnPartSelectDropDown()
{
	CXTPWindowRect rc(&_btnPartSelect);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_PART_SELECT);
	for(UINT i=0; i< PART_MAX; i++)
	{
		if(_patientInfo.IsPartChecked(i))
		{
			menu.CheckMenuItem(i + IDM_PART_HEART, MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			menu.CheckMenuItem(i + IDM_PART_HEART, MF_BYCOMMAND|MF_UNCHECKED);
		}
	}

	TrackPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), 0, rc.left, rc.bottom, 0, m_hWnd, 0);

}

void CZN2000Dlg::OnPartSelected( UINT part )
{
	CString text;
	_btnPartSelect.SetWindowText(BODY_STRING[part-IDM_PART_HEART]);
	_patientInfo.SetPartChecked(part - IDM_PART_HEART);
	_dwState |=OS_SELECT_PART;
}

void CZN2000Dlg::OnBnClickedInput()
{
	CPersonInfoInpputDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		CTime date=CTime::GetCurrentTime();
		_patientInfo.SetId(dlg._Id);
		_patientInfo.SetName(dlg._Name);
		_patientInfo.SetGender(dlg._gender);
		_patientInfo.SetHeight(dlg._height);
		_patientInfo.SetWeight(dlg._weight);
		_patientInfo.SetElevation(dlg._evelation);
		_patientInfo.SetElectrodeDistance(dlg._electrodeDistance);
		_patientInfo.SetSystolicBloodPressure(dlg._systolicBloodPresure);
		_patientInfo.SetDiastolicBloodPressure(dlg._diastolicBloodPresure);
		_patientInfo.SetDate(date.Format(_T("%Y%m%d")));
		_infoPane.RefreshInfo();
		_dwState |=OS_PARAM_INPUT;
	}
}

void CZN2000Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	}
	CXTResizeDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CZN2000Dlg::OnBnClickedCalc()
{
	_dwState |=OS_CALC;
}
