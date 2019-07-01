#pragma once
#include <uc/libdrawing.h>
#include <uc/drawing/RectObject.h>

namespace uc
{
    namespace drawing{

        class DRAWING_CLASS LineObject : public RectObject
        {
        protected:
            LineObject();
        public:
            virtual ~LineObject();
            LineObject(const CString & name, const Gdiplus::Rect & rect);
            LineObject(const CString & name, int x, int y, int width, int height);
            LineObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size &size );

            virtual void OnDraw(Gdiplus::Graphics &graph);
            virtual void OnDrawBorder(Gdiplus::Graphics &graph);
            virtual void OnDrawFillObject(Gdiplus::Graphics &graph);

            virtual int GetHandleCount();
            virtual int HitTest(const Gdiplus::Point &pt);
            virtual DrawObject* CreateInstance(){return new LineObject();}
        };
    }
}
