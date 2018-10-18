// RangeAreaView.cpp : implementation file
//

#include "stdafx.h"
#include "ChartBrowser.h"
#include "RangeAreaView.h"


// CRangeAreaView

IMPLEMENT_DYNCREATE(CRangeAreaView, CBaseView)

CRangeAreaView::CRangeAreaView()
	: CBaseView(CRangeAreaView::IDD)
{
	m_strCaption = _T("Range Area Style");

	m_nTransparency = 1;
	m_nMarkerType = 0;
	m_bShowLabels = FALSE;
	m_nLabelAngle = 1;
}

CRangeAreaView::~CRangeAreaView()
{
}

void CRangeAreaView::DoDataExchange(CDataExchange* pDX)
{
	CBaseView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);
	
	DDX_Check(pDX, IDC_CHECK_SHOWLABELS, m_bShowLabels);

	DDX_CBIndex(pDX, IDC_COMBO_LABEL_ANGLE, m_nLabelAngle);
	DDX_CBIndex(pDX, IDC_COMBO_TRANSPARENCY, m_nTransparency);
	DDX_CBIndex(pDX, IDC_COMBO_MARKER_TYPE, m_nMarkerType);
}

BEGIN_MESSAGE_MAP(CRangeAreaView, CBaseView)
	ON_BN_CLICKED(IDC_CHECK_SHOWLABELS, OnCheckShowLabels)

	ON_CBN_SELCHANGE(IDC_COMBO_LABEL_ANGLE, OnCbnSelchangeComboLabelAngle)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKER_TYPE, OnCbnSelchangeComboMarkerType)
	ON_CBN_SELCHANGE(IDC_COMBO_TRANSPARENCY, OnCbnSelchangeComboTransparency)
END_MESSAGE_MAP()


// CRangeAreaView diagnostics

#ifdef _DEBUG
void CRangeAreaView::AssertValid() const
{
	CBaseView::AssertValid();
}

#ifndef _WIN32_WCE
void CRangeAreaView::Dump(CDumpContext& dc) const
{
	CBaseView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRangeAreaView message handlers


void CRangeAreaView::OnInitialUpdate() 
{
	CBaseView::OnInitialUpdate();

	CreateChart();
}


void CRangeAreaView::SetAxisTitle(CXTPChartDiagram2D* pDiagram, CString strAxisX, CString strAxisY)
{
	if (pDiagram)
	{
		CXTPChartAxis* pAxisX = pDiagram->GetAxisX();
		if (pAxisX)
		{
			CXTPChartAxisTitle* pTitle = pAxisX->GetTitle();
			if (pTitle)
			{
				pTitle->SetText(strAxisX);
				pTitle->SetVisible(TRUE);
			}
		}

		CXTPChartAxis* pAxisY = pDiagram->GetAxisY();
		if (pAxisY)
		{
			CXTPChartAxisTitle* pTitle = pAxisY->GetTitle();
			if (pTitle)
			{
				pTitle->SetText(strAxisY);
				pTitle->SetVisible(TRUE);
			}
		}
	}
}

void CRangeAreaView::CreateChart()
{
	CXTPChartContent* pContent = m_wndChartControl.GetContent();

	pContent->GetLegend()->SetVisible(TRUE);

	CXTPChartTitle* pTitle = pContent->GetTitles()->Add(new CXTPChartTitle());
	pTitle->SetText(_T("Profit Range"));

	CXTPChartSeriesCollection* pCollection = pContent->GetSeries();
	pCollection->RemoveAll();

	if (pCollection)
	{
		CXTPChartSeries* pSeries1 = pCollection->Add(new CXTPChartSeries());
		if (pSeries1)
		{
			pSeries1->SetName(_T("Company 1"));

			CXTPChartSeriesPointCollection* pPoints = pSeries1->GetPoints();
			if (pPoints)
			{
				CXTPChartSeriesPoint* pPoint = NULL;

				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("Junary"), 0.7, 6.1));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("February"), 0.6, 6.3));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("March"), 1.9, 8.5));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("April"), 3.1, 10.8));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("May"), 5.7, 14.4));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("June"), 8.4, 16.9));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("July"), 10.6, 19.2));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("August"), 10.5, 18.9));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("September"), 8.5, 16.1));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("October"), 6.0, 12.5));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("November"), 3.0, 8.8));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("December"), 1.5, 6.9));
			}

			pSeries1->SetStyle(new CXTPChartRangeSplineAreaSeriesStyle());
		}


		pSeries1 = pCollection->Add(new CXTPChartSeries());
		if (pSeries1)
		{
			pSeries1->SetName(_T("Company 2"));
			
			CXTPChartSeriesPointCollection* pPoints = pSeries1->GetPoints();
			if (pPoints)
			{
				CXTPChartSeriesPoint* pPoint = NULL;
				
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("Junary"), 1.7, 4.1));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("February"), 1.6, 4.3));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("March"), 2.9, 4.5));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("April"), 4.2, 5.8));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("May"), 4.7, 6.4));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("June"), 4.4, 6.9));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("July"), 4.6, 7.2));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("August"), 6.5, 8.9));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("September"), 6.5, 9.1));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("October"), 7.0, 9.5));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("November"), 8.0, 10.8));
				pPoint = pPoints->Add(new CXTPChartSeriesPoint(_T("December"), 9.5, 10.9));
			}
			
			pSeries1->SetStyle(new CXTPChartRangeSplineAreaSeriesStyle());
		}

	
	}

	// Set the X and Y Axis title for the series.
	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pCollection->GetAt(0)->GetDiagram());
	ASSERT (pDiagram);

	//SetAxisTitle(pDiagram, _T("Season"), _T("Number of Points Scored"));

	pDiagram->GetAxisX()->GetRange()->SetSideMargins(FALSE);
	pDiagram->GetAxisX()->GetLabel()->SetAngle(90);

	pDiagram->GetAxisY()->GetLabel()->GetFormat()->SetCategory(xtpChartCurrency);

	OnCbnSelchangeComboTransparency();

	OnCheckShowLabels();

}


void CRangeAreaView::OnCbnSelchangeComboTransparency()
{
	UpdateData();

	int nTransparency = m_nTransparency * 45;
	if (nTransparency > 255) nTransparency = 255;

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartAreaSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartAreaSeriesStyle, pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->SetTransparency(nTransparency);
	}

}

void CRangeAreaView::OnCbnSelchangeComboMarkerType()
{
	UpdateData();

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartPointSeriesStyle, pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->GetMarker()->SetType((XTPChartMarkerType)m_nMarkerType);
	}
}
void CRangeAreaView::OnCheckShowLabels() 
{
	UpdateData();

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle =  DYNAMIC_DOWNCAST(CXTPChartPointSeriesStyle, pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->GetLabel()->SetVisible(m_bShowLabels);
	}

	GetDlgItem(IDC_COMBO_LABEL_ANGLE)->EnableWindow(m_bShowLabels);
}

void CRangeAreaView::OnCbnSelchangeComboLabelAngle()
{
	UpdateData();

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartPointSeriesStyle, pSeriesCollection->GetAt(i)->GetStyle());

		CXTPChartPointSeriesLabel* pLabel = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();

		pLabel->SetAngle(45 * m_nLabelAngle);
	}

}
