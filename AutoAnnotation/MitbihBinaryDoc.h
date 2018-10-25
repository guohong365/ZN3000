#pragma once
#include "MitbihDoc.h"

// CMitbihBinaryDoc �ĵ�

class CMitbihBinaryDoc : public CMitbihDoc
{
	DECLARE_DYNCREATE(CMitbihBinaryDoc)

public:
	CMitbihBinaryDoc();
	virtual ~CMitbihBinaryDoc();
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
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
