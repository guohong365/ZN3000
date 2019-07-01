#pragma once
#include <uc/libcore.h>
#include <uc/signals/SignalBuffer.h>
#include <uc/signals/SignalSeries.h>



namespace uc
{
	namespace signals
	{
		LIB_CORE_EXTERN template class LIB_CORE_API std::allocator<Annotation*>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::vector<Annotation *>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::_Vector_val<Annotation *, std::allocator<Annotation*>>;
        
		class LIB_CORE_API SignalSeriesImpl : public SignalSeries, public SignalBuffer<double>
		{
		public:
			virtual ~SignalSeriesImpl();
			SignalSeriesImpl(size_t bufferSize, double samplingFrequency, const TSTRING& dimension = _T(""),
			                 const TSTRING& name = _T(""));
			virtual const TSTRING& name() const;
			virtual void name(const TSTRING& name);
			virtual const TSTRING& dimension();
			virtual void dimension(const TSTRING& dimension);
			virtual double samplingFrequency() const;
			virtual void samplingFrequency(double samplingFrequency);
			virtual size_t sampleCount() const;
			virtual const SignalBuffer<double>& signalBuffer() const;
			virtual SignalBuffer<double>& signalBuffer();
			virtual const std::vector<Annotation*>& annotations() const;
			virtual std::vector<Annotation *>& annotations();
			virtual void add(Annotation* annotation);
			virtual void removeAt(size_t index);
		private:
			TSTRING _name;
			TSTRING _dimension;
			double _samplingFrequency;
			std::vector<Annotation*> _annotations;
		};

	}
}
