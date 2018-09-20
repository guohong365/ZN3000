// Layout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageContainer.h"
#include "RulerLineObject.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CImageContainer, CObjectsContainer, 0)

void CImageContainer::Initialize()
{  
    _pImageObject = NULL;
}

CImageContainer::CImageContainer():CObjectsContainer()
{
    Initialize();
}

CImageContainer::CImageContainer(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options)
:CObjectsContainer(name, pt, size, options)
{
    Initialize();
    _pImageObject = new CAspectRatioImageObject(_T(""), Gdiplus::Point(0, 0), Gdiplus::Size(1, 1));
    _pImageObject->SetVisible(false);
    _pImageObject->SetParent(this);
}

CImageContainer::CImageContainer(const CString & name, const Gdiplus::Rect & rect, DWORD options)
:CObjectsContainer(name, rect, options)
{
    Initialize();
    _pImageObject = new CAspectRatioImageObject(_T(""), Gdiplus::Rect(0, 0, 1, 1));
    _pImageObject->SetVisible(false);
    _pImageObject->SetParent(this);
}

CImageContainer::~CImageContainer()
{
    delete _pImageObject;
}

void CImageContainer::ResetContent()
{
    CObjectsContainer::ResetContent();
    if (_pImageObject)
    {
        _pImageObject->SetImage(NULL);
    }
}


void CImageContainer::SetImage(Gdiplus::Image * image)
{
    ASSERT(_pImageObject);
	if(_pImageObject->GetImage()==image)
		return;
	OnImageChanging(image);
    if (image)
    {
        Gdiplus::Size size = GetSize();
        _pImageObject->SetImage(image);
        if (_pImageObject->GetSize().Equals(size))
        {
            _pImageObject->OnSizeChanging(size);
        }
        _pImageObject->SetSize(size);
        _pImageObject->SetVisible(true);
    }
    else
    {
        _pImageObject->SetImage(NULL);
        _pImageObject->SetVisible(false);
    }
	OnImageChanged();
}
const Gdiplus::Image * CImageContainer::GetImage() const 
{
	if(_pImageObject)
		return _pImageObject->GetImage();
	return NULL;
}

Gdiplus::Image * CImageContainer::GetImage()
{
	if(_pImageObject)
		return _pImageObject->GetImage();
	return NULL;
}

void CImageContainer::OnImageChanging(Gdiplus::Image * image)
{
	Notify(DNM_IMAGE_CHANGING, 0, &image);
}
void CImageContainer::OnImageChanged()
{
	Notify(DNM_IMAGE_CHANGED);
}


CDrawObject *CImageContainer::CopyFrom(CDrawObject * source)
{
    CImageContainer *pImageObject = DYNAMIC_DOWNCAST(CImageContainer, source);
    ASSERT(pImageObject);
    CObjectsContainer::CopyFrom(source);
    delete _pImageObject;
    if (pImageObject->_pImageObject)
    {
        _pImageObject = DYNAMIC_DOWNCAST(CAspectRatioImageObject, pImageObject->_pImageObject->Clone());
		_pImageObject->SetParent(this);
    }
    else
    {
        _pImageObject = NULL;
    }
    //m_ActiveColor = pImageObject->m_ActiveColor;
    return this;
}

void CImageContainer::MoveImageTo(int x, int y)
{
    _pImageObject->MoveTo(x, y);
}
void CImageContainer::OffsetImage(int dx, int dy)
{
    _pImageObject->Offset(dx, dy);
}
void CImageContainer::ScaleImage(double scale)
{

}
void CImageContainer::RotateImage(double angle)
{
    _pImageObject->SetAngle(_pImageObject->GetAngle() + angle);
}
void CImageContainer::FlipImage(long direction)
{

}
void CImageContainer::SelectImage(bool isSelected)
{
    _pImageObject->SetSelected(isSelected);
}

void CImageContainer::Serialize(CArchive & ar)
{
    CObjectsContainer::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << _pImageObject;
    }
    else
    {
        ar >> _pImageObject;
        if (_pImageObject)
        {
            _pImageObject->SetParent(this);
        }
    }
}

CDrawObject *CImageContainer::GetObjectsAt(const Gdiplus::Point & point)
{
    CDrawObject *pObject = CObjectsContainer::GetObjectsAt(point);
    if (pObject)
    {
        return pObject;
    }
    if (_pImageObject && _pImageObject->GetVisible())
    {
        Gdiplus::Point pt = point;
        Global2Local(&pt);
        _pImageObject->Global2Local(&pt);
        if (_pImageObject->HitTest(pt))
        {
            return _pImageObject;
        }
    }
    return NULL;
}


//(����)
void CImageContainer::OnDrawBorder(Gdiplus::Graphics & graph)
{
	Gdiplus::Color color;
	float width=GetLineWidth();
	if(GetActive() && !CDrawObject::IsPrinting())
	{
		color=GetActiveColor();
		width=5;
	}
	else
	{
		color=GetLineColor();
	}
	Gdiplus::Pen pen(color, width);
	pen.SetDashStyle((Gdiplus::DashStyle)GetLineStyle());
	Gdiplus::Rect rect = Gdiplus::Rect(Gdiplus::Point(0, 0), GetSize());
	graph.DrawRectangle(&pen, rect);
}
void CImageContainer::OnDrawTracker(Gdiplus::Graphics & graph)
{
    Gdiplus::Pen pen(GetActiveColor(), 4.0f);
    Gdiplus::Rect rect = Gdiplus::Rect(Gdiplus::Point(0, 0), GetSize());
    rect.Inflate(-2, -2);
    graph.DrawRectangle(&pen, rect);
}


//�¼�(����)
void CImageContainer::OnDraw(Gdiplus::Graphics & graph)
{
    DrawFillObject(graph);
    if (GetShowName())
    {
        float h = (float) (GetSize().Width) / GetName().GetLength() / 6;
        Gdiplus::StringFormat stringFormat;
        stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
        stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
        Gdiplus::Color color = GetFontColor();
        BYTE alpha = (BYTE) (GetTransparent() * 255);
        Gdiplus::ARGB argb = color.GetValue();
        argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
        color = argb;
        Gdiplus::SolidBrush brush(color);
        Gdiplus::Font font(_T("����"), h);
        graph.DrawString(GetName(), -1, &font, Gdiplus::PointF(10, 10), &stringFormat, &brush);
    }
    if(_pImageObject)
	{
		_pImageObject->Draw(graph);
	}
	DrawRuler(graph);
    GetObjects().Draw(graph);

}

bool CImageContainer::BuildProperties(CXTPPropertyGridItem * pCategoryObjects)
{
    if (!CRectObject::BuildProperties(pCategoryObjects))
        return false;

//       CXTPPropertyGridItem *pItem;

    //pCategoryObjects->
    //    AddChildItem(new
    //                 CXTPPropertyGridItemColor(ID_OBJECT_IMAGE_CONTAINER_ACTIVE_COLOR,
    //                                           RGB(m_ActiveColor.GetR(), m_ActiveColor.GetG(), m_ActiveColor.GetB())))->SetItemData((DWORD_PTR) this);

    return BuildRulerProperty(pCategoryObjects);
}

void CImageContainer::OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem)
{
    CObjectsContainer::OnPropertyItemChangedNotify(pItem);

    CXTPPropertyGridItemColor *pItemColor = DYNAMIC_DOWNCAST(CXTPPropertyGridItemColor, pItem);
    Gdiplus::Color color;
    switch (pItem->GetID())
    {
    case ID_OBJECT_IMAGE_CONTAINER_ACTIVE_COLOR:
        color.SetFromCOLORREF(pItemColor->GetColor());
        SetActiveColor(color);
        break;
    default:
        break;
    }
}

CAspectRatioImageObject *CImageContainer::GetImageObject()
{
    return _pImageObject;
}

bool CImageContainer::LoadImage(const CString & fileName)
{
    _pImageObject->LoadImage(fileName);
    _pImageObject->SetSize(GetSize());
    _pImageObject->Offset((GetSize().Width - _pImageObject->GetSize().Width) / 2, (GetSize().Height - _pImageObject->GetSize().Height) / 2);
    _pImageObject->SetRotateCenter(Gdiplus::Point(_pImageObject->GetSize().Width / 2, _pImageObject->GetSize().Height / 2));
    _pImageObject->SetVisible(true);
    return true;
}

void CImageContainer::Clear()
{
    CObjectsContainer::Clear();
	if(_pImageObject)
	{
		_pImageObject->SetImage(NULL);
	}
}

void CImageContainer::SelectAll(CDrawObjectList & objList, int nFlag /*=SELECT_ADD*/ )
{
    if (nFlag == SELECT_REPLACE)
    {
        objList.RemoveAll();
    }
    if (_pImageObject)
    {
        objList.AddTail(_pImageObject);
    }
    CObjectsContainer::SelectAll(objList);
}


void CImageContainer::OnSizeChanged()
{
    CObjectsContainer::OnSizeChanged();
    double ratio;
    if (_pImageObject)
    {
        Gdiplus::Size size = _pImageObject->GetSize();
        _pImageObject->SetSize(GetSize());
        ratio = (double) _pImageObject->GetSize().Width / size.Width;
        double rulerScale = GetRulerScale() / ratio;
        SetRulerScale(rulerScale);
    }
}

void CImageContainer::OnTransparentChanged()
{
	__super::OnTransparentChanged();
    _pImageObject->SetTransparent(GetTransparent());
}

void CImageContainer::OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam)
{
    POSITION pos;
    CDrawObject *pObject;
    if(pSource->IsKindOf(RUNTIME_CLASS(C3DCorrectorObject)))
    {
        C3DCorrectorObject * pCorrector=DYNAMIC_DOWNCAST(C3DCorrectorObject, pSource);
        ASSERT(pCorrector);
        pos=GetObjects().GetHeadPosition();
        while(pos)
        {
            pObject=GetObjects().GetNext(pos);
            if(pObject->IsKindOf(RUNTIME_CLASS(CRulerObject)))
            {
                //���±�߿̶�
            }
        }
    }
    if (pSource == _pImageObject)
    {
        double *pdValue = (double *) lpParam;
        Gdiplus::Size * pSize = (Gdiplus::Size *) lpParam;
        Gdiplus::Point * pPoint = (Gdiplus::Point *) lpParam;

        CString strValue;
        pos = GetObjects().GetHeadPosition();
        while (pos)
        {
            pObject = GetObjects().GetNext(pos);
            switch (messageID)
            {
            case DNM_POSITION_CHANGING:        //      5
                ASSERT(pPoint);
                if (pObject->GetCustomProperty(_T("LockOnImage"), strValue) && strValue == _T("TRUE"))
                {
                    Gdiplus::Point offset = *pPoint - pSource->GetPosition();
                    pObject->Offset(offset);
                }
                break;
            case DNM_ANGLE_CHANGING:   //         6
                ASSERT(pdValue);
                if (pObject->GetCustomProperty(_T("LockOnImage"), strValue) && strValue == _T("TRUE"))
                {
					//������ͼƬ��ͼƬ��ת������ת���൱�ڸ�������������ת������תһ�Ƕȣ�Ȼ����ת������
					//ͼƬ��ת������ת�ƶ�
                    
					//ȡͼƬ��ת����
					Gdiplus::Point center1 = pSource->GetRotateCenter();
					//ȥ������ת����
					Gdiplus::Point center2 = pObject->GetRotateCenter();
					Gdiplus::Point center0;
					//����������תΪȫ�����꣨����������ϵ��
                    pSource->Local2Global(&center1);
					pObject->Local2Global(&center2);
					//��������ԭʼ��ת��������
					center0=center2;

                    //����������ת������ͼƬ��ת����ת��������
					Gdiplus::Matrix matrix;
					matrix.RotateAt((float)(*pdValue - pSource->GetAngle()), Gdiplus::PointF((float)center1.X, (float)center1.Y));
					matrix.TransformPoints(&center2);

					//������������ת������תһ�Ƕ�
                    pObject->Rotate(*pdValue - pSource->GetAngle());
					//������ת����ƽ����ת����λ��
					pObject->Offset(center2-center0);
                }
                break;
            case DNM_SIZE_CHANGING:    //          4
				{
					double ratio;
					Gdiplus::Point pos;
					//WARNING:BUG ���㷨���ʺ������ͼƬ����ת�Ƕ�һ�µ������
					//�������ͼƬ�ǶȲ�һ��ʱ�����겻��ȷ�����޸ģ�
					if (pObject->GetCustomProperty(_T("LockOnImage"), strValue) && strValue == _T("TRUE"))
					{
						//����Ŵ�ϵ��
						ratio = (double) pSize->Width / pSource->GetSize().Width;
						//������PositionΪԭ��Ŵ�������
						pObject->Scale(ratio, ratio);
						//ȡ����λ�ã�������CImageContainer���꣩
						pos = pObject->GetPosition();
						//ת��ΪͼƬ��������
						pSource->Global2Local(&pos);
						//����ͼƬ�Ŵ��ԭ����λ�õ�������
						pos.X=(int)(pos.X  * ratio);
						pos.Y=(int)(pos.Y  * ratio);
						//����ظ�����ȫ������
						pSource->Local2Global(&pos);
						//�趨����λ�õ�������
						pObject->SetPosition(pos);
					}
				}
                break;
            case DNM_ROTATE_CENTER_CHANGED:    //  8   
                if (pObject->GetCustomProperty(_T("LockOnImage"), strValue) && strValue == _T("TRUE"))
                {
                    Gdiplus::Point center = pSource->GetRotateCenter();
                    pSource->Local2Global(&center);
                    pObject->Global2Local(&center);
                    pObject->SetRotateCenter(center);
					pObject->Local2Global(&center);
                }
                break;
            case DNM_IMAGE_CHANGED:
                SetRulerScale(0);
                break;
            default:
                break;
            }
		}
	}
}

bool CImageContainer::IsEmpty()
{
    return (_pImageObject==NULL || _pImageObject->GetImage()==NULL);
}
