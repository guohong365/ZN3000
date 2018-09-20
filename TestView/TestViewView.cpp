
// TestViewView.cpp : CTestViewView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTestViewView 构造/析构

CTestViewView::CTestViewView()
{
	// TODO: 在此处添加构造代码

}

CTestViewView::~CTestViewView()
{
}

BOOL CTestViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestViewView 绘制

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


// CTestViewView 打印


void CTestViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CTestViewView 诊断

#ifdef _DEBUG
void CTestViewView::AssertValid() const
{
	CView::AssertValid();
}

void CTestViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestViewDoc* CTestViewView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestViewDoc)));
	return (CTestViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestViewView 消息处理程序


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
