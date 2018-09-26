
// ZN2000Dlg.h : ͷ�ļ�
//

#pragma once
#include "InfoPane.h"
#include "../libzn/ZNDData.h"
#include "../libzn/SerialPortSampler.h"
#include "../libzn/ZnRecord.h"
#include "../LibCtrl/WaveBaseCtrl.h"


// CZN2000Dlg �Ի���
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
	CWaveBaseCtrl _mainBaseCtrl;
	CInfoPane _infoPane;
	public:
	CZN2000Dlg(CWnd* pParent = nullptr);	// ��׼���캯��
	virtual ~CZN2000Dlg();

// �Ի�������
	enum { IDD = IDD_ZN2000_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	Settings& getSettings() const;

// ʵ��
protected:

	// ���ɵ���Ϣӳ�亯��
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
