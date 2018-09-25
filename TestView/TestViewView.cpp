
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
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTestViewView ����/����

CTestViewView::CTestViewView(): _pBackground(nullptr)
{	
}

CTestViewView::~CTestViewView()
{
	delete _pBackground;
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

	//CDC memDC;
	//memDC.CreateCompatibleDC(pDC);
	CRect rect;
	GetClientRect(&rect);
	if(_memBitmap.GetSafeHandle()== nullptr ||
		_memBitmap.GetBitmapDimension().cx != rect.Width() || 
		_memBitmap.GetBitmapDimension().cy !=rect.Height())
	{
		_memBitmap.DeleteObject();		
		_memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		_pBackground = BitmapCreate(rect.Width(), rect.Height(), PixelFormat32bppARGB);
		Gdiplus::Graphics back(_pBackground);
		back.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		back.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		back.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
		back.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		CSize sz(1000,1000);
		ScreenInfo screen;
		back.ScaleTransform(1.0f/UICoordinateHelper::GetHelper().HorizontalLmPerDeviceUnit, 1.0f/UICoordinateHelper::GetHelper().VerticalLmPerDeviceUnit);	
		//back.ScaleTransform(0.1/ScreenInfo::GetScreenInfo().GetDpmmX(), 0.1/ScreenInfo::GetScreenInfo().GetDpmmY());
		GetDocument()->GetBackground()->Draw(back);
	}

	//CBitmap * pOldBitmap=static_cast<CBitmap*>(memDC.SelectObject(&_memBitmap));
	//memDC.SetMapMode(MM_ANISOTROPIC);
	//CSize sz(1000,1000);
	//memDC.SetWindowExt(sz);
	//UICoordinateHelper::GetHelper().LPtoDP(&sz, 1);
	//memDC.SetViewportExt(sz);
	//Gdiplus::Graphics graph(memDC.GetSafeHdc());
	//graph.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
	//GetDocument()->GetBackground()->Draw(graph);

	//pDC->BitBlt(0,0,rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	//memDC.SelectObject(pOldBitmap);
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	Gdiplus::RectF bounds;
	Gdiplus::Unit unit;
	_pBackground->GetBounds(&bounds, &unit);
	graphics.DrawImage(_pBackground, Gdiplus::Rect(0,0, rect.Width(), rect.Height()),
		0,0, _pBackground->GetWidth(), _pBackground->GetHeight(), Gdiplus::UnitPixel);
	if(GetDocument()->GetCanvas())
	{
		graphics.ScaleTransform(1.0f/UICoordinateHelper::GetHelper().HorizontalLmPerDeviceUnit, 1.0f/UICoordinateHelper::GetHelper().VerticalLmPerDeviceUnit);
		GetDocument()->GetCanvas()->Draw(graphics);
	}

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
	Gdiplus::Size sz(cx, cy);	
	UICoordinateHelper::GetHelper().DPtoLP(&sz, 1);	
	if(GetDocument()->GetCanvas()!= nullptr)
	{
		GetDocument()->GetCanvas()->SetSize(sz);
	}
	if(GetDocument()->GetBackground()!=nullptr)
	{
		GetDocument()->GetBackground()->SetSize(sz);
	}
}


BOOL CTestViewView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
