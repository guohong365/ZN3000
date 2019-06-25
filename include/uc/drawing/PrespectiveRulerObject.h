#pragma once
#include "PolyLineObject.h"

class CPrespectiveRulerObject: public CPolyLineObject
{
protected:
	CPrespectiveRulerObject();
	DECLARE_SERIAL(CPrespectiveRulerObject) 

public:
	virtual ~CPrespectiveRulerObject();
	CPrespectiveRulerObject(const CString & name, int x, int y);
	CPrespectiveRulerObject(const CString & name, const Gdiplus::Point & pt);

	void SetCoordMapParameter( const double * mapParam, const double * reversMapParam );

	virtual void OnDraw(Gdiplus::Graphics &graph);
private:
	const double * m_pMapParameter;
	const double * m_pReversMapParameter;

};
