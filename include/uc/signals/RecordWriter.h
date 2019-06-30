#pragma once
#include <uc/signals/Record.h>
namespace uc
{
    namespace signals{
        class RecordWriter
        {
        public:
            virtual ~RecordWriter(void);
            virtual void write(Record *pRecord, const TSTRING fileName)=0;
            virtual void write(Record *pRecord, FILE * fp)=0;
        };
    }
}

