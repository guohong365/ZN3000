#pragma once

#include <uc/libuc.h>

namespace uc
{
    namespace drawing{

        class LIB_UC_API ScreenInfo
        {
            int _physicalWidth;	//width in mm
            int _physicalHeight;	//height in mm
            int _width;			//width in pixel
            int _height;		//height in pixel
            float _dpmmX;        //pixel per mm 
            float _dpmmY;
            static ScreenInfo _screenInfo;
        public:
            ScreenInfo();
            int GetPhysicalWidth() const;
            int GetPhysicalHeight() const;
            int GetWidth() const;
            int GetHeight() const;
            float GetDpmmX() const;
            float GetDpmmY() const;
            static const ScreenInfo & GetScreenInfo();
        };
    }
}