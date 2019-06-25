#pragma once
#include <GdiPlus.h>
#include "RectObject.h"
#include "PolyLineObject.h"
#define RULER_OFFSET 8

#define WITH_HORIZ_RULER 0x00000001
#define WITH_VERT_RULER  0x00000002

#define SHOW_HORIZ_RULER_MASK 0x000000010
#define SHOW_VERT_RULER_MASK  0x000000020

#define HORIZ_RULER_HEIGHT 8

//////////////////////////////////////////////////////////////////////////
//���뷽ʽ
#define AlignNone        1      ///�����루���ɶ��룩
#define AlignLeftTop     2      ///����루��ֱ���򣩡��϶��루ˮƽ����
#define AlignRightBottom 3      ///�Ҷ��루��ֱ���򣩡��¶��루ˮƽ����
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//��ߵ�λ
#define UnitNone              -1        ///�޵�λ
#define RulerUnitMillimeter   0 ///����
#define UnitCentiMeter        1 ///����
#define RulerUnitDecimeter    2 ///����
#define RulerUnitMeter        3 ///��
#define RulerUnitKilometre    4 ///ǧ��
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�̶��߱��λ�á�
#define BaselineAtTop      1    ///ˮƽλ��ʱ���̶��߻��ڱ�߻����Ϸ�����ֱλ��ʱ���̶��߻��ڱ�����Ҳ�
#define BaselineAtBottom   2    ///ˮƽλ��ʱ���̶��߻��ڱ�߻����·�����ֱλ��ʱ���̶��߻��ڱ�������
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�̶�������������
#define IncreaseNormally   1    ///��������ˮƽλ�ô����ң���ֱλ�ô��ϵ���
#define IncreaseReverse    2    ///������ˮƽλ�ô��ҵ��󣬴�ֱλ�ô��µ���
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���ֱ�עλ��
#define NumAtLeft      1        //���ֱ�ע�ڿ̶�����ࡣ���Ϊ����ʱ�ڿ̶����Ϸ�
#define NumAtCenter    2        //���ֱ�ע�ڿ̶������·���
#define NumAtRight     3        //���ֱ�ע�ڿ̶����Ҳࡣ���Ϊ����ʱ�ڿ̶����·�
//////////////////////////////////////////////////////////////////////////

class CRulerObject:public CRectObject
{
protected:
	CRulerObject();
	void Initialize();
	DECLARE_SERIAL(CRulerObject) 
public:
	CRulerObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size);
	CRulerObject(const CString & name, const Gdiplus::Rect & rect);
	CRulerObject(const CString & name, int x, int y, int width, int height);

	//ȡӢ�ĵ�λ��
	static const TCHAR *GetUnitEnName(long unit);
	//ȡ���ĵ�λ��
	static const TCHAR *GetUnitZhName(long unit);
	//��Ӣ������ȡ��λ����
	static long GetUnitByEnName(const TCHAR * unitName);
	//����������ȡ��λ����
	static long GetUnitByZhName(const TCHAR * unitName);


	//////////////////////////////////////////////////////////////////////////
	//���β���

	//��ǰ��λ��mm,cm��
	void SetCurrentUnit(long unit);
	long GetCurrentUnit() const;
	virtual void OnCurrentUnitChanging(long &newUnit);
	virtual void OnCurrentUnitCHanged();

	//ÿ���صĳ���
	void SetUnitPerPixel(double unitPerPixel);
	double GetUnitPerPixel() const;
	virtual void OnUnitPerPixelChanging(double &newUnitPerPixel);
	virtual void OnUnitPerPixelChanged();

	//��߶��뷽ʽ
	void SetAlignment(long align);
	long GetAlignment() const;
	virtual void OnAlignmentChanging(long &align);
	virtual void OnAlignmentChanged();

	//////////////////////////////////////////////////////////////////////////
	//�̶��߲���

	//�̶�����С���ؼ��
	void SetMinGraduationPixelInteral(int interval);
	int GetMinGraduationPixelInterval() const;
	virtual void OnMinGraduationPixelIntervalChanging(int &newInterval);
	virtual void OnMinGraduationPixelIntervalChanged();
	//�̶��ߵ�λ���ֱ������ӡ��̶��߱����������������ӱ�����
	void SetGraduationFactor(int factor);
	int GetGraduationFactor() const;
	virtual void OnGraduationFactorChanging(int &newFactor);
	virtual void OnGraduationFactorChanged();
	//�̶�����Ա�߻��ߵ�λ��
	void SetGraduationPosition(long position);
	long GetGraduationPosition() const;
	virtual void OnGraduationPositionChanging(long &newPos);
	virtual void OnGraduationPositionChanged();
	//���̶��߼������
	void SetLongGraduationInterval(int interval);
	int GetLongGraduationInterval() const;
	virtual void OnLongGraduationIntervalChanging(int &interval);
	virtual void OnLongGraduationIntervalChanged();

	//////////////////////////////////////////////////////////////////////////
	//�̶���

	//�Ƿ���ʾ�̶�����
	void SetShowNumber(bool isShowNumber);
	bool GetShowNumber() const;
	virtual void OnShowNumberChanging(bool & isShowNumber);
	virtual void OnShowNumberChanged() ;
	//�̶�������������
	void SetIncreaseDirection(long direction);
	long GetIncreaseDirection() const;
	virtual void OnIncreaseDirectionChanging(long &direction);
	virtual void OnIncreaseDirectionChanged();
	//�̶����ּ���С���ؼ��
	void SetMinNumberPixelInterval(int interval);
	int GetMinNumberPixelInterval() const;
	virtual void MinNumberPixelIntervalChanging(int &interval);
	virtual void MinNumberPixelIntervalChanged();
	//�̶����ֱ��λ�ã���Դ���ǿ̶��ߣ�
	void SetNumberPosition(long position);
	long GetNumberPosition() const;
	virtual void OnNumberPositionChanging(long &pos);
	virtual void OnNumberPositionChanged();
	//�̶����ֱ������ӣ��̶ȱ������Ϊ������������
	void SetNumberFactor(int factor);
	int GetNumberFactor() const;
	virtual void OnNumberFactorChanging(int &factor);
	virtual void OnNumberFactorChanged();

	//////////////////////////////////////////////////////////////////////////
	//��ߵ�λ

	//�Ƿ���ʾ�̶ȵ�λ
	void SetShowUnitName(bool isShowUnitName);
	bool GetShowUnitName() const;
	virtual void OnShowUnitNameChanging(bool & isShowUnitName);
	virtual void OnShowUnitNameChanged();

	//////////////////////////////////////////////////////////////////////////
	//�̶��߼�����Ʋ���

	//�̶��߼������
	double GetGraduationPixelInterval() const;
	//�̶��߼������
	double GetGraduationUnitInterval() const;

	//////////////////////////////////////////////////////////////////////////
	//�̶����ֱ�ǿ��Ʋ���

	//���ֱ�Ǽ������
	int GetNumberPixelInterval() const;
	//���ֱ�Ǽ������
	int GetNumberUnitInterval() const;

	//////////////////////////////////////////////////////////////////////////
	//���ƺ���
	virtual CDrawObject *CopyFrom(CDrawObject * source);

	//////////////////////////////////////////////////////////////////////////
	//��ͼ����
	virtual void OnDraw(Gdiplus::Graphics & graph);

	//���ƻ���
	virtual void DrawBaseLine(Gdiplus::Graphics & graph);
	//���ƿ̶��߼����ֱ��
	virtual void DrawGraduation(Gdiplus::Graphics & graph);
	//���Ƶ�λ����
	virtual void DrawUnitName(Gdiplus::Graphics & graph);

	//      virtual int HitTest(const Gdiplus::Point & pt);

	virtual void Serialize(CArchive & ar);

	virtual void OnSizeChanging(Gdiplus::Size & newSize);
protected:
private:
	//����̶��߼�������ȼ�������ؼ��
	void CalculateGraduatoinStep();

	Gdiplus::Size GetTextExtent(Gdiplus::Graphics & graph, int fontHeight, const CString & text);

	//////////////////////////////////////////////////////////////////////////
	//���ƴ�ֱ��߿̶��߼����ֱ��
	void DrawVertGraduation(Gdiplus::Graphics & graph, int unitInterval, int pixelInterval);
	//����ˮƽ��߿̶��߼����ֱ��
	void DrawHorzGraduation(Gdiplus::Graphics & graph, int unitInterval, int pixelInterval);
	//�������ɱ�߿̶��߼����ֱ��
	void DrawFreeGraduation(Gdiplus::Graphics & graph, int unitInterval, int pixelInterval);

	//////////////////////////////////////////////////////////////////////////
	//���ƴ�ֱ��ߵ�λ����
	void DrawVertUnitName(Gdiplus::Graphics & graph);
	//����ˮƽ��ߵ�λ����
	void DrawHorzUnitName(Gdiplus::Graphics & graph);
	//�������ɱ�ߵ�λ����
	void DrawFreeUnitName(Gdiplus::Graphics & graph);


	//////////////////////////////////////////////////////////////////////////
	//���β���

	//��ǰ��λ��mm,cm��
	long m_CurrentUnit;
	//ÿ���صĳ���
	double m_UnitPerPixel;

	//��߶��뷽ʽ
	long m_Align;

	//////////////////////////////////////////////////////////////////////////
	//�̶��߲���

	//�̶�����С���ؼ��
	int m_MinGraduationPixelInterval;
	//�̶��ߵ�λ���ֱ������ӡ��̶��߱����������������ӱ�����
	int m_GraduationFactor;
	//��߻��ߵ�λ��
	long m_GraduationPosition;
	//���̶��߼������
	int m_LongGraduationInterval;

	//////////////////////////////////////////////////////////////////////////
	//�̶���

	//�Ƿ���ʾ�̶�����
	bool m_bShowNumber;
	//�̶�������������
	long m_IncreaseDirection;
	//�̶����ּ���С���ؼ��
	int m_MinNumberPixelInterval;
	//�̶����ֱ��λ�ã���Դ���ǿ̶��ߣ�
	long m_NumberPosition;
	//�̶����ֱ������ӣ��̶ȱ������Ϊ������������
	int m_NumberFactor;

	//////////////////////////////////////////////////////////////////////////
	//��ߵ�λ

	//�Ƿ���ʾ�̶ȵ�λ
	bool m_bShowUnitName;

	//////////////////////////////////////////////////////////////////////////
	//�̶��߼�����Ʋ���

	//�̶��߼������
	double m_GraduationPixelInterval;
	//�̶��߼������
	double m_GraduationUnitInterval;

	//////////////////////////////////////////////////////////////////////////
	//�̶����ֱ�ǿ��Ʋ���

	//���ֱ�Ǽ������
	int m_NumberPixelInterval;
	//���ֱ�Ǽ������
	int m_NumberUnitInterval;

};

