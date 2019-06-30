#include "stdafx.h"
#include "AnnotationImpl.h"

namespace uc
{
    namespace signals
    {
        AnnotationImpl::AnnotationImpl(const TSTRING& name, size_t offset)
            : _name(name)
              , _offset(offset)
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
    }
}
