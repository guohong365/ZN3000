#include "stdafx.h"
#include <uc/signals/SignalSeries.h>
#include "RecordImpl.h"
namespace uc
{
    namespace signals
    {
        RecordImpl::RecordImpl(const TSTRING& name, const TSTRING& desc)
            : _name(name), _description(desc)
        {
        }

        RecordImpl::~RecordImpl()
        {
            for (std::vector<SignalSeries*>::iterator it = _signals.begin(); it != _signals.end(); ++it)
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
            _name = name;
        }

        const TSTRING& RecordImpl::description() const
        {
            return _description;
        }

        void RecordImpl::description(const TSTRING& description)
        {
            _description = description;
        }

        void RecordImpl::add(SignalSeries* pSignal)
        {
            _signals.push_back(pSignal);
        }

        void RecordImpl::removeAt(size_t index)
        {
            _signals.erase(_signals.begin() + index);
        }

        std::vector<SignalSeries *>& RecordImpl::signals()
        {
            return _signals;
        }

        const std::vector<SignalSeries *>& RecordImpl::signals() const
        {
            return _signals;
        }
    }
}
