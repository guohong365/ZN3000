#ifndef __IMAGE_UTILITY_H__
#define __IMAGE_UTILITY_H__
#include <GdiPlus.h>
#include <atlimage.h>
class GdiplusEnveriment
{
	ULONG_PTR _gdiplusToken;
    Gdiplus::GdiplusStartupInput _gdiplusStartupInput;
public:
	GdiplusEnveriment()
	{

		Gdiplus::GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL);
	}
	~GdiplusEnveriment()
	{
		Gdiplus::GdiplusShutdown(_gdiplusToken);
	}
};
class ScreenInfo
{
	int _physicWidth;	//width in mm
	int _physicHeight;	//height in mm
	int _width;			//width in pixel
	int _height;		//height in pixel
	float _dpmmX;        //pixel per mm 
	float _dpmmY;
	static ScreenInfo _screenInfo;
public:
	ScreenInfo();
	int GetPhysicWidth();
	int GetPhysicHeight();
	int GetWidth();
	int GetHeight();
	float GetDpmmX() const;
	float GetDpmmY() const;
	static const ScreenInfo & GetScreenInfo();
};
int GetEncoderClsid(const WCHAR * format, CLSID * pClsid);
Gdiplus::Bitmap * CreateIconizeImage(Gdiplus::Bitmap * input, int width, int height,const Gdiplus::Color &background=(Gdiplus::ARGB)Gdiplus::Color::Transparent);
Gdiplus::Bitmap * BitmapTransparent(Gdiplus::Bitmap * pSrc, Gdiplus::Color transparentColor, bool clearForground, Gdiplus::Color foreground, int error=0);
void CalcAspectScale(const Gdiplus::Size &source, const Gdiplus::Size &dest, Gdiplus::Size &result, float &scale);
Gdiplus::Image * LoadThumbImage(const WCHAR *filename, int TagetRectWidth, int TagetRectHeight);
Gdiplus::Image * LoadThumbImage(const WCHAR *filename, const Gdiplus::Rect& pTagetRect);
Gdiplus::Image * GetThumbImage(Gdiplus::Image *pSrc, const Gdiplus::Rect &pTagetRect);
CImage *LoadThumImage2(const WCHAR *filname, int width, int height);
CBitmap *LoadThumImage3(const WCHAR *filname, int width, int height);
Gdiplus::Bitmap *LoadBitmapFromFile(const WCHAR * fileName);
Gdiplus::Bitmap *LoadBitmapFromBuffer(const char * buffer, long bufferLength);
Gdiplus::Bitmap *LoadBitmapFromHGlobal(HGLOBAL hGlobal);

int SaveImageToFile(const WCHAR * filename,Gdiplus::Image *pImage, const WCHAR * type=L"image/png");
HGLOBAL SaveBitmapToHGlobal(Gdiplus::Bitmap *pBitmap, const WCHAR* type=L"image/jpeg");

Gdiplus::Bitmap * GetThumbnial(Gdiplus::Image* pSource, int width, int height);
void BuildCombox(CComboBox & cb, CStringList & strList);
Gdiplus::Color FromRGB(COLORREF rgbColor);
Gdiplus::Bitmap * BitmapClone(Gdiplus::Image * pImage);
Gdiplus::Bitmap * BitmapCreate(int width, int height, Gdiplus::PixelFormat pixelFormat);
Gdiplus::Bitmap * BitmapToGray(Gdiplus::Bitmap * pBitmap);
Gdiplus::Bitmap * BitmapToBlackWhite(Gdiplus::Bitmap *pBitmap, int threshold= 128);
Gdiplus::Bitmap * BitmapToBlackWhiteAverageGray(Gdiplus::Bitmap *pBitmap);
Gdiplus::Bitmap * BitmapSetColor(Gdiplus::Bitmap * pBitmap, Gdiplus::Color);
void RGB2HSL(Gdiplus::Color AColor, double &H,double &S,double &L);
Gdiplus::Color HSL2RGB(double H,double S,double L);

#endif //__IMAGE_UTILITY_H__