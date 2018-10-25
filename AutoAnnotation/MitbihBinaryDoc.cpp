// MitbihBinaryDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoAnnotation.h"
#include "MitbihBinaryDoc.h"


// CMitbihBinaryDoc

IMPLEMENT_DYNCREATE(CMitbihBinaryDoc, CMitbihDoc)

CMitbihBinaryDoc::CMitbihBinaryDoc()
{
    _pReader = new MitbihBinaryReader();
}

BOOL CMitbihBinaryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMitbihBinaryDoc::~CMitbihBinaryDoc()
{
}


BEGIN_MESSAGE_MAP(CMitbihBinaryDoc, CMitbihDoc)
END_MESSAGE_MAP()


// CMitbihBinaryDoc 诊断

#ifdef _DEBUG
void CMitbihBinaryDoc::AssertValid() const
{
	CMitbihDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CMitbihBinaryDoc::Dump(CDumpContext& dc) const
{
	CMitbihDoc::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CMitbihBinaryDoc 序列化

void CMitbihBinaryDoc::Serialize(CArchive& ar)
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
