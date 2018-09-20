#pragma once
#include "DrawObjectPropertiesID.h"
#include "BaseTypes.h"
#include "DrawObjectGlobal.h"
#include "ObjectObserver.h"
#include "IVisaulized.h"
#include "Appearance.h"
#include <vector>
#define FONT_SIZE_FACETOR   3.53f

class CDrawObject;

typedef CTypedPtrList <CObList,  CDrawObject * >CObjectsList;

/////
///����Ϣ
///
///���ڼ����ĵ�ʱ�ع��󶨹�ϵ������󶨹�ϵʱ��ֻ�����Ŀ����ڲ������б�
///����ʱ����ȫ�ְ���Ϣ��������ɺ��ؽ����ж���İ󶨹�ϵ��
///@see SetBindBuilderState
///@see IsBindBuilding
///@see BuildBinding
///
struct BIND_OBJ_INFO
{
	CDrawObject *pSource;   //< ��Դ
	CStringList targetList; //< ��Ŀ���ڲ������б�
};

//////////////////////////////////////////////////////////////////////////
///������Ϣ
///
///����ı�ʱ���ڱ���Undo��־���¡��������������ֵ
///@see CHistoryFontChanged
///@see notifyAttribtesChanged
///
struct FontInfo
{
    UINT mask;
	CString fontName; ///< ��������
	float fontSize;   ///<�����С
	int fontStyle;    ///<�����񣬰���Bold��Italic��Underline
	Gdiplus::Color fontColor; ///<������ɫ
};

template <typename T> struct CLogData
{
	T _data;
	CLogData(const T &data)
	{
		_data=data;
	}
};


/*!
 * \brief
 * ��ͼ�������
 * 
 * Write detailed description for CDrawObject here.
 * 
 * \remarks
 * Write remarks for CDrawObject here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CDrawObject:public CObject, public IObjectObserver, public IVisaulized
{
public:
    typedef std::vector<CDrawObject*> DRAW_OBJECT_LIST;

	////////////
	///��ʼ����Ա����
	///
	void Initialize();
	CStringList m_CustomPropertyList;
	CMapStringToString m_CustomProperties;

protected:
	//////////////////////////////////////////////////////////////////////////
	///Ĭ�Ϲ��캯��
	///
	///��RuntimeClass��CreateObject����
	///
	CDrawObject();

	///
	///����Դ�����б�. �󶨵�����Ķ����б��������Է����仯ʱͨ��Notify֪ͨ��Щ����Դ��
	///@see Notify
	///
	std::vector<CDrawObject*> _bindedObjects;

	//////////////////////////////////////////////////////////////////////////
	///��Ŀ������б�
	///
	///����󶨵Ķ����б�Ŀ��������Է����仯ʱ�������Notify֪ͨ������
	///������OnNotify�ж���Щ֪ͨ��Ϣ���д���
	///@see Notify
	///@see OnNotify
	std::vector<CDrawObject*> _bindTargets;

	///
	///ȡ�ð󶨵�����Ķ����б�
	///
	///@return �����б�
	///
	std::vector<CDrawObject*> & GeBindedObjects();
	///
	///ȡ������󶨵Ķ����б�
	///
	///@return �����б�
	///
	std::vector<CDrawObject*> & GetBindTargets();

	///
	///��Ŀ�����ǰ�¼�
	///
	///�̳���ɸ��Ǹ÷���������󶨵�Ŀ�����ǰ��׼������
	///
	///@param pTarget ���󶨵�Ŀ����� 
	///
	virtual void OnBinding(CDrawObject*pTarget);
	///
	///��Ŀ�������¼�
	///
	///�̳���ɸ��Ǹ÷���������󶨵�Ŀ������ĺ���������
	///
	///@param pTarget ���󶨵�Ŀ����� 
	///
	virtual void OnBinded(CDrawObject *pTarget);

	virtual void OnUnbindingFrom(CDrawObject *pTarget){}
	virtual void OnUnbindedFrom(CDrawObject *pTarget){}
	DECLARE_SERIAL(CDrawObject) 
	
public:  //IObjectObserver directive
	///
	///IObjectObserver�ӿ�����
	///
	virtual void notifyAttribtesChanged(CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal);
    virtual void notifyRevert(CDrawObject *pChanged, int &action );

    //IVisaulized
    Gdiplus::Image* GetPicture( int  width, int  height, Gdiplus::Color background=Gdiplus::Color::Transparent, int flag=GET_PIC_FLAG_ASPECT | GET_PIC_POS_HCENTER | GET_PIC_POS_VCENTER );

	///
	///��������󶨵�Ŀ�����
	///
	///@param pTarget ���󶨵�Ŀ�����
	///
	void BindTo(CDrawObject *pTarget);
	///�ѱ�������Ŀ���������
	///
	///@param pTarget ����󶨵�Ŀ�����
	///
	void UnbindFrom(CDrawObject *pTarget);
	void UnbindAll();

    virtual void ResetContent();


	CDrawObject(const CString & name);
	CDrawObject(const CString & name, int x, int y, int width, int height);
	CDrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);
	CDrawObject(const CString & name, const Gdiplus::Rect & rect);
	virtual ~ CDrawObject();

	//////////////////////////////////////////////////////////////////////////
	///���ð��ؽ�״̬λ
	///
	///@param state 
	/// - true ��ֹ���¼���Ӧ
	/// - false ������¼���Ӧ
	///
	///@remark���ļ�����ʱ����Ҫ�ð��ؽ�״̬λΪture��������¼��������󶨹�ϵ������ɺ󣬵��øú�����Ϊfalse�����������
	///
	static void SetBindBuilderState(bool state);
	//////////////////////////////////////////////////////////////////////////
	///ȡ���ؽ�״̬λ
	///
	///@return �ؽ�״̬λ
	///
	static bool IsBindBuilding();
	//////////////////////////////////////////////////////////////////////////
	///�ؽ��󶨹�ϵ
	///
	///�÷������ļ����غ󣬼����ã��Իָ�����󶨹�ϵ��
	///
	static void BuildBinding();
	///
	///���Ҷ����б��е��ض����������
	///
	///@param objects �����б�
	///@param pTarget ����Ŀ��
	///@return Ŀ��������б��е�����λ�ã�0��ʼ��δ�ҵ�����-1��
	///
	static int _findIndex(std::vector<CDrawObject*> & objects, CDrawObject* pTarget);
	///
	///���ݶ��ڲ������ƣ����б��в��Ҷ���
	///
	///@param objects �����б�
	///@param innerName �����ڲ�����
	///@return �ҵ��Ķ���δ�ҵ�����NULL��
	///
	static CDrawObject * _findObjectByName(DRAW_OBJECT_LIST &objects,  const CString & innerName);

	//////////////////////////////////////////////////////////////////////////
	///���ô�ӡ״̬λ
	///
	///��ӡ���ǰ�����ô�ӡ״̬λΪtrue�����⣬���ֶ������ʾ�����ɴ�ӡ�Ĳ��ֱ���ӡ��
	///
	///@param state ��ӡ״̬λ����ֵ
	///
	static void SetPrintingState(bool state);
	///ȡ��ӡ״̬λ
	///
	///@return ��ӡ״̬λ
	/// - true ��ӡ��
	/// - false ����
	static bool IsPrinting();
	///����Ψһ�ַ���
	///
	///�������ɶ����ڲ�����
	///
	///@return ����ʱ��Ψһ�Ե��ַ���
	static CString GenerateUniqueName();
	
	//Attributes

	//ֻ��
	
	///
	///��������
	///
	enum ObjectType
	{
		Shape, ///< ��״��һ���ͼ����
		Operator, ///< ����������������������Ԥ�Ȼ��ƵĶ����������ڼ��е�CCutMask����
		Tool,  ///< ���߶������������߶��󣬸����߶���
		Mark   ///< ��Ƕ���Ŀǰδ�õ�
	};
	//////////////////////////////////////////////////////////////////////////
	///�Ӷ�������Idȡ�ö�����������
	///
	///@prama Id ��������Id�� ȡֵΪObjectTypeö��ֵ
	///@return ������������
	static LPCTSTR GetObjectType(int Id);
	///
	///ȡ������������
	///
	///@return ��������
	///
	virtual LPCTSTR GetObjectType() const;
	///
	///ȡ��������Id
	///
	///@return ��������ID
	///
	virtual int GetObjectTypeID() const;
	///
	///���ö�������
	///
	///@param ������
	///
	virtual void SetName(CString name);
	///
	///ȡ��������
	///
	virtual CString  GetName();
	virtual void OnNameChanging(CString & newName);
	virtual void OnNameChanged();

	///�����ڲ����ƣ���Ϊ����������Ψһ��ʶ
	virtual const CString GetInternalName() const;
	virtual void SetInternalName(CString internalName);
	virtual void OnInternalNameChanging(CString & internalName);
	virtual void OnInternalNameChanged();

	//////////////////////////////////////////////////////////////////////////

	//�������ԡ����Ƕȵ�λ���ȣ�

	//���굥λ
	static void SetUnit(Gdiplus::Unit unit);
	static Gdiplus::Unit GetUnit();

	///����λ��
	virtual void SetPosition(Gdiplus::Point point);
	virtual Gdiplus::Point GetPosition() const;
	virtual void OnPositionChanging(Gdiplus::Point & newPoint);
	virtual void OnPositionChanged();
	//�����С
	virtual void SetSize(Gdiplus::Size size);
	virtual Gdiplus::Size GetSize() const;
	virtual void OnSizeChanging(Gdiplus::Size & newSize);
	virtual void OnSizeChanged();
	//������ת����
	//TODO: BUG ��ת���ƶ���ת���Ľ�Ӱ��_Angle��ȡֵ��������_Angle��ֵ
	void SetRotateCenter(Gdiplus::Point center);
	virtual Gdiplus::Point GetRotateCenter();
	virtual void OnRotateCenterChanging(Gdiplus::Point & newCenter);
	virtual void OnRotateCenterChanged();
	//��ת�Ƕ�
	virtual void SetAngle(double angle);
	virtual double GetAngle() const;
	virtual void OnAngleChanging(double &newAngle);
	virtual void OnAngleChanged();

	virtual bool GetWorldAngle(double *angle, Gdiplus::Point * rotateCenter);

	//////////////////////////////////////////////////////////////////////////

	//��ͼ����

	//���б߿�
	virtual void SetClipRect(Gdiplus::Rect rect);
	virtual Gdiplus::Rect GetClipRect() const;
	virtual void OnClipRectChanging(Gdiplus::Rect & newRect);
	virtual void OnClipRectChanged();
	//�Ƿ����
	virtual void SetUsingClip(bool isUsingClip);
	virtual bool GetUsingClip() const;
	virtual void OnUsingClipChanging(bool & newUsingClip);
	virtual void OnUsingClipChanged();
	//�Ƿ���ʾ
	virtual void SetVisible(bool isVisible);
	virtual bool GetVisible() const;
	virtual void OnVisibleChanging(bool & newVisible);
	virtual void OnVisibleChanged();
	//�Ƿ����
	virtual void SetFilled(bool isFilled);
	virtual bool GetFilled() const;
	virtual void OnFilledChanging(bool & newFilled);
	virtual void OnFilledChanged();
	//�Ƿ���Ʊ߿�
	virtual void SetShowBorder(bool isShowBorder);
	virtual bool GetShowBorder() const;
	virtual void OnShowBorderChanging(bool & newShowBorder);
	virtual void OnShowBorderChanged();

	//�Ƿ���ʾ������
	virtual void SetShowTracker(bool isShowTracker);
	virtual bool GetShowTracker() const;
	virtual void OnShowTrackerChanging(bool & newShowTracker);
	virtual void OnShowTrackerChanged();

	//��������ɫ
	virtual void SetTrackerColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetTrackerColor() const;
	virtual void OnTrackerColorChanging(Gdiplus::Color & color);
	virtual void OnTrackerColorChanged();


	//͸����
	virtual void SetTransparent(FLOAT transparent);
	virtual FLOAT GetTransparent() const;
	virtual void OnTransparentChanging(FLOAT &newTransparent);
	virtual void OnTransparentChanged();

	//�����ɫ
	virtual void SetFillColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetFillColor() const;
	virtual void OnFillColorChanging(Gdiplus::Color & newColor);
	virtual void OnFillColorChanged();
	//������ɫ
	virtual void SetLineColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetLineColor() const;
	virtual void OnLineColorChanging(Gdiplus::Color & newColor);
	virtual void OnLineColorChanged();

	//�߿�
	virtual void SetLineWidth(FLOAT width);
	virtual FLOAT GetLineWidth();
	virtual void OnLineWidthChanging(FLOAT &width);
	virtual void OnLineWidthChanged();

	//����
	virtual void SetLineStyle(INT style);
	virtual INT GetLineStyle();
	virtual void OnLineStyleChanging(INT & style);
	virtual void OnLineStyleChanged();

	///���󼤻���ɫ
	virtual void SetActiveColor(Gdiplus::Color activeColor);
	virtual Gdiplus::Color GetActiveColor();
	virtual void OnActiveColorChanging(Gdiplus::Color &color);
	virtual void OnActiveColorChanged();

	//������ɫ
	virtual void SetFontColor(Gdiplus::Color color);
	virtual Gdiplus::Color GetFontColor() const;
	virtual void OnFontColorChanging(Gdiplus::Color & newFontColor);
	virtual void OnFontColorChanged();

	//��������
	virtual void SetFontFace(const CString & fontFace);
	virtual CString GetFontFace();
	virtual void OnFontFaceChanging(CString & faceName);
	virtual void OnFontFaceChanged();
	
	//������
	virtual void SetFontStyle(INT style);
	virtual INT GetFontStyle();
	virtual void OnFontStyleChanging(INT & style);
	virtual void OnFontStyleChanged();

	//�����С
	virtual void SetFontSize(FLOAT fontSize);
	virtual FLOAT GetFontSize();
	virtual void OnFontSizeChanging(FLOAT &fontSize);
	virtual void OnFontSizeChanged();

	//����ˮƽ���뷽ʽ
	virtual void SetTextAlign(INT align);
	virtual INT GetTextAlign();
	virtual void OnTextAlignmentChanging(INT & align);
	virtual void OnTextAlignmentChanged();

	//���ִ�ֱ���뷽ʽ
	virtual void SetTextLineAlign(INT align);
	virtual INT GetTextLineAlign();
	virtual void OnTextLineAlignChanging(INT & align);
	virtual void OnTextLineAlignChanged();
	
	//��������
	virtual void SetTextIsVert(bool isVert);
	virtual bool GetTextIsVert();
	virtual void OnTextVertChanging(bool & isVert);
	virtual void OnTextVertChanged();

	//////////////////////////////////////////////////////////////////////////

	//��������

	//���󼤻�״̬
	virtual void SetActive(bool isActive);
	virtual bool GetActive();
	virtual void OnActiveChanging(bool &isActived);
	virtual void OnActiveChanged();

	//����ѡ��״̬
	virtual void SetSelected(bool isSelected);
	virtual bool GetSelected() const;
	virtual void OnSelectedChanging(bool & newSelected);
	virtual void OnSelectedChanged();
	//////////////////////////////////////////////////////////////////////////

	//���󸸶���
	virtual void SetParent(CDrawObject * pParent);
	virtual const CDrawObject *GetParent() const;
	virtual CDrawObject *GetParent();
	virtual void OnParentChanging(CDrawObject * & pObject);
	virtual void OnParentChanged();

	//////////////////////////////////////////////////////////////////////////

	//ʹ������

	//�����֧�ֵ����������б�
	static CStringList & GetPropertyList();

	//��ѡ��
	virtual void SetSelectable(bool isSelectable);
	virtual bool GetSelectable();
	virtual void OnSelectableChanging(bool & isSelectable);
	virtual void OnSelectableChanged();

	//���ƶ�
	virtual void SetMovable(bool isMovable);
	virtual bool GetMovable();
	virtual void OnMovableChanging(bool & isMovable);
	virtual void OnMovableChanged();

	//����ת
	virtual void SetRotatable(bool isRotatable);
	virtual bool GetRotatable();
	virtual void OnRotatableChanging(bool &isRotatable);
	virtual void OnRotatableChanged();

	//�ɸı��С
	virtual void SetSizable(bool isSizable);
	virtual bool GetSizable();
	virtual void OnSizableChanging(bool & isSizable);
	virtual void OnSizableChanged();


	//////////////////////////////////////////////////////////////////////////

	//�Զ�������
	virtual CStringList & GetCustomPropertyList();
	virtual void AddCustomProperty(const CString & propertyName, const CString & propertyValue);
	virtual void RemoveCustomProperty(const CString & propertyName);
	virtual bool GetCustomProperty(const CString & propertyName, CString & propertyValue);
	//////////////////////////////////////////////////////////////////////////


	//method
	///
	///��Ϣ֪ͨ
	virtual void Notify(UINT msgID, DWORD_PTR wParam = 0, LPVOID lpParam = NULL);
	///֪ͨ��Ӧ
	virtual void OnNotify(CDrawObject * pSource, UINT messageID, DWORD_PTR wParam, LPVOID lpParam);

	//UI ����

	//�������Ա�
	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	///
	///���Ըı���Ӧ
	///
	///ͨ�����Ա����޸ı����������ֵ��ͨ���ص��÷����޸Ķ�����Ӧ����ֵ
	///
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);

	///TODO
	void SetCommonMenuId(UINT commonMenuId);
	virtual void SetMenuId(UINT menuId);
	virtual UINT GetMenuId();
	///
	CMenu * GetObjectMenu(CMenu * pMenu);

	virtual void OnMenuCommand(UINT command);

	virtual bool OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point);
	virtual bool OnLButtonDblClk(CWnd *pWnd, UINT nFlags, Gdiplus::Point point);

	//////////////////////////////////////////////////////////////////////////
	//�����б�֧��
	//////////////////////////////////////////////////////////////////////////
	//���ƶ���
	//@param source ����source����ǰ���ݵ������󣬲����Ƴ����б�
	virtual CDrawObject *CopyFrom(CDrawObject * source);

	//������ȸ��Ƹ���
	//�����µĶ���ռ䣬���Ƶ�ǰ���ݵ��¶���
	virtual CDrawObject *Clone();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//��ͼ֧��
	//////////////////////////////////////////////////////////////////////////

	//���ƶ�����
	virtual void DrawTracker(Gdiplus::Graphics & graph);
	//�����������¼���Ӧ
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);

	//���ƶ���
	virtual void Draw(Gdiplus::Graphics & graph);
	//��������¼���Ӧ
	virtual void OnDraw(Gdiplus::Graphics & graph);

	//���Ʊ߿�
	virtual void DrawBorder(Gdiplus::Graphics & graph);
	//�߿�����¼���Ӧ
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);

	//������
	virtual void DrawFillObject(Gdiplus::Graphics & graph);
	//��������¼���Ӧ
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);


	//////////////////////////////////////////////////////////////////////////
	//���β���
	//////////////////////////////////////////////////////////////////////////
	
	//����ת������

	//��������任����������
	virtual void Local2World(Gdiplus::Point * pt, int count = 1);
	virtual void Local2World(Gdiplus::Rect *rect, int count = 1);
	//��������任����������
	virtual void World2Local(Gdiplus::Point * pt, int count = 1);
	virtual void World2Local(Gdiplus::Rect *rect, int count = 1);

	//ȫ�����꣨���������꣩�任����������
	virtual void Global2Local(Gdiplus::Point * pt, int count = 1);
	virtual void Global2Local(Gdiplus::Rect *rect, int count = 1);
	//��������任��ȫ�����꣨���������꣩
	virtual void Local2Global(Gdiplus::Point * pt, int count = 1);
	virtual void Local2Global(Gdiplus::Rect *rect, int count = 1);

	//ȡ�����������ꡣ��������
	virtual Gdiplus::Point GetCenter();
	//ȡ���������Σ��������ת����ת����������
	virtual Gdiplus::Rect GetObjectRect();

	//ȡ�������򣬱�������
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);


	//ȡ�������߽������Σ�ȫ������
	virtual Gdiplus::Rect GetBounds();

	virtual Gdiplus::Rect GetWorldBounds();

	//�ƶ���pointλ��
	//@param point ����λ�ã�ȫ������
	virtual void MoveTo(Gdiplus::Point point);
	virtual void MoveTo(int x, int y);

	//ƫ��
	//@param x x����ƫ��
	//@param y y����ƫ��
	virtual void Offset(int x, int y);
	virtual void Offset(Gdiplus::Point point);

	//��ת
	//���ڵ�ǰ��ת���ģ��Ե�ǰ�Ƕ���תangle�ȣ�angleС��0����������angle����0��������
	virtual void Rotate(double angle);

	
	//����
	//���ڱ�������ԭ������
	//@param scaleX  x�������ű���
	//@param scaleY  y�������ű���
	virtual void Scale(double scaleX, double scaleY);

	//���ڸ������Ż�������
	virtual void ScaleAt(double scaleX, double scaleY, Gdiplus::Point origin);
	
	//ȡ����������
	virtual int GetHandleCount();
	
	//���ݾ����ȡ���λ�ã���������
	virtual Gdiplus::Point GetHandle(int nHandle);

	//���ݾ���ţ�ȡ������ο򣬱�������
	virtual Gdiplus::Rect GetHandleRect(int nHandle);

	//���ݾ���ţ�ȡ�þ����Ӧ���
	//��������ƶ����ù����ʱ��ʾ��Ӧ���
	virtual HCURSOR GetHandleCursor(int nHandle);

	//�ƶ���ӦnHandleָ���ľ���ƶ�����pointλ��
	//@param nHandle ���ƶ��ľ�����÷��������nHandle��ı�
	//@param point ���ƶ�����λ�ã�ȫ������
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);

	//����ptλ���뱾����ġ����С�λ�ã������ж������λ��ѡ�ж���Ĳ�λ
	//@param pt ���ڲ��Ե������
	//@return �����С�λ��
	//     - 0 δ����
	//     - 1��GetHandleCount()�����и�����ı��
	//     - GetHandleCount()+1�� δ���о�������ж�������
	virtual int HitTest(const Gdiplus::Point & pt);

	///���Զ����Ƿ��ھ��ο���
	///
	///�ж����ݶ��������Ƿ��ھ�����
	///@param rect ���Ծ��Σ���������
	virtual bool IsInRect(const Gdiplus::Rect &rect);


	//���л�֧��
	virtual void Serialize(CArchive & ar);


	static void SetStorage(IStorage * pStorage);
	static IStorage *GetStorage();
	static Gdiplus::RectF MeasureString( const CStringW & text, Gdiplus::Font * pFont, Gdiplus::PointF origin, Gdiplus::StringFormat *pFormat);
    //���굥λ
	static Gdiplus::Unit m_Unit;
	static IStorage *m_pRootStorage;

private:
	//��������
	CDrawObject * m_pParent;

	//��������
	CString _name;
	//�����ڲ�����
	CString _internalName;
	//////////////////////////////////////////////////////////////////////////

	//�������ԡ����Ƕȵ�λ���ȣ�


	//λ�ã�ȫ�����꣨�����������꣩
	Gdiplus::Point _position;
	//��С
	Gdiplus::Size _size;
	//�Զ�����ת���ģ���������
	Gdiplus::Point _rotateCenter;
	//�Ƿ�������ת����
	bool m_bUseRotateCenter;
	//��ת�Ƕ�
	DOUBLE _angle;
	//////////////////////////////////////////////////////////////////////////

	//��ͼ����

	//���б߿�
	Gdiplus::Rect m_ClipRect;
	//�Ƿ����
	bool m_bUsingClip;
	//�Ƿ���ʾ
	bool _bVisible;

	Appearance _appearance;

	//////////////////////////////////////////////////////////////////////////

	//��������
	bool _isActive;
	//�Ƿ�Ϊѡ��״̬
	bool m_bSelected;
	//////////////////////////////////////////////////////////////////////////
	//ʹ������

	//�Ƿ��ѡ��
	bool _bSelectable;
	//�Ƿ���ƶ�
	bool _bMovable;

	//�Ƿ��ѡת
	bool _bRotatable;
	//�Ƿ�ɸı��С
	bool _bSizable;


    //Gdiplus::Image * _pVisualizedImage;
	//////////////////////////////////////////////////////////////////////////

	static bool m_bIsPrinting;
	static bool _bIsBindBuilding;
	static std::vector<BIND_OBJ_INFO*> _preBindObjects;
	static DRAW_OBJECT_LIST _allObjects;

};
