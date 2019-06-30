#pragma once
#include <uc/signals/Record.h>

namespace uc
{
    namespace signals
    {
        class LIB_UC_API RecordReader
        {
        public:
            virtual ~RecordReader(){};
            virtual Record* read(const TSTRING& recordName) =0;
            virtual Record* read(FILE* fp) =0;
        };
    }
}
