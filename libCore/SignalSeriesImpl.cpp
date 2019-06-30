#include "stdafx.h"
#include "SignalSeriesImpl.h"


namespace uc{
    namespace signals{

        SignalSeriesImpl::SignalSeriesImpl(size_t bufferSize, double samplingFrequency,const TSTRING& dimension, const TSTRING& name)
            : SignalBuffer<double>(bufferSize)
            ,_name(name)
            ,_dimension(dimension)
            , _samplingFrequency(samplingFrequency)
        {
        }
        const TSTRING& SignalSeriesImpl::name() const
        {
            return _name;
        }
        void SignalSeriesImpl::name(const TSTRING& name)
        {
            _name = name;
        }
        const TSTRING& SignalSeriesImpl::dimension()
        {
            return _dimension;
        }
        void SignalSeriesImpl::dimension(const TSTRING& dimension)
        {
            _dimension=dimension;
        }
        SignalSeriesImpl::~SignalSeriesImpl()
        {
            for(std::vector<Annotation*>::iterator it=_annotations.begin(); it!=_annotations.end(); ++it)
            {
                delete *it;
            }
        }
        double SignalSeriesImpl::samplingFrequency() const
        {
            return _samplingFrequency;
        }
        void SignalSeriesImpl::samplingFrequency(double samplingFrequency)
        {
            _samplingFrequency=samplingFrequency;
        }
        size_t SignalSeriesImpl::sampleCount() const
        {
            return length();
        }
        const SignalBuffer<double> & SignalSeriesImpl::signalBuffer() const
        {
            return *this;
        }
        SignalBuffer<double>& SignalSeriesImpl::signalBuffer()
        {
            return *this;
        }
        const std::vector<Annotation*>& SignalSeriesImpl::annotations() const
        {
            return _annotations;
        }
        std::vector<Annotation *>& SignalSeriesImpl::annotations()
        {
            return _annotations;
        }
        void SignalSeriesImpl::add(Annotation* annotation)
        {
            _annotations.push_back(annotation);
        }
        void SignalSeriesImpl::removeAt(size_t index)
        {
            _annotations.erase(_annotations.begin() + index);
        }

    }
}
