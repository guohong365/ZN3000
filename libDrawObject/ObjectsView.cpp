#include "StdAfx.h"
#include "DrawObject.h"
#include "RectObject.h"
#include "ImageObject.h"
#include "RulerObject.h"
#include "ObjectsContainer.h"
#include "ImageContainer.h"
#include "ObjectsView.h"
#include "AutoComparor.h"
#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

IMPLEMENT_SERIAL(CObjectsView, CObjectsContainer, 0)
void CObjectsView::Initailize()
{
    m_pBaseContainer = NULL;
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        m_pLayoutContainers[i] = NULL;
        m_pRealContainers[i] = NULL;
    }
    m_pCurrentContainer = NULL;
    m_Layout = VIEW_LAYOUT_2x2;
    m_ViewState = VIEW_STATE_NORMAL;
    m_LastLayout = VIEW_LAYOUT_2x2;
    m_CompareImage1 = 0;
    m_CompareImage2 = 1;


}

CObjectsView::CObjectsView()
{
    Initailize();
}
CObjectsView::CObjectsView(const CString & viewName):CObjectsContainer(viewName, Gdiplus::Point(0, 0), Gdiplus::Size(100, 100))
{
    Initailize();
    m_pBaseContainer = new CObjectsContainer(_T("Base"), Gdiplus::Rect(0, 0, 1, 1));
    m_pBaseContainer->SetParent(this);
    m_pBaseContainer->SetVisible(false);
    CString name;
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        name.Format(_T("Layout %d"), i + 1);
        m_pLayoutContainers[i] = new CObjectsContainer(name, Gdiplus::Rect(0, 0, 1, 1));
        m_LayeredObjects.AddTail(m_pLayoutContainers[i]);
        m_pLayoutContainers[i]->SetParent(this);
        m_pLayoutContainers[i]->SetShowBorder(true);
        m_pLayoutContainers[i]->SetShowRuler(HORIZ_RULER, false);
        m_pLayoutContainers[i]->SetShowRuler(VERT_RULER, false);
        m_pLayoutContainers[i]->SetLineColor(Gdiplus::Color::Yellow);
        name.Format(_T("Image %d"), i + 1);
        m_pRealContainers[i] = new CImageContainer(name, Gdiplus::Rect(0, 0, 1, 1));
        m_pRealContainers[i]->SetParent(m_pLayoutContainers[i]);
        m_pRealContainers[i]->SetShowBorder(false);
        m_pRealContainers[i]->SetShowRuler(HORIZ_RULER, false);
        m_pRealContainers[i]->SetShowRuler(VERT_RULER, false);
        m_pRealContainers[i]->SetLineColor(Gdiplus::Color::Yellow);
        m_pLayoutContainers[i]->AddObject(m_pRealContainers[i]);
    }
    m_pRealContainers[0]->SetActiveColor(Gdiplus::Color::Red);
    m_pRealContainers[1]->SetActiveColor(Gdiplus::Color::Blue);
    m_pRealContainers[2]->SetActiveColor(Gdiplus::Color::Green);
    m_pRealContainers[3]->SetActiveColor(Gdiplus::Color::Cyan);
    //SetFilled(true);
}

CObjectsView::~CObjectsView()
{
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        delete m_pLayoutContainers[i];
    }
    delete m_pBaseContainer;
}

void CObjectsView::SetCompareImageIndex(long image1, long image2)
{
    m_CompareImage1 = image1;
    m_CompareImage2 = image2;
}
void CObjectsView::GetComareImageIndex(long &image1, long &image2)
{
    image1 = m_CompareImage1;
    image2 = m_CompareImage2;
}

//////////////////////////////////////////////////////////////////////////
//属性操作
void CObjectsView::SetLayout(long layout)
{
    if (m_Layout != layout)
    {
        OnLayoutChanging(layout);
        m_Layout = layout;
        OnLayoutChanged();
    }
}
long CObjectsView::GetLayout() const 
{
    return m_Layout;
}
void CObjectsView::OnLayoutChanging(long &layout)
{
    m_LastLayout = m_Layout;
}

void CObjectsView::OnLayoutChanged()
{
    RecalculateLayout();
}

void CObjectsView::SetViewState(long view_state)
{
    if (m_ViewState != view_state)
    {
        OnViewStateChanging(view_state);
        m_ViewState = view_state;
        OnViewStateChanged();
    }
}
long CObjectsView::GetViewState() const 
{
    return m_ViewState;
}
void CObjectsView::OnViewStateChanging(long &state)
{

}
void CObjectsView::OnViewStateChanged()
{
    switch (m_ViewState)
    {
    case VIEW_STATE_NORMAL:
        OnCancelCompare();
        break;
    case VIEW_STATE_VERT_COMPARE:
        OnVertCompare();
        break;
    case VIEW_STATE_HORIZ_COMPARE:
        OnHorizCompare();
        break;
    case VIEW_STATE_OVERLAP_COMPARE:
        OnOverlayCompare();
        break;
    default:
        break;
    }
}

void CObjectsView::Add(int nItem, CDrawObject * pObj)
{
    ASSERT(nItem >= 0);
    ASSERT(nItem < IMAGE_CONTAINER_COUNT);
    ASSERT(pObj);
    ASSERT(m_pLayoutContainers[nItem]);
    CImageContainer *pImageContainer = GetContainer(nItem);
    pImageContainer->GetObjects().AddTail(pObj);
}
void CObjectsView::Remove(int nItem, CDrawObject * pObj)
{
    ASSERT(nItem >= 0 && nItem < IMAGE_CONTAINER_COUNT);
    ASSERT(pObj);
    ASSERT(m_pLayoutContainers[nItem]);
    CImageContainer *pImageContainer = GetContainer(nItem);

    POSITION pos = pImageContainer->GetObjects().Find(pObj);
    ASSERT(pos);
    pImageContainer->GetObjects().RemoveAt(pos);
}

int CObjectsView::GetImageCount()
{
    int count = 0;
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        if (m_pRealContainers[i]->GetImage())
        {
            count++;
        }
    }
    return count;
}

CImageContainer *CObjectsView::GetContainer(int nItem)
{
    POSITION pos = m_pLayoutContainers[nItem]->GetObjects().GetHeadPosition();
    while (pos)
    {
        CDrawObject *pObject = m_pLayoutContainers[nItem]->GetObjects().GetNext(pos);
        if (pObject && pObject->IsKindOf(RUNTIME_CLASS(CImageContainer)))
        {
            return DYNAMIC_DOWNCAST(CImageContainer, pObject);
        }
    }
    return NULL;
}

CImageContainer *CObjectsView::GetCurrentContainer(CObjectsContainer * pContainer)
{
    if (pContainer)
    {
        POSITION pos = pContainer->GetObjects().GetHeadPosition();
        while (pos)
        {
            CDrawObject *pObject = pContainer->GetObjects().GetNext(pos);
            if (pObject && pObject->IsKindOf(RUNTIME_CLASS(CImageContainer)))
            {
                return DYNAMIC_DOWNCAST(CImageContainer, pObject);
            }
        }
    }
    return NULL;
}

int CObjectsView::GetContainerIndex(CImageContainer * pImageContainer)
{
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        if (m_pRealContainers[i] == pImageContainer)
            return i;
    }
    return -1;
}

void CObjectsView::ExchangeImage(CDrawObject * pSrcImageContainer, CDrawObject * pDestImageContainer)
{
    CObjectsContainer *pParentSource = DYNAMIC_DOWNCAST(CObjectsContainer, pSrcImageContainer->GetParent());
    CObjectsContainer *pParentDest = DYNAMIC_DOWNCAST(CObjectsContainer, pDestImageContainer->GetParent());
    ASSERT(pParentSource && pParentDest);

    Gdiplus::Point ptSource = pSrcImageContainer->GetPosition();
    Gdiplus::Size sizeSource = pSrcImageContainer->GetSize();

    Gdiplus::Point ptDest = pDestImageContainer->GetPosition();
    Gdiplus::Size sizeDest = pDestImageContainer->GetSize();

    pParentSource->RemoveObject(pSrcImageContainer);
    pParentDest->RemoveObject(pDestImageContainer);

    pParentSource->AddObject(pDestImageContainer);
    pSrcImageContainer->SetPosition(ptDest);
    pSrcImageContainer->SetSize(sizeDest);

    pParentDest->AddObject(pSrcImageContainer);
    pDestImageContainer->SetPosition(ptSource);
    pDestImageContainer->SetSize(sizeSource);
}

bool CObjectsView::CanCompare(bool * canCompare)
{
    int i;
    int count = 0;
    for (i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        if (GetContainer(i)->GetImage() && GetContainer(i)->GetRulerScale() > 0)
        {
            if (canCompare != NULL)
            {
                canCompare[i] = true;
            }
            count++;
        }
        else
        {
            if (canCompare != NULL)
            {
                canCompare[i] = false;
            }
        }
    }
    return count > 1;
}

//设置参与比较的两张图片下标到数组[0]，[1]位置，其他两张图片置换到[2]、[3]位置
static void setCompareIndexArray(int *a, int comp1, int comp2)
{
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    a[3] = 3;
    if (comp1 != a[0])
    {
        a[0] = comp1;
        a[comp1] = 0;
    }
    if (comp2 != a[1])
    {
        long temp = a[1];
        a[1] = comp2;
        a[comp2] = temp;
    }

}
void CObjectsView::OnVertCompare()
{
    int a[4];

    setCompareIndexArray(a, m_CompareImage1, m_CompareImage2);

    CImageContainer *pImage1 = GetContainer(a[0]);
    CImageContainer *pImage2 = GetContainer(a[1]);
    double scale1 = pImage1->GetRulerScale();
    double scale2 = pImage1->GetRulerScale();
    if (scale1 == 0.0 || scale2 == 0.0)
    {
        return;
    }
    m_pLayoutContainers[a[2]]->SetVisible(false);
    m_pLayoutContainers[a[3]]->SetVisible(false);

    Gdiplus::Size size = GetSize();
    m_pLayoutContainers[a[0]]->SetSize(size);
    pImage1->SetSize(size);
    m_pLayoutContainers[a[1]]->SetSize(size);
    pImage2->SetSize(size);
    scale1 = pImage1->GetRulerScale();
    scale2 = pImage2->GetRulerScale();
    double rate;
    if (scale1 > scale2)
    {
        rate = scale2 / scale1;
        pImage2->GetImageObject()->Scale(rate, rate);
        pImage2->SetRulerScale(scale1);
    }
    else
    {
        rate = scale1 / scale2;
        pImage1->GetImageObject()->Scale(rate, rate);
        pImage1->SetRulerScale(scale2);
    }
    //pImage2->Scale(rate);

    Gdiplus::Point lastPos(0, 0);
    size.Height = size.Height / 2;
    m_pLayoutContainers[a[0]]->SetPosition(lastPos);
    m_pLayoutContainers[a[0]]->SetClipRect(Gdiplus::Rect(Gdiplus::Point(0, 0), size));
    m_pLayoutContainers[a[0]]->SetUsingClip(true);
    m_pLayoutContainers[a[0]]->SetShowBorder(false);
    pImage1->SetTransparent(1.0f);
    pImage1->SetShowBorder(false);
    pImage1->SetShowTracker(false);

    lastPos.Y = size.Height;
    m_pLayoutContainers[a[1]]->SetPosition(lastPos);
    m_pLayoutContainers[a[1]]->SetClipRect(Gdiplus::Rect(Gdiplus::Point(0, 0), size));
    m_pLayoutContainers[a[1]]->SetUsingClip(true);
    m_pLayoutContainers[a[1]]->SetShowBorder(false);
    pImage2->SetTransparent(1.0f);
    pImage2->SetShowBorder(false);
    pImage2->SetShowTracker(false);
    pImage2->SetShowRuler(HORIZ_RULER, false);
}

void CObjectsView::OnHorizCompare()
{
    int a[4];

    setCompareIndexArray(a, m_CompareImage1, m_CompareImage2);

    CImageContainer *pImage1 = GetContainer(a[0]);
    CImageContainer *pImage2 = GetContainer(a[1]);

    double scale1 = pImage1->GetRulerScale();
    double scale2 = pImage1->GetRulerScale();
    if (scale1 == 0.0 || scale2 == 0.0)
    {
        return;
    }
    m_pLayoutContainers[a[2]]->SetVisible(false);
    m_pLayoutContainers[a[3]]->SetVisible(false);

    Gdiplus::Size size = GetSize();
    m_pLayoutContainers[a[0]]->SetSize(size);
    pImage1->SetSize(size);
    m_pLayoutContainers[a[1]]->SetSize(size);
    pImage2->SetSize(size);
    scale1 = pImage1->GetRulerScale();
    scale2 = pImage2->GetRulerScale();
    double rate;
    if (scale1 > scale2)
    {
        rate = scale2 / scale1;
        pImage2->GetImageObject()->Scale(rate,rate);
        pImage2->SetRulerScale(scale1);
    }
    else
    {
        rate = scale1 / scale2;
        pImage1->GetImageObject()->Scale(rate,rate);
        pImage1->SetRulerScale(scale2);
    }
    //pImage2->Scale(rate);

    Gdiplus::Point lastPos(0, 0);
    size.Width = size.Width / 2;
    m_pLayoutContainers[a[0]]->SetPosition(lastPos);
    m_pLayoutContainers[a[0]]->SetClipRect(Gdiplus::Rect(Gdiplus::Point(0, 0), size));
    m_pLayoutContainers[a[0]]->SetUsingClip(true);
    m_pLayoutContainers[a[0]]->SetShowBorder(false);
    pImage1->SetTransparent(1.0f);
    pImage1->SetShowBorder(false);
    pImage1->SetShowTracker(false);

    lastPos.X = size.Width;
    m_pLayoutContainers[a[1]]->SetPosition(lastPos);
    m_pLayoutContainers[a[1]]->SetClipRect(Gdiplus::Rect(Gdiplus::Point(0, 0), size));
    m_pLayoutContainers[a[1]]->SetUsingClip(true);
    m_pLayoutContainers[a[1]]->SetShowBorder(false);
    pImage2->SetTransparent(1.0f);
    pImage2->SetShowBorder(false);
    pImage2->SetShowTracker(false);
    pImage2->SetShowRuler(VERT_RULER, false);
}

void CObjectsView::OnOverlayCompare()
{
    int a[4];

    setCompareIndexArray(a, m_CompareImage1, m_CompareImage2);

    CImageContainer *pImage1 = GetContainer(a[0]);
    CImageContainer *pImage2 = GetContainer(a[1]);

    double scale1 = pImage1->GetRulerScale();
    double scale2 = pImage1->GetRulerScale();
    if (scale1 == 0.0 || scale2 == 0.0)
    {
        return;
    }
    m_pLayoutContainers[a[2]]->SetVisible(false);
    m_pLayoutContainers[a[3]]->SetVisible(false);

    Gdiplus::Size size = GetSize();
    m_pLayoutContainers[a[0]]->SetSize(size);
    pImage1->SetSize(size);
    m_pLayoutContainers[a[1]]->SetSize(size);
    pImage2->SetSize(size);
    scale1 = pImage1->GetRulerScale();
    scale2 = pImage2->GetRulerScale();
    double rate;
    if (scale1 > scale2)
    {
        rate = scale2 / scale1;
        pImage2->GetImageObject()->Scale(rate,rate);
        pImage2->SetRulerScale(scale1);
    }
    else
    {
        rate = scale1 / scale2;
        pImage1->GetImageObject()->Scale(rate,rate);
        pImage1->SetRulerScale(scale2);
    }

    Gdiplus::Point lastPos(0, 0);
    m_pLayoutContainers[a[0]]->SetPosition(lastPos);
    m_pLayoutContainers[a[0]]->SetClipRect(Gdiplus::Rect(Gdiplus::Point(0, 0), size));
    m_pLayoutContainers[a[0]]->SetUsingClip(true);
    m_pLayoutContainers[a[0]]->SetShowBorder(false);
    pImage1->SetTransparent(0.5f);
    pImage1->SetShowBorder(false);
    pImage1->SetShowTracker(false);
    pImage1->SetFilled(false);

    m_pLayoutContainers[a[1]]->SetPosition(lastPos);
    m_pLayoutContainers[a[1]]->SetClipRect(Gdiplus::Rect(Gdiplus::Point(0, 0), size));
    m_pLayoutContainers[a[1]]->SetUsingClip(true);
    m_pLayoutContainers[a[1]]->SetShowBorder(false);
    pImage2->SetTransparent(0.5f);
    pImage2->SetShowBorder(false);
    pImage2->SetShowTracker(false);
    pImage2->SetShowRuler(HORIZ_RULER, false);
    pImage2->SetShowRuler(VERT_RULER, false);
    pImage2->SetFilled(false);
}

void CObjectsView::OnCancelCompare()
{
    RecalculateLayout();
}

void CObjectsView::OnSizeChanged()
{
    CObjectsContainer::OnSizeChanged();
    m_pBaseContainer->SetSize(GetSize());
    OnViewStateChanged();
}

CDrawObjectList & CObjectsView::GetLayer()
{
    return m_LayeredObjects;
}

bool CObjectsView::IsEmpty(int nItem)
{
    ASSERT(nItem >= 0 && nItem < IMAGE_CONTAINER_COUNT);
    return GetContainer(nItem)->GetImage() != NULL;
}

void CObjectsView::ClearContainer(CImageContainer * pItem)
{
    ASSERT(pItem);
    pItem->Clear();
}
void CObjectsView::ClearContainer(int nItem)
{
    ASSERT(nItem >= 0 && nItem < IMAGE_CONTAINER_COUNT);
    m_pLayoutContainers[nItem]->Clear();
}

void CObjectsView::ActiveContainer(int nItem, bool isActive)
{
    ASSERT(nItem >= 0 && nItem < IMAGE_CONTAINER_COUNT);
    if (isActive)
    {
        if (m_pCurrentContainer != m_pLayoutContainers[nItem])
        {
            if (m_pCurrentContainer)
            {
                GetCurrentContainer(m_pCurrentContainer)->SetSelected(false);
                m_pCurrentContainer->SetSelected(false);
            }
            m_pCurrentContainer = m_pLayoutContainers[nItem];
            m_pLayoutContainers[nItem]->SetSelected(true);
            GetCurrentContainer(m_pCurrentContainer)->SetSelected(true);
            m_LayeredObjects.MoveToTail(m_pCurrentContainer);
        }
    }
    else
    {
        if (m_pCurrentContainer == m_pLayoutContainers[nItem])
        {
            m_pCurrentContainer->SetSelected(false);
            GetCurrentContainer(m_pCurrentContainer)->SetSelected(false);
            m_pCurrentContainer = NULL;
        }
        else
        {
            m_pLayoutContainers[nItem]->SetSelected(false);
            GetContainer(nItem)->SetSelected(false);
        }
    }

}
void CObjectsView::ActiveContainer(CImageContainer * pItem, bool isActive)
{
    ASSERT(pItem);
    CImageContainer *pImageContainer;
    if (isActive)
    {
        if (m_pCurrentContainer != pItem)
        {
            if (m_pCurrentContainer != NULL)
            {
                pImageContainer = GetCurrentContainer(m_pCurrentContainer);
                pImageContainer->SetSelected(false);
                m_pCurrentContainer->SetSelected(false);
            }
        }
        m_pCurrentContainer = (CObjectsContainer *) pItem->GetParent();
        m_pCurrentContainer->SetSelected(true);
        pItem->SetSelected(true);
        m_LayeredObjects.MoveToTail(m_pCurrentContainer);
    }
    else
    {
        pItem->SetSelected(false);
        pItem->GetParent()->SetSelected(false);
        if (m_pCurrentContainer == pItem)
        {
            m_pCurrentContainer = NULL;
        }
    }
}
CImageContainer *CObjectsView::ActiveContainer(const Gdiplus::Point & point)
{
    CImageContainer *pImageContainer = NULL;
    Gdiplus::Point pt;
    //Global2Local(&point);
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        pt = point;
        m_pLayoutContainers[i]->Global2Local(&pt);
        if (m_pLayoutContainers[i]->HitTest(pt) && m_pLayoutContainers[i]->GetVisible())
        {
            m_pLayoutContainers[i]->SetSelected(true);
            m_pCurrentContainer = m_pLayoutContainers[i];
            pImageContainer = GetCurrentContainer(m_pCurrentContainer);
            pImageContainer->SetSelected(true);
            m_LayeredObjects.MoveToTail(m_pCurrentContainer);
        }
        else
        {
            m_pLayoutContainers[i]->SetSelected(false);
            GetContainer(i)->SetSelected(false);
        }
    }
    return pImageContainer;
}

CImageContainer *CObjectsView::GetContainer(const Gdiplus::Point point)
{
    CImageContainer *pImageContainer = NULL;
    Gdiplus::Point pt;
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        pt = point;
        m_pLayoutContainers[i]->Global2Local(&pt);
        if (m_pLayoutContainers[i]->HitTest(pt) && m_pLayoutContainers[i]->GetVisible())
        {
            return GetCurrentContainer(m_pLayoutContainers[i]);
        }
    }
    return NULL;
}


CImageContainer *CObjectsView::GetActiveContainer()
{
    if (m_pCurrentContainer == NULL)
    {
        return NULL;
    }
    POSITION pos = m_pCurrentContainer->GetObjects().GetHeadPosition();
    while (pos)
    {
        CDrawObject *pObject = m_pCurrentContainer->GetObjects().GetNext(pos);
        if (pObject && pObject->IsKindOf(RUNTIME_CLASS(CImageContainer)))
        {
            return DYNAMIC_DOWNCAST(CImageContainer, pObject);
        }
    }
    return NULL;
}

CObjectsContainer *CObjectsView::GetBaseContainer()
{
    return m_pBaseContainer;
}

void CObjectsView::RecalculateLayout()
{
    Gdiplus::Size size = GetSize();
    int i;
    CImageContainer *pImageContainer;
    for (i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        m_pLayoutContainers[i]->SetVisible(true);
        m_pLayoutContainers[i]->SetShowBorder(false);
        m_pLayoutContainers[i]->SetUsingClip(false);
        pImageContainer = GetContainer(i);
        pImageContainer->SetTransparent(1.0f);
        pImageContainer->SetShowBorder(true);
        pImageContainer->SetShowTracker(true);
        pImageContainer->SetShowRuler(HORIZ_RULER, true);
        pImageContainer->SetShowRuler(VERT_RULER, true);
        pImageContainer->SetFilled(true);
    }
    switch (m_Layout)
    {
    case VIEW_LAYOUT_NONE:
        break;
    case VIEW_LAYOUT_1_1_HORIZ:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width, size.Height / 2 - 1));
        m_pLayoutContainers[1]->MoveTo(0, size.Height / 2);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width, size.Height / 2 - 1));
        m_pLayoutContainers[2]->SetVisible(false);
        m_pLayoutContainers[3]->SetVisible(false);
        break;
    case VIEW_LAYOUT_1_2_HORIZ:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width, size.Height / 2 - 1));
        m_pLayoutContainers[1]->MoveTo(0, size.Height / 2);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[2]->MoveTo(size.Width / 2 + 1, size.Height / 2 + 1);
        m_pLayoutContainers[2]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[3]->SetVisible(false);
        break;
    case VIEW_LAYOUT_2_1_HORIZ:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[1]->MoveTo(size.Width / 2, 0);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[2]->MoveTo(0, size.Height / 2);
        m_pLayoutContainers[2]->SetSize(Gdiplus::Size(size.Width, size.Height / 2 - 1));
        m_pLayoutContainers[3]->SetVisible(false);
        break;
    case VIEW_LAYOUT_1_1_VERT:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width / 2, size.Height));
        m_pLayoutContainers[1]->MoveTo(size.Width / 2, 0);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width / 2, size.Height));
        m_pLayoutContainers[2]->SetVisible(false);
        m_pLayoutContainers[3]->SetVisible(false);
        break;
    case VIEW_LAYOUT_1_2_VERT:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height));
        m_pLayoutContainers[1]->MoveTo(size.Width / 2, 0);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[2]->MoveTo(size.Width / 2, size.Height / 2);
        m_pLayoutContainers[2]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[3]->SetVisible(false);
        break;
    case VIEW_LAYOUT_2_1_VERT:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[1]->MoveTo(0, size.Height / 2);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[2]->MoveTo(size.Width / 2, 0);
        m_pLayoutContainers[2]->SetSize(Gdiplus::Size(size.Width / 2, size.Height));
        m_pLayoutContainers[3]->SetVisible(false);
        break;
    case VIEW_LAYOUT_2x2:
        m_pLayoutContainers[0]->MoveTo(0, 0);
        m_pLayoutContainers[0]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[1]->MoveTo(size.Width / 2, 0);
        m_pLayoutContainers[1]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[2]->MoveTo(0, size.Height / 2);
        m_pLayoutContainers[2]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        m_pLayoutContainers[3]->MoveTo(size.Width / 2, size.Height / 2);
        m_pLayoutContainers[3]->SetSize(Gdiplus::Size(size.Width / 2 - 1, size.Height / 2 - 1));
        break;
    case VIEW_LAYOUT_SINGLE_FULL:
        if (m_pCurrentContainer)
        {
            for (i = 0; i < IMAGE_CONTAINER_COUNT; i++)
            {
                if (m_pLayoutContainers[i] != m_pCurrentContainer)
                {
                    m_pLayoutContainers[i]->SetVisible(false);
                }
            }
            m_pCurrentContainer->MoveTo(0, 0);
            m_pCurrentContainer->SetSize(size);
            m_pCurrentContainer->SetVisible(true);
        }
        break;
    default:
        break;
    }
    for (i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        pImageContainer = GetContainer(i);
        pImageContainer->SetSize(m_pLayoutContainers[i]->GetSize());
    }
}

void CObjectsView::OnDraw(Gdiplus::Graphics & graph)
{
    CObjectsContainer::OnDraw(graph);
    m_pBaseContainer->Draw(graph);
    m_LayeredObjects.Draw(graph);
}

void CObjectsView::Serialize(CArchive & ar)
{
    CObjectsContainer::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_Layout;
        ar << m_ViewState;
        ar << m_pBaseContainer;
        for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
        {
            ar << m_pLayoutContainers[i];
        }
    }
    else
    {
        ar >> m_Layout;
        m_LastLayout = m_Layout;
        ar >> m_ViewState;
        ar >> m_pBaseContainer;
        if (m_pBaseContainer)
        {
            m_pBaseContainer->SetParent(this);
        }
        for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
        {
            ar >> m_pLayoutContainers[i];
            m_pLayoutContainers[i]->SetParent(this);
            m_pRealContainers[i] = GetCurrentContainer(m_pLayoutContainers[i]);
            m_LayeredObjects.AddTail(m_pLayoutContainers[i]);
        }
    }
}

bool CObjectsView::HasAutoComparor(CImageContainer * pImageContainer)
{
    ASSERT(pImageContainer);
    CDrawObjectList & ObjList = pImageContainer->GetObjects();
    CDrawObject *pObject;
    POSITION pos = ObjList.GetHeadPosition();
    while (pos)
    {
        pObject = ObjList.GetNext(pos);
        if (pObject->IsKindOf(RUNTIME_CLASS(CAutoComparor)))
        {
            return true;
        }
    }
    return false;
}

bool CObjectsView::CanAutoCompare(bool * canAutoCompare)
{
    bool can[4];
    CanCompare(can);
    int autoCompare = 0;
    for (int i = 0; i < IMAGE_CONTAINER_COUNT; i++)
    {
        if (can[i] && HasAutoComparor(m_pRealContainers[i]))
        {
            if (canAutoCompare)
            {
                canAutoCompare[i] = TRUE;
            }
            autoCompare++;
        }
        else
        {
            if (canAutoCompare)
            {
                canAutoCompare[i] = FALSE;
            }
        }
    }
    if (autoCompare >= 2)
    {
        return true;
    }
    return false;
}

static Gdiplus::Point GetTriangleCenter(Gdiplus::Point * pt)
{
    Gdiplus::Point c;
    c.X = (int) ((pt[0].X + pt[1].X + pt[2].X) / 3.0);
    c.Y = (int) ((pt[0].Y + pt[1].Y + pt[2].Y) / 3.0);
    return c;
}
static void GetTriangleCentreAngles(Gdiplus::Point * pt, double *angles)
{
    ASSERT(angles && pt);
    Gdiplus::Point c = GetTriangleCenter(pt);
    for (int i = 0; i < 3; i++)
    {
        angles[i] = CMathUtility::CalculateLineAngle(pt[i].X, pt[i].Y, c.X, c.Y);
    }
}
static CAutoComparor *FindAutoComparor(CImageContainer * pImageContainer)
{
    ASSERT(pImageContainer);
    POSITION pos = pImageContainer->GetObjects().GetHeadPosition();
    CDrawObject *pObject;
    while (pos)
    {
        pObject = pImageContainer->GetObjects().GetNext(pos);
        if (pObject->IsKindOf(RUNTIME_CLASS(CAutoComparor)))
        {
            return (CAutoComparor *) pObject;
        }
    }
    return NULL;
}

void CObjectsView::OnAutoOverlayCompare()
{
    m_ViewState = VIEW_STATE_OVERLAP_COMPARE;
    OnOverlayCompare();

    CImageContainer *pImage1 = GetContainer(m_CompareImage1);
    CImageContainer *pImage2 = GetContainer(m_CompareImage2);

    double scale1 = pImage1->GetRulerScale();
    double scale2 = pImage1->GetRulerScale();
    ASSERT(scale1 != 0.0 && scale2 != 0.0);


    CAutoComparor *ac1 = FindAutoComparor(pImage1);
    CAutoComparor *ac2 = FindAutoComparor(pImage2);
    ASSERT(ac1 && ac2);
    Gdiplus::Point p1[3];
    Gdiplus::Point c1;

    Gdiplus::Point p2[3];
    Gdiplus::Point c2;

    double angles1[3];
    double angles2[3];


    ac1->GetVertexes(p1, 3);
    ac1->Local2Global(p1, 3);   //转到ImageContainer坐标系

    ac2->GetVertexes(p2, 3);
    ac2->Local2Global(p2, 3);


    GetTriangleCentreAngles(p1, angles1);
    GetTriangleCentreAngles(p2, angles2);

    pImage1->GetImageObject()->Global2Local(p1, 3);     //转到ImageObject坐标系
    pImage2->GetImageObject()->Global2Local(p2, 3);

    //c1=GetTriangleCenter(p1);
    //c2=GetTriangleCenter(p2);
    //pImage2->GetImageObject()->SetRotateCenter(c2);
    //double deltaAngle=(angles2[0] + angles2[1] + angles2[2] -angles1[0] - angles1[1] - angles1[2])/3;
    //pImage2->GetImageObject()->Rotate(deltaAngle);

    Gdiplus::Point offset = p1[0] - p2[0];
    pImage2->GetImageObject()->Offset(offset);
    ac2->SetLineColor(Gdiplus::Color::Red);
    ac2->Offset(offset);
    //ac2->Rotate(deltaAngle);
}

void CObjectsView::RestoreLayout()
{
    SetLayout(m_LastLayout);
}
