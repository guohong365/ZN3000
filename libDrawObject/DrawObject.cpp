#include "stdafx.h"
#include <Rpc.h>
#include "DrawObject.h"
#include "MathUtility.h"
#include "ImageUtility.h"
#include "DrawObjectPropertiesID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
#define STRING2x(x) #x
#define STRINGxx(x) STRING2x(x)

#pragma message ( "_SECURE_SCL=" STRINGxx(_SECURE_SCL))
#pragma message ("_HAS_ITERATOR_DEBUGGING=" STRINGxx(_HAS_ITERATOR_DEBUGGING))

#if (_SECURE_SCL!=0)
#endif
#if(_HAS_ITERATOR_DEBUGGING!=0)
#endif

//坐标单位
Gdiplus::Unit CDrawObject::Unit = Gdiplus::UnitPixel;

IMPLEMENT_SERIAL(CDrawObject, CObject, 0)
void CDrawObject::Initialize()
{
	_name=_T("");
	_angle = 0.0;
	m_bUsingClip = false;
	_bVisible = true;
	_isActive=false;
	m_bSelected = false;
	_bSelectable = true;
	_bMovable = true;
	_bRotatable = true;
	_bSizable = true;
	_pParent = NULL;
	//_appearance.FontSize=FontSizeArray[DEFAULT_FONT_SIZE_INDEX].SizePoint*FONT_SIZE_FACETOR;

	 m_bUseRotateCenter = false;

	_internalName = GenerateUniqueName();
}

CDrawObject::CDrawObject()
{
	Initialize();
}

CDrawObject::CDrawObject(const CString & name)
{
	Initialize();
	_name = name;
}
CDrawObject::CDrawObject(const CString & name, int x, int y, int width, int height)
{
	Initialize();
	_name = name;
	_position = Gdiplus::Point(x, y);
	_size = Gdiplus::Size(width, height);
}
CDrawObject::CDrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size)
{
	Initialize();
	_name = name;
	_position = point;
	_size = size;
}
CDrawObject::CDrawObject(const CString & name, const Gdiplus::Rect & rect)
{
	Initialize();
	_name = name;
	_position = Gdiplus::Point(rect.X, rect.Y); 
	_size =Gdiplus::Size(rect.Width, rect.Height); 
}

CDrawObject::~CDrawObject()
{
}

CString CDrawObject::GenerateUniqueName()
{
	LARGE_INTEGER PerformanceCount;
	QueryPerformanceCounter(&PerformanceCount);

	CString name;
	name.Format(_T("%020I64u"), PerformanceCount.QuadPart);
	return name;
}


//对象名称
void CDrawObject::SetName(CString name)
{
	if (_name != name)
	{
		OnNameChanging(name);
		_name = name;
		OnNameChanged();
	}
}
 CString CDrawObject::GetName()  
{
	return _name;
}

void CDrawObject::OnNameChanging(CString & newName)
{
    Notify(DNM_NAME_CHANGING, 0, &newName);
}
void CDrawObject::OnNameChanged()
{
	Notify(DNM_NAME_CHANGED);
}

void CDrawObject::SetInternalName(CString internalName)
{
	if (_internalName != internalName)
	{
		OnInternalNameChanging(internalName);
		_internalName = internalName;
		OnInternalNameChanged();
	}
}
const CString CDrawObject::GetInternalName() const 
{
	return _internalName;
}

void CDrawObject::OnInternalNameChanging(CString & internalName)
{

}
void CDrawObject::OnInternalNameChanged()
{

}

//////////////////////////////////////////////////////////////////////////

//几何属性。，角度单位（度）

//坐标单位
void CDrawObject::SetUnit(Gdiplus::Unit unit)
{
	CDrawObject::Unit = unit;
}

Gdiplus::Unit CDrawObject::GetUnit()
{
	return CDrawObject::Unit;
}

//位置
void CDrawObject::SetPosition(Gdiplus::Point point)
{
	if (!_position.Equals(point))
	{
		OnPositionChanging(point);
		_position = point;
		OnPositionChanged();
	}
}

Gdiplus::Point CDrawObject::GetPosition() const
{
	return _position;
}

void CDrawObject::OnPositionChanging(Gdiplus::Point & newPoint)
{
	Notify(DNM_POSITION_CHANGING, 0, &newPoint);
}
void CDrawObject::OnPositionChanged()
{
	Notify(DNM_POSITION_CHANGED);
}

//大小
void CDrawObject::SetSize(Gdiplus::Size size)
{
	if (!_size.Equals(size))
	{
		OnSizeChanging(size);
		_size = size;
		OnSizeChanged();
	}
}

Gdiplus::Size CDrawObject::GetSize() const
{
	return _size;
}

void CDrawObject::OnSizeChanging(Gdiplus::Size & newSize)
{
	Notify(DNM_SIZE_CHANGING, 0, &newSize);
}
void CDrawObject::OnSizeChanged()
{
	Notify(DNM_SIZE_CHANGED);
}

//旋转中心
void CDrawObject::SetRotateCenter(Gdiplus::Point center)
{
	m_bUseRotateCenter = true;
	OnRotateCenterChanging(center);
	_rotateCenter = center;
	OnRotateCenterChanged();
}

Gdiplus::Point CDrawObject::GetRotateCenter()
{
	if (m_bUseRotateCenter)
	{
		return _rotateCenter;
	}
	else
	{
		return GetCenter();
	}
}
void CDrawObject::OnRotateCenterChanging(Gdiplus::Point & newCenter)
{
	Notify(DNM_ROTATE_CENTER_CHANGING, 0, &newCenter);
}
void CDrawObject::OnRotateCenterChanged()
{
	Notify(DNM_ROTATE_CENTER_CHANGED);
}

//旋转角度
void CDrawObject::SetAngle(double angle)
{
	if (_angle != angle)
	{
		OnAngleChanging(angle);
		_angle = angle;
		OnAngleChanged();
	}
}
double CDrawObject::GetAngle() const 
{
	return _angle;
}
void CDrawObject::OnAngleChanging(double &newAngle)
{
	Notify(DNM_ANGLE_CHANGING, 0, &newAngle);
}
void CDrawObject::OnAngleChanged()
{
	Notify(DNM_ANGLE_CHANGED);
}


//////////////////////////////////////////////////////////////////////////

//绘图属性

//剪切边框
void CDrawObject::SetClipRect(Gdiplus::Rect rect)
{
	if (!m_ClipRect.Equals(rect))
	{
		OnClipRectChanging(rect);
		m_ClipRect = rect;
		OnClipRectChanged();
	}
}

Gdiplus::Rect CDrawObject::GetClipRect() const
{
	return m_ClipRect;
}

void CDrawObject::OnClipRectChanging(Gdiplus::Rect & newRect)
{
	Notify(DNM_CLIP_RECT_CHANGING, 0, &newRect);
}
void CDrawObject::OnClipRectChanged()
{
	Notify(DNM_CLIP_RECT_CHANGED);
}

//是否剪切
void CDrawObject::SetUsingClip(bool isUsingClip)
{
	if (m_bUsingClip != isUsingClip)
	{
		OnUsingClipChanging(isUsingClip);
		m_bUsingClip = isUsingClip;
		OnUsingClipChanged();
	}
}
bool CDrawObject::GetUsingClip() const 
{
	return m_bUsingClip;
}

void CDrawObject::OnUsingClipChanging(bool & newUsingClip)
{
	Notify(DNM_USING_CLIP_CHANGING, 0, & newUsingClip);
}
void CDrawObject::OnUsingClipChanged()
{
	Notify(DNM_USING_CLIP_CHANGED);
}

//是否显示
void CDrawObject::SetVisible(bool isVisible)
{
	if (isVisible != _bVisible)
	{
		OnVisibleChanging(isVisible);
		_bVisible = isVisible;
		OnVisibleChanged();
	}
}
bool CDrawObject::GetVisible() const 
{
	return _bVisible;
}

void CDrawObject::OnVisibleChanging(bool & newVisible)
{
    Notify(DNM_VISIBLE_CHANGING, 0, &newVisible);
}
void CDrawObject::OnVisibleChanged()
{
	Notify(DNM_VISIBLE_CHANGED);
}

//是否填充
void CDrawObject::SetFilled(bool isFilled)
{
	if (_appearance.ShowFill != isFilled)
	{
		OnFilledChanging(isFilled);
		_appearance.ShowFill = isFilled;
		OnFilledChanged();
	}
}
bool CDrawObject::GetFilled() const 
{
	return _appearance.ShowFill;
}

void CDrawObject::OnFilledChanging(bool & newFilled)
{
    Notify(DNM_FILLED_CHANGING, 0, &newFilled);
}
void CDrawObject::OnFilledChanged()
{
	Notify(DNM_FILLED_CHANGED);
}

//是否绘制边框
void CDrawObject::SetShowBorder(bool isShowBorder)
{
	if (_appearance.ShowBorder != isShowBorder)
	{
		OnShowBorderChanging(isShowBorder);
		_appearance.ShowBorder = isShowBorder;
		OnVisibleChanged();
	}
}
bool CDrawObject::GetShowBorder() const 
{
	return _appearance.ShowBorder;
}

void CDrawObject::OnShowBorderChanging(bool & newShowBorder)
{
	Notify(DNM_SHOW_BORDER_CHANGING, 0, &newShowBorder);
}
void CDrawObject::OnShowBorderChanged()
{
	Notify(DNM_SHOW_BORDER_CHANGED);
}
void CDrawObject::SetShowTracker(bool isShowTracker)
{
	if (isShowTracker != _appearance.ShowTrack)
	{
		OnShowTrackerChanging(isShowTracker);
		_appearance.ShowTrack = isShowTracker;
		OnShowTrackerChanged();
	}
}
bool CDrawObject::GetShowTracker() const 
{
	return _appearance.ShowTrack;
}

void CDrawObject::OnShowTrackerChanging(bool & newShowTracker)
{
	Notify(DNM_SHOW_TRACKER_CHANGING, 0, &newShowTracker);
}
void CDrawObject::OnShowTrackerChanged()
{
	Notify(DNM_SHOW_TRACKER_CHANGED);
}

void CDrawObject::SetTrackerColor(Gdiplus::Color color)
{
	if (color.GetValue() != _appearance.TraceColor.GetValue())
	{
		OnTrackerColorChanging(color);
		_appearance.TraceColor = color;
		OnTrackerColorChanged();
	}
}

Gdiplus::Color CDrawObject::GetTrackerColor() const
{
	return _appearance.TraceColor;
}

void CDrawObject::OnTrackerColorChanging(Gdiplus::Color & color)
{
    Notify(DNM_TRACKER_COLOR_CHANGING, 0,&color);
}
void CDrawObject::OnTrackerColorChanged()
{
	Notify(DNM_TRACKER_COLOR_CHANGED);
}

//透明度
void CDrawObject::SetTransparent(float transparent)
{
	if (_appearance.Transparent != transparent)
	{
		OnTransparentChanging(transparent);
		_appearance.Transparent = transparent;
		OnTransparentChanged();
	}
}
float CDrawObject::GetTransparent() const 
{
	return _appearance.Transparent;
}
void CDrawObject::OnTransparentChanging(float &newTransparent)
{
	Notify(DNM_TRANSPARENT_CHANGING, 0, &newTransparent);
}
void CDrawObject::OnTransparentChanged()
{
	Notify(DNM_TRANSPARENT_CHANGED);
}


//填充颜色
void CDrawObject::SetFillColor(Gdiplus::Color color)
{
	if (_appearance.FillColor.GetValue() != color.GetValue())
	{
		OnFillColorChanging(color);
		_appearance.FillColor = color;
		OnFillColorChanged();
	}
}

Gdiplus::Color CDrawObject::GetFillColor() const
{
	return _appearance.FillColor;
}

void CDrawObject::OnFillColorChanging(Gdiplus::Color & newColor)
{
	Notify(DNM_FILL_COLOR_CHANGING, 0, &newColor);
}
void CDrawObject::OnFillColorChanged()
{
	Notify(DNM_FILL_COLOR_CHANGED);
}

//边框颜色
void CDrawObject::SetLineColor(Gdiplus::Color color)
{
	if (_appearance.LineColor.GetValue() != color.GetValue())
	{
		OnLineColorChanging(color);
		_appearance.LineColor = color;
		OnLineColorChanged();
	}
}

Gdiplus::Color CDrawObject::GetLineColor() const
{
	return _appearance.LineColor;
}

void CDrawObject::OnLineColorChanging(Gdiplus::Color & newColor)
{
	Notify(DNM_LINE_COLOR_CHANGING, 0, &newColor);
}
void CDrawObject::OnLineColorChanged()
{
	Notify(DNM_LINE_COLOR_CHANGED);
}

void CDrawObject::SetLineWidth(float width)
{
	if (_appearance.LineWidth != width)
	{
		OnLineWidthChanging(width);
		_appearance.LineWidth = width;
		OnLineWidthChanged();
	}
}
float CDrawObject::GetLineWidth()
{
	return _appearance.LineWidth;
}
void CDrawObject::OnLineWidthChanging(float &width)
{
	Notify(DNM_LINE_WIDTH_CHANGING, 0, &width);
}
void CDrawObject::OnLineWidthChanged()
{
	Notify(DNM_LINE_WIDTH_CHANGED);
}

//字体颜色
void CDrawObject::SetFontColor(Gdiplus::Color color)
{
	if (_appearance.FontColor.GetValue() != color.GetValue())
	{
		OnFontColorChanging(color);
		_appearance.FontColor = color;
		OnFontColorChanged();
	}
}

Gdiplus::Color CDrawObject::GetFontColor() const
{
	return _appearance.FontColor;
}

void CDrawObject::OnFontColorChanging(Gdiplus::Color & newFontColor)
{
	Notify(DNM_FONT_COLOR_CHANGING, 0, &newFontColor);
}
void CDrawObject::OnFontColorChanged()
{
	Notify(DNM_FONT_COLOR_CHANGED);
}

//////////////////////////////////////////////////////////////////////////

//控制属性

//是否为选择状态
void CDrawObject::SetSelected(bool isSelected)
{
	if (m_bSelected != isSelected)
	{
		OnSelectedChanging(isSelected);
		m_bSelected = isSelected;
		OnSelectedChanged();
	}
}

bool CDrawObject::GetSelected() const 
{
	return m_bSelected;
}

void CDrawObject::OnSelectedChanging(bool & newSelected)
{
	Notify(DNM_SELECTED_CHANGING, 0, &newSelected);	
}
void CDrawObject::OnSelectedChanged()
{
	Notify(DNM_SELECTED_CHANGED); 
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
void CDrawObject::SetParent(CDrawObject * pParent)
{
	if (_pParent != pParent)
	{
		OnParentChanging(pParent);
		_pParent = pParent;
		OnParentChanged();
	}
}
const CDrawObject *CDrawObject::GetParent() const 
{
	return _pParent;
}

CDrawObject *CDrawObject::GetParent()
{
	return _pParent;
}

void CDrawObject::OnParentChanging(CDrawObject * &pObject)
{
	Notify(DNM_PARENT_CHANGING, 0, &pObject);
}
void CDrawObject::OnParentChanged()
{
	Notify(DNM_PARENT_CHANGED);
}

//使能属性


//////////////////////////////////////////////////////////////////////////

//自定义属性
CStringList & CDrawObject::GetCustomPropertyList()
{
	return m_CustomPropertyList;
}
void CDrawObject::AddCustomProperty(const CString & propertyName, const CString & propertyValue)
{
	CString rValue;
	//try to find existed key
	if (m_CustomProperties.Lookup(propertyName, rValue) == NULL)
	{
		m_CustomPropertyList.AddHead(propertyName);
	}
	m_CustomProperties[propertyName] = propertyValue;
}
void CDrawObject::RemoveCustomProperty(const CString & propertyName)
{
	CString rValue;
	POSITION pos = m_CustomPropertyList.Find(propertyName);
	if (pos)
	{
		m_CustomPropertyList.RemoveAt(pos);
		m_CustomProperties.RemoveKey(propertyName);
	}
}
bool CDrawObject::GetCustomProperty(const CString & propertyName, CString & propertyValue)
{
	return m_CustomProperties.Lookup(propertyName, propertyValue) == TRUE;
}

//////////////////////////////////////////////////////////////////////////


//method

//bool CDrawObject::Notify(UINT messageID, DWORD wParam, LPVOID lpParam)
//{
//      switch(messageID)
//      {
//      case DN_PARENT_SIZE_CHANGED:
//              {
//
//              }
//              break;
//      case DN_PROPERTY_CHANGED:
//              {
//
//              }
//              break;
//      default:
//              break;
//      }
//      return true;
//}
//


LOGFONTW OBJECT_DEFAULT_FONT = { 0 };

//////////////////////////////////////////////////////////////////////////
//撤销列表支持
//////////////////////////////////////////////////////////////////////////
//复制对象
//@param source 复制source对象当前数据到本对象，不复制撤销列表。
CDrawObject *CDrawObject::CopyFrom(CDrawObject * source)
{
	_name = source->_name;
	_position = source->_position;
	_size = source->_size;
	_rotateCenter = source->_rotateCenter;
	m_bUseRotateCenter = source->m_bUseRotateCenter;
	_angle = source->_angle;
	m_ClipRect = source->m_ClipRect;
	m_bUsingClip = source->m_bUsingClip;
	_bVisible = source->_bVisible;
	_appearance=source->_appearance;
	m_bSelected = source->m_bSelected;
	_bSelectable = source->_bSelectable;
	_bMovable = source->_bMovable;
	_bRotatable = source->_bRotatable;
	_bSizable = source->_bSizable;
	m_CustomPropertyList.RemoveAll();
	POSITION pos = source->m_CustomPropertyList.GetHeadPosition();
	while (pos)
	{
		m_CustomPropertyList.AddTail(source->m_CustomPropertyList.GetNext(pos));
	}
	m_CustomProperties.RemoveAll();
	CString key;
	CString rValue;
	pos = source->m_CustomProperties.GetStartPosition();
	while (pos)
	{
		source->m_CustomProperties.GetNextAssoc(pos, key, rValue);
		m_CustomProperties[key] = rValue;
	}
	return this;
}


//创建副本
//分配新的对象空间，复制当前数据到新对象，但不复制撤销列表。
CDrawObject *CDrawObject::Clone()
{
	CDrawObject *pObject = DYNAMIC_DOWNCAST(CDrawObject, GetRuntimeClass()->CreateObject());
	ASSERT(pObject);
	pObject->CopyFrom(this);
	return pObject;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//绘图支持
//////////////////////////////////////////////////////////////////////////

void CDrawObject::DrawTracker(Gdiplus::Graphics & graph)
{
	if (GetSelected() && GetShowTracker() )
	{
		OnDrawTracker(graph);
	}
}
void CDrawObject::OnDrawTracker(Gdiplus::Graphics & graph)
{
	if (GetSelected())
	{
		Gdiplus::CompositingMode mode = graph.GetCompositingMode();
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			Gdiplus::Color color=(0xC0FFFFFF&Gdiplus::Color::Green);;
			if(!GetSizable())
			{
				color=(0xC0FFFFFF & Gdiplus::Color::Red);
			}
			Gdiplus::Pen pen(color);
			Gdiplus::SolidBrush brush(color);
			Gdiplus::Rect rect = GetHandleRect(nHandle);
			graph.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
			graph.FillRectangle(&brush, rect);
			graph.DrawRectangle(&pen, rect);
		}
		graph.SetCompositingMode(mode);
	}
}

void CDrawObject::Draw(Gdiplus::Graphics & graph)
{
	if (GetVisible())
	{
		graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graph.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
		graph.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		float m[6];
		Gdiplus::Matrix matrix;
		Gdiplus::Matrix oldMatrix;
		graph.GetTransform(&oldMatrix);
		graph.GetTransform(&matrix);
		matrix.GetElements(m);
		matrix.Translate((Gdiplus::REAL) GetPosition().X, (Gdiplus::REAL) GetPosition().Y);
		Gdiplus::PointF pointF((Gdiplus::REAL) GetRotateCenter().X, (Gdiplus::REAL) GetRotateCenter().Y);
		matrix.RotateAt((Gdiplus::REAL) GetAngle(), pointF);
		graph.SetTransform(&matrix);
		matrix.GetElements(m);

		Gdiplus::Region clip;
		Gdiplus::Rect rf;
		bool setClip = false;
		if (!graph.IsClipEmpty())
		{
			if (GetUsingClip())
			{
				graph.GetClip(&clip);
				clip.GetBounds(&rf, &graph);
				graph.SetClip(m_ClipRect, Gdiplus::CombineModeIntersect);

				graph.GetClipBounds(&rf);
				setClip = true;
			}
		}
		OnDraw(graph);
		if(setClip)
		{
			graph.SetClip(&clip);
		}
		DrawBorder(graph);
		DrawTracker(graph);
		graph.SetTransform(&oldMatrix);
	}
}
void CDrawObject::OnDraw(Gdiplus::Graphics & graph)
{
	DrawFillObject(graph);
}

void CDrawObject::DrawBorder(Gdiplus::Graphics & graph)
{
	if ((GetShowBorder() || GetActive()))
	{
		OnDrawBorder(graph);
	}
}
void CDrawObject::OnDrawBorder(Gdiplus::Graphics & graph)
{

}

void CDrawObject::DrawFillObject(Gdiplus::Graphics & graph)
{
	if (GetFilled())
	{
		OnDrawFillObject(graph);
	}
}
void CDrawObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{

}

//////////////////////////////////////////////////////////////////////////
//几何操作
//////////////////////////////////////////////////////////////////////////
Gdiplus::Point CDrawObject::GetCenter()
{
	Gdiplus::Rect rect = GetObjectRect();
	return Gdiplus::Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
}


Gdiplus::Rect CDrawObject::GetObjectRect()
{
	return Gdiplus::Rect(Gdiplus::Point(0, 0), GetSize());
}

void CDrawObject::GetObjectRegion(Gdiplus::GraphicsPath & region)
{
	region.Reset();
	region.AddRectangle(GetObjectRect());
}


Gdiplus::Rect CDrawObject::GetBounds()
{
	Gdiplus::Rect rect=GetObjectRect();
	Gdiplus::Point pt[4];
	pt[0] =Gdiplus::Point(rect.X,rect.Y);
	pt[1] = Gdiplus::Point(rect.X, rect.Y + rect.Height);
	pt[2] = Gdiplus::Point(rect.X + rect.Width,rect.Y);
	pt[3] = Gdiplus::Point(rect.X + rect.Width, rect.Y + rect.Height);
	Local2Global(pt, 4);
	int minx, maxx;
	minx = maxx = pt[0].X;
	int miny, maxy;
	miny = maxy = pt[0].Y;
	for (int i = 1; i < 4; i++)
	{
		if (minx > pt[i].X)
			minx = pt[i].X;
		if (maxx < pt[i].X)
			maxx = pt[i].X;
		if (miny > pt[i].Y)
			miny = pt[i].Y;
		if (maxy < pt[i].Y)
			maxy = pt[i].Y;
	}
	return Gdiplus::Rect(minx, miny, maxx - minx, maxy - miny);
}

Gdiplus::Rect CDrawObject::GetWorldBounds()
{
	Gdiplus::Rect rect=GetObjectRect();
	Gdiplus::Point pt[4];
	pt[0] =Gdiplus::Point(rect.X,rect.Y);
	pt[1] = Gdiplus::Point(rect.X, rect.Y + rect.Height);
	pt[2] = Gdiplus::Point(rect.X + rect.Width,rect.Y);
	pt[3] = Gdiplus::Point(rect.X + rect.Width, rect.Y + rect.Height);
	Local2World(pt, 4);
	int minx, maxx;
	minx = maxx = pt[0].X;
	int miny, maxy;
	miny = maxy = pt[0].Y;
	for (int i = 1; i < 4; i++)
	{
		if (minx > pt[i].X)
			minx = pt[i].X;
		if (maxx < pt[i].X)
			maxx = pt[i].X;
		if (miny > pt[i].Y)
			miny = pt[i].Y;
		if (maxy < pt[i].Y)
			maxy = pt[i].Y;
	}
	return Gdiplus::Rect(minx, miny, maxx - minx, maxy - miny);
}


void CDrawObject::MoveTo(Gdiplus::Point point)
{
	SetPosition(point);
}
void CDrawObject::MoveTo(int x, int y)
{
	SetPosition(Gdiplus::Point(x, y));
}
void CDrawObject::Offset(int x, int y)
{
	Offset(Gdiplus::Point(x, y));
}

void CDrawObject::Offset(Gdiplus::Point point)
{
	SetPosition(_position + point);
}

void CDrawObject::Rotate(double angle)
{
	SetAngle(GetAngle() + angle);
}

void CDrawObject::Scale(double scaleX, double scaleY)
{
	Gdiplus::Size size;
	size.Width = (int) (_size.Width * scaleX);
	size.Height = (int) (_size.Height * scaleY);
	SetSize(size);
}

int CDrawObject::GetHandleCount()
{
	return 8;
}

Gdiplus::Point CDrawObject::GetHandle(int nHandle)
{
	Gdiplus::Size size = GetSize();
	int x, y, xCenter, yCenter;

	xCenter = size.Width / 2;
	yCenter = size.Height / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = 0;
		y = 0;
		break;

	case 2:
		x = xCenter;
		y = 0;
		break;

	case 3:
		x = size.Width;
		y = 0;
		break;

	case 4:
		x = size.Width;
		y = yCenter;
		break;

	case 5:
		x = size.Width;
		y = size.Height;
		break;

	case 6:
		x = xCenter;
		y = size.Height;
		break;

	case 7:
		x = 0;
		y = size.Height;
		break;

	case 8:
		x = 0;
		y = yCenter;
		break;
	}

	return Gdiplus::Point(x, y);
}

Gdiplus::Rect CDrawObject::GetHandleRect(int nHandle)
{
	Gdiplus::Point pt = GetHandle(nHandle);
	return Gdiplus::Rect(pt.X - 15, pt.Y - 15, 31, 31);
}

HCURSOR CDrawObject::GetHandleCursor(int nHandle)
{
	LPCTSTR id;
	if (!GetSizable())
	{
		id = IDC_ARROW;
	}
	else
	{
		switch (nHandle)
		{
		default:
			id = IDC_ARROW;
			break;
		case 1:
		case 5:
			id = IDC_SIZENWSE;
			break;

		case 2:
		case 6:
			id = IDC_SIZENS;
			break;

		case 3:
		case 7:
			id = IDC_SIZENESW;
			break;

		case 4:
		case 8:
			id = IDC_SIZEWE;
			break;
		case 9:
			id = IDC_SIZEALL;
			break;
		}
	}

	return AfxGetApp()->LoadStandardCursor(id);
}
void CDrawObject::MoveHandleTo(int &nHandle, Gdiplus::Point point)
{
	if (GetSizable())
	{
		CRect rect(_position.X, _position.Y, _position.X + _size.Width, _position.Y + _size.Height);
		Gdiplus::Size newSize = _size;
		Gdiplus::Point newPos = _position;
		switch (nHandle)
		{
		case 1:
			if (point.X > rect.right && point.Y < rect.bottom)  //x revert
			{
				nHandle = 3;
			}
			if (point.X < rect.right && point.Y > rect.bottom)  //y revert
			{
				nHandle = 7;
			}
			if (point.X > rect.right && point.Y > rect.bottom)  // x & y revert
			{
				nHandle = 5;
			}
			rect.left = point.X;
			rect.top = point.Y;
			break;
		case 2:
			if (point.Y > rect.bottom)
			{
				nHandle = 6;
			}
			rect.top = point.Y;
			break;
		case 3:
			if (point.X < rect.left && point.Y < rect.bottom)
			{
				nHandle = 1;
			}
			if (point.X < rect.left && point.Y > rect.bottom)
			{
				nHandle = 7;
			}
			if (point.X > rect.left && point.Y > rect.bottom)
			{
				nHandle = 5;
			}
			rect.right = point.X;
			rect.top = point.Y;
			break;
		case 4:
			if (point.X < rect.left)
			{
				nHandle = 8;
			}
			rect.right = point.X;
			break;
		case 5:
			if (point.X < rect.left && point.Y > rect.top)
			{
				nHandle = 7;
			}
			if (point.X > rect.left && point.Y < rect.top)
			{
				nHandle = 3;
			}
			if (point.X < rect.left && point.Y < rect.top)
			{
				nHandle = 1;
			}
			rect.right = point.X;
			rect.bottom = point.Y;
			break;
		case 6:
			if (point.Y < rect.top)
			{
				nHandle = 2;
			}
			rect.bottom = point.Y;
			break;
		case 7:
			if (point.X > rect.right && point.Y > rect.top)
			{
				nHandle = 5;
			}
			if (point.X < rect.right && point.Y < rect.top)
			{
				nHandle = 1;
			}
			if (point.X > rect.right && point.Y < rect.top)
			{
				nHandle = 3;
			}
			rect.left = point.X;
			rect.bottom = point.Y;
			break;
		case 8:
			if (point.X > rect.right)
			{
				nHandle = 4;
			}
			rect.left = point.X;
			break;
		}
		rect.NormalizeRect();
		newSize.Width = rect.Width();
		newSize.Height = rect.Height();
		newPos.X = rect.left;
		newPos.Y = rect.top;
		SetPosition(newPos);
		SetSize(newSize);
	}
}

int CDrawObject::HitTest(const Gdiplus::Point & pt)
{
	if (GetVisible() == false && GetSelected() == false)
	{
		return 0;
	}
	int nHandleCount = GetHandleCount();
	if (GetSelected())
	{
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			Gdiplus::Rect rc = GetHandleRect(nHandle);
			if (rc.Contains(pt))
			{
				return nHandle;
			}
		}
	}
	Gdiplus::Rect rect = GetObjectRect();
	if (rect.Contains(pt))
	{
		return nHandleCount + 1;
	}
	return 0;
}

bool CDrawObject::OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point)
{
	return false;
}
bool CDrawObject::OnLButtonDblClk(CWnd *pWnd, UINT nFlags, Gdiplus::Point point)
{
	return false;
}
void CDrawObject::Local2World(Gdiplus::Point * pt, int count /*=1*/ )
{
	CDrawObject *pParent = GetParent();
	Local2Global(pt, count);
	while (pParent)
	{
		pParent->Local2Global(pt, count);
		pParent = pParent->GetParent();
	}
}

void CDrawObject::Local2World( Gdiplus::Rect *rect, int count /*= 1*/ )
{
	for(int i=0; i< count; i++)
	{
		Gdiplus::Point pt;
		rect[i].GetLocation(&pt);
		Local2World(&pt);
		rect[i].X=pt.X;
		rect[i].Y=pt.Y;
	}
}

void CDrawObject::World2Local(Gdiplus::Point * pt, int count /*=1*/ )
{
	CDrawObject *pParent = GetParent();
	if (pParent)
	{
		pParent->World2Local(pt, count);
	}
	Global2Local(pt, count);
}

void CDrawObject::World2Local( Gdiplus::Rect *rect, int count /*= 1*/ )
{
	for(int i=0; i< count; i++)
	{
		Gdiplus::Point pt;
		rect[i].GetLocation(&pt);
		World2Local(&pt);
		rect[i].X=pt.X;
		rect[i].Y=pt.Y;
	}
}

void CDrawObject::Global2Local(Gdiplus::Point * pt, int count /*=1*/ )
{
	Gdiplus::Matrix matrix;
	matrix.RotateAt(-(Gdiplus::REAL) GetAngle(), Gdiplus::PointF((Gdiplus::REAL) GetRotateCenter().X, (Gdiplus::REAL) GetRotateCenter().Y));
	matrix.Translate(-(Gdiplus::REAL) GetPosition().X, -(Gdiplus::REAL) GetPosition().Y);
	matrix.TransformPoints(pt, count);
}

void CDrawObject::Global2Local( Gdiplus::Rect *rect, int count /*= 1*/ )
{
	for(int i=0; i< count; i++)
	{
		Gdiplus::Point pt;
		rect[i].GetLocation(&pt);
		Global2Local(&pt);
		rect[i].X=pt.X;
		rect[i].Y=pt.Y;
	}
}

void CDrawObject::Local2Global(Gdiplus::Point * pt, int count /*=1*/ )
{
	Gdiplus::Matrix matrix;
	matrix.Translate((Gdiplus::REAL) GetPosition().X, (Gdiplus::REAL) GetPosition().Y);
	matrix.RotateAt((Gdiplus::REAL) GetAngle(), Gdiplus::PointF((Gdiplus::REAL) GetRotateCenter().X, (Gdiplus::REAL) GetRotateCenter().Y));
	matrix.TransformPoints(pt, count);
}

void CDrawObject::Local2Global( Gdiplus::Rect *rect, int count /*= 1*/ )
{
	for(int i=0; i< count; i++)
	{
		Gdiplus::Point pt;
		rect[i].GetLocation(&pt);
		Local2Global(&pt);
		rect[i].X=pt.X;
		rect[i].Y=pt.Y;
	}
}

void CDrawObject::OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam)
{
}

LPCTSTR CDrawObject::GetObjectType() const 
{
	return GetObjectType(GetObjectTypeID());
}

LPCTSTR CDrawObject::GetObjectType( int Id )
{
	switch(Id)
	{
	case Shape:
		return _T("形状");
	case Operator:
		return _T("操作对象");
	case Tool:
		return _T("工具");
	case Mark:
		return _T("标记");
	}
	return _T("未定义");
}


void CDrawObject::Notify(UINT msgID, DWORD_PTR wParam, LPVOID lpParam)
{
	if (GetParent())
	{
		GetParent()->OnNotify(this, msgID, wParam, lpParam);
	}
}


Gdiplus::RectF CDrawObject::MeasureString( const CStringW & text, Gdiplus::Font * pFont, Gdiplus::PointF origin, Gdiplus::StringFormat *pFormat)
{
	Gdiplus::RectF retRectF;
	Gdiplus::Bitmap bmp(1,1);
	Gdiplus::Graphics graph(&bmp);
	graph.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graph.MeasureString(text, -1, pFont, origin, pFormat, &retRectF);
	return retRectF;
}

void CDrawObject::SetLineStyle( INT style )
{
	if(_appearance.LineStyle!=style)
	{
		INT s=style;
		OnLineStyleChanging(s);
		_appearance.LineStyle=s;
		OnLineStyleChanged();
	}
}

INT CDrawObject::GetLineStyle()
{
	return _appearance.LineStyle;
}

void CDrawObject::OnLineStyleChanging(INT & style )
{
    Notify(DNM_LINE_STYLE_CHANGING, 0, &style);
}

void CDrawObject::OnLineStyleChanged()
{
	Notify(DNM_LINE_STYLE_CHANGED);
}

void CDrawObject::SetActiveColor( Gdiplus::Color activeColor )
{
	if(activeColor.GetValue()!=_appearance.ActiveColor.GetValue())
	{
        Gdiplus::Color color=activeColor;
		OnActiveColorChanging(color);
		_appearance.ActiveColor=color;
		OnActiveColorChanged();
	}
}

Gdiplus::Color CDrawObject::GetActiveColor()
{
	return _appearance.ActiveColor;
}
void CDrawObject::OnActiveColorChanging(Gdiplus::Color & color)
{
	Notify(DNM_ACTIVE_COLOR_CHANGING, 0, &color);
}
void CDrawObject::OnActiveColorChanged()
{
	Notify(DNM_ACTIVE_COLOR_CHANGED);
}
void CDrawObject::SetActive( bool isActived )
{
	if(_isActive!=isActived)
	{
		bool actived=isActived;
		OnActiveChanging(actived);
		_isActive=actived;
		OnActiveChanged();
	}
}
void CDrawObject::OnActiveChanging(bool &isActived)
{
	Notify(DNM_ACTIVE_CHANGING, 0, &isActived);
}
void CDrawObject::OnActiveChanged()
{
	Notify(DNM_ACTIVE_CHANGED);
}

bool CDrawObject::GetActive()
{
	return _isActive;
}

void CDrawObject::ScaleAt( double scaleX, double scaleY, Gdiplus::Point origin )
{
	Scale(scaleX, scaleY);
}

void CDrawObject::SetFontFace( const CString & fontFace )
{
	if(_appearance.FontName!=fontFace)
	{
		CString f=fontFace;
		OnFontFaceChanging(f);
		_appearance.FontName=f;
		OnFontFaceChanged();
	}
}

CString CDrawObject::GetFontFace()
{
	return _appearance.FontName;
}

void CDrawObject::OnFontFaceChanging( CString & faceName )
{
	Notify(DNM_FONT_FACE_CHANGING, 0, &faceName);
}

void CDrawObject::OnFontFaceChanged()
{
	Notify(DNM_FONT_FACE_CHANGED);
}

void CDrawObject::SetFontStyle( INT style )
{
	if(_appearance.FontStyle !=style)
	{
		OnFontStyleChanging(style);
		_appearance.FontStyle=style;
		OnFontStyleChanged();
	}
}

void CDrawObject::OnFontStyleChanging( INT & style )
{
    Notify(DNM_FONT_STYLE_CHANGING, 0, &style);
}

void CDrawObject::OnFontStyleChanged()
{
	Notify(DNM_FONT_STYLE_CHANGED);
}

void CDrawObject::SetFontSize( FLOAT fontSize )
{
	if(_appearance.FontSize!=fontSize)
	{
		FLOAT f=fontSize;
		OnFontSizeChanging(f);
		_appearance.FontSize=f;
		OnFontSizeChanged();
	}
}

FLOAT CDrawObject::GetFontSize()
{
	return _appearance.FontSize;
}

void CDrawObject::OnFontSizeChanging( FLOAT &fontSize )
{
	Notify(DNM_FONT_SIZE_CHANGING, 0, &fontSize);
}

void CDrawObject::OnFontSizeChanged()
{
	Notify(DNM_FONT_SIZE_CHANGED);
}

void CDrawObject::SetTextAlign( INT align )
{
	if(_appearance.TextAlignment!=align)
	{
		OnTextAlignmentChanging(align);
		_appearance.TextAlignment=align;
		OnTextAlignmentChanged();
	}
}

int CDrawObject::GetTextAlign()
{
	return _appearance.TextAlignment;
}

void CDrawObject::OnTextAlignmentChanging( INT & align )
{
	Notify(DNM_TEXT_ALIGN_CHANGING, 0, &align);
}

void CDrawObject::OnTextAlignmentChanged()
{
	Notify(DNM_TEXT_ALIGN_CHANGED);
}

void CDrawObject::SetTextLineAlign( INT align )
{
	if(_appearance.LineAlignment!=align)
	{
		OnTextLineAlignChanging(align);
		_appearance.LineAlignment=align;
		OnTextLineAlignChanged();
	}
}

INT CDrawObject::GetTextLineAlign()
{
	return _appearance.LineAlignment;
}

void CDrawObject::OnTextLineAlignChanging( INT & align )
{
	Notify(DNM_TEXT_LINE_ALIGN_CHANGING, 0, &align);
}

void CDrawObject::OnTextLineAlignChanged()
{
	Notify(DNM_TEXT_LINE_ALIGN_CHANGED);
}

void CDrawObject::SetTextIsVert( bool isVert )
{
	if(_appearance.VerticalText!=isVert)
	{
		OnTextVertChanging(isVert);
		_appearance.VerticalText=isVert;
		OnTextVertChanged();
	}
}

bool CDrawObject::GetTextIsVert()
{
	return _appearance.VerticalText;
}

void CDrawObject::OnTextVertChanging( bool & isVert )
{
    Notify(DNM_TEXT_VERT_CHANGING, 0, &isVert);
}

void CDrawObject::OnTextVertChanged()
{
	Notify(DNM_TEXT_VERT_CHANGED);
}

int CDrawObject::_findIndex( std::vector<CDrawObject*> & objects, CDrawObject* pTarget )
{
	for(size_t i=0; i< objects.size(); i++)
	{
		if(pTarget==objects[i])
		{
			return i;
		}
	}
	return -1;
}

CDrawObject * CDrawObject::_findObjectByName( DRAW_OBJECT_LIST &objects, const CString & innerName )
{
    for(DRAW_OBJECT_LIST::iterator i=objects.begin(); i!= objects.end(); ++i)
	{
		if(innerName==(*i)->GetInternalName())
		{
			return *i;
		}
	}
	return nullptr;
}

int CDrawObject::GetObjectTypeID() const
{
	return Shape;
}

INT CDrawObject::GetFontStyle()
{
	return _appearance.FontStyle;
}

void CDrawObject::SetSelectable( bool isSelectable )
{
	if(isSelectable!=_bSelectable)
	{
		OnSelectableChanging(isSelectable);
		_bSelectable=isSelectable;
		OnSelectableChanged();			
	}
}

bool CDrawObject::GetSelectable()
{
	return _bSelectable;
}

void CDrawObject::OnSelectableChanging( bool & isSelectable )
{
	Notify(DNM_SELECTABLE_CHANGING,0, &isSelectable);
}

void CDrawObject::OnSelectableChanged()
{
	Notify(DNM_SELECTABLE_CHANGED);
}

void CDrawObject::SetMovable( bool isMovable )
{
	if(isMovable!=_bMovable)
	{
		OnMovableChanging(isMovable);
		_bMovable=isMovable;
		OnMovableChanged();			
	}
}

bool CDrawObject::GetMovable()
{
	return _bMovable;
}

void CDrawObject::OnMovableChanging( bool & isMovable )
{
	Notify(DNM_MOVABLE_CHANGING, 0, &isMovable);
}

void CDrawObject::OnMovableChanged()
{
	Notify(DNM_MOVABLE_CHANGED);
}


void CDrawObject::SetRotatable( bool isRotatable )
{
	if(_bRotatable!=isRotatable)
	{
		OnRotatableChanging(isRotatable);
		_bRotatable=isRotatable;
		OnRotatableChanged();

	}
}

bool CDrawObject::GetRotatable()
{
	return _bRotatable;
}

void CDrawObject::OnRotatableChanging( bool &isRotatable )
{
	Notify(DNM_ROTATABLE_CHANGING, 0, &isRotatable);
}

void CDrawObject::OnRotatableChanged()
{
	Notify(DNM_POSITION_CHANGED);
}

void CDrawObject::SetSizable( bool isSizable )
{
	if(_bSizable!=isSizable)
	{
		OnSizableChanging(isSizable);
		_bSizable=isSizable;
		OnSizableChanged();
	}
}

bool CDrawObject::GetSizable()
{
	return _bSizable;
}

void CDrawObject::OnSizableChanging( bool & isSizable )
{
	Notify(DNM_SIZABLE_CHANGING, 0, &isSizable);
}

void CDrawObject::OnSizableChanged()
{
	Notify(DNM_SIZABLE_CHANGED);
}

bool CDrawObject::IsInRect( const Gdiplus::Rect &rect )
{
	Gdiplus::Point center=GetCenter();
	Local2World(&center);
	return (center.X > rect.GetLeft() && center.X < rect.GetRight() &&
		center.Y > rect.GetTop() && center.Y < rect.GetBottom());
}

bool CDrawObject::GetWorldAngle(double *angle, Gdiplus::Point * rotateCenter)
{
	Gdiplus::Rect rect=GetObjectRect();
	Gdiplus::Point ptRightTop(rect.Width,0);
	Gdiplus::Point ptLeftTop;
	Gdiplus::Point ptCenter;
	Local2World(&ptLeftTop);
	Local2World(&ptRightTop);
	World2Local(&ptCenter);
	
	if(angle)
	{
		*angle=CMathUtility::CalculateLineAngle(ptLeftTop.X, ptLeftTop.Y, ptRightTop.X, ptRightTop.Y);
	}
	if(rotateCenter)
	{
		*rotateCenter=ptCenter;
	}
	return true;
}

void CDrawObject::SetCommonMenuId( UINT commonMenuId )
{

}

void CDrawObject::SetMenuId( UINT menuId )
{

}

UINT CDrawObject::GetMenuId()
{
    return -1;
}

CMenu * CDrawObject::GetObjectMenu( CMenu * pMenu )
{
    return NULL;
}

void CDrawObject::OnMenuCommand( UINT command )
{

}

Gdiplus::Image* CDrawObject::GetPicture( int width, int height, Gdiplus::Color background/*=Color::Transparent*/, int flag/*=GET_PIC_FLAG_ASPECT */ )
{
    //TODO BUG 1 未考虑旋转及父对象影响 2 图像实际高宽未考虑线宽影响，简单缩小4个像素
    //int w=width;
    //int h=height;
    int w=width-4;
    int h=height-4;
    int x=0;
    int y=0;
    Gdiplus::Size size;
    Gdiplus::Point pt;
    //Rect bounds=this->GetWorldBounds();
    //bounds.GetSize(&size);
    //bounds.GetLocation(&pt);
    //Point localPos(0,0);
    //Point localOffset;
    //if(GetParent())
    //{
    //    GetParent()->Local2World(&localPos);
    //    localOffset=localPos - pt;
    //}
    size=GetSize();
    pt=GetPosition();
    //float ratiox = (float)width/size.Width;
    //float ratioy = (float)height/size.Height;
    //计算比例时用缩小后的尺寸
    float ratiox = (float)w/size.Width;
    float ratioy = (float)h/size.Height;
   float ratio=(float)size.Width/size.Height;
    switch(flag & GET_PIC_FLAG_MASK)
    {
    case GET_PIC_FLAG_ASPECT:
        if(ratiox>= ratioy) 
        {
            w=(int)(size.Width * ratioy);
            //h=height;
            ratiox=ratioy;
        }
        else
        {
            //w=width;
            h=(int)(size.Height*ratiox);
            ratioy=ratiox;
        }
        break;
    case GET_PIC_FLAG_STRETCH:
        //初始化时，已计算过了
        //ratiox=(float)w/size.Height;
        //ratioy=(float)h/size.Height;
        //w=width;
        //h=height;
        break;
    case GET_PIC_FLAG_CLIP:
        ASSERT(FALSE);
        //not implemented, yet
        break;
    default:
        //not allowed with other input value. only one flag bit can set
        ASSERT(FALSE);
        break;
    }
    DWORD pos=flag & GET_PIC_POS_MASK;
    if(pos & GET_PIC_POS_LEFT)
    {
        x=0;
    } 
    else if(pos & GET_PIC_POS_RIGHT)
    {
        x=width-w;
    }
    else if(pos & GET_PIC_POS_HCENTER)
    {
        x=(width-w)/2;
    }
    if(pos & GET_PIC_POS_TOP)
    {
        y=0;
    }
    else if(pos & GET_PIC_POS_BOTTOM)
    {
        y=height-h;
    }
    else if(pos & GET_PIC_POS_VCENTER)
    {
        y=(height-h)/2;
    }

    Gdiplus::Bitmap *pBitmap=BitmapCreate(width, height, PixelFormat32bppARGB);

    Gdiplus::Graphics graph(pBitmap);
    graph.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
    graph.Clear(background);
    
    //抵消父对象偏移
    //graph.TranslateTransform(localOffset.X, localOffset.Y);
    //抵消自身偏移
    graph.TranslateTransform((float)-pt.X, (float)-pt.Y, Gdiplus::MatrixOrderAppend);
    //缩放
    graph.ScaleTransform(ratiox,ratioy, Gdiplus::MatrixOrderAppend);
    //在位图中移动
    graph.TranslateTransform((float)x,(float)y, Gdiplus::MatrixOrderAppend);
    Draw(graph);
    return pBitmap;
}

void CDrawObject::ResetContent()
{

}


