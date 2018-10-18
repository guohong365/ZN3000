
// FormViewTestView.cpp : CFormViewTestView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CFormViewTestView ����/����

CFormViewTestView::CFormViewTestView()
	: CFormView(CFormViewTestView::IDD)
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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


// CFormViewTestView ���

#ifdef _DEBUG
void CFormViewTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormViewTestDoc* CFormViewTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormViewTestDoc)));
	return (CFormViewTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CFormViewTestView ��Ϣ�������
