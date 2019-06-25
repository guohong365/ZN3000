// Layout.cpp : 实现文件
//
#pragma once

#include "stdafx.h"
#include "ImageObject.h"
#include "KeyAuthority.h"

#include <GdiPlus.h>
#include "DrawObject.h"
#include "RectObject.h"
#include "DrawObjectList.h"
#include "ImageUtility.h"
#include "History.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CHANNEL_RED   1
#define CHANNEL_GREEN 2
#define CHANNEL_BLUE  4

#define FLIP_TYPE_HORIZNAL 1
#define FLIP_TYPE_VERTICAL 2

#define DEFAULT_THUMBNAIL_WIDTH 512
#define DEFAULT_THUMBNAIL_HEIGHT 512

using namespace Gdiplus;
IMPLEMENT_SERIAL(CImageObject, CRectObject, 0)

static Gdiplus::ColorMatrix GrayMatrix = {
	0.299f, 0.299f, 0.299f, 0, 0,
	0.587f, 0.587f, 0.587f, 0, 0,
	0.114f, 0.114f, 0.114f, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1
};
static Gdiplus::ColorMatrix InvertMatrix = {
	-1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

static Gdiplus::ColorMatrix IdentifyColorMatrix = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

void CImageObject::Initialize()
{
	_pImage = NULL;
	_pThumbnailImage = NULL;
	m_colorMatrix = IdentifyColorMatrix;
	m_Channel = CHANNEL_RED | CHANNEL_GREEN | CHANNEL_BLUE;
	m_bGray = false;
	m_bInvert = false;
}

CImageObject::CImageObject()
{
	Initialize();
}

CImageObject::CImageObject(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size):CRectObject(name, pt, size)
{
	Initialize();
}

CImageObject::CImageObject(const CString & name, const Gdiplus::Rect & rect):CRectObject(name, rect)
{
	Initialize();
}

CImageObject::~CImageObject()
{
	delete _pImage;
	delete _pThumbnailImage;
}



void CImageObject::SetImage(Gdiplus::Image * image)
{
	if (_pImage != image)
	{
		OnImageChanging(image);
		delete _pImage;
		_pImage = image;
		OnImageChanged();
	}
}
const Gdiplus::Image * CImageObject::GetImage() const 
{
	return _pImage;
}

Gdiplus::Image * CImageObject::GetImage()
{
	if(this==NULL)
		return NULL;
	return _pImage;
}

void CImageObject::OnImageChanging(Gdiplus::Image * image)
{
	Notify(DNM_IMAGE_CHANGING, 0, image);
}
void CImageObject::OnImageChanged()
{
	delete _pThumbnailImage;
	_pThumbnailImage=NULL;
	Notify(DNM_IMAGE_CHANGED);
}

void CImageObject::SetImageFileName(CString imageFileName)
{
	if (imageFileName != m_ImageFileName)
	{
		OnImageFileNameChanging(imageFileName);
		m_ImageFileName = imageFileName;
		OnImageFileNameChanged();
	}
}

Gdiplus::Image * CImageObject::GetThumbnailImage()
{
	return _pThumbnailImage;
}


void CImageObject::SetColorMatrix(Gdiplus::ColorMatrix colorMatrix)
{
	if (memcpy(&m_colorMatrix, &colorMatrix, sizeof(m_colorMatrix)) != 0)
	{
		OnColorMatrixChanging(colorMatrix);
		m_colorMatrix = colorMatrix;
		OnColorMatrixChanged();
	}
}

Gdiplus::ColorMatrix CImageObject::GetColorMatrix() const
{
	return m_colorMatrix;
}

void CImageObject::OnColorMatrixChanging(Gdiplus::ColorMatrix & colorMatrix)
{
	Notify(DNM_IMAGE_COLOR_MATRIX_CHANGING, 0, &colorMatrix);
}
void CImageObject::OnColorMatrixChanged()
{
	Notify(DNM_IMAGE_COLOR_MATRIX_CHANGED);
}

void CImageObject::SetShowGray(bool isShowGray)
{
	if (m_bGray != isShowGray)
	{
		OnShowGrayChanging(isShowGray);
		m_bGray = isShowGray;
		OnShowGrayChanged();
	}
}
bool CImageObject::GetShowGray() const 
{
	return m_bGray;
}

void CImageObject::OnShowGrayChanging(bool & isShowGray)
{
	Notify(DNM_IMAGE_SHOW_GRAY_CHANGING, 0, &isShowGray);
}
void CImageObject::OnShowGrayChanged()
{
	Notify(DNM_IMAGE_SHOW_GRAY_CHANGED);
}

void CImageObject::SetShowInvert(bool isShowInvert)
{
	if (m_bInvert != isShowInvert)
	{
		OnShowInvertChanging(isShowInvert);
		m_bInvert = isShowInvert;
		OnShowInvertChanged();
	}
}
bool CImageObject::GetShowInvert() const 
{
	return m_bInvert;
}

void CImageObject::OnShowInvertChanging(bool & isShowInvert)
{
	Notify(DNM_IMAGE_SHOW_INVERT_CHANGING, 0, &isShowInvert);
}
void CImageObject::OnShowInvertChanged()
{
	Notify(DNM_IMAGE_SHOW_INVERT_CHANGED);
}


void CImageObject::SetChannel(unsigned char channel)
{
	if (channel != m_Channel)
	{
		OnChannelChanging(channel);
		m_Channel = channel;
		OnChannelChanged();
	}
}
unsigned char CImageObject::GetChannel() const 
{
	return m_Channel;
}
void CImageObject::OnChannelChanging(unsigned char &channel)
{
	Notify(DNM_IMAGE_CHANNEL_CHANGING, 0, &channel);

}
void CImageObject::OnChannelChanged()
{
	Notify(DNM_IMAGE_CHANNEL_CHANGED);
}

const CString CImageObject::GetImageFileName() const 
{
	return m_ImageFileName;
}

void CImageObject::OnImageFileNameChanging(CString & imageFileName)
{
	Notify(DNM_IMAGE_FILENAME_CHANGING, 0, &imageFileName);
	imageFileName.Replace(_T('\\'), _T('/'));
	int index = imageFileName.ReverseFind(_T('/'));
	if (index >= 0)
	{
		CString name = imageFileName.Right(imageFileName.GetLength() - index - 1);
		imageFileName = name;
	}
}


void CImageObject::OnImageFileNameChanged()
{
	Notify(DNM_IMAGE_FILENAME_CHANGED);
}

CDrawObject *CImageObject::CopyFrom(CDrawObject * source)
{
	CImageObject *pImageObject = DYNAMIC_DOWNCAST(CImageObject, source);
	ASSERT(pImageObject);
	CRectObject::CopyFrom(source);
	delete _pImage;
	_pImage = NULL;
	if (pImageObject->_pImage)
	{
		_pImage =BitmapClone(pImageObject->_pImage);
	}
	return this;
}


int CImageObject::HitTest(const Gdiplus::Point & pt)
{
	return CRectObject::HitTest(pt);
}

//装载图像文件
bool CImageObject::LoadImage(const TCHAR * pathName)
{
	//Gdiplus::Bitmap *pImage= Gdiplus::Bitmap::FromFile(pathName);
	Bitmap *pImage=LoadBitmapFromFile(pathName);
	SetImage(pImage);

	if (_pImage != NULL && _pImage->GetLastStatus() == Gdiplus::Ok)
	{
		SetImageFileName(pathName);
		return true;
	}
	m_ImageFileName = "";
	delete _pImage;
	_pImage = NULL;
	return false;
}

bool CImageObject::LoadImage(const TCHAR * pathName, IStorage * pRootStorage, bool encrypted)
{
	HRESULT ret;
	IStorage *pStorage;
	IStream *pStream;
	if (pRootStorage == NULL)
	{
		return LoadImage(pathName);
	}
	else
	{
		bool result = false;
		ret = pRootStorage->OpenStorage(L"images", NULL, STGM_READ | STGM_SHARE_EXCLUSIVE, NULL, 0, &pStorage);
        if (SUCCEEDED(ret))
        {
            ret = pStorage->OpenStream(pathName, NULL, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, &pStream);
            if (SUCCEEDED(ret))
            {
                if(encrypted)
                {
                    LARGE_INTEGER pos={0};
                    ULARGE_INTEGER size;
                    
                    if(pStream->Seek(pos, STREAM_SEEK_END, &size)!=S_OK ||
                        pStream->Seek(pos,STREAM_SEEK_SET, NULL)!=S_OK)
                    {
                        pStream->Release();
                        pStorage->Release();
                        return false;
                    }
                    LPSTREAM lpStream;
                    if(CreateStreamOnHGlobal(NULL, TRUE, &lpStream)!=S_OK)
                    {
                        pStream->Release();
                        pStorage->Release();
                        return false;
                    }
                    if(pStream->CopyTo(lpStream,size, NULL, NULL)!=S_OK)
                    {
                        pStream->Release();
                        pStorage->Release();
                        lpStream->Release();
                        return false;
                    }

                    HGLOBAL hGlobal;
                    if(lpStream->Seek(pos,STREAM_SEEK_SET, NULL)==S_OK)
                    {
                        if(GetHGlobalFromStream(lpStream, &hGlobal)==S_OK)
                        {
                            void *addr=GlobalLock(hGlobal);
                            if(addr)
                            {
                                if(HardDecrypt(addr, 32)==KEY_NO_ERROR)
                                {
                                     result=LoadImage(lpStream);
                                }
                                GlobalUnlock(hGlobal);
                            }
                        }
                    }
                    lpStream->Release();
                }
                else
                {
                    result = LoadImage(pStream);
                }
            }
        }
        if (pStream)
        {
            pStream->Release();
        }
        if (pStorage)
		{
			pStorage->Release();
		}
		return result;
	}
}
bool CImageObject::LoadImage(IStream * pStream)
{
	Bitmap image(pStream);
	if(image.GetLastStatus()==Ok)
	{
		Image* pNewImage=image.Clone(0,0, image.GetWidth(),image.GetHeight(), image.GetPixelFormat());
		Graphics graph(pNewImage);
		graph.DrawImage(&image, 0,0,image.GetWidth(), image.GetHeight());
		SetImage(pNewImage);
		if (_pImage->GetLastStatus() == Ok)
		{
			return true;
		}
	}
	return false;
}


bool CImageObject::SaveImage(const TCHAR * pathName, IStorage * pRootStorage, bool encrypt)
{
	ASSERT(pathName);
	ASSERT(pRootStorage);
	bool ret = false;
	IStorage *pStorage;
	IStream *pStream;
	HRESULT hResult;
	hResult = pRootStorage->OpenStorage(L"images", NULL, STGM_SHARE_EXCLUSIVE | STGM_READWRITE, NULL, 0, &pStorage);
	if (!SUCCEEDED(hResult))
	{
		hResult = pRootStorage->CreateStorage(L"images", STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &pStorage);
	}
	if (SUCCEEDED(hResult))
	{
		hResult = pStorage->CreateStream(pathName, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &pStream);
		if (SUCCEEDED(hResult))
		{
            if(encrypt)
            {
                LPSTREAM lpStream = NULL;
                if(CreateStreamOnHGlobal(NULL, TRUE, &lpStream)==S_OK)
                {
                    if(SaveImage(lpStream))
                    {
                        HGLOBAL hGlobal;
                        if(SUCCEEDED(GetHGlobalFromStream(lpStream, &hGlobal)))
                        {
                            SIZE_T size=GlobalSize(hGlobal);
                            void *addr=GlobalLock(hGlobal);
                            if(addr && HardEncrypt(addr, 32)==KEY_NO_ERROR)
                            {
                               if(pStream->Write(addr, size, &size)==S_OK)
                               {
                                   pStream->Release();
                                   ret = true;
                               }
                            }
                            GlobalUnlock(hGlobal);
                        }
                    }
                    lpStream->Release();
                }
            }
            else
            {
                ret = SaveImage(pStream);
    			pStream->Release();
            }
		}
		pStorage->Commit(STGC_DEFAULT);
		pStorage->Release();
	}
	return ret;
}

bool CImageObject::SaveImage(IStream * pStream)
{
	ASSERT(pStream);
	CLSID encoderClsid;
	if (GetEncoderClsid(L"image/png", &encoderClsid) != -1)
	{
		_pImage->Save(pStream, &encoderClsid, NULL);
		return (_pImage->GetLastStatus() == Gdiplus::Ok);
	}
	return false;
}

bool CImageObject::SaveImage(const TCHAR * pathName)
{
	ASSERT(pathName);
	CLSID encoderClsid;
	if (GetEncoderClsid(L"image/png", &encoderClsid) != -1)
	{
		_pImage->Save(pathName, &encoderClsid, NULL);
		return (_pImage->GetLastStatus() == Gdiplus::Ok);
	}
	return false;
}

void CImageObject::Serialize(CArchive & ar)
{
	CRectObject::Serialize(ar);
	CString streamName = _T("IMAGE-") + GetInternalName() + _T(".png");
	if (ar.IsStoring())
	{
		if (_pImage)
		{
			if (m_ImageFileName.GetLength() == 0)
			{
				m_ImageFileName = CDrawObject::GenerateUniqueName() + _T(".png");
			}
			ar << m_ImageFileName;
			if (GetStorage())
			{
				SaveImage(streamName, GetStorage());
			}
			else
			{
				SaveImage(m_ImageFileName);
			}
		}
		else
		{
			m_ImageFileName = "";
			ar << m_ImageFileName;
		}
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				ar << m_colorMatrix.m[i][j];
			}
		}
		ar << m_bGray;
		ar << m_Channel;
		ar << m_bInvert;

	}
	else
	{
		ar >> m_ImageFileName;
		if (m_ImageFileName.GetLength() > 0)
		{
			if (GetStorage())
			{
				LoadImage(streamName, GetStorage());
			}
			else
			{
				LoadImage(m_ImageFileName);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				ar >> m_colorMatrix.m[i][j];
			}
		}
		ar >> m_bGray;
		ar >> m_Channel;
		ar >> m_bInvert;
	}
}

static Gdiplus::ColorMatrix MultMatrix(Gdiplus::ColorMatrix a, Gdiplus::ColorMatrix b)
{
	Gdiplus::ColorMatrix matrix;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			matrix.m[i][j] = 0;
			for (int k = 0; k < 5; k++)
			{
				matrix.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return matrix;
}

//事件(覆盖)
void CImageObject::OnDraw(Gdiplus::Graphics & graph)
{
	if (_pImage)
	{
		Gdiplus::ColorMatrix grayMatrix;
		Gdiplus::ColorMatrix matrix;
		if (m_bGray)
		{
			grayMatrix = GrayMatrix;
		}
		else
		{
			grayMatrix = IdentifyColorMatrix;
		}
		if ((m_Channel & CHANNEL_RED) == 0)
		{
			grayMatrix.m[0][0] = grayMatrix.m[1][0] = grayMatrix.m[2][0] = 0;
		}
		if ((m_Channel & CHANNEL_GREEN) == 0)
		{
			grayMatrix.m[0][1] = grayMatrix.m[1][1] = grayMatrix.m[2][1] = 0;
		}
		if ((m_Channel & CHANNEL_BLUE) == 0)
		{
			grayMatrix.m[0][2] = grayMatrix.m[1][2] = grayMatrix.m[2][2] = 0;
		}
		m_colorMatrix.m[3][3] = GetTransparent();
		matrix = MultMatrix(m_colorMatrix, grayMatrix);
		if (m_bInvert)
		{
			matrix = MultMatrix(matrix, InvertMatrix);
		}

		Gdiplus::ImageAttributes ImgAttr;
		ImgAttr.SetColorMatrix(&matrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
		if(!CDrawObject::IsPrinting())
		{
			if (_pThumbnailImage == NULL)
			{
				_pThumbnailImage =GetThumbnial(_pImage, DEFAULT_THUMBNAIL_WIDTH, DEFAULT_THUMBNAIL_HEIGHT);
				//_pImage->GetThumbnailImage(GetSize().Width, GetSize().Height, NULL, NULL);
			}
		
			graph.DrawImage(_pThumbnailImage,
				Gdiplus::Rect(0, 0, GetSize().Width, GetSize().Height),
				0, 0, _pThumbnailImage->GetWidth(), _pThumbnailImage->GetHeight(), UnitPixel, &ImgAttr);
		}
		else
		{
			graph.DrawImage(_pImage, Gdiplus::Rect(0, 0, GetSize().Width, GetSize().Height),
				0, 0, _pImage->GetWidth(), _pImage->GetHeight(), UnitPixel, &ImgAttr);
		}

	}
}

bool CImageObject::BuildProperties(CXTPPropertyGridItem * pCategoryObjects)
{
	if (!CRectObject::BuildProperties(pCategoryObjects))
		return false;

	CXTPPropertyGridItem *pItem;
	pItem = pCategoryObjects->AddChildItem(new CXTPPropertyGridItem(ID_OBJECT_IMAGE_FILE_NAME, m_ImageFileName));
	pItem->SetReadOnly(TRUE);

	pCategoryObjects->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_IMAGE_GRAY_SHOW, m_bGray))->SetItemData((DWORD_PTR) this);
	pCategoryObjects->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_IMAGE_INVERT_SHOW, m_bInvert))->SetItemData((DWORD_PTR) this);
	pItem = pCategoryObjects->AddChildItem(new CXTPPropertyGridItem(_T("颜色通道")));
	pItem->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_IMAGE_CHANNEL_RED, m_Channel & CHANNEL_RED))->SetItemData((DWORD_PTR) this);
	pItem->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_IMAGE_CHANNEL_GREEN, m_Channel & CHANNEL_GREEN))->SetItemData((DWORD_PTR) this);
	pItem->AddChildItem(new CXTPPropertyGridItemBool(ID_OBJECT_IMAGE_CHANNEL_BLUE, m_Channel & CHANNEL_BLUE))->SetItemData((DWORD_PTR) this);
	return true;
}

void CImageObject::OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem)
{
	CRectObject::OnPropertyItemChangedNotify(pItem);

	CXTPPropertyGridItemBool *pItemBool = DYNAMIC_DOWNCAST(CXTPPropertyGridItemBool, pItem);
	switch (pItem->GetID())
	{
	case ID_OBJECT_IMAGE_FILE_NAME:
		SetImageFileName(pItem->GetValue());
		break;
	case ID_OBJECT_IMAGE_GRAY_SHOW:
		SetShowGray(pItemBool->GetBool() == TRUE);
		break;
	case ID_OBJECT_IMAGE_INVERT_SHOW:
		SetShowInvert(pItemBool->GetBool() == TRUE);
		break;
	case ID_OBJECT_IMAGE_CHANNEL:
		break;
	case ID_OBJECT_IMAGE_CHANNEL_RED:
		if (pItemBool->GetBool() == TRUE)
		{
			m_Channel |= CHANNEL_RED;
		}
		else
		{
			m_Channel &= ~CHANNEL_RED;
		}
		break;
	case ID_OBJECT_IMAGE_CHANNEL_GREEN:
		if (pItemBool->GetBool() == TRUE)
		{
			m_Channel |= CHANNEL_GREEN;
		}
		else
		{
			m_Channel &= ~CHANNEL_GREEN;
		}
		break;

	case ID_OBJECT_IMAGE_CHANNEL_BLUE:
		if (pItemBool->GetBool() == TRUE)
		{
			m_Channel |= CHANNEL_BLUE;
		}
		else
		{
			m_Channel &= ~CHANNEL_BLUE;
		}
		break;
	default:
		break;
	}
}

void CImageObject::Flip90Right()
{
	if(_pImage)
	{
		_pImage->RotateFlip(Rotate90FlipNone);
		Size size=GetSize();
		SetSize(Size(size.Height, size.Width));
		OnImageChanged();
	}
}

void CImageObject::Flip90Left()
{
	if(_pImage)
	{
		_pImage->RotateFlip(Rotate270FlipNone);
		Size size=GetSize();
		SetSize(Size(size.Height, size.Width));
		OnImageChanged();
	}
}

void CImageObject::FlipX()
{
	if(_pImage)
	{
		_pImage->RotateFlip(RotateNoneFlipX);
		OnImageChanged();
	}
}

void CImageObject::FlipY()
{
	if(_pImage)
	{
		_pImage->RotateFlip(RotateNoneFlipY);
		OnImageChanged();
	}
}

Bitmap * CImageObject::DetachImage()
{
    Bitmap * pImage=static_cast<Bitmap*>(_pImage);
    _pImage=NULL;
    return pImage;
}

void CImageObject::notifyAttribtesChanged( CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal )
{
    switch(attributeIndex)
    {
    case ID_OBJECT_IMAGE:
        DetachImage();
        SetImage(newVal);
        break;
    case ID_OBJECT_IMAGE_FILE_NAME:
        SetImageFileName((CString)newVal);
        break;
    case ID_OBJECT_IMAGE_COLOR_MATRIX:
        SetColorMatrix((ColorMatrix)newVal);
        break;
    case ID_OBJECT_IMAGE_INVERT_SHOW:
        SetShowInvert((bool)newVal);
        break;
    case ID_OBJECT_IMAGE_GRAY_SHOW:
        SetShowGray((bool)newVal);
        break;
    case ID_OBJECT_IMAGE_CHANNEL:
        SetChannel(newVal);
        break;
    case ID_OBJECT_IMAGE_CHANNEL_RED:
    case ID_OBJECT_IMAGE_CHANNEL_GREEN:
    case ID_OBJECT_IMAGE_CHANNEL_BLUE:
        ASSERT(FALSE);
        break;
    default:
        __super::notifyAttribtesChanged(changedObject, attributeIndex, newVal, oldVal);
        break;
    }
}

void CImageObject::notifyRevert( CDrawObject *pChanged, int &action )
{
    __super::notifyRevert(pChanged, action);
    switch(action)        
    {
    case ID_OBJECT_IMAGE_FLIPX:
        FlipX();
        break;
    case ID_OBJECT_IMAGE_FLIPY:
        FlipY();
        break;
    case ID_OBJECT_IMAGE_FLIP_90_LEFT:
        Flip90Right();
        action=ID_OBJECT_IMAGE_FLIP_90_RIGHT;
        break;
    case ID_OBJECT_IMAGE_FLIP_90_RIGHT:
        Flip90Left();
        action=ID_OBJECT_IMAGE_FLIP_90_LEFT;
        break;
    default:
        break;
    }
}
