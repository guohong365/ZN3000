
// XTPZN3000Dlg.h : ͷ�ļ�
//

#pragma once
#include "../libzn/Sampler.h"
#include "../libzn/SignalChannelImpl.h"


// CXTPZN3000Dlg �Ի���
class CXTPZN3000Dlg : public CXTPResizeDialog
{
// ����
public:
	CXTPZN3000Dlg(CWnd* pParent = nullptr);	// ��׼���캯��
    ~CXTPZN3000Dlg();

// �Ի�������
	enum { IDD = IDD_XTPZN3000_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

    static Settings& GetSettings();
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg HRESULT OnKickIdle(WPARAM wParam, LPARAM lParam)
    {
        UpdateDialogControls(this, TRUE);
        return 0;
    }

    afx_msg void OnBnClickedBegin();
    afx_msg void OnUpdateBtnBegin(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioCalibration();
	afx_msg void OnBnClickedRadioErasure();
	afx_msg void OnBnClickedRadioExamine();
	afx_msg void OnBnClickedRadioRolling();
	afx_msg void OnBnClickedStop();
    afx_msg void OnUpdateBtnStop(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void Destroy();
    virtual void OnCancel();

private:
	void _initChart();
	CXTPChartControl _wndCharControl;
	Sampler * _pSampler;
	SignalChannel *	_pFeedback;
	SignalChannel *	_pAdmittance;
	SignalChannel *	_pDifferential;
	SignalChannel * _pEcg;
    SignalBuffer<float> * _points;
    SIZE_T _last;
    DWORD _state;
};
