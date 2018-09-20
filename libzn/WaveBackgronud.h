#pragma once

#include <vector>
#include "../libDrawObject/DrawObject.h"

class WaveBackground:public CDrawObject
{
	DECLARE_DYNCREATE(WaveBackground);
public:
	WaveBackground();
	WaveBackground(const Gdiplus::Point &position, const Gdiplus::Size & size);
	virtual void OnDraw(Gdiplus::Graphics & graph);

	virtual void SetShowThickGrid(bool isShow)
	{
		_isDrawThickGrid=isShow;
	}
	virtual bool GetShowThickGrid()
	{
		return _isDrawThickGrid;
	}
	virtual void SetShowThinGrid(bool isShow)
	{
		_isDrawThinGrid=isShow;
	}
	virtual bool GetShowThinGrid()
	{
		return _isDrawThinGrid;
	}
	virtual void SetThickGridLineWidth(float width)
	{
		_thickGridLineWidth=width;
	}
	virtual float GetThickGridLineWidth()
	{
		return _thickGridLineWidth;
	}
	virtual void SetThickGridLineColor(Gdiplus::Color color)
	{
		_thickGridColor=color;
	}
	virtual Gdiplus::Color GetThickGridLineColor()
	{
		return _thickGridColor;
	}
	virtual void SetThickGridLineStyle(int style)
	{
		_thickGridLineStyle=(Gdiplus::DashStyle)style;
	}
	virtual int GetThickGridLineStyle()
	{
		return _thickGridLineStyle;
	}
	virtual void SetThickGridSpacing(int spacing)
	{
		_thickGridSpacing=spacing;
	}
	virtual int GetThickGridSpacing()
	{
		return _thickGridSpacing;
	}
	virtual void SetThinGridLineWidth(float width)
	{
		_thinGridLineWidth=width;
	}
	virtual float GetThinGridLineWidth()
	{
		return _thinGridLineWidth;
	}
	virtual void SetThinGridLineColor(Gdiplus::Color color)
	{
		_thinGridLineColor=color;
	}
	virtual Gdiplus::Color GetThinGridLineColor()
	{
		return _thinGridLineColor;
	}
	virtual void SetThinGridLineStyle(int style)
	{
		_thinGridLineStyle=(Gdiplus::DashStyle)style;
	}
	virtual int GetThinGridLineStyle()
	{
		return _thinGridLineStyle;
	}
	virtual void SetThinGridSpacing(int spacing)
	{
		_thinGridSpacing=spacing;
	}
	virtual int GetThinGridSpacing()
	{
		return _thinGridSpacing;
	}

	virtual int GetBaselineCount()
	{
		return _baselines.size();
	}
	virtual void AddBaseline(int y, float width, Gdiplus::Color color)
	{
		_baselines.push_back(y);
		_baselineWidths.push_back(width);
		_baselineColors.push_back(color);
	}
	virtual int GetBaseline(int i)
	{
		ASSERT(i>=0);
		ASSERT(i < _baselines.size());
		return _baselines[i];
	}
	virtual void SetBaseline(int i, int y)
	{
		ASSERT(i>=0);
		ASSERT(i < _baselines.size());
		_baselines[i]=y;
	}
	virtual void SetBaselineWidth(int i, float width)
	{
		ASSERT(i>=0 && i< _baselineWidths.size());
		_baselineWidths[i]=width;
	}
	virtual float GetBaselineWidth(int i)
	{
		ASSERT(i>=0 && i< _baselineWidths.size());
		return _baselineWidths[i];
	}
	virtual void SetBaselineColor(int i, Gdiplus::Color color)
	{
		ASSERT(i>=0 && i< _baselineWidths.size());
		_baselineColors[i]=color;
	}
	virtual Gdiplus::Color GetBaselineColor(int i)
	{
		ASSERT(i>=0 && i< _baselineWidths.size());
		return _baselineColors[i];
	}

protected:
	void DrawGrid(Gdiplus::Graphics & graph, Gdiplus::Pen * pPen, 
		const Gdiplus::Point &pos, const Gdiplus::Size & size, int interval);
	
private:
	void initialize();

	virtual void OnDrawBorder( Gdiplus::Graphics & graph );

	bool _isDrawThickGrid;
	bool _isDrawThinGrid;
	float _thickGridLineWidth;  //0.1mm
	float _thinGridLineWidth;   //0.1mm
	Gdiplus::DashStyle _thickGridLineStyle;
	Gdiplus::DashStyle _thinGridLineStyle;
	int _thickGridSpacing;   //mm
	int _thinGridSpacing;    //mm
	Gdiplus::Color _thickGridColor;
	Gdiplus::Color _thinGridLineColor;

	std::vector<float> _baselineWidths;
	std::vector<Gdiplus::Color> _baselineColors;
	std::vector<int> _baselines;

};



