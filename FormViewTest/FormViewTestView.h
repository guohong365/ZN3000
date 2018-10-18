
// FormViewTestView.h : CFormViewTestView 类的接口
//

#pragma once

#include "resource.h"


class CFormViewTestView : public CFormView
{
protected: // 仅从序列化创建
	CFormViewTestView();
	DECLARE_DYNCREATE(CFormViewTestView)

public:
	enum{ IDD = IDD_FORMVIEWTEST_FORM };

// 特性
public:
	CFormViewTestDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CFormViewTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FormViewTestView.cpp 中的调试版本
inline CFormViewTestDoc* CFormViewTestView::GetDocument() const
   { return reinterpret_cast<CFormViewTestDoc*>(m_pDocument); }
#endif

