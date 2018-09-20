#pragma once
#include <GdiPlus.h>
#include "RectObject.h"
#include "PolyLineObject.h"
#define RULER_OFFSET 8

#define WITH_HORIZ_RULER 0x00000001
#define WITH_VERT_RULER  0x00000002

#define SHOW_HORIZ_RULER_MASK 0x000000010
#define SHOW_VERT_RULER_MASK  0x000000020

#define HORIZ_RULER_HEIGHT 8

//////////////////////////////////////////////////////////////////////////
//对齐方式
#define AlignNone        1      ///不对齐（自由对齐）
#define AlignLeftTop     2      ///左对齐（垂直方向）、上对齐（水平方向）
#define AlignRightBottom 3      ///右对齐（垂直方向）、下对齐（水平方向）
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//标尺单位
#define UnitNone              -1        ///无单位
#define RulerUnitMillimeter   0 ///毫米
#define UnitCentiMeter        1 ///厘米
#define RulerUnitDecimeter    2 ///分米
#define RulerUnitMeter        3 ///米
#define RulerUnitKilometre    4 ///千米
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//刻度线标记位置。
#define BaselineAtTop      1    ///水平位置时，刻度线画在标尺基线上方。垂直位置时，刻度线画在标尺线右侧
#define BaselineAtBottom   2    ///水平位置时，刻度线画在标尺基线下方。垂直位置时，刻度线画在标尺线左侧
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//刻度数字增长方向
#define IncreaseNormally   1    ///正常方向，水平位置从左到右，垂直位置从上到下
#define IncreaseReverse    2    ///反方向，水平位置从右到左，垂直位置从下到上
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//数字标注位置
#define NumAtLeft      1        //数字标注在刻度线左侧。标尺为方向时在刻度线上方
#define NumAtCenter    2        //数字标注在刻度线正下方。
#define NumAtRight     3        //数字标注在刻度线右侧。标尺为方向时在刻度线下方
//////////////////////////////////////////////////////////////////////////

class CRulerObject:public CRectObject
{
protected:
	CRulerObject();
	void Initialize();
	DECLARE_SERIAL(CRulerObject) 
public:
	CRulerObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);
	CRulerObject(const CString & name, const Gdiplus::Rect & rect);
	CRulerObject(const CString & name, int x, int y, int width, int height);

	//取英文单位名
	static const TCHAR *GetUnitEnName(long unit);
	//取中文单位名
	static const TCHAR *GetUnitZhName(long unit);
	//从英文名称取单位代码
	static long GetUnitByEnName(const TCHAR * unitName);
	//从中文名称取单位代码
	static long GetUnitByZhName(const TCHAR * unitName);


	//////////////////////////////////////////////////////////////////////////
	//几何参数

	//当前单位，mm,cm等
	void SetCurrentUnit(long unit);
	long GetCurrentUnit() const;
	virtual void OnCurrentUnitChanging(long &newUnit);
	virtual void OnCurrentUnitCHanged();

	//每像素的长度
	void SetUnitPerPixel(double unitPerPixel);
	double GetUnitPerPixel() const;
	virtual void OnUnitPerPixelChanging(double &newUnitPerPixel);
	virtual void OnUnitPerPixelChanged();

	//标尺对齐方式
	void SetAlignment(long align);
	long GetAlignment() const;
	virtual void OnAlignmentChanging(long &align);
	virtual void OnAlignmentChanged();

	//////////////////////////////////////////////////////////////////////////
	//刻度线参数

	//刻度线最小像素间隔
	void SetMinGraduationPixelInteral(int interval);
	int GetMinGraduationPixelInterval() const;
	virtual void OnMinGraduationPixelIntervalChanging(int &newInterval);
	virtual void OnMinGraduationPixelIntervalChanged();
	//刻度线单位数字倍数因子。刻度线必须间隔整数倍该因子倍数。
	void SetGraduationFactor(int factor);
	int GetGraduationFactor() const;
	virtual void OnGraduationFactorChanging(int &newFactor);
	virtual void OnGraduationFactorChanged();
	//刻度线相对标尺基线的位置
	void SetGraduationPosition(long position);
	long GetGraduationPosition() const;
	virtual void OnGraduationPositionChanging(long &newPos);
	virtual void OnGraduationPositionChanged();
	//长刻度线间隔条数
	void SetLongGraduationInterval(int interval);
	int GetLongGraduationInterval() const;
	virtual void OnLongGraduationIntervalChanging(int &interval);
	virtual void OnLongGraduationIntervalChanged();

	//////////////////////////////////////////////////////////////////////////
	//刻度线

	//是否显示刻度数字
	void SetShowNumber(bool isShowNumber);
	bool GetShowNumber() const;
	virtual void OnShowNumberChanging(bool & isShowNumber);
	virtual void OnShowNumberChanged() ;
	//刻度数字增长方向
	void SetIncreaseDirection(long direction);
	long GetIncreaseDirection() const;
	virtual void OnIncreaseDirectionChanging(long &direction);
	virtual void OnIncreaseDirectionChanged();
	//刻度数字间最小像素间隔
	void SetMinNumberPixelInterval(int interval);
	int GetMinNumberPixelInterval() const;
	virtual void MinNumberPixelIntervalChanging(int &interval);
	virtual void MinNumberPixelIntervalChanged();
	//刻度数字标记位置（相对待标记刻度线）
	void SetNumberPosition(long position);
	long GetNumberPosition() const;
	virtual void OnNumberPositionChanging(long &pos);
	virtual void OnNumberPositionChanged();
	//刻度数字倍数因子，刻度标记数字为该因子整数倍
	void SetNumberFactor(int factor);
	int GetNumberFactor() const;
	virtual void OnNumberFactorChanging(int &factor);
	virtual void OnNumberFactorChanged();

	//////////////////////////////////////////////////////////////////////////
	//标尺单位

	//是否显示刻度单位
	void SetShowUnitName(bool isShowUnitName);
	bool GetShowUnitName() const;
	virtual void OnShowUnitNameChanging(bool & isShowUnitName);
	virtual void OnShowUnitNameChanged();

	//////////////////////////////////////////////////////////////////////////
	//刻度线间隔控制参数

	//刻度线间隔像素
	double GetGraduationPixelInterval() const;
	//刻度线间隔长度
	double GetGraduationUnitInterval() const;

	//////////////////////////////////////////////////////////////////////////
	//刻度数字标记控制参数

	//数字标记间隔像素
	int GetNumberPixelInterval() const;
	//数字标记间隔长度
	int GetNumberUnitInterval() const;

	//////////////////////////////////////////////////////////////////////////
	//复制函数
	virtual CDrawObject *CopyFrom(CDrawObject * source);

	//////////////////////////////////////////////////////////////////////////
	//绘图函数
	virtual void OnDraw(Gdiplus::Graphics & graph);

	//绘制基线
	virtual void DrawBaseLine(Gdiplus::Graphics & graph);
	//绘制刻度线及数字标记
	virtual void DrawGraduation(Gdiplus::Graphics & graph);
	//绘制单位名称
	virtual void DrawUnitName(Gdiplus::Graphics & graph);

	//      virtual int HitTest(const Gdiplus::Point & pt);

	virtual void Serialize(CArchive & ar);

	virtual void OnSizeChanging(Gdiplus::Size & newSize);
protected:
private:
	//计算刻度线间隔：长度间隔和像素间隔
	void CalculateGraduatoinStep();

	Gdiplus::Size GetTextExtent(Gdiplus::Graphics & graph, int fontHeight, const CString & text);

	//////////////////////////////////////////////////////////////////////////
	//绘制垂直标尺刻度线及数字标记
	void DrawVertGraduation(Gdiplus::Graphics & graph, int unitInterval, int pixelInterval);
	//绘制水平标尺刻度线及数字标记
	void DrawHorzGraduation(Gdiplus::Graphics & graph, int unitInterval, int pixelInterval);
	//绘制自由标尺刻度线及数字标记
	void DrawFreeGraduation(Gdiplus::Graphics & graph, int unitInterval, int pixelInterval);

	//////////////////////////////////////////////////////////////////////////
	//绘制垂直标尺单位名称
	void DrawVertUnitName(Gdiplus::Graphics & graph);
	//绘制水平标尺单位名称
	void DrawHorzUnitName(Gdiplus::Graphics & graph);
	//绘制自由标尺单位名称
	void DrawFreeUnitName(Gdiplus::Graphics & graph);


	//////////////////////////////////////////////////////////////////////////
	//几何参数

	//当前单位，mm,cm等
	long m_CurrentUnit;
	//每像素的长度
	double m_UnitPerPixel;

	//标尺对齐方式
	long m_Align;

	//////////////////////////////////////////////////////////////////////////
	//刻度线参数

	//刻度线最小像素间隔
	int m_MinGraduationPixelInterval;
	//刻度线单位数字倍数因子。刻度线必须间隔整数倍该因子倍数。
	int m_GraduationFactor;
	//标尺基线的位置
	long m_GraduationPosition;
	//长刻度线间隔条数
	int m_LongGraduationInterval;

	//////////////////////////////////////////////////////////////////////////
	//刻度线

	//是否显示刻度数字
	bool m_bShowNumber;
	//刻度数字增长方向
	long m_IncreaseDirection;
	//刻度数字间最小像素间隔
	int m_MinNumberPixelInterval;
	//刻度数字标记位置（相对待标记刻度线）
	long m_NumberPosition;
	//刻度数字倍数因子，刻度标记数字为该因子整数倍
	int m_NumberFactor;

	//////////////////////////////////////////////////////////////////////////
	//标尺单位

	//是否显示刻度单位
	bool m_bShowUnitName;

	//////////////////////////////////////////////////////////////////////////
	//刻度线间隔控制参数

	//刻度线间隔像素
	double m_GraduationPixelInterval;
	//刻度线间隔长度
	double m_GraduationUnitInterval;

	//////////////////////////////////////////////////////////////////////////
	//刻度数字标记控制参数

	//数字标记间隔像素
	int m_NumberPixelInterval;
	//数字标记间隔长度
	int m_NumberUnitInterval;

};

