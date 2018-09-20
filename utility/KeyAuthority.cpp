#include "StdAfx.h"
#include "KeyAuthority.h"
#include "utility.h"
#include "SenseLock/crypt2.h"

#define REG_KEY_PATH "SMARTSoft\\智能警务系列\\蓝色档案"

#define CHECK_FUNCTION_COUNT 5

typedef DWORD (*PCHECK_FUNCTION)(void);

static void * s_pSerialNumber=NULL;
static BYTE s_vender=0;
static BYTE s_appId=0;
static PCHECK_FUNCTION *_pCheckFunctions=NULL;
static ULONG s_heartBeat=0;
ULONG GetHeartBeat()
{
    return s_heartBeat;
}

DWORD ExchangeKey(BYTE * key, size_t size)
{
    return KEY_NO_ERROR;
}

void InitCheckFunctions()
{
    _pCheckFunctions=new PCHECK_FUNCTION[CHECK_FUNCTION_COUNT];
    _pCheckFunctions[0]=CheckPin;
    _pCheckFunctions[1]=CheckKey;
    _pCheckFunctions[2]=CheckEnctypt;
    _pCheckFunctions[3]=CheckDecrypt;
    _pCheckFunctions[4]=CheckAll;
}
void FreeCheckFunctions()
{
    delete _pCheckFunctions;
}
DWORD CheckKey()
{
    s_heartBeat++;
   SENSE4_CONTEXT context={0};
    return FindKey(&context);
}

DWORD CheckEnctypt()
{
    s_heartBeat++;
    char buffer[16];
    for(int i=0; i<16; i++)
    {
        buffer[i]=(char)rand()%255;
    }
    return HardEncrypt(buffer, 16);
}
DWORD CheckDecrypt()
{
    s_heartBeat++;
   char buffer[16];
    for(int i=0; i<16; i++)
    {
        buffer[i]=(char)rand()%255;
    }
    return HardDecrypt(buffer, 16);
}

DWORD CheckAll()
{
    char buffer[16];
    for(int i=0; i<16; i++)
    {
        buffer[i]=(char)rand()%255;
    }
    char out[16];
    memcpy(out, buffer, 16);
    DWORD ret=HardEncrypt(out, 16);
    if(ret!=KEY_NO_ERROR)
    {
        return ret;
    }
    s_heartBeat++;
    if(memcmp(buffer, out, 16)==0)
    {
        return KEY_UNKOWN;
    }
    ret=HardDecrypt(out, 16);
    if(ret!=KEY_NO_ERROR)
    {
        return ret;
    }
    if(memcmp(buffer, out, 16)!=0)
    {
        return KEY_UNKOWN;
    }
    return KEY_NO_ERROR;
}

void SetKeyVender(BYTE vender)
{
    s_vender=vender;
}
void SetAppId(BYTE appId)
{
    s_appId=appId;
}

void SetSerialNumber(void * pSerial)
{
    ASSERT(pSerial);
    s_pSerialNumber=pSerial;
}

DWORD HardDecrypt(void * pBuffer, size_t size)
{
    //ASSERT(size%8==0);
    SENSE4_CONTEXT context;
    DWORD result=FindKey(&context);
    if(result!=KEY_NO_ERROR)
    {
        return result;
    }
    char envName[5]={0};
    if(!Bin2HEXString(context.bID, 2, envName, 5))
    {
        return KEY_NO_SERIAL;
    }
    char *serial=NULL;
	size_t slen=0;
	if(_dupenv_s(&serial,&slen, envName))
	{
		return KEY_NO_SERIAL;
	}

    BYTE pwd[8];
    if(!HEXString2Bin(serial, pwd, 8))
    {
		free(serial);
        return KEY_SERIAL_FORMAT;
    }
    char key[]="guo hong";
    BYTE psn[8];
    if(DES((BYTE*)key, 0, psn, pwd, 8)!=S4_SUCCESS)
    {
		free(serial);
        return KEY_SYSTEM_DES_DECODE;
    }

    S4OPENINFO openInfo={0};
    openInfo.dwS4OpenInfoSize=sizeof(S4OPENINFO);
    openInfo.dwShareMode=S4_SHARE_MODE;
    if(S4OpenEx(&context, &openInfo)!=S4_SUCCESS)
    {
		free(serial);
        return KEY_SYSTEM_OPEN;
    }


    BufferXOR(pwd, psn, context.bID, 8);
    char dir[6];
    dir[0]='\\';
    if(!Bin2HEXString(context.bID, 2, dir+1, 5))
    {
		free(serial);
        S4Close(&context);
        return KEY_ID_FORMAT;
    }

    result=S4ChangeDir(&context, dir);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        S4Close(&context);
        return KEY_SYSTEM_CHDIR;
    }
    result=S4VerifyPin(&context, pwd, 8, S4_USER_PIN);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        S4Close(&context);
        return KEY_NOT_FOUND;
    }
    DWORD len;
    char *pOut=new char[size + 2];
    result=S4Execute(&context, EXEC_DECODE_FILE_NAME, pBuffer, size, pOut, size +2 , &len);
    S4Close(&context);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        delete pOut;
        return KEY_INVALID_PIN;
    }
	free(serial);
    memcpy(pBuffer, pOut, size);
    delete pOut;
    return KEY_NO_ERROR;
}

DWORD HardEncrypt(void * pBuffer, size_t size)
{
    //ASSERT(size%8==0);
    SENSE4_CONTEXT context;
    DWORD result=FindKey(&context);
    if(result!=KEY_NO_ERROR)
    {
        return result;
    }
    char envName[5]={0};
    if(!Bin2HEXString(context.bID, 2, envName, 5))
    {
        return KEY_NO_SERIAL;
    }
    char *serial=NULL;
	size_t slen=0;
	if(_dupenv_s(&serial,&slen,envName))
    {
        return KEY_NO_SERIAL;
    }

    BYTE pwd[8];
    if(!HEXString2Bin(serial, pwd, 8))
    {
		free(serial);
        return KEY_SERIAL_FORMAT;
    }
    char key[]="guo hong";
    BYTE psn[8];
    if(DES((BYTE*)key, 0, psn, pwd, 8)!=S4_SUCCESS)
    {
		free(serial);
        return KEY_SYSTEM_DES_DECODE;
    }

    S4OPENINFO openInfo={0};
    openInfo.dwS4OpenInfoSize=sizeof(S4OPENINFO);
    openInfo.dwShareMode=S4_SHARE_MODE;
    if(S4OpenEx(&context, &openInfo)!=S4_SUCCESS)
    {
		free(serial);
        return KEY_SYSTEM_OPEN;
    }


    BufferXOR(pwd, psn, context.bID, 8);
    char dir[6];
    dir[0]='\\';
    if(!Bin2HEXString(context.bID, 2, dir+1, 5))
    {
		free(serial);
        S4Close(&context);
        return KEY_ID_FORMAT;
    }

    result=S4ChangeDir(&context, dir);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        S4Close(&context);
        return KEY_SYSTEM_CHDIR;
    }
    result=S4VerifyPin(&context, pwd, 8, S4_USER_PIN);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        S4Close(&context);
        return KEY_NOT_FOUND;
    }
    DWORD len;
    char *pOut=new char[size + 2];
    result=S4Execute(&context, EXEC_ENCODE_FILE_NAME, pBuffer, size, pOut, size + 2, &len);
    S4Close(&context);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        delete pOut;
        return KEY_INVALID_PIN;
    }
    memcpy(pBuffer, pOut, size);
    delete pOut;
	free(serial);
    return KEY_NO_ERROR;
}

DWORD FindKey(SENSE4_CONTEXT *pOutContext)
{
    SENSE4_CONTEXT *pContext;
    DWORD size=0;
    DWORD count;
    ASSERT(pOutContext);
    DWORD result=S4Enum(NULL, &size);

    if(S4_INSUFFICIENT_BUFFER==result || S4_SUCCESS==result)
    {
        if(size==0)
        {
            return KEY_NOT_FOUND;
        }

        pContext=(SENSE4_CONTEXT *)malloc(size);
        if(!pContext)
        {
            return KEY_NO_MEMORY;
        }
        count=size/sizeof(SENSE4_CONTEXT);
        if(count==0)
        {
            free(pContext);
            return KEY_UNKOWN;
        }
        result=S4Enum(pContext, &size);
        if(result != S4_SUCCESS)
        {
            free(pContext);
            return KEY_UNKOWN;
        }
        for(DWORD i=0; i< count; i++)
        {
            if(pContext[i].bID[0]==s_vender && pContext[i].bID[1]==s_appId)
            {
                memcpy(pOutContext, pContext + i, sizeof(SENSE4_CONTEXT));
                free(pContext);
                return KEY_NO_ERROR;
            }
        }
        return KEY_NOT_FOUND;
    }
    return KEY_UNKOWN;
}
DWORD CheckPin()
{
    DWORD result;
    SENSE4_CONTEXT context;
    result=FindKey(&context);
    if(result!=KEY_NO_ERROR)
    {
        return result;
    }
    S4OPENINFO openInfo={0};
    openInfo.dwS4OpenInfoSize=sizeof(S4OPENINFO);
    openInfo.dwShareMode=S4_SHARE_MODE;
    if(S4OpenEx(&context, &openInfo)!=S4_SUCCESS)
    {
        return KEY_SYSTEM_OPEN;
    }
    char envName[5]={0};
    Bin2HEXString(context.bID, 2, envName, 5);
    char *serial=NULL;
	size_t len=0;
	if(_dupenv_s(&serial,&len, envName))
    {
        S4Close(&context);
        return KEY_NO_SERIAL;
    }
    char key[]="guo hong";
    BYTE psn[8];
    BYTE pwd[8];
    if(!HEXString2Bin(serial, pwd, 8))
    {
		free(serial);
        S4Close(&context);
        return KEY_SERIAL_FORMAT;
    }
    if(DES((BYTE*)key, 0, psn, pwd, 8)!=S4_SUCCESS)
    {
		free(serial);
        S4Close(&context);
        return KEY_SYSTEM_DES_DECODE;
    }
    BufferXOR(pwd, psn, context.bID, 8);
    char dir[6];
    dir[0]='\\';
    if(!Bin2HEXString(context.bID, 2, dir+1, 5))
    {
		free(serial);
        S4Close(&context);
        return KEY_ID_FORMAT;
    }
    result=S4ChangeDir(&context, dir);
    if(result!=S4_SUCCESS)
    {
		free(serial);
        S4Close(&context);
        return KEY_SYSTEM_CHDIR;
    }
    result=S4VerifyPin(&context, pwd, 8, S4_USER_PIN);
    S4Close(&context);
	free(serial);
    if(result==S4_SUCCESS)
    {
        return KEY_NO_ERROR;
    }
    else
    {
       return KEY_INVALID_PIN;
    }
}


DWORD __stdcall KeyWatchDog(LPVOID lpParam)
{
    ASSERT(lpParam);
    srand(time(NULL));
    WatchDogParameter * param=(WatchDogParameter*)lpParam;
    HANDLE handle=CreateEvent(NULL, TRUE, FALSE, WATCH_DOG_EVENT_NAME);
    if(handle && GetLastError()==ERROR_ALREADY_EXISTS)
    {
        if(WaitForSingleObject(handle, WAIT_RETRY_TIMEOUT)!=WAIT_OBJECT_0)
        {
#ifdef _DEBUG
            TRACE(_T("initial check fialed\n"));
#endif
            _exit(0);
        }
        ResetEvent(handle);
        int max_fialed_count=rand()%10 + 5;
        int fialed_count=0;
        int count=0;
        int max_count;
        int errorflag=0;
        int funcIndex=rand()%5;
        max_count=rand()%3+2;
#ifdef _DEBUG
        TRACE(_T("max retry count=[%d]\n"), max_count);
#endif
        while(true)
        {
            int sleepInterval;
            sleepInterval=rand()%20 + 5;
            if(_pCheckFunctions[funcIndex]()!=KEY_NO_ERROR)
            {
                count ++;
#ifdef _DEBUG
                TRACE(_T("retry count=[%d]\n"), count);
#endif
                if(max_count<=count)
                {
                    switch(errorflag)
                    {
                    case 0:
                        PostMessage(param->NotifyHandle.hWnd, param->NotifyHandle.msg, param->NotifyHandle.wParam, param->NotifyHandle.lParam);
#ifdef _DEBUG
                        TRACE(_T("\npost to Window[0x%X], msg=[0x%x] w=[0x%x] l=[0x%x]\n"), param->NotifyHandle.hWnd, param->NotifyHandle.msg, param->NotifyHandle.wParam, param->NotifyHandle.lParam);
#endif
                        if(WaitForSingleObject(handle, WAIT_RETRY_TIMEOUT)!=WAIT_OBJECT_0)
                        {
#ifdef _DEBUG
                            TRACE(_T("wait time out.\n"));
#endif
                            CloseHandle(handle);
                            _exit(0);
                        }
                        else
                        {
                            ResetEvent(handle);
#ifdef _DEBUG
                            TRACE(_T("reset ok\n"));
#endif
                        }                    
                        errorflag=1;
                        break;
                    case 1:
                        SendMessage(param->DisableHandle.hWnd, param->DisableHandle.msg, param->DisableHandle.wParam, param->DisableHandle.lParam);
                        errorflag=2;
                        break;
                    case 2:
                        Sleep(rand()%2000);
                        fialed_count++;
                        if(fialed_count > max_fialed_count)
                        {
                            abort();
                        }
                        continue;
                    default:
                        break;
                    }
                }
            }
            else
            {
                if(errorflag==2)
                {
                    SendMessage(param->EnableHandle.hWnd, param->EnableHandle.msg, param->EnableHandle.wParam, param->EnableHandle.lParam);
                    errorflag=0;
                }
                count=0;
                fialed_count=0;
           }
#ifdef _DEBUG
            TRACE(_T("sleep [%d]s\n"), sleepInterval);
#endif
            Sleep(sleepInterval * 1000);
        }
    }
    _exit(0);
    return 0;
}