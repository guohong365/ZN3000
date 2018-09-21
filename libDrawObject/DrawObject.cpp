#include "stdafx.h"
#include <Rpc.h>
#include "DrawObject.h"
#include "ImageUtility.h"
#include "BaseTypes.h"

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
Gdiplus::Unit DrawObject::Unit = Gdiplus::UnitPixel;

void DrawObject::Initialize()
{
	_name=_T("");
	_bUsingClip = false;
	_bVisible = true;
	_isActive=false;
	_bSelected = false;	
	_pParent = nullptr;
	//_appearance.FontSize=FontSizeArray[DEFAULT_FONT_SIZE_INDEX].SizePoint*FONT_SIZE_FACETOR;

	_internalName = GenerateUniqueName();
}

DrawObject::DrawObject()
{
	Initialize();
}

DrawObject::DrawObject(const CString & name)
{
	Initialize();
	_name = name;
}
DrawObject::DrawObject(const CString & name, int x, int y, int width, int height)
{
	Initialize();
	_name = name;
	_position = Gdiplus::Point(x, y);
	_size = Gdiplus::Size(width, height);
}
DrawObject::DrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size)
{
	Initialize();
	_name = name;
	_position = point;
	_size = size;
}
DrawObject::DrawObject(const CString & name, const Gdiplus::Rect & rect)
{
	Initialize();
	_name = name;
	_position = Gdiplus::Point(rect.X, rect.Y); 
	_size =Gdiplus::Size(rect.Width, rect.Height); 
}

DrawObject::~DrawObject()
{
}

CString DrawObject::GenerateUniqueName()
{
	LARGE_INTEGER PerformanceCount;
	QueryPerformanceCounter(&PerformanceCount);

	CString name;
	name.Format(_T("%020I64u"), PerformanceCount.QuadPart);
	return name;
}


//对象名称
void DrawObject::SetName(CString name)
{
	if (_name != name)
	{
		OnNameChanging(name);
		_name = name;
		OnNameChanged();
	}
}
CString DrawObject::GetName()  
{
	return _name;
}

void DrawObject::OnNameChanging(CString & newName)
{
	Notify(DNM_NAME_CHANGING, 0, &newName);
}
void DrawObject::OnNameChanged()
{
	Notify(DNM_NAME_CHANGED);
}

//////////////////////////////////////////////////////////////////////////

//几何属性。，角度单位（度）

//坐标单位
void DrawObject::SetUnit(Gdiplus::Unit unit)
{
	DrawObject::Unit = unit;
}

Gdiplus::Unit DrawObject::GetUnit()
{
	return DrawObject::Unit;
}

//位置
void DrawObject::SetPosition(Gdiplus::Point point)
{
	if (!_position.Equals(point))
	{
		OnPositionChanging(point);
		_position = point;
		OnPositionChanged();
	}
}

Gdiplus::Point DrawObject::GetPosition() const
{
	return _position;
}

void DrawObject::OnPositionChanging(Gdiplus::Point & newPoint)
{
	Notify(DNM_POSITION_CHANGING, 0, &newPoint);
}
void DrawObject::OnPositionChanged()
{
	Notify(DNM_POSITION_CHANGED);
}

//大小
void DrawObject::SetSize(Gdiplus::Size size)
{
	if (!_size.Equals(size))
	{
		OnSizeChanging(size);
		_size = size;
		OnSizeChanged();
	}
}

Gdiplus::Size DrawObject::GetSize() const
{
	return _size;
}

void DrawObject::OnSizeChanging(Gdiplus::Size & newSize)
{
	Notify(DNM_SIZE_CHANGING, 0, &newSize);
}
void DrawObject::OnSizeChanged()
{
	Notify(DNM_SIZE_CHANGED);
}

//////////////////////////////////////////////////////////////////////////

//绘图属性

//剪切边框
void DrawObject::SetClipRect(Gdiplus::Rect rect)
{
	if (!_clipRect.Equals(rect))
	{
		OnClipRectChanging(rect);
		_clipRect = rect;
		OnClipRectChanged();
	}
}

Gdiplus::Rect DrawObject::GetClipRect() const
{
	return _clipRect;
}

void DrawObject::OnClipRectChanging(Gdiplus::Rect & newRect)
{
	Notify(DNM_CLIP_RECT_CHANGING, 0, &newRect);
}
void DrawObject::OnClipRectChanged()
{
	Notify(DNM_CLIP_RECT_CHANGED);
}

//是否剪切
void DrawObject::SetUsingClip(bool isUsingClip)
{
	if (_bUsingClip != isUsingClip)
	{
		OnUsingClipChanging(isUsingClip);
		_bUsingClip = isUsingClip;
		OnUsingClipChanged();
	}
}
bool DrawObject::GetUsingClip() const 
{
	return _bUsingClip;
}

void DrawObject::OnUsingClipChanging(bool & newUsingClip)
{
	Notify(DNM_USING_CLIP_CHANGING, 0, & newUsingClip);
}
void DrawObject::OnUsingClipChanged()
{
	Notify(DNM_USING_CLIP_CHANGED);
}

//是否显示
void DrawObject::SetVisible(bool isVisible)
{
	if (isVisible != _bVisible)
	{
		OnVisibleChanging(isVisible);
		_bVisible = isVisible;
		OnVisibleChanged();
	}
}
bool DrawObject::GetVisible() const 
{
	return _bVisible;
}

void DrawObject::OnVisibleChanging(bool & newVisible)
{
	Notify(DNM_VISIBLE_CHANGING, 0, &newVisible);
}
void DrawObject::OnVisibleChanged()
{
	Notify(DNM_VISIBLE_CHANGED);
}

//是否填充
void DrawObject::SetFilled(bool isFilled)
{
	if (_appearance.ShowFill != isFilled)
	{
		OnFilledChanging(isFilled);
		_appearance.ShowFill = isFilled;
		OnFilledChanged();
	}
}
bool DrawObject::GetFilled() const 
{
	return _appearance.ShowFill;
}

void DrawObject::OnFilledChanging(bool & newFilled)
{
	Notify(DNM_FILLED_CHANGING, 0, &newFilled);
}
void DrawObject::OnFilledChanged()
{
	Notify(DNM_FILLED_CHANGED);
}

//是否绘制边框
void DrawObject::SetShowBorder(bool isShowBorder)
{
	if (_appearance.ShowBorder != isShowBorder)
	{
		OnShowBorderChanging(isShowBorder);
		_appearance.ShowBorder = isShowBorder;
		OnVisibleChanged();
	}
}
bool DrawObject::GetShowBorder() const 
{
	return _appearance.ShowBorder;
}

void DrawObject::OnShowBorderChanging(bool & newShowBorder)
{
	Notify(DNM_SHOW_BORDER_CHANGING, 0, &newShowBorder);
}
void DrawObject::OnShowBorderChanged()
{
	Notify(DNM_SHOW_BORDER_CHANGED);
}
void DrawObject::SetShowTracker(bool isShowTracker)
{
	if (isShowTracker != _appearance.ShowTrack)
	{
		OnShowTrackerChanging(isShowTracker);
		_appearance.ShowTrack = isShowTracker;
		OnShowTrackerChanged();
	}
}
bool DrawObject::GetShowTracker() const 
{
	return _appearance.ShowTrack;
}

void DrawObject::OnShowTrackerChanging(bool & newShowTracker)
{
	Notify(DNM_SHOW_TRACKER_CHANGING, 0, &newShowTracker);
}
void DrawObject::OnShowTrackerChanged()
{
	Notify(DNM_SHOW_TRACKER_CHANGED);
}

void DrawObject::SetTrackerColor(Gdiplus::Color color)
{
	if (color.GetValue() != _appearance.TraceColor.GetValue())
	{
		OnTrackerColorChanging(color);
		_appearance.TraceColor = color;
		OnTrackerColorChanged();
	}
}

Gdiplus::Color DrawObject::GetTrackerColor() const
{
	return _appearance.TraceColor;
}

void DrawObject::OnTrackerColorChanging(Gdiplus::Color & color)
{
	Notify(DNM_TRACKER_COLOR_CHANGING, 0,&color);
}
void DrawObject::OnTrackerColorChanged()
{
	Notify(DNM_TRACKER_COLOR_CHANGED);
}

//透明度
void DrawObject::SetTransparent(float transparent)
{
	if (_appearance.Transparent != transparent)
	{
		OnTransparentChanging(transparent);
		_appearance.Transparent = transparent;
		OnTransparentChanged();
	}
}
float DrawObject::GetTransparent() const 
{
	return _appearance.Transparent;
}
void DrawObject::OnTransparentChanging(float &newTransparent)
{
	Notify(DNM_TRANSPARENT_CHANGING, 0, &newTransparent);
}
void DrawObject::OnTransparentChanged()
{
	Notify(DNM_TRANSPARENT_CHANGED);
}


//填充颜色
void DrawObject::SetFillColor(Gdiplus::Color color)
{
	if (_appearance.FillColor.GetValue() != color.GetValue())
	{
		OnFillColorChanging(color);
		_appearance.FillColor = color;
		OnFillColorChanged();
	}
}

Gdiplus::Color DrawObject::GetFillColor() const
{
	return _appearance.FillColor;
}

void DrawObject::OnFillColorChanging(Gdiplus::Color & newColor)
{
	Notify(DNM_FILL_COLOR_CHANGING, 0, &newColor);
}
void DrawObject::OnFillColorChanged()
{
	Notify(DNM_FILL_COLOR_CHANGED);
}

//边框颜色
void DrawObject::SetLineColor(Gdiplus::Color color)
{
	if (_appearance.LineColor.GetValue() != color.GetValue())
	{
		OnLineColorChanging(color);
		_appearance.LineColor = color;
		OnLineColorChanged();
	}
}

Gdiplus::Color DrawObject::GetLineColor() const
{
	return _appearance.LineColor;
}

void DrawObject::OnLineColorChanging(Gdiplus::Color & newColor)
{
	Notify(DNM_LINE_COLOR_CHANGING, 0, &newColor);
}
void DrawObject::OnLineColorChanged()
{
	Notify(DNM_LINE_COLOR_CHANGED);
}

void DrawObject::SetLineWidth(float width)
{
	if (_appearance.LineWidth != width)
	{
		OnLineWidthChanging(width);
		_appearance.LineWidth = width;
		OnLineWidthChanged();
	}
}
float DrawObject::GetLineWidth()
{
	return _appearance.LineWidth;
}
void DrawObject::OnLineWidthChanging(float &width)
{
	Notify(DNM_LINE_WIDTH_CHANGING, 0, &width);
}
void DrawObject::OnLineWidthChanged()
{
	Notify(DNM_LINE_WIDTH_CHANGED);
}

//字体颜色
void DrawObject::SetFontColor(Gdiplus::Color color)
{
	if (_appearance.FontColor.GetValue() != color.GetValue())
	{
		OnFontColorChanging(color);
		_appearance.FontColor = color;
		OnFontColorChanged();
	}
}

Gdiplus::Color DrawObject::GetFontColor() const
{
	return _appearance.FontColor;
}

void DrawObject::OnFontColorChanging(Gdiplus::Color & newFontColor)
{
	Notify(DNM_FONT_COLOR_CHANGING, 0, &newFontColor);
}
void DrawObject::OnFontColorChanged()
{
	Notify(DNM_FONT_COLOR_CHANGED);
}

//////////////////////////////////////////////////////////////////////////

//控制属性

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
void DrawObject::SetParent(DrawObject * pParent)
{
	if (_pParent != pParent)
	{
		OnParentChanging(pParent);
		_pParent = pParent;
		OnParentChanged();
	}
}
const DrawObject *DrawObject::GetParent() const 
{
	return _pParent;
}

DrawObject *DrawObject::GetParent()
{
	return _pParent;
}

void DrawObject::OnParentChanging(DrawObject * &pObject)
{
	Notify(DNM_PARENT_CHANGING, 0, &pObject);
}
void DrawObject::OnParentChanged()
{
	Notify(DNM_PARENT_CHANGED);
}

LOGFONTW OBJECT_DEFAULT_FONT = { 0 };

//////////////////////////////////////////////////////////////////////////
//撤销列表支持
//////////////////////////////////////////////////////////////////////////
//复制对象
//@param source 复制source对象当前数据到本对象，不复制撤销列表。
DrawObject *DrawObject::CopyFrom(DrawObject * source)
{
	_name = source->_name;
	_position = source->_position;
	_size = source->_size;
	_clipRect = source->_clipRect;
	_bUsingClip = source->_bUsingClip;
	_bVisible = source->_bVisible;
	_appearance=source->_appearance;
	_bSelected = source->_bSelected;
	return this;
}


//创建副本
//分配新的对象空间，复制当前数据到新对象，但不复制撤销列表。
DrawObject *DrawObject::Clone()
{
	DrawObject *pObject = CreateInstance();
	ASSERT(pObject);
	pObject->CopyFrom(this);
	return pObject;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//绘图支持
//////////////////////////////////////////////////////////////////////////

void DrawObject::Draw(Gdiplus::Graphics & graph)
{
	if (!GetVisible())
	{
		return;
	}
	graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graph.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	graph.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	Gdiplus::Matrix matrix;
	Gdiplus::Matrix oldMatrix;
	graph.GetTransform(&oldMatrix);
	graph.GetTransform(&matrix);
	matrix.Translate(Gdiplus::REAL(GetPosition().X), Gdiplus::REAL(GetPosition().Y));
	graph.SetTransform(&matrix);
	Gdiplus::Region clip;
	Gdiplus::Rect rf;
	bool setClip = false;
	if (!graph.IsClipEmpty())
	{
		if (GetUsingClip())
		{
			graph.GetClip(&clip);
			clip.GetBounds(&rf, &graph);
			graph.SetClip(_clipRect, Gdiplus::CombineModeIntersect);

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
	graph.SetTransform(&oldMatrix);

}
void DrawObject::OnDraw(Gdiplus::Graphics & graph)
{
	DrawFillObject(graph);
}

void DrawObject::DrawBorder(Gdiplus::Graphics & graph)
{
	if ((GetShowBorder() || GetActive()))
	{
		OnDrawBorder(graph);
	}
}
void DrawObject::OnDrawBorder(Gdiplus::Graphics & graph)
{

}

void DrawObject::DrawFillObject(Gdiplus::Graphics & graph)
{
	if (GetFilled())
	{
		OnDrawFillObject(graph);
	}
}
void DrawObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{

}

//////////////////////////////////////////////////////////////////////////
//几何操作
//////////////////////////////////////////////////////////////////////////
Gdiplus::Point DrawObject::GetCenter()
{
	Gdiplus::Rect rect = GetObjectRect();
	return Gdiplus::Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
}


Gdiplus::Rect DrawObject::GetObjectRect()
{
	return Gdiplus::Rect(Gdiplus::Point(0, 0), GetSize());
}

void DrawObject::GetObjectRegion(Gdiplus::GraphicsPath & region)
{
	region.Reset();
	region.AddRectangle(GetObjectRect());
}


Gdiplus::Rect DrawObject::GetBounds()
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

Gdiplus::Rect DrawObject::GetWorldBounds()
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


void DrawObject::MoveTo(Gdiplus::Point point)
{
	SetPosition(point);
}
void DrawObject::MoveTo(int x, int y)
{
	SetPosition(Gdiplus::Point(x, y));
}
void DrawObject::Offset(int x, int y)
{
	Offset(Gdiplus::Point(x, y));
}

void DrawObject::Offset(Gdiplus::Point point)
{
	SetPosition(_position + point);
}

void DrawObject::Scale(double scaleX, double scaleY)
{
	Gdiplus::Size size;
	size.Width = int(_size.Width * scaleX);
	size.Height = int(_size.Height * scaleY);
	SetSize(size);
}

int DrawObject::GetHandleCount()
{
	return 8;
}

int DrawObject::HitTest(const Gdiplus::Point & pt)
{
	if (!GetVisible())
	{
		return 0;
	}
	const int nHandleCount = GetHandleCount();

	Gdiplus::Rect rect = GetObjectRect();
	if (rect.Contains(pt))
	{
		return nHandleCount + 1;
	}
	return 0;
}

bool DrawObject::OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point)
{
	return false;
}
bool DrawObject::OnLButtonDblClk(CWnd *pWnd, UINT nFlags, Gdiplus::Point point)
{
	return false;
}
void DrawObject::Local2World(Gdiplus::Point * pt, int count /*=1*/ )
{
	DrawObject *pParent = GetParent();
	Local2Global(pt, count);
	while (pParent)
	{
		pParent->Local2Global(pt, count);
		pParent = pParent->GetParent();
	}
}

void DrawObject::Local2World( Gdiplus::Rect *rect, int count /*= 1*/ )
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

void DrawObject::World2Local(Gdiplus::Point * pt, int count /*=1*/ )
{
	DrawObject *pParent = GetParent();
	if (pParent)
	{
		pParent->World2Local(pt, count);
	}
	Global2Local(pt, count);
}

void DrawObject::World2Local( Gdiplus::Rect *rect, int count /*= 1*/ )
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

void DrawObject::Global2Local(Gdiplus::Point * pt, int count /*=1*/ )
{
	Gdiplus::Matrix matrix;
	matrix.Translate(-Gdiplus::REAL(GetPosition().X), -(Gdiplus::REAL) GetPosition().Y);
	matrix.TransformPoints(pt, count);
}

void DrawObject::Global2Local( Gdiplus::Rect *rect, int count /*= 1*/ )
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

void DrawObject::Local2Global(Gdiplus::Point * pt, int count /*=1*/ )
{
	Gdiplus::Matrix matrix;
	matrix.Translate(Gdiplus::REAL(GetPosition().X), Gdiplus::REAL(GetPosition().Y));
	matrix.TransformPoints(pt, count);
}

void DrawObject::Local2Global( Gdiplus::Rect *rect, int count /*= 1*/ )
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

void DrawObject::OnNotify(DrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam)
{
}

void DrawObject::Notify(UINT msgID, DWORD_PTR wParam, LPVOID lpParam)
{
	if (GetParent())
	{
		GetParent()->OnNotify(this, msgID, wParam, lpParam);
	}
}


Gdiplus::RectF DrawObject::MeasureString( const CStringW & text, Gdiplus::Font * pFont, Gdiplus::PointF origin, Gdiplus::StringFormat *pFormat)
{
	Gdiplus::RectF retRectF;
	Gdiplus::Bitmap bmp(1,1);
	Gdiplus::Graphics graph(&bmp);
	graph.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	graph.MeasureString(text, -1, pFont, origin, pFormat, &retRectF);
	return retRectF;
}

void DrawObject::SetLineStyle( INT style )
{
	if(_appearance.LineStyle!=style)
	{
		INT s=style;
		OnLineStyleChanging(s);
		_appearance.LineStyle=s;
		OnLineStyleChanged();
	}
}

INT DrawObject::GetLineStyle()
{
	return _appearance.LineStyle;
}

void DrawObject::OnLineStyleChanging(INT & style )
{
	Notify(DNM_LINE_STYLE_CHANGING, 0, &style);
}

void DrawObject::OnLineStyleChanged()
{
	Notify(DNM_LINE_STYLE_CHANGED);
}

void DrawObject::SetActiveColor( Gdiplus::Color activeColor )
{
	if(activeColor.GetValue()!=_appearance.ActiveColor.GetValue())
	{
		Gdiplus::Color color=activeColor;
		OnActiveColorChanging(color);
		_appearance.ActiveColor=color;
		OnActiveColorChanged();
	}
}

Gdiplus::Color DrawObject::GetActiveColor()
{
	return _appearance.ActiveColor;
}
void DrawObject::OnActiveColorChanging(Gdiplus::Color & color)
{
	Notify(DNM_ACTIVE_COLOR_CHANGING, 0, &color);
}
void DrawObject::OnActiveColorChanged()
{
	Notify(DNM_ACTIVE_COLOR_CHANGED);
}
void DrawObject::SetActive( bool isActived )
{
	if(_isActive!=isActived)
	{
		bool actived=isActived;
		OnActiveChanging(actived);
		_isActive=actived;
		OnActiveChanged();
	}
}
void DrawObject::OnActiveChanging(bool &isActived)
{
	Notify(DNM_ACTIVE_CHANGING, 0, &isActived);
}
void DrawObject::OnActiveChanged()
{
	Notify(DNM_ACTIVE_CHANGED);
}

bool DrawObject::GetActive()
{
	return _isActive;
}

void DrawObject::ScaleAt( double scaleX, double scaleY, Gdiplus::Point origin )
{
	Scale(scaleX, scaleY);
}

void DrawObject::SetFontFace( const CString & fontFace )
{
	if(_appearance.FontName!=fontFace)
	{
		CString f=fontFace;
		OnFontFaceChanging(f);
		_appearance.FontName=f;
		OnFontFaceChanged();
	}
}

CString DrawObject::GetFontFace()
{
	return _appearance.FontName;
}

void DrawObject::OnFontFaceChanging( CString & faceName )
{
	Notify(DNM_FONT_FACE_CHANGING, 0, &faceName);
}

void DrawObject::OnFontFaceChanged()
{
	Notify(DNM_FONT_FACE_CHANGED);
}

void DrawObject::SetFontStyle( INT style )
{
	if(_appearance.FontStyle !=style)
	{
		OnFontStyleChanging(style);
		_appearance.FontStyle=style;
		OnFontStyleChanged();
	}
}

void DrawObject::OnFontStyleChanging( INT & style )
{
	Notify(DNM_FONT_STYLE_CHANGING, 0, &style);
}

void DrawObject::OnFontStyleChanged()
{
	Notify(DNM_FONT_STYLE_CHANGED);
}

void DrawObject::SetFontSize( FLOAT fontSize )
{
	if(_appearance.FontSize!=fontSize)
	{
		FLOAT f=fontSize;
		OnFontSizeChanging(f);
		_appearance.FontSize=f;
		OnFontSizeChanged();
	}
}

FLOAT DrawObject::GetFontSize()
{
	return _appearance.FontSize;
}

void DrawObject::OnFontSizeChanging( FLOAT &fontSize )
{
	Notify(DNM_FONT_SIZE_CHANGING, 0, &fontSize);
}

void DrawObject::OnFontSizeChanged()
{
	Notify(DNM_FONT_SIZE_CHANGED);
}

void DrawObject::SetTextAlign( INT align )
{
	if(_appearance.TextAlignment!=align)
	{
		OnTextAlignmentChanging(align);
		_appearance.TextAlignment=align;
		OnTextAlignmentChanged();
	}
}

int DrawObject::GetTextAlign()
{
	return _appearance.TextAlignment;
}

void DrawObject::OnTextAlignmentChanging( INT & align )
{
	Notify(DNM_TEXT_ALIGN_CHANGING, 0, &align);
}

void DrawObject::OnTextAlignmentChanged()
{
	Notify(DNM_TEXT_ALIGN_CHANGED);
}

void DrawObject::SetTextLineAlign( INT align )
{
	if(_appearance.LineAlignment!=align)
	{
		OnTextLineAlignChanging(align);
		_appearance.LineAlignment=align;
		OnTextLineAlignChanged();
	}
}

INT DrawObject::GetTextLineAlign()
{
	return _appearance.LineAlignment;
}

void DrawObject::OnTextLineAlignChanging( INT & align )
{
	Notify(DNM_TEXT_LINE_ALIGN_CHANGING, 0, &align);
}

void DrawObject::OnTextLineAlignChanged()
{
	Notify(DNM_TEXT_LINE_ALIGN_CHANGED);
}

void DrawObject::SetTextIsVertical( bool isVert )
{
	if(_appearance.VerticalText!=isVert)
	{
		OnTextVerticalChanging(isVert);
		_appearance.VerticalText=isVert;
		OnTextVerticalChanged();
	}
}

bool DrawObject::GetTextIsVertical()
{
	return _appearance.VerticalText;
}

void DrawObject::OnTextVerticalChanging( bool & isVert )
{
	Notify(DNM_TEXT_VERT_CHANGING, 0, &isVert);
}

void DrawObject::OnTextVerticalChanged()
{
	Notify(DNM_TEXT_VERT_CHANGED);
}

INT DrawObject::GetFontStyle()
{
	return _appearance.FontStyle;
}

bool DrawObject::IsInRect( const Gdiplus::Rect &rect )
{
	Gdiplus::Point center=GetCenter();
	Local2World(&center);
	return (center.X > rect.GetLeft() && center.X < rect.GetRight() &&
		center.Y > rect.GetTop() && center.Y < rect.GetBottom());
}

Gdiplus::Image* DrawObject::GetPicture( int width, int height, Gdiplus::Color background/*=Color::Transparent*/, DWORD flag/*=GET_PIC_FLAG_ASPECT */ )
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

void DrawObject::ResetContent()
{
}


