#pragma once
#include <uc/libdrawing.h>
#include <GdiPlus.h>

namespace uc
{
    namespace drawing{

        class DRAWING_CLASS UICoordinateHelper
        {
            bool _canRelease;
        public:
            int HorizontalSize;
            int VerticalSize;
            int HorizontalRes;
            int VerticalRes;
            double HorizontalLmPerDeviceUnit;
            double VerticalLmPerDeviceUnit;
            int Separate;

            explicit UICoordinateHelper(HDC hdc);
            static UICoordinateHelper& GetHelper(HDC hdc= nullptr);
            void Release();
            void DPtoLP(POINT* pPoints, int count, int scale=100) const;
            void DPtoLP(Gdiplus::Point *pPoints, int count, int scale=100) const;
            void DPtoLP(RECT* pRects, int count, int scale=100) const;
            void DPtoLP(Gdiplus::Rect *pRects, int count, int scale=100) const;
            void LPtoDP(POINT* pPoints, int count, int scale=100) const;
            void LPtoDP(Gdiplus::Point *pPoints, int count, int scale=100) const;
            void LPtoDP(RECT* pRects, int count, int scale=100) const;
            void LPtoDP(Gdiplus::Rect *pRects, int count, int scale=100) const;
            int DHLtoLHL(int length /*in 0.1 mm unit*/, int scale=100) const;
            int DVLtoLVL(int length, int scale=100) const;
            int LHLtoDHL(int length, int scale = 100) const;
            int LVLtoDVL(int length, int scale = 100) const;
            void DPtoLP(SIZE* pSize, int count, int scale=100) const;
            void DPtoLP(Gdiplus::Size* pSize, int count, int scale=100)const;
            void LPtoDP(SIZE* pSize, int count , int scale=100) const;
            void LPtoDP(Gdiplus::Size* pSize, int count, int scale=100)const;
        };
    }
}