#pragma once
#include "PolyLineObject.h"
#include "PolygonObject.h"
#include "MathUtility.h"

class CRulerLineObject:public CPolyLineObject
{
protected:
	CRulerLineObject(void);
	DECLARE_SERIAL(CRulerLineObject) public:
	virtual ~ CRulerLineObject(void);

	CRulerLineObject(const CString & name, int x, int y);
	CRulerLineObject(const CString & name, const Gdiplus::Point & point);

	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);

};

class C3DCorrectorObject:public CPolygonObject
{
protected:
	C3DCorrectorObject();
	DECLARE_SERIAL(C3DCorrectorObject) public:
	C3DCorrectorObject(int x, int y);
	C3DCorrectorObject(const Gdiplus::Point & pt);
	virtual ~C3DCorrectorObject();
public:
	void DoCalculate();

	double *GetParam();
	void SetOrigion(double x, double y);
	void SetBaselineAngle(double angle);
	virtual bool OnRButtonDown(CWnd * pWnd, UINT nFlags, Gdiplus::Point point);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	//virtual void OnDrawTracker(Gdiplus::Graphics & graph);
private:
	double m_CorrectParem[8];
	double m_OrigionX;
	double m_OrigionY;
	double m_BaselineAngle;
public:
	//梯形校正，矩形高宽比
	static double m_Ratio;

};
