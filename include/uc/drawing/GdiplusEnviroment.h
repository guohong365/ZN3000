#pragma once
#include <uc/libuc.h>
#include <GdiPlus.h>

namespace uc
{
    namespace drawing{


        class LIB_UC_API GdiplusEnvironment 
        {
            ULONG_PTR _gdiPlusToken;
            Gdiplus::GdiplusStartupInput _gdiPlusStartupInput;
        public:
            GdiplusEnvironment();
            ~GdiplusEnvironment();
        };
    }
}