#pragma once
#include <uc/libdrawing.h>
#include <vector>
#include <uc/signals/visualization/WaveDrawer.h>
#include <uc/signals//visualization/GridBackgronud.h>


#define DEFAULT_WAVE_BACKGROUND_COLOR (RGB(0, 64, 0)|0xFF000000)
#define DEFAULT_WAVE_COLOR            (RGB(0, 255, 0)|0xFF000000)
#define DEFAULT_THICK_GRID_LINE_COLOR (RGB(0, 255, 0)|0xFF000000)
#define DEFAULT_THIN_GRID_LINE_COLOR  (RGB(192,192,192)|0xFF000000)
#define DEFAULT_WAVE_BASELINE_COLOR        (RGB(0, 255, 255)|0xFF000000)

namespace uc {
	namespace signals {
		class SignalSeries;
	}
}

namespace uc
{    
    namespace signals
    {
        namespace visualization
        {

class DRAWING_CLASS WaveCanvas : public DrawObject
{
	void initialize();
public:
	WaveCanvas(const Gdiplus::Point &pt, const Gdiplus::Size & size);
	~WaveCanvas();
	void AddWave(signals::SignalSeries* pChannel, double percent);
	virtual void OnSizeChanged();
	void OnDraw(Gdiplus::Graphics & graph);
	int GetWaveCount() const;
	void SetPadding(int left, int top, int right, int bottom);
	WaveDrawer* GetWave(int i);
	void Clear();
	void SetDrawMode(DrawMode drawMode);

	DrawMode GetDrawMode() const;
	void SetGridAppearance(GridBackgroundAppearance& backgroundAppearance);
	void SetWaveAppearance(WaveDrawerAppearance& waveDrawerAppearance);
protected:
	WaveCanvas();
	void _calcLayout();
	void _drawBackground();
	void _drawLabel(Gdiplus::Graphics &graph,int x, int y, int width, int height, int baseline) const;
	void _drawHorizontalLabel(Gdiplus::Graphics & graph);
public:
	virtual DrawObject* CreateInstance();
private:
	int _paddingLeft;
	int _paddingTop;
	int _paddingRight;
	int _paddingBottom;
	DrawMode _drawMode;
	Gdiplus::Font *_pFont;
	Gdiplus::Bitmap * _pBackground;
	GridBackground * _pWaveBackground;
	std::vector<WaveDrawer *> _waveDrawers;
	int _labelInterval;
	WaveDrawerAppearance _waveAppearance;
};

        }
    }
}
