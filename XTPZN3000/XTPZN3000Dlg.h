
// XTPZN3000Dlg.h : 头文件
//

#pragma once
#include "../libzn/Sampler.h"
#include "../libzn/SignalChannelImpl.h"


// CXTPZN3000Dlg 对话框
class CXTPZN3000Dlg : public CXTPResizeDialog
{
// 构造
public:
	CXTPZN3000Dlg(CWnd* pParent = nullptr);	// 标准构造函数
    ~CXTPZN3000Dlg();

// 对话框数据
	enum { IDD = IDD_XTPZN3000_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

    static Settings& GetSettings();
	// 生成的消息映射函数
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
