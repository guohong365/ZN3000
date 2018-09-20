#pragma once
#include <GdiPlus.h>
#include "DrawObject.h"
#include "RulerObject.h"

#define HORIZ_RULER 0
#define VERT_RULER  1

//����ͼƬ���Ʒ�ʽ
#define	ImageStretched	0       ///���ŵ������С
#define	ImageCentered	1       ///ԭʼ��С������λ�ö���
#define	ImagedTiled		2       ///ԭʼ��С��ƽ����������ռ�

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

	//�¼�(����)
	virtual void OnSizeChanged();
	virtual void OnDraw(Gdiplus::Graphics & graph);

private:
	double m_RulerUnitPerPixel;

	//////////////////////////////////////////////////////////////////////////
	//���ƿ�������

	//����ͼƬ���Ʒ�ʽ
	int m_ImageShowMode;

	//����ѡ�1��2λ�ֱ���Ʊ��ˮƽ����ֱ����Ƿ�ʹ��
	DWORD m_Options;

	//////////////////////////////////////////////////////////////////////////
	//�������

	//����ͼƬ
	Gdiplus::Image * m_pBackgoundImage;
	//��ֱ���
	CRulerObject *m_pVerticalRuler;
	//ˮƽ���
	CRulerObject *m_pHorizalRuler;
};
