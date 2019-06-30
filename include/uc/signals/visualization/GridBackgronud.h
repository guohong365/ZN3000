#pragma once
#include <uc/drawing/DrawObject.h>
#include <uc/dsp/visualization/GridBackgroundAppearance.h>

namespace uc
{    
    namespace dsp
    {
        namespace visualization
        {

            class LIB_UC_API GridBackground:public drawing::DrawObject
            {
            public:
                GridBackground();
                GridBackground(const Gdiplus::Point &position, const Gdiplus::Size & size);	
                virtual ~GridBackground();
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
            protected:
                static void _drawGrid(Gdiplus::Graphics & graph, Gdiplus::Pen * pPen, 
                    const Gdiplus::Point &pos, const Gdiplus::Size & size, int interval);
                virtual void OnDrawBorder( Gdiplus::Graphics & graph );
                virtual void OnDraw(Gdiplus::Graphics & graph);
                virtual void OnDrawFillObject(Gdiplus::Graphics& graph);
            private:
                void _initialize();
                GridBackgroundAppearance & _getThisAppearance();

            public:
                virtual DrawObject* CreateInstance();
            };

        }
    }
}

