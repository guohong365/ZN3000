
// TestSamplerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSampler.h"
#include "TestSamplerDlg.h"
#include "afxdialogex.h"
#include "../libzn/SerialPortSampler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestSamplerDlg 对话框




CTestSamplerDlg::CTestSamplerDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTestSamplerDlg::IDD, pParent)
	  , _workMode(0)
	  , _showMode(0)
	  , _pFeedback(nullptr)
	  , _pAdmittance(nullptr)
	  , _pDifferential(nullptr)
	  , _pEcg(nullptr)
	  , _sampler(
		  CTestSamplerApp::GetSettings().getSerialPort(),
		  CTestSamplerApp::GetSettings().getBaudRate(),
		  CTestSamplerApp::GetSettings().getDataBits(),
		  CTestSamplerApp::GetSettings().getStopBits())
	  , _dwState(OS_IDLE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestSamplerDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CALIBRATION, _workMode);
	DDX_Radio(pDX, IDC_RADIO_ROLLING, _showMode);
	DDX_Control(pDX, IDC_STATIC_WAVE_CTRL_FRAME, _waveFrame);
}

BEGIN_MESSAGE_MAP(CTestSamplerDlg, CXTResizeDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_KICKIDLE, &CTestSamplerDlg::OnKickIdle)
	ON_BN_CLICKED(IDC_BEGIN, &CTestSamplerDlg::OnBnClickedBegin)
	ON_UPDATE_COMMAND_UI(IDC_BEGIN, &CTestSamplerDlg::OnUpdateBtnBegin)
	ON_BN_CLICKED(IDC_STOP, &CTestSamplerDlg::OnBnClickedStop)
	ON_UPDATE_COMMAND_UI(IDC_STOP, &CTestSamplerDlg::OnUpdateBtnStop)
	ON_BN_CLICKED(IDC_RADIO_CALIBRATION, &CTestSamplerDlg::OnBnClickedRadioCalibration)
	ON_BN_CLICKED(IDC_RADIO_EXAMINE, &CTestSamplerDlg::OnBnClickedRadioExamine)
	ON_BN_CLICKED(IDC_RADIO_ROLLING, &CTestSamplerDlg::OnBnClickedRadioRolling)
	ON_BN_CLICKED(IDC_RADIO_ERASURE, &CTestSamplerDlg::OnBnClickedRadioErasure)
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTestSamplerDlg 消息处理程序

BOOL CTestSamplerDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		CString strAboutMenu;
		const BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetResize(IDC_STATIC_WAVE_CTRL_FRAME, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_WORK_MODE_FRAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_CALIBRATION, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_EXAMINE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDC_STATIC_SHOW_MODE_FRAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_ROLLING, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_ERASURE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_CMD_FRAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BEGIN, SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);
	SetResize(IDC_STOP, SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	ShowWindow(SW_MAXIMIZE);

	CRect rect;
	_waveFrame.GetWindowRect(&rect);
	ScreenToClient(&rect);
	_waveCtrl.Create(CWaveBaseCtrl::IDD, this);

	_waveCtrl.MoveWindow(&rect);
	_waveFrame.ShowWindow(SW_HIDE);
	_waveCtrl.ShowWindow(SW_SHOW);
	
	Settings & settings=CTestSamplerApp::GetSettings();
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
	_waveCtrl.SetBuffers(_pAdmittance, _pDifferential, _pEcg);
	_waveCtrl.SetCurrentPart(PART_CALIBRATION);

	_sampler.attachBuffer(FEEDBACK_INDEX, &_pFeedback->getSignalBuffer());
	_sampler.attachBuffer(ADMITTANCE_INDEX, &_pAdmittance->getSignalBuffer());
	_sampler.attachBuffer(DIFFERENTIAL_INDEX, &_pDifferential->getSignalBuffer());
	_sampler.attachBuffer(ECG_INDEX, &_pEcg->getSignalBuffer());
	_sampler.begin();
	_sampler.setMode(WORK_MODE_CALIBRATION);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestSamplerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CXTResizeDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestSamplerDlg::OnPaint()
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
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;

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
HCURSOR CTestSamplerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CTestSamplerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UpdateDialogControls(this, TRUE);
	return 0;
}

void CTestSamplerDlg::OnBnClickedBegin()
{
	_waveCtrl.start();
	_dwState |= OS_BEGIN;
}

void CTestSamplerDlg::OnUpdateBtnBegin(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!(_dwState & OS_BEGIN));
}


void CTestSamplerDlg::OnBnClickedStop()
{
	_waveCtrl.stop();
	_dwState &= ~OS_BEGIN;
	_dwState |=OS_PAUSE;
}

void CTestSamplerDlg::OnUpdateBtnStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(_dwState & OS_BEGIN);
}


void CTestSamplerDlg::OnBnClickedRadioCalibration()
{
	if(_waveCtrl.GetCurrentPart()==PART_CALIBRATION) return;
	_sampler.pause();
	_waveCtrl.SetCurrentPart(PART_CALIBRATION);
	_sampler.setMode(WORK_MODE_CALIBRATION);
	Invalidate();
}


void CTestSamplerDlg::OnBnClickedRadioExamine()
{
	if(_waveCtrl.GetCurrentPart()==PART_HEART) return;
	_sampler.pause();
	_waveCtrl.SetCurrentPart(PART_HEART);
	_sampler.setMode(WORK_MODE_EXAMINE);
	Invalidate();
}


void CTestSamplerDlg::OnBnClickedRadioRolling()
{
	_waveCtrl.SetDrawMode(DRAW_ROLLING);
}


void CTestSamplerDlg::OnBnClickedRadioErasure()
{
	_waveCtrl.SetDrawMode(DRAW_ERASURE);
}


void CTestSamplerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		//TODO: zoom in wave
		break;
	case VK_DOWN:
		//TODO: zoom out wave
		break;
	default:
		break;
	}
	CXTResizeDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestSamplerDlg::OnDestroy()
{
	_waveCtrl.stop();
	_sampler.quit();
	CXTResizeDialog::OnDestroy();
}


void CTestSamplerDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	if(_waveCtrl.GetSafeHwnd())
	{
		CRect rect;
		_waveFrame.GetWindowRect(&rect);
		ScreenToClient(&rect);
		_waveCtrl.MoveWindow(&rect);
		_waveCtrl.Invalidate();
	}
}
