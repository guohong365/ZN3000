
// ZN2000ViewView.cpp : CZN2000ViewView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ZN2000View.h"
#endif

#include "ZN2000ViewDoc.h"
#include "ZN2000ViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZN2000ViewView

IMPLEMENT_DYNCREATE(CZN2000ViewView, CView)

BEGIN_MESSAGE_MAP(CZN2000ViewView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CZN2000ViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CZN2000ViewView ����/����

CZN2000ViewView::CZN2000ViewView()
	:_pCanvase(NULL)
	,_pWaveDrawer(NULL)
{
	// TODO: �ڴ˴���ӹ������

}

CZN2000ViewView::~CZN2000ViewView()
{
	delete _pCanvase;
}

BOOL CZN2000ViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CZN2000ViewView ����

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
	UICoordinateHelper::GetHelper().LPtoDP(&sz, 1);
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


// CZN2000ViewView ��ӡ


void CZN2000ViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CZN2000ViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CZN2000ViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CZN2000ViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CZN2000ViewView ���

#ifdef _DEBUG
void CZN2000ViewView::AssertValid() const
{
	CView::AssertValid();
}

void CZN2000ViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZN2000ViewDoc* CZN2000ViewView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZN2000ViewDoc)));
	return (CZN2000ViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CZN2000ViewView ��Ϣ�������


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
	pBuffer->setName(_T("����΢�ֲ�"));

	_pCanvase=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(0,0));
	_pCanvase->AddWave(pBuffer, 50);
	if(part==0)
	{
		pBuffer=new SignalBuffer<long>(ZND_DATA_SIZE, zndBuffer.zndData[part].sampleInterval);
		minData=minBuffer(zndBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE);
		maxData=maxBuffer(zndBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE);
		copyBufferNormalized(pBuffer->getBuffer(), zndBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE, 40960.0/(maxData-minData), minData);
		pBuffer->setLength(ZND_DATA_SIZE);
		pBuffer->setName(_T("�ĵ�"));
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
