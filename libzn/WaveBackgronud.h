#pragma once

#include <vector>
#include "../libDrawObject/DrawObject.h"
#include "../libDrawObject/LineObject.h"

class WaveBackground:public DrawObject
{
public:
	WaveBackground();
	WaveBackground(const Gdiplus::Point &position, const Gdiplus::Size & size);
	virtual void OnDraw(Gdiplus::Graphics & graph);

	virtual void SetShowThickGrid(bool isShow);

	virtual bool GetShowThickGrid();

	virtual void SetShowThinGrid(bool isShow);

	virtual bool GetShowThinGrid();

	virtual void SetThickGridLineWidth(float width);

	virtual float GetThickGridLineWidth();

	virtual void SetThickGridLineColor(Gdiplus::Color color);

	virtual Gdiplus::Color GetThickGridLineColor();

	virtual void SetThickGridLineStyle(int style);

	virtual int GetThickGridLineStyle();

	virtual void SetThickGridSpacing(int spacing);

	virtual int GetThickGridSpacing();

	virtual void SetThinGridLineWidth(float width);

	virtual float GetThinGridLineWidth();

	virtual void SetThinGridLineColor(Gdiplus::Color color);

	virtual Gdiplus::Color GetThinGridLineColor();

	virtual void SetThinGridLineStyle(int style);

	virtual int GetThinGridLineStyle();

	virtual void SetThinGridSpacing(int spacing);

	virtual int GetThinGridSpacing();

	virtual int GetBaselineCount();

	virtual void AddBaseline(const CString& name, int y, float width, Gdiplus::Color color);

	virtual CLineObject* GetBaseline(int i);

	virtual void SetBaseline(int i, int y);

	virtual void SetBaselineWidth(int i, float width);

	virtual float GetBaselineWidth(int i);

	virtual void SetBaselineColor(int i, Gdiplus::Color color);

	virtual Gdiplus::Color GetBaselineColor(int i);

protected:
	void DrawGrid(Gdiplus::Graphics & graph, Gdiplus::Pen * pPen, 
		const Gdiplus::Point &pos, const Gdiplus::Size & size, int interval);
	
private:
	void initialize();

	virtual void OnDrawBorder( Gdiplus::Graphics & graph );

public:
	virtual DrawObject* CreateInstance();
private:
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

	std::vector<CLineObject*> _baselines;
};



