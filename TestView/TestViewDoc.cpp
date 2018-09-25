
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
#include "../libzn/Packet.h"
#include <propkey.h>
#include "../libzn/SignalBuffer.h"
#include <libini.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LEN_BUFFER 10240
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
		BYTE *pTemp=new BYTE[LEN_BUFFER*sizeof(Packet)];
		ar.Read(pTemp, LEN_BUFFER * sizeof(Packet));		
		SignalChannel* pBuffer[4];
		pBuffer[0]=new SignalChannelImpl(_T("Feedback"), 100, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[1]=new SignalChannelImpl(_T("Admittance"), 100, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[2]=new SignalChannelImpl(_T("Differential"), 100, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[3]=new SignalChannelImpl(_T("ECG"), 100, 0, 10, _T("V"),LEN_BUFFER);
		int index=0;
		BYTE * p=pTemp;
		for(int i=0; i< (LEN_BUFFER -1) * sizeof(Packet); i++)
		{
			if(p[i] != 0xCA || p[sizeof(Packet) -1]!=0xF1)
			{
				continue;
			}
			DataBuffer* pData = reinterpret_cast<DataBuffer*>(p);
			if(!checkPacket(pData))
			{
				continue;
			}
			index=i;
			do
			{
				pData=reinterpret_cast<DataBuffer*>(p + index);
				pBuffer[0]->getSignalBuffer().append(RevertUInt16(pData->Paket.Feedback));
				pBuffer[1]->getSignalBuffer().append(RevertUInt16(pData->Paket.Admittance));
				pBuffer[2]->getSignalBuffer().append(RevertUInt16(pData->Paket.Differential));
				pBuffer[3]->getSignalBuffer().append(RevertUInt16(pData->Paket.ECG));
				index +=sizeof(Packet);
			}while(index< (LEN_BUFFER -1) * sizeof(Packet));
			break;
		}
		_pCanvas=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(0,0));
		_pCanvas->AddWave(pBuffer[0], 25);
		_pCanvas->AddWave(pBuffer[1], 25);
		_pCanvas->AddWave(pBuffer[2], 25);
		_pCanvas->AddWave(pBuffer[3], 25);
		delete []pTemp;
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
