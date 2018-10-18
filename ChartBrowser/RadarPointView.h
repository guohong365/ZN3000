#pragma once

#include "BaseView.h"


// CRadarPointView form view

class CRadarPointView : public CBaseView
{
	DECLARE_DYNCREATE(CRadarPointView)

protected:
	CRadarPointView();           // protected constructor used by dynamic creation
	virtual ~CRadarPointView();

public:
	enum { IDD = IDD_VIEW_RADARPOINT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	BOOL m_bShowLabels;
	int m_nMarkerType;
	int m_nMarkerSize;

	BOOL m_bShowAxisX;
	BOOL m_bShowAxisY;

	BOOL m_bInterlaced;

	void CreateChart();

	void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnCheckShowLabels();
	afx_msg void OnCbnSelchangeComboMarkerSize();
	afx_msg void OnCbnSelchangeComboMarkerType();
	afx_msg void OnCheckShowAxisX();
	afx_msg void OnCheckShowAxisY();
	afx_msg void OnCheckInterlaced();
public:
};


