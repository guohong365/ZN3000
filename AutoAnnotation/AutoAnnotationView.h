
// AutoAnnotationView.h : CAutoAnnotationView ��Ľӿ�
//

#pragma once

#include "resource.h"


class CAutoAnnotationView : public CFormView
{
protected: // �������л�����
	CAutoAnnotationView();
	DECLARE_DYNCREATE(CAutoAnnotationView)

public:
	enum{ IDD = IDD_AUTOANNOTATION_FORM };

// ����
public:
	CMitbihTextDoc* GetDocument() const;

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
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AutoAnnotationView.cpp �еĵ��԰汾
inline CMitbihTextDoc* CAutoAnnotationView::GetDocument() const
   { return reinterpret_cast<CMitbihTextDoc*>(m_pDocument); }
#endif

