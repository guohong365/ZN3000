
// FormViewTestView.h : CFormViewTestView ��Ľӿ�
//

#pragma once

#include "resource.h"


class CFormViewTestView : public CFormView
{
protected: // �������л�����
	CFormViewTestView();
	DECLARE_DYNCREATE(CFormViewTestView)

public:
	enum{ IDD = IDD_FORMVIEWTEST_FORM };

// ����
public:
	CFormViewTestDoc* GetDocument() const;

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
	virtual ~CFormViewTestView();
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

#ifndef _DEBUG  // FormViewTestView.cpp �еĵ��԰汾
inline CFormViewTestDoc* CFormViewTestView::GetDocument() const
   { return reinterpret_cast<CFormViewTestDoc*>(m_pDocument); }
#endif

