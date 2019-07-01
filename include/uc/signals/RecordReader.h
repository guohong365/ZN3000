#pragma once
#include <uc/libcore.h>
#include <uc/signals/Record.h>
namespace uc
{
    namespace signals
    {
        class LIB_CORE_CLASS RecordReader
        {
        public:
			RecordReader(){}
            virtual ~RecordReader(){}
            virtual Record* read(const TSTRING& recordName) =0;
            virtual Record* read(FILE* fp) =0;
        };
    }
}
