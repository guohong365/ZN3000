#pragma once
#include "../libzn/GridBackgronud.h"
#include "../libzn/WaveDrawer.h"
#include "../libzn/WaveCanvas.h"
#include "../libzn/ZnRecord.h"
#include "resource.h"
#include "../libzn/SampleController.h"


// CMainBaseCtrl �Ի���

class CMainBaseCtrl : public CDialog, public SampleController
{
	DECLARE_DYNAMIC(CMainBaseCtrl)

public:
	CMainBaseCtrl(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CMainBaseCtrl();
	void SetBuffers(SignalChannel* pAdmittance, SignalChannel* pDifferential,
	                SignalChannel* pEcg);
	void SetCurrentPart(PartId part);

	// �Ի�������
	enum { IDD = IDD_MAIN_BASE_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CBitmap _background;
	WaveDrawer* _pWaveDrawer;
	WaveCanvas *_pCanvas;
	CBitmap  _memBitmap;
	PartId _currentPart;
	SignalChannel* _pAdmittance;
	SignalChannel* _pDifferential;
	SignalChannel* _pEcg;
	ULONG _current;
};
