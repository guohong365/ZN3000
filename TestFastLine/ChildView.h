
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include <Common/XTPDrawHelpers.h>

template <class TBase>
class CChartBorder : public TBase
{
public:
	CChartBorder()
	{
		CXTPChartColor color;
		color.SetFromCOLORREF(0xcf9365);
		GetContent()->GetBorder()->SetColor(color);
	}
};

// CChildView ����

class CChildView : public CFormView
{
	DECLARE_DYNAMIC(CChildView)
// ����
public:
	CChildView();

// ����
public:

// ����
public:

// ��д
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);
// ʵ��
public:
	virtual ~CChildView();
	virtual void OnInitialUpdate();
	void CreateChart();
	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void AddPoints();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
protected:
	CString _caption;
	CBrush _brush;
    CChartBorder<CXTPChartControl> _wndChartControl;

	int m_nTopGap;
	int m_nLeftGap;
	UINT _nTimer;
};

