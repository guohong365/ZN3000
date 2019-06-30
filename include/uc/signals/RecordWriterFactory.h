#pragma once
namespace uc
{
    namespace signals{
        class RecordWriter;
        class RecordWriterFactory
        {
        public:
            virtual ~RecordWriterFactory(void){};
            virtual RecordWriter * create()=0;
        };
    }
}
