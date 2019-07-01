#pragma once
#include <uc/signals/Record.h>

namespace uc
{
    namespace signals
    {
        class LIB_CORE_API RecordImpl : public Record
        {
        public:
            RecordImpl(const TSTRING& name = _T(""), const TSTRING& desc = _T(""));
            virtual ~RecordImpl();
            virtual const TSTRING& name() const;
            virtual void name(const TSTRING& name);
            virtual const TSTRING& description() const;
            virtual void description(const TSTRING& description);
            virtual void add(SignalSeries* pSignal);
            virtual void removeAt(size_t index);
            virtual std::vector<SignalSeries *>& signals();
            virtual const std::vector<SignalSeries *>& signals() const;
        private:
            TSTRING _name;
            TSTRING _description;
            std::vector<SignalSeries*> _signals;
        };
    }
}
