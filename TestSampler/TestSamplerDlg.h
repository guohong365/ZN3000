
// TestSamplerDlg.h : ͷ�ļ�
//

#pragma once
#include "../LibCtrl/MainBaseCtrl.h"
#include "../libzn/SerialPortSampler.h"


// CTestSamplerDlg �Ի���
class CTestSamplerDlg : public CXTResizeDialog
{
// ����
public:
	CTestSamplerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTSAMPLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
