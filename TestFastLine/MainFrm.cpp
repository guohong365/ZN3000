
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "TestFastLine.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!static_cast<CView*>(&m_wndView)->Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}
	m_wndView.UpdateData(FALSE);
	if (!_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	if(!InitCommandBars())
		return -1;

	CXTPPaintManager::SetTheme(xtpThemeRibbon);
	CXTPCommandBars* pCommandBars=GetCommandBars();
	CXTPCommandBar* pMenuBar=pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
	pCommandBars->EnableFrameTheme(TRUE);
    


    //m_wndView.OnInitialUpdate();
    //RecalcLayout(TRUE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.lpszClass = _T("XTPMainFrame");
	
	HBRUSH hBackground = 0;
#ifndef _XTP_INCLUDE_TOOLKIT
	hBackground = GetSysColorBrush(COLOR_3DDKSHADOW);
#endif

	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME), hBackground);
	
	
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle |= WS_EX_CONTROLPARENT;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	
	cs.cx = 1000;
	cs.cy = 700;
    return TRUE;

}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	if(m_wndView.GetSafeHwnd())
	{
		m_wndView.SetFocus();
	}
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if(m_wndView.GetSafeHwnd())
	{
		if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CFrameWnd::OnSize(nType, cx, cy);    
}
    


void CMainFrame::RecalcLayout(BOOL bNotify)
{
    CXTPFrameWnd::RecalcLayout(bNotify);
}
