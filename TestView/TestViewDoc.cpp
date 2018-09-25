
// TestViewDoc.cpp : CTestViewDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TestView.h"
#endif
#include "../libzn/WaveCanvas.h"
#include "TestViewDoc.h"

#include <propkey.h>
#include "../libzn/SignalBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestViewDoc

IMPLEMENT_DYNCREATE(CTestViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestViewDoc, CDocument)
END_MESSAGE_MAP()


// CTestViewDoc ����/����

CTestViewDoc::CTestViewDoc()
	:_pCanvas(nullptr)
{
	_pBackground=new GridBackground();	
}

CTestViewDoc::~CTestViewDoc()
{
	delete _pBackground;
}

BOOL CTestViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CTestViewDoc ���л�

void CTestViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{		
	}
	else
	{
		short temp[LEN_BUFFER]={};
		ar.Read(temp, LEN_BUFFER * sizeof(short));
		_pCanvas=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(0,0));
		SignalChannel* pBuffer[4];
		pBuffer[0]=new SignalChannelImpl(_T("a"), 1000, 0, 10, _T("V"),LEN_BUFFER/4);
		pBuffer[1]=new SignalChannelImpl(_T("b"), 1000, 0, 10, _T("V"),LEN_BUFFER/4);
		pBuffer[2]=new SignalChannelImpl(_T("c"), 1000, 0, 10, _T("V"),LEN_BUFFER/4);
		pBuffer[3]=new SignalChannelImpl(_T("d"), 1000, 0, 10, _T("V"),LEN_BUFFER/4);
		int index=0;
		for(int i=0; i< LEN_BUFFER; i++)
		{
			pBuffer[i%4]->getSignalBuffer().append(temp[i]);
			index += 4;
		}
		_pCanvas->AddWave(pBuffer[0], 25);
		_pCanvas->AddWave(pBuffer[1], 25);
		_pCanvas->AddWave(pBuffer[2], 25);
		_pCanvas->AddWave(pBuffer[3], 25);
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CTestViewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CTestViewDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CTestViewDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTestViewDoc ���

#ifdef _DEBUG
void CTestViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTestViewDoc ����
