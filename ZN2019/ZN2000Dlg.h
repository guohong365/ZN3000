
// ZN2000Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MainBaseCtrl.h"
#include "InfoPane.h"
#include "../libzn/ZNDData.h"


// CZN2000Dlg �Ի���
class CZN2000Dlg : public CXTResizeDialog
{
	RheographyRecord * _pRecord;
// ����
public:
	CZN2000Dlg(CWnd* pParent = nullptr);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZN2000_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	DWORD _dwState;
// ʵ��
protected:
	HICON m_hIcon;
	CXTPButton _btnPartSelect;
	CXTPButton _btnStart;
	CXTPButton _btnPause;
	CXTPButton _btnCalc;
	CXTPButton _btnInputParam;
	CXTPButton _btnSearch;
	CXTPButton _btnQuit;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPartSelectDropDown();
	afx_msg void OnPartSelected(UINT part);
	afx_msg HRESULT OnKickIdle(WPARAM, LPARAM)
	{
		UpdateDialogControls(this, TRUE);
		return 0;
	}

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic _mainBaseFrame;
	CStatic _InfoPaneFrame;
	CMainBaseCtrl _mainBaseCtrl;
	CInfoPane _infoPane;

	afx_msg void OnBnClickedBegin();
	afx_msg void OnUpdateBtnBegin(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(_dwState == (OS_PARAM_INPUT|OS_SELECT_PART) || (_dwState & OS_PAUSE));
	}
	afx_msg void OnBnClickedPause();
	afx_msg void OnUpdateBtnPause(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(_dwState & OS_BEGIN );
	}
	afx_msg void OnBnClickedSearch();
	afx_msg void OnUpdateBtnSearch(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(!(_dwState & OS_BEGIN));
	}
	afx_msg void OnBnClickedInput();
	afx_msg void OnUpdateBtnInputParam(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(!(_dwState & OS_BEGIN));
	}
	afx_msg void OnBnClickedCalc();
	afx_msg void OnUpdateBtnCalc(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(_dwState & OS_PAUSE);
	}
	afx_msg void OnUpdateBtnQuit(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(!(_dwState & OS_BEGIN));
	}
	afx_msg void OnUpdateBtnPartSelect(CCmdUI *pCmdUI)
	{
		pCmdUI->Enable(!(_dwState & OS_BEGIN));
	}
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
