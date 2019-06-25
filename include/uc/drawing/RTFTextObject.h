#pragma once
#include "Rectobject.h"
#include "RichTextRender.h"

class CRTFTextObject :
	public CRectObject
{
	DECLARE_SERIAL(CRTFTextObject)
public:
	CRTFTextObject(void);
	CRTFTextObject(ITextHost *pHost);
	virtual ~CRTFTextObject(void);
	virtual ITextHost *GetTextHost();

	virtual void SetTextHost(ITextHost *pHost);

	virtual void SetRichText(LPCTSTR text);
	virtual const CString &GetRichText();
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);
	virtual void OnDraw( Gdiplus::Graphics & graph );
	//复制对象
	//@param source 复制source对象当前数据到本对象，不复制撤销列表。
	virtual CDrawObject *CopyFrom(CDrawObject * source);	
	virtual void Serialize(CArchive & ar);

	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);

protected:

private:
	void Initialize();
	CHARFORMAT2 _charFormat;
	PARAFORMAT2 _paraFormat;
	ITextHost *_pTextHost;
	CString _rtfText;
	CRichTextRender *_pRichTextRender;
};
