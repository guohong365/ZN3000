// Layout.cpp : 实现文件
//

#include "stdafx.h"
#include "ObjectsContainer.h"
#include "History.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;


IMPLEMENT_SERIAL(CObjectsContainer, CRectObject, 0)
void CObjectsContainer::Initialize()
{
	m_RulerUnitPerPixel = 0.0;
	m_Options = 0;
	m_pHorizalRuler = NULL;
	m_pVerticalRuler = NULL;
	m_bShowNameOnBackground = false;
}

CObjectsContainer::CObjectsContainer()
{
	Initialize();
}

CObjectsContainer::CObjectsContainer(const CString & name, const Point & pt, const Size & size, DWORD options)
:CRectObject(name, pt, size)
{
    Initialize();
    m_Options = options;
    if ((options & WITH_HORIZ_RULER) != 0)
    {
        m_pHorizalRuler = new CRulerObject(_T("HORIZ"),
                                           Point(RULER_OFFSET, RULER_OFFSET),
                                           Size(size.Width - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT));
        m_pHorizalRuler->SetLineColor(Color::Black);
        m_pHorizalRuler->SetVisible(true);
        m_pHorizalRuler->SetParent(this);
        m_pHorizalRuler->SetSelectable(false);
    }
    if ((options & WITH_VERT_RULER) != 0)
    {
        m_pVerticalRuler = new CRulerObject(_T("VERT"),
                                            Point(RULER_OFFSET + HORIZ_RULER_HEIGHT, RULER_OFFSET),
                                            Size(size.Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT));
        m_pVerticalRuler->SetGraduationPosition(BaselineAtBottom);
        m_pVerticalRuler->SetAngle(90);;
        m_pVerticalRuler->SetLineColor(Color::Black);
        m_pVerticalRuler->SetVisible(true);
        m_pVerticalRuler->SetParent(this);
        m_pVerticalRuler->SetSelectable(false);

    }
    SetUsingClip(true);
    Rect rect(0, 0, size.Width, size.Height);
    rect.Inflate(1, 1);
    SetClipRect(rect);
}

CObjectsContainer::CObjectsContainer(const CString & name, const Rect & rect, DWORD options)
:CRectObject(name, rect)
{
    Initialize();
    if ((options & WITH_HORIZ_RULER) != 0)
    {
        m_pHorizalRuler = new CRulerObject(_T("HORIZ"),
                                           Point(RULER_OFFSET, RULER_OFFSET),
                                           Size(rect.Width - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT));
        m_pHorizalRuler->SetLineColor(Color::Black);
        m_pHorizalRuler->SetVisible(true);
        m_pHorizalRuler->SetParent(this);
    }
    if ((options & WITH_VERT_RULER) != 0)
    {
        m_pVerticalRuler = new CRulerObject(_T("VERT"),
                                            Point(RULER_OFFSET + HORIZ_RULER_HEIGHT, RULER_OFFSET),
                                            Size(rect.Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT));
        m_pVerticalRuler->SetGraduationPosition(BaselineAtBottom);
        m_pVerticalRuler->SetAngle(90);;
        m_pVerticalRuler->SetLineColor(Color::Black);
        m_pVerticalRuler->SetVisible(true);
        m_pVerticalRuler->SetParent(this);
    }
    SetUsingClip(true);
    SetClipRect(Rect(0, 0, rect.Width, rect.Height));
}

CObjectsContainer::~CObjectsContainer()
{
    Clear();
    delete m_pHorizalRuler;
    delete m_pVerticalRuler;
}

void CObjectsContainer::ResetContent()
{
    Clear();
    SetRulerScale(0.0);
}


void CObjectsContainer::SetRulerUnit(int nRuler, int unit)
{
    if (nRuler == HORIZ_RULER && m_pHorizalRuler)
    {
        m_pHorizalRuler->SetCurrentUnit(unit);
    }
    if (nRuler == VERT_RULER && m_pVerticalRuler)
    {
        m_pVerticalRuler->SetCurrentUnit(unit);
    }
}
int CObjectsContainer::GetRulerUnit(int nRuler) const 
{
    if (m_pHorizalRuler && HORIZ_RULER)
    {
        return m_pHorizalRuler->GetCurrentUnit();
    }
    if (m_pVerticalRuler && VERT_RULER)
    {
        return m_pVerticalRuler->GetCurrentUnit();
    }
    return -1;
}

void CObjectsContainer::SetRulerScale(double unitPerPixel)
{
    if (m_RulerUnitPerPixel != unitPerPixel)
    {
        OnRulerScaleChanging(unitPerPixel);
        m_RulerUnitPerPixel = unitPerPixel;
        if(m_pVerticalRuler)
			m_pVerticalRuler->SetUnitPerPixel(unitPerPixel);
        if(m_pHorizalRuler)
			m_pHorizalRuler->SetUnitPerPixel(unitPerPixel);
        OnRulerScaleChanged();
    }
}
double CObjectsContainer::GetRulerScale() const 
{
    return m_RulerUnitPerPixel;
}
void CObjectsContainer::OnRulerScaleChanging(double &newUnitPerPixel)
{
	Notify(DNM_RULER_SCALE_CHANGING, 0, &newUnitPerPixel);
}
void CObjectsContainer::OnRulerScaleChanged()
{
	Notify(DNM_RULER_SCALE_CHANGED);
}

void CObjectsContainer::SetShowRuler(int nRuler, bool isShow)
{
    if (nRuler == HORIZ_RULER && m_pHorizalRuler)
    {
        m_pHorizalRuler->SetVisible(isShow);
    }
    if (nRuler == VERT_RULER && m_pVerticalRuler)
    {
        m_pVerticalRuler->SetVisible(isShow);
    }
}
bool CObjectsContainer::GetShowRuler(int nRuler) const 
{
    if (nRuler == HORIZ_RULER && m_pHorizalRuler)
    {
        return m_pHorizalRuler->GetVisible();
    }
    if (nRuler == VERT_RULER && m_pVerticalRuler)
    {
        return m_pVerticalRuler->GetVisible();
    }
    return false;
}
void CObjectsContainer::OnShowRulerChanging(int nRuler, bool & newIsShow)
{
	Notify(DNM_SHOW_RULER_CHANGING, nRuler, &newIsShow);
}
void CObjectsContainer::OnShowRulerChanged(int nRuler)
{
	Notify(DNM_SHOW_RULER_CHANGED, nRuler);
}


void CObjectsContainer::SetRulerColor(int nRuler, Color color)
{
    if (nRuler == HORIZ_RULER)
    {
        if (m_pHorizalRuler->GetLineColor().GetValue() != color.GetValue())
        {
            OnRulerColorChanging(nRuler, color);
            m_pHorizalRuler->SetLineColor(color);
            OnRulerScaleChanged();
        }
    }
    if (nRuler == VERT_RULER)
    {
        if (m_pVerticalRuler->GetLineColor().GetValue() != color.GetValue())
        {
            OnRulerColorChanging(nRuler, color);
            m_pVerticalRuler->SetLineColor(color);
            OnRulerScaleChanged();
        }
    }
}
Color CObjectsContainer::GetRulerColor(int nRuler) const 
{
    if (nRuler == HORIZ_RULER)
    {
        return m_pHorizalRuler->GetLineColor();
    }
    if (nRuler == VERT_RULER)
    {
        return m_pVerticalRuler->GetLineColor();
    }
    return Color::Black;
}
void CObjectsContainer::OnRulerColorChanging(int nRuler, Color & color)
{
	Notify(DNM_RULER_COLOR_CHANGING, nRuler, &color);
}
void CObjectsContainer::OnRulerColorChanged(int nRuler)
{
	Notify(DNM_RULER_COLOR_CHANGED, nRuler);
}

void CObjectsContainer::SetShowName(bool isShow)
{
    if (m_bShowNameOnBackground != isShow)
    {
        OnShowNameChanging(isShow);
        m_bShowNameOnBackground = isShow;
        OnShowNameChanged();
    }
}
bool CObjectsContainer::GetShowName() const 
{
    return m_bShowNameOnBackground;
}

void CObjectsContainer::OnShowNameChanging(bool & newIsShow)
{
	Notify(DNM_CONTAINER_SHOW_NAME_CHANGING, 0, &newIsShow);
}
void CObjectsContainer::OnShowNameChanged()
{
	Notify(DNM_CONTAINER_SHOW_NAME_CHANGED);
}


//void CObjectsContainer::Enable(const CString & propertyName)
//{
//    CRectObject::Enable(propertyName);
//    if (propertyName == _T("VertRuler"))
//    {
//        SetShowRuler(VERT_RULER, true);
//    }
//    if (propertyName == _T("HorizRuler"))
//    {
//        SetShowRuler(HORIZ_RULER, true);
//    }
//
//}
//void CObjectsContainer::Disable(const CString & propertyName)
//{
//    CRectObject::Disable(propertyName);
//    if (propertyName == _T("VertRuler"))
//    {
//        SetShowRuler(VERT_RULER, false);
//    }
//    if (propertyName == _T("HorizRuler"))
//    {
//        SetShowRuler(HORIZ_RULER, false);
//    }
//}
//bool CObjectsContainer::IsEnabled(const CString & propertyName)
//{
//    bool ok = CRectObject::IsEnabled(propertyName);
//    if (ok)
//    {
//        return true;
//    }
//    if (propertyName == _T("VertRuler"))
//    {
//        return m_pVerticalRuler->GetVisible();
//    }
//    if (propertyName == _T("HorizRuler"))
//    {
//        return m_pHorizalRuler->GetVisible();
//    }
//    return false;
//}
//
CDrawObject *CObjectsContainer::CopyFrom(CDrawObject * source)
{
    CObjectsContainer *pImageObject = DYNAMIC_DOWNCAST(CObjectsContainer, source);
    ASSERT(pImageObject);
    __super::CopyFrom(source);
    m_RulerUnitPerPixel = pImageObject->m_RulerUnitPerPixel;
    m_Options = pImageObject->m_Options;
	if(m_Options|WITH_HORIZ_RULER && pImageObject->m_pHorizalRuler)
	{
		delete m_pHorizalRuler;
		m_pHorizalRuler=(CRulerObject*)(pImageObject->m_pHorizalRuler->Clone());
	}
	if(m_Options| WITH_VERT_RULER && pImageObject->m_pVerticalRuler)
	{
		delete m_pVerticalRuler;
		m_pVerticalRuler=(CRulerObject*)(pImageObject->m_pVerticalRuler->Clone());
	}
	Clear();
	POSITION pos=pImageObject->m_DrawObjects.GetHeadPosition();
	while(pos)
	{
		CDrawObject *pObject=pImageObject->m_DrawObjects.GetNext(pos);
		ASSERT(pObject);
		pObject=pObject->Clone();
		pObject->SetParent(this);
		m_DrawObjects.AddTail(pObject);
		
	}
    return this;
}


void CObjectsContainer::Serialize(CArchive & ar)
{
    CDrawObject *pObject = NULL;
    CRectObject::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_pHorizalRuler;
        ar << m_pVerticalRuler;
        ar << m_RulerUnitPerPixel;
        ar << m_Options;
        ar << m_bShowNameOnBackground;
        INT_PTR count = m_DrawObjects.GetCount();
        ar << count;
        POSITION pos = m_DrawObjects.GetHeadPosition();
        while (pos)
        {
            pObject = m_DrawObjects.GetNext(pos);
            ar << pObject;
        }
    }
    else
    {
        ar >> m_pHorizalRuler;
        ar >> m_pVerticalRuler;
        ar >> m_RulerUnitPerPixel;
        if (m_pHorizalRuler)
        {
            m_pHorizalRuler->SetParent(this);
            m_pHorizalRuler->SetUnitPerPixel(m_RulerUnitPerPixel);
        }
        if (m_pVerticalRuler)
        {
            m_pVerticalRuler->SetParent(this);
            m_pVerticalRuler->SetUnitPerPixel(m_RulerUnitPerPixel);
        }
        ar >> m_Options;
        ar >> m_bShowNameOnBackground;
        int count;
        ar >> count;
        for (int i = 0; i < count; i++)
        {
            ar >> pObject;
            ASSERT(pObject);
            pObject->SetParent(this);
            m_DrawObjects.AddTail(pObject);
        }
    }
}

//(覆盖)
void CObjectsContainer::OnDrawTracker(Graphics & graph)
{

}


Gdiplus::Rect CObjectsContainer::GetBounds()
{
    Rect rect;
    Rect temp;
    Rect out;
    POSITION pos = m_DrawObjects.GetHeadPosition();
    while (pos)
    {
        CDrawObject *pObject = m_DrawObjects.GetNext(pos);
        temp = pObject->GetBounds();
        Rect::Union(out, rect, temp);
        rect = out;
    }
    temp = __super::GetBounds();
    if (!rect.IsEmptyArea())
    {
        Rect::Union(out, rect, temp);
	}
	else
	{
		out=temp;
	}
    return out;
}
Gdiplus::Rect CObjectsContainer::GetWorldBounds()
{
	Rect rect;
	Rect temp;
	Rect out;
    Local2World(&rect);
	POSITION pos = m_DrawObjects.GetHeadPosition();
	while (pos)
	{
		CDrawObject *pObject = m_DrawObjects.GetNext(pos);
		temp = pObject->GetWorldBounds();
		Rect::Union(out, rect, temp);
		rect = out;
	}
	temp = __super::GetWorldBounds();
	Rect::Union(out, rect, temp);
	return out;
}

//事件(覆盖)

void CObjectsContainer::OnSizeChanged()
{
	__super::OnSizeChanged();
    Size size;
    Size newSize;
    if (m_pHorizalRuler)
    {
        size = m_pHorizalRuler->GetSize();
        newSize = Size(GetSize().Width - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
        m_pHorizalRuler->SetSize(newSize);
    }
    if (m_pVerticalRuler)
    {
        size = m_pVerticalRuler->GetSize();
        newSize = Size(GetSize().Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
        m_pVerticalRuler->SetSize(newSize);
    }
    SetClipRect(Rect(Point(0, 0), GetSize()));
}



void CObjectsContainer::OnDraw(Graphics & graph)
{
    DrawFillObject(graph);
    if (m_bShowNameOnBackground)
    {
        float h = GetSize().Width / GetName().GetLength() / 6.0f;
        StringFormat stringFormat;
        stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
        stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
        Color color = GetFillColor();
        BYTE alpha = (BYTE) (GetTransparent() * 255);
        ARGB argb = color.GetValue();
        argb &= (((ARGB) (alpha) << Color::AlphaShift) | 0x00FFFFFF);
        color = argb;
        SolidBrush brush(color);
        Gdiplus::Font font(_T("宋体"), h);
        graph.DrawString(GetName(), -1, &font, PointF(0, 0), &stringFormat, &brush);
    }
    POSITION pos = m_DrawObjects.GetHeadPosition();
    while (pos)
    {
        CDrawObject *pObject = m_DrawObjects.GetNext(pos);
        ASSERT(pObject);
        pObject->Draw(graph);
    }
    DrawRuler(graph);
}

void CObjectsContainer::DrawRuler(Graphics & graph)
{
    if(m_pHorizalRuler)
        m_pHorizalRuler->Draw(graph);
    if(m_pVerticalRuler)
        m_pVerticalRuler->Draw(graph);
}

bool CObjectsContainer::BuildProperties(CXTPPropertyGridItem * pCategoryObjects)
{
    if (!CRectObject::BuildProperties(pCategoryObjects))
        return false;
    return BuildRulerProperty(pCategoryObjects);
}

bool CObjectsContainer::BuildRulerProperty(CXTPPropertyGridItem * pCategoryObjects)
{
    CXTPPropertyGridItem *pItem;
    if (m_Options & (WITH_VERT_RULER | WITH_HORIZ_RULER))
    {
        if (m_RulerUnitPerPixel != 0)
        {
            if (m_pHorizalRuler)
            {
                pItem = pCategoryObjects->AddChildItem(new CXTPPropertyGridItem(ID_OBJECT_CONTAINER_HORIZ_RULER));
                pItem->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_VISIBLE, m_pHorizalRuler->GetVisible()))->
                    SetItemData((DWORD_PTR) m_pHorizalRuler);
                pItem->AddChildItem(new CXTPPropertyGridItemColor(ID_OBJECT_CONTAINER_RULER_COLOR, m_pHorizalRuler->GetLineColor().GetValue()))->
                    SetItemData((DWORD_PTR) m_pHorizalRuler);
                pItem =
                    pItem->AddChildItem(new CXTPPropertyGridItem(ID_OBJECT_CONTAINER_RULER_UNIT, m_pHorizalRuler->GetUnitZhName(m_pHorizalRuler->GetUnit())));
                pItem->SetReadOnly(TRUE);
                pItem->SetItemData((DWORD_PTR) m_pHorizalRuler);
            }
            if (m_pVerticalRuler)
            {
                pItem = pCategoryObjects->AddChildItem(new CXTPPropertyGridItem(ID_OBJECT_CONTAINER_VERT_RULER));
                pItem->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_VISIBLE, m_pVerticalRuler->GetVisible()))->
                    SetItemData((DWORD_PTR) m_pVerticalRuler);
                pItem->AddChildItem(new CXTPPropertyGridItemColor(ID_OBJECT_CONTAINER_RULER_COLOR, m_pVerticalRuler->GetLineColor().GetValue()))->
                    SetItemData((DWORD_PTR) m_pVerticalRuler);
                pItem =
                    pItem->AddChildItem(new CXTPPropertyGridItem(ID_OBJECT_CONTAINER_RULER_UNIT, m_pVerticalRuler->GetUnitZhName(m_pVerticalRuler->GetUnit())));
                pItem->SetReadOnly(TRUE);
                pItem->SetItemData((DWORD_PTR) m_pVerticalRuler);
            }
        }
    }
    return true;
}

void CObjectsContainer::OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem)
{
    CRectObject::OnPropertyItemChangedNotify(pItem);
}

CDrawObjectList & CObjectsContainer::GetObjects()
{
    return m_DrawObjects;
}

void CObjectsContainer::AddObject(CDrawObject * pObject)
{
    pObject->SetParent(this);
    m_DrawObjects.AddTail(pObject);
}

CDrawObject *CObjectsContainer::RemoveObject(CDrawObject * pObject)
{
    POSITION pos = m_DrawObjects.Find(pObject);
    if (pos)
    {
        m_DrawObjects.RemoveAt(pos);
        pObject->SetParent(NULL);
        return pObject;
    }
    return NULL;
}

void CObjectsContainer::Clear()
{
    CDrawObject *pObject;
    while (!m_DrawObjects.IsEmpty())
    {
        pObject = m_DrawObjects.GetHead();
        m_DrawObjects.RemoveHead();
        delete pObject;
    }
}

void CObjectsContainer::SelectAll(CDrawObjectList & objList, int nFlag /*=SELECT_ADD*/ )
{
    if (nFlag == SELECT_REPLACE)
    {
        objList.RemoveAll();
    }
    POSITION pos = m_DrawObjects.GetHeadPosition();
    while (pos)
    {
        objList.AddTail(m_DrawObjects.GetNext(pos));
    }
}

CDrawObject *CObjectsContainer::GetObjectsAt(const Gdiplus::Point & point)
{
    Point pt = point;
	Global2Local(&pt);
    POSITION pos = m_DrawObjects.GetTailPosition();
    while (pos)
    {
        Gdiplus::Point pt2;
        CDrawObject *pObject = m_DrawObjects.GetPrev(pos);
        ASSERT(pObject);
        pt2 = pt;
        pObject->Global2Local(&pt2);
        if (pObject->HitTest(pt2))
        {
            return pObject;
        }
    }
    return NULL;
}
CDrawObject *CObjectsContainer::GetTypeOfObjectAt(CRuntimeClass *pClass, const Gdiplus::Point &point)
{
	Gdiplus::Point pt=point;
	Global2Local(&pt);
	POSITION pos = m_DrawObjects.GetTailPosition();
	while (pos)
	{
		Gdiplus::Point pt2;
		CDrawObject *pObject = m_DrawObjects.GetPrev(pos);
		ASSERT(pObject);
		if(pObject->IsKindOf(pClass))
		{
			pt2 = pt;
			pObject->Global2Local(&pt2);
			if (pObject->HitTest(pt2))
			{
				return pObject;
			}
		}
	}
	return NULL;
}
CDrawObject * CObjectsContainer::FindObject( const CString & name )
{
	POSITION pos=m_DrawObjects.GetHeadPosition();
        while(pos)
        {
            CDrawObject *pObj=m_DrawObjects.GetNext(pos);
            if(pObj->GetName()== name)
            {
                return pObj;
            }
        }
        return NULL;
}

void CObjectsContainer::notifyAttribtesChanged( CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal )
{
    switch(attributeIndex)
    {
    case ID_OBJECT_CONTAINER_RULER_OPTION:
        ASSERT(FALSE);
        break;
    case ID_OBJECT_CONTAINER_HORIZ_RULER:
        SetShowRuler(HORIZ_RULER, newVal);
        break;
    case ID_OBJECT_CONTAINER_VERT_RULER:
        SetShowRuler(VERT_RULER, newVal);
        break;
    case ID_OBJECT_CONTAINER_RULER_COLOR:
        SetRulerColor(HORIZ_RULER, Color((int)newVal));
        SetRulerColor(VERT_RULER, Color((int)newVal));
        break;
    case ID_OBJECT_CONTAINER_RULER_UNIT:
        SetRulerUnit(HORIZ_RULER, (int)newVal);
        SetRulerUnit(VERT_RULER, (int)newVal);
        break;
    default:
        __super::notifyAttribtesChanged(changedObject, attributeIndex, newVal, oldVal);
        break;
    }
}

void CObjectsContainer::notifyChildAdded( CDrawObject * container, CDrawObject *childToAdd, CDrawObject *afterObject )
{
    ASSERT(container==this);
    if(afterObject==NULL)
    {
        GetObjects().AddHead(childToAdd);
    }
    else
    {
        POSITION pos=GetObjects().Find(afterObject);
        GetObjects().InsertAfter(pos, childToAdd);
    }
}

void CObjectsContainer::notifyChildDeleted( CDrawObject *changedObject, CDrawObject *childToDelete, CDrawObject * afterObject )
{
    ASSERT(changedObject==this);
    RemoveObject(childToDelete);
}

void CObjectsContainer::notifyOrderChanged( CDrawObject* container, CDrawObject * changedObject, CDrawObject * newAfterObject, CDrawObject *oldAfterObject )
{
    ASSERT(container==this);
    POSITION pos=GetObjects().Find(changedObject);
    ASSERT(pos);
    GetObjects().RemoveAt(pos);
    if(newAfterObject==NULL)
    {
        GetObjects().AddHead(changedObject);
    }
    else
    {
        pos= GetObjects().Find(newAfterObject);
        ASSERT(pos);
        GetObjects().InsertAfter(pos, changedObject);
    }
}

bool CObjectsContainer::IsEmpty()
{
    return GetObjects().GetCount()!=0;
}
