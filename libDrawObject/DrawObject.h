#pragma once
#include "Visualized.h"
#include "Appearance.h"
#define FONT_SIZE_FACTOR   3.53f

struct FontInfo
{
    UINT mask;
	CString fontName; ///< ��������
	float fontSize;   ///<�����С
	int fontStyle;    ///<�����񣬰���Bold��Italic��Underline
	Gdiplus::Color fontColor; ///<������ɫ
};

class DrawObject: public Visualized
{
	void _initialize();	
protected:
	DrawObject();
public:  
    //Visualized
    virtual Gdiplus::Image* GetPicture(int width, int height, Gdiplus::Color background = Gdiplus::Color::Transparent,
                               DWORD flag = GET_PIC_FLAG_ASPECT | GET_PIC_POS_HCENTER | GET_PIC_POS_VCENTER);

    virtual void ResetContent();

	explicit DrawObject(const CString & name);
	DrawObject(const CString & name, int x, int y, int width, int height);
	DrawObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);
	DrawObject(const CString & name, const Gdiplus::Rect & rect);
	virtual ~DrawObject();

	///����Ψһ�ַ���
	///
	///�������ɶ����ڲ�����
	///
	///@return ����ʱ��Ψһ�Ե��ַ���
	static CString GenerateUniqueName();
	
	//Attributes

	///
	///���ö�������
	///
	///@param name ������
	///
	virtual void SetName(CString name);
	///
	///ȡ��������
	///
	virtual CString  GetName();
	virtual void OnNameChanging(CString & newName);
	virtual void OnNameChanged();

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
	
	//////////////////////////////////////////////////////////////////////////

	//��ͼ����
	virtual void SetAppearance(Appearance & appearance);
	virtual Appearance & GetAppearance();
	virtual Appearance & GetAppearance() const;
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
	virtual void SetTextIsVertical(bool isVertical);
	virtual bool GetTextIsVertical();
	virtual void OnTextVerticalChanging(bool & isVertical);
	virtual void OnTextVerticalChanged();

	//////////////////////////////////////////////////////////////////////////

	//��������

	//���󼤻�״̬
	virtual void SetActive(bool isActive);
	virtual bool GetActive();
	virtual void OnActiveChanging(bool &isActive);
	virtual void OnActiveChanged();

	//////////////////////////////////////////////////////////////////////////

	//���󸸶���
	virtual void SetParent(DrawObject * pParent);
	virtual const DrawObject *GetParent() const;
	virtual DrawObject *GetParent();
	virtual void OnParentChanging(DrawObject * & pObject);
	virtual void OnParentChanged();

	//method
	///
	///��Ϣ֪ͨ
	virtual void Notify(UINT msgId, DWORD_PTR wParam = 0, LPVOID lpParam = nullptr);
	///֪ͨ��Ӧ
	virtual void OnNotify(DrawObject * pSource, UINT messageId, DWORD_PTR wParam, LPVOID lpParam);

	//////////////////////////////////////////////////////////////////////////
	//�����б�֧��
	//////////////////////////////////////////////////////////////////////////
	//���ƶ���
	//@param source ����source����ǰ���ݵ������󣬲����Ƴ����б�
	virtual DrawObject *CopyFrom(DrawObject * source);

	virtual DrawObject * CreateInstance() = 0;
	//������ȸ��Ƹ���
	//�����µĶ���ռ䣬���Ƶ�ǰ���ݵ��¶���
	virtual DrawObject *Clone();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//��ͼ֧��
	//////////////////////////////////////////////////////////////////////////

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
	virtual void MoveTo(const Gdiplus::Point& point);
	virtual void MoveTo(int x, int y);

	//ƫ��
	//@param x x����ƫ��
	//@param y y����ƫ��
	virtual void Offset(int x, int y);
	virtual void Offset(const Gdiplus::Point& point);

	//����
	//���ڱ�������ԭ������
	//@param scaleX  x�������ű���
	//@param scaleY  y�������ű���
	virtual void Scale(double scaleX, double scaleY);

	//���ڸ������Ż�������
	virtual void ScaleAt(double scaleX, double scaleY,const Gdiplus::Point& origin);
	
	///���Զ����Ƿ��ھ��ο���
	///
	///�ж����ݶ��������Ƿ��ھ�����
	///@param rect ���Ծ��Σ���������
	virtual bool IsInRect(const Gdiplus::Rect &rect);

	static Gdiplus::RectF MeasureString( const CStringW & text, Gdiplus::Font * pFont,const Gdiplus::PointF& origin, Gdiplus::StringFormat *pFormat);
    //���굥λ
	static Gdiplus::Unit Unit;

private:
	//��������
	DrawObject * _pParent;

	//��������
	CString _name;
	//////////////////////////////////////////////////////////////////////////

	//�������ԡ����Ƕȵ�λ���ȣ�


	//λ�ã�ȫ�����꣨�����������꣩
	Gdiplus::Point _position;
	//��С
	Gdiplus::Size _size;

	//��ͼ����

	//���б߿�
	Gdiplus::Rect _clipRect;
	//�Ƿ����
	bool _bUsingClip;
	//�Ƿ���ʾ
	bool _bVisible;

	Appearance * _pAppearance;


	//////////////////////////////////////////////////////////////////////////

	//��������
	bool _isActive;
	//�Ƿ�Ϊѡ��״̬
	bool _bSelected;
};
