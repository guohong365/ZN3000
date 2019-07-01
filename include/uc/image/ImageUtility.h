#ifndef __IMAGE_UTILITY_H__
#define __IMAGE_UTILITY_H__

#include <uc/libdrawing.h>
#include <GdiPlus.h>
#include <atlimage.h>

DRAWING_API Gdiplus::Bitmap* CreateIconizedImage(Gdiplus::Bitmap* input, int width, int height,
                                                const Gdiplus::Color& background = static_cast<Gdiplus::ARGB>(Gdiplus::
	                                                Color::Transparent));
DRAWING_API Gdiplus::Bitmap* BitmapTransparent(Gdiplus::Bitmap* pSrc, Gdiplus::Color transparentColor, bool clearForeground,
                                   Gdiplus::Color foregroundColor, int error = 0);
DRAWING_API void CalcAspectScale(const Gdiplus::Size &source, const Gdiplus::Size &dest, Gdiplus::Size &result, float &scale);
DRAWING_API Gdiplus::Image *  LoadThumbImage(const WCHAR *fileName, int targetRectWidth, int targetRectHeight);
DRAWING_API Gdiplus::Image * LoadThumbImage(const WCHAR *fileName, const Gdiplus::Rect& pTargetRect);
DRAWING_API Gdiplus::Image * GetThumbImage(Gdiplus::Image *pSrc, const Gdiplus::Rect &pTargetRect);
DRAWING_API CImage * LoadThumbnailImage2(const WCHAR *fileName, int width, int height);
DRAWING_API CBitmap * LoadThumbnailImage3(const WCHAR *fileName, int width, int height);
DRAWING_API Gdiplus::Bitmap * LoadBitmapFromFile(const WCHAR * fileName);
DRAWING_API Gdiplus::Bitmap * LoadBitmapFromBuffer(const char * buffer, long bufferLength);
DRAWING_API Gdiplus::Bitmap * LoadBitmapFromHGlobal(HGLOBAL hGlobal);

DRAWING_API int SaveImageToFile(const WCHAR * filename,Gdiplus::Image *pImage, const WCHAR * type=L"image/png");
DRAWING_API HGLOBAL SaveBitmapToHGlobal(Gdiplus::Bitmap *pBitmap, const WCHAR* type=L"image/jpeg");

DRAWING_API Gdiplus::Bitmap * GetThumbnail(Gdiplus::Image* pSource, int width, int height);
DRAWING_API void BuildCombobox(CComboBox & cb, CStringList & strList);
DRAWING_API Gdiplus::Color FromRGB(COLORREF rgbColor);
DRAWING_API Gdiplus::Bitmap * BitmapClone(Gdiplus::Image * pImage);
DRAWING_API Gdiplus::Bitmap * BitmapCreate(int width, int height, Gdiplus::PixelFormat pixelFormat);
DRAWING_API Gdiplus::Bitmap * BitmapToGray(Gdiplus::Bitmap * pBitmap);
DRAWING_API Gdiplus::Bitmap * BitmapToBlackWhite(Gdiplus::Bitmap *pBitmap, int threshold= 128);
DRAWING_API Gdiplus::Bitmap * BitmapToBlackWhiteAverageGray(Gdiplus::Bitmap *pBitmap);
DRAWING_API Gdiplus::Bitmap * BitmapSetColor(Gdiplus::Bitmap * pBitmap, Gdiplus::Color);
DRAWING_API void RGB2HSL(Gdiplus::Color color, double &H,double &S,double &L);
DRAWING_API Gdiplus::Color HSL2RGB(double H,double S,double L);

#endif //__IMAGE_UTILITY_H__