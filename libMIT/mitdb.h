#pragma once

namespace uc
{
    namespace signals
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
    }
}