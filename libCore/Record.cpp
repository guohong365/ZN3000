#include "stdafx.h"
#include <uc/dsp/Record.h>

namespace uc
{
    namespace dsp
    {
        AnnotationImpl::AnnotationImpl(const TSTRING& name, size_t offset)
            :_name(name)
            ,_offset(offset)
        {

        }
        const TSTRING& AnnotationImpl::name() const
        {
            return _name;
        }

        void AnnotationImpl::name(const TSTRING& name)            
        {
            _name = name;
        }
        size_t AnnotationImpl::offset() const
        {
            return _offset;
        }
        void AnnotationImpl::offset(size_t offset)
        {
            _offset = offset;            
        }

        SignalImpl::SignalImpl(size_t bufferSize, double samplingFrequency,const TSTRING& dimension, const TSTRING& name)
            : SignalBuffer<double>(bufferSize)
            ,_name(name)
            ,_dimension(dimension)
            , _samplingFrequency(samplingFrequency)
        {
        }
        const TSTRING& SignalImpl::name() const
        {
            return _name;
        }
        void SignalImpl::name(const TSTRING& name)
        {
            _name = name;
        }
        const TSTRING& SignalImpl::dimension()
        {
            return _dimension;
        }
        void SignalImpl::dimension(const TSTRING& dimension)
        {
            _dimension=dimension;
        }
        SignalImpl::~SignalImpl()
        {
            for(std::vector<Annotation*>::iterator it=_annotations.begin(); it!=_annotations.end(); ++it)
            {
                delete *it;
            }
        }
        double SignalImpl::samplingFrequency() const
        {
            return _samplingFrequency;
        }
        void SignalImpl::samplingFrequency(double samplingFrequency)
        {
            _samplingFrequency=samplingFrequency;
        }
        size_t SignalImpl::sampleCount() const
        {
            return length();
        }
        const SignalBuffer<double> & SignalImpl::signalBuffer() const
        {
            return *this;
        }
        SignalBuffer<double>& SignalImpl::signalBuffer()
        {
            return *this;
        }
        const std::vector<Annotation*>& SignalImpl::annotations() const
        {
            return _annotations;
        }
        std::vector<Annotation *>& SignalImpl::annotations()
        {
            return _annotations;
        }
        void SignalImpl::add(Annotation* annotation)
        {
            _annotations.push_back(annotation);
        }
        void SignalImpl::removeAt(size_t index)
        {
            _annotations.erase(_annotations.begin() + index);
        }

        RecordImpl::RecordImpl(const TSTRING & name, const TSTRING& desc)
            :_name(name), _description(desc)
        {

        }
        RecordImpl::~RecordImpl()
        {
            for(std::vector<Signal*>::iterator it=_signals.begin(); it!=_signals.end(); ++it)
            {
                delete *it;
            }
        }
        const TSTRING& RecordImpl::name() const
        {
            return _name;
        }
        void RecordImpl::name(const TSTRING& name)
        {
            _name=name;
        }
        const TSTRING& RecordImpl::description() const
        {
            return _description;
        }
        void RecordImpl::description(const TSTRING& description)
        {
            _description = description;
        }
        void RecordImpl::add(Signal * pSignal)
        {
            _signals.push_back(pSignal);
        }

        void RecordImpl::removeAt(size_t index)
        {
            _signals.erase(_signals.begin() + index);
        }
        std::vector<Signal *>& RecordImpl::signals()
        {
            return _signals;
        }
        const std::vector<Signal *> & RecordImpl::signals() const
        {
            return _signals;
        }
    }
}