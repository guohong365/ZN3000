#pragma once

#include <vector>
#include "SignalBuffer.h"
#include "WaveDrawer.h"
#include "WaveBackgronud.h"
#define DEFAULT_WAVE_BACKGROUND_COLOR (RGB(0, 64, 0)|0xFF000000)
#define DEFAULT_WAVE_COLOR            (RGB(0, 255, 0)|0xFF000000)
#define DEFAULT_THICK_GRID_LINE_COLOR (RGB(0, 255, 0)|0xFF000000)
#define DEFAULT_THIN_GRID_LINE_COLOR  (RGB(192,192,192)|0xFF000000)
#define DEFAULT_WAVE_BASELINE_COLOR        (RGB(0, 255, 255)|0xFF000000)

class WaveCanvas : public CDrawObject
{
	DECLARE_DYNCREATE(WaveCanvas)
	void initialize();
public:
	WaveCanvas(const Gdiplus::Point &pt, const Gdiplus::Size & size);
	~WaveCanvas();
	void AddWave(SignalChannel * pChannel, int percent)
	{
		WaveDrawer * pDrawer=new WaveDrawer(pChannel, Gdiplus::Point(0,0), Gdiplus::Size(0,0));
		pDrawer->SetParent(this);
		_waveDrawers.push_back(pDrawer);
		_layoutRatio.push_back(percent);
		_pWaveBackground->AddBaseline(0, 5, DEFAULT_WAVE_BASELINE_COLOR);
	}
	void OnSizeChanged();
	void OnDraw(Gdiplus::Graphics & graph);
	void PrepareCanvas(int dx, int dy);
	int GetWaveCount() const
	{
		return _waveDrawers.size();
	}
	WaveDrawer * GetWave(int i)
	{
		ASSERT(i>=0 && i< _waveDrawers.size());
		return _waveDrawers[i];
	}
protected:
	WaveCanvas();
	void CalcLayout();
	void DrawBackground();
	void DrawLabel(Gdiplus::Graphics &graph,int x, int y, int width, int height, int baseline);
	void DrawHorizontalLabel(Gdiplus::Graphics & graph);
private:
	Gdiplus::Font *_pFont;
	Gdiplus::Bitmap * _pBackground;
	WaveBackground * _pWaveBackground;
	std::vector<WaveDrawer *> _waveDrawers;
	std::vector<int> _layoutRatio;
	int _labelInterval;
};