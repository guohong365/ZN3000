
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RichTextRender.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const IID IID_XTPITextServices =
{
	0x8d33f740, 0xcf58, 0x11ce, {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

const IID IID_XTPITextHost =
{
	0xc5bdd8d0, 0xd26e, 0x11ce, {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

CRichTextRender::CRichTextRender()
{
	_exten.cx=_exten.cy=-1;
	NONCLIENTMETRICS ncm;
	::ZeroMemory(&ncm, sizeof(ncm));
	ncm.cbSize = sizeof(ncm);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);

	HDC hDC = ::GetDC(NULL);

	::ZeroMemory(&m_charformat, sizeof(m_charformat));
	m_charformat.cbSize = sizeof(m_charformat);
	m_charformat.dwMask = CFM_BOLD | CFM_CHARSET | CFM_COLOR  |CFM_FACE | CFM_ITALIC |
		CFM_SIZE | CFM_STRIKEOUT | CFM_UNDERLINE;
	m_charformat.yHeight = -MulDiv(ncm.lfMessageFont.lfHeight, 1440, ::GetDeviceCaps(hDC, LOGPIXELSY));
	m_charformat.crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	m_charformat.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	WCSNCPY_S(m_charformat.szFaceName, LF_FACESIZE, XTP_CT2CW(ncm.lfMessageFont.lfFaceName), LF_FACESIZE);

	::ReleaseDC(NULL,hDC);

	::ZeroMemory(&m_paraformat, sizeof(m_paraformat));
	m_paraformat.cbSize = sizeof(m_paraformat);
	m_paraformat.dwMask = PFM_ALIGNMENT | PFM_NUMBERING | PFM_OFFSET | PFM_OFFSETINDENT |
		PFM_RIGHTINDENT | PFM_RTLPARA | PFM_STARTINDENT | PFM_TABSTOPS;
	m_paraformat.wAlignment = PFA_LEFT;

	m_pTextService = NULL;

	m_hModule = LoadLibrary(_T("msftedit.dll"));
	if (!m_hModule)
		return;
	TCHAR filename[MAX_PATH];
	GetModuleFileName(m_hModule, filename, MAX_PATH);

	// Get an interface to the windowless rich edit control
	IUnknown* pUnknown = NULL;
	PCreateTextServices lpCreateTextServices = (PCreateTextServices)GetProcAddress(m_hModule, "CreateTextServices");
	HRESULT hr = lpCreateTextServices(NULL, &m_xTextHost, &pUnknown);

	if (SUCCEEDED(hr))
	{
		pUnknown->QueryInterface(IID_XTPITextServices, (void**)&m_pTextService);
		pUnknown->Release();
	}
}

CRichTextRender::~CRichTextRender()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
	}
}

void CRichTextRender::SetDefaultCharFormat(CHARFORMATW* pcf)
{
	if (!pcf)
		return;

	m_charformat = *pcf;

	if (m_pTextService)
	{
		LRESULT lResult = 0;
		m_pTextService->TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)pcf, &lResult);
	}
}

DWORD CALLBACK CRichTextRender::RichTextCtrlCallbackIn(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	CFile* pFile = (CFile*) dwCookie;
	*pcb = pFile->Read(pbBuff, cb);
	return 0;
}

LRESULT CRichTextRender::SetText(LPCTSTR lpszText)
{
	if (!m_pTextService)
		return 0;

	if (lpszText == 0 || lpszText[0] == 0)
	{
		m_pTextService->TxSetText(L"");
		return 0;
	}

	LRESULT nResult = 0;

	EDITSTREAM es;
	es.pfnCallback = &CRichTextRender::RichTextCtrlCallbackIn;

	if (lpszText[0] == _T('{'))
	{
	#ifdef _UNICODE
		CXTPResourceManager::CXTPW2A _lpa(lpszText);
		LPSTR lpBuffer = (LPSTR)(LPCSTR)_lpa;
	#else
		LPSTR lpBuffer = (LPSTR)lpszText;
	#endif

		CMemFile cFile((BYTE*)lpBuffer, (UINT)strlen(lpBuffer));
		es.dwCookie = (DWORD_PTR)&cFile;

		m_pTextService->TxSendMessage(EM_STREAMIN, SF_RTF, (LPARAM)&es, &nResult);
		if (nResult > 0)
		{
			return nResult;
		}
	}

	CMemFile cFile((BYTE*)lpszText, (UINT)_tcslen(lpszText) * sizeof(TCHAR));
	es.dwCookie = (DWORD_PTR)&cFile;

	m_pTextService->TxSendMessage(EM_STREAMIN, SF_TEXT | (sizeof(TCHAR) > 1 ? SF_UNICODE : 0), (LPARAM)&es, &nResult);

	return nResult;
}


CSize CRichTextRender::GetTextExtent(CDC* pDC, int nMaxWidth)
{
	if (!m_pTextService)
		return CSize(0);

	LRESULT lResult = 0;

	m_pTextService->TxSendMessage(EM_SETEVENTMASK, 0, ENM_REQUESTRESIZE, &lResult);
	m_sizeEditMin.cx = m_sizeEditMin.cy = 0;

	// Performing the binary search for the best dimension
	int cxFirst = 0;
	int cxLast = nMaxWidth;
	int cyMin = 0;

	cxLast *= 2;    // cos the first thing we do it divide it by two
	RECT rcUpdate = {0, 0, 0, 0};
	do
	{
		// Taking a guess
		int cx = (cxFirst + cxLast) / 2;

		// Testing this guess
		RECTL rc = {0, 0, cx, 1};

		m_pTextService->TxDraw(DVASPECT_CONTENT, 0, NULL, NULL, pDC->GetSafeHdc(), 0,
			&rc, NULL, &rcUpdate, NULL, 0, 0);

		// If it's the first time, take the result anyway.
		// This is the minimum height the control needs
		if (cyMin == 0)
			cyMin = m_sizeEditMin.cy;

		// Iterating
		if (m_sizeEditMin.cy > cyMin)
		{
			// If the control required a larger height, then
			// it's too narrow.
			cxFirst = cx + 1;
		}
		else
		{
			// If the control didn't required a larger height,
			// then it's too wide.
			cxLast = cx - 1;
		}
	}
	while (cxFirst < cxLast);

	if (m_sizeEditMin.cy > cyMin)
	{
		RECTL rc = {0, 0, cxLast + 1, 1};
		m_pTextService->TxDraw(DVASPECT_CONTENT, 0, NULL, NULL, pDC->GetSafeHdc(), 0,
			&rc, NULL, &rcUpdate, NULL, 0, 0);
	}

	return m_sizeEditMin;
}

void CRichTextRender::DrawText(HDC hdc, LPCRECT lpRect)
{
	if (!m_pTextService)
		return;

	HRESULT hr=m_pTextService->TxDraw(DVASPECT_CONTENT, 0, NULL, NULL, hdc, 0,
		(LPCRECTL)lpRect, NULL, NULL, NULL, 0, 0);
}

BEGIN_INTERFACE_MAP(CRichTextRender, CCmdTarget)
	INTERFACE_PART(CRichTextRender, IID_XTPITextHost, TextHost)
END_INTERFACE_MAP()

void CRichTextRender::SetExten( int width, int height )
{
	_exten.cx=width;
	_exten.cy=height;
}

STDMETHODIMP_(ULONG) CRichTextRender::XTextHost::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) CRichTextRender::XTextHost::Release()
{
	return 1;
}

STDMETHODIMP CRichTextRender::XTextHost::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CRichTextRender, TextHost)
	return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

HDC CRichTextRender::XTextHost::TxGetDC()
{
	return 0;
}

INT CRichTextRender::XTextHost::TxReleaseDC(HDC /*hdc*/)
{
	return 0;
}

BOOL CRichTextRender::XTextHost::TxShowScrollBar(INT /*fnBar*/, BOOL /*fShow*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxEnableScrollBar(INT /*fuSBFlags*/, INT /*fuArrowflags*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxSetScrollRange(INT /*fnBar*/, LONG /*nMinPos*/, INT /*nMaxPos*/, BOOL /*fRedraw*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxSetScrollPos(INT /*fnBar*/, INT /*nPos*/, BOOL /*fRedraw*/)
{
	return FALSE;
}

void CRichTextRender::XTextHost::TxInvalidateRect(LPCRECT /*prc*/, BOOL /*fMode*/)
{
}

void CRichTextRender::XTextHost::TxViewChange(BOOL /*fUpdate*/)
{
}

BOOL CRichTextRender::XTextHost::TxCreateCaret(HBITMAP /*hbmp*/, INT /*xWidth*/, INT /*yHeight*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxShowCaret(BOOL /*fShow*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxSetCaretPos(INT /*x*/, INT /*y*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxSetTimer(UINT /*idTimer*/, UINT /*uTimeout*/)
{
	return FALSE;
}

void CRichTextRender::XTextHost::TxKillTimer(UINT /*idTimer*/)
{
}

void CRichTextRender::XTextHost::TxScrollWindowEx(INT /*dx*/, INT /*dy*/, LPCRECT /*lprcScroll*/,
	LPCRECT /*lprcClip*/, HRGN /*hrgnUpdate*/, LPRECT /*lprcUpdate*/, UINT /*fuScroll*/)
{
}

void CRichTextRender::XTextHost::TxSetCapture(BOOL /*fCapture*/)
{
}

void CRichTextRender::XTextHost::TxSetFocus()
{
}

void CRichTextRender::XTextHost::TxSetCursor(HCURSOR /*hcur*/, BOOL /*fText*/)
{
}

BOOL CRichTextRender::XTextHost::TxScreenToClient(LPPOINT /*lppt*/)
{
	return FALSE;
}

BOOL CRichTextRender::XTextHost::TxClientToScreen(LPPOINT /*lppt*/)
{
	return FALSE;
}

HRESULT CRichTextRender::XTextHost::TxActivate(LONG* /*plOldState*/)
{
	return E_FAIL;
}

HRESULT CRichTextRender::XTextHost::TxDeactivate(LONG /*lNewState*/)
{
	return E_FAIL;
}

HRESULT CRichTextRender::XTextHost::TxGetClientRect(LPRECT /*prc*/)
{
	return E_FAIL;
}

HRESULT CRichTextRender::XTextHost::TxGetViewInset(LPRECT prc)
{
	*prc = CRect(0,0,0,0);
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
	METHOD_PROLOGUE(CRichTextRender, TextHost)

	*ppCF = &(pThis->m_charformat);
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
	METHOD_PROLOGUE(CRichTextRender, TextHost)

	*ppPF = &(pThis->m_paraformat);
	return S_OK;
}

COLORREF CRichTextRender::XTextHost::TxGetSysColor(int nIndex)
{
	return ::GetSysColor(nIndex);
}

HRESULT CRichTextRender::XTextHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
	*pstyle = TXTBACK_TRANSPARENT;
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxGetMaxLength(DWORD *plength)
{
	*plength = 1024*1024*16;
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
	*pdwScrollBar = 0;
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxGetPasswordChar(TCHAR* /*pch*/)
{
	return S_FALSE;
}

HRESULT CRichTextRender::XTextHost::TxGetAcceleratorPos(LONG *pcp)
{
	*pcp = -1;
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxGetExtent(LPSIZEL lpExtent)
{
	METHOD_PROLOGUE(CRichTextRender, TextHost)
	if(pThis->_exten.cx!=-1)
	{
		*lpExtent=pThis->_exten;
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT CRichTextRender::XTextHost::OnTxCharFormatChange(const CHARFORMATW * /*pcf*/)
{
	return E_FAIL;
}

HRESULT CRichTextRender::XTextHost::OnTxParaFormatChange(const PARAFORMAT * /*ppf*/)
{
	return E_FAIL;
}

HRESULT CRichTextRender::XTextHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits)
{
	DWORD bits = TXTBIT_MULTILINE | TXTBIT_RICHTEXT | TXTBIT_WORDWRAP;
	*pdwBits = bits & dwMask;
	return S_OK;
}

HRESULT CRichTextRender::XTextHost::TxNotify(DWORD iNotify, void* pv)
{
	METHOD_PROLOGUE(CRichTextRender, TextHost)
	if (iNotify == EN_REQUESTRESIZE)
	{
		REQRESIZE* prr = (REQRESIZE*)pv;
		pThis->m_sizeEditMin.cx = prr->rc.right - prr->rc.left;
		pThis->m_sizeEditMin.cy = prr->rc.bottom - prr->rc.top;
	}

	return S_OK;
}

XTP_HIMC CRichTextRender::XTextHost::TxImmGetContext()
{
	return 0;
}

void CRichTextRender::XTextHost::TxImmReleaseContext(XTP_HIMC /*himc*/)
{
}

HRESULT CRichTextRender::XTextHost::TxGetSelectionBarWidth(LONG* lSelBarWidth)
{
	*lSelBarWidth = 0;
	return S_OK;
}
