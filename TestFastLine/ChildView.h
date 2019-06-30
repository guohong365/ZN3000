
// ChildView.h : CChildView 类的接口
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

// CChildView 窗口

class CChildView : public CFormView
{
	DECLARE_DYNAMIC(CChildView)
// 构造
public:
	CChildView();

// 特性
public:

// 操作
public:

// 重写
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);
// 实现
public:
	virtual ~CChildView();
	virtual void OnInitialUpdate();
	void CreateChart();
	// 生成的消息映射函数
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

