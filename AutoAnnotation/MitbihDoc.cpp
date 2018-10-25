// MitbihDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoAnnotation.h"
#include "MitbihDoc.h"


// CMitbihDoc

IMPLEMENT_DYNCREATE(CMitbihDoc, CDocument)

CMitbihDoc::CMitbihDoc(): _pRecord(nullptr), _pReader(nullptr)
{
}

BOOL CMitbihDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMitbihDoc::~CMitbihDoc()
{
    delete _pRecord;
    delete _pReader;
}


BEGIN_MESSAGE_MAP(CMitbihDoc, CDocument)
END_MESSAGE_MAP()


// CMitbihDoc 诊断

#ifdef _DEBUG
void CMitbihDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CMitbihDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CMitbihDoc 序列化

void CMitbihDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif


// CMitbihDoc 命令


BOOL CMitbihDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    _pRecord = _pReader->load(lpszPathName);

    return _pRecord!=nullptr;
}
