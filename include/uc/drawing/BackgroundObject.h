#pragma once
#include <GdiPlus.h>
#include "DrawObject.h"
#include "RulerObject.h"

#define HORIZ_RULER 0
#define VERT_RULER  1

//背景图片绘制方式
#define	ImageStretched	0       ///缩放到对象大小
#define	ImageCentered	1       ///原始大小，中心位置对齐
#define	ImagedTiled		2       ///原始大小，平铺填满对象空间

class CBackgroundObject:public CRectObject
{
protected:
	CBackgroundObject();
	void Initialize();
	DECLARE_SERIAL(CBackgroundObject) public:
	CBackgroundObject(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options =
		WITH_HORIZ_RULER | WITH_VERT_RULER);
	CBackgroundObject(const CString & name, const Gdiplus::Rect & rect, DWORD options = WITH_HORIZ_RULER | WITH_VERT_RULER);
	virtual ~ CBackgroundObject();


	void SetImageShowMode(int mode);
	int GetImageShowMode() const;
	virtual void OnImageShowModeChanging(int &newMode);
	virtual void OnImageShowModeChanged();

	void SetImage(Gdiplus::Image * image);
	const Gdiplus::Image * GetImage() const;
	Gdiplus::Image * GetImage();
	virtual void OnImageChanging(Gdiplus::Image * image);
	virtual void OnImageChanged();

	void SetRulerScale(double unitPerPixel);
	double GetRulerScale() const;
	virtual void OnRulerScaleChanging(double &newUnitPerPixel);
	virtual void OnRulerScaleChanged();

	void SetShowRuler(int nRuler, bool isShow);
	bool GetShowRuler(int nRuler) const;
	virtual void OnShowRulerChanging(int nRuler, bool & newIsShow);
	virtual void OnShowRulerChanged(int nRuler);

	virtual void EnableRuler(int nRuler, bool isEnable);
	virtual bool IsRulerEnabled(int nRuler);

	virtual CDrawObject *CopyFrom(CDrawObject * source);

	virtual int HitTest(const Gdiplus::Point & pt);

	virtual void DrawRuler(Gdiplus::Graphics & graph);

	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);
	virtual void Serialize(CArchive & ar);

protected:

	//事件(覆盖)
	virtual void OnSizeChanged();
	virtual void OnDraw(Gdiplus::Graphics & graph);

private:
	double m_RulerUnitPerPixel;

	//////////////////////////////////////////////////////////////////////////
	//绘制控制属性

	//背景图片绘制方式
	int m_ImageShowMode;

	//对象选项，1，2位分别控制标迟水平，垂直标尺是否使用
	DWORD m_Options;

	//////////////////////////////////////////////////////////////////////////
	//组件属性

	//背景图片
	Gdiplus::Image * m_pBackgoundImage;
	//垂直标尺
	CRulerObject *m_pVerticalRuler;
	//水平标尺
	CRulerObject *m_pHorizalRuler;
};
