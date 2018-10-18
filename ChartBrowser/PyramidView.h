#pragma once

#include "BaseView.h"

// CPyramidView form view

class CPyramidView : public CBaseView
{
	DECLARE_DYNCREATE(CPyramidView)

protected:
	CPyramidView(UINT nID = IDD);           // protected constructor used by dynamic creation
	virtual ~CPyramidView();

public:
	enum { IDD = IDD_VIEW_PYRAMID };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void CreateChart();

	void OnInitialUpdate();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckShowlabels();
	BOOL m_bShowLabels;
	int m_nPosition;
	afx_msg void OnCbnSelchangeComboLabelPosition();
	
	afx_msg void OnEditChangePointDistance();
	afx_msg void OnSelChangePointDistance();
	
	afx_msg void OnEditChangeHeightToWidth();
	afx_msg void OnSelChangeHeightToWidth();

	CString m_strPointDistance;
	CComboBox m_cboPointDistance;

	CString m_strHeightToWidth;
	CComboBox m_cboHeightToWidth;

};


