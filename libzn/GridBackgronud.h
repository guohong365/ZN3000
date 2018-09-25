#pragma once

#include <vector>
#include "../libDrawObject/DrawObject.h"
#include "../libDrawObject/LineObject.h"
#include "GridBackgroundAppearance.h"

class GridBackground:public DrawObject
{
public:
	GridBackground();
	GridBackground(const Gdiplus::Point &position, const Gdiplus::Size & size);	
	
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

	virtual LineObject* GetBaseline(int i);

	virtual void SetBaseline(int i, int y);

	virtual void SetBaselineWidth(int i, float width);

	virtual float GetBaselineWidth(int i);

	virtual void SetBaselineColor(int i, Gdiplus::Color color);

	virtual Gdiplus::Color GetBaselineColor(int i);

protected:
	static void _drawGrid(Gdiplus::Graphics & graph, Gdiplus::Pen * pPen, 
		const Gdiplus::Point &pos, const Gdiplus::Size & size, int interval);
	virtual void OnDrawBorder( Gdiplus::Graphics & graph );
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics& graph);
	virtual void OnSizeChanged();
private:
	void _initialize();
	GridBackgroundAppearance & _getThisAppearance();

public:
	virtual DrawObject* CreateInstance();
private:
	std::vector<LineObject*> _baselines;
};



