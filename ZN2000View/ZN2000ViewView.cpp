
// ZN2000ViewView.cpp : CZN2000ViewView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ZN2000View.h"
#endif

#include "ZN2000ViewDoc.h"
#include "ZN2000ViewView.h"
#include "../libDrawObject/UIHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZN2000ViewView

IMPLEMENT_DYNCREATE(CZN2000ViewView, CView)

BEGIN_MESSAGE_MAP(CZN2000ViewView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CZN2000ViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CZN2000ViewView 构造/析构

CZN2000ViewView::CZN2000ViewView()
	:_pCanvase(NULL)
	,_pWaveDrawer(NULL)
{
	// TODO: 在此处添加构造代码

}

CZN2000ViewView::~CZN2000ViewView()
{
	delete _pCanvase;
}

BOOL CZN2000ViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CZN2000ViewView 绘制

void CZN2000ViewView::OnDraw(CDC* pDC)
{
	CZN2000ViewDoc* pDoc = GetDocument();
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
	_pCanvase->Draw(graph);
	pDC->BitBlt(0,0,rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	//QueryPerformanceCounter(&end);
	//double duiation=(double)(end.QuadPart - begin.QuadPart)/_frequency.QuadPart;
}


// CZN2000ViewView 打印


void CZN2000ViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CZN2000ViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CZN2000ViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CZN2000ViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CZN2000ViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CZN2000ViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CZN2000ViewView 诊断

#ifdef _DEBUG
void CZN2000ViewView::AssertValid() const
{
	CView::AssertValid();
}

void CZN2000ViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZN2000ViewDoc* CZN2000ViewView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZN2000ViewDoc)));
	return (CZN2000ViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CZN2000ViewView 消息处理程序


void CZN2000ViewView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	int part=-1;
	float minData;
	float maxData;
	
	for(int i=0; i< PART_MAX; i++)
	{
		if(zndBuffer.PZndHeader->offset[i]!=0)
		{
			part=i;
			break;
		}
	}
	pBuffer=new SignalBuffer<long>(ZND_DATA_SIZE, zndBuffer.zndData[part].sampleInterval);
		 minData=minBuffer(zndBuffer.zndData[part].pWave, ZND_DATA_SIZE);
		 maxData=maxBuffer(zndBuffer.zndData[part].pWave, ZND_DATA_SIZE);
		copyBufferNormalized(pBuffer->getBuffer(), zndBuffer.zndData[part].pWave, ZND_DATA_SIZE, 40960.0/(maxData-minData), minData);
	
	pBuffer->setLength(ZND_DATA_SIZE);
	pBuffer->setName(_T("导纳微分波"));

	_pCanvase=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(0,0));
	_pCanvase->AddWave(pBuffer, 50);
	if(part==0)
	{
		pBuffer=new SignalBuffer<long>(ZND_DATA_SIZE, zndBuffer.zndData[part].sampleInterval);
		minData=minBuffer(zndBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE);
		maxData=maxBuffer(zndBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE);
		copyBufferNormalized(pBuffer->getBuffer(), zndBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE, 40960.0/(maxData-minData), minData);
		pBuffer->setLength(ZND_DATA_SIZE);
		pBuffer->setName(_T("心电"));
		_pCanvase->AddWave(pBuffer, 50);
	}

}


void CZN2000ViewView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if(_pCanvase== nullptr) return;
	CSize sz(cx, cy);	
	CUICoordinateHelper::GetHelper().DPtoLP(&sz, 1);	
	_pCanvase->SetSize(Gdiplus::Size(sz.cx, sz.cy));
	//_pCanvase->PrepareCanvas(cx, cy);
}
