
// TestViewDoc.h : CTestViewDoc ��Ľӿ�
//


#pragma once

class WaveCanvas;

class CTestViewDoc : public CDocument
{
	WaveCanvas *_pCanvas;
	GridBackground *_pBackground;
protected: // �������л�����
	CTestViewDoc();
	DECLARE_DYNCREATE(CTestViewDoc)

// ����
public:
	WaveCanvas * GetCanvas() const
	{
		return _pCanvas;
	}
	GridBackground *GetBackground() const
	{
		return _pBackground;
	}
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
	virtual ~CTestViewDoc();
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
