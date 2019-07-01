#pragma once

namespace uc{
    namespace signals{
        class LIB_CORE_API Annotation
        {
        public:
            virtual ~Annotation(){}
            virtual const TSTRING& name() const =0;
            virtual void name(const TSTRING& name)=0;
            virtual size_t offset() const = 0;
            virtual void offset(size_t offset) =0;
        };
    }
}
