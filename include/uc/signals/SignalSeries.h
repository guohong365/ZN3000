#pragma once
#include <uc/libcore.h>
#include <uc/signals/SignalBuffer.h>
#include <uc/signals/Annotation.h>

namespace uc
{
    namespace signals
    {
        class LIB_CORE_API SignalSeries
        {
        public:
            virtual ~SignalSeries(){}
            virtual const TSTRING& name() const =0;
            virtual void name(const TSTRING& name) =0;
            virtual const TSTRING& dimension() =0;
            virtual void dimension(const TSTRING& dimension) =0;
            virtual double samplingFrequency() const =0;
            virtual void samplingFrequency(double samplingFrequency) =0;
            virtual size_t sampleCount() const =0;
            virtual const SignalBuffer<double>& signalBuffer() const =0;
            virtual SignalBuffer<double>& signalBuffer() =0;
            virtual const std::vector<Annotation*>& annotations() const =0;
            virtual std::vector<Annotation *>& annotations() =0;
            virtual void add(Annotation* annotation) =0;
            virtual void removeAt(size_t index) = 0;
        };
    }
}
