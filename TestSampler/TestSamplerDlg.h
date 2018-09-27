
// TestSamplerDlg.h : 头文件
//

#pragma once
#include "../LibCtrl/WaveBaseCtrl.h"
#include "../libzn/SerialPortSampler.h"


// CTestSamplerDlg 对话框
class CTestSamplerDlg : public CXTResizeDialog
{
// 构造
public:
	CTestSamplerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CTestSamplerDlg()
	{
		delete _pFeedback;
		delete _pAdmittance;
		delete _pDifferential;
		delete _pEcg;
	}
// 对话框数据
	enum { IDD = IDD_TESTSAMPLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	GridBackgroundAppearance createGridAppearance();
	static WaveDrawerAppearance createWaveAppearance();
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	int _workMode;
	int _showMode;
	CStatic _waveFrame;
	CWaveBaseCtrl _waveCtrl;
	SignalChannel *	_pFeedback;
	SignalChannel *	_pAdmittance;
	SignalChannel *	_pDifferential;
	SignalChannel * _pEcg;
	SerialPortSampler _sampler;
	DWORD _dwState;
public:
	afx_msg void OnBnClickedBegin();
	afx_msg void OnUpdateBtnBegin(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedStop();
	afx_msg void OnUpdateBtnStop(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioCalibration();
	afx_msg void OnBnClickedRadioExamine();
	afx_msg void OnBnClickedRadioRolling();
	afx_msg void OnBnClickedRadioErasure();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
