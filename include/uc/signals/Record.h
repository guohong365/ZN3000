#pragma once
#include <uc/libcore.h>
namespace uc {
    namespace signals {        
        class Annotation;
        class SignalSeries;
        
        LIB_CORE_EXTERN template class LIB_CORE_API std::allocator<Annotation*>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::vector<Annotation *>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::_Vector_val<Annotation *, std::allocator<Annotation*>>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::allocator<SignalSeries*>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::vector<SignalSeries *>;
        LIB_CORE_EXTERN template class LIB_CORE_API std::_Vector_val<SignalSeries *, std::allocator<SignalSeries*>>;
        
        class LIB_CORE_API Record
        {
        public:
            virtual ~Record(){}
            virtual const TSTRING& name() const=0;
            virtual void name(const TSTRING& id)=0;
            virtual const TSTRING& description() const=0;
            virtual void description(const TSTRING& description)=0;
            virtual void add(SignalSeries * pSignal)=0;
            virtual void removeAt(size_t index)=0;
            virtual std::vector<SignalSeries *>& signals()=0;
            virtual const std::vector<SignalSeries *> & signals() const=0;
        };
    }
}

