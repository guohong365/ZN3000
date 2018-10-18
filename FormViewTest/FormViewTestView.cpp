
// FormViewTestView.cpp : CFormViewTestView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FormViewTest.h"
#endif

#include "FormViewTestDoc.h"
#include "FormViewTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFormViewTestView

IMPLEMENT_DYNCREATE(CFormViewTestView, CFormView)

BEGIN_MESSAGE_MAP(CFormViewTestView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CFormViewTestView 构造/析构

CFormViewTestView::CFormViewTestView()
	: CFormView(CFormViewTestView::IDD)
{
	// TODO: 在此处添加构造代码

}

CFormViewTestView::~CFormViewTestView()
{
}

void CFormViewTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CFormViewTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CFormViewTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CFormViewTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFormViewTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFormViewTestView 诊断

#ifdef _DEBUG
void CFormViewTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormViewTestDoc* CFormViewTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormViewTestDoc)));
	return (CFormViewTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CFormViewTestView 消息处理程序
