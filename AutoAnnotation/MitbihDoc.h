#pragma once
#include "../libMIT/MitbihRecord.h"

// CMitbihDoc �ĵ�

class CMitbihDoc : public CDocument
{
	DECLARE_DYNCREATE(CMitbihDoc)    
public:
	CMitbihDoc();
	virtual ~CMitbihDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
    MitbihRecord *_pRecord;
    MitbihReader *_pReader;
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
