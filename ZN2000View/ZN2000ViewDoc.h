
// ZN2000ViewDoc.h : CZN2000ViewDoc ��Ľӿ�
//


#pragma once
#include "../libzn/ZNDData.h"
#include "../libzn/RheographyRecord.h"

class CZN2000ViewDoc : public CDocument
{
	RheographyRecordImpl _record;
protected: // �������л�����
	CZN2000ViewDoc();
	DECLARE_DYNCREATE(CZN2000ViewDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CZN2000ViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
