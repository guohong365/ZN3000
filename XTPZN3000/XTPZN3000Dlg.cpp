
// XTPZN3000Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XTPZN3000.h"
#include "XTPZN3000Dlg.h"
#include "afxdialogex.h"
#include "../libzn/SimSampler.h"
#include "../libzn/SerialPortSampler.h"
#include "../libzn/ZnRecord.h"

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


// CXTPZN3000Dlg 对话框




CXTPZN3000Dlg::CXTPZN3000Dlg(CWnd* pParent /*=NULL*/)
    : CXTPResizeDialog(CXTPZN3000Dlg::IDD, pParent)
    , _pSampler(nullptr)
    , _pFeedback(nullptr)
    , _pAdmittance(nullptr)
    , _pDifferential(nullptr)
    , _pEcg(nullptr)
    , _points(nullptr)
    , _last(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CXTPZN3000Dlg::~CXTPZN3000Dlg()
{
    delete _pFeedback;
    delete _pAdmittance;
    delete _pDifferential;
    delete _pEcg;
    delete _points;
    delete _pSampler;
}

void CXTPZN3000Dlg::DoDataExchange(CDataExchange* pDX)
{
    CXTPResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHART_CTRL, _wndCharControl);
}

BEGIN_MESSAGE_MAP(CXTPZN3000Dlg, CXTPResizeDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BEGIN, &CXTPZN3000Dlg::OnBnClickedBegin)
    ON_UPDATE_COMMAND_UI(IDC_BEGIN, &CXTPZN3000Dlg::OnUpdateBtnBegin)
    ON_BN_CLICKED(IDC_RADIO_CALIBRATION, &CXTPZN3000Dlg::OnBnClickedRadioCalibration)
    ON_BN_CLICKED(IDC_RADIO_ERASURE, &CXTPZN3000Dlg::OnBnClickedRadioErasure)
    ON_BN_CLICKED(IDC_RADIO_EXAMINE, &CXTPZN3000Dlg::OnBnClickedRadioExamine)
    ON_BN_CLICKED(IDC_RADIO_ROLLING, &CXTPZN3000Dlg::OnBnClickedRadioRolling)
    ON_BN_CLICKED(IDC_STOP, &CXTPZN3000Dlg::OnBnClickedStop)
    ON_UPDATE_COMMAND_UI(IDC_STOP, &CXTPZN3000Dlg::OnUpdateBtnStop)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_MESSAGE(WM_KICKIDLE, &CXTPZN3000Dlg::OnKickIdle)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

void CXTPZN3000Dlg::Destroy()
{    
    KillTimer(0);
    _pSampler->quit();
}

void CXTPZN3000Dlg::OnCancel()
{
    if (_pSampler)
    {
        _pSampler->quit();
    }
    CXTPResizeDialog::OnCancel();
}


// CXTPZN3000Dlg 消息处理程序

Settings& CXTPZN3000Dlg::GetSettings()
{
    return CXTPZN3000App::GetSettings();
}

BOOL CXTPZN3000Dlg::OnInitDialog()
{
    CXTPResizeDialog::OnInitDialog();

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

    SetResize(IDC_CHART_CTRL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

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

    if(CXTPZN3000App::GetSettings().getDeviceVersion()==_T("0000"))
    {
        _pSampler=new SimSampler(new SineSampleGenerator<float>(25));
    } 
    else
    {
        _pSampler = new SerialPortSampler(
            GetSettings().getSerialPort(), 
            GetSettings().getBaudRate(),
            GetSettings().getDataBits(), 
            GetSettings().getParity(),
            GetSettings().getStopBits());	    
    }
    _points = new SignalBuffer<float>(10000);
    _pSampler->attach(0,_points);
    _initChart();
    _pSampler->begin();
    SetTimer(0, 200, nullptr);
    _state |= OS_BEGIN;
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CXTPZN3000Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CXTPResizeDialog::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXTPZN3000Dlg::OnPaint()
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
        CXTPResizeDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CXTPZN3000Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CXTPZN3000Dlg::OnBnClickedBegin()
{
    SetTimer(0, 100, nullptr);
    _state |=OS_BEGIN;
}

void CXTPZN3000Dlg::OnUpdateBtnBegin(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(!(_state & OS_BEGIN));
}


void CXTPZN3000Dlg::OnBnClickedRadioCalibration()
{

}


void CXTPZN3000Dlg::OnBnClickedRadioErasure()
{

}


void CXTPZN3000Dlg::OnBnClickedRadioExamine()
{

}


void CXTPZN3000Dlg::OnBnClickedRadioRolling()
{

}


void CXTPZN3000Dlg::OnBnClickedStop()
{
    KillTimer(0);
    _state &=~OS_BEGIN;
}

void CXTPZN3000Dlg::OnUpdateBtnStop(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(_state & OS_BEGIN);
}


void CXTPZN3000Dlg::OnSize(UINT nType, int cx, int cy)
{
    CXTPResizeDialog::OnSize(nType, cx, cy);
}


void CXTPZN3000Dlg::OnTimer(UINT_PTR nIDEvent)
{
    const SIZE_T current =_points->getLength();
    const SIZE_T size=_points->getSize();
    float *pBuffer=_points->getBuffer();
    CXTPChartSeriesPointCollection* pPoints=_wndCharControl.GetContent()->GetSeries()->GetAt(0)->GetPoints();
    for(SIZE_T i= _last; i< current; i++)
    {        
        pPoints->Add(new CXTPChartSeriesPoint(i, pBuffer[i%size]));                
    }
    _last=current;
    CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, 
        _wndCharControl.GetContent()->GetPanels()->GetAt(0));
    ASSERT (pDiagram);


    if (_last > 4000)
    {
        CXTPChartAxisRange* pRange = pDiagram->GetAxisX()->GetRange();

        const BOOL bAutoScroll = pRange->GetViewMaxValue() == pRange->GetMaxValue();

        pRange->SetMaxValue(_last);

        if (bAutoScroll)
        {
            const double delta = pRange->GetViewMaxValue() - pRange->GetViewMinValue();

            pRange->SetViewAutoRange(FALSE);
            pRange->SetViewMaxValue(_last);
            pRange->SetViewMinValue(_last - delta);
        }

    }
}

void CXTPZN3000Dlg::_initChart()
{
    CXTPChartContent* pContent = _wndCharControl.GetContent();
    if (pContent)
    {
        CXTPChartTitleCollection* pTitles = pContent->GetTitles();
        if (pTitles)
        {
            CXTPChartTitle* pTitle = pTitles->Add(new CXTPChartTitle());
            if (pTitle)
            {
                pTitle->SetText(_T("My Chart"));
            }
        }
        CXTPChartSeriesCollection* pCollection = pContent->GetSeries();
        if (pCollection)
        {
            CXTPChartSeries* pSeries = pCollection->Add(new CXTPChartSeries());
            if (pSeries)
            {
                // ----------------------------------------------------------
                // SET THE CHART SERIES STYLE
                // ----------------------------------------------------------
                // You can call SetStyle to set the chart style such as bar, 
                // line or area chart. Here we set the style to line chart by 
                // instantiating a CXTPChartLineSeriesStyle object.
                CXTPChartFastLineSeriesStyle* pStyle=new CXTPChartFastLineSeriesStyle();
                pStyle->SetAntialiasing(TRUE);
                pSeries->SetStyle(pStyle);
                pSeries->SetArgumentScaleType(xtpChartScaleNumerical);
            }
        }

        // Set the X and Y Axis title for the series.
        CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pCollection->GetAt(0)->GetDiagram());
        ASSERT (pDiagram);

        pDiagram->SetAllowZoom(TRUE);


        pDiagram->GetAxisY()->GetRange()->SetMaxValue(40000);
        pDiagram->GetAxisY()->GetRange()->SetMinValue(-40000);
        pDiagram->GetAxisY()->GetRange()->SetAutoRange(FALSE);
        pDiagram->GetAxisY()->SetAllowZoom(FALSE);

        pDiagram->GetAxisX()->GetRange()->SetMaxValue(4000);
        pDiagram->GetAxisX()->GetRange()->SetMinValue(0);
        pDiagram->GetAxisX()->GetRange()->SetAutoRange(FALSE);
        pDiagram->GetAxisX()->GetRange()->SetZoomLimit(10);

        pDiagram->GetAxisX()->SetInterlaced(FALSE);
        pDiagram->GetAxisY()->SetInterlaced(FALSE);	

        pDiagram->GetPane()->GetFillStyle()->SetFillMode(xtpChartFillSolid);
    }
}

