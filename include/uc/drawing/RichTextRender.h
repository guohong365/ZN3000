/////////////////////////////////////////////////////////////////////////////

#pragma once
interface ITextRange;

class CRichTextRender : public CCmdTarget
{
public:
	CRichTextRender();
	~CRichTextRender();
	void SetExten(int width, int height);
	CSize GetTextExtent(CDC* pDC, int nMaxWidth);
	void DrawText(HDC hdc, LPCRECT lpRect);
	LRESULT SetText(LPCTSTR lpszText);
	void SetDefaultCharFormat(CHARFORMATW* pcf);

	ITextServices* GetTextService() const;

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(TextHost, ITextHost)
		HDC TxGetDC();
		INT TxReleaseDC(HDC hdc);
		BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
		BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags);
		BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
		BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw);
		void TxInvalidateRect(LPCRECT prc, BOOL fMode);
		void TxViewChange(BOOL fUpdate);
		BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
		BOOL TxShowCaret(BOOL fShow);
		BOOL TxSetCaretPos(INT x, INT y);
		BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
		void TxKillTimer(UINT idTimer);
		void TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip,
			HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
		void TxSetCapture(BOOL fCapture);
		void TxSetFocus();
		void TxSetCursor(HCURSOR hcur, BOOL fText);
		BOOL TxScreenToClient(LPPOINT lppt);
		BOOL TxClientToScreen(LPPOINT lppt);
		HRESULT TxActivate(LONG * plOldState);
		HRESULT TxDeactivate(LONG lNewState);
		HRESULT TxGetClientRect(LPRECT prc);
		HRESULT TxGetViewInset(LPRECT prc);
		HRESULT TxGetCharFormat(const CHARFORMATW **ppCF);
		HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
		COLORREF TxGetSysColor(int nIndex);
		HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
		HRESULT TxGetMaxLength(DWORD *plength);
		HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
		HRESULT TxGetPasswordChar(TCHAR *pch);
		HRESULT TxGetAcceleratorPos(LONG *pcp);
		HRESULT TxGetExtent(LPSIZEL lpExtent);
		HRESULT OnTxCharFormatChange(const CHARFORMATW * pcf);
		HRESULT OnTxParaFormatChange(const PARAFORMAT * ppf);
		HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
		HRESULT TxNotify(DWORD iNotify, void *pv);
		XTP_HIMC TxImmGetContext();
		void TxImmReleaseContext(XTP_HIMC himc);
		HRESULT TxGetSelectionBarWidth(LONG *lSelBarWidth);
	END_INTERFACE_PART(TextHost)

		static DWORD CALLBACK RichTextCtrlCallbackIn(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
	//}}AFX_CODEJOCK_PRIVATE
private:
	SIZEL _exten;
	ITextServices* m_pTextService;
	CHARFORMATW m_charformat;
	PARAFORMAT m_paraformat;
	HMODULE m_hModule;
	CSize m_sizeEditMin;
};

inline ITextServices* CRichTextRender::GetTextService() const
{
	return m_pTextService;
}
