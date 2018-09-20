#pragma  once

#include "DrawObject.h"
#include "RectObject.h"
#include "ImageObject.h"
#include "RulerObject.h"
#include "ObjectsContainer.h"
#include "ImageContainer.h"

#define VIEW_STATE_NONE            0
#define VIEW_STATE_NORMAL          1
#define VIEW_STATE_VERT_COMPARE    2
#define VIEW_STATE_HORIZ_COMPARE   3
#define VIEW_STATE_OVERLAP_COMPARE 4

#define VIEW_LAYOUT_NONE      0
#define VIEW_LAYOUT_1_1_HORIZ 1
#define VIEW_LAYOUT_1_1_VERT  2
#define VIEW_LAYOUT_1_2_HORIZ 3
#define VIEW_LAYOUT_1_2_VERT  4
#define VIEW_LAYOUT_2_1_VERT  5
#define VIEW_LAYOUT_2_1_HORIZ 6
#define VIEW_LAYOUT_2x2       7
#define VIEW_LAYOUT_SINGLE_FULL 8

#define IMAGE_CONTAINER_COUNT 4


class CObjectsView:public CObjectsContainer
{
protected:                   // create from serialization only
    CObjectsView();
    void Initailize();
    DECLARE_SERIAL(CObjectsView) public:
    CObjectsView(const CString & name);
    void RestoreLayout();


    // Operations
public:
    virtual ~ CObjectsView();
    void CObjectsView::SetCompareImageIndex(long image1, long image2);
    void CObjectsView::GetComareImageIndex(long &image1, long &image2);
    //////////////////////////////////////////////////////////////////////////
    //属性操作
    void SetLayout(long layout);
    long GetLayout() const;
    virtual void OnLayoutChanging(long &layout);
    virtual void OnLayoutChanged();

    void SetViewState(long state);
    long GetViewState() const;
    virtual void OnViewStateChanging(long &state);
    virtual void OnViewStateChanged();

    //////////////////////////////////////////////////////////////////////////
    //数据操作

    //取当前坐标对象。
    //按先全局对象，后分层容器对象
    CDrawObject *ObjectAt(const Gdiplus::Point & point);
    void Add(int nItem, CDrawObject * pObj);
    void Remove(int nItem, CDrawObject * pObj);


    int GetImageCount();
    void OnVertCompare();
    void OnHorizCompare();
    void OnOverlayCompare();
    void OnCancelCompare();


    virtual void Serialize(CArchive & ar);

    // Implementation
public:
    //////////////////////////////////////////////////////////////////////////
    virtual void OnSizeChanged();

    CDrawObjectList & GetLayer();
    bool IsEmpty(int nItem);
    bool CanCompare(bool * canCompare = NULL);

    void ClearContainer(CImageContainer * pItem);
    void ClearContainer(int nItem);

    int GetContainerIndex(CImageContainer * pImageContainer);

    void ActiveContainer(int nItem, bool isActive);
    void ActiveContainer(CImageContainer * pItem, bool isActive);
    CImageContainer *ActiveContainer(const Gdiplus::Point & point);
    CImageContainer *GetCurrentContainer(CObjectsContainer * pContainer);
    CImageContainer *GetContainer(int nItem);
    CImageContainer *GetContainer(const Gdiplus::Point point);
    void ExchangeImage(CDrawObject * pSrcImageContainer, CDrawObject * pDestImageContainer);
    void ArrangeImages();

    CImageContainer *GetActiveContainer();
    //使用第nItem个图片容器替换当前活动容器位置
    //CImageContainer * ReplaceActiveContainer(int nItem);

    //CImageContainer * ReplaceActiveContainer(CImageContainer *pItem);

    CObjectsContainer *GetBaseContainer();

    bool HasAutoComparor(CImageContainer * pImageContainer);

    bool CanAutoCompare(bool * canAutoCompare = NULL);
    void OnAutoOverlayCompare();

    void RecalculateLayout();

    void OnDraw(Gdiplus::Graphics & graph);

    //当前布局模式
    long m_Layout;

    //文档状态
    long m_ViewState;

    CDrawObjectList m_LayeredObjects;
    CObjectsContainer *m_pLayoutContainers[IMAGE_CONTAINER_COUNT];
    CImageContainer *m_pRealContainers[IMAGE_CONTAINER_COUNT];
    CObjectsContainer *m_pBaseContainer;
    CObjectsContainer *m_pCurrentContainer;
    long m_LastLayout;

    long m_CompareImage1;
    long m_CompareImage2;


};
