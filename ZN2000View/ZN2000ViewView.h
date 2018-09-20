
// ZN2000ViewView.h : CZN2000ViewView 类的接口
//

#pragma once

#include "../libzn/WaveDrawer.h"
#include "../libzn/WaveCanvas.h"
class CZN2000ViewView : public CView
{
	WaveDrawer* _pWaveDrawer;
	WaveCanvas *_pCanvase;
	CBitmap  _MemBitmap;
protected: // 仅从序列化创建
	CZN2000ViewView();
	DECLARE_DYNCREATE(CZN2000ViewView)

// 特性
public:
	CZN2000ViewDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CZN2000ViewView();
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
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // ZN2000ViewView.cpp 中的调试版本
inline CZN2000ViewDoc* CZN2000ViewView::GetDocument() const
   { return reinterpret_cast<CZN2000ViewDoc*>(m_pDocument); }
#endif

