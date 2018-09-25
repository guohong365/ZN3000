
// TestSamplerDlg.h : 头文件
//

#pragma once
#include "../LibCtrl/MainBaseCtrl.h"
#include "../libzn/SerialPortSampler.h"


// CTestSamplerDlg 对话框
class CTestSamplerDlg : public CXTResizeDialog
{
// 构造
public:
	CTestSamplerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTSAMPLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
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
public:
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedRadioCalibration();
	afx_msg void OnBnClickedRadioExamine();
	afx_msg void OnBnClickedRadioRolling();
	afx_msg void OnBnClickedRadioErasure();
};
