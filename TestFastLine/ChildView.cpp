// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "TestFastLine.h"
#include "ChildView.h"
#include "../include/uc/libuc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
IMPLEMENT_DYNAMIC(CChildView, CFormView)

CChildView::CChildView()
    : CFormView(IDD_MAIN_FRAME_VIEW)
      , m_nTopGap(0)
      , m_nLeftGap(0)
      , _count(0)
{
    _brush.CreateSolidBrush(RGB(227, 238, 255));
    memset(_data, 0, sizeof(EcgDataItem) * (MAX_SAMPLE_COUNT + 1));
}

CChildView::~CChildView()
{
}

void CChildView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    _wndGroupBox.SubclassDlgItem(IDC_STATIC_LABELS, this);
    _wndGroupBox.SetTheme(xtpControlThemeResource);

    CXTPWindowRect rc(_wndChartControl);
    ScreenToClient(rc);
    m_nTopGap = rc.top;
    m_nLeftGap = rc.left;
    CXTPChartContent* pContent = _wndChartControl.GetContent();


    CXTPChartTitle* pSubTitle = pContent->GetTitles()->Add(new CXTPChartTitle());
    pSubTitle->SetText(_T("Use mouse wheel to zoom chart"));
    pSubTitle->SetDocking(xtpChartDockBottom);
    pSubTitle->SetAlignment(xtpChartAlignFar);
    pSubTitle->SetFont(CXTPChartFont::GetTahoma8());
    pSubTitle->SetTextColor(CXTPChartColor::Gray);

    CreateChart();
}


BEGIN_MESSAGE_MAP(CChildView, CFormView)
        ON_WM_CTLCOLOR()
        ON_WM_SIZE()
        ON_WM_ERASEBKGND()
        ON_BN_CLICKED(IDC_BTN_CAL, &CChildView::OnBnClickedBtnCal)
        ON_BN_CLICKED(IDC_BTN_LOAD_ANN, &CChildView::OnBnClickedBtnLoadAnn)
        ON_BN_CLICKED(IDC_BTN_LOAD_DATA, &CChildView::OnBnClickedBtnLoadData)
END_MESSAGE_MAP()

HBRUSH CChildView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    const HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    if (nCtlColor == CTLCOLOR_DLG ||
        nCtlColor == CTLCOLOR_STATIC ||
        nCtlColor == CTLCOLOR_BTN)
    {
        pDC->SetBkColor(TRANSPARENT);
        return _brush;
    }
    return hbr;
}

// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFormView::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

    return TRUE;
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_FAST_LINE_CTRL, _wndChartControl);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
    if (!_wndGroupBox.GetSafeHwnd() ||
        !_wndChartControl.GetSafeHwnd())
    {
        return;
    }
    CXTPWindowRect rc(_wndGroupBox);
    const int nWidth = max(m_totalDev.cx, cx);
    const int nHeight = max(m_totalDev.cy, cy);
    _wndGroupBox.SetWindowPos(nullptr,0,0,nWidth - 2*m_nLeftGap, rc.Height(), SWP_NOMOVE | SWP_NOZORDER);
    _wndChartControl.SetWindowPos(nullptr, 0, 0, nWidth - 2 * m_nLeftGap, nHeight - m_nTopGap - m_nLeftGap,
                                  SWP_NOMOVE | SWP_NOZORDER);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
    return CFormView::OnEraseBkgnd(pDC);
}

void CChildView::AddPoints()
{
    CXTPChartContent* pContent = _wndChartControl.GetContent();

    CXTPChartSeriesCollection* pCollection = pContent->GetSeries();
    double maxV=0;
    double minV=0;
    if (pCollection)
    {
        for (int s = 0; s < pCollection->GetCount(); s++)
        {
            CXTPChartSeries* pSeries = pCollection->GetAt(s);
            if (pSeries)
            {
                pSeries->GetPoints()->RemoveAll();

                for(long i=0; i<_count; i++)
                {
                    double value= s== 0 ? _data[i].s1 : _data[i].s2;
                    if(value < minV) minV = value;
                    if(value > maxV) maxV = value;
                    pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(_data[i].index, value));
                }
            }
        }
    }

    CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D,
        _wndChartControl.GetContent()->GetPanels()->GetAt(0));
    ASSERT (pDiagram);

    CXTPChartAxisRange* pRange = pDiagram->GetAxisX()->GetRange();
    if (_count > 1000)
    {
        const BOOL bAutoScroll = pRange->GetViewMaxValue() == pRange->GetMaxValue();
        pRange->SetMaxValue(_count);
        if (bAutoScroll)
        {
            pRange->SetViewAutoRange(FALSE);
            pRange->SetViewMaxValue(1000);
            pRange->SetViewMinValue(0);
        }
    }
    pRange = pDiagram->GetAxisY()->GetRange();
    pRange->SetMaxValue(maxV);
    pRange->SetMinValue(minV);


}

void CChildView::CreateChart()
{
    CXTPChartContent* pContent = _wndChartControl.GetContent();
    CXTPChartSeriesCollection* pCollection = pContent->GetSeries();
    pCollection->RemoveAll();

    for (int s = 0; s < 2; s++)
    {
        CXTPChartSeries* pSeries = pCollection->Add(new CXTPChartSeries());
        if (pSeries)
        {
            pSeries->SetName(_T("series"));
            CXTPChartFastLineSeriesStyle* pStyle = new CXTPChartFastLineSeriesStyle();
            pSeries->SetStyle(pStyle);
            pStyle->SetAntialiasing(true);
            pSeries->SetArgumentScaleType(xtpChartScaleNumerical);
        }
    }
    // Set the X and Y Axis title for the series.
    CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pCollection->GetAt(0)->GetDiagram());
    ASSERT (pDiagram);

    pDiagram->SetAllowZoom(TRUE);
    pDiagram->GetAxisY()->GetRange()->SetMaxValue(1.0);
    pDiagram->GetAxisY()->GetRange()->SetMinValue(-1.0);
    pDiagram->GetAxisY()->GetRange()->SetAutoRange(FALSE);
    //pDiagram->GetAxisY()->SetAllowZoom(FALSE);

    pDiagram->GetAxisX()->GetRange()->SetMaxValue(1000.1);
    pDiagram->GetAxisX()->GetRange()->SetAutoRange(FALSE);
    pDiagram->GetAxisX()->GetRange()->SetZoomLimit(10);

    pDiagram->GetAxisX()->SetInterlaced(FALSE);
    pDiagram->GetAxisY()->SetInterlaced(FALSE);

    pDiagram->GetPane()->GetFillStyle()->SetFillMode(xtpChartFillSolid);
}


void CChildView::OnBnClickedBtnCal()
{
    
}


void CChildView::OnBnClickedBtnLoadAnn()
{
   
}


void CChildView::OnBnClickedBtnLoadData()
{
    CFileDialog dlg(TRUE, nullptr, nullptr, 6, _T("All Files (*.*)|*.*|Text Files (*.txt)|*.txt|Data Files (*.dat)|*.dat||"));
    if(dlg.DoModal()!=IDOK) return;

    const CString file=dlg.GetPathName();

    FILE * fp =nullptr;
    char buffer[4096];
    _tfopen_s(&fp, file, _T("rt"));
    _count=0;
    if(fp)
    {
        fgets(buffer, 4096, fp);
        fgets(buffer, 4096, fp);
        char* p = fgets(buffer, 4096, fp);
        while(p && !feof(fp) && _count < MAX_SAMPLE_COUNT)
        {
            if(sscanf_s(buffer, "%15lf\t%15lf\t%15lf", &(_data[_count].time), &(_data[_count].s1), &(_data[_count].s2))==3)
            {
                _data[_count].index = _count;
                _count++;
                p=fgets(buffer, 4096, fp);
            }
            else
            {
                break;
            }
        }
        fclose(fp);
    }
    AddPoints();

}
