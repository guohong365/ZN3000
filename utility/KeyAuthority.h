#pragma once
#include "SenseLock/sense4.h"

#define KEY_NO_ERROR 0
#define KEY_NOT_FOUND 1
#define KEY_OPERATION_ERROR 2
#define KEY_CRYPT_ERROR 3
#define KEY_NO_MEMORY 4
#define KEY_NO_SERIAL 5
#define KEY_SYSTEM_OPEN 6
#define KEY_SERIAL_FORMAT 7 
#define KEY_SYSTEM_DES_DECODE 8
#define KEY_ID_FORMAT 9
#define KEY_INVALID_PIN 10
#define KEY_SYSTEM_CHDIR 11
#define KEY_SYSTEM_EXEC_DECODE 12
#define KEY_SYSTEM_EXEC_ENCODE 13
#define KEY_UNKOWN  9999

#define EXEC_ENCODE_FILE_NAME "A000"
#define EXEC_DECODE_FILE_NAME "B000"

#define VENDER_ID 0x88
#define VENDER_ID_STR "88"

#define DEV_PWD_LEN 24
#define USR_PWD_LEN 8
#define LICENSE_LEN 16

#define WATCH_DOG_EVENT_NAME_W L"Local\\SmartSoft.XXX"
#define WATCH_DOG_EVENT_NAME_A "Local\\SmartSoft.XXX"

#define WAIT_RETRY_TIMEOUT 30000

#ifdef UNICODE
#define WATCH_DOG_EVENT_NAME WATCH_DOG_EVENT_NAME_W
#else
#define WATCH_DOG_EVENT_NAME WATCH_DOG_EVENT_NAME_A
#endif

struct WatchDogHanderEvent
{
    HWND hWnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam; 
};
struct WatchDogParameter
{
    WatchDogHanderEvent NotifyHandle;
    WatchDogHanderEvent DisableHandle;
    WatchDogHanderEvent EnableHandle;
};

DWORD CheckPin();
DWORD HardDecrypt(void * pBuffer, size_t size);
DWORD FindKey(SENSE4_CONTEXT *pOutContext);
void SetKeyVender(BYTE vender);
void SetAppId(BYTE appId);
void SetSerialNumber(void * pSerial);
DWORD HardEncrypt(void * pBuffer, size_t size);
DWORD __stdcall KeyWatchDog(LPVOID lpParam);
DWORD CheckKey();
DWORD CheckEnctypt();
DWORD CheckDecrypt();
DWORD CheckAll();
void InitCheckFunctions();
void FreeCheckFunctions();