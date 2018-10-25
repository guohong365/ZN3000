
// AutoAnnotationView.cpp : CAutoAnnotationView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "AutoAnnotation.h"
#endif

#include "MitbihTextDoc.h"
#include "AutoAnnotationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoAnnotationView

IMPLEMENT_DYNCREATE(CAutoAnnotationView, CFormView)

BEGIN_MESSAGE_MAP(CAutoAnnotationView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CAutoAnnotationView 构造/析构

CAutoAnnotationView::CAutoAnnotationView()
	: CFormView(CAutoAnnotationView::IDD)
{
	// TODO: 在此处添加构造代码

}

CAutoAnnotationView::~CAutoAnnotationView()
{
}

void CAutoAnnotationView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CAutoAnnotationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CAutoAnnotationView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}

void CAutoAnnotationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAutoAnnotationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAutoAnnotationView 诊断

#ifdef _DEBUG
void CAutoAnnotationView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAutoAnnotationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMitbihTextDoc* CAutoAnnotationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMitbihTextDoc)));
	return (CMitbihTextDoc*)m_pDocument;
}
#endif //_DEBUG


// CAutoAnnotationView 消息处理程序
