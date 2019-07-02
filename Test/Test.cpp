// Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Test.h"
#include "../libzn/ZNDData.h"
#include <tchar.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            _tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
            nRetCode = 1;
        }
        else
        {
            FILE* fp = nullptr;
            char buffer[4096];
            double time, s1,s2;
            _tfopen_s(&fp, _T("d:/Works/working/ZN3000/data/100.txt"), _T("rt"));
            long index = 0;
            if (fp)
            {
                std::cout << "open ok." << std::endl;

                char* p = fgets(buffer, 4096, fp);
                std::cout << buffer;
                p = fgets(buffer, 4096, fp);
                std::cout << buffer;
                p = fgets(buffer, 4096, fp);
                std::cout << buffer;
                while (p && !feof(fp))
                {
                    if (sscanf_s(buffer, "%15lf\t%15lf\t%15lf", &time, &s1,&s2) == 3)
                    {
                        std::cout << index << " " << time << "," << s1 << "," << s2 << std::endl;
                        index++;
                        p = fgets(buffer, 4096, fp);
                        std::cout << buffer;
                    }
                    else
                    {
                        break;
                    }
                }
                fclose(fp);
            }
        }
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        _tprintf(_T("����: GetModuleHandle ʧ��\n"));
        nRetCode = 1;
    }

    return nRetCode;
}
