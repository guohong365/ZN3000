#include "stdafx.h"
#include "AspectRatioImageObject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAspectRatioImageObject, CImageObject, 0)

CAspectRatioImageObject::CAspectRatioImageObject()
:CImageObject()
{
};
CAspectRatioImageObject::CAspectRatioImageObject(const CString & name, const Gdiplus::Rect & rect)
:CImageObject(name, rect)
{

}
CAspectRatioImageObject::CAspectRatioImageObject(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size)
:CImageObject(name, pt,  size)
{
}

void CAspectRatioImageObject::OnSizeChanging(Gdiplus::Size & newSize)
{
	if (!GetImage())
	{
		return;
	}
	Gdiplus::Size old(GetImage()->GetWidth(), GetImage()->GetHeight());
	double ratio = 1.0;
	if ((double) old.Width / old.Height > (double) newSize.Width / newSize.Height)
	{
		newSize.Height = newSize.Width * old.Height / old.Width;
		ratio = (double) newSize.Height / GetSize().Width;
	}
	else
	{
		newSize.Width = newSize.Height * old.Width / old.Height;
		ratio = (double) newSize.Width / GetSize().Width;
	}
	CImageObject::OnSizeChanging(newSize);
}

void CAspectRatioImageObject::OnSizeChanged()
{
	SetRotateCenter(Gdiplus::Size(GetSize().Width / 2, GetSize().Height / 2));
	CImageObject::OnSizeChanged();
}

int CAspectRatioImageObject::GetHandleCount()
{
	return 4;
}

Gdiplus::Point CAspectRatioImageObject::GetHandle(int nHandle)
{
	Gdiplus::Size size = GetSize();
	int x, y, xCenter, yCenter;

	xCenter = size.Width / 2;
	yCenter = size.Height / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
	case 1:
		x = 0;
		y = 0;
		break;

	case 2:
		x = size.Width;
		y = 0;
		break;

	case 3:
		x = size.Width;
		y = size.Height;
		break;

	case 4:
		x = 0;
		y = size.Height;
		break;

		break;
	}

	return Gdiplus::Point(x, y);
}

HCURSOR CAspectRatioImageObject::GetHandleCursor(int nHandle)
{
	LPCTSTR id;
	if (!GetSizable())
	{
		id = IDC_ARROW;
	}
	else
	{
		switch (nHandle)
		{
		default:
			id = IDC_ARROW;
			break;
		case 1:
		case 3:
			id = IDC_SIZENWSE;
			break;
		case 2:
		case 4:
			id = IDC_SIZENESW;
			break;
		}
	}

	return AfxGetApp()->LoadStandardCursor(id);
}
void CAspectRatioImageObject::MoveHandleTo(int &nHandle, Gdiplus::Point point)
{
	if (GetSizable())
	{
		CRect rect(GetPosition().X, GetPosition().Y, GetPosition().X + GetSize().Width, GetPosition().Y + GetSize().Height);
		Gdiplus::Size newSize;
		Gdiplus::Point newPos;
		switch (nHandle)
		{
		case 1:
			rect.left = point.X;
			rect.top = point.Y;
			break;
		case 3:
			rect.right = point.X;
			rect.bottom = point.Y;
			break;
		case 2:
			rect.right = point.X;
			rect.top = point.Y;
			break;
		case 4:
			rect.left = point.X;
			rect.bottom = point.Y;
			break;
			break;
		}
		rect.NormalizeRect();
		newSize.Width = rect.Width();
		newSize.Height = rect.Height();
		newPos.X = rect.left;
		newPos.Y = rect.top;
		SetPosition(newPos);
		SetSize(newSize);
	}
}

void CAspectRatioImageObject::Serialize( CArchive & ar )
{
	__super::Serialize(ar);
}

