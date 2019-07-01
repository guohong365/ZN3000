#pragma once
namespace uc
{
    namespace signals{
        class RecordWriter;
        class LIB_CORE_API RecordWriterFactory
        {
        public:
            virtual ~RecordWriterFactory(void){};
            virtual RecordWriter * create()=0;
        };
    }
}
