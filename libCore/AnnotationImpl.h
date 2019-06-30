#pragma once
#include <uc/signals/Annotation.h>
namespace uc
{
    namespace signals{

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
}
}