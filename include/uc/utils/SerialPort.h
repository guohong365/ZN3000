#pragma once

#define READ_INTERVAL_TIMEOUT 0
#define READ_TOTAL_TIMEOUT_MULTIPLIER 0
#define READ_TOTAL_TIMEOUT_CONSTANT 5000
#define WRITE_TOTAL_TIMEOUT_CONSTANT 5000
#define WRITE_TOTAL_TIMEOUT_MULTIPLIER 0
#define COMM_INPUT_BUFFER_SIZE 10240
#define COMM_OUTPUT_BUFFER_SIZE 1024

#include <uc/libuc.h>
#include <string>
#include <tchar.h>

namespace uc
{
    namespace utils
    {

        class LIB_CORE_API SerialPort
        {
        public:
            explicit SerialPort(
                const TSTRING& portName = _T("COM1"),
                unsigned long baudRate = 14400,
                unsigned char dataBits = 8,
                unsigned char parity =0, // NOPARITY,
                unsigned char stopBits =0); //  ONESTOPBIT;

            ~SerialPort();
            bool open();
            unsigned long read(void* lpBytes, unsigned long length);
            unsigned long write(const void* lpBytes, unsigned long length);
            void close();
            void clear() const;
            bool isOpened() const;
        private:
            void* _handle;
            TSTRING _portName;
            unsigned long _baudRate;
            unsigned char _dataBits;
            unsigned char _parity;
            unsigned char _stopBits;	
        };

    }
}
