
// TestSamplerDlg.h : ͷ�ļ�
//

#pragma once
#include "../LibCtrl/WaveBaseCtrl.h"
#include "../libzn/SerialPortSampler.h"


// CTestSamplerDlg �Ի���
class CTestSamplerDlg : public CXTResizeDialog
{
// ����
public:
	CTestSamplerDlg(CWnd* pParent = nullptr);	// ��׼���캯��
	~CTestSamplerDlg()
	{
		delete _pFeedback;
		delete _pAdmittance;
		delete _pDifferential;
		delete _pEcg;
	}
// �Ի�������
	enum { IDD = IDD_TESTSAMPLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	GridBackgroundAppearance createGridAppearance();
	static WaveDrawerAppearance createWaveAppearance();
	// ���ɵ���Ϣӳ�亯��
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
