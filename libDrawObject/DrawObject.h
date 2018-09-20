#pragma once
#include "DrawObjectPropertiesID.h"
#include "BaseTypes.h"
#include "DrawObjectGlobal.h"
#include "ObjectObserver.h"
#include "IVisaulized.h"
#include "Appearance.h"
#include <vector>
#define FONT_SIZE_FACETOR   3.53f

class CDrawObject;

typedef CTypedPtrList <CObList,  CDrawObject * >CObjectsList;

/////
///绑定信息
///
///用于加载文档时重构绑定关系。保存绑定关系时，只保存绑定目标的内部名称列表。
///加载时建立全局绑定信息表，加载完成后，重建所有对象的绑定关系。
///@see SetBindBuilderState
///@see IsBindBuilding
///@see BuildBinding
///
struct BIND_OBJ_INFO
{
	CDrawObject *pSource;   //< 绑定源
	CStringList targetList; //< 绑定目标内部名称列表
};

//////////////////////////////////////////////////////////////////////////
///字体信息
///
///字体改变时用于保存Undo日志的新、旧字体相关属性值
///@see CHistoryFontChanged
///@see notifyAttribtesChanged
///
struct FontInfo
{
    UINT mask;
	CString fontName; ///< 字体名称
	float fontSize;   ///<字体大小
	int fontStyle;    ///<字体风格，包括Bold、Italic和Underline
	Gdiplus::Color fontColor; ///<字体颜色
};

template <typename T> struct CLogData
{
	T _data;
	CLogData(const T &data)
	{
		_data=data;
	}
};


/*!
 * \brief
 * 绘图对象基类
 * 
 * Write detailed description for CDrawObject here.
 * 
 * \remarks
 * Write remarks for CDrawObject here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CDrawObject:public CObject, public IObjectObserver, public IVisaulized
{
public:
    typedef std::vector<CDrawObject*> DRAW_OBJECT_LIST;

	////////////
	///初始化成员变量
	///
	void Initialize();
	CStringList m_CustomPropertyList;
	CMapStringToString m_CustomProperties;

protected:
	//////////////////////////////////////////////////////////////////////////
	///默认构造函数
	///
	///由RuntimeClass的CreateObject调用
	///
	CDrawObject();

	///
	///绑定来源对象列表. 绑定到自身的对象列表。自身属性发生变化时通过Notify通知这些绑定来源。
	///@see Notify
	///
	std::vector<CDrawObject*> _bindedObjects;

	//////////////////////////////////////////////////////////////////////////
	///绑定目标对象列表。
	///
	///自身绑定的对象列表。目标对象属性发生变化时，会调用Notify通知本对象。
	///可以在OnNotify中对这些通知消息进行处理。
	///@see Notify
	///@see OnNotify
	std::vector<CDrawObject*> _bindTargets;

	///
	///取得绑定到自身的对象列表
	///
	///@return 对象列表
	///
	std::vector<CDrawObject*> & GetBindedObjects();
	///
	///取得自身绑定的对象列表
	///
	///@return 对象列表
	///
	std::vector<CDrawObject*> & GetBindTargets();

	///
	///绑定目标对象前事件
	///
	///继承类可覆盖该方法，处理绑定到目标对象前的准备工作
	///
	///@param pTarget 将绑定的目标对象 
	///
	virtual void OnBinding(CDrawObject*pTarget);
	///
	///绑定目标对象后事件
	///
	///继承类可覆盖该方法，处理绑定到目标对象后的后续处理工作
	///
	///@param pTarget 将绑定的目标对象 
	///
	virtual void OnBinded(CDrawObject *pTarget);

	virtual void OnUnbindingFrom(CDrawObject *pTarget){}
	virtual void OnUnbindedFrom(CDrawObject *pTarget){}
	DECLARE_SERIAL(CDrawObject) 
	
public:  //IObjectObserver directive
	///
	///IObjectObserver接口重载
	///
	virtual void notifyAttributesChanged(CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal);
    virtual void notifyRevert(CDrawObject *pChanged, int &action );

    //IVisaulized
    Gdiplus::Image* GetPicture(int width, int height, Gdiplus::Color background = Gdiplus::Color::Transparent,
                               int flag = GET_PIC_FLAG_ASPECT | GET_PIC_POS_HCENTER | GET_PIC_POS_VCENTER);

	///
	///将本对象绑定到目标对象
	///
	///@param pTarget 被绑定的目标对象
	///
	void BindTo(CDrawObject *pTarget);
	///把本对象与目标对象解除绑定
	///
	///@param pTarget 解除绑定的目标对象
	///
	void UnbindFrom(CDrawObject *pTarget);
	void UnbindAll();

    virtual void ResetContent();


	CDrawObject(const CString & name);
	CDrawObject(const CString & name, int x, int y, int width, int height);
	CDrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);
	CDrawObject(const CString & name, const Gdiplus::Rect & rect);
	virtual ~ CDrawObject();

	//////////////////////////////////////////////////////////////////////////
	///设置绑定重建状态位
	///
	///@param state 
	/// - true 禁止绑定事件响应
	/// - false 允许绑定事件响应
	///
	///@remark当文件加载时，需要置绑定重建状态位为ture，避免绑定事件触发。绑定关系建立完成后，调用该函数置为false。解除锁定。
	///
	static void SetBindBuilderState(bool state);
	//////////////////////////////////////////////////////////////////////////
	///取绑定重建状态位
	///
	///@return 重建状态位
	///
	static bool IsBindBuilding();
	//////////////////////////////////////////////////////////////////////////
	///重建绑定关系
	///
	///该方法在文件加载后，即调用，以恢复对象绑定关系。
	///
	static void BuildBinding();
	///
	///查找对象列表中的特定对象的索引
	///
	///@param objects 对象列表
	///@param pTarget 查找目标
	///@return 目标对象在列表中的索引位置，0起始。未找到返回-1。
	///
	static int _findIndex(std::vector<CDrawObject*> & objects, CDrawObject* pTarget);
	///
	///根据对内部象名称，在列表中查找对象
	///
	///@param objects 对象列表
	///@param innerName 对象内部名称
	///@return 找到的对象，未找到返回NULL。
	///
	static CDrawObject * _findObjectByName(DRAW_OBJECT_LIST &objects,  const CString & innerName);

	//////////////////////////////////////////////////////////////////////////
	///设置打印状态位
	///
	///打印输出前，设置打印状态位为true，避免，部分对象仅显示而不可打印的部分被打印。
	///
	///@param state 打印状态位的新值
	///
	static void SetPrintingState(bool state);
	///取打印状态位
	///
	///@return 打印状态位
	/// - true 打印中
	/// - false 正常
	static bool IsPrinting();
	///生成唯一字符串
	///
	///用于生成对象内部名称
	///
	///@return 具有时空唯一性的字符串
	static CString GenerateUniqueName();
	
	//Attributes

	//只读
	
	///
	///对象类型
	///
	enum ObjectType
	{
		Shape, ///< 形状。一般绘图对象
		Operator, ///< 操作对象。用于做其他操作预先绘制的对象。例如用于剪切的CCutMask对象
		Tool,  ///< 工具对象。例如测量标尺对象，辅助线对象
		Mark   ///< 标记对象。目前未用到
	};
	//////////////////////////////////////////////////////////////////////////
	///从对象类型Id取得对象类型名称
	///
	///@prama Id 对象类型Id， 取值为ObjectType枚举值
	///@return 对象类型名称
	static LPCTSTR GetObjectType(int Id);
	///
	///取对象类型名称
	///
	///@return 类型名称
	///
	virtual LPCTSTR GetObjectType() const;
	///
	///取对象类型Id
	///
	///@return 对象类型ID
	///
	virtual int GetObjectTypeID() const;
	///
	///设置对象名称
	///
	///@param 新名称
	///
	virtual void SetName(CString name);
	///
	///取对象名称
	///
	virtual CString  GetName();
	virtual void OnNameChanging(CString & newName);
	virtual void OnNameChanged();

	///对象内部名称，作为名称索引的唯一标识
	virtual const CString GetInternalName() const;
	virtual void SetInternalName(CString internalName);
	virtual void OnInternalNameChanging(CString & internalName);
	virtual void OnInternalNameChanged();

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
	//对象旋转中心
	//TODO: BUG 旋转后移动旋转中心将影响_Angle的取值，需修正_Angle的值
	void SetRotateCenter(Gdiplus::Point center);
	virtual Gdiplus::Point GetRotateCenter();
	virtual void OnRotateCenterChanging(Gdiplus::Point & newCenter);
	virtual void OnRotateCenterChanged();
	//旋转角度
	virtual void SetAngle(double angle);
	virtual double GetAngle() const;
	virtual void OnAngleChanging(double &newAngle);
	virtual void OnAngleChanged();

	virtual bool GetWorldAngle(double *angle, Gdiplus::Point * rotateCenter);

	//////////////////////////////////////////////////////////////////////////

	//绘图属性

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

	//是否显示对象句柄
	virtual void SetShowTracker(bool isShowTracker);
	virtual bool GetShowTracker() const;
	virtual void OnShowTrackerChanging(bool & newShowTracker);
	virtual void OnShowTrackerChanged();

	//对象句柄颜色
	virtual void SetTrackerColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetTrackerColor() const;
	virtual void OnTrackerColorChanging(Gdiplus::Color & color);
	virtual void OnTrackerColorChanged();


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
	virtual void SetTextIsVert(bool isVert);
	virtual bool GetTextIsVert();
	virtual void OnTextVertChanging(bool & isVert);
	virtual void OnTextVertChanged();

	//////////////////////////////////////////////////////////////////////////

	//控制属性

	//对象激活状态
	virtual void SetActive(bool isActive);
	virtual bool GetActive();
	virtual void OnActiveChanging(bool &isActived);
	virtual void OnActiveChanged();

	//对象选择状态
	virtual void SetSelected(bool isSelected);
	virtual bool GetSelected() const;
	virtual void OnSelectedChanging(bool & newSelected);
	virtual void OnSelectedChanged();
	//////////////////////////////////////////////////////////////////////////

	//对象父对象
	virtual void SetParent(CDrawObject * pParent);
	virtual const CDrawObject *GetParent() const;
	virtual CDrawObject *GetParent();
	virtual void OnParentChanging(CDrawObject * & pObject);
	virtual void OnParentChanged();

	//////////////////////////////////////////////////////////////////////////

	//使能属性

	//获得所支持的属性名称列表
	static CStringList & GetPropertyList();

	//可选择
	virtual void SetSelectable(bool isSelectable);
	virtual bool GetSelectable();
	virtual void OnSelectableChanging(bool & isSelectable);
	virtual void OnSelectableChanged();

	//可移动
	virtual void SetMovable(bool isMovable);
	virtual bool GetMovable();
	virtual void OnMovableChanging(bool & isMovable);
	virtual void OnMovableChanged();

	//可旋转
	virtual void SetRotatable(bool isRotatable);
	virtual bool GetRotatable();
	virtual void OnRotatableChanging(bool &isRotatable);
	virtual void OnRotatableChanged();

	//可改变大小
	virtual void SetSizable(bool isSizable);
	virtual bool GetSizable();
	virtual void OnSizableChanging(bool & isSizable);
	virtual void OnSizableChanged();


	//////////////////////////////////////////////////////////////////////////

	//自定义属性
	virtual CStringList & GetCustomPropertyList();
	virtual void AddCustomProperty(const CString & propertyName, const CString & propertyValue);
	virtual void RemoveCustomProperty(const CString & propertyName);
	virtual bool GetCustomProperty(const CString & propertyName, CString & propertyValue);
	//////////////////////////////////////////////////////////////////////////


	//method
	///
	///消息通知
	virtual void Notify(UINT msgID, DWORD_PTR wParam = 0, LPVOID lpParam = NULL);
	///通知响应
	virtual void OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam);

	//UI 交互

	//创建属性表
	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	///
	///属性改变响应
	///
	///通过属性表交互修改表中相关属性值后，通过回调该方法修改对象相应属性值
	///
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);

	///TODO
	void SetCommonMenuId(UINT commonMenuId);
	virtual void SetMenuId(UINT menuId);
	virtual UINT GetMenuId();
	///
	CMenu * GetObjectMenu(CMenu * pMenu);

	virtual void OnMenuCommand(UINT command);

	virtual bool OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point);
	virtual bool OnLButtonDblClk(CWnd *pWnd, UINT nFlags, Gdiplus::Point point);

	//////////////////////////////////////////////////////////////////////////
	//撤销列表支持
	//////////////////////////////////////////////////////////////////////////
	//复制对象
	//@param source 复制source对象当前数据到本对象，不复制撤销列表。
	virtual CDrawObject *CopyFrom(CDrawObject * source);

	//创建深度复制副本
	//分配新的对象空间，复制当前数据到新对象。
	virtual CDrawObject *Clone();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//绘图支持
	//////////////////////////////////////////////////////////////////////////

	//绘制对象句柄
	virtual void DrawTracker(Gdiplus::Graphics & graph);
	//对象句柄绘制事件响应
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);

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
	virtual void MoveTo(Gdiplus::Point point);
	virtual void MoveTo(int x, int y);

	//偏移
	//@param x x方向偏移
	//@param y y方向偏移
	virtual void Offset(int x, int y);
	virtual void Offset(Gdiplus::Point point);

	//旋转
	//基于当前旋转中心，自当前角度旋转angle度，angle小于0，向左旋；angle大于0，向右旋
	virtual void Rotate(double angle);

	
	//缩放
	//基于本体坐标原点缩放
	//@param scaleX  x方向缩放比例
	//@param scaleY  y方向缩放比例
	virtual void Scale(double scaleX, double scaleY);

	//基于给定缩放基点缩放
	virtual void ScaleAt(double scaleX, double scaleY, Gdiplus::Point origin);
	
	//取对象句柄总数
	virtual int GetHandleCount();
	
	//根据句柄号取句柄位置，本体坐标
	virtual Gdiplus::Point GetHandle(int nHandle);

	//根据句柄号，取句柄矩形框，本体坐标
	virtual Gdiplus::Rect GetHandleRect(int nHandle);

	//根据句柄号，取该句柄对应光标
	//用于鼠标移动至该光标上时显示对应光标
	virtual HCURSOR GetHandleCursor(int nHandle);

	//移动对应nHandle指定的句柄移动到，point位置
	//@param nHandle 需移动的句柄，该方法处理后，nHandle会改变
	//@param point 需移动到的位置，全局坐标
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);

	//测试pt位置与本对象的“击中”位置，用于判断鼠标点击位置选中对象的部位
	//@param pt 用于测试的坐标点
	//@return “击中”位置
	//     - 0 未击中
	//     - 1—GetHandleCount()，击中各句柄的编号
	//     - GetHandleCount()+1， 未击中句柄，击中对象区域
	virtual int HitTest(const Gdiplus::Point & pt);

	///测试对象是否在矩形框中
	///
	///判断依据对象中心是否在矩形中
	///@param rect 测试矩形，世界坐标
	virtual bool IsInRect(const Gdiplus::Rect &rect);


	//序列化支持
	virtual void Serialize(CArchive & ar);


	static void SetStorage(IStorage * pStorage);
	static IStorage *GetStorage();
	static Gdiplus::RectF MeasureString( const CStringW & text, Gdiplus::Font * pFont, Gdiplus::PointF origin, Gdiplus::StringFormat *pFormat);
    //坐标单位
	static Gdiplus::Unit m_Unit;
	static IStorage *m_pRootStorage;

private:
	//基本属性
	CDrawObject * m_pParent;

	//对象名称
	CString _name;
	//对象内部名称
	CString _internalName;
	//////////////////////////////////////////////////////////////////////////

	//几何属性。，角度单位（度）


	//位置，全局坐标（父对象本体坐标）
	Gdiplus::Point _position;
	//大小
	Gdiplus::Size _size;
	//自定义旋转中心，本体坐标
	Gdiplus::Point _rotateCenter;
	//是否启用旋转中心
	bool m_bUseRotateCenter;
	//旋转角度
	DOUBLE _angle;
	//////////////////////////////////////////////////////////////////////////

	//绘图属性

	//剪切边框
	Gdiplus::Rect m_ClipRect;
	//是否剪切
	bool m_bUsingClip;
	//是否显示
	bool _bVisible;

	Appearance _appearance;

	//////////////////////////////////////////////////////////////////////////

	//控制属性
	bool _isActive;
	//是否为选择状态
	bool m_bSelected;
	//////////////////////////////////////////////////////////////////////////
	//使能属性

	//是否可选择
	bool _bSelectable;
	//是否可移动
	bool _bMovable;

	//是否可选转
	bool _bRotatable;
	//是否可改变大小
	bool _bSizable;


    //Gdiplus::Image * _pVisualizedImage;
	//////////////////////////////////////////////////////////////////////////

	static bool m_bIsPrinting;
	static bool _bIsBindBuilding;
	static std::vector<BIND_OBJ_INFO*> _preBindObjects;
	static DRAW_OBJECT_LIST _allObjects;

};
