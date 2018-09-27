#pragma once
#include "../libDrawObject/Appearance.h"
#include "Settings.h"

enum BaselineAlignment
	{
		MINIMUM_VALUE =0,
		ZERO_VALUE =1,
		MIDDLE_VALUE= 2
	};
	enum DrawMode
	{
		DRAW_STATIC  =0,
		DRAW_ROLLING = 1,
		DRAW_ERASURE = 2
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
	double Velocity;
	WaveDrawerAppearance();
	virtual ~WaveDrawerAppearance();
	virtual Appearance& operator=(const Appearance& another);
	virtual void CopyFrom(const Appearance& another);
	virtual Appearance* Clone();
};

