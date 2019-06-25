#pragma once
#include <uc/dsp/visualization/GridBackgronud.h>
#include <uc/dsp/visualization/WaveDrawer.h>
#include <uc/dsp/visualization/WaveCanvas.h>
#include <uc/dsp/SampleController.h>
#include "resource.h"
#include "../libzn/ZNDData.h"

// CWaveBaseCtrl 对话框


class AFX_EXT_CLASS  CWaveBaseCtrl : public CDialog, public SampleController
{
	DECLARE_DYNAMIC(CWaveBaseCtrl)

public:
	CWaveBaseCtrl(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWaveBaseCtrl();
	void SetBuffers(Signal* pAdmittance, Signal* pDifferential,
	                Signal* pEcg);
	void SetCurrentPart(PartId part);
	PartId GetCurrentPart() const;
	void SetDrawMode(uc::dsp::visualization::DrawMode drawMode);
	void SetGridAppearance(uc::dsp::visualization::GridBackgroundAppearance& backgroundAppearance);
	void SetWaveAppearance(uc::dsp::visualization::WaveDrawerAppearance& waveDrawerAppearance);


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
    uc::dsp::visualization::WaveCanvas *_pCanvas;
	CBitmap  _memBitmap;
	PartId _currentPart;
	Signal* _pAdmittance;
	Signal* _pDifferential;
	Signal* _pEcg;
    uc::dsp::visualization::DrawMode _drawMode;
	LARGE_INTEGER _last;
	LARGE_INTEGER _frequency;
};
