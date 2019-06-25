#include "stdafx.h"

#include <uc/drawing/GdiplusEnviroment.h>
namespace uc
{
    namespace drawing{

        GdiplusEnvironment::GdiplusEnvironment()
        {
            GdiplusStartup(&_gdiPlusToken, &_gdiPlusStartupInput, nullptr);
        }

        GdiplusEnvironment::~GdiplusEnvironment()
        {
            Gdiplus::GdiplusShutdown(_gdiPlusToken);
        }
    }
}
