
// ZN2000Dlg.h : 头文件
//

#pragma once
#include "MainBaseCtrl.h"
#include "InfoPane.h"
#include "../libzn/ZNDData.h"
#include "../libzn/SerialPortSampler.h"


// CZN2000Dlg 对话框
class CZN2000Dlg : public CXTResizeDialog
{
	SerialPortSampler _sampler;
	ZnRecordImpl *_pRecord;
	SignalChannel * _pFeedback;
	SignalChannel * _pAdmittance;
	SignalChannel * _pDifferential;
	SignalChannel * _pEcg;
	PartId _currentPart;
	DWORD _dwState;
	HICON _hIcon;
	CXTPButton _btnPartSelect;
	CXTPButton _btnStart;
	CXTPButton _btnPause;
	CXTPButton _btnCalc;
	CXTPButton _btnInputParam;
	CXTPButton _btnSearch;
	CXTPButton _btnQuit;
	CStatic _mainBaseFrame;
	CStatic _InfoPaneFrame;
	CMainBaseCtrl _mainBaseCtrl;
	CInfoPane _infoPane;
	public:
	CZN2000Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ZN2000_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPartSelectDropDown();
	afx_msg void OnPartSelected(UINT part);
	afx_msg HRESULT OnKickIdle(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	afx_msg void OnBnClickedBegin();
	afx_msg void OnUpdateBtnBegin(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedPause();
	afx_msg void OnUpdateBtnPause(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedSearch();
	afx_msg void OnUpdateBtnSearch(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedInput();
	afx_msg void OnUpdateBtnInputParam(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedCalc();
	afx_msg void OnUpdateBtnCalc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnQuit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnPartSelect(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
};
