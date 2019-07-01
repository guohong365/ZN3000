#pragma once
#include <uc/libuc.h>
#include <map>

namespace uc
{
    namespace signals{
        class RecordReaderFactory;
        class RecordWriterFactory;
        class RecordReader;
        class RecordWriter;

        class LIB_CORE_API FactoryRepository
        {
        public:
            virtual ~FactoryRepository(void){};
            static FactoryRepository& instance();
            virtual void registerReader(const TSTRING& name, RecordReaderFactory* pFactory)=0;
            virtual void registerWriter(const TSTRING& name, RecordWriterFactory* pFactory)=0;
            virtual RecordReader* getReader(const TSTRING& name)=0;
            virtual RecordWriter* getWriter(const TSTRING& name)=0;

        };
    }
}

