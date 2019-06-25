
// AutoAnnotationView.h : CAutoAnnotationView ��Ľӿ�
//

#pragma once

#include "resource.h"
#include "../LibCtrl/WaveBaseCtrl.h"


class CAutoAnnotationView : public CFormView
{
protected: // �������л�����
	CAutoAnnotationView();
	DECLARE_DYNCREATE(CAutoAnnotationView)

public:
	enum{ IDD = IDD_AUTOANNOTATION_FORM };

// ����
public:
	CMitbihDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CAutoAnnotationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
    CWaveBaseCtrl _waveCtrl;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // AutoAnnotationView.cpp �еĵ��԰汾
inline CMitbihTextDoc* CAutoAnnotationView::GetDocument() const
   { return reinterpret_cast<CMitbihTextDoc*>(m_pDocument); }
#endif

