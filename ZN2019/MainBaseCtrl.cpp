// MainBaseCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "zn2000.h"
#include "MainBaseCtrl.h"
#include "../libzn/SignalBuffer.h"
#include "../libzn/Sampler.h"
#include "../libzn/ZNDLoader.h"
#include "../libzn/patient.h"
#include "../libDrawObject/UIHelper.h"

#define GRID_LARGE_INTERVAL 10.0f
#define GRID_SMALL_INTERVAL 1.0f
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAMPLE_SHOW_TIMER_ID 1
#define SAMPLE_SHOW_ELAPSE   100
// CMainBaseCtrl 对话框

IMPLEMENT_DYNAMIC(CMainBaseCtrl, CDialog)

typedef SignalBuffer<long> WAVE_BUFFER_TYPE;

CMainBaseCtrl::CMainBaseCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CMainBaseCtrl::IDD, pParent)
{
	memset(&_zndFileBuffer, 0, sizeof(ZndFileBuffer));
	SineSampleGenerator gen;
	
	WAVE_BUFFER_TYPE * pBuffer;

	int part=-1;
float minData;
float maxData;
	if(ZNDLoader::load(_T("2.znd"), &_zndFileBuffer)==0 )
	{
		for(int i=0; i< PART_MAX; i++)
		{
			if(_zndFileBuffer.PZndHeader->offset[i]!=0)
			{
				part=i;
				break;
			}
		}
		pBuffer=new WAVE_BUFFER_TYPE(ZND_DATA_SIZE, _zndFileBuffer.zndData[part].sampleInterval);
		 minData=minBuffer(_zndFileBuffer.zndData[part].pWave, ZND_DATA_SIZE);
		 maxData=maxBuffer(_zndFileBuffer.zndData[part].pWave, ZND_DATA_SIZE);
		copyBufferNormalized(pBuffer->getBuffer(), _zndFileBuffer.zndData[part].pWave, ZND_DATA_SIZE, 40960.0/(maxData-minData), minData);
	}	
	else
	{
		pBuffer=new WAVE_BUFFER_TYPE(ZND_DATA_SIZE, 0.002);
		gen.generator(pBuffer->getBuffer(), pBuffer->getSize(), 2, 30);
	}
	pBuffer->setLength(ZND_DATA_SIZE);
	pBuffer->setName(_T("导纳微分波"));

	_pCavase=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(0,0));
	_pCavase->AddWave(pBuffer, 50);
	if(part==0)
	{
		pBuffer=new WAVE_BUFFER_TYPE(ZND_DATA_SIZE, _zndFileBuffer.zndData[part].sampleInterval);
		minData=minBuffer(_zndFileBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE);
		maxData=maxBuffer(_zndFileBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE);
		copyBufferNormalized(pBuffer->getBuffer(), _zndFileBuffer.zndData[part].pHeartECG, ZND_DATA_SIZE, 40960.0/(maxData-minData), minData);
		pBuffer->setLength(ZND_DATA_SIZE);
		pBuffer->setName(_T("心电"));

		_pCavase->AddWave(pBuffer, 50);
	}
	//else
	//{
	//	pBuffer=new WAVE_BUFFER_TYPE(ZND_DATA_SIZE, 0.002);
	//	gen.generator(pBuffer->pBuffer, pBuffer->total, 2, 4);
	//}

	_last.QuadPart=0;
	_current.QuadPart=0;
	QueryPerformanceFrequency(&_frequency);


}

CMainBaseCtrl::~CMainBaseCtrl()
{
	delete _pCavase;
	_MemBitmap.DeleteObject();
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


// CMainBaseCtrl 消息处理程序

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
	if(_MemBitmap.GetSafeHandle()==NULL )
	{
		_MemBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	}

	CBitmap * pOldBitmap=(CBitmap*)memDC.SelectObject(&_MemBitmap);
	memDC.SetMapMode(MM_ANISOTROPIC);

	CSize sz(1000,1000);
	memDC.SetWindowExt(sz);
	CUICoordinateHelper::GetHelper().LPtoDP(&sz, 1);
	memDC.SetViewportExt(sz);

	Gdiplus::Graphics graph(memDC.GetSafeHdc());
	LARGE_INTEGER begin;
	LARGE_INTEGER end;
	QueryPerformanceCounter(&begin);
	_pCavase->Draw(graph);
	dc.BitBlt(0,0,rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	QueryPerformanceCounter(&end);
	double duiation=(double)(end.QuadPart - begin.QuadPart)/_frequency.QuadPart;
}

void CMainBaseCtrl::OnSize(UINT nType, int cx, int cy)
{
	CSize sz(cx, cy);
	CDialog::OnSize(nType, cx, cy);
	CUICoordinateHelper::GetHelper().DPtoLP(&sz, 1);
	//_pCavase->SetSize(Gdiplus::Size(cx/_screenInfo.GetDpmmX()*10, cy/_screenInfo.GetDpmmY()* 10));
	_pCavase->SetSize(Gdiplus::Size(sz.cx, sz.cy));
	_pCavase->PrepareCanvas(cx, cy);
	//CPaintDC dc(this);
	//_MemBitmap.DeleteObject();
	//_MemBitmap.CreateCompatibleBitmap(&dc, cx, cy);
}

HBRUSH CMainBaseCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//CBrush brush(RGB(0,255,0));

	//hbr=(HBRUSH)brush.Detach();

	hbr=(HBRUSH)GetStockObject(BLACK_BRUSH);
	return hbr;
}


BOOL CMainBaseCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMainBaseCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		if(_last.QuadPart>0)
		{
			QueryPerformanceCounter(&_current);
			int count=(int)((double)( _current.QuadPart - _last.QuadPart)/_frequency.QuadPart * 1000 )/2;
			for(int i = 0; i< _pCavase->GetWaveCount(); i++)
			{
				_pCavase->GetWave(i)->GetWaveBuffer()->setLength(_pCavase->GetWave(i)->GetWaveBuffer()->getLength() + count);
			}
		}
		QueryPerformanceCounter(&_last);
		Invalidate(TRUE);
	}
}

BOOL CMainBaseCtrl::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CMainBaseCtrl::stop()
{
	KillTimer(SAMPLE_SHOW_TIMER_ID);
}

void CMainBaseCtrl::start()
{
	SetTimer(SAMPLE_SHOW_TIMER_ID, SAMPLE_SHOW_ELAPSE, NULL);
}
