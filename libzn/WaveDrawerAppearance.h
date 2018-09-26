#pragma once
#include "../libDrawObject/Appearance.h"
	enum BaselineAlignment
	{
		MINIMUM_VALUE =0,
		ZERO_VALUE =1,
		MIDDLE_VALUE= 2
	};
	enum DrawMode
	{
		DRAW_ROLLING,
		DRAW_ERASURE
	};
struct WaveDrawerAppearance : Appearance
{

	bool ShowBaseline;
	int BaselineAlignment;
	unsigned int BaselineColor;
	float BaselineWidth;  //0.1mm
	int BaselineStyle;
	int WaveDrawMode;
	int EraseWidth;   //0.1mm
	WaveDrawerAppearance();
	virtual ~WaveDrawerAppearance();
	virtual Appearance& operator=(const Appearance& another);
	virtual void CopyFrom(const Appearance& another);
	virtual Appearance* Clone();
};

