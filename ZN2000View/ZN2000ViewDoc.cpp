
// ZN2000ViewDoc.cpp : CZN2000ViewDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ZN2000View.h"
#endif

#include "ZN2000ViewDoc.h"

#include <propkey.h>
#include "../libzn/ZNDLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CZN2000ViewDoc

IMPLEMENT_DYNCREATE(CZN2000ViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CZN2000ViewDoc, CDocument)
END_MESSAGE_MAP()


// CZN2000ViewDoc 构造/析构

CZN2000ViewDoc::CZN2000ViewDoc()
{
	// TODO: 在此添加一次性构造代码

}

CZN2000ViewDoc::~CZN2000ViewDoc()
{
}

BOOL CZN2000ViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CZN2000ViewDoc 序列化

void CZN2000ViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CZN2000ViewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CZN2000ViewDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CZN2000ViewDoc::SetSearchContent(const CString& value)
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

// CZN2000ViewDoc 诊断

#ifdef _DEBUG
void CZN2000ViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZN2000ViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CZN2000ViewDoc 命令
