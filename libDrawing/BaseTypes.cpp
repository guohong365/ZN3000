#include "stdafx.h"
#include <uc/drawing/BaseTypes.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace uc
{
    namespace drawing
    {
        ATTR_VALUE::ATTR_VALUE() :_type(VALUE_EMPTY)
        {
            ZeroMemory(&_val, sizeof(_val));
        }

        ATTR_VALUE::ATTR_VALUE( const UCHAR val ) :_type(VALUE_UCHAR)
        {
            _val._ucVal=val;
        }

        ATTR_VALUE::ATTR_VALUE( const INT32 val ) :_type(VALUE_INT32)
        {
            _val._i32Val=val;
        }

        ATTR_VALUE::ATTR_VALUE( const INT64 val ) :_type(VALUE_INT64)
        {
            _val._i64Val=val;
        }

        ATTR_VALUE::ATTR_VALUE( const float val ) :_type(VALUE_FLOAT)
        {
            _val._fVal=val;
        }

        ATTR_VALUE::ATTR_VALUE( const double val ) :_type(VALUE_DOUBLE)
        {
            _val._dVal=val;
        }

        ATTR_VALUE::ATTR_VALUE( const bool val ) :_type(VALUE_BOOL)
        {
            _val._bVal;
        }

        ATTR_VALUE::ATTR_VALUE( const Gdiplus::Size &size ) :_type(VALUE_SIZE)
        {
            _val._szVal=new Gdiplus::Size(size);
        }

        ATTR_VALUE::ATTR_VALUE( const Gdiplus::Point &point ) :_type(VALUE_POINT)
        {
            _val._ptVal=new Gdiplus::Point(point);
        }

        ATTR_VALUE::ATTR_VALUE( const Gdiplus::Rect &rect )
        {
            _val._rcVal=new Gdiplus::Rect(rect);
        }

        ATTR_VALUE::ATTR_VALUE( LPCTSTR str ) :_type(VALUE_STRING)
        {
            _val._strVal=new CString;
            *(_val._strVal)= str ? str:_T("");
        }

        ATTR_VALUE::ATTR_VALUE( Gdiplus::Image * pImage ) :_type(VALUE_IMAGE)
        {
            _val._imgVal=pImage;
        }

        ATTR_VALUE::ATTR_VALUE( Gdiplus::ColorMatrix colorMatrix ) :_type(VALUE_COLOR_MATRIX)
        {
            _val._cmVal=new Gdiplus::ColorMatrix(colorMatrix);
        }

        ATTR_VALUE::ATTR_VALUE( POLYLINE_NODE_INFO polylineNode ) :_type(VALUE_POLYLINE_NODE)
        {
            _val._pnVal=new POLYLINE_NODE_INFO(polylineNode);
        }

        ATTR_VALUE::~ATTR_VALUE()
        {
            Release();
        }

        void ATTR_VALUE::Release()
        {
            switch(_type)
            {
            case VALUE_STRING:
                delete _val._strVal;
                _type=VALUE_EMPTY;
            case VALUE_SIZE:
                delete _val._szVal;
                _type=VALUE_EMPTY;
                break;
            case VALUE_POINT:
                delete _val._ptVal;
                _type=VALUE_EMPTY;
                break;
            case VALUE_RECT:
                delete _val._rcVal;
                _type=VALUE_EMPTY;
                break;
            case VALUE_COLOR_MATRIX:
                delete _val._cmVal;
                _type=VALUE_EMPTY;
                break;
            case VALUE_POLYLINE_NODE:
                delete _val._pnVal;
                _type=VALUE_EMPTY;
                break;
            }
        }

        void ATTR_VALUE::finalRelease()
        {
            if(_type==VALUE_IMAGE)
            {
                delete _val._imgVal;
                _type=VALUE_EMPTY;
            }
            else
            {
                Release();
            }
            ZeroMemory(&_val, sizeof(_val));
        }
    }
}