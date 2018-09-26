#include "StdAfx.h"
#include "WaveDrawerAppearance.h"

#define DEFAULT_BASELINE_COLOR 0xFF00FFFF
#define DEFAULT_BASELINE_WIDTH 5
#define DEFAULT_BASELINE_SHOW true
#define DEFAULT_BASELINE_ALIGNMENT 1
#define DEFAULT_BASELINE_STYLE 0
#define DEFAULT_WAVE_LINE_WIDTH 5
#define DEFAULT_WAVE_LINE_COLOR 0xFF90EE90
#define DEFAULT_WAVE_DRAW_MODE 0
#define DEFAULT_WAVE_ERASE_WIDTH 40
WaveDrawerAppearance::WaveDrawerAppearance()
	: Appearance()
	, ShowBaseline(DEFAULT_BASELINE_SHOW)
	, BaselineAlignment(DEFAULT_BASELINE_ALIGNMENT)
	, BaselineColor(DEFAULT_BASELINE_COLOR)
	, BaselineWidth(DEFAULT_BASELINE_WIDTH)
	, BaselineStyle(DEFAULT_BASELINE_STYLE)
	, WaveDrawMode(DEFAULT_WAVE_DRAW_MODE)
	, EraseWidth(DEFAULT_WAVE_ERASE_WIDTH)
{
	LineWidth = DEFAULT_WAVE_LINE_WIDTH;
	LineColor = DEFAULT_WAVE_LINE_COLOR;
}

WaveDrawerAppearance::~WaveDrawerAppearance()
{
}

Appearance& WaveDrawerAppearance::operator=(const Appearance& another)
{
	CopyFrom(another);
	return *this;
}

void WaveDrawerAppearance::CopyFrom(const Appearance& another)
{
	Appearance::CopyFrom(another);
	const WaveDrawerAppearance* pDrawerAppearance=dynamic_cast<const WaveDrawerAppearance*>(&another);
	if(!pDrawerAppearance) return;
	BaselineAlignment = pDrawerAppearance->BaselineAlignment;
	BaselineColor = pDrawerAppearance->BaselineColor;
	BaselineWidth=pDrawerAppearance->BaselineWidth;
	ShowBaseline = pDrawerAppearance->ShowBaseline;
}

Appearance* WaveDrawerAppearance::Clone()
{
	WaveDrawerAppearance *pAppearance=new WaveDrawerAppearance();
	pAppearance->CopyFrom(*this);
	return pAppearance;
}
