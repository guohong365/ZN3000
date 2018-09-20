// WaveCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include <zn\WaveCtrl.h>
#include <DrawObject\UIHelper.h>

// CWaveCtrl 对话框

IMPLEMENT_DYNAMIC(CWaveCtrl, CDialog)

CWaveCtrl::CWaveCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveCtrl::IDD, pParent)
	,_pCanvas(NULL)
{

}

CWaveCtrl::~CWaveCtrl()
{
}

void CWaveCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaveCtrl, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CWaveCtrl 消息处理程序

void CWaveCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.SetMapMode(MM_ANISOTROPIC);
	CSize sz(1000, 1000);
	dc.SetWindowExt(sz);
	CUICoordinateHelper::GetHelper().LPtoDP(&sz, 1);
	dc.SetViewportExt(sz);

	Gdiplus::Graphics graph(dc.GetSafeHdc());

	_pCanvas->Draw(graph);
}

void CWaveCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CSize sz(cx, cy);
	CUICoordinateHelper::GetHelper().DPtoLP(&sz, 1);
	if(_pCanvas==NULL)
	{
		_pCanvas=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(sz.cx, sz.cy));
	}
	else
	{
		_pCanvas->SetSize(Gdiplus::Size(sz.cx, sz.cy));
	}
	_pCanvas->PrepareCanvas(cx, cy);
}

BOOL CWaveCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CWaveCtrl::OnOK()
{
}

void CWaveCtrl::OnCancel()
{
}
