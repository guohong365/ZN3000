#pragma once

#include "BaseView.h"


// CVirtualPointsView form view

class CVirtualPointsView : public CBaseView
{
	DECLARE_DYNCREATE(CVirtualPointsView)

protected:
	CVirtualPointsView();           // protected constructor used by dynamic creation
	virtual ~CVirtualPointsView();

public:
	enum { IDD = IDD_VIEW_VIRTUALPOINTS};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	int m_nFunction;
	int m_nCountIndex;

	void CreateChart();

	void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnCbnSelchangeComboFunction();
	afx_msg void OnCbnSelchangeComboPointsCount();
public:
};


