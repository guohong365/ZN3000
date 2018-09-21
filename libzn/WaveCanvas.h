#pragma once

#include <vector>
#include "WaveDrawer.h"
#include "WaveBackgronud.h"
#define DEFAULT_WAVE_BACKGROUND_COLOR (RGB(0, 64, 0)|0xFF000000)
#define DEFAULT_WAVE_COLOR            (RGB(0, 255, 0)|0xFF000000)
#define DEFAULT_THICK_GRID_LINE_COLOR (RGB(0, 255, 0)|0xFF000000)
#define DEFAULT_THIN_GRID_LINE_COLOR  (RGB(192,192,192)|0xFF000000)
#define DEFAULT_WAVE_BASELINE_COLOR        (RGB(0, 255, 255)|0xFF000000)

class WaveCanvas : public DrawObject
{
	void initialize();
public:
	WaveCanvas(const Gdiplus::Point &pt, const Gdiplus::Size & size);
	~WaveCanvas();
	void AddWave(SignalChannel* pChannel, int percent);
	void OnSizeChanged();
	void OnDraw(Gdiplus::Graphics & graph);
	void PrepareCanvas(int dx, int dy);
	int GetWaveCount() const;

	WaveDrawer* GetWave(int i);
protected:
	WaveCanvas();
	void _calcLayout();
	void _drawBackground();
	void _drawLabel(Gdiplus::Graphics &graph,int x, int y, int width, int height, int baseline);
	void _drawHorizontalLabel(Gdiplus::Graphics & graph);
public:
	virtual DrawObject* CreateInstance();
private:
	Gdiplus::Font *_pFont;
	Gdiplus::Bitmap * _pBackground;
	WaveBackground * _pWaveBackground;
	std::vector<WaveDrawer *> _waveDrawers;
	std::vector<int> _layoutRatio;
	int _labelInterval;
};