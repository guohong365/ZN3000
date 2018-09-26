#pragma once
#include "Visualized.h"
#include "Appearance.h"
#define FONT_SIZE_FACTOR   3.53f

struct FontInfo
{
    UINT mask;
	CString fontName; ///< 字体名称
	float fontSize;   ///<字体大小
	int fontStyle;    ///<字体风格，包括Bold、Italic和Underline
	Gdiplus::Color fontColor; ///<字体颜色
};

class DrawObject: public Visualized
{
	void _initialize();	
protected:
	DrawObject();
public:  
    //Visualized
    virtual Gdiplus::Image* GetPicture(int width, int height, Gdiplus::Color background = Gdiplus::Color::Transparent,
                               DWORD flag = GET_PIC_FLAG_ASPECT | GET_PIC_POS_HCENTER | GET_PIC_POS_VCENTER);

    virtual void ResetContent();

	explicit DrawObject(const CString & name);
	DrawObject(const CString & name, int x, int y, int width, int height);
	DrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);
	DrawObject(const CString & name, const Gdiplus::Rect & rect);
	virtual ~DrawObject();

	///生成唯一字符串
	///
	///用于生成对象内部名称
	///
	///@return 具有时空唯一性的字符串
	static CString GenerateUniqueName();
	
	//Attributes

	///
	///设置对象名称
	///
	///@param name 新名称
	///
	virtual void SetName(CString name);
	///
	///取对象名称
	///
	virtual CString  GetName();
	virtual void OnNameChanging(CString & newName);
	virtual void OnNameChanged();

	//////////////////////////////////////////////////////////////////////////

	//几何属性。，角度单位（度）

	//坐标单位
	static void SetUnit(Gdiplus::Unit unit);
	static Gdiplus::Unit GetUnit();

	///对象位置
	virtual void SetPosition(Gdiplus::Point point);
	virtual Gdiplus::Point GetPosition() const;
	virtual void OnPositionChanging(Gdiplus::Point & newPoint);
	virtual void OnPositionChanged();
	//对象大小
	virtual void SetSize(Gdiplus::Size size);
	virtual Gdiplus::Size GetSize() const;
	virtual void OnSizeChanging(Gdiplus::Size & newSize);
	virtual void OnSizeChanged();
	
	//////////////////////////////////////////////////////////////////////////

	//绘图属性
	virtual void SetAppearance(Appearance & appearance);
	virtual Appearance & GetAppearance();
	virtual Appearance & GetAppearance() const;
	//剪切边框
	virtual void SetClipRect(Gdiplus::Rect rect);
	virtual Gdiplus::Rect GetClipRect() const;
	virtual void OnClipRectChanging(Gdiplus::Rect & newRect);
	virtual void OnClipRectChanged();
	//是否剪切
	virtual void SetUsingClip(bool isUsingClip);
	virtual bool GetUsingClip() const;
	virtual void OnUsingClipChanging(bool & newUsingClip);
	virtual void OnUsingClipChanged();
	//是否显示
	virtual void SetVisible(bool isVisible);
	virtual bool GetVisible() const;
	virtual void OnVisibleChanging(bool & newVisible);
	virtual void OnVisibleChanged();
	//是否填充
	virtual void SetFilled(bool isFilled);
	virtual bool GetFilled() const;
	virtual void OnFilledChanging(bool & newFilled);
	virtual void OnFilledChanged();
	//是否绘制边框
	virtual void SetShowBorder(bool isShowBorder);
	virtual bool GetShowBorder() const;
	virtual void OnShowBorderChanging(bool & newShowBorder);
	virtual void OnShowBorderChanged();

	//透明度
	virtual void SetTransparent(FLOAT transparent);
	virtual FLOAT GetTransparent() const;
	virtual void OnTransparentChanging(FLOAT &newTransparent);
	virtual void OnTransparentChanged();

	//填充颜色
	virtual void SetFillColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetFillColor() const;
	virtual void OnFillColorChanging(Gdiplus::Color & newColor);
	virtual void OnFillColorChanged();
	//划线颜色
	virtual void SetLineColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetLineColor() const;
	virtual void OnLineColorChanging(Gdiplus::Color & newColor);
	virtual void OnLineColorChanged();

	//线宽
	virtual void SetLineWidth(FLOAT width);
	virtual FLOAT GetLineWidth();
	virtual void OnLineWidthChanging(FLOAT &width);
	virtual void OnLineWidthChanged();

	//线型
	virtual void SetLineStyle(INT style);
	virtual INT GetLineStyle();
	virtual void OnLineStyleChanging(INT & style);
	virtual void OnLineStyleChanged();

	///对象激活颜色
	virtual void SetActiveColor(Gdiplus::Color activeColor);
	virtual Gdiplus::Color GetActiveColor();
	virtual void OnActiveColorChanging(Gdiplus::Color &color);
	virtual void OnActiveColorChanged();

	//字体颜色
	virtual void SetFontColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetFontColor() const;
	virtual void OnFontColorChanging(Gdiplus::Color & newFontColor);
	virtual void OnFontColorChanged();

	//字体名称
	virtual void SetFontFace(const CString & fontFace);
	virtual CString GetFontFace();
	virtual void OnFontFaceChanging(CString & faceName);
	virtual void OnFontFaceChanged();
	
	//字体风格
	virtual void SetFontStyle(INT style);
	virtual INT GetFontStyle();
	virtual void OnFontStyleChanging(INT & style);
	virtual void OnFontStyleChanged();

	//字体大小
	virtual void SetFontSize(FLOAT fontSize);
	virtual FLOAT GetFontSize();
	virtual void OnFontSizeChanging(FLOAT &fontSize);
	virtual void OnFontSizeChanged();

	//文字水平对齐方式
	virtual void SetTextAlign(INT align);
	virtual INT GetTextAlign();
	virtual void OnTextAlignmentChanging(INT & align);
	virtual void OnTextAlignmentChanged();

	//文字垂直对齐方式
	virtual void SetTextLineAlign(INT align);
	virtual INT GetTextLineAlign();
	virtual void OnTextLineAlignChanging(INT & align);
	virtual void OnTextLineAlignChanged();
	
	//文字竖排
	virtual void SetTextIsVertical(bool isVertical);
	virtual bool GetTextIsVertical();
	virtual void OnTextVerticalChanging(bool & isVertical);
	virtual void OnTextVerticalChanged();

	//////////////////////////////////////////////////////////////////////////

	//控制属性

	//对象激活状态
	virtual void SetActive(bool isActive);
	virtual bool GetActive();
	virtual void OnActiveChanging(bool &isActive);
	virtual void OnActiveChanged();

	//////////////////////////////////////////////////////////////////////////

	//对象父对象
	virtual void SetParent(DrawObject * pParent);
	virtual const DrawObject *GetParent() const;
	virtual DrawObject *GetParent();
	virtual void OnParentChanging(DrawObject * & pObject);
	virtual void OnParentChanged();

	//method
	///
	///消息通知
	virtual void Notify(UINT msgId, DWORD_PTR wParam = 0, LPVOID lpParam = nullptr);
	///通知响应
	virtual void OnNotify(DrawObject * pSource, UINT messageId, DWORD_PTR wParam, LPVOID lpParam);

	//////////////////////////////////////////////////////////////////////////
	//撤销列表支持
	//////////////////////////////////////////////////////////////////////////
	//复制对象
	//@param source 复制source对象当前数据到本对象，不复制撤销列表。
	virtual DrawObject *CopyFrom(DrawObject * source);

	virtual DrawObject * CreateInstance() = 0;
	//创建深度复制副本
	//分配新的对象空间，复制当前数据到新对象。
	virtual DrawObject *Clone();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//绘图支持
	//////////////////////////////////////////////////////////////////////////

	//绘制对象
	virtual void Draw(Gdiplus::Graphics & graph);
	//对象绘制事件响应
	virtual void OnDraw(Gdiplus::Graphics & graph);

	//绘制边框
	virtual void DrawBorder(Gdiplus::Graphics & graph);
	//边框绘制事件响应
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);

	//填充对象
	virtual void DrawFillObject(Gdiplus::Graphics & graph);
	//对象填充事件响应
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);


	//////////////////////////////////////////////////////////////////////////
	//几何操作
	//////////////////////////////////////////////////////////////////////////
	
	//坐标转换操作

	//本体坐标变换至世界坐标
	virtual void Local2World(Gdiplus::Point * pt, int count = 1);
	virtual void Local2World(Gdiplus::Rect *rect, int count = 1);
	//世界坐标变换至本体坐标
	virtual void World2Local(Gdiplus::Point * pt, int count = 1);
	virtual void World2Local(Gdiplus::Rect *rect, int count = 1);

	//全局坐标（父对象坐标）变换至本体坐标
	virtual void Global2Local(Gdiplus::Point * pt, int count = 1);
	virtual void Global2Local(Gdiplus::Rect *rect, int count = 1);
	//本体坐标变换至全局坐标（父对象坐标）
	virtual void Local2Global(Gdiplus::Point * pt, int count = 1);
	virtual void Local2Global(Gdiplus::Rect *rect, int count = 1);

	//取对象中心坐标。本体坐标
	virtual Gdiplus::Point GetCenter();
	//取对象外框矩形，随对象旋转而旋转。本体坐标
	virtual Gdiplus::Rect GetObjectRect();

	//取对象区域，本体坐标
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);


	//取对象外框边界正矩形，全局坐标
	virtual Gdiplus::Rect GetBounds();

	virtual Gdiplus::Rect GetWorldBounds();

	//移动至point位置
	//@param point 对象位置，全局坐标
	virtual void MoveTo(const Gdiplus::Point& point);
	virtual void MoveTo(int x, int y);

	//偏移
	//@param x x方向偏移
	//@param y y方向偏移
	virtual void Offset(int x, int y);
	virtual void Offset(const Gdiplus::Point& point);

	//缩放
	//基于本体坐标原点缩放
	//@param scaleX  x方向缩放比例
	//@param scaleY  y方向缩放比例
	virtual void Scale(double scaleX, double scaleY);

	//基于给定缩放基点缩放
	virtual void ScaleAt(double scaleX, double scaleY,const Gdiplus::Point& origin);
	
	///测试对象是否在矩形框中
	///
	///判断依据对象中心是否在矩形中
	///@param rect 测试矩形，世界坐标
	virtual bool IsInRect(const Gdiplus::Rect &rect);

	static Gdiplus::RectF MeasureString( const CStringW & text, Gdiplus::Font * pFont,const Gdiplus::PointF& origin, Gdiplus::StringFormat *pFormat);
    //坐标单位
	static Gdiplus::Unit Unit;

private:
	//基本属性
	DrawObject * _pParent;

	//对象名称
	CString _name;
	//////////////////////////////////////////////////////////////////////////

	//几何属性。，角度单位（度）


	//位置，全局坐标（父对象本体坐标）
	Gdiplus::Point _position;
	//大小
	Gdiplus::Size _size;

	//绘图属性

	//剪切边框
	Gdiplus::Rect _clipRect;
	//是否剪切
	bool _bUsingClip;
	//是否显示
	bool _bVisible;

	Appearance * _pAppearance;


	//////////////////////////////////////////////////////////////////////////

	//控制属性
	bool _isActive;
	//是否为选择状态
	bool _bSelected;
};
