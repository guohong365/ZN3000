#pragma once

enum
{
    DNM_SIZE_CHANGED,
    DNM_POSITION_CHANGED,
    DNM_ANGLE_CHANGED,
    DNM_SIZE_CHANGING,
    DNM_POSITION_CHANGING,
    DNM_ANGLE_CHANGING,
    DNM_ROTATE_CENTER_CHANGING,
    DNM_ROTATE_CENTER_CHANGED,
    DNM_IMAGE_CHANGING,
    DNM_IMAGE_CHANGED,
    DNM_SELECTED_CHANGING,
    DNM_SELECTED_CHANGED,
    DNM_PARENT_CHANGING,
    DNM_PARENT_CHANGED,
    DNM_NAME_CHANGING,
    DNM_NAME_CHANGED,
    DNM_CLIP_RECT_CHANGING,
    DNM_CLIP_RECT_CHANGED,
    DNM_USING_CLIP_CHANGING,
    DNM_USING_CLIP_CHANGED,
    DNM_VISIBLE_CHANGING,
    DNM_VISIBLE_CHANGED,
    DNM_FILLED_CHANGING,
    DNM_FILLED_CHANGED,
    DNM_SHOW_BORDER_CHANGING,
    DNM_SHOW_BORDER_CHANGED,
    DNM_SHOW_TRACKER_CHANGING,
    DNM_SHOW_TRACKER_CHANGED,
    DNM_TRACKER_COLOR_CHANGING,
    DNM_TRACKER_COLOR_CHANGED,
    DNM_TRANSPARENT_CHANGING,
    DNM_TRANSPARENT_CHANGED,
    DNM_FILL_COLOR_CHANGING,
    DNM_FILL_COLOR_CHANGED,
    DNM_LINE_COLOR_CHANGING,
    DNM_LINE_COLOR_CHANGED,
    DNM_LINE_WIDTH_CHANGING,
    DNM_LINE_WIDTH_CHANGED,
    DNM_FONT_COLOR_CHANGING,
    DNM_FONT_COLOR_CHANGED,
    DNM_LINE_STYLE_CHANGING,
    DNM_LINE_STYLE_CHANGED,
    DNM_ACTIVE_COLOR_CHANGING,
    DNM_ACTIVE_COLOR_CHANGED,
    DNM_ACTIVE_CHANGING,
    DNM_ACTIVE_CHANGED,
    DNM_FONT_FACE_CHANGING,
    DNM_FONT_FACE_CHANGED,
    DNM_FONT_STYLE_CHANGING,
    DNM_FONT_STYLE_CHANGED,
    DNM_FONT_SIZE_CHANGING,
    DNM_FONT_SIZE_CHANGED,
    DNM_TEXT_ALIGN_CHANGING,
    DNM_TEXT_ALIGN_CHANGED,
    DNM_TEXT_LINE_ALIGN_CHANGING,
    DNM_TEXT_LINE_ALIGN_CHANGED,
    DNM_TEXT_VERT_CHANGING,
    DNM_TEXT_VERT_CHANGED,
    DNM_RULER_SCALE_CHANGING,
    DNM_RULER_SCALE_CHANGED,
    DNM_SHOW_RULER_CHANGING,
    DNM_SHOW_RULER_CHANGED,
    DNM_SYMBOL_DATA_CHANGING,
    DNM_SYMBOL_DATA_CHANGED,
    DNM_IMAGE_FLIPTYPE_CHANGING,
    DNM_IMAGE_FLIPTYPE_CHANGED,
    DNM_IMAGE_COLOR_MATRIX_CHANGING,
    DNM_IMAGE_COLOR_MATRIX_CHANGED,
    DNM_IMAGE_SHOW_GRAY_CHANGING,
    DNM_IMAGE_SHOW_GRAY_CHANGED,
    DNM_IMAGE_SHOW_INVERT_CHANGING,
    DNM_IMAGE_SHOW_INVERT_CHANGED,
    DNM_IMAGE_CHANNEL_CHANGING,
    DNM_IMAGE_CHANNEL_CHANGED,
    DNM_IMAGE_FILENAME_CHANGING,
    DNM_IMAGE_FILENAME_CHANGED,
    DNM_RULER_COLOR_CHANGING,
    DNM_RULER_COLOR_CHANGED,
    DNM_CONTAINER_SHOW_NAME_CHANGING,
    DNM_CONTAINER_SHOW_NAME_CHANGED,
    DNM_VERTEX_CHANGING,
    DNM_VERTEX_CHANGED,
    DNM_RULER_UNIT_CHANGING,
    DNM_RULER_UNIT_CHANGED,
    DNM_RULER_ALIGN_CHANGING,
    DNM_RULER_ALIGN_CHANGED,
    DNM_RULER_MINI_GRADUATION_INTERVAL_CHANGING,
    DNM_RULER_MINI_GRADUATION_INTERVAL_CHANGED,
    DNM_RULER_GRADUATION_FACTOR_CHANGING,
    DNM_RULER_GRADUATION_FACTOR_CHANGED,
    DNM_RULER_GRADUATION_POS_CHANGING,
    DNM_RULER_GRADUATION_POS_CHANGED,
    DNM_RULER_GRADUATION_INTERVAL_CHANGING,
    DNM_RULER_GRADUATION_INTERVAL_CHANGED,
    DNM_RULER_SHOW_NUMBER_CHANGING,
    DNM_RULER_SHOW_NUMBER_CHANGED,
    DNM_RULER_INC_DIR_CHANGING,
    DNM_RULER_INC_DIR_CHANGED,
    DNM_RULER_NUMBER_INTERVAL_CHANGING,
    DNM_RULER_NUMBER_INTERVAL_CHANGED,
    DNM_RULER_NUMBER_POS_CHANGING,
    DNM_RULER_NUMBER_POS_CHANGED,
    DNM_RULER_NUMBER_FACTOR_CHANGING,
    DNM_RULER_NUMBER_FACTOR_CHANGED,
    DNM_RULER_UNIT_NAME_CHANGING,
    DNM_RULER_UNIT_NAME_CHANGED,
    DNM_SYMBOL_CATEGORY_CHANGING,
    DNM_SYMBOL_CATEGORY_CHANGED,
    DNM_SELECTABLE_CHANGING,
    DNM_SELECTABLE_CHANGED,
    DNM_MOVABLE_CHANGING,
    DNM_MOVABLE_CHANGED,
    DNM_SIZABLE_CHANGING,
    DNM_SIZABLE_CHANGED,
    DNM_ROTATABLE_CHANGING,
    DNM_ROTATABLE_CHANGED
};

#define POLYLINE_NODE_NONE      0
#define POLYLINE_NODE_ADD       1
#define POLYLINE_NODE_DELETE    2
#define POLYLINE_NODE_MODIFY    3

struct POLYLINE_NODE_INFO
{
    int _action;
    int _index;
    Gdiplus::Point _pt;
    POLYLINE_NODE_INFO(const POLYLINE_NODE_INFO & polyNode)
    {
        _action=polyNode._action;
        _index=polyNode._index;
        _pt=polyNode._pt;
    }
    POLYLINE_NODE_INFO(int action, int index, Gdiplus::Point pt)
        :_action(action)
        ,_index(index)
        ,_pt(pt)
    {

    }
};

enum VALUE_TYPE
{
    VALUE_EMPTY,
    VALUE_IMAGE,
    VALUE_STRING,
    VALUE_UCHAR,
    VALUE_FLOAT,
    VALUE_DOUBLE,
    VALUE_INT32,
    VALUE_INT64,
    VALUE_BOOL,
    VALUE_SIZE,
    VALUE_POINT,
    VALUE_RECT,
    VALUE_COLOR_MATRIX,
    VALUE_POLYLINE_NODE
};
struct ATTR_VALUE
{
    VALUE_TYPE _type;
    union
    {
        UCHAR _ucVal;
        INT32 _i32Val;
        INT64 _i64Val;
        float _fVal;
        double _dVal;
        bool _bVal;
        Gdiplus::Size  *_szVal;
        Gdiplus::Point *_ptVal;
        Gdiplus::Rect  *_rcVal;
        Gdiplus::Image * _imgVal;
        Gdiplus::ColorMatrix *_cmVal;
        CString *_strVal;
        POLYLINE_NODE_INFO *_pnVal;
    } _val;
    ATTR_VALUE();
    ATTR_VALUE(const UCHAR val);
    ATTR_VALUE(const INT32 val);
    ATTR_VALUE(const INT64 val);
    ATTR_VALUE(const float val);
    ATTR_VALUE(const double val);
    ATTR_VALUE(const bool val);
    ATTR_VALUE(const Gdiplus::Size &size);
    ATTR_VALUE(const Gdiplus::Point &point);
    ATTR_VALUE(const Gdiplus::Rect &rect);
    ATTR_VALUE(LPCTSTR str);
    ATTR_VALUE(Gdiplus::Image * pImage);
    ATTR_VALUE(Gdiplus::ColorMatrix colorMatrix);
    ATTR_VALUE(POLYLINE_NODE_INFO polylineNode);
    ~ATTR_VALUE();
    void Release();
    operator UCHAR()
    {
        ASSERT(_type==VALUE_UCHAR);
        return _val._ucVal;
    }
    operator INT32()
    {
        ASSERT(_type==VALUE_INT32);
        return _val._i32Val;
    }
    operator INT64()
    {
        ASSERT(_type==VALUE_INT64);
        return _val._i64Val;
    }
    operator float()
    {
        ASSERT(_type==VALUE_FLOAT);
        return _val._fVal;
    }
    operator double()
    {
        ASSERT(_type==VALUE_DOUBLE);
        return _val._dVal;
    }
    operator bool()
    {
        ASSERT(_type==VALUE_BOOL);
        return _val._bVal;
    }
    operator Gdiplus::Size()
    {
        ASSERT(_type==VALUE_SIZE);
        return *_val._szVal;
    }
    operator Gdiplus::Point()
    {
        ASSERT(_type==VALUE_POINT);
        return *_val._ptVal;
    }
    operator Gdiplus::Rect()
    {
        ASSERT(_type==VALUE_RECT);
        return *_val._rcVal;
    }
    operator Gdiplus::ColorMatrix()
    {
        ASSERT(_type==VALUE_COLOR_MATRIX);
        return *_val._cmVal;
    }
    operator CString() 
    {
        ASSERT(_type==VALUE_STRING);
        return (*_val._strVal);       
    }
    operator Gdiplus::Image*()
    {
        ASSERT(_type==VALUE_IMAGE);
        return _val._imgVal;
    }
    operator POLYLINE_NODE_INFO()
    {
        ASSERT(_type==VALUE_POLYLINE_NODE);
        return *_val._pnVal;
    }

    void finalRelease();


};


enum CONNECTOR_ARCHOR_TYPE
{
    ToNone,
    ToLeft,
    ToUp,
    ToRight,
    ToDown
};