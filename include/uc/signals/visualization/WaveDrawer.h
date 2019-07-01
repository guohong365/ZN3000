#pragma once
#include <uc/libdrawing.h>
#include <uc/drawing/DrawObject.h>
#include <uc/signals/Record.h>
#include <uc/signals/visualization/WaveDrawerAppearance.h>
#include <uc/signals/SignalSeries.h>
using uc::signals::SignalSeries;
using uc::signals::SignalBuffer;
using uc::drawing::DrawObject;
namespace uc
{    
    namespace signals
    {
        namespace visualization
        {

            class DRAWING_CLASS WaveDrawer:public DrawObject
            {
            public:
                WaveDrawer();
                WaveDrawer(SignalSeries* pChannel, double layoutPercent);
                virtual ~WaveDrawer();
                SignalSeries* getChannelBuffer() const;
                void setChannelBuffer(SignalSeries* pBuffer);
                virtual void SetVelocity(double velocity);
                virtual double GetVelocity();
                virtual void SetBaseline(int pos);
                virtual int GetBaseline();
                virtual void SetWaveHeight(int height);
                virtual int GetWaveHeight();
                virtual DrawObject* CreateInstance();
                double GetLayoutPercent() const;
                void SetLayoutPercent(double layoutPercent);
                BaselineAlignment GetBaselineAlignment();

                void SetBaselineAlignment(BaselineAlignment alignment);

                float GetBaselineWidth();

                void SetBaselineWidth(float width);

                Gdiplus::Color GetBaselineColor();

                void SetBaselineColor(Gdiplus::Color color);

                int GetBaselineStyle();

                void SetBaselineStyle(int style);

                bool IsShowBaseline();

                void SetShowBaseline(bool isShow);

                DrawMode GetWaveDrawMode();

                void SetWaveDrawMode(DrawMode drawMode);

                int GetEraseWidth();

                void SetEraseWidth(int width);
            protected:	
                virtual void OnSizeChanged();
                void _drawBaseline(Gdiplus::Graphics& graph, int startX, int width);
                void _drawErase(Gdiplus::Graphics& graphics, SignalBuffer<double>& buffer, size_t totalSampleCount, double sampleDotSpacing);
                virtual void OnDraw(Gdiplus::Graphics & graph);
                virtual void _drawWaveByPixels(Gdiplus::Graphics& graphics,double* pBuffer, size_t bufferSize,int offsetY, int startX, int endX, size_t startIndex, size_t endIndex);
                virtual void _drawWaveBySamples(Gdiplus::Graphics& graphics,double *pBuffer, size_t bufferSize,int offsetY, int startX, int endX, size_t startIndex, size_t endIndex);
                virtual void _drawFull(Gdiplus::Graphics& graphics, SignalBuffer<double>& buffer, size_t totalSampleCount, double sampleDotSpacing);
            private:
                //basic properties
                //draw componets
                int  _baseline;
                int _waveHeight;
                float _scale;
                double _layoutPercent;
                SignalSeries * _pSignal;
                //SIZE_T _totalSampleCount;
                //float _sampleDotSpacing;
                WaveDrawerAppearance& getThisAppearance();
                Gdiplus::Bitmap * _pCacheBitmap;
                size_t _lastEndSample;
                int _lastEndX;
                void _initialize();
            };

        }
    }
}