#include "StdAfx.h"
#include "ImageSymbol.h"
#include "ImageUtility.h"
using namespace Gdiplus;

IMPLEMENT_SERIAL(CImageSymbol, CSymbolObject, 0)

CImageSymbol::CImageSymbol(void)
:_pSymbolImage(NULL)
,_clrTransparent(0)
,_bUseTranparent(false)
{
}

CImageSymbol::CImageSymbol(Gdiplus::Image *pImage, const CString &name, 
						   const CString &category,const Gdiplus::Size &size,int type) 
    :CSymbolObject(name, category, size, type)
    ,_clrTransparent(0)
    ,_bUseTranparent(false)
{
    _pSymbolImage=NULL;
    SetImage(pImage);
}

CImageSymbol::~CImageSymbol(void)
{
    delete _pSymbolImage;
}

void CImageSymbol::SetImage( Gdiplus::Image * pImage )
{
    delete _pSymbolImage;
    _pSymbolImage=NULL;
    if(pImage)
    {
        _pSymbolImage=new Gdiplus::Bitmap(pImage->GetWidth(), pImage->GetHeight(), PixelFormat32bppARGB);
        Graphics graph(_pSymbolImage);
        graph.DrawImage(pImage, Rect(0,0, _pSymbolImage->GetWidth(), _pSymbolImage->GetHeight()), 0,0, _pSymbolImage->GetWidth(), _pSymbolImage->GetHeight(), UnitPixel);
    }
}

Gdiplus::Bitmap * CImageSymbol::GetSymbolImage()
{
    return _pSymbolImage;
}

void CImageSymbol::OnDraw( Gdiplus::Graphics & graph )
{
    if(_pSymbolImage)
    {
        graph.DrawImage(_pSymbolImage, Rect(Point(0,0), GetSize()), 0,0, _pSymbolImage->GetWidth(), _pSymbolImage->GetHeight(), UnitPixel);
    }
}

void CImageSymbol::SetTranparentColor( Gdiplus::Color color )
{
    _clrTransparent=color;
    return;
    if(_pSymbolImage)
    {
        BitmapData bmpData;
        Rect rect(0,0, _pSymbolImage->GetWidth(), _pSymbolImage->GetHeight());
        _pSymbolImage->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
        long *pixel=(long*)bmpData.Scan0;
        for(int i=0; i< rect.Width; i++)
        {   
            for(int j=0; j < rect.Height; j++)
            {
                if(pixel[j]==_clrTransparent.GetValue())
                {
                    pixel[j]=pixel[j] & 0x00FFFFFF;
                }
            }
            pixel = pixel + bmpData.Stride/4;
        }
        _pSymbolImage->UnlockBits(&bmpData);
    }
}

Gdiplus::Color CImageSymbol::GetTransparentColor()
{
    return _clrTransparent;
}

void CImageSymbol::SetUseTransparentColor( bool bUse )
{
    _bUseTranparent=bUse;
}

bool CImageSymbol::GetUseTrasparentColor()
{
    return _bUseTranparent;
}

void CImageSymbol::Serialize( CArchive & ar )
{
    __super::Serialize(ar);
    int hasImage;
    HGLOBAL handle=NULL;
    if(ar.IsLoading())
    {
        delete _pSymbolImage;
        _pSymbolImage=NULL;
         ar >> hasImage;
        if(hasImage)
        {
            SIZE_T size;
            ar >> size;
            handle=GlobalAlloc(GHND , size);
            if(handle==NULL)
            {
                AfxThrowMemoryException();
            }
            void *ptr=GlobalLock(handle);
            if(ptr==NULL)
            {
                AfxThrowMemoryException();
            }
            ar.Read(ptr, size);
            _pSymbolImage=LoadBitmapFromHGlobal(handle);
        }
        ar >> _bUseTranparent;
        int color;
        ar >> color;
        _clrTransparent.SetValue(color);
        if(_bUseTranparent)
        {
            SetTranparentColor(_clrTransparent);
        }
    }
    else
    {
        if(_pSymbolImage && _pSymbolImage->GetLastStatus()==Ok)
        {
            handle=SaveBitmapToHGlobal(_pSymbolImage, L"image/png");
            if(handle==NULL)
            {
                AfxThrowMemoryException();
            }
            void *ptr=GlobalLock(handle);
            SIZE_T size=GlobalSize(handle);
            if(ptr==NULL && size==0)
            {
                AfxThrowMemoryException();
            }
            ar << 1;
            ar << size;
            ar.Write(ptr, size);
            GlobalFree(handle);
        }
        else
        {
            ar << 0;
        }
        ar << _bUseTranparent;
        ar << _clrTransparent.GetValue();
    }
}

CDrawObject* CImageSymbol::CopyFrom( CDrawObject * source )
{
    delete _pSymbolImage;
    __super::CopyFrom(source);
    CImageSymbol *pSymbol=DYNAMIC_DOWNCAST(CImageSymbol, source);
    if(pSymbol)
    {
        if(pSymbol->_pSymbolImage)
        {
            _pSymbolImage=new Gdiplus::Bitmap(pSymbol->_pSymbolImage->GetWidth(), pSymbol->_pSymbolImage->GetHeight(), PixelFormat32bppARGB);
            Graphics graph(_pSymbolImage);
            graph.DrawImage(pSymbol->_pSymbolImage, Rect(0,0, _pSymbolImage->GetWidth(), _pSymbolImage->GetHeight()), 0,0, _pSymbolImage->GetWidth(), _pSymbolImage->GetHeight(), UnitPixel);
        }
        _bUseTranparent=pSymbol->_bUseTranparent;
        if(_bUseTranparent)
        {
            SetTranparentColor(pSymbol->_clrTransparent);
        }
    }
    else
    {
        _bUseTranparent=false;
        _clrTransparent.SetValue(0);
        _pSymbolImage=NULL;
    }
    return this;
}

void CImageSymbol::OnFillColorChanged()
{
    Color color=GetFillColor();
    Bitmap *pBitmap=GetSymbolImage();
    BitmapSetColor(pBitmap, color);
}


