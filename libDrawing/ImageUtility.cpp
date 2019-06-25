#include "stdafx.h"
#include <atlconv.h>
#include <GdiPlus.h>
#include <uc/image/ImageUtility.h>


static Gdiplus::ColorMatrix GrayMatrix = {
    0.299f, 0.299f, 0.299f, 0, 0,
    0.587f, 0.587f, 0.587f, 0, 0,
    0.114f, 0.114f, 0.114f, 0, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 0, 1
};

Gdiplus::Bitmap * BitmapCreate(int width, const int height, const Gdiplus::PixelFormat pixelFormat)
{
    return new Gdiplus::Bitmap(width, height, pixelFormat);
}

Gdiplus::Bitmap * BitmapClone(Gdiplus::Image * pImage)
{
	Gdiplus::Bitmap * pBitmap=new Gdiplus::Bitmap(pImage->GetWidth(), pImage->GetHeight(), pImage->GetPixelFormat());
    if(pBitmap && pBitmap->GetLastStatus()== Gdiplus::Ok)
    {
	    Gdiplus::Graphics graph(pBitmap);
        graph.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
        graph.Clear(Gdiplus::Color::Transparent);
        graph.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

        graph.DrawImage(pImage,
                        Gdiplus::Rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()),
            0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), Gdiplus::UnitPixel);
        return pBitmap;
    }
    return nullptr;
}
static int GetEncoderClsid(const WCHAR * format, CLSID * pClsid)
{
	UINT num = 0;               // number of image encoders
	UINT size = 0;              // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo * pImageCodecInfo = nullptr;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;              // Failure

	pImageCodecInfo = static_cast<Gdiplus::ImageCodecInfo *>(malloc(size));
	if (pImageCodecInfo == nullptr)
		return -1;              // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;           // Success
		}
	}

	free(pImageCodecInfo);
	return -1;                  // Failure
}

Gdiplus::Image * LoadThumbImage(const WCHAR *fileName, const int targetRectWidth, const int targetRectHeight)
{
	return LoadThumbImage(fileName, Gdiplus::Rect(0,0, targetRectWidth, targetRectHeight));
}
Gdiplus::Image * LoadThumbImage(const WCHAR *fileName, const Gdiplus::Rect& pTargetRect)
{
	Gdiplus::Image* pImage = Gdiplus::Image::FromFile(fileName);
		if(!pImage)
		{
			return nullptr;
		}
		SIZE size;
		size.cx=pImage->GetWidth();
		size.cy=pImage->GetHeight();
	const float rate=float(size.cx) /float(size.cy);
		int w,h;
		if(rate >=1)
		{
			w=int(pTargetRect.Width);
			h=int(pTargetRect.Width / rate);
		}
		else
		{
			w=static_cast<int>(pTargetRect.Height * rate);
			h=int(pTargetRect.Height);
		}
		Gdiplus::Image *p=pImage->GetThumbnailImage(w,h, nullptr, nullptr);
		delete pImage;
		return p;
}

Gdiplus::Bitmap * GetThumbnail(Gdiplus::Image* pSource, const int width, const int height)
{
	if(!pSource)
	{
		return nullptr;
	}
	SIZE size;
	size.cx=pSource->GetWidth();
	size.cy=pSource->GetHeight();
	const float rate=float(size.cx) /float(size.cy);
	int w,h;
	if(rate >=1)
	{
		w=width;
		h=int(width / rate);
	}
	else
	{
		w=int(height * rate);
		h=int(height);
	}
	Gdiplus::Bitmap *p=new Gdiplus::Bitmap(w, h, pSource->GetPixelFormat());
	Gdiplus::Graphics graph(p);
	graph.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graph.DrawImage(pSource, Gdiplus::Rect(0,0,w,h), 0,0, pSource->GetWidth(), pSource->GetHeight(), Gdiplus::UnitPixel);
	return p;
}


CImage *LoadThumbnailImage2(const WCHAR *filname, int width, int height)
{
	// 	USES_CONVERSION;
	Gdiplus::Bitmap bmp(filname);
	if(bmp.GetLastStatus()==Gdiplus::Ok)
	{
		const float rate=float(bmp.GetWidth()) /float(bmp.GetHeight());
		int w,h;
		if(rate >=1)
		{
			w=int(width);
			h=int(width / rate);
		}
		else
		{
			w=int(height * rate);
			h=int(height);
		}
		Gdiplus::Bitmap *pBmp=static_cast<Gdiplus::Bitmap*>(bmp.GetThumbnailImage(w,h, nullptr, nullptr));
		CImage* pImage = new CImage();
		pImage->Create(width, height, 24);
		CImage temp;
		HBITMAP hBmp;
		pBmp->GetHBITMAP(NULL, &hBmp);
		temp.Attach(hBmp);
		HDC hdc=pImage->GetDC();
		CDC *pDC=CDC::FromHandle(hdc);
		temp.BitBlt(hdc, 0,0, w, h, 0,0);
		pImage->ReleaseDC();
		delete pBmp;
		::DeleteObject(hBmp);
		return pImage;
	}
	return NULL;

}
CBitmap *LoadThumbnailImage3(const WCHAR *filname, const int width, const int height)
{
	Gdiplus::Bitmap bmp(filname);
	if(bmp.GetLastStatus()==Gdiplus::Ok)
	{
		const float rate=float(bmp.GetWidth()) /float(bmp.GetHeight());
		int w,h;
		if(rate >=1)
		{
			w=int(width);
			h=int(width / rate);
		}
		else
		{
			w=(int)(height * rate);
			h=(int)height;
		}
		Gdiplus::Bitmap *pBmp=static_cast<Gdiplus::Bitmap*>(bmp.GetThumbnailImage(w,h, NULL, NULL));
		Gdiplus::Bitmap outBmp(width, height, PixelFormat24bppRGB);
		Gdiplus::Graphics graph(&outBmp);
		Gdiplus::SolidBrush brush(Gdiplus::Color::White);
		Gdiplus::Pen pen(Gdiplus::Color::LightGray);
		graph.FillRectangle(&brush, 0,0, width-1, height-1);
		graph.DrawImage(pBmp, (width-w)/2,(height-h)/2, w-1, h-1);
		graph.DrawRectangle(&pen, 0,0, width-1, height-1);
		delete pBmp;
		HBITMAP hbmp;
		outBmp.GetHBITMAP(NULL, &hbmp);
		CBitmap *p=new CBitmap();
		p->Attach(hbmp);
		return p;
	}
	return NULL;
}

Gdiplus::Bitmap *LoadBitmapFromFile(const WCHAR * fileName)
{
	Gdiplus::Bitmap *pBitmap=NULL;
	FILE *fp;
	_wfopen_s(&fp,fileName, L"rb");
	long size;
	char *buffer;	
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		size=ftell(fp);
		if(size > 0)	
		{
			buffer=new char[size];
			fseek(fp, 0, SEEK_SET);
			if(fread(buffer, 1, size, fp)!=size)
			{
				fclose(fp);
				return NULL;
			}
		}
		else
		{
			fclose(fp);
			return NULL;
		}
		fclose(fp);
		pBitmap=LoadBitmapFromBuffer(buffer, size);
		return pBitmap;
	}
    return NULL;
}

Gdiplus::Bitmap *LoadBitmapFromBuffer(const char * buffer, long bufferLength)
{
	if(buffer && bufferLength)
	{
		HGLOBAL handle=GlobalAlloc(GHND|GMEM_MOVEABLE, bufferLength);
		if(handle)
		{
			IStream *pStream;
			if(CreateStreamOnHGlobal(handle, TRUE, &pStream)==S_OK)
			{
				ULONG written;
				if(pStream->Write(buffer, bufferLength, &written)==S_OK)
				{
					ULARGE_INTEGER pos;
					LARGE_INTEGER start={0,0};
					if(pStream->Seek(start, STREAM_SEEK_SET, &pos)==S_OK)
					{
						Gdiplus::Bitmap *pBitmap= Gdiplus::Bitmap::FromStream(pStream);
						pStream->Release();
						return pBitmap;
					}
				}
				pStream->Release();
			}
		}
	}
	return NULL;
}

Gdiplus::Bitmap * LoadBitmapFromHGlobal(HGLOBAL hGlobal)
{
    if(hGlobal!=NULL)
    {

        IStream *pStream;
        if(CreateStreamOnHGlobal(hGlobal, FALSE, &pStream)==S_OK)
        {
	        Gdiplus::Bitmap * pBitmap= Gdiplus::Bitmap::FromStream(pStream);
            pStream->Release();
            return pBitmap;
        }
    }
    return NULL;
}
HGLOBAL SaveBitmapToHGlobal(Gdiplus::Bitmap *pBitmap, const WCHAR * type)
{
	CLSID jpgClsid;
	if(GetEncoderClsid(type, &jpgClsid)==-1)
    {
        return NULL;
    }

	if(pBitmap && pBitmap->GetLastStatus()== Gdiplus::Ok)
	{
		IStream *pStream;
		if(CreateStreamOnHGlobal(NULL, FALSE, &pStream)==S_OK)
		{
			HGLOBAL handle=NULL;
			if(pBitmap->Save(pStream, &jpgClsid, NULL)== Gdiplus::Ok)
			{
				HRESULT lResult=GetHGlobalFromStream(pStream, &handle);
				pStream->Release();
				if(lResult==S_OK)
				{
					return handle;
				}
			}
			else
			{
				GetHGlobalFromStream(pStream, &handle);
				pStream->Release();
				GlobalFree(handle);
			}
		}
	}
	return NULL;
}
int SaveImageToFile(const WCHAR * filename, Gdiplus::Image * pImage, const WCHAR * type)
{
    if(filename==NULL || type==NULL || pImage==NULL || pImage->GetLastStatus()!= Gdiplus::Ok)
        return -1;
    CString imageType=type;
    
    CLSID jpgClsid;
    if(GetEncoderClsid(imageType, &jpgClsid)!=-1)
    {
        if(pImage->Save(filename, &jpgClsid, NULL)== Gdiplus::Ok)
        {
            return 0;
        }
    }
    return -1;
}
int SaveImageToStream(LPSTREAM lpStream, Gdiplus::Image * pImage, const WCHAR * type)
{
    if(lpStream==NULL || type==NULL || pImage==NULL || pImage->GetLastStatus()!= Gdiplus::Ok)
        return -1;
    CString imageType=type;

    CLSID jpgClsid;
    if(GetEncoderClsid(imageType, &jpgClsid)!=-1)
    {
        if(pImage->Save(lpStream, &jpgClsid, NULL)== Gdiplus::Ok)
        {
            return 0;
        }
    }
    return -1;
}
int SaveImageToHGlobal(HGLOBAL hGlobal, Gdiplus::Image * pImage, const WCHAR * type)
{
    if(hGlobal==NULL || type==NULL || pImage==NULL || pImage->GetLastStatus()!= Gdiplus::Ok)
        return -1;
    LPSTREAM lpStream = NULL;
    if(CreateStreamOnHGlobal(hGlobal, FALSE, &lpStream)==S_OK)
    {
        int ret=SaveImageToStream(lpStream, pImage, type);
        lpStream->Release();
        return ret;
    }

    return -1;
}

void BuildCombobox(CComboBox & cb, CStringList & strList)
{
	POSITION pos=strList.GetHeadPosition();
	while(pos)
	{
		cb.AddString(strList.GetNext(pos));
	}
}

Gdiplus::Color FromRGB( COLORREF rgbColor )
{
	Gdiplus::Color color;
	color.SetFromCOLORREF(rgbColor);
	return color;
}

void CalcAspectScale( const Gdiplus::Size &source, const Gdiplus::Size &dest, Gdiplus::Size &result, float &scale )
{
    if(source.Width==0 || source.Height==0 || dest.Width==0 || dest.Height==0)
    {
        result.Width=result.Height=0;
        scale=1.0f;
        return;
    }
    scale=(float)(dest.Width) / (float)(source.Width);
    result.Height=(int)(source.Height * scale);
    if(result.Height <= dest.Height)
    {
        result.Width=dest.Width;
    }
    else
    {
        scale=(float)(dest.Height) / (float)(source.Height);
        result.Height=dest.Height;
        result.Width=(int)(source.Width * scale);
    }
}

Gdiplus::Bitmap * BitmapTransparent( Gdiplus::Bitmap * result, Gdiplus::Color transparentColor, bool bClearForeground, Gdiplus::Color foreground, int error)
{
    if(result==NULL)
        return NULL;
	Gdiplus::Rect rc(0,0, result->GetWidth(), result->GetHeight());

	Gdiplus::BitmapData data;

    if(result->LockBits(&rc, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data)== Gdiplus::Ok)
    {
        BYTE *pLine=(BYTE*)data.Scan0;
        BYTE * pPixel;
        for(UINT y=0; y<data.Height; y++)
        {
            pPixel=pLine;
            for(UINT x=0; x < data.Width; x++)
            {
                //if(transparentColor.GetA()==0)
                //{
                //    if(error!=0)
                //    {
                //        if((pPixel[3]-transparentColor.GetA())* pLine[x+3]-transparentColor.GetA() < error * error)
                //        {
                //            pPixel[3] =0;
                //        }
                //    }
                //}
                //else
                if(transparentColor.GetA()!=0)
                {
                    if(error==0)
                    {
                        if(*((UINT32*)pPixel)==(UINT32)transparentColor.GetValue())
                        {
                            *((UINT32*)pPixel)=0;
                        }
                        else if(bClearForeground)
                        {
                            *((UINT32*)pPixel)=(UINT32)foreground.GetValue();
                        }
                    }
                    else
                    {
                        int sum=0;
                        int diff;
                        diff = pPixel[2] - transparentColor.GetRed();
                        sum += (1+diff * diff);
                        diff = pPixel[1] -transparentColor.GetG();
                        sum += (1+diff * diff);
                        diff = pPixel[0] - transparentColor.GetB();
                        sum += (1+diff * diff);
                        sum =sum * transparentColor.GetA()/256;
                        diff = pPixel[3] - transparentColor.GetA();
                        sum +=diff * diff;
                        if(sum <= error * error * 4)
                        {
                            pPixel[3]=0;
                        }
                        else if(bClearForeground)
                        {
                            *((UINT32*)(pPixel))=(UINT32)foreground.GetValue();
                        }
                    }
                }
                pPixel +=4;
            }
            pLine += data.Stride;
        }
        result->UnlockBits(&data);
    }
    return result;
}

Gdiplus::Bitmap * BitmapToGray(Gdiplus::Bitmap * pBitmap)
{
    if(pBitmap==NULL)
        return NULL;
	Gdiplus::Rect rc(0,0, pBitmap->GetWidth(), pBitmap->GetHeight());
	Gdiplus::BitmapData data;
    if(pBitmap->LockBits(&rc, Gdiplus::ImageLockModeRead| Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data)== Gdiplus::Ok)
    {
        BYTE *pData=(BYTE*)data.Scan0;
        for(UINT y=0; y< data.Height; y++)
        {
            int index=0;
            for(UINT x=0; x < data.Width; x++)
            {
                float bright=(pData[index + 2]*29.9f + pData[index +1]*58.7f + pData[index]*11.4f)/100;
                pData[index]=pData[index + 1]=pData[index + 2]=(BYTE)(bright);
                index += 4;
            }
            pData += data.Stride;
        }
        pBitmap->UnlockBits(&data);
    }
    return pBitmap;
}

Gdiplus::Bitmap * BitmapSetColor(Gdiplus::Bitmap * pBitmap, Gdiplus::Color color)
{
    if(pBitmap==NULL)
        return NULL;
	Gdiplus::Rect rc(0,0, pBitmap->GetWidth(), pBitmap->GetHeight());
	Gdiplus::BitmapData data;
	Gdiplus::Color pixel=color;
    if(pBitmap->LockBits(&rc, Gdiplus::ImageLockModeRead| Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data)== Gdiplus::Ok)
    {
        BYTE *pData=(BYTE*)data.Scan0;
        for(UINT y=0; y< data.Height; y++)
        {
            int index=0;
            for(UINT x=0; x < data.Width; x++)
            {
                //float bright=(pData[index]*29.9f + pData[index +1]*58.7f + pData[index +2]*11.4f)/25500;
             
                //RGB2HSL(color, H, S, L);
                //pixel = HSL2RGB(H, S, bright);
                pData[index]=pixel.GetB();
                pData[index + 1]=pixel.GetG();
                pData[index + 2]=pixel.GetR();
                index += 4;
            }
            pData += data.Stride;
        }
        pBitmap->UnlockBits(&data);
    }
    return pBitmap;
}


void RGB2HSL(Gdiplus::Color AColor, double &H,double &S,double &L)
{
    double R,G,B,Max,Min,del_R,del_G,del_B,del_Max;
    R = AColor.GetR() / 255.0;       //Where RGB values = 0 ÷ 255
    G = AColor.GetG() / 255.0;
    B = AColor.GetB() / 255.0;

    Min = min(R, min(G, B));    //Min. value of RGB
    Max = max(R, max(G, B));    //Max. value of RGB
    del_Max = Max - Min;        //Delta RGB value

    L = (Max + Min) / 2.0;

    if (del_Max == 0)           //This is a gray, no chroma...
    {
        //H = 2.0/3.0;          //Windows下S值为0时，H值始终为160（2/3*240）
        H = 0;                  //HSL results = 0 ÷ 1
        S = 0;
    }
    else                        //Chromatic data...
    {
        if (L < 0.5) S = del_Max / (Max + Min);
        else         S = del_Max / (2 - Max - Min);

        del_R = (((Max - R) / 6.0) + (del_Max / 2.0)) / del_Max;
        del_G = (((Max - G) / 6.0) + (del_Max / 2.0)) / del_Max;
        del_B = (((Max - B) / 6.0) + (del_Max / 2.0)) / del_Max;

        if      (R == Max) H = del_B - del_G;
        else if (G == Max) H = (1.0 / 3.0) + del_R - del_B;
        else if (B == Max) H = (2.0 / 3.0) + del_G - del_R;

        if (H < 0)  H += 1;
        if (H > 1)  H -= 1;
    }
}

//---------------------------------------------------------------------------
static double Hue2RGB(double v1, double v2, double vH)
{
    if (vH < 0) vH += 1;
    if (vH > 1) vH -= 1;
    if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
    if (2.0 * vH < 1) return v2;
    if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
    return (v1);
}//HSL转换为RGB：
Gdiplus::Color HSL2RGB(double H,double S,double L)
{
    double R,G,B;
    double var_1, var_2;
    if (S == 0)                       //HSL values = 0 ÷ 1
    {
        R = L * 255.0;                   //RGB results = 0 ÷ 255
        G = L * 255.0;
        B = L * 255.0;
    }
    else
    {
        if (L < 0.5) var_2 = L * (1 + S);
        else         var_2 = (L + S) - (S * L);

        var_1 = 2.0 * L - var_2;

        R = 255.0 * Hue2RGB(var_1, var_2, H + (1.0 / 3.0));
        G = 255.0 * Hue2RGB(var_1, var_2, H);
        B = 255.0 * Hue2RGB(var_1, var_2, H - (1.0 / 3.0));
    }
    return FromRGB(RGB(R,G,B));
}

Gdiplus::Bitmap * CreateIconizedImage( Gdiplus::Bitmap * input, int width, int height, const Gdiplus::Color &background/*=Gdiplus::Color::Transparent*/ )
{
	Gdiplus::Bitmap* temp=(Gdiplus::Bitmap*)GetThumbnail(input, width, height);
	if(temp && temp->GetLastStatus()==Gdiplus::Ok)
	{
		int w=temp->GetWidth();
		int h=temp->GetHeight();
		Gdiplus::Bitmap* output=new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
		Gdiplus::Graphics graph(output);
		Gdiplus::SolidBrush brush(background);
		graph.FillRectangle(&brush, 0,0,width,height);
		graph.DrawImage(temp, Gdiplus::Rect((width -temp->GetWidth())/2, (height- temp->GetHeight())/2, temp->GetWidth(), temp->GetHeight()), 0,0,temp->GetWidth(), temp->GetHeight(), Gdiplus::UnitPixel);
		delete temp;
		return output;
	}
	return NULL;
}

Gdiplus::Bitmap * BitmapToBlackWhite( Gdiplus::Bitmap *pBitmap, int threshold)
{
	Gdiplus::Bitmap* pBmp=BitmapToGray(pBitmap);
	Gdiplus::BitmapData data;
	Gdiplus::Rect rect(0,0,pBitmap->GetWidth(), pBitmap->GetHeight());
	pBmp->LockBits(&rect, Gdiplus::ImageLockModeRead|Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data);
	
	BYTE * pLine=(BYTE*)data.Scan0;

	for(int h= 0; h<rect.Height; h++)
	{
		BYTE * pPt=pLine;
		for(int w=0; w<rect.Width; w++)
		{
			if(pPt[0] < threshold)
			{
				*((ULONG32*)pPt)=0xFF000000;
			}
			else
			{
				*((ULONG32*)pPt)=0xFFFFFFFF;
			}
			pPt +=4;
		}
		pLine +=data.Stride;
	}
	pBmp->UnlockBits(&data);
	return pBmp;
}

Gdiplus::Bitmap * BitmapToBlackWhiteAverageGray(Gdiplus::Bitmap *pBitmap)
{
	 int threshold;
	Gdiplus::Bitmap* pBmp=BitmapToGray(pBitmap);
	Gdiplus::BitmapData data;
	Gdiplus::Rect rect(0,0,pBitmap->GetWidth(), pBitmap->GetHeight());
	pBmp->LockBits(&rect, Gdiplus::ImageLockModeRead|Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data);
	BYTE * pLine=(BYTE*)data.Scan0;
	ULONG sum = 0;

	for(int h= 0; h<rect.Height; h++)
	{
		BYTE * pPt=pLine;
		for(int w=0; w<rect.Width; w++)
		{
			sum +=pPt[0];
			pPt +=4;
		}
		pLine +=data.Stride;
	}
	threshold= sum /(rect.Width * rect.Height);

	pLine=(BYTE*)data.Scan0;

	for(int h= 0; h<rect.Height; h++)
	{
		BYTE * pPt=pLine;
		for(int w=0; w<rect.Width; w++)
		{
			if(pPt[0] < threshold)
			{
				*((ULONG32*)pPt)=0xFF000000;
			}
			else
			{
				*((ULONG32*)pPt)=0xFFFFFFFF;
			}
			pPt +=4;
		}
		pLine +=data.Stride;
	}
	pBmp->UnlockBits(&data);
	return pBmp;
}

