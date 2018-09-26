
// TestViewDoc.cpp : CTestViewDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CTestViewDoc 构造/析构

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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTestViewDoc 序列化
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
		SignalChannel* pBuffer[4];
		pBuffer[0]=new SignalChannelImpl(_T("Feedback"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[1]=new SignalChannelImpl(_T("Admittance"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[2]=new SignalChannelImpl(_T("Differential"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		pBuffer[3]=new SignalChannelImpl(_T("ECG"), 1000, 0, 10, _T("V"),LEN_BUFFER);
		int index=0;
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
			index=i;
			do
			{
				pData=reinterpret_cast<DataBuffer*>(p + index);
				revert(&pData->Paket);
				pBuffer[0]->getSignalBuffer().append(pData->Paket.Feedback);
				pBuffer[1]->getSignalBuffer().append(pData->Paket.Admittance);
				pBuffer[2]->getSignalBuffer().append(pData->Paket.Differential);
				pBuffer[3]->getSignalBuffer().append(pData->Paket.ECG);
				//pBuffer[0]->getSignalBuffer().append(pData->Paket.Feedback);
				//pBuffer[1]->getSignalBuffer().append(pData->Paket.Admittance);
				//pBuffer[2]->getSignalBuffer().append(pData->Paket.Differential);
				//pBuffer[3]->getSignalBuffer().append(pData->Paket.ECG);
				index +=sizeof(Packet);
			}while(index< (LEN_BUFFER -1) * sizeof(Packet));
			break;
		}
		_pCanvas=new WaveCanvas(Gdiplus::Point(0,0), Gdiplus::Size(0,0));
		//_pCanvas->AddWave(pBuffer[0], 25);
		_pCanvas->AddWave(pBuffer[1], 25);
		_pCanvas->AddWave(pBuffer[2], 25);
		//_pCanvas->AddWave(pBuffer[3], 25);
		delete []pTemp;
		FILE * fp;
		_tfopen_s(&fp, _T("out.txt"), _T("w"));
		for(int i=0; i< pBuffer[1]->getSignalBuffer().getLength(); ++i)
		{
			_ftprintf(fp, _T("%4.3f,"), pBuffer[1]->getSignalBuffer().getBuffer()[i]);
		}
		fclose(fp);
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CTestViewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CTestViewDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CTestViewDoc 诊断

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


// CTestViewDoc 命令
