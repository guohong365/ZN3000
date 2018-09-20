#pragma once

#include "WaveCanvas.h"

// CWaveCtrl �Ի���

class CWaveCtrl : public CDialog
{
	DECLARE_DYNAMIC(CWaveCtrl)
	WaveCanvas* _pCanvas;
public:
	CWaveCtrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaveCtrl();

// �Ի�������
	enum { IDD = IDD_WAVE_CTRL };

template <class VALUE_TYPE>
	void AddWave( WaveBuffer<VALUE_TYPE> *pWaveBuffer, float percent)
	{

	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
