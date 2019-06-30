#pragma once
namespace uc
{
    namespace signals{
        class RecordReader;
        class RecordReaderFactory
        {
        public:
            virtual ~RecordReaderFactory(void){};
            virtual RecordReader * create()=0;
        };
    }
}
