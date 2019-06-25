#include "stdafx.h"
#include "BackgroundObject.h"
#include "ImageObject.h"
#include <objidl.h>
#include "CustomItems.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CBackgroundObject, CRectObject, 0)
void CBackgroundObject::Initialize()
{
	m_RulerUnitPerPixel = 0.0;
	m_pBackgoundImage = NULL;
	m_ImageShowMode = ImageStretched;
	m_pVerticalRuler = NULL;
	m_pHorizalRuler = NULL;
	m_Options = 0;
}

CBackgroundObject::CBackgroundObject()
{
	Initialize();
}

CBackgroundObject::CBackgroundObject(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options):CRectObject(name, pt,
																																			 size)
{
	Initialize();
	m_Options = options;
	if ((options & WITH_HORIZ_RULER) != 0)
	{
		m_pHorizalRuler = new CRulerObject(_T("HORIZ"),
			RULER_OFFSET, RULER_OFFSET, size.Width - 2 * HORIZ_RULER_HEIGHT - RULER_OFFSET, HORIZ_RULER_HEIGHT);
		m_pHorizalRuler->SetLineColor(Gdiplus::Color::Yellow);
	}
	if ((options & WITH_VERT_RULER) != 0)
	{
		m_pVerticalRuler = new CRulerObject(_T("VERT"),
			RULER_OFFSET + HORIZ_RULER_HEIGHT, RULER_OFFSET, size.Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
		m_pVerticalRuler->SetGraduationPosition(BaselineAtBottom);
		m_pVerticalRuler->SetAngle(90);
		m_pVerticalRuler->SetLineColor(Gdiplus::Color::Yellow);
	}
}

CBackgroundObject::CBackgroundObject(const CString & name, const Gdiplus::Rect & rect, DWORD options):CRectObject(name, rect)
{
	Initialize();
	if ((options & WITH_HORIZ_RULER) != 0)
	{
		m_pHorizalRuler = new CRulerObject(_T("HORIZ"), RULER_OFFSET, RULER_OFFSET, rect.Width - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
		m_pHorizalRuler->SetLineColor(Gdiplus::Color::Yellow);
	}
	if ((options & WITH_VERT_RULER) != 0)
	{
		m_pVerticalRuler = new CRulerObject(_T("VERT"),
			RULER_OFFSET + HORIZ_RULER_HEIGHT, HORIZ_RULER_HEIGHT,
			rect.Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
		m_pVerticalRuler->SetGraduationPosition(BaselineAtBottom);
		m_pVerticalRuler->SetAngle(90);
		m_pVerticalRuler->SetLineColor(Gdiplus::Color::Yellow);
	}
}

CBackgroundObject::~CBackgroundObject()
{
	delete m_pBackgoundImage;
	delete m_pHorizalRuler;
	delete m_pVerticalRuler;
}



void CBackgroundObject::SetImageShowMode(int mode)
{
	if (m_ImageShowMode != mode)
	{
		OnImageShowModeChanging(mode);
		m_ImageShowMode = mode;
		OnImageShowModeChanged();
	}
}
int CBackgroundObject::GetImageShowMode() const 
{
	return m_ImageShowMode;
}
void CBackgroundObject::OnImageShowModeChanging(int &newMode)
{
}
void CBackgroundObject::OnImageShowModeChanged()
{

}

void CBackgroundObject::SetImage(Gdiplus::Image * image)
{
	if (m_pBackgoundImage != image)
	{
		OnImageChanging(image);
		delete m_pBackgoundImage;
		m_pBackgoundImage = image;
		OnImageChanged();
	}
}
const Gdiplus::Image * CBackgroundObject::GetImage() const 
{
	return m_pBackgoundImage;
}

Gdiplus::Image * CBackgroundObject::GetImage()
{
	return m_pBackgoundImage;
}

void CBackgroundObject::OnImageChanging(Gdiplus::Image * image)
{
	Notify(DNM_IMAGE_CHANGING, 0, &image);
}
void CBackgroundObject::OnImageChanged()
{
	Notify(DNM_PARENT_CHANGED);
}

void CBackgroundObject::SetRulerScale(double unitPerPixel)
{
	if (m_RulerUnitPerPixel != unitPerPixel)
	{
		OnRulerScaleChanging(unitPerPixel);
		m_RulerUnitPerPixel = unitPerPixel;
		if (m_pVerticalRuler)
		{
			m_pVerticalRuler->SetUnitPerPixel(unitPerPixel);
		}
		if (m_pHorizalRuler)
		{
			m_pHorizalRuler->SetUnitPerPixel(unitPerPixel);
		}
		OnRulerScaleChanged();
	}
}
double CBackgroundObject::GetRulerScale() const 
{
	return m_RulerUnitPerPixel;
}
void CBackgroundObject::OnRulerScaleChanging(double &newUnitPerPixel)
{
	Notify(DNM_RULER_SCALE_CHANGING, 0, &newUnitPerPixel);
}
void CBackgroundObject::OnRulerScaleChanged()
{
	Notify(DNM_RULER_SCALE_CHANGED);
}

void CBackgroundObject::SetShowRuler(int nRuler, bool isShow)
{
	if (nRuler == HORIZ_RULER && m_pHorizalRuler)
	{
		m_pHorizalRuler->SetVisible(isShow);
	}
	if (nRuler == VERT_RULER && m_pVerticalRuler)
	{
		m_pVerticalRuler->SetVisible(isShow);
	}
}
bool CBackgroundObject::GetShowRuler(int nRuler) const 
{
	if (nRuler == HORIZ_RULER && m_pHorizalRuler)
	{
		return m_pHorizalRuler->GetVisible();
	}
	if (nRuler == VERT_RULER)
	{
		return m_pVerticalRuler->GetVisible();
	}
	return false;
}
void CBackgroundObject::OnShowRulerChanging(int nRuler, bool & newIsShow)
{
	Notify(DNM_SHOW_RULER_CHANGING, nRuler, &newIsShow);
}
void CBackgroundObject::OnShowRulerChanged(int nRuler)
{
	Notify(DNM_SHOW_RULER_CHANGED, nRuler);
}

void CBackgroundObject::EnableRuler(int nRuler, bool isEnable)
{
	if (nRuler == VERT_RULER)
	{
		if(isEnable)
		{
			m_Options |= WITH_VERT_RULER;
		if (!m_pVerticalRuler)
		{
			m_pVerticalRuler = new CRulerObject(_T("VERT"),
				RULER_OFFSET,
				HORIZ_RULER_HEIGHT + RULER_OFFSET, GetSize().Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
			m_pVerticalRuler->SetGraduationPosition(BaselineAtBottom);
			m_pVerticalRuler->SetAngle(90);
			m_pVerticalRuler->SetLineColor(Gdiplus::Color::Yellow);
		}
		}
		else
		{
			m_Options &= (~WITH_VERT_RULER);
			delete m_pVerticalRuler;
			m_pVerticalRuler = NULL;
		}
	}
	if (nRuler == HORIZ_RULER)
	{
		if(isEnable)
		{
			m_Options |= WITH_HORIZ_RULER;
		if (!m_pHorizalRuler)
		{
			m_pHorizalRuler = new CRulerObject(_T("HORIZ"), RULER_OFFSET, RULER_OFFSET, GetSize().Width - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
			m_pHorizalRuler->SetLineColor(Gdiplus::Color::Yellow);
		}
		}
		else
		{
			m_Options &= (~WITH_HORIZ_RULER);
			delete m_pHorizalRuler;
			m_pHorizalRuler = NULL;
		}
	}
}

bool CBackgroundObject::IsRulerEnabled(int nRuler)
{
	if (nRuler == VERT_RULER)
	{
		return (m_pVerticalRuler != NULL) && (m_Options & WITH_VERT_RULER);
	}
	if (nRuler == HORIZ_RULER)
	{
		return (m_pHorizalRuler != NULL) && (m_Options & WITH_HORIZ_RULER);
	}
	return false;
}

CDrawObject *CBackgroundObject::CopyFrom(CDrawObject * source)
{
	CBackgroundObject *pBackground = DYNAMIC_DOWNCAST(CBackgroundObject, source);
	ASSERT(pBackground);
	CRectObject::CopyFrom(source);
	m_RulerUnitPerPixel = pBackground->m_RulerUnitPerPixel;
	m_ImageShowMode = pBackground->m_ImageShowMode;
	m_Options = pBackground->m_Options;
	delete m_pBackgoundImage;
	m_pBackgoundImage = NULL;
	if (pBackground->m_pBackgoundImage)
	{
		m_pBackgoundImage = pBackground->m_pBackgoundImage->Clone();
	}
	delete m_pHorizalRuler;
	delete m_pVerticalRuler;
	m_pHorizalRuler = NULL;
	m_pVerticalRuler = NULL;
	if (pBackground->m_pHorizalRuler)
	{
		m_pHorizalRuler = (CRulerObject *) pBackground->m_pHorizalRuler->Clone();
	}
	if (pBackground->m_pVerticalRuler)
	{
		m_pVerticalRuler = (CRulerObject *) pBackground->m_pVerticalRuler->Clone();
	}
	return this;
}


int CBackgroundObject::HitTest(const Gdiplus::Point & pt)
{
	return 0;
}

//事件(覆盖)
void CBackgroundObject::OnSizeChanged()
{
	Gdiplus::Size size;
	Gdiplus::Size newSize;
	if (m_pHorizalRuler)
	{
		size = m_pHorizalRuler->GetSize();
		newSize = Gdiplus::Size(GetSize().Width - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
		if (!size.Equals(newSize))
		{
			m_pHorizalRuler->SetSize(newSize);
		}
	}
	if (m_pVerticalRuler)
	{
		size = m_pVerticalRuler->GetSize();
		newSize = Gdiplus::Size(GetSize().Height - 2 * RULER_OFFSET, HORIZ_RULER_HEIGHT);
		if (!size.Equals(newSize))
		{
			m_pVerticalRuler->SetSize(newSize);
		}
	}
	__super::OnSizeChanged();
}


void CBackgroundObject::OnDraw(Gdiplus::Graphics & graph)
{
	Gdiplus::Pen pen(Gdiplus::Color::Black);
	CRectObject::OnDraw(graph);
	if (m_pBackgoundImage)
	{
		Gdiplus::Rect rect;
		switch (m_ImageShowMode)
		{
		case ImageStretched:
			graph.DrawImage(m_pBackgoundImage, 0, 0, GetSize().Width, GetSize().Height);
			break;
		case ImageCentered:
			rect.X = (GetSize().Width / 2) - m_pBackgoundImage->GetWidth() / 2;
			rect.Y = (GetSize().Height / 2) - m_pBackgoundImage->GetHeight() / 2;
			rect.Width = m_pBackgoundImage->GetWidth();
			rect.Height = m_pBackgoundImage->GetHeight();
			graph.DrawImage(m_pBackgoundImage, rect, 0, 0, m_pBackgoundImage->GetWidth(), m_pBackgoundImage->GetHeight(), Gdiplus::UnitPixel);
			break;
		case ImagedTiled:
			{
				int width = m_pBackgoundImage->GetWidth();
				int height = m_pBackgoundImage->GetHeight();
				int nx = GetSize().Width / m_pBackgoundImage->GetWidth() + ((GetSize().Width % m_pBackgoundImage->GetWidth())? 1 : 0);

				int ny = GetSize().Height / m_pBackgoundImage->GetHeight() + ((GetSize().Height % m_pBackgoundImage->GetHeight())? 1 : 0);

				rect.X = 0;
				for (int i = 0; i < nx; i++)
				{
					rect.Y = 0;
					rect.Width = (rect.X + width < GetSize().Width ? width : GetSize().Width - rect.X);
					for (int j = 0; j < ny; j++)
					{
						rect.Height = (rect.Y + height < GetSize().Height ? height : GetSize().Height - rect.Y);
						graph.DrawImage(m_pBackgoundImage, rect, 0, 0, rect.Width, rect.Height, Gdiplus::UnitPixel);
						rect.Y += m_pBackgoundImage->GetHeight();
					}
					rect.X += m_pBackgoundImage->GetWidth();
				}
			}
			break;
		default:
			break;
		}
	}
	DrawRuler(graph);
	graph.DrawLine(&pen, Gdiplus::Point(0, 0), Gdiplus::Point(GetSize().Width, GetSize().Height));
	graph.DrawLine(&pen, Gdiplus::Point(0, GetSize().Height), Gdiplus::Point(GetSize().Width, 0));
}

void CBackgroundObject::DrawRuler(Gdiplus::Graphics & graph)
{
	if (m_pVerticalRuler)
	{
		m_pVerticalRuler->Draw(graph);
	}
	if (m_pHorizalRuler)
	{
		m_pHorizalRuler->Draw(graph);
	}
}

void CBackgroundObject::Serialize(CArchive & ar)
{
	ULONG size = 0;
	char *buf = NULL;
	CLSID encoderClsid;
	CRectObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_RulerUnitPerPixel;
		ar << m_ImageShowMode;
		ar << m_Options;
		if (m_pBackgoundImage != NULL)
		{
			IStream *pStream;
			if (CreateStreamOnHGlobal(NULL, TRUE, &pStream) == S_OK)
			{
				m_pBackgoundImage->Save(pStream, &encoderClsid);
				pStream->Commit(STGC_DEFAULT);
				STATSTG stg;
				pStream->Stat(&stg, STATFLAG_DEFAULT);
				LARGE_INTEGER offset;
				offset.QuadPart = 0;
				pStream->Seek(offset, STREAM_SEEK_SET, NULL);
				buf = new char[stg.cbSize.LowPart];
				pStream->Read(buf, stg.cbSize.LowPart, &size);
				ar << stg.cbSize.LowPart;
				pStream->Release();
			}
		}
		ar << size;
		if (size > 0)
		{
			ar.Write(buf, size);
		}
		ar << m_pVerticalRuler;
		ar << m_pHorizalRuler;
		delete buf;
	}
	else
	{
		ar >> m_RulerUnitPerPixel;
		ar >> m_ImageShowMode;
		ar >> m_Options;
		ar >> size;
		if (size > 0)
		{
			buf = new char[size];
			if (ar.Read(buf, size) == size)
			{
				IStream *pStream;
				if (CreateStreamOnHGlobal(NULL, TRUE, &pStream) == S_OK)
				{
					pStream->Write(buf, size, NULL);
					pStream->Commit(STGC_DEFAULT);
					m_pBackgoundImage = Gdiplus::Image::FromStream(pStream);
					pStream->Release();
				}
			}
		}
		ar >> m_pVerticalRuler;
		ar >> m_pHorizalRuler;
		if (m_pHorizalRuler)
		{
			m_pHorizalRuler->SetUnitPerPixel(m_RulerUnitPerPixel);
			m_pHorizalRuler->SetParent(this);
		}
		if (m_pVerticalRuler)
		{
			m_pVerticalRuler->SetUnitPerPixel(m_RulerUnitPerPixel);
			m_pVerticalRuler->SetParent(this);
		}
	}
}
bool CBackgroundObject::BuildProperties(CXTPPropertyGridItem * pCategoryObjects)
{
	if (!CRectObject::BuildProperties(pCategoryObjects))
	{
		return false;
	}
	CXTPPropertyGridItem *pItem;
	pCategoryObjects->AddChildItem(new CCustomItemPicture(ID_OBJECT_BACKGROUND_IMAGE, GetImage()))->SetItemData((DWORD_PTR) this);
	pItem = pCategoryObjects->AddChildItem(new CXTPPropertyGridItemEnum(ID_OBJECT_BACKGOUND_MODE));
	pItem->GetConstraints()->AddConstraint(_T("拉伸"), 0);
	pItem->GetConstraints()->AddConstraint(_T("中心"), 1);
	pItem->GetConstraints()->AddConstraint(_T("平铺"), 2);
	pItem->SetItemData((DWORD_PTR) this);
	if (m_Options & (WITH_HORIZ_RULER & WITH_VERT_RULER))
	{
		if (m_RulerUnitPerPixel != 0)
		{
			pItem = pCategoryObjects->AddChildItem(new CXTPPropertyGridItemFlags(ID_OBJECT_BACKGROUND_OPTION, 1 + 2));
			pItem->SetItemData((DWORD_PTR) this);
			pItem->GetConstraints()->AddConstraint(_T("垂直标尺"), 2);
			pItem->GetConstraints()->AddConstraint(_T("水平标尺"), 1);
		}
	}
	return true;
}

void CBackgroundObject::OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem)
{
	CCustomItemPicture *pItemPicture = DYNAMIC_DOWNCAST(CCustomItemPicture, pItem);
	CXTPPropertyGridItemFlags *pItemFlags = DYNAMIC_DOWNCAST(CXTPPropertyGridItemFlags, pItem);
	CXTPPropertyGridItemEnum *pItemEnum = DYNAMIC_DOWNCAST(CXTPPropertyGridItemEnum, pItem);

	CRectObject::OnPropertyItemChangedNotify(pItem);
	switch (pItem->GetID())
	{
	case ID_OBJECT_BACKGROUND_IMAGE:
		{
			SetImage(pItemPicture->GetPicture());
		}
		break;
	case ID_OBJECT_BACKGOUND_MODE:
		SetImageShowMode(pItemEnum->GetEnum());
		break;
	case ID_OBJECT_BACKGROUND_OPTION:
		{
			int flag = pItemFlags->GetFlags();
			if (flag & WITH_HORIZ_RULER)
			{
				EnableRuler(HORIZ_RULER, true);
			}
			else
			{
				EnableRuler(HORIZ_RULER, false);
			}
			if (flag & WITH_VERT_RULER)
			{
				EnableRuler(VERT_RULER, true);
			}
			else
			{
				EnableRuler(VERT_RULER, false);
			}
		}
		break;
	default:
		break;

	}
}
