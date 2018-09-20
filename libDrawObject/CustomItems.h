

#pragma once
#include <GdiPlus.h>

class CCustomItemPicture:public CXTPPropertyGridItem
{
public:
    CCustomItemPicture(CString strCaption, Gdiplus::Image * pImage = NULL);
    CCustomItemPicture(UINT ID, Gdiplus::Image * pImage = NULL);
    ~CCustomItemPicture(void);

    Gdiplus::Image * GetPicture();
protected:
    virtual BOOL OnDrawItemValue(CDC & dc, CRect rcValue);
    virtual void OnInplaceButtonDown();

private:
    Gdiplus::Image * m_pImage;
};

class CCustomItemSpin;

class CCustomItemSpinInplaceButton:public CSpinButtonCtrl
{
    friend class CCustomItemSpin;
    CCustomItemSpin *m_pItem;
    DECLARE_MESSAGE_MAP() public:
    afx_msg void OnDeltapos(NMHDR * pNMHDR, LRESULT * pResult);
};

class CCustomItemSpin:public CXTPPropertyGridItemNumber
{
    friend class CCustomItemSpinInplaceButton;
public:
    CCustomItemSpin(CString strCaption);


protected:
    virtual void OnDeselect();
    virtual void OnSelect();
    virtual CRect GetValueRect();


private:
    CCustomItemSpinInplaceButton m_wndSpin;
};


class CCustomItemFileBox:public CXTPPropertyGridItem
{
public:
    CCustomItemFileBox(CString strCaption);


protected:
    virtual void OnInplaceButtonDown();
};



class CCustomItemCheckBox;

class CInplaceCheckBox:public CButton
{
public:
    afx_msg LRESULT OnCheck(WPARAM wParam, LPARAM lParam);
    afx_msg HBRUSH CtlColor(CDC * pDC, UINT /*nCtlColor */ );

    DECLARE_MESSAGE_MAP() protected:
    CCustomItemCheckBox * m_pItem;
    COLORREF m_clrBack;
    CBrush m_brBack;

    friend class CCustomItemCheckBox;
};

class CCustomItemCheckBox:public CXTPPropertyGridItem
{
protected:

public:
    CCustomItemCheckBox(CString strCaption);

    BOOL GetBool();
    void SetBool(BOOL bValue);

protected:
    virtual void OnDeselect();
    virtual void OnSelect();
    virtual CRect GetValueRect();
    virtual BOOL OnDrawItemValue(CDC & dc, CRect rcValue);



private:
    CInplaceCheckBox m_wndCheckBox;
    BOOL m_bValue;

    friend class CInplaceCheckBox;
};



class CCustomItemChilds:public CXTPPropertyGridItem
{
    class CCustomItemChildsAll;
    class CCustomItemChildsPad;

    friend class CCustomItemChildsAll;
    friend class CCustomItemChildsPad;

public:
    CCustomItemChilds(CString strCaption, CRect rcValue);

protected:
    virtual void OnAddChildItem();
    virtual void SetValue(CString strValue);

private:
    void UpdateChilds();
    CString RectToString(CRect rc);


private:
    CCustomItemChildsAll * m_itemAll;
    CCustomItemChildsPad *m_itemLeft;
    CCustomItemChildsPad *m_itemTop;
    CCustomItemChildsPad *m_itemRight;
    CCustomItemChildsPad *m_itemBottom;

    CRect m_rcValue;
};




class CCustomItemColor:public CXTPPropertyGridItemColor
{
public:
    CCustomItemColor(CString strCaption, COLORREF clr = 0);

protected:
    virtual void OnInplaceButtonDown();

#ifndef _DECIMAL_VALUE
public:
    COLORREF StringToRGB(CString str);

    CString RGBToString(COLORREF clr);

    void SetValue(CString strValue);

    void SetColor(COLORREF clr);
#endif
};




//////////////////////////////////////////////////////////////////////////


class CInplaceUpperCase:public CXTPPropertyGridInplaceEdit
{
public:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    DECLARE_DYNAMIC(CInplaceUpperCase);
    DECLARE_MESSAGE_MAP()};


    class CCustomItemUpperCase:public CXTPPropertyGridItem
    {
    public:
        CCustomItemUpperCase(CString strCaption);

    protected:
        virtual CXTPPropertyGridInplaceEdit & GetInplaceEdit();

    private:
        CInplaceUpperCase m_wndEdit;

    };






    class CCustomItemSlider;

    class CInplaceSlider:public CSliderCtrl
    {
    public:
        afx_msg HBRUSH CtlColor(CDC * pDC, UINT /*nCtlColor */ );
        afx_msg void OnCustomDraw(NMHDR * pNMHDR, LRESULT * pResult);

        DECLARE_MESSAGE_MAP() protected:
        CCustomItemSlider * m_pItem;
        COLORREF m_clrBack;
        CBrush m_brBack;
        int m_nValue;

        friend class CCustomItemSlider;
    };

    class CCustomItemSlider:public CXTPPropertyGridItemNumber
    {
    protected:

    public:
        CCustomItemSlider(CString strCaption, long initNumber = 0);
        CCustomItemSlider(UINT nID, long initNumber = 0);

    protected:
        virtual void OnDeselect();
        virtual void OnSelect();

    private:
        CInplaceSlider m_wndSlider;
        int m_nWidth;

        friend class CInplaceSlider;
    };
