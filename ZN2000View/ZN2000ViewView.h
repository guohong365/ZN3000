
// ZN2000ViewView.h : CZN2000ViewView ��Ľӿ�
//

#pragma once

#include "../libzn/WaveDrawer.h"
#include "../libzn/WaveCanvas.h"
class CZN2000ViewView : public CView
{
	WaveDrawer* _pWaveDrawer;
	WaveCanvas *_pCanvase;
	CBitmap  _MemBitmap;
protected: // �������л�����
	CZN2000ViewView();
	DECLARE_DYNCREATE(CZN2000ViewView)

// ����
public:
	CZN2000ViewDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CZN2000ViewView();
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
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // ZN2000ViewView.cpp �еĵ��԰汾
inline CZN2000ViewDoc* CZN2000ViewView::GetDocument() const
   { return reinterpret_cast<CZN2000ViewDoc*>(m_pDocument); }
#endif

