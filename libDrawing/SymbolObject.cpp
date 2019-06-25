#include "StdAfx.h"
#include <DrawObject\SymbolObject.h>
#include <DrawObject\SymbolManager.h>
#include <gdiplus\ImageUtility.h>
using namespace Gdiplus;

IMPLEMENT_SERIAL(CSymbolObject, CRectObject, 0)
int CSymbolObject::_minSize=DEFAULT_MIN_SYMBOL_SIZE;
int CSymbolObject::_maxSize=DEFAULT_MAX_SYMBOL_SIZE;

CSymbolObject::CSymbolObject(void)
:_type(INTERNAL_SYMBOL)
,_hCursor(NULL)
{
	_hCursor=NULL;
}

CSymbolObject::CSymbolObject(const CString &name, const CString & category,const Size &size, int type)
:CRectObject(name, Point(0,0), size)
,_category(category)
,_hCursor(NULL)
,_type(type)
{
}

CSymbolObject::~CSymbolObject(void)
{
	if(_hCursor)
    {
        DestroyCursor(_hCursor);
    }
}

void CSymbolObject::SetMaxSize( int maxSize )
{
	_maxSize=maxSize;
}

int CSymbolObject::GetMaxSize()
{
	return _maxSize;
}

void CSymbolObject::SetMinSize( int minSize )
{
	_minSize=minSize;
}

int CSymbolObject::GetMinSize()
{
	return _minSize;
}

HICON CSymbolObject::GetSymbolIcon(int w, int h, Color backgound)
{
    Bitmap * bmp=(Bitmap*)GetPicture(w, h, backgound);
    HICON hIcon;
    bmp->GetHICON(&hIcon);
    delete bmp;
    return hIcon;

}

HBITMAP CSymbolObject::GetSymbolImage(int w, int h, Color backgound)
{
	Bitmap *bmp=(Bitmap*)GetPicture(w,h,backgound);
    HBITMAP hBmp;
    bmp->GetHBITMAP(backgound, &hBmp);
	delete bmp;
	return hBmp;
}

void CSymbolObject::OnSizeChanging( Gdiplus::Size & newSize )
{
	__super::OnSizeChanging(newSize);
	return;
	if(newSize.Width> GetMaxSize())
	{
		newSize.Width=GetMaxSize();
	}
	if(newSize.Width <GetMinSize())
	{
		newSize.Width=GetMinSize();
	}
	newSize.Height=newSize.Width;
}

void CSymbolObject::SetCategory( const CString & category )
{
	if(_category!=category)
	{
		CString c=category;
		OnCategoryChanging(c);
		_category=c;
		OnCategoryChanged();
	}
}

void CSymbolObject::OnCategoryChanging( CString & newCategory )
{
	Notify(DNM_SYMBOL_CATEGORY_CHANGING, 0, &newCategory);
}

void CSymbolObject::OnCategoryChanged()
{
	Notify(DNM_SYMBOL_CATEGORY_CHANGED);
}

CDrawObject* CSymbolObject::CopyFrom( CDrawObject * source )
{
	__super::CopyFrom(source);
	CSymbolObject* pSymbolObject=DYNAMIC_DOWNCAST(CSymbolObject, source);
	if(pSymbolObject)
	{
		_category=pSymbolObject->_category;
	}
	else
	{
		_category=_T("");
	}
	return this;
}

void CSymbolObject::Serialize( CArchive & ar )
{
	__super::Serialize(ar);
	if(ar.IsLoading())
	{
		ar >> _category;
        ar >> _type;
	}
	else
	{
		ar << _category;
        ar << _type;
	}
	SetShowTracker(true);
}

CString CSymbolObject::GetCategory()
{
	return _category;
}

HCURSOR CSymbolObject::GetSymbolCursor()
{
	if(_hCursor)
		return _hCursor;
	Bitmap maskBmp(32,32, PixelFormat1bppIndexed);
	Color oldColor=GetFillColor();
	Size oldSize=GetSize();
	SetFillColor(Color::Black);
	SetSize(Size(32,32));
	Graphics graph1(&maskBmp);
	SolidBrush brush(Color::White);
	graph1.FillRectangle(&brush, 0,0,32,32);
	Draw(graph1);
	SetFillColor(oldColor);
	SetSize(oldSize);
	BitmapData data;
	Rect rect(0,0,32,32);
	maskBmp.LockBits(&rect,ImageLockModeRead|ImageLockModeWrite, PixelFormat1bppIndexed, &data);

	BYTE maskArray[4*32];
	char *p=(char*)data.Scan0;
	BYTE *pDest=maskArray;
	for(int i=0; i< 32; i++)
	{
		memcpy(pDest, p, 4);
		pDest+=4;
		p+=data.Stride;
	}
	BYTE xorArray[4*32];
	memset(xorArray, 0, 4*32);

	_hCursor=CreateCursor(AfxGetApp()->m_hInstance, 16,16, 32,32, maskArray, xorArray);
	return _hCursor;
}

int CSymbolObject::GetHandleCount()
{
	return 4;
}

HCURSOR CSymbolObject::GetHandleCursor(int nHandle)
{
	ASSERT(nHandle>=1 && nHandle<=5);
	switch(nHandle)
	{
	case 1:
		nHandle=1;
		break;
	case 2:
		nHandle=3;
		break;
	case 3:
		nHandle=5;
		break;
	case 4:
		nHandle=7;
		break;
	case 5:
		nHandle=9;
		break;
	}
	return CRectObject::GetHandleCursor(nHandle);
}

Gdiplus::Point CSymbolObject::GetHandle( int nHandle )
{
	ASSERT(nHandle>=1 && nHandle<=4);
	switch(nHandle)
	{
	case 1:
		nHandle=1;
		break;
	case 2:
		nHandle=3;
		break;
	case 3:
		nHandle=5;
		break;
	case 4:
		nHandle=7;
		break;
	}
	return CRectObject::GetHandle(nHandle);
}

void CSymbolObject::MoveHandleTo(int &nHandle, Gdiplus::Point point)
{
	switch(nHandle)
	{
	case 1:
		nHandle=1;
		break;
	case 2:
		nHandle=3;
		break;
	case 3:
		nHandle=5;
		break;
	case 4:
		nHandle=7;
		break;
	case 5:
		nHandle=9;
		break;
	}
	__super::MoveHandleTo(nHandle, point);
	switch(nHandle)
	{
	case 1:
		nHandle=1;
		break;
	case 3:
		nHandle=2;
		break;
	case 5:
		nHandle=3;
		break;
	case 7:
		nHandle=4;
		break;
	case 9:
		nHandle=5;
		break;
	}
}

void CSymbolObject::OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam)
{
	std::vector<CDrawObject*> &list=GetBindTargets();
	if(_findIndex(list, pSource) >=0)
	{
		switch(messageID)
		{
		case DNM_POSITION_CHANGING:
			{
				Point oldPos;
				Point newPos=*(Point*)lpParam;
				//��Ŀ��ԭλ��ȫ������
				pSource->Local2World(&oldPos);
				//��Ŀ����λ��ȫ������
				if(pSource->GetParent())
				{
					pSource->GetParent()->Local2World(&newPos);
				}
				//�õ�����ƫ��
				Point offset=newPos-oldPos;
				//��Ŀ��ƫ��
				Offset(offset);
			}
			break;
		case DNM_SIZE_CHANGING:
			{
				//0  ȡλ��ȫ������
				Point position;
				Local2World(&position);

				//1. ����x��y�������ű�
				Size oldSize;
				float rx;
				float ry;

				//�����size
				Size *newSize=(Size*)lpParam;

				//1.2 ȡ��ǰĿ������С
				oldSize=pSource->GetSize();
				//1.3 ����x��y�������ű�
				//BUGS  �账��߽�ֵ����0��
				rx=(float)newSize->Width/oldSize.Width;
				ry=(float)newSize->Height/oldSize.Height;

				//2. �������ź�λ�õ�ȫ������

				//2.1 �任�˵�ȫ�����굽Ŀ����󱾵�����
				pSource->World2Local(&position);

				//2.2 ���ű任
				Point newPosition((int)(position.X*rx),(int)(position.Y*ry));
				//2.3 �任��ȫ������
				//����ƫ��
				Point offset=newPosition-position;
				//3 �ƶ�����Ŀ��任�����λ��
				
				Offset(offset);
				//4 ��Ŀ��ͬ������
				//Scale(rx,ry);
			}
			break;
		}
	}
}

void CSymbolObject::SetIndex( int index )
{
    _index=index;
}

int CSymbolObject::GetIndex()
{
    return _index;
}
