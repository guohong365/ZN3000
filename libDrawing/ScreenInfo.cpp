#include "stdafx.h"
#include <uc/drawing/ScreenInfo.h>

namespace uc
{
    namespace drawing{


        ScreenInfo ScreenInfo::_screenInfo;

        ScreenInfo::ScreenInfo()
        {
            const HDC hDC=GetDC(nullptr);
            _physicalWidth=GetDeviceCaps(hDC, HORZSIZE);
            _physicalHeight=GetDeviceCaps(hDC, VERTSIZE);
            _width=GetDeviceCaps(hDC, HORZRES);
            _height=GetDeviceCaps(hDC, VERTRES);
            _dpmmX= static_cast<float>(_width)/_physicalWidth;
            _dpmmY= static_cast<float>(_height)/_physicalHeight;
        }

        int ScreenInfo::GetPhysicalWidth() const
        {
            return _physicalWidth;
        }

        int ScreenInfo::GetPhysicalHeight() const
        {
            return _physicalHeight;
        }

        int ScreenInfo::GetWidth() const
        {
            return _width;
        }

        int ScreenInfo::GetHeight() const
        {
            return _height;
        }

        float ScreenInfo::GetDpmmX() const
        {
            return _dpmmX;
        }

        float ScreenInfo::GetDpmmY() const
        {
            return _dpmmY;
        }

        const ScreenInfo & ScreenInfo::GetScreenInfo()
        {
            return _screenInfo;
        }

    }
}
