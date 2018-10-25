// MitbihDocument.cpp : ʵ���ļ�
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


// CMitbihDoc ���

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
// CMitbihDoc ���л�

void CMitbihDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
#endif


// CMitbihDoc ����


BOOL CMitbihDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    _pRecord = _pReader->load(lpszPathName);

    return _pRecord!=nullptr;
}
