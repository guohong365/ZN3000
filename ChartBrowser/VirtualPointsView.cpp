// PointView.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>

#include "ChartBrowser.h"
#include "VirtualPointsView.h"


// CVirtualPointsView

IMPLEMENT_DYNCREATE(CVirtualPointsView, CBaseView)

CVirtualPointsView::CVirtualPointsView()
	: CBaseView(CVirtualPointsView::IDD)
{
	m_strCaption = _T("Virtual Points Sample");

	m_nFunction = 0;
	m_nCountIndex = 2;
}


CVirtualPointsView::~CVirtualPointsView()
{
}

void CVirtualPointsView::DoDataExchange(CDataExchange* pDX)
{
	CBaseView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);

	DDX_CBIndex(pDX, IDC_COMBO_FUNCTION, m_nFunction);
	DDX_CBIndex(pDX, IDC_COMBO_COUNT, m_nCountIndex);

}

BEGIN_MESSAGE_MAP(CVirtualPointsView, CBaseView)

	ON_CBN_SELCHANGE(IDC_COMBO_FUNCTION, OnCbnSelchangeComboFunction)
	ON_CBN_SELCHANGE(IDC_COMBO_COUNT, OnCbnSelchangeComboPointsCount)

END_MESSAGE_MAP()


// CVirtualPointsView diagnostics

#ifdef _DEBUG
void CVirtualPointsView::AssertValid() const
{
	CBaseView::AssertValid();
}

#ifndef _WIN32_WCE
void CVirtualPointsView::Dump(CDumpContext& dc) const
{
	CBaseView::Dump(dc);
}
#endif
#endif //_DEBUG


// CVirtualPointsView message handlers

void CVirtualPointsView::OnInitialUpdate() 
{
	CBaseView::OnInitialUpdate();


	CreateChart();

}

void CVirtualPointsView::CreateChart()
{
	CXTPChartContent* pContent = m_wndChartControl.GetContent();

	pContent->GetLegend()->SetVisible(TRUE);

	CXTPChartTitle* pTitle = pContent->GetTitles()->Add(new CXTPChartTitle());
	pTitle->SetText(_T("Math function"));

	CXTPChartSeries* pSeries = pContent->GetSeries()->Add(new CXTPChartSeries());
	pSeries->SetArgumentScaleType(xtpChartScaleNumerical);

	pSeries->SetName(_T("Function"));

	CXTPChartFastLineSeriesStyle* pStyle = new CXTPChartFastLineSeriesStyle();
	pSeries->SetStyle(pStyle);

	
	pSeries->SetArgumentScaleType(xtpChartScaleNumerical);

	OnCbnSelchangeComboFunction();


	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pSeries->GetDiagram());
	ASSERT (pDiagram);

	pDiagram->GetAxisX()->GetRange()->SetAutoRange(FALSE);
	pDiagram->GetAxisX()->GetRange()->SetMinValue(-5);
	pDiagram->GetAxisX()->GetRange()->SetMaxValue(5);

	pDiagram->GetAxisY()->GetRange()->SetAutoRange(FALSE);
	pDiagram->GetAxisY()->GetRange()->SetMinValue(-1);
	pDiagram->GetAxisY()->GetRange()->SetMaxValue(1);
	
}

double GetSinPointValue(CXTPChartSeries* /*pSeries*/, double x)
{
	return sin(x);
}

double GetCosPointValue(CXTPChartSeries* /*pSeries*/, double x)
{
	return cos(x);
}


void CVirtualPointsView::OnCbnSelchangeComboFunction()
{
	UpdateData();

	CXTPChartContent* pContent = m_wndChartControl.GetContent();

	CXTPChartSeries* pSeries = pContent->GetSeries()->GetAt(0);


	double delta = m_nCountIndex == 0 ? 1.0 : m_nCountIndex == 1 ? 0.1 : m_nCountIndex == 2 ? 0.01 : 0.001;
	
	pSeries->SetPoints(new CXTPChartSeriesVirtualPointCollection(-5, 5, delta, m_nFunction == 0 ? GetSinPointValue : GetCosPointValue));
}

void CVirtualPointsView::OnCbnSelchangeComboPointsCount()
{
	UpdateData();

	OnCbnSelchangeComboFunction();

}
