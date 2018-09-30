
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include <Common/XTPDrawHelpers.h>

template <class TBase>
class CChartBorder
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
	CChildView(const UINT nID);

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
	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		HBRUSH hbr=CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
		if(nCtlColor == CTLCOLOR_DLG ||
			nCtlColor == CTLCOLOR_STATIC ||
			nCtlColor == CTLCOLOR_BTN)
		{
			pDC->SetBkColor(TRANSPARENT);
			return _brush;
		}
		return hbr;
	}

	afx_msg void OnSize(UINT nType, int cx, int cy)
	{
		CFormView::OnSize(nType, cx, cy);
		CXTPWindowRect rc()
	}

	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
protected:
	CString _caption;
	CBrush _brush;
	CChartBorder<CXTPChartControl> m_wndChartControl;

	int m_nTopGap;
	int m_nLeftGap;
};

