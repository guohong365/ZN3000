
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
	, _sampler(getSettings().getSerialPort(), getSettings().getBaudRate(), getSettings().getDataBits(),
	           getSettings().getParity(), getSettings().getStopBits())	
	  , _pRecord(nullptr)
	  , _pFeedback(nullptr)
	  , _pAdmittance(nullptr)
	  , _pDifferential(nullptr)
	  , _pEcg(nullptr)
	  , _currentPart(PART_NONE)
	  , _dwState(OS_IDLE)
{
	_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZN2000Dlg::~CZN2000Dlg()
{
	delete _pAdmittance;
	delete _pDifferential;
	delete _pEcg;
	delete _pFeedback;
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

Settings& CZN2000Dlg::getSettings() const
{
	CZN2000App* app = DYNAMIC_DOWNCAST(CZN2000App ,AfxGetApp());
	return app->getSettings();
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

	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CZN2000Dlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	SetIcon(_hIcon, TRUE);			// 设置大图标
	SetIcon(_hIcon, FALSE);		// 设置小图标

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
	_mainBaseCtrl.Create(CWaveBaseCtrl::IDD, this);
	_mainBaseFrame.GetWindowRect(&rect);
	_mainBaseCtrl.MoveWindow(&rect, TRUE);
	_mainBaseFrame.ShowWindow(SW_HIDE);
	_mainBaseCtrl.ShowWindow(SW_SHOW);
	_infoPane.Create(CInfoPane::IDD, this);
	_InfoPaneFrame.GetWindowRect(&rect);
	_infoPane.MoveWindow(&rect, TRUE);
	_InfoPaneFrame.ShowWindow(SW_HIDE);
	_infoPane.ShowWindow(SW_SHOW);
	Settings & settings=getSettings();
	const ValueScopeDouble scope=settings.getDevicePhysicalScope();
	const double frequency=settings.getFrequency();
	const SIZE_T bufferSize=settings.getBufferSize();
	_pFeedback = new ZnSignalChannelImpl(frequency, scope.LowValue, scope.HighValue,
	                                     _T("Feedback"), bufferSize, _T("Feedback"), PART_NONE);
	_pAdmittance = new ZnSignalChannelImpl(frequency, scope.LowValue, scope.HighValue, _T(""), bufferSize,
	                                       _T("Admittance"), PART_NONE);
	_pDifferential = new ZnSignalChannelImpl(frequency, scope.LowValue, scope.HighValue, _T("HEART"), bufferSize,
	                                         _T("Differential"), PART_HEART);
	_pEcg = new ZnSignalChannelImpl(frequency, scope.LowValue, scope.HighValue, _T("ECG"), bufferSize, _T("ECG"),
	                                PART_HEART_ECG);
	_sampler.attachBuffer(FEEDBACK_INDEX, &_pFeedback->getSignalBuffer());
	_sampler.attachBuffer(ADMITTANCE_INDEX, &_pAdmittance->getSignalBuffer());
	_sampler.attachBuffer(DIFFERENTIAL_INDEX, &_pDifferential->getSignalBuffer());
	_sampler.attachBuffer(ECG_INDEX, &_pEcg->getSignalBuffer());
	_sampler.begin();
	_mainBaseCtrl.SetBuffers(_pAdmittance, _pDifferential, _pEcg);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CZN2000Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, _hIcon);
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
	return static_cast<HCURSOR>(_hIcon);
}


BOOL CZN2000Dlg::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style|=WS_MAXIMIZE;
	return CXTResizeDialog::PreCreateWindow(cs);
}

HBRUSH CZN2000Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		break;
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(0, 255, 0));
		pDC->SetBkMode(TRANSPARENT);
		break;
	}
	return HBRUSH(GetStockObject(BLACK_BRUSH));
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
	
}

void CZN2000Dlg::OnPartSelectDropDown() 
{
	const CXTPWindowRect rc(&_btnPartSelect);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_PART_SELECT);
	if(_pRecord) 
	{
		for(int i = 0; i< PART_MAX; i++)
		{
			if(dynamic_cast<ZnRecord*>(_pRecord)->isExamined(PartId(i)))
			{
				menu.CheckMenuItem(i + IDM_PART_HEART, MF_BYCOMMAND | MF_CHECKED);
			}
			else
			{
				menu.CheckMenuItem(i + IDM_PART_HEART, MF_BYCOMMAND|MF_UNCHECKED);
			}
		}
	}
	TrackPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), 0, rc.left, rc.bottom, 0, m_hWnd, 0);

}

void CZN2000Dlg::OnPartSelected( UINT part )
{
	const PartId partId=PartId(part-IDM_PART_HEART);
	const CString text=BODY_STRING[part-IDM_PART_HEART];	
	if(!_pRecord) //首次运行，需录入患者信息
	{		
		CPersonInfoInpputDlg dlg;
		if(dlg.DoModal()!=IDOK)
		{
			return;
		}
		_pRecord=new ZnRecordImpl();
		_pRecord->setHospital(getSettings().getHospital());
		_pRecord->setDepartment(getSettings().getDepartment());
		_pRecord->setElevation(getSettings().getEvaluate());
		_pRecord->setPatientName(dlg._Name);
		_pRecord->setPatientId(dlg._Id);
		_pRecord->setAge(dlg._age);		
		_pRecord->setDiastolicBloodPressure(dlg._diastolicBloodPresure);
		_pRecord->setHeight(dlg._height);
		_pRecord->setWeight(dlg._weight);
		_pRecord->setGender(dlg._gender == 0 ? _T('M') : _T('F'));
		_infoPane.SetRecord(_pRecord);
		_dwState |=OS_PARAM_INPUT;
	}
	_btnPartSelect.SetWindowText(text);
	if(partId != PART_HEART)
	{
		_pAdmittance->setLabel(text);
	}
	_mainBaseCtrl.SetCurrentPart(partId);
	//_sampler.resetBuffer();
	_dwState |=OS_SELECT_PART;
}

HRESULT CZN2000Dlg::OnKickIdle(WPARAM, LPARAM)
{
	UpdateDialogControls(this, TRUE);
	return 0;
}

void CZN2000Dlg::OnUpdateBtnBegin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_dwState == (OS_PARAM_INPUT | OS_SELECT_PART) || (_dwState & OS_PAUSE));
}

void CZN2000Dlg::OnUpdateBtnPause(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_dwState & OS_BEGIN);
}

void CZN2000Dlg::OnUpdateBtnSearch(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!(_dwState & OS_BEGIN));
}

void CZN2000Dlg::OnUpdateBtnInputParam(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!(_dwState & OS_BEGIN));
}

void CZN2000Dlg::OnUpdateBtnCalc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_dwState & OS_PAUSE);
}

void CZN2000Dlg::OnUpdateBtnQuit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!(_dwState & OS_BEGIN));
}

void CZN2000Dlg::OnUpdateBtnPartSelect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!(_dwState & OS_BEGIN));
}

void CZN2000Dlg::OnBnClickedInput()
{
	CPersonInfoInpputDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		const __time64_t time=_time64(nullptr);
		_pRecord->setPatientId(dlg._Id);
		_pRecord->setPatientName(dlg._Name);
		_pRecord->setGender(dlg._gender);
		_pRecord->setHeight(dlg._height);
		_pRecord->setWeight(dlg._weight);
		_pRecord->setElevation(dlg._evelation);
		_pRecord->setSystolicBloodPressure(dlg._systolicBloodPresure);
		_pRecord->setDiastolicBloodPressure(dlg._diastolicBloodPresure);
		_pRecord->setStartTime(time);		
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


void CZN2000Dlg::OnDestroy()
{
	_mainBaseCtrl.stop();
	_sampler.quit();

	CXTResizeDialog::OnDestroy();
}
