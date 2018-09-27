#pragma once

#include "../libDrawObject/Appearance.h"

struct GridBackgroundAppearance : Appearance
{
	bool ShowThickGridLine;
	bool ShowThinGridLine;
	float ThickGridLineWidth;  //0.1mm
	float ThinGridLineWidth;   //0.1mm
	int ThickGridLineStyle;
	int ThinGridLineStyle;
	int ThickGridLineSpacing;   //mm
	int ThinGridLineSpacing;    //mm
	unsigned long ThickGridLineColor;
	unsigned long ThinGridLineColor;

	GridBackgroundAppearance(const GridBackgroundAppearance & appearance);
	GridBackgroundAppearance();
	virtual ~GridBackgroundAppearance();
	virtual Appearance& operator=(const Appearance& another);
	virtual void CopyFrom(const Appearance& another);
	virtual Appearance* Clone();
};

