
// TestViewDoc.cpp : CTestViewDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#endif
#include "../libzn/WaveCanvas.h"
#include "TestViewDoc.h"
#include "../libzn/Packet.h"
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
	: _pCanvas(nullptr)
{
	pBuffer[0]=nullptr;
	pBuffer[1]=nullptr;
	pBuffer[2]=nullptr;
	pBuffer[3]=nullptr;
	_pBackground = new GridBackground();
}

CTestViewDoc::~CTestViewDoc()
{
	delete pBuffer[0];
	delete pBuffer[1];
	delete pBuffer[2];
	delete pBuffer[3];
	delete _pBackground;
	delete _pCanvas;
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
void revert(Packet * packet)
{
	packet->FrameCount=RevertUInt16(packet->FrameCount);
	packet->Feedback=RevertUInt16(packet->Feedback);
	packet->Admittance=RevertUInt16(packet->Admittance);
	packet->Differential=RevertUInt16(packet->Differential);
	packet->ECG=RevertUInt16(packet->ECG);
}
void CTestViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{		
	}
	else
	{
		BYTE *pTemp=new BYTE[LEN_BUFFER*sizeof(Packet)];
		const UINT length=ar.Read(pTemp, LEN_BUFFER * sizeof(Packet));		
		delete pBuffer[0];
		delete pBuffer[1];
		delete pBuffer[2];
		delete pBuffer[3];
		pBuffer[0]=new SignalChannelImpl(_T("Feedback"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[1]=new SignalChannelImpl(_T("Admittance"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[2]=new SignalChannelImpl(_T("Differential"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[3]=new SignalChannelImpl(_T("ECG"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		BYTE * p=pTemp;
		for(int i=0; i< length - sizeof(Packet); i++)
		{
			if(p[i] != 0xCA || p[i + sizeof(Packet) -1]!=0xF1)
			{
				continue;
			}
			DataBuffer* pData = reinterpret_cast<DataBuffer*>(p + i);
			if(!checkPacket(pData))
			{
				continue;
			}
			int index = i;
			do
			{
				pData=reinterpret_cast<DataBuffer*>(p + index);
				revert(&pData->Paket);
				pBuffer[0]->getSignalBuffer().append(pData->Paket.Feedback*200.0f/32767);
				pBuffer[1]->getSignalBuffer().append(pData->Paket.Admittance*200.0f/32767);
				pBuffer[2]->getSignalBuffer().append(pData->Paket.Differential*200.0f/32767);
				pBuffer[3]->getSignalBuffer().append(pData->Paket.ECG*200.0f/32767);
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
