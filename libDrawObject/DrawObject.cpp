#include "stdafx.h"
#include <Rpc.h>
#include "DrawObject.h"
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

Gdiplus::Unit DrawObject::Unit = Gdiplus::UnitPixel;
Appearance DrawObject::_basicAppearance;

void DrawObject::_initialize()
{
	_pAppearance=new Appearance(_basicAppearance);
	_name=_T("");
	_bUsingClip = false;
	_bVisible = true;
	_isActive=false;
	_bSelected = false;	
	_pParent = nullptr;
	//GetAppearance().FontSize=FontSizeArray[DEFAULT_FONT_SIZE_INDEX].SizePoint*FONT_SIZE_FACTOR;

	_internalName = GenerateUniqueName();
}

DrawObject::DrawObject()
{
	_initialize();
}

DrawObject::DrawObject(const CString & name)
{
	_initialize();
	_name = name;
}
DrawObject::DrawObject(const CString & name, int x, int y, int width, int height)
{
	_initialize();
	_name = name;
	_position = Gdiplus::Point(x, y);
	_size = Gdiplus::Size(width, height);
}
DrawObject::DrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size)
{
	_initialize();
	_name = name;
	_position = point;
	_size = size;
}
DrawObject::DrawObject(const CString & name, const Gdiplus::Rect & rect)
{
	_initialize();
	_name = name;
	_position = Gdiplus::Point(rect.X, rect.Y); 
	_size =Gdiplus::Size(rect.Width, rect.Height); 
}

DrawObject::~DrawObject()
{
	delete _pAppearance;
}

CString DrawObject::GenerateUniqueName()
{
	LARGE_INTEGER PerformanceCount;
	QueryPerformanceCounter(&PerformanceCount);

	CString name;
	name.Format(_T("%020I64u"), PerformanceCount.QuadPart);
	return name;
}

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

void DrawObject::SetUnit(Gdiplus::Unit unit)
{
	Unit = unit;
}

Gdiplus::Unit DrawObject::GetUnit()
{
	return Unit;
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

void DrawObject::SetAppearance(Appearance& appearance)
{
	delete _pAppearance;
	_pAppearance=appearance.Clone();
}

Appearance& DrawObject::GetAppearance()
{
	return *_pAppearance;
}

Appearance& DrawObject::GetAppearance() const
{
	return *_pAppearance;
}


//////////////////////////////////////////////////////////////////////////

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

void DrawObject::SetFilled(bool isFilled)
{
	if (GetAppearance().ShowFill != isFilled)
	{
		OnFilledChanging(isFilled);
		GetAppearance().ShowFill = isFilled;
		OnFilledChanged();
	}
}
bool DrawObject::GetFilled() const 
{
	return GetAppearance().ShowFill;
}

void DrawObject::OnFilledChanging(bool & newFilled)
{
	Notify(DNM_FILLED_CHANGING, 0, &newFilled);
}
void DrawObject::OnFilledChanged()
{
	Notify(DNM_FILLED_CHANGED);
}

void DrawObject::SetShowBorder(bool isShowBorder)
{
	if (GetAppearance().ShowBorder != isShowBorder)
	{
		OnShowBorderChanging(isShowBorder);
		GetAppearance().ShowBorder = isShowBorder;
		OnVisibleChanged();
	}
}
bool DrawObject::GetShowBorder() const 
{
	return GetAppearance().ShowBorder;
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
	if (isShowTracker != GetAppearance().ShowTrack)
	{
		OnShowTrackerChanging(isShowTracker);
		GetAppearance().ShowTrack = isShowTracker;
		OnShowTrackerChanged();
	}
}
bool DrawObject::GetShowTracker() const 
{
	return GetAppearance().ShowTrack;
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
	if (color.GetValue() != GetAppearance().TraceColor)
	{
		OnTrackerColorChanging(color);
		GetAppearance().TraceColor = color.GetValue();
		OnTrackerColorChanged();
	}
}

Gdiplus::Color DrawObject::GetTrackerColor() const
{
	return GetAppearance().TraceColor;
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
	if (GetAppearance().Transparent != transparent)
	{
		OnTransparentChanging(transparent);
		GetAppearance().Transparent = transparent;
		OnTransparentChanged();
	}
}
float DrawObject::GetTransparent() const 
{
	return GetAppearance().Transparent;
}
void DrawObject::OnTransparentChanging(float &newTransparent)
{
	Notify(DNM_TRANSPARENT_CHANGING, 0, &newTransparent);
}
void DrawObject::OnTransparentChanged()
{
	Notify(DNM_TRANSPARENT_CHANGED);
}


void DrawObject::SetFillColor(Gdiplus::Color color)
{
	if (GetAppearance().FillColor != color.GetValue())
	{
		OnFillColorChanging(color);
		GetAppearance().FillColor = color.GetValue();
		OnFillColorChanged();
	}
}

Gdiplus::Color DrawObject::GetFillColor() const
{
	return GetAppearance().FillColor;
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
	if (GetAppearance().LineColor != color.GetValue())
	{
		OnLineColorChanging(color);
		GetAppearance().LineColor = color.GetValue();
		OnLineColorChanged();
	}
}

Gdiplus::Color DrawObject::GetLineColor() const
{
	return GetAppearance().LineColor;
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
	if (GetAppearance().LineWidth != width)
	{
		OnLineWidthChanging(width);
		GetAppearance().LineWidth = width;
		OnLineWidthChanged();
	}
}
float DrawObject::GetLineWidth()
{
	return GetAppearance().LineWidth;
}
void DrawObject::OnLineWidthChanging(float &width)
{
	Notify(DNM_LINE_WIDTH_CHANGING, 0, &width);
}
void DrawObject::OnLineWidthChanged()
{
	Notify(DNM_LINE_WIDTH_CHANGED);
}

void DrawObject::SetFontColor(Gdiplus::Color color)
{
	if (GetAppearance().FontColor != color.GetValue())
	{
		OnFontColorChanging(color);
		GetAppearance().FontColor = color.GetValue();
		OnFontColorChanged();
	}
}

Gdiplus::Color DrawObject::GetFontColor() const
{
	return GetAppearance().FontColor;
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

DrawObject *DrawObject::CopyFrom(DrawObject * source)
{
	_name = source->_name;
	_position = source->_position;
	_size = source->_size;
	_clipRect = source->_clipRect;
	_bUsingClip = source->_bUsingClip;
	_bVisible = source->_bVisible;
	_pAppearance=source->_pAppearance->Clone();
	_bSelected = source->_bSelected;
	return this;
}

DrawObject *DrawObject::Clone()
{
	DrawObject *pObject = CreateInstance();
	ASSERT(pObject);
	pObject->CopyFrom(this);
	return pObject;
}

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
	DrawFillObject(graph);
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

Gdiplus::Point DrawObject::GetCenter()
{
	const Gdiplus::Rect rect = GetObjectRect();
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
	const Gdiplus::Rect rect=GetObjectRect();
	Gdiplus::Point pt[4];
	pt[0] =Gdiplus::Point(rect.X,rect.Y);
	pt[1] = Gdiplus::Point(rect.X, rect.Y + rect.Height);
	pt[2] = Gdiplus::Point(rect.X + rect.Width,rect.Y);
	pt[3] = Gdiplus::Point(rect.X + rect.Width, rect.Y + rect.Height);
	Local2Global(pt, 4);
	int maxX;
	int minX = maxX = pt[0].X;
	int maxY;
	int minY = maxY = pt[0].Y;
	for (int i = 1; i < 4; i++)
	{
		if (minX > pt[i].X)
			minX = pt[i].X;
		if (maxX < pt[i].X)
			maxX = pt[i].X;
		if (minY > pt[i].Y)
			minY = pt[i].Y;
		if (maxY < pt[i].Y)
			maxY = pt[i].Y;
	}
	return Gdiplus::Rect(minX, minY, maxX - minX, maxY - minY);
}

Gdiplus::Rect DrawObject::GetWorldBounds()
{
	const Gdiplus::Rect rect=GetObjectRect();
	Gdiplus::Point pt[4];
	pt[0] =Gdiplus::Point(rect.X,rect.Y);
	pt[1] = Gdiplus::Point(rect.X, rect.Y + rect.Height);
	pt[2] = Gdiplus::Point(rect.X + rect.Width,rect.Y);
	pt[3] = Gdiplus::Point(rect.X + rect.Width, rect.Y + rect.Height);
	Local2World(pt, 4);
	int maxX;
	int minX = maxX = pt[0].X;
	int maxY;
	int minY = maxY = pt[0].Y;
	for (int i = 1; i < 4; i++)
	{
		if (minX > pt[i].X)
			minX = pt[i].X;
		if (maxX < pt[i].X)
			maxX = pt[i].X;
		if (minY > pt[i].Y)
			minY = pt[i].Y;
		if (maxY < pt[i].Y)
			maxY = pt[i].Y;
	}
	return Gdiplus::Rect(minX, minY, maxX - minX, maxY - minY);
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
	if(GetAppearance().LineStyle!=style)
	{
		INT s=style;
		OnLineStyleChanging(s);
		GetAppearance().LineStyle=s;
		OnLineStyleChanged();
	}
}

INT DrawObject::GetLineStyle()
{
	return GetAppearance().LineStyle;
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
	if(activeColor.GetValue()!=GetAppearance().ActiveColor)
	{
		Gdiplus::Color color=activeColor;
		OnActiveColorChanging(color);
		GetAppearance().ActiveColor=color.GetValue();
		OnActiveColorChanged();
	}
}

Gdiplus::Color DrawObject::GetActiveColor()
{
	return GetAppearance().ActiveColor;
}
void DrawObject::OnActiveColorChanging(Gdiplus::Color & color)
{
	Notify(DNM_ACTIVE_COLOR_CHANGING, 0, &color);
}
void DrawObject::OnActiveColorChanged()
{
	Notify(DNM_ACTIVE_COLOR_CHANGED);
}
void DrawObject::SetActive(const bool isActived )
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
	if(GetAppearance().FontName!=fontFace)
	{
		CString f=fontFace;
		OnFontFaceChanging(f);
		GetAppearance().FontName=f;
		OnFontFaceChanged();
	}
}

CString DrawObject::GetFontFace()
{
	return GetAppearance().FontName;
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
	if(GetAppearance().FontStyle !=style)
	{
		OnFontStyleChanging(style);
		GetAppearance().FontStyle=style;
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
	if(GetAppearance().FontSize!=fontSize)
	{
		FLOAT f=fontSize;
		OnFontSizeChanging(f);
		GetAppearance().FontSize=f;
		OnFontSizeChanged();
	}
}

FLOAT DrawObject::GetFontSize()
{
	return GetAppearance().FontSize;
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
	if(GetAppearance().TextAlignment!=align)
	{
		OnTextAlignmentChanging(align);
		GetAppearance().TextAlignment=align;
		OnTextAlignmentChanged();
	}
}

int DrawObject::GetTextAlign()
{
	return GetAppearance().TextAlignment;
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
	if(GetAppearance().LineAlignment!=align)
	{
		OnTextLineAlignChanging(align);
		GetAppearance().LineAlignment=align;
		OnTextLineAlignChanged();
	}
}

INT DrawObject::GetTextLineAlign()
{
	return GetAppearance().LineAlignment;
}

void DrawObject::OnTextLineAlignChanging( INT & align )
{
	Notify(DNM_TEXT_LINE_ALIGN_CHANGING, 0, &align);
}

void DrawObject::OnTextLineAlignChanged()
{
	Notify(DNM_TEXT_LINE_ALIGN_CHANGED);
}

void DrawObject::SetTextIsVertical( bool isVertical )
{
	if(GetAppearance().VerticalText!=isVertical)
	{
		OnTextVerticalChanging(isVertical);
		GetAppearance().VerticalText=isVertical;
		OnTextVerticalChanged();
	}
}

bool DrawObject::GetTextIsVertical()
{
	return GetAppearance().VerticalText;
}

void DrawObject::OnTextVerticalChanging( bool & isVertical )
{
	Notify(DNM_TEXT_VERT_CHANGING, 0, &isVertical);
}

void DrawObject::OnTextVerticalChanged()
{
	Notify(DNM_TEXT_VERT_CHANGED);
}

INT DrawObject::GetFontStyle()
{
	return GetAppearance().FontStyle;
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
	const Gdiplus::Size size = GetSize();
	const Gdiplus::Point pt = GetPosition();
	//float ratiox = (float)width/size.Width;
	//float ratioy = (float)height/size.Height;
	//计算比例时用缩小后的尺寸
	float ratioX = float(w)/size.Width;
	float ratioY = float(h)/size.Height;
	float ratio=float(size.Width)/size.Height;
	switch(flag & GET_PIC_FLAG_MASK)
	{
	case GET_PIC_FLAG_ASPECT:
		if(ratioX>= ratioY) 
		{
			w=int(size.Width * ratioY);
			//h=height;
			ratioX=ratioY;
		}
		else
		{
			//w=width;
			h=int(size.Height * ratioX);
			ratioY=ratioX;
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
	const DWORD pos=flag & GET_PIC_POS_MASK;
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
	graph.TranslateTransform(float(-pt.X), float(-pt.Y), Gdiplus::MatrixOrderAppend);
	//缩放
	graph.ScaleTransform(ratioX,ratioY, Gdiplus::MatrixOrderAppend);
	//在位图中移动
	graph.TranslateTransform(float(x),float(y), Gdiplus::MatrixOrderAppend);
	Draw(graph);
	return pBitmap;
}

void DrawObject::ResetContent()
{
}


