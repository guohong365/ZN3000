#pragma once
#include <uc/libdrawing.h>
#include <GdiPlus.h>

namespace uc
{
    namespace drawing{


        class DRAWING_CLASS GdiplusEnvironment 
        {
            ULONG_PTR _gdiPlusToken;
            Gdiplus::GdiplusStartupInput _gdiPlusStartupInput;
        public:
            GdiplusEnvironment();
            ~GdiplusEnvironment();
        };
    }
}