
// TestViewView.cpp : CTestViewView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TestView.h"
#endif
#include "../libzn/WaveCanvas.h"
#include "TestViewDoc.h"
#include "TestViewView.h"
#include "../libDrawObject/UIHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestViewView

IMPLEMENT_DYNCREATE(CTestViewView, CView)

BEGIN_MESSAGE_MAP(CTestViewView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTestViewView ����/����

CTestViewView::CTestViewView()
{
	// TODO: �ڴ˴���ӹ������

}

CTestViewView::~CTestViewView()
{
}

BOOL CTestViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTestViewView ����

void CTestViewView::OnDraw(CDC* pDC)
{
	CTestViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CRect rect;
	GetClientRect(&rect);
	if(_MemBitmap.GetSafeHandle()==NULL )
	{
		_MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	}

	CBitmap * pOldBitmap=(CBitmap*)memDC.SelectObject(&_MemBitmap);
	memDC.SetMapMode(MM_ANISOTROPIC);

	CSize sz(1000,1000);
	memDC.SetWindowExt(sz);
	CUICoordinateHelper::GetHelper().LPtoDP(&sz, 1);
	memDC.SetViewportExt(sz);

	Gdiplus::Graphics graph(memDC.GetSafeHdc());
	//LARGE_INTEGER begin;
	//LARGE_INTEGER end;
	//QueryPerformanceCounter(&begin);
	GetDocument()->GetCanvas()->Draw(graph);
	pDC->BitBlt(0,0,rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}


// CTestViewView ��ӡ


void CTestViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CTestViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTestViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CTestViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestViewView ���

#ifdef _DEBUG
void CTestViewView::AssertValid() const
{
	CView::AssertValid();
}

void CTestViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestViewDoc* CTestViewView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestViewDoc)));
	return (CTestViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestViewView ��Ϣ�������


void CTestViewView::OnInitialUpdate()
{
	CView::OnInitialUpdate();	
}


void CTestViewView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(GetDocument()->GetCanvas()== nullptr) return;
	CSize sz(cx, cy);	
	CUICoordinateHelper::GetHelper().DPtoLP(&sz, 1);	
	GetDocument()->GetCanvas()->SetSize(Gdiplus::Size(sz.cx, sz.cy));
}
