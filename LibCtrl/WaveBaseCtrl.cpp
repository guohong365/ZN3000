// MainBaseCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "WaveBaseCtrl.h"

#define GRID_LARGE_INTERVAL 10.0f
#define GRID_SMALL_INTERVAL 1.0f
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAMPLE_SHOW_TIMER_ID 1
#define SAMPLE_SHOW_ELAPSE   100
// CWaveBaseCtrl 对话框

static unsigned long counter=0;
static unsigned long drawCounter=0;
IMPLEMENT_DYNAMIC(CWaveBaseCtrl, CDialog)

CWaveBaseCtrl::CWaveBaseCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
	  , _currentPart(PART_NONE)
	  , _pAdmittance(nullptr)
	  , _pDifferential(nullptr)
	  , _pEcg(nullptr)
	  , _drawMode(DRAW_ROLLING)
{
	_last.QuadPart = 0;
	QueryPerformanceFrequency(&_frequency);
	_pCanvas = new WaveCanvas(Gdiplus::Point(0, 0), Gdiplus::Size(0, 0));
}

CWaveBaseCtrl::~CWaveBaseCtrl()
{
	delete _pCanvas;
	_memBitmap.DeleteObject();
}

void CWaveBaseCtrl::SetBuffers(SignalChannel* pAdmittance, SignalChannel* pDifferential,
                               SignalChannel* pEcg)
{
	_pAdmittance = pAdmittance;
	_pDifferential = pDifferential;
	_pEcg = pEcg;
}

void CWaveBaseCtrl::SetCurrentPart(PartId part)
{
	if(part == _currentPart) return;
	_currentPart = part;
	_pCanvas->Clear();
	_pCanvas->SetDrawMode(_drawMode);
	switch (part)
	{
	case PART_CALIBRATION:
		ASSERT(_pAdmittance && _pDifferential);		
		_pCanvas->AddWave(_pAdmittance, 50);
		_pCanvas->AddWave(_pDifferential, 50);		
		break;
	case PART_NONE:
		break;
	case PART_HEART:
		ASSERT(_pDifferential && _pEcg);
		_pCanvas->AddWave(_pDifferential, 60);
		_pCanvas->AddWave(_pEcg, 40);
		break;
	default:
		ASSERT(_pAdmittance);
		_pCanvas->AddWave(_pAdmittance, 100);
		break;
	}
	Invalidate();
}

PartId CWaveBaseCtrl::GetCurrentPart() const
{
	return _currentPart;
}

void CWaveBaseCtrl::SetDrawMode(DrawMode drawMode)
{	
	_pCanvas->SetDrawMode(drawMode);
	Invalidate();
}

void CWaveBaseCtrl::SetGridAppearance(GridBackgroundAppearance& backgroundAppearance)
{
	ASSERT(_pCanvas);
	_pCanvas->SetGridAppearance(backgroundAppearance);
}

void CWaveBaseCtrl::SetWaveAppearance(WaveDrawerAppearance& waveDrawerAppearance)
{
	ASSERT(_pCanvas);
	_pCanvas->SetWaveAppearance(waveDrawerAppearance);
}

void CWaveBaseCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaveBaseCtrl, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CWaveBaseCtrl::OnCancel()
{
}

void CWaveBaseCtrl::OnOK()
{
}

void CWaveBaseCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	drawCounter ++;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);
	if(_memBitmap.GetSafeHandle()== nullptr ||
		_memBitmap.GetBitmapDimension().cx!= rect.Width() ||
		_memBitmap.GetBitmapDimension().cy != rect.Height())
	{
		_memBitmap.DeleteObject();
		_memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());	
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

void CWaveBaseCtrl::OnSize(UINT nType, int cx, int cy)
{
	Gdiplus::Size sz(cx, cy);
	CDialog::OnSize(nType, cx, cy);
	UICoordinateHelper::GetHelper().DPtoLP(&sz, 1);
	_pCanvas->SetSize(sz);
}

HBRUSH CWaveBaseCtrl::OnCtlColor(CDC* /*pDC*/, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{
	return HBRUSH(GetStockObject(BLACK_BRUSH));
}


BOOL CWaveBaseCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	
}
void CWaveBaseCtrl::OnTimer(UINT_PTR nIDEvent)
{	
	if(nIDEvent!=SAMPLE_SHOW_TIMER_ID) return;
	/*
	counter ++;
	CString info;
	SignalBuffer<float> & buffer=_pCanvas->GetWave(0)->getChannelBuffer()->getSignalBuffer();
	info.Format(_T("%lu, %lu, %lu"), counter, buffer.getLength(), drawCounter);
	CWnd *pWnd=GetDlgItem(IDC_STATIC_COUNTER);
	if(pWnd)
	{
		pWnd->SetWindowText(info);
	}
	*/
	Invalidate();
}

BOOL CWaveBaseCtrl::OnEraseBkgnd( CDC* /*pDC*/ )
{
	return TRUE;
}

void CWaveBaseCtrl::stop()
{
	KillTimer(SAMPLE_SHOW_TIMER_ID);
}

void CWaveBaseCtrl::start()
{
	SetTimer(SAMPLE_SHOW_TIMER_ID, SAMPLE_SHOW_ELAPSE, nullptr);
}
