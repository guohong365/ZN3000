// MainBaseCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MainBaseCtrl.h"
#include "../libzn/Sampler.h"

#define GRID_LARGE_INTERVAL 10.0f
#define GRID_SMALL_INTERVAL 1.0f
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAMPLE_SHOW_TIMER_ID 1
#define SAMPLE_SHOW_ELAPSE   100
// CMainBaseCtrl 对话框

IMPLEMENT_DYNAMIC(CMainBaseCtrl, CDialog)

CMainBaseCtrl::CMainBaseCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
	, _pWaveDrawer(nullptr)
	, _currentPart(PART_NONE)
	, _pAdmittance(nullptr)
	, _pDifferential(nullptr)
	, _pEcg(nullptr)
{
	_pCanvas = new WaveCanvas(Gdiplus::Point(0, 0), Gdiplus::Size(0, 0));
}

CMainBaseCtrl::~CMainBaseCtrl()
{
	delete _pCanvas;
	_memBitmap.DeleteObject();
}

void CMainBaseCtrl::SetBuffers(SignalChannel* pAdmittance, SignalChannel* pDifferential,
                               SignalChannel* pEcg)
{
	_pAdmittance = pAdmittance;
	_pDifferential = pDifferential;
	_pEcg = pEcg;
}

void CMainBaseCtrl::SetCurrentPart(PartId part)
{
	if(part == _currentPart) return;
	_currentPart = part;
	_pCanvas->Clear();
	switch (part)
	{
	case PART_NONE:
		break;
	case PART_HEART:		
		_pCanvas->AddWave(_pDifferential, 60);
		_pCanvas->AddWave(_pEcg, 40);
		break;
	default:
		_pCanvas->AddWave(_pAdmittance, 100);
		break;
	}
}

void CMainBaseCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainBaseCtrl, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CMainBaseCtrl::OnCancel()
{
}

void CMainBaseCtrl::OnOK()
{
}

void CMainBaseCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);
	if(_memBitmap.GetSafeHandle()== nullptr ||
		_memBitmap.GetBitmapDimension().cx!= rect.Width() ||
		_memBitmap.GetBitmapDimension().cy != rect.Height())
	{
		_memBitmap.DeleteObject();
		_background.DeleteObject();
		_memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());	
		_background.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	}
	CBitmap * pOldBitmap=static_cast<CBitmap*>(memDC.SelectObject(&_memBitmap));
	
	memDC.SetMapMode(MM_ANISOTROPIC);

	CSize sz(1000,1000);
	memDC.SetWindowExt(sz);
	UICoordinateHelper::GetHelper().LPtoDP(&sz, 1);
	memDC.SetViewportExt(sz);

	Gdiplus::Graphics graph(memDC.GetSafeHdc());

	_pCanvas->Draw(graph);
	dc.BitBlt(0,0,rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

void CMainBaseCtrl::OnSize(UINT nType, int cx, int cy)
{
	Gdiplus::Size sz(cx, cy);
	CDialog::OnSize(nType, cx, cy);
	UICoordinateHelper::GetHelper().DPtoLP(&sz, 1);
	_pCanvas->SetSize(sz);
}

HBRUSH CMainBaseCtrl::OnCtlColor(CDC* /*pDC*/, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{
	return HBRUSH(GetStockObject(BLACK_BRUSH));
}


BOOL CMainBaseCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	
}
void CMainBaseCtrl::OnTimer(UINT_PTR nIDEvent)
{	
	if(nIDEvent==1)
	{
		Invalidate(TRUE);
	}
}

BOOL CMainBaseCtrl::OnEraseBkgnd( CDC* /*pDC*/ )
{
	return TRUE;
}

void CMainBaseCtrl::stop()
{
	KillTimer(SAMPLE_SHOW_TIMER_ID);
}

void CMainBaseCtrl::start()
{
	SetTimer(SAMPLE_SHOW_TIMER_ID, SAMPLE_SHOW_ELAPSE, nullptr);
}
