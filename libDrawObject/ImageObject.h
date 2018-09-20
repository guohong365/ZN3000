#pragma once
#include <GdiPlus.h>
#include "DrawObject.h"
#include "RectObject.h"
#include "DrawObjectList.h"
#define CHANNEL_RED   1
#define CHANNEL_GREEN 2
#define CHANNEL_BLUE  4

#define FLIP_TYPE_HORIZNAL 1
#define FLIP_TYPE_VERTICAL 2


class CImageObject:public CRectObject
{
protected:
	CImageObject();
	void Initialize();
	DECLARE_SERIAL(CImageObject) public:
	CImageObject(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size);
	CImageObject(const CString & name, const Gdiplus::Rect & rect);
	virtual ~ CImageObject();

	void SetImage(Gdiplus::Image * image);
	const Gdiplus::Image * GetImage() const;
	Gdiplus::Image * GetImage();
	virtual void OnImageChanging(Gdiplus::Image * image);
	virtual void OnImageChanged();

	Gdiplus::Image * GetThumbnailImage();

	void SetColorMatrix(Gdiplus::ColorMatrix colorMatrix);
	Gdiplus::ColorMatrix GetColorMatrix() const;
	virtual void OnColorMatrixChanging(Gdiplus::ColorMatrix & colorMatrix);
	virtual void OnColorMatrixChanged();

	void SetShowGray(bool isShowGray);
	bool GetShowGray() const;
	virtual void OnShowGrayChanging(bool & isShowGray);
	virtual void OnShowGrayChanged();

	void SetShowInvert(bool isShowInvert);
	bool GetShowInvert() const;
	virtual void OnShowInvertChanging(bool & isShowInvert);
	virtual void OnShowInvertChanged();

	void SetChannel(unsigned char channel);
	unsigned char GetChannel() const;
	virtual void OnChannelChanging(unsigned char &channel);
	virtual void OnChannelChanged();

	void SetImageFileName(CString imageFileName);
	const CString GetImageFileName() const;
	virtual void OnImageFileNameChanging(CString & imageFileName);
	virtual void OnImageFileNameChanged();


	virtual CDrawObject *CopyFrom(CDrawObject * source);

	virtual int HitTest(const Gdiplus::Point & pt);

	//数据操作
	void Flip90Right();
	void Flip90Left();
	void FlipX();
	void FlipY();

	//装载图像文件
	virtual bool LoadImage(const TCHAR * pathName);
	virtual bool LoadImage(const TCHAR * fileName, IStorage * pStorage, bool encrypted=false);
	virtual bool LoadImage(IStream * pStream);

	virtual bool SaveImage(const TCHAR * pathName);
	virtual bool SaveImage(IStream * pStream);
	virtual bool SaveImage(const TCHAR * pathName, IStorage * pStorage, bool encrypt=false);

	virtual void Serialize(CArchive & ar);

	//事件(覆盖)
	virtual void OnDraw(Gdiplus::Graphics & graph);

	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);
    Gdiplus::Bitmap * DetachImage();

    virtual void notifyAttribtesChanged(CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal);
    virtual void notifyRevert(CDrawObject *pChanged, int &action );
    virtual void ResetContent()
    {
        __super::ResetContent();
        delete _pImage;
        delete _pThumbnailImage;
        _pImage=NULL;
        _pThumbnailImage=NULL;
    }

private:
	Gdiplus::ColorMatrix m_colorMatrix;
	bool m_bGray;
	bool m_bInvert;
	unsigned char m_Channel;

	Gdiplus::Image * _pImage;
	Gdiplus::Image * _pThumbnailImage;
	CString m_ImageFileName;

	CDrawObjectList m_ImageMask;

};

