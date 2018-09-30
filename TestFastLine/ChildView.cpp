
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "TestFastLine.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
IMPLEMENT_DYNAMIC(CChildView, CFormView)

CChildView::CChildView(const UINT nID)
:CFormView(nID)
{
	_brush.CreateSolidBrush(RGB(227, 238, 255));
}

CChildView::~CChildView()
{
}

void CChildView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	CXTPWindowRect rc(m_wndChartControl);
	ScreenToClient(rc);
	m_nTopGap=rc.top;
	m_nLeftGap=rc.left;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
}

