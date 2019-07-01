#pragma once
#include <uc/signals/visualization/GridBackgronud.h>
#include <uc/signals/visualization/WaveDrawer.h>
#include <uc/signals/visualization/WaveCanvas.h>
#include <uc/signals/SampleController.h>
#include "resource.h"
#include "../libzn/ZNDData.h"
#include "uc/signals/SignalSeries.h"

// CWaveBaseCtrl 对话框
using uc::signals::SignalSeries;

class AFX_EXT_CLASS  CWaveBaseCtrl : public CDialog, public SampleController
{
	DECLARE_DYNAMIC(CWaveBaseCtrl)

public:
	CWaveBaseCtrl(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWaveBaseCtrl();
	void SetBuffers(SignalSeries* pAdmittance, SignalSeries* pDifferential,
	                SignalSeries* pEcg);
	void SetCurrentPart(PartId part);
	PartId GetCurrentPart() const;
	void SetDrawMode(uc::signals::visualization::DrawMode drawMode);
	void SetGridAppearance(uc::signals::visualization::GridBackgroundAppearance& backgroundAppearance);
	void SetWaveAppearance(uc::signals::visualization::WaveDrawerAppearance& waveDrawerAppearance);


	// 对话框数据
	enum { IDD = IDD_WAVE_BASE_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void start();
	virtual void stop();
private:
    uc::signals::visualization::WaveCanvas *_pCanvas;
	CBitmap  _memBitmap;
	PartId _currentPart;
	SignalSeries* _pAdmittance;
	SignalSeries* _pDifferential;
	SignalSeries* _pEcg;
    uc::signals::visualization::DrawMode _drawMode;
	LARGE_INTEGER _last;
	LARGE_INTEGER _frequency;
};
