#pragma once
#include <GdiPlus.h>
#include "DrawObject.h"
#include "RulerObject.h"
#include "ImageObject.h"
#include "ObjectsContainer.h"
#include "AspectRatioImageObject.h"
#define IMAGE_MODE_EDIT    1
#define IMAGE_MODE_COMPARE 2


class CImageContainer: public CObjectsContainer
{
protected:
	CImageContainer();
	void Initialize();
	DECLARE_SERIAL(CImageContainer) 
public:
	CImageContainer(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options = WITH_HORIZ_RULER | WITH_VERT_RULER);
	CImageContainer(const CString & name, const Gdiplus::Rect & rect, DWORD options = WITH_HORIZ_RULER | WITH_VERT_RULER);
	virtual ~ CImageContainer();
	virtual void ResetContent();

	virtual CDrawObject *CopyFrom(CDrawObject * source);

	//////////////////////////////////////////////////////////////////////////
	//图像操作


	void SetImage(Gdiplus::Image * image);
	const Gdiplus::Image * GetImage() const;
	Gdiplus::Image * GetImage();
	virtual void OnImageChanging(Gdiplus::Image * image);
	virtual void OnImageChanged();

	virtual CDrawObject *GetObjectsAt(const Gdiplus::Point & point);

	CAspectRatioImageObject *GetImageObject();;

	bool LoadImage(const CString & fileName);

	void MoveImageTo(int x, int y);
	void OffsetImage(int dx, int dy);
	void ScaleImage(double scale);
	void RotateImage(double angle);
	void FlipImage(long direction);
	void SelectImage(bool isSelected);
	virtual void Serialize(CArchive & ar);

	virtual void Clear();
	virtual void SelectAll(CDrawObjectList & objList, int nFlag = SELECT_ADD);

	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);

    virtual bool IsEmpty();
protected:
	virtual void OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam);
	virtual void OnSizeChanged();

	virtual void OnTransparentChanged();
	//事件(覆盖)
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
private:

	//////////////////////////////////////////////////////////////////////////
	//绘制控制属性


	//////////////////////////////////////////////////////////////////////////
	//组件属性

	//图片
	CAspectRatioImageObject *_pImageObject;

};
