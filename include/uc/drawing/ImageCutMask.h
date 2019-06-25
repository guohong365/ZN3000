#pragma once
#include "PolygonObject.h"
#include "RectObject.h"
#include "EllipseObject.h"
class CImageCutMaskObject:public CDrawObject
{
	CDrawObject *_pTarget;
	CDrawObject *_pInternalObject;
	Gdiplus::GraphicsPath _cutPath;
	DECLARE_SERIAL(CImageCutMaskObject)
public:
	CImageCutMaskObject(CDrawObject *pTarget=NULL);
	virtual ~CImageCutMaskObject();
	virtual CDrawObject* GetTarget();
	virtual void SetTarget(CDrawObject *pTarget);
	virtual void CalculateCutRegion(Gdiplus::Point *pPoints, int count, Gdiplus::GraphicsPath *pCutRegion);
	virtual bool CutImage();
	virtual void GetCutRegion(Gdiplus::GraphicsPath &path);
	virtual int GetPointsCount();
	virtual int GetPoints(Gdiplus::Point *pBuffer, int count);
	virtual bool OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point);

	virtual CDrawObject* GetInternalObject();
	virtual void SetInternalObject(CDrawObject*pObject);
	virtual LPCTSTR GetObjectType() const;
	//��������
	virtual void SetName(CString name);
	virtual CString  GetName();

	//λ��
	virtual void SetPosition(Gdiplus::Point point);
	virtual Gdiplus::Point GetPosition() const;
	//��С
	virtual void SetSize(Gdiplus::Size size);
	virtual Gdiplus::Size GetSize() const;
	//��ת����
	void SetRotateCenter(Gdiplus::Point center);
	virtual Gdiplus::Point GetRotateCenter();
	//��ת�Ƕ�
	virtual void SetAngle(double angle);
	virtual double GetAngle() const;

	virtual void SetClipRect(Gdiplus::Rect rect);
	virtual Gdiplus::Rect GetClipRect() const;
	//�Ƿ����
	virtual void SetUsingClip(bool isUsingClip);
	virtual bool GetUsingClip() const;
	//�Ƿ���ʾ
	virtual void SetVisible(bool isVisible);
	virtual bool GetVisible() const;
	//�Ƿ����
	virtual void SetFilled(bool isFilled);
	virtual bool GetFilled() const;
	//�Ƿ���Ʊ߿�
	virtual void SetShowBorder(bool isShowBorder);
	virtual bool GetShowBorder() const;

	virtual void SetShowTracker(bool isShowTracker);
	virtual bool GetShowTracker() const;
	virtual void SetTrackerColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetTrackerColor() const;

	//͸����
	virtual void SetTransparent(float transparent);
	virtual float GetTransparent() const;

	//�����ɫ
	virtual void SetFillColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetFillColor() const;
	//�߿���ɫ
	virtual void SetLineColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetLineColor() const;
	virtual void SetLineWidth(float width);
	virtual float GetLineWidth();

	virtual void SetLineStyle(int style);
	virtual int GetLineStyle();

	virtual void SetActiveColor(Gdiplus::Color activeColor);
	virtual Gdiplus::Color GetActiveColor();

	//������ɫ
	virtual void SetFontColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetFontColor() const;

	virtual void SetFontFace(const CString & fontFace);
	virtual CString GetFontFace();
	virtual void SetFontStyle(int style);
	virtual int GetFontStyle();
	virtual void SetFontSize(float fontSize);
	virtual float GetFontSize();

	virtual void SetTextAlign(int align);
	virtual int GetTextAlign();

	virtual void SetTextLineAlign(int align);
	virtual int GetTextLineAlign();
	virtual void SetTextIsVert(bool isVert);
	virtual bool GetTextIsVert();

	//////////////////////////////////////////////////////////////////////////

	//��������
	virtual void SetActive(bool isActive);
	virtual bool GetActive();

	//�Ƿ�Ϊѡ��״̬
	virtual void SetSelected(bool isSelected);
	virtual bool GetSelected() const;

	//////////////////////////////////////////////////////////////////////////

	virtual void SetParent(CDrawObject * pParent);
	virtual const CDrawObject *GetParent() const;
	virtual CDrawObject *GetParent();

	virtual Gdiplus::Rect GetWorldBounds()
	{
		return _pInternalObject->GetWorldBounds();
	}
	virtual bool GetWorldAngle(double *angle, Gdiplus::Point * rotateCenter)
	{
		return _pInternalObject->GetWorldAngle(angle, rotateCenter);
	}

	//////////////////////////////////////////////////////////////////////////

	//ʹ������

	virtual void SetSelectable(bool isSelectable)
	{
		_pInternalObject->SetSelectable(isSelectable);
	}
	virtual bool GetSelectable()
	{
		return _pInternalObject->GetSelectable();
	}

	//���ƶ�
	virtual void SetMovable(bool isMovable)
	{
		_pInternalObject->SetMovable(isMovable);
	}
	virtual bool GetMovable()
	{
		return _pInternalObject->GetMovable();
	}


	//����ת
	virtual void SetRotatable(bool isRotatable)
	{
		_pInternalObject->SetRotatable(isRotatable);
	}
	virtual bool GetRotatable()
	{
		return _pInternalObject->GetRotatable();
	}

	//�ɸı��С
	virtual void SetSizable(bool isSizable)
	{
		_pInternalObject->SetSizable(isSizable);
	}
	virtual bool GetSizable()
	{
		return _pInternalObject->GetSizable();
	}

	//method

	virtual void OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam);
	virtual void Notify(UINT msgID, DWORD_PTR wParam = 0, LPVOID lpParam = NULL);


	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);

	//////////////////////////////////////////////////////////////////////////
	//�����б�֧��
	//////////////////////////////////////////////////////////////////////////
	//���ƶ���
	//@param source ����source����ǰ���ݵ������󣬲����Ƴ����б�
	virtual CDrawObject *CopyFrom(CDrawObject * source);

	//////////////////////////////////////////////////////////////////////////

	virtual void Draw(Gdiplus::Graphics & graph);


	//////////////////////////////////////////////////////////////////////////
	//���β���
	//////////////////////////////////////////////////////////////////////////
	virtual void Local2World(Gdiplus::Point * pt, int count = 1);
	virtual void World2Local(Gdiplus::Point * pt, int count = 1);

	//����ת������
	virtual void Global2Local(Gdiplus::Point * pt, int count = 1);
	virtual void Local2Global(Gdiplus::Point * pt, int count = 1);

	virtual Gdiplus::Point GetCenter();
	virtual Gdiplus::Rect GetObjectRect();
	virtual void GetObjectRegion(Gdiplus::GraphicsPath &regionPath);
	virtual Gdiplus::Rect GetBounds();
	virtual void MoveTo(Gdiplus::Point point);
	virtual void MoveTo(int x, int y);

	virtual void Offset(int x, int y);
	virtual void Offset(Gdiplus::Point point);
	virtual void Rotate(double angle);
	virtual void Scale(double scaleX, double scaleY);

	virtual void ScaleAt(double scaleX, double scaleY, Gdiplus::Point origin);

	virtual int GetHandleCount();
	virtual Gdiplus::Point GetHandle(int nHandle);
	virtual Gdiplus::Rect GetHandleRect(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);

	virtual int HitTest(const Gdiplus::Point & pt);

	//���л�֧��
	virtual void Serialize(CArchive & ar);
};

