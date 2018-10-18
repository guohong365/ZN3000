
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "TestFastLine.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
IMPLEMENT_DYNAMIC(CChildView, CFormView)

CChildView::CChildView()
	: CFormView(IDD_MAIN_FRAME_VIEW), m_nTopGap(0), m_nLeftGap(0), _nTimer(0)
{
	_brush.CreateSolidBrush(RGB(227, 238, 255));
}

CChildView::~CChildView()
{
}

void CChildView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	CXTPWindowRect rc(_wndChartControl);
	ScreenToClient(rc);
	m_nTopGap=rc.top;
	m_nLeftGap=rc.left;

	CXTPChartContent* pContent = _wndChartControl.GetContent();
	
	
	CXTPChartTitle* pSubTitle = _wndChartControl.GetContent()->GetTitles()->Add(new CXTPChartTitle());
	pSubTitle->SetText(_T("Use mouse wheel to zoom chart"));
	pSubTitle->SetDocking(xtpChartDockBottom);
	pSubTitle->SetAlignment(xtpChartAlignFar);
	pSubTitle->SetFont(CXTPChartFont::GetTahoma8());
	pSubTitle->SetTextColor(CXTPChartColor::Gray);
	
	SetTimer(_nTimer, 100,nullptr);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView ��Ϣ�������

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FAST_LINE_CTRL, _wndChartControl);

}

void CChildView::AddPoints()
{
		CXTPChartContent* pContent = _wndChartControl.GetContent();
	
	CXTPChartSeriesCollection* pCollection = pContent->GetSeries();

	int nCount;
	
	if (pCollection)
	{
		for (int s = 0; s < pCollection->GetCount(); s++)
		{
			CXTPChartSeries* pSeries = pCollection->GetAt(s);
			if (pSeries)
			{
				int nValue = 50;

				nCount = pSeries->GetPoints()->GetCount();
				
				if (nCount)
					nValue = (int)pSeries->GetPoints()->GetAt(nCount - 1)->GetValue(0);
				
				nValue = nValue + (rand() % 20) - 10;
				
				if (nValue < 0) nValue = 0;
				if (nValue > 100) nValue = 100;
				
				pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(nCount, nValue));

			}
		}
	}

	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, 
		_wndChartControl.GetContent()->GetPanels()->GetAt(0));
	ASSERT (pDiagram);


	if (nCount > 100)
	{
		CXTPChartAxisRange* pRange = pDiagram->GetAxisX()->GetRange();

		BOOL bAutoScroll = pRange->GetViewMaxValue() == pRange->GetMaxValue();

		pRange->SetMaxValue(nCount);

		if (bAutoScroll)
		{
			double delta = pRange->GetViewMaxValue() - pRange->GetViewMinValue();

			pRange->SetViewAutoRange(FALSE);
			pRange->SetViewMaxValue(nCount);
			pRange->SetViewMinValue(nCount - delta);
		}

	}
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	AddPoints();
}

void CChildView::OnDestroy()
{
	KillTimer(_nTimer);		
}

void CChildView::CreateChart()
{
	CXTPChartContent * pContent=_wndChartControl.GetContent();
	CXTPChartSeriesCollection * pCollection=pContent->GetSeries();
	pCollection->RemoveAll();

	for(int s=0; s< 2; s++)
	{
		CXTPChartSeries * pSeries=pCollection->Add(new CXTPChartSeries());
		if(pSeries)
		{
			pSeries->SetName(_T("series"));
			CXTPChartFastLineSeriesStyle *pStyle = new CXTPChartFastLineSeriesStyle();
			pSeries->SetStyle(pStyle);
			pStyle->SetAntialiasing(true);
			pSeries->SetArgumentScaleType(xtpChartScaleNumerical);
		}
	}
	// Set the X and Y Axis title for the series.
	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pCollection->GetAt(0)->GetDiagram());
	ASSERT (pDiagram);

	pDiagram->SetAllowZoom(TRUE);
	
	
	pDiagram->GetAxisY()->GetRange()->SetMaxValue(100.1);
	pDiagram->GetAxisY()->GetRange()->SetAutoRange(FALSE);
	pDiagram->GetAxisY()->SetAllowZoom(FALSE);
	
	pDiagram->GetAxisX()->GetRange()->SetMaxValue(100.1);
	pDiagram->GetAxisX()->GetRange()->SetAutoRange(FALSE);
	pDiagram->GetAxisX()->GetRange()->SetZoomLimit(10);
	
	pDiagram->GetAxisX()->SetInterlaced(FALSE);
	pDiagram->GetAxisY()->SetInterlaced(FALSE);	
	
	pDiagram->GetPane()->GetFillStyle()->SetFillMode(xtpChartFillSolid);
}
