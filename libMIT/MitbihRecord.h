#pragma once
#include <uc/libuc.h>
#include <uc/signal/Record.h>
namespace uc
{
    namespace signal
    {

        typedef struct DataHdr {
            char hdr[4];
            unsigned int size;
            float sr;
            unsigned char bits;
            unsigned char lead;
            unsigned short umV;
            unsigned short baseLine;
            unsigned char hh;
            unsigned char mm;
            unsigned char ss;
            char reserved[19];
        } DATA_HDR, *PDATA_HDR;

        void LIB_UC_API wfdbSetup(const char *database_path);

        class LIB_UC_API MitdbBinaryReader : public RecordReader
        {
        public:
            virtual Record* read(const TSTRING& recordName);
            virtual Record* read(FILE* fp);
        };

        class LIB_UC_API MitdbTextReader : public RecordReader
        {
        public:
            virtual Record* read(const TSTRING& recordName);
            virtual Record* read(FILE* fp);
        };

    }
}