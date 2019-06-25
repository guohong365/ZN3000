#pragma once
#include <uc/libuc.h>
#include <uc/dsp/SignalBuffer.h>
#include <vector>
#include <tchar.h>


namespace uc {
    namespace dsp {        
        class Annotation;
        class Signal;

        template class LIB_UC_API std::allocator<Annotation*>;
         template class LIB_UC_API std::vector<Annotation *>;
         template class LIB_UC_API std::_Vector_val<Annotation *, std::allocator<Annotation*>>;
         template class LIB_UC_API std::allocator<Signal*>;
         template class LIB_UC_API std::vector<Signal *>;
         template class LIB_UC_API std::_Vector_val<Signal *, std::allocator<Signal*>>;

         template class LIB_UC_API std::allocator<char>;
         template class LIB_UC_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
         template class LIB_UC_API std::allocator<wchar_t>;
         template class LIB_UC_API std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;

        class LIB_UC_API Annotation
        {
        public:
            virtual ~Annotation(){}
            virtual const TSTRING& name() const =0;
            virtual void name(const TSTRING& name)=0;
            virtual size_t offset() const = 0;
            virtual void offset(size_t offset) =0;
        };
        class LIB_UC_API AnnotationImpl : public Annotation
        {
        public:            
            virtual ~AnnotationImpl(){}
            explicit AnnotationImpl(const TSTRING& name=_T(""), size_t offset=0);
            virtual const TSTRING& name() const =0;
            virtual void name(const TSTRING& name)=0;
            virtual size_t offset() const;
            virtual void offset(size_t offset);
        private:
            TSTRING _name;
            size_t _offset;
        };
        class LIB_UC_API Signal
        {	
        public:
            virtual ~Signal(){}
            virtual const TSTRING& name() const=0;
            virtual void name(const TSTRING& name)=0;            
            virtual const TSTRING& dimension()=0;
            virtual void dimension(const TSTRING & dimension)=0;
            virtual double samplingFrequency() const=0;
            virtual void samplingFrequency(double samplingFrequency)=0;
            virtual size_t sampleCount() const=0;
            virtual const SignalBuffer<double> &signalBuffer() const=0;
            virtual SignalBuffer<double>& signalBuffer()=0;
            virtual const std::vector<Annotation*>& annotations() const=0;
            virtual std::vector<Annotation *>& annotations()=0;
            virtual void add(Annotation* annotation)=0;
            virtual void removeAt(size_t index) = 0;
        };
        class LIB_UC_API SignalImpl : public Signal, public SignalBuffer<double>
        {
        public:
            virtual ~SignalImpl();
            SignalImpl(size_t bufferSize, double samplingFrequency,const TSTRING& dimension=_T(""), const TSTRING& name=_T(""));
            virtual const TSTRING& name() const;
            virtual void name(const TSTRING& name);            
            virtual const TSTRING& dimension();
            virtual void dimension(const TSTRING & dimension);
            virtual double samplingFrequency() const;
            virtual void samplingFrequency(double samplingFrequency);
            virtual size_t sampleCount() const;
            virtual const SignalBuffer<double> &signalBuffer() const;
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

        class LIB_UC_API Record
        {
        public:
            virtual ~Record(){}
            virtual const TSTRING& name() const=0;
            virtual void name(const TSTRING& id)=0;
            virtual const TSTRING& description() const=0;
            virtual void description(const TSTRING& description)=0;
            virtual void add(Signal * pSignal)=0;
            virtual void removeAt(size_t index)=0;
            virtual std::vector<Signal *>& signals()=0;
            virtual const std::vector<Signal *> & signals() const=0;
        };
        class LIB_UC_API RecordImpl : public Record
        {
        public:
            RecordImpl(const TSTRING & name = _T(""), const TSTRING& desc=_T(""));
            virtual ~RecordImpl();
            virtual const TSTRING& name() const;
            virtual void name(const TSTRING& name);
            virtual const TSTRING& description() const;
            virtual void description(const TSTRING& description);
            virtual void add(Signal * pSignal);
            virtual void removeAt(size_t index);
            virtual std::vector<Signal *>& signals();
            virtual const std::vector<Signal *> & signals() const;
        private:
            TSTRING _name;
            TSTRING _description;
            std::vector<Signal*> _signals;            
        };

        class LIB_UC_API RecordReader
        {
        public:
            virtual ~RecordReader(){}
            virtual Record * read(const TSTRING &recordName)=0;
            virtual Record * read(FILE* fp)=0;
        };
    }
}

