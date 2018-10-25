
// AutoAnnotationView.cpp : CAutoAnnotationView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CAutoAnnotationView ����/����

CAutoAnnotationView::CAutoAnnotationView()
	: CFormView(CAutoAnnotationView::IDD)
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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


// CAutoAnnotationView ���

#ifdef _DEBUG
void CAutoAnnotationView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAutoAnnotationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMitbihTextDoc* CAutoAnnotationView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMitbihTextDoc)));
	return (CMitbihTextDoc*)m_pDocument;
}
#endif //_DEBUG


// CAutoAnnotationView ��Ϣ�������
