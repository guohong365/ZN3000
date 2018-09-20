
// ZN2000Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "zn2000.h"
#include "ZN2000Dlg.h"

#include "PersonInfoInpputDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZN2000Dlg �Ի���

#define SHOW_TIMER_ELAPSE  100


CZN2000Dlg::CZN2000Dlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CZN2000Dlg::IDD, pParent)
	  , _pRecord(nullptr)
	  , _pCurrentChannel(nullptr)
	  , _pEcg(nullptr)
	  , _currentPart(PART_NONE)
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


// CZN2000Dlg ��Ϣ�������

BOOL CZN2000Dlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

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
	_infoPane.SetRecord(_pRecord);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CZN2000Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CZN2000Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CZN2000Dlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CZN2000Dlg::OnPartSelectDropDown()
{
	CXTPWindowRect rc(&_btnPartSelect);
	CMenu menu;
	menu.LoadMenu(IDR_MENU_PART_SELECT);
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

	TrackPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), 0, rc.left, rc.bottom, 0, m_hWnd, 0);

}

void CZN2000Dlg::OnPartSelected( UINT part )
{
	const PartId partId=PartId(part-IDM_PART_HEART);
	const CString text=BODY_STRING[part-IDM_PART_HEART];
	_btnPartSelect.SetWindowText(text);
	SignalChannel * pChannel=ZnHelper::createSignalChannel(partId, ZN_SAMPLE_FREQUENCY);
	dynamic_cast<ZnRecord*>(_pRecord)->addChannel(pChannel);	
	_dwState |=OS_SELECT_PART;
}

void CZN2000Dlg::OnBnClickedInput()
{
	CPersonInfoInpputDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		__time64_t time=_time64(nullptr);
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
