#ifndef __IMAGE_UTILITY_H__
#define __IMAGE_UTILITY_H__
#include <uc/libuc.h>
#include <GdiPlus.h>
#include <atlimage.h>

LIB_UC_API Gdiplus::Bitmap* CreateIconizedImage(Gdiplus::Bitmap* input, int width, int height,
                                                const Gdiplus::Color& background = static_cast<Gdiplus::ARGB>(Gdiplus::
	                                                Color::Transparent));
LIB_UC_API Gdiplus::Bitmap* BitmapTransparent(Gdiplus::Bitmap* pSrc, Gdiplus::Color transparentColor, bool clearForeground,
                                   Gdiplus::Color foregroundColor, int error = 0);
LIB_UC_API void CalcAspectScale(const Gdiplus::Size &source, const Gdiplus::Size &dest, Gdiplus::Size &result, float &scale);
LIB_UC_API Gdiplus::Image *  LoadThumbImage(const WCHAR *fileName, int targetRectWidth, int targetRectHeight);
LIB_UC_API Gdiplus::Image * LoadThumbImage(const WCHAR *fileName, const Gdiplus::Rect& pTargetRect);
LIB_UC_API Gdiplus::Image * GetThumbImage(Gdiplus::Image *pSrc, const Gdiplus::Rect &pTargetRect);
LIB_UC_API CImage * LoadThumbnailImage2(const WCHAR *fileName, int width, int height);
LIB_UC_API CBitmap * LoadThumbnailImage3(const WCHAR *fileName, int width, int height);
LIB_UC_API Gdiplus::Bitmap * LoadBitmapFromFile(const WCHAR * fileName);
LIB_UC_API Gdiplus::Bitmap * LoadBitmapFromBuffer(const char * buffer, long bufferLength);
LIB_UC_API Gdiplus::Bitmap * LoadBitmapFromHGlobal(HGLOBAL hGlobal);

LIB_UC_API int SaveImageToFile(const WCHAR * filename,Gdiplus::Image *pImage, const WCHAR * type=L"image/png");
LIB_UC_API HGLOBAL SaveBitmapToHGlobal(Gdiplus::Bitmap *pBitmap, const WCHAR* type=L"image/jpeg");

LIB_UC_API Gdiplus::Bitmap * GetThumbnail(Gdiplus::Image* pSource, int width, int height);
LIB_UC_API void BuildCombobox(CComboBox & cb, CStringList & strList);
LIB_UC_API Gdiplus::Color FromRGB(COLORREF rgbColor);
LIB_UC_API Gdiplus::Bitmap * BitmapClone(Gdiplus::Image * pImage);
LIB_UC_API Gdiplus::Bitmap * BitmapCreate(int width, int height, Gdiplus::PixelFormat pixelFormat);
LIB_UC_API Gdiplus::Bitmap * BitmapToGray(Gdiplus::Bitmap * pBitmap);
LIB_UC_API Gdiplus::Bitmap * BitmapToBlackWhite(Gdiplus::Bitmap *pBitmap, int threshold= 128);
LIB_UC_API Gdiplus::Bitmap * BitmapToBlackWhiteAverageGray(Gdiplus::Bitmap *pBitmap);
LIB_UC_API Gdiplus::Bitmap * BitmapSetColor(Gdiplus::Bitmap * pBitmap, Gdiplus::Color);
LIB_UC_API void RGB2HSL(Gdiplus::Color color, double &H,double &S,double &L);
LIB_UC_API Gdiplus::Color HSL2RGB(double H,double S,double L);

#endif //__IMAGE_UTILITY_H__